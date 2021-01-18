/*
 OpenLCD is an LCD with Serial/I2C/SPI interfaces.
 By: Nathan Seidle
 SparkFun Electronics
 Date: April 19th, 2015
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This is example code that shows how to send data over SPI to the display.

 To get this code to work, attached an OpenLCD to an Arduino Uno using the following pins:
 CS (OpenLCD) to 10 (Arduino)
 SDI to 11
 SDO to 12 (optional)
 SCK to 13
 VIN to 5V
 GND to GND

 Command cheat sheet:
 ASCII / DEC / HEX
 '|'    / 124 / 0x7C - Put into setting mode
 Ctrl+c / 3 / 0x03 - Change width to 20
 Ctrl+d / 4 / 0x04 - Change width to 16
 Ctrl+e / 5 / 0x05 - Change lines to 4
 Ctrl+f / 6 / 0x06 - Change lines to 2
 Ctrl+g / 7 / 0x07 - Change lines to 1
 Ctrl+h / 8 / 0x08 - Software reset of the system
 Ctrl+i / 9 / 0x09 - Enable/disable splash screen
 Ctrl+j / 10 / 0x0A - Save currently displayed text as splash
 Ctrl+k / 11 / 0x0B - Change baud to 2400bps
 Ctrl+l / 12 / 0x0C - Change baud to 4800bps
 Ctrl+m / 13 / 0x0D - Change baud to 9600bps
 Ctrl+n / 14 / 0x0E - Change baud to 14400bps
 Ctrl+o / 15 / 0x0F - Change baud to 19200bps
 Ctrl+p / 16 / 0x10 - Change baud to 38400bps
 Ctrl+q / 17 / 0x11 - Change baud to 57600bps
 Ctrl+r / 18 / 0x12 - Change baud to 115200bps
 Ctrl+s / 19 / 0x13 - Change baud to 230400bps
 Ctrl+t / 20 / 0x14 - Change baud to 460800bps
 Ctrl+u / 21 / 0x15 - Change baud to 921600bps
 Ctrl+v / 22 / 0x16 - Change baud to 1000000bps
 Ctrl+w / 23 / 0x17 - Change baud to 1200bps
 Ctrl+x / 24 / 0x18 - Change the contrast. Follow Ctrl+x with number 0 to 255. 120 is default.
 Ctrl+y / 25 / 0x19 - Change the TWI address. Follow Ctrl+x with number 0 to 255. 114 (0x72) is default.
 Ctrl+z / 26 / 0x1A - Enable/disable ignore RX pin on startup (ignore emergency reset)
 '-'    / 45 / 0x2D - Clear display. Move cursor to home position.
        / 128-157 / 0x80-0x9D - Set the primary backlight brightness. 128 = Off, 157 = 100%.
        / 158-187 / 0x9E-0xBB - Set the green backlight brightness. 158 = Off, 187 = 100%.
        / 188-217 / 0xBC-0xD9 - Set the blue backlight brightness. 188 = Off, 217 = 100%.
         For example, to change the baud rate to 115200 send 124 followed by 18.
 '+'    / 43 / 0x2B - Set Backlight to RGB value, follow + by 3 numbers 0 to 255, for the r, g and b values.
         For example, to change the backlight to yellow send + followed by 255, 255 and 0.
*/

#include <SPI.h>

//////// PINS FOR ESP32S2 to LCD
/*
 * Pin 4 => SCK
 * Pin 5 => SDO
 * Pin 6 => SDI
 * Pin 7 => CS
 */

#define VSPI FSPI   // unclear what this is.....
#define VSPI_MISO   5
#define VSPI_MOSI   6
#define VSPI_SCLK   4
#define VSPI_SS     7
#define SCR_WIDTH   16
#define LCD_SIZE    32

void init_lcd();
void clear_screen();
void print_lcd(const char* msg, bool clear_buf = true);
void setBacklight(uint8_t R, uint8_t G, uint8_t B);

// data for lcd using SPI
SPIClass * vspi = NULL;
char cur_color = 0;

//static const int spiClk = 1000000; // 1 MHz
 
int cycles = 0;

void setup() 
{ 
  Serial.begin(115200);  
  init_lcd();
}

void loop() 
{
  //Send the clear display command to the display - this forces the cursor to return to the beginning of the display
  clear_screen();
  
  // print hello with clear screen, and then goodbye without clear screen
  print_lcd("Hello, ");
  delay(1000);
  print_lcd("Goodbye!", false);
  

  // cycle through colors for the backlight
  if(cycles < 100 && cur_color != 'R'){
    setBacklight(255,0,0);
    cur_color = 'R';
  }
  else if ( cycles > 100 && cycles < 200 && cur_color != 'G'){
    setBacklight(0,255,0);
    cur_color = 'G';
    }
  else if (cycles > 200 && cur_color != 'B'){
    setBacklight(0,0,255);
    cur_color = 'B';
    }
    

  Serial.println(tempString);

  delay(500);
}



// function initialies the SPI object to transmit to LCD
// Uses global vspi pointer
void init_lcd(){
  vspi = new SPIClass(VSPI);
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS); 
  pinMode(VSPI_SS, OUTPUT); // set Slave Select as output
  digitalWrite(VSPI_SS, LOW); //Drive the CS pin low to select OpenLCD
  vspi->transfer(0x04); // set width to 16
  vspi->transfer(0x06); // set lines to 2
  digitalWrite(VSPI_SS, HIGH); //Release the CS pin to de-select OpenLCD
  vspi->endTransaction();
}


// clear the lcd display and reset cursor
void clear_screen(){
  digitalWrite(VSPI_SS, LOW); //Drive the CS pin low to select OpenLCD
  // need to let LCD move to setting mode before clearing screen, hence the delay
  vspi->transfer('|'); //Put LCD into setting mode
  delay(5);
  vspi->transfer('-'); //Send clear display command
  delay(5);
  digitalWrite(VSPI_SS, HIGH); //Release the CS pin to de-select OpenLCD
  //vspi->endTransaction();
}


// print string to screen, by defualt clears existing screen
void print_lcd(const char* msg, bool clear_buf){
  int i;
  int len;
  
  // clear the screen and reset the length of the buffer  
  if (clear_buf){
    clear_screen();
  }

  for(i = 0; i < LCD_SIZE; i++){
    if (msg[i] == '\0')
      break;
    }
  len = i;
  
  //Drive the CS pin low to select OpenLCD
  digitalWrite(VSPI_SS, LOW); 
  // display buffer
  for(i = 0; i < len; i++){
    vspi->transfer(msg[i]);
    delay(1);
  }
  digitalWrite(VSPI_SS, HIGH); //Release the CS pin to de-select OpenLCD
  //vspi->endTransaction();
}

void setBacklight(uint8_t R, uint8_t G, uint8_t B){
  
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
