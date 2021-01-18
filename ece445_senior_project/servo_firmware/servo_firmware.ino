#include <WiFi.h>
#include <SPI.h>

#define BUF_SIZE 128
#define END_MSG     'E'
#define CMD_MSG     'C'
#define TIME_MSG    'T'
#define JITTER_MSG  'J'
#define TURN_OFF    '0'

// lcd defines
//////// PINS FOR ESP32S2 to LCD
/*
   Pin 4 => SCK
   Pin 5 => SDO
   Pin 6 => SDI
   Pin 7 => CS
*/
#define VSPI FSPI   // unclear what this is.....
#define VSPI_MISO   5
#define VSPI_MOSI   6
#define VSPI_SCLK   4
#define VSPI_SS     7
#define SCR_WIDTH   16
#define LCD_SIZE    32

// variables for servo board
// frequency 4000 = 20ms
#define servo_pin     2
#define frequency     4000
#define pwm_channel   0
#define resolution    8

// constant ratio for converting analog input to voltage: 278 per 1V
#define BATTERY_PIN 1
#define VOLTAGE_CONVERT 278.0

double analog2voltage(int input);
void init_lcd();
void clear_screen();
void print_lcd(const char* msg, bool clear_buf = true);
void setBacklight(uint8_t R, uint8_t G, uint8_t B);
void run_motor();

// data for lcd using SPI
SPIClass * vspi = NULL;
char cur_color = '0';

// data for TCP communication
uint port = 8888;
WiFiServer server(port);
WiFiClient host;

// place SSID and password for WiFi here
const char* ssid = "BenPhone";
const char* password = "rocko2323";

// data for jitter buffer
int jitter_buf = 200;
unsigned long start_time;

int   buf_cur = 0;
// list of states, the number given to a state correlates by index to a meaning in this array
const String states[4] = {"BOOT", "WIFI_CONNECT", "TCP_ENABLED", "OPERATIONAL"};

// function prototypes
int init_tcp_connect();
char recv_msg();
String ip2string(IPAddress ip);
void check_battery();

// glolbal variables
int   state = 0;
int   iter;
char  rx_buf[BUF_SIZE];
char  rx_msg[BUF_SIZE];
char  in_byte;
long  rx_time;
int duty_cycle = 0;
bool motor_state = false;

void setup() {
  // connect to wifi network
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // initiate lcd spi interface
  init_lcd();

  print_lcd("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print("Connecting to Wifi network: ");
    //Serial.println(ssid);
  }

  state++;
  Serial.println("Success!");
  print_lcd("WiFi Connected!");

  // set up a server for tcp communication

  server.begin();
  Serial.println("Server set up.");

  // pin modes and set up for relay
  ledcSetup(pwm_channel, frequency, resolution);
  ledcAttachPin(servo_pin, pwm_channel);

}

void loop() {
  bool printIP = true;
  // while loop waits for TCP connection, exits loop when connected
  while (state < 2) {
    check_battery();
    if (printIP) {
      String ip_str = ip2string(WiFi.localIP());
      print_lcd("IP: ");
      print_lcd(ip_str.c_str(), false);
      print_lcd("   ", false);
      print_lcd("PORT: ", false);
      print_lcd(((String)port).c_str(), false);
      Serial.println("Waiting for TCP connection...");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      printIP = false;
    }

    if (server.hasClient()) {
      if (host.connected()) {
        server.available().stop();
      }
      else {
        host = server.available();
        start_time = millis();
        state = 2;
      }
    }
    delay(1);
  }
  Serial.println("Client found!");
  print_lcd("TCP Connected!");

  // listen for msg from client while a connection is maintained
  /////// EDIT THIS WHILE LOOP FOR TESTING

  print_lcd("Listening for commands...");
  while (host.connected()) {
    check_battery();

    // first get string msg
    char msg_type = recv_msg();

    // reset the current timer
    if (msg_type == JITTER_MSG) {
      start_time = millis();
      jitter_buf = atoi(rx_buf);
    }

    if (msg_type == CMD_MSG) {
      strcpy(rx_msg, rx_buf);

      // recieve timestamp that follows all commands
      if (recv_msg() == TIME_MSG) {
        rx_time = atoi(rx_buf);
        int latency = get_time() - rx_time;

        // if the latency is smaller than the buffer, delay execution
        if (jitter_buf > latency) {
          delay(jitter_buf - latency);
          Serial.print("msg: ");
          Serial.println(rx_msg);
          Serial.print("latency: ");
          Serial.println(latency);
          Serial.print("jitter: ");
          Serial.println(get_time() - rx_time);
        }
      }

      // turn on or off relays by first char, relay number from second char
      if (rx_msg[0] == '1') {
        motor_state = true;

        while (motor_state) {
          print_lcd("servo ");
          print_lcd(" on", false);
          run_motor();
          char msg = recv_msg();

          if (rx_buf[0] == TURN_OFF) {
            motor_state = false;
          }
        }

        digitalWrite(servo_pin, LOW);
        print_lcd("servo ");
        print_lcd(" off", false);
      }
    }

    // get time stamp
    else if (msg_type == TIME_MSG) {
      rx_time = atoi(rx_buf);
      Serial.print("recieved time msg: ");
      Serial.println(rx_time);
    }

    delay(1);
  }

  Serial.print("UNDEFINED: ");
  Serial.println(states[state]);

  host.stop();
  state = 1;
}

