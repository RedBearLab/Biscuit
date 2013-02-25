
/*

  Copyright (c) 2013 RedBearLab

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal 
  in the Software without restriction, including without limitation the rights 
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#include "iocc2540.h"
#include "i2c.h"

void i2c_init(void)
{
  P1SEL &= ~(SCL | SDA);    // 0 = I/O
  P1DIR |= (SDA);           // 1 = OUTPUT
  SDA_PIN = HIGH;              // P1_6 = SDA
  P1DIR |= (SCL);           // 1 = OUTPUT
  SCL_PIN = HIGH;               // P1_7 = SCL  
}

// Set up start condition for I2C
unsigned char i2c_start(unsigned char addressRW)
{  
  SDA_PIN = LOW; // SDA
  asm("nop");
  SCL_PIN = LOW; // SCL
  
  return i2c_write(addressRW);
}

unsigned char i2c_write(unsigned char data)
{
  // write byte
  for (unsigned char m = 0x80; m != 0; m >>= 1)
  {
    if (m & data)
      SDA_PIN = HIGH;
    else
      SDA_PIN = LOW;
    
    asm("nop");
    SCL_PIN = HIGH;
    asm("nop");
    asm("nop");
    SCL_PIN = LOW;
    asm("nop");
  }
           
  // get Ack or Nak
  P1DIR &= ~(SDA); // INPUT
  asm("nop");
  SDA_PIN = HIGH;
  asm("nop");
  SCL_PIN = HIGH;
  asm("nop");
  asm("nop");  
  unsigned char rtn = SDA_PIN;
  asm("nop");
  asm("nop");
  SCL_PIN = LOW;
  asm("nop");
  asm("nop");
  P1DIR |= (SDA); // OUTPUT
  asm("nop");
  asm("nop");
  SDA_PIN = LOW;  
  asm("nop");
  asm("nop");

  return rtn == 0;
}
            
unsigned char i2c_read(unsigned char last)  // Read 8 bits of I2C data
{
  unsigned char b = 0;

  SDA_PIN = HIGH;
  P1DIR &= ~(SDA); // INPUT
  asm("nop");
  asm("nop");
  
  // read byte
  for (unsigned char i = 0; i < 8; i++)
  {
    b <<= 1;
    asm("nop");
    SCL_PIN = HIGH;
    asm("nop");
    asm("nop");
    if (SDA_PIN)
      b |= 1;
    SCL_PIN = LOW;
  }

  // send Ack or Nak
  P1DIR |= (SDA); // OUTPUT
  asm("nop");
  asm("nop");
  
  if (last)
    SDA_PIN = HIGH;
  else
    SDA_PIN = LOW;
  
  asm("nop");
  SCL_PIN = HIGH;
  asm("nop");
  SCL_PIN = LOW;
  asm("nop");
  SDA_PIN = LOW;
  asm("nop");

  return b;
}   

unsigned char i2c_restart(unsigned char addressRW)
{
  SDA_PIN = HIGH;
  SCL_PIN = HIGH;
  asm("nop");
  asm("nop");
  return i2c_start(addressRW);
}

void i2c_stop(void)
{
  SDA_PIN = LOW;
  asm("nop");
  SCL_PIN = HIGH;
  asm("nop");
  SDA_PIN = HIGH;
  asm("nop");    
}
