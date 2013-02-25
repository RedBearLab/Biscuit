
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

#include <iocc2540.h>

#define HIGH    1
#define LOW     0
#define INPUT   0
#define OUTPUT  1
#define SDA_PIN P1_6
#define SCL_PIN P1_7

#define SDA   (1<<6)
#define SCL   (1<<7)

void i2c_init(void);
unsigned char i2c_start(unsigned char addressRW);
unsigned char i2c_restart(unsigned char addressRW);
unsigned char i2c_write(unsigned char data);
unsigned char i2c_read(unsigned char last);
void i2c_stop(void);