/*
   DESC: Function abstracts reciving msgs from the the client. Will populate global rx_buf
   This function terminates and returns when rx_buf is overflowed or found the 'E' terminating
*/
char recv_msg() {
  char in_byte = 'x';
  char msg_type = '\0';
  buf_cur = 0;

  // check that a new msg is available
  if (host.available()) {
    // repeat read until message terminated
    while (in_byte != END_MSG) {
      // check buffer overflow
      if (buf_cur >= BUF_SIZE) {
        Serial.println("RX BUFFER OVERFLOW");
        buf_cur = 0;
      }
      // read one byte at a time
      in_byte = host.read();

      if (in_byte == TIME_MSG || in_byte == CMD_MSG || in_byte == JITTER_MSG) {
        msg_type = in_byte;
      }

      // if terminated add the null terminator to the buf and return
      else if (in_byte == END_MSG) {
        rx_buf[buf_cur++] = '\0';
        break;
      }
      // else add to the buffer
      else
        rx_buf[buf_cur++] = in_byte;
    }
  }
  // delay before next check to avoid crashes
  else
    delay(1);

  return msg_type;
}



// function initialies the SPI object to transmit to LCD
// Uses global vspi pointer
void init_lcd() {
  vspi = new SPIClass(VSPI);
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);
  pinMode(VSPI_SS, OUTPUT); // set Slave Select as output
  digitalWrite(VSPI_SS, LOW); //Drive the CS pin low to select OpenLCD
  vspi->transfer(0x04); // set width to 16
  vspi->transfer(0x06); // set lines to 2
  digitalWrite(VSPI_SS, HIGH); //Release the CS pin to de-select OpenLCD
  vspi->endTransaction();
}


void clear_screen() {
  digitalWrite(VSPI_SS, LOW); //Drive the CS pin low to select OpenLCD
  // need to let LCD move to setting mode before clearing screen, hence the delay
  vspi->transfer('|'); //Put LCD into setting mode
  delay(5);
  vspi->transfer('-'); //Send clear display command
  delay(5);
  digitalWrite(VSPI_SS, HIGH); //Release the CS pin to de-select OpenLCD
  //vspi->endTransaction();
}


// print all string to screen, by defualt clears existing screen
void print_lcd(const char* msg, bool clear_buf) {
  int i;
  int len;

  // clear the screen and reset the length of the buffer
  if (clear_buf) {
    clear_screen();
  }

  for (i = 0; i < LCD_SIZE; i++) {
    if (msg[i] == '\0')
      break;
  }
  len = i;

  //Drive the CS pin low to select OpenLCD
  digitalWrite(VSPI_SS, LOW);
  // display buffer
  for (i = 0; i < len; i++) {
    vspi->transfer(msg[i]);
    delay(1);
  }
  digitalWrite(VSPI_SS, HIGH); //Release the CS pin to de-select OpenLCD
  //vspi->endTransaction();
}

void setBacklight(uint8_t R, uint8_t G, uint8_t B) {

  // enable CS
  digitalWrite(VSPI_SS, LOW); //Drive the CS pin low to select OpenLCD

  // command tells LCD to expect RGB
  vspi->transfer('|');
  delay(1);

  vspi->transfer('+');
  delay(1);

  // send RGB
  vspi->transfer(R);
  delay(1);
  vspi->transfer(G);
  delay(1);
  vspi->transfer(B);
  delay(1);

  // end communication
  digitalWrite(VSPI_SS, HIGH); //Release the CS pin to de-select OpenLCD
  vspi->endTransaction();
  return;
}

String ip2string(IPAddress ip) {
  String ip_str = "";
  for (int i = 0; i < 4; i++) {
    ip_str = ip_str + (String)ip[i];
    if (i != 3)
      ip_str = ip_str + ".";
  }
  return ip_str;
}

unsigned long get_time() {
  return millis() - start_time;
}

void run_motor() {
  for (duty_cycle = 0; duty_cycle <= 52; duty_cycle++) {
    ledcWrite(pwm_channel, duty_cycle);
    delay(15);
  }
  for (duty_cycle = 52; duty_cycle >= 0; duty_cycle--) {
    ledcWrite(pwm_channel, duty_cycle);
    delay(15);
  }
}

// converts analog read input from battery sense circuit to approximate voltage
double analog2voltage(int input) {
  return input / VOLTAGE_CONVERT;
}


double voltage = 0;
void check_battery() {
  // read battery
  voltage = 0;
  for (int i = 0; i < 10; i++) {
    voltage += analog2voltage(analogRead(BATTERY_PIN));
  }
  voltage = voltage / 10;
  Serial.print("voltage: ");
  Serial.println(voltage);
  if (voltage >= 5.4 && cur_color != 'G') {
    setBacklight(0, 255, 0);
    delay(1);
    cur_color = 'G';
  }

  else if (voltage < 5.0 && cur_color != 'R') {
    setBacklight(255, 0, 0);
    delay(1);
    cur_color = 'R';
  }
  return;
}
