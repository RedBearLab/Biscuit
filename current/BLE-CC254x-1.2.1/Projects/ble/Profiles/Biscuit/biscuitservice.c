
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

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "biscuitservice.h"

#include "hal_led.h"
#include "hal_uart.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        17

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0x0000
CONST uint8 biscuitServUUID[ATT_UUID_SIZE] =
{ 
  UUID_BASE_TAIL, LO_UINT16(BISCUIT_SERV_UUID), HI_UINT16(BISCUIT_SERV_UUID), UUID_BASE_HEAD
};

// Vendor Name Char UUID: 0x0001
CONST uint8 vendorNameCharUUID[ATT_UUID_SIZE] =
{ 
  UUID_BASE_TAIL, LO_UINT16(VENDOR_NAME_CHAR_UUID), HI_UINT16(VENDOR_NAME_CHAR_UUID), UUID_BASE_HEAD
};

// TX Data Char UUID: 0x0002
CONST uint8 txDataCharUUID[ATT_UUID_SIZE] =
{ 
  UUID_BASE_TAIL, LO_UINT16(TX_DATA_CHAR_UUID), HI_UINT16(TX_DATA_CHAR_UUID), UUID_BASE_HEAD
};

// RX Data Char UUID: 0x0003
CONST uint8 rxDataCharUUID[ATT_UUID_SIZE] =
{ 
  UUID_BASE_TAIL, LO_UINT16(RX_DATA_CHAR_UUID), HI_UINT16(RX_DATA_CHAR_UUID), UUID_BASE_HEAD
};

// Reset Write Char UUID: 0x0004
CONST uint8 resetWriteCharUUID[ATT_UUID_SIZE] =
{ 
  UUID_BASE_TAIL, LO_UINT16(RESET_WRITE_CHAR_UUID), HI_UINT16(RESET_WRITE_CHAR_UUID), UUID_BASE_HEAD
};

// Library Version Char UUID: 0x0005
CONST uint8 libVerCharUUID[ATT_UUID_SIZE] =
{ 
  UUID_BASE_TAIL, LO_UINT16(LIB_VER_CHAR_UUID), HI_UINT16(LIB_VER_CHAR_UUID), UUID_BASE_HEAD
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static biscuitProfileCBs_t *biscuitProfile_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t biscuitService = { UUID_SIZE, biscuitServUUID };


// Simple Profile Characteristic 1 Properties
static uint8 vendorNameCharProps = GATT_PROP_READ;

// Characteristic 1 Value
static uint8 vendorNameCharValue[13] = "Red Bear Lab.";

// Simple Profile Characteristic 1 User Description
static uint8 simpleProfileChar1UserDesp[17] = "Characteristic 1\0";


// Simple Profile Characteristic 2 Properties
static uint8 simpleProfileChar2Props = GATT_PROP_NOTIFY;

// Characteristic 2 Value
static uint8 txDataChar[20] = {0};

static gattCharCfg_t simpleProfileChar2Config[GATT_MAX_NUM_CONN];

// Simple Profile Characteristic 2 User Description
static uint8 simpleProfileChar2UserDesp[17] = "Characteristic 2\0";


// Simple Profile Characteristic 3 Properties
static uint8 simpleProfileChar3Props = GATT_PROP_WRITE;

// Characteristic 3 Value
static uint8 rxDataChar = 0;

// Simple Profile Characteristic 3 User Description
static uint8 simpleProfileChar3UserDesp[17] = "Characteristic 3\0";


// Simple Profile Characteristic 4 Properties
static uint8 simpleProfileChar4Props = GATT_PROP_WRITE;

// Characteristic 4 Value
static uint8 resetWriteChar = 0;

// Simple Profile Characteristic 4 Configuration Each client has its own
// instantiation of the Client Characteristic Configuration. Reads of the
// Client Characteristic Configuration only shows the configuration for
// that client and writes only affect the configuration of that client.
//static gattCharCfg_t simpleProfileChar4Config[GATT_MAX_NUM_CONN];
                                        
// Simple Profile Characteristic 4 User Description
static uint8 simpleProfileChar4UserDesp[17] = "Characteristic 4\0";


// Simple Profile Characteristic 5 Properties
static uint8 simpleProfileChar5Props = GATT_PROP_READ;

// Characteristic 5 Value
static uint8 libVerChar[2] = { 0x01, 0x01 };

// Simple Profile Characteristic 5 User Description
static uint8 simpleProfileChar5UserDesp[17] = "Characteristic 5\0";

static uint16 bLen;

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t biscuitAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&biscuitService            /* pValue */
  },

    // Vendor Name Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &vendorNameCharProps 
    },

      // Vendor Name Value
      { 
        { UUID_SIZE, vendorNameCharUUID },
        GATT_PERMIT_READ, 
        0, 
        vendorNameCharValue 
      },

      // Vendor Name User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileChar1UserDesp 
      },      

    // Characteristic 2 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileChar2Props 
    },

      // Characteristic Value 2
      { 
        { UUID_SIZE, txDataCharUUID },
        GATT_PERMIT_READ, 
        0, 
        txDataChar 
      },
      
      // Characteristic 2 configuration
      { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)simpleProfileChar2Config 
      },
      
      // Characteristic 2 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileChar2UserDesp 
      },           
      
    // Characteristic 3 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileChar3Props 
    },

      // Characteristic Value 3
      { 
        { UUID_SIZE, rxDataCharUUID },
        GATT_PERMIT_WRITE, 
        0, 
        &rxDataChar 
      },

      // Characteristic 3 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileChar3UserDesp 
      },

    // Characteristic 4 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileChar4Props 
    },

      // Characteristic Value 4
      { 
        { UUID_SIZE, resetWriteCharUUID },
        GATT_PERMIT_WRITE, 
        0, 
        &resetWriteChar 
      },
