#include <WiFi.h>

#define BUF_SIZE 128
#define END_MSG 'E'

uint port = 8888;
WiFiServer server(port);
WiFiClient host;

const char* ssid = "A.FamilyWifi";
const char* password = "fifty3489cactus";

// list of states, the number given to a state correlates by index to a meaning in this array
const String states[4] = {"BOOT", "WIFI_CONNECT", "TCP_ENABLED", "OPERATIONAL"};
// list of possible commands during OPERATIONAL state, should compare input to commands when deciding response
const String cmds[4] = {"set_wifi", "on", "off", "reboot"};

// function prototypes
int init_tcp_connect();

// glolbal variables
int   state = 0;
int   buf_cur = 0;
int   iter;
char  rx_buf[BUF_SIZE];
char  rx_msg[BUF_SIZE];
char  in_byte;
long  rx_time;


void setup() {
  // connect to wifi network
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting to Wifi network: ");
    Serial.println(ssid);
  }

  state++;
  Serial.println("Success!");
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());

  // set up a server for tcp communication

  server.begin();
  Serial.println("Server set up.");
  Serial.print("IP after server init:");
  Serial.println(WiFi.localIP());
}

void loop() {

  while (state < 2) {
    Serial.println("Waiting for TCP connection...");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    if (server.hasClient()) {
      if (host.connected()) {
        server.available().stop();
      }
      else {
        host = server.available();
        state = 2;
      }
    }
    delay(200);
  }
  Serial.println("Client found!");


  // listen for msg from client while a connection is maintained
  /////// EDIT THIS WHILE LOOP FOR TESTING
  while (host.connected()) {
    // first get string msg
    recv_msg();
    strcpy(rx_msg, rx_buf);

    // get time stamp
    recv_msg();
    rx_time = atoi(rx_buf);

    Serial.print("msg: ");
    Serial.println(rx_msg);
    Serial.print("time: ");
    Serial.println(rx_time);
    delay(1);
  }

  Serial.print("UNDEFINED: ");
  Serial.println(state);

  host.stop();
  state = 1;
}


//////  IN PROGRESS
/*
   DESC: Function calculates the approximate time difference between the ESP and
   the host machine. This is doen by receiveing 10 timestamps from the host, calculating
   t1 for each where t1 = currTime - timestamp, and maintaining the minimal t1
   The same is then repeated in the other direction by sending 10 timestamps to the
   host, and receiving t2 from th host. Finally timeDiff = t2 - t1/2
   RETURN: retuns the interger time difference in milliseconds
*/
int getTimeDifference() {
  int i = 0;
  int curTime;
  int timestamp;
  int t1 = INT_MAX;
  int t2 = INT_MAX;
  int temp;
  buf_cur = 0;

  while (host.connected()) {
    if (host.available()) {
      in_byte = host.read();

      if (in_byte != 'E')
        rx_buf[buf_cur++] = in_byte;
      else {
        rx_buf[buf_cur++] = '\0';
        timestamp = atoi(rx_buf);
        // calculate t1, compare to previous value
      }

      if (buf_cur >= BUF_SIZE)
        buf_cur = 0;
    }
    if (i >= 10)
      break;
  }
  return 0;
}

//////// IN TESTING
/*
   DESC: Function abstracts reciving msgs from the the client. Will populate global rx_buf
   This function terminates and returns when rx_buf is overflowed or found the 'E' terminating
*/
void recv_msg() {
  char in_byte;
  buf_cur = 0;
  // check that host is connected
  while (host.connected()) {
    // check that a new msg is available
    if (host.available()) {
      // check buffer overflow
      if (buf_cur >= BUF_SIZE) {
        Serial.println("RX BUFFER OVERFLOW");
        break;
      }
      // read one byte at a time
      in_byte = host.read();
      // if terminated add the null terminator to the buf and return
      if (in_byte == END_MSG) {
        rx_buf[buf_cur++] = '\0';
        break;
      }
      // else add to the buffer
      else
        rx_buf[buf_cur++] = in_byte;
    }
    // delay before next check to avoid crashes
    else
      delay(1);
  }
  return;
}
