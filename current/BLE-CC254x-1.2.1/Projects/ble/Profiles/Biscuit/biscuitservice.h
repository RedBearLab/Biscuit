
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

#ifndef BISCUITSERVICE_H
#define BISCUITSERVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Profile Parameters
#define VENDOR_NAME_CHAR                      0  // RW uint8 - Profile Characteristic 1 value 
#define TX_DATA_CHAR                          1  // RW uint8 - Profile Characteristic 2 value
#define RX_DATA_CHAR                          2  // RW uint8 - Profile Characteristic 3 value
#define RESET_WRITE_CHAR                      3  // RW uint8 - Profile Characteristic 4 value
#define LIB_VER_CHAR                          4  // RW uint8 - Profile Characteristic 4 value
  
// Biscuit Profile Service UUID
#define BISCUIT_SERV_UUID                   0x0000
    
// Char. UUID
#define VENDOR_NAME_CHAR_UUID               0x0001 // Vendor Name 
#define TX_DATA_CHAR_UUID                   0x0002 // For transferring data
#define RX_DATA_CHAR_UUID                   0x0003 // For receiving data
#define RESET_WRITE_CHAR_UUID               0x0004 // Enable write
#define LIB_VER_CHAR_UUID                   0x0005 // F/W version
  
// Biscuit Profile Services bit fields
#define BISCUIT_SERVICE               0x00000001 

#define VENDOR_NAME                       "Red Bear Lab."
#define VENDOR_NAME_SIZE                  13  
#define LIB_VER                           0x0201
  
#if (defined USE_128_BIT_UUID)
#define UUID_SIZE 16
#else
#define UUID_SIZE 2
#endif
  
/*********************************************************************
 * TYPEDEFS
 */

  
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef NULL_OK void (*biscuitProfileChange_t)( uint8 paramID );

typedef struct
{
  biscuitProfileChange_t        pfnBiscuitProfileChange;  // Called when characteristic value changes
} biscuitProfileCBs_t;

    

/*********************************************************************
 * API FUNCTIONS 
 */


/*
 * SimpleProfile_AddService- Initializes the Simple GATT Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 */

extern bStatus_t Biscuit_AddService( uint32 services );

/*
 * SimpleProfile_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t Biscuit_RegisterAppCBs( biscuitProfileCBs_t *appCallbacks );

/*
 * SimpleProfile_SetParameter - Set a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t Biscuit_SetParameter( uint8 param, uint8 len, void *value );
  
/*
 * SimpleProfile_GetParameter - Get a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t Biscuit_GetParameter( uint8 param, uint8 len, void *value );


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEGATTPROFILE_H */