/*
      // Characteristic 4 configuration
      { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)simpleProfileChar4Config 
      },
*/      
      // Characteristic 4 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileChar4UserDesp 
      },
      
    // Characteristic 5 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &simpleProfileChar5Props 
    },

      // Characteristic Value 5
      { 
        { UUID_SIZE, libVerCharUUID },
        GATT_PERMIT_READ, 
        0, 
        libVerChar 
      },

      // Characteristic 5 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        simpleProfileChar5UserDesp 
      },
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 biscuit_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t biscuit_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );

static void biscuit_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t biscuitCBs =
{
  biscuit_ReadAttrCB,  // Read callback function pointer
  biscuit_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Biscuit_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t Biscuit_AddService( uint32 services )
{
  uint8 status = SUCCESS;

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, simpleProfileChar2Config );

  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( biscuit_HandleConnStatusCB );  
  
  if ( services & BISCUIT_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( biscuitAttrTbl, 
                                          GATT_NUM_ATTRS( biscuitAttrTbl ),
                                          &biscuitCBs );
  }

  return ( status );
}


/*********************************************************************
 * @fn      Biscuit_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t Biscuit_RegisterAppCBs( biscuitProfileCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    biscuitProfile_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
  

/*********************************************************************
 * @fn      Biscuit_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t Biscuit_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case VENDOR_NAME_CHAR:
      if ( len == sizeof ( uint8 ) ) 
      {
    //    vendorNameCharValue = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case TX_DATA_CHAR:
      if ( len <= 20 ) 
      {
        //txDataChar = *((uint8*)value);
        VOID osal_memcpy( txDataChar, value, len );
        bLen = len;
        GATTServApp_ProcessCharCfg( simpleProfileChar2Config, txDataChar, FALSE,
                      biscuitAttrTbl, GATT_NUM_ATTRS( biscuitAttrTbl ),
                      INVALID_TASK_ID );

      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case RX_DATA_CHAR:
      if ( len == sizeof ( uint8 ) ) 
      {
        rxDataChar = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case RESET_WRITE_CHAR:
      if ( len == sizeof ( uint8 ) ) 
      {
        resetWriteChar = *((uint8*)value);
        
        // See if Notification has been enabled
//        GATTServApp_ProcessCharCfg( simpleProfileChar4Config, &resetWriteChar, FALSE,
//                                    biscuitAttrTbl, GATT_NUM_ATTRS( biscuitAttrTbl ),
//                                    INVALID_TASK_ID );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case LIB_VER_CHAR:
      if ( len == 2 ) 
      {
        VOID osal_memcpy( libVerChar, value, 2 );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn      Biscuit_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t Biscuit_GetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case VENDOR_NAME_CHAR:
   //   *((uint8*)value) = vendorNameCharValue;
      break;

    case TX_DATA_CHAR:
     // *((uint8*)value) = txDataChar;
      VOID osal_memcpy(value, txDataChar, len);
      break;      

    case RX_DATA_CHAR:
      *((uint8*)value) = rxDataChar;
      break;  

    case RESET_WRITE_CHAR:
      *((uint8*)value) = resetWriteChar;
      break;

    case LIB_VER_CHAR:
      VOID osal_memcpy( value, libVerChar, 2 );
      break;      
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          simpleProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 biscuit_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
 
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
      // gattserverapp handles those reads

      // characteristics 1 and 2 have read permissions
      // characteritisc 3 does not have read permissions; therefore it is not
      //   included here
      // characteristic 4 does not have read permissions, but because it
      //   can be sent as a notification, it is included here
      case VENDOR_NAME_CHAR_UUID:
        *pLen = VENDOR_NAME_SIZE;
        VOID osal_memcpy( pValue, pAttr->pValue, VENDOR_NAME_SIZE);
        break;
        
      case TX_DATA_CHAR_UUID:
      case RESET_WRITE_CHAR_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
        break;

      case LIB_VER_CHAR_UUID:
        *pLen = 2;
        VOID osal_memcpy( pValue, pAttr->pValue, 2 );
        break;
        
      default:
        // Should never get here! (characteristics 3 and 4 do not have read permissions)
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    const uint8 uuid[ATT_UUID_SIZE] = 
    { 
      UUID_BASE_TAIL, LO_UINT16(pAttr->type.uuid[0]), pAttr->type.uuid[1], UUID_BASE_HEAD
    };
    
    // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
    // gattserverapp handles those reads

    // characteristics 1 and 2 have read permissions
    // characteritisc 3 does not have read permissions; therefore it is not
    //   included here
    // characteristic 4 does not have read permissions, but because it
    //   can be sent as a notification, it is included here
    if ( osal_memcmp(pAttr->type.uuid, txDataCharUUID, ATT_UUID_SIZE) )
    {
      *pLen = bLen;
      VOID osal_memcpy( pValue, pAttr->pValue, bLen );     
    }
    
    else if (osal_memcmp(pAttr->type.uuid, vendorNameCharUUID, ATT_UUID_SIZE))
    {
      *pLen = 13;
      VOID osal_memcpy( pValue, pAttr->pValue, 13 );
    }
    
    else if (osal_memcmp(pAttr->type.uuid, libVerCharUUID, ATT_UUID_SIZE))
    {
      *pLen = 2;
      VOID osal_memcpy( pValue, pAttr->pValue, 2 );
    }
    
    else
    {
      // Should never get here! (characteristics 3 and 4 do not have read permissions)
      *pLen = 0;
      status = ATT_ERR_ATTR_NOT_FOUND;
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      simpleProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   complete - whether this is the last packet
 * @param   oper - whether to validate and/or write attribute value  
 *
 * @return  Success or Failure
 */
