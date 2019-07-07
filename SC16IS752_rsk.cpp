/*
  SC16IS752.cpp - Library for interfacing with SC16IS752 i2c to serial and gpio port expander with esp32
  ported from Eric O'Neill, July 30, 2015
  by rsk

baud rate and divisor table
	2400 	48
	3600 	32
	4800 	24
	7200 	16
	9600 	12
	19200 	6
*/

#include "SC16IS752_rsk.h"
#include "Arduino.h"
#include "Wire.h"

SC16IS752::SC16IS752(int address, int channel){
	
	_outputRegVal = 0x00;
	_inputRegVal = 0x00;
	_deviceAddress = address;
	_channel = channel;
}

//-------------------- private functions ---------------------------

unsigned char SC16IS752::readRegister(unsigned char regAddress){
	//Serial.print("Dev:");
	//Serial.print(_deviceAddress, HEX);
	//Serial.print(" rReg:");
	//Serial.print(regAddress, HEX);

	//byte dat;
	uint8_t result;
	Wire.beginTransmission(_deviceAddress);
	Wire.write((regAddress << 3 | _channel << 1));
	Wire.endTransmission();
	Wire.requestFrom(_deviceAddress, 1);
	result = Wire.read();
	return result;
}

void SC16IS752::writeRegister(unsigned char regAddress, unsigned char data){
	Wire.beginTransmission(_deviceAddress);
  	Wire.write((regAddress << 3 | _channel << 1));
  	Wire.write(data);
 	Wire.endTransmission();
}

bool SC16IS752::uartConnected(){
	const char TEST_CHARACTER = 'H';
  
 	writeRegister(SC16IS752_SPR, TEST_CHARACTER);

 	return (readRegister(SC16IS752_SPR) == TEST_CHARACTER);
}

//-------------------- public functions ---------------------------

unsigned char SC16IS752::available() {
  /*
  
    Get the number of bytes (characters) available for reading.

    This is data that's already arrived and stored in the receive
    buffer (which holds 64 bytes).
  
   */
  // This alternative just checks if there's data but doesn't
  // return how many characters are in the buffer:
  //    readRegister(LSR) & 0x01
  return readRegister(SC16IS752_RXLVL);
}

unsigned char SC16IS752::txBufferSize(){
	// returns the number of empty spaces in the tx buffer, so 0 means it's full
	//
	return readRegister(SC16IS752_TXLVL);
}

int SC16IS752::read(){
  return readRegister(SC16IS752_RHR);
}

void SC16IS752::write(unsigned char value) {
	//  Write byte to UART.
	
  while (readRegister(SC16IS752_TXLVL) == 0) {
    // Wait for space in TX buffer, returns the number of spaces in the buffer.
  };
  writeRegister(SC16IS752_THR, value); 
}

// ----------------------------------- added by rsk -------

void SC16IS752::flush()
{
	uint8_t tmp_lsr;

	do {
		tmp_lsr = readRegister(SC16IS752_LSR);
	} while ((tmp_lsr&0x20) ==0);
}

void SC16IS752::SetBaudrate(uint16_t baudrateDivisor) // uses baud rate divisor from p17 of the datasheet.
{
    uint8_t temp_lcr;

    temp_lcr = readRegister(SC16IS752_LCR);
    temp_lcr |= 0x80;
    writeRegister(SC16IS752_LCR,temp_lcr);
    //write to DLL
    writeRegister(SC16IS752_DLL,(uint8_t)baudrateDivisor);
    //write to DLH
    writeRegister(SC16IS752_DLH,(uint8_t)(baudrateDivisor>>8));
    temp_lcr &= 0x7F;
    writeRegister(SC16IS752_LCR,temp_lcr);

    return;
}

void SC16IS752::ResetDevice(void)
{
    uint8_t reg;

    reg = readRegister(SC16IS752_IOControl);
    reg |= 0x08;
    writeRegister(SC16IS752_IOControl, reg);

    return;
}

void SC16IS752::FIFOEnable(uint8_t fifo_enable)
{
    uint8_t temp_fcr;

    temp_fcr = readRegister(SC16IS752_FCR);

    if (fifo_enable == 0){
        temp_fcr &= 0xFE;
    } else {
        temp_fcr |= 0x01;
    }
    writeRegister(SC16IS752_FCR,temp_fcr);

    return;
}

void SC16IS752::SetLine(uint8_t data_length, uint8_t parity_select, uint8_t stop_length )
{
    uint8_t temp_lcr;
    temp_lcr = readRegister(SC16IS752_LCR);
    temp_lcr &= 0xC0; //Clear the lower six bit of LCR (LCR[0] to LCR[5]
#ifdef  SC16IS750_DEBUG_PRINT
    Serial.print("LCR Register:0x");
    Serial.println(temp_lcr,DEC);
#endif
    switch (data_length) {            //data length settings
        case 5:
            break;
        case 6:
            temp_lcr |= 0x01;
            break;
        case 7:
            temp_lcr |= 0x02;
            break;
        case 8:
            temp_lcr |= 0x03;
            break;
        default:
            temp_lcr |= 0x03;
            break;
    }

    if ( stop_length == 2 ) {
        temp_lcr |= 0x04;
    }

    switch (parity_select) {            //parity selection length settings
        case 0:                         //no parity
             break;
        case 1:                         //odd parity
            temp_lcr |= 0x08;
            break;
        case 2:                         //even parity
            temp_lcr |= 0x18;
            break;
        case 3:                         //force '1' parity
            temp_lcr |= 0x03;
            break;
        case 4:                         //force '0' parity
            break;
        default:
            break;
    }

    writeRegister(SC16IS752_LCR,temp_lcr);
}
