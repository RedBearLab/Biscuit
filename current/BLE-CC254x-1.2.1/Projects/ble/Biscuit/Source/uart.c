
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

#include "bcomdef.h"
#include "hal_uart.h"
#include "uart.h"
#include "biscuitservice.h"
#include "peripheral.h"

extern gaprole_States_t gapProfileState;

static void uart0CB(uint8 port, uint8 event)
{
  static uint8 buf[255];
  uint8 writeReady = 1;
  uint16 bLen;

  if (port != HAL_UART_PORT_0)
    return;
    
  switch (event)
  {
    case HAL_UART_RX_FULL:
      {
        uint16 l = Hal_UART_RxBufLen(HAL_UART_PORT_0);
        
  //    HalUARTRead(HAL_UART_PORT_0, buf, 200);
        writeReady = 1;
        Biscuit_SetParameter(RESET_WRITE_CHAR, 1, &writeReady); 

        uint8 b[] = "(*)";
        HalUARTWrite(HAL_UART_PORT_0, b, 3); 
        HalUARTWrite(HAL_UART_PORT_0, (uint8*)&l, 2); 
        HalUARTWrite(HAL_UART_PORT_0, b, 3); 
      }
      return;
      
      break;
    
    case HAL_UART_RX_ABOUT_FULL:
 //     HalUARTRead(HAL_UART_PORT_0, buf, 200);
 //     writeReady = 1;
 //     Biscuit_SetParameter(RESET_WRITE_CHAR, 1, &writeReady); 

      break;
      
    case HAL_UART_RX_TIMEOUT:
            
      break;
    
    default:
      return;
  }
  
  bLen = Hal_UART_RxBufLen(HAL_UART_PORT_0);
    
  if (bLen > 20)
    bLen = 20;
  
  Biscuit_GetParameter(RESET_WRITE_CHAR, 1, &writeReady);

 //   HalUARTRead(HAL_UART_PORT_0, buf, bLen);
 //   HalUARTWrite(HAL_UART_PORT_0, buf, bLen);
 // return;
  //
  
//  if (writeReady)
  if (1)
  {
    HalUARTRead(HAL_UART_PORT_0, buf, bLen);
   
//    uint8 b[] = "x ";
//      HalUARTWrite(HAL_UART_PORT_0, b, 2); 
      
    if (gapProfileState == GAPROLE_CONNECTED)
    { 
      uart_data_received(buf, bLen);

      writeReady = 0;
      Biscuit_SetParameter(RESET_WRITE_CHAR, 1, &writeReady);    
    }
  }
  else
  {
    if (gapProfileState != GAPROLE_CONNECTED)
    {
      HalUARTRead(HAL_UART_PORT_0, buf, bLen);
    
      writeReady = 1;
      Biscuit_SetParameter(RESET_WRITE_CHAR, 1, &writeReady);    
    }    
  }
}

void uart_init(uint8 baudrate)
{
  P1SEL = 0x30;
  PERCFG |= 1;
  
  halUARTCfg_t config;
  config.configured = true;
  config.baudRate = baudrate;
  config.flowControl = false;
  config.flowControlThreshold = 64;
  config.idleTimeout = 0; // 6
  config.rx.maxBufSize = 128;
  config.tx.maxBufSize = 128;
  config.intEnable = true;
  config.callBackFunc = uart0CB;
  HalUARTOpen(HAL_UART_PORT_0, &config);
}