static bStatus_t biscuit_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset )
{
  bStatus_t status = SUCCESS;
  uint8 notifyApp = 0xFF;
  
  // If attribute permissions require authorization to write, return error
  if ( gattPermitAuthorWrite( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      case VENDOR_NAME_CHAR_UUID:
      case RX_DATA_CHAR_UUID:

        //Validate the value
        // Make sure it's not a blob oper
        if ( offset == 0 )
        {
          if ( len != 1 )
          {
            status = ATT_ERR_INVALID_VALUE_SIZE;
          }
        }
        else
        {
          status = ATT_ERR_ATTR_NOT_LONG;
        }
        
        //Write the value
        if ( status == SUCCESS )
        {
          uint8 *pCurValue = (uint8 *)pAttr->pValue;        
          *pCurValue = pValue[0];

  //        if( pAttr->pValue == &vendorNameCharValue )
          {
            notifyApp = VENDOR_NAME_CHAR;        
          }
  //        else
          {
            notifyApp = RX_DATA_CHAR;           
          }
        }
             
        break;

      case GATT_CLIENT_CHAR_CFG_UUID:
        status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                 offset, GATT_CLIENT_CFG_NOTIFY );
        break;
        
      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    const uint8 uuid[ATT_UUID_SIZE] = 
    { 
      UUID_BASE_TAIL, LO_UINT16(pAttr->type.uuid[0]), pAttr->type.uuid[1], UUID_BASE_HEAD
    };

    if ( osal_memcmp(pAttr->type.uuid, rxDataCharUUID, ATT_UUID_SIZE) )
    {
      //Validate the value
      // Make sure it's not a blob oper
      if ( offset == 0 )
      {
        if ( len > 20 )
        {
          status = ATT_ERR_INVALID_VALUE_SIZE;
        }
      }
      else
      {
        status = ATT_ERR_ATTR_NOT_LONG;
      }
        
      //Write the value
      if ( status == SUCCESS )
      {
   //     uint8 *pCurValue = (uint8 *)pAttr->pValue;        
   //     *pCurValue = pValue[0];

        HalUARTWrite(HAL_UART_PORT_0, pValue, len);
                     
        notifyApp = RX_DATA_CHAR;           
      }

      else
      {      
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
      }
    }
    
    else if ( osal_memcmp(pAttr->type.uuid, resetWriteCharUUID, ATT_UUID_SIZE) ) // UUID 0004
    {
      resetWriteChar = 1;
    }
    
    else if ( osal_memcmp(uuid, vendorNameCharUUID, ATT_UUID_SIZE) )
    {
      //Validate the value
      // Make sure it's not a blob oper
      if ( offset == 0 )
      {
        if ( len != 1 )
        {
          status = ATT_ERR_INVALID_VALUE_SIZE;
        }
      }
      else
      {
        status = ATT_ERR_ATTR_NOT_LONG;
      }
        
      //Write the value
      if ( status == SUCCESS )
      {
        uint8 *pCurValue = (uint8 *)pAttr->pValue;        
        *pCurValue = pValue[0];

    //    if( pAttr->pValue == &vendorNameCharValue ) //char 1 or 3?
        {
          notifyApp = VENDOR_NAME_CHAR;        
        }
    //    else
        {
          notifyApp = RX_DATA_CHAR;           
        }
      }

      else
      {      
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
      }
    }
  }

  // If a charactersitic value changed then callback function to notify application of change
  if ( (notifyApp != 0xFF ) && biscuitProfile_AppCBs && biscuitProfile_AppCBs->pfnBiscuitProfileChange )
  {
    biscuitProfile_AppCBs->pfnBiscuitProfileChange( notifyApp );  
  }
  
  return ( status );
}

/*********************************************************************
 * @fn          simpleProfile_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void biscuit_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset Client Char Config if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
         ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) && 
           ( !linkDB_Up( connHandle ) ) ) )
    { 
      GATTServApp_InitCharCfg( connHandle, simpleProfileChar2Config );
    }
  }
}


/*********************************************************************
*********************************************************************/
