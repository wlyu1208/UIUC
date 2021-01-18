#include <WiFi.h>

#define BUF_SIZE 256
#define END_MSG 'E'
#define CMD_MSG 'C'
#define TIME_MSG 'T'
#define JITTER_MSG 'J'

uint port = 8888;
WiFiServer server(port);
WiFiClient host;

const char* ssid = "RockoNet";
const char* password = "Sasroc0882!";

// list of states, the number given to a state correlates by index to a meaning in this array
const String states[5] = {"BOOT", "WIFI_CONNECT",  "JITTER_SETUP", "TCP_ENABLED","OPERATIONAL"};
// list of possible commands during OPERATIONAL state, should compare input to commands when deciding response
const String cmds[4] = {"set_wifi", "on", "off", "reboot"};

// function prototypes
int init_tcp_connect();
char recv_msg();
void send_time();

// glolbal variables
int   state = 0;
int   buf_cur = 0;
int   time_difference;
int   latency;

// set jitter buffer to 200 ms
int jitter_buf = 200;

char  rx_buf[BUF_SIZE];
char  rx_msg[BUF_SIZE];
char  in_byte;
long  rx_time;

unsigned long startTime;

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
  bool printIP = true;
  while (state < 2) {
    if(printIP){
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
        startTime = millis();
        state = 2;
      }
    }
    delay(1);
  }
  Serial.println("Client found!");
  
  // listen for msg from client while a connection is maintained
  /////// EDIT THIS WHILE LOOP FOR TESTING
  while (host.connected()) {

    char msg_type = recv_msg();

    // reset the current timer
    if(msg_type == JITTER_MSG){
      startTime = millis();
      }
    
    if (msg_type == CMD_MSG){
      strcpy(rx_msg, rx_buf);
    } 
    
    else if(msg_type == TIME_MSG){
      rx_time = atoi(rx_buf);

      int latency = millis() - startTime - rx_time;
      if (jitter_buf > latency)
        delay(jitter_buf - latency);
        
      Serial.print("msg: ");
      Serial.println(rx_msg);
      Serial.print("latency: ");
      Serial.println(latency);
      Serial.print("jitter: ");
      Serial.println(millis() - startTime - rx_time);
    }
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
   host, and receiving t2 f now install an extension without synchronizing it while settings syncrom th host. Finally timeDiff = t2 - t1/2
   RETURN: retuns the interger time difference in milliseconds
*/
int getTimeDifference(unsigned int num_msgs) {
  int i = 0;
  int curTime;
  int timestamp;
  int t1_arr[num_msgs];
  int t1 = INT_MAX;
  int t2 = INT_MAX;
  int temp;
  char msg_type;
  buf_cur = 0;

  // first get 10 time msgs from the host to populate the t1 array\
  // edit this to use the recv_msg function
  while (host.connected() && i < num_msgs) {

    // get time msg and calculate t1 = time on esp - time on host when sent = travel time  - time difference
    if (recv_msg() == TIME_MSG){
        timestamp = atoi(rx_buf);
        t1_arr[i++] = millis() - timestamp;
      }
  }

  // now find the minimum of the t1_arr and save as t1
  for(i = 0; i < num_msgs;i++)
    if (t1 > t1_arr[i])
      t1 = t1_arr[i];
  
  // now send time back to the host for the host to figure t2
  for(i = 0; i < num_msgs; i++)
    send_time();

  // recieve the t2 from host
  if (msg_type = recv_msg() == JITTER_MSG)
    t2 = atoi(rx_buf);
  else{
    Serial.println("ERROR decoding msg, not time msg.");
    Serial.print("MSG type: ");
    Serial.println(msg_type);
    }
 
  // now calculate the time difference
  // print for debugging
  Serial.print("t1 = ");
  Serial.println(t1);
  Serial.print("t2 = ");
  Serial.println(t2);
  Serial.print("time difference = ");
  Serial.println((t2-t1)/2);
  
  return (t2 - t1)/2;
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
    while(in_byte != END_MSG){
      // check buffer overflow
      if (buf_cur >= BUF_SIZE) {
        Serial.println("RX BUFFER OVERFLOW");
        buf_cur = 0;
      }
      // read one byte at a time
      in_byte = host.read();
      
      if (in_byte == TIME_MSG || in_byte == CMD_MSG || in_byte == JITTER_MSG){
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


void send_time(){
  unsigned long cur_time = millis();
  //Serial.print("size of cur_time = ");
  //Serial.println(sizeof(cur_time));
  char tx_buf[sizeof(cur_time)];
  memcpy(tx_buf, &cur_time,sizeof(cur_time));
  
  if(host.connected()){
    host.write(tx_buf, sizeof(cur_time));
    //Serial.print("sent host time: ");
    //Serial.println(cur_time);
  }
  return;
}

unsigned long getTime(){
  return millis() - startTime;
  }
