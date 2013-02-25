
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

#include "i2c.h"

void eeprom_write(unsigned short addr, unsigned char wdata)
{
    i2c_start(0xa0);
    i2c_write(addr>>8);  // High byte
    i2c_write(addr);  // Low byte
    i2c_write(wdata);
    i2c_stop();

    for (int i = 0; i < 5000; i++)
      asm("nop");
}

unsigned char eeprom_read(unsigned short addr)
{
    unsigned char r;
    
    i2c_start(0xa0);
    i2c_write(addr>>8);  // High byte
    i2c_write(addr);  // Low byte    
    i2c_restart(0xa1);
    r=i2c_read(1);
    i2c_stop();
    
    return r;    
}

void test_eeprom()
{
  unsigned short k = 65535;  
  while(k--)
  {
    eeprom_write(k, 0xBC);
    unsigned char r = eeprom_read(k);
 //   if (r != 0xBC)
 //     HalUARTWrite(HAL_UART_PORT_0, &r, 1);
  }
//  uint8 z = 'D';
//  HalUARTWrite(HAL_UART_PORT_0, &z, 1);
}