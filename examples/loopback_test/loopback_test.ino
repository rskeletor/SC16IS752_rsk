// test code for sc16is752 library.
//
// loopback required between txa -> rxa, and txb -> rxb
//

#include "Wire.h"
#include "SC16IS752_rsk.h"

SC16IS752 uart1 = SC16IS752(0x4D,0x00); // channel 0
SC16IS752 uart2 = SC16IS752(0x4D,0x01); // channel 1

void setup() {
  Serial.begin(115200);
  Serial.print("\n\n\n*SC15IS752 i2c uart test*\n");

  // 
  Wire.begin();
  Wire.setClock(1000000);     // lets go fast..
  delay(200);                 // let it settle.

  // new device setup
  Serial.print("Call reset\n");
  uart1.ResetDevice();
  Serial.print("Call FIFO enable\n");
  uart1.FIFOEnable(1);
  uart2.FIFOEnable(1);
  Serial.print("Call SetBaudrate\n");
  uart1.SetBaudrate(0x18);      // 0x18 == 24 == 4800 baud
  uart2.SetBaudrate(0x18);      // 0x18 == 24 == 4800 baud
  Serial.print("Call SetLine\n");
  uart1.SetLine(8,0,1);
  uart2.SetLine(8,0,1);
   
  }
void loop() 
{
    unsigned char c;

    // uart 1
    //
    uart1.write(0x55);
    uart1.write(0xAA);
    uart1.flush();
    delay(10);
    while(uart1.available()!=0) {  // get all the bytes
        c = uart1.read();
        Serial.print(c);
        Serial.print(".");   
        delay(5);
        }
    delay(10);
    
     // uart 2
    //
    uart1.write(0x07);
    uart2.write(0x08);
    uart1.write(0x07);
    uart2.write(0x08);
    uart2.flush();
    delay(10);
    while(uart2.available()!=0) {  // got any datas ?
        c = uart2.read();
        Serial.print(c);
        Serial.print(".");   
        delay(5);
        } 
    Serial.println();
    delay(10);
     
};
