
/*
  SC16IS752_rsk.h - Library for interfacing with SC16IS752 i2c to serial and gpio port expander with esp32
  Created by rsk, ported from liamaha/SC16IS752_ESP8266
  
*/
#include <Wire.h>
//#include "Arduino.h"

#ifndef SC16IS752_rsk_H
#define SC16IS752_rsk_H

//Channel definitions
#define SC16IS752_CHANNEL_A 					0x00
#define SC16IS752_CHANNEL_B 					0x01

//Uart registers
#define SC16IS752_RHR          					0x00 //  Recv Holding Register is 0x00 in READ Mode
#define SC16IS752_THR          					0x00 //  Xmit Holding Register is 0x00 in WRITE Mode

//GPIO control registers
#define SC16IS752_IODir        					0x0A  // I/O P:ins Direction Register
#define SC16IS752_IOState      					0x0B  // I/O Pins State Register
#define SC16IS752_IOIntEna     					0x0C  // I/O Interrupt Enable Register
#define SC16IS752_IOControl    					0x0E  // I/O Pins Control Register

#define SC16IS752_IER          					0x01  // Interrupt Enable Register

#define SC16IS752_FCR          					0x02  // FIFO Control Register in WRITE Mode
#define SC16IS752_IIR          					0x02  // Interrupt Identification Register in READ Mode

#define SC16IS752_LCR          					0x03  // Line Control Register
#define SC16IS752_MCR          					0x04  // Modem Control Register
#define SC16IS752_LSR          					0x05  // Line status Register
#define SC16IS752_MSR          					0x06  // Modem Status Register
#define SC16IS752_SPR          					0x07  // ScratchPad Register
#define SC16IS752_TCR          					0x06  // Transmission Control Register
#define SC16IS752_TLR          					0x07  // Trigger Level Register
#define SC16IS752_TXLVL        					0x08  // Xmit FIFO Level Register
#define SC16IS752_RXLVL        					0x09  // Recv FIFO Level Register
#define SC16IS752_EFCR                          0x0F  // Extra Features Control Register

//Baud rate divisor registers
#define SC16IS752_DLL         					0x00  // Divisor Latch LSB  0x00
#define SC16IS752_DLH          					0x01  // Divisor Latch MSB  0x01

#define SC16IS752_EFR                           0x02 // Enhanced Function Register

class SC16IS752 {
public:
	
	//i2c to uart functions
	SC16IS752(int address,int channel);

	//void begin();
    unsigned char available();
    unsigned char txBufferSize();
    int read();
    void write(unsigned char value);
    void write(const char *str);
    //void configureUart();  	// don't use this
    bool uartConnected();
    void SetBaudrate(uint16_t baudrateDivisor);
	void SetLine(uint8_t data_length, uint8_t parity_select, uint8_t stop_length );
	void FIFOEnable(uint8_t fifo_enable);
	void ResetDevice(void);
	void flush();

private:
	unsigned char _deviceAddress;
	unsigned char _channel;
	unsigned char _outputRegVal;
	unsigned char _inputRegVal;
	unsigned char _pinConfig;

	void writeRegister(unsigned char registerAddress, unsigned char data);
    unsigned char readRegister(unsigned char registerAddress);
    void initUart();
    
};

#endif
