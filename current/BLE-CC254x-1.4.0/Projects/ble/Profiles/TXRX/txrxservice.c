
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

#include "txrxservice.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// TXRX Profile Service UUID
CONST uint8 txrxServUUID[ATT_UUID_SIZE] =
{ 
  TXRX_SERV_UUID
};

// TX Data Char UUID: 0x0002
CONST uint8 txDataCharUUID[ATT_UUID_SIZE] =
{ 
  TX_DATA_CHAR_UUID
};

// RX Data Char UUID: 0x0003
CONST uint8 rxDataCharUUID[ATT_UUID_SIZE] =
{ 
  RX_DATA_CHAR_UUID
};

// Baudrate Char UUID: 0x0004
CONST uint8 BaudrateCharUUID[ATT_UUID_SIZE] =
{ 
  BAUDRATE_CHAR_UUID
};

// Device name Char UUID: 0x0005
CONST uint8 DevNameCharUUID[ATT_UUID_SIZE] =
{ 
  DEV_NAME_CHAR_UUID
};

// Version Char UUID: 0x0006
CONST uint8 VersionCharUUID[ATT_UUID_SIZE] =
{ 
  VERSION_CHAR_UUID
};

// Version Char UUID: 0x0007
CONST uint8 TxPowerCharUUID[ATT_UUID_SIZE] =
{ 
  TX_POWER_CHAR_UUID
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

static txrxServiceCBs_t *txrxService_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Service attribute
static CONST gattAttrType_t txrxService = { ATT_UUID_SIZE, txrxServUUID };


// Characteristic 2 Properties
static uint8 txrxServiceChar2Props = GATT_PROP_NOTIFY;

// Characteristic 2 Value
static uint8 txDataChar[20] = {0};

// Characteristic 2 Length
static uint8 txDataLen = 0;

static gattCharCfg_t txrxServiceChar2Config[GATT_MAX_NUM_CONN];

// Characteristic 2 User Description
static uint8 txrxServiceChar2UserDesp[17] = "Characteristic 2\0";


// Characteristic 3 Properties
static uint8 txrxServiceChar3Props = GATT_PROP_WRITE_NO_RSP;

// Characteristic 3 Value
static uint8 rxDataChar[20] = {0};

// Characteristic 3 Length
static uint8 rxDataLen = 0;

// Characteristic 3 User Description
static uint8 txrxServiceChar3UserDesp[17] = "Characteristic 3\0";


// Characteristic 4 Properties
static uint8 txrxServiceChar4Props = GATT_PROP_WRITE | GATT_PROP_READ;

// Characteristic 4 Value
static uint8 Baudrate = 0;

// Characteristic 4 User Description
static uint8 txrxServiceChar4UserDesp[17] = "Characteristic 4\0";


// Characteristic 5 Properties
static uint8 txrxServiceChar5Props = GATT_PROP_WRITE | GATT_PROP_READ;

// Characteristic 5 Value
static uint8 DevName[20] = "Biscuit 2";

// Characteristic 5 Length
static uint8 DevNameLen = 9;

// Characteristic 5 User Description
static uint8 txrxServiceChar5UserDesp[17] = "Characteristic 5\0";


// Characteristic 6 Properties
static uint8 txrxServiceChar6Props = GATT_PROP_READ;

// Characteristic 6 Value
static uint8 Version[12] = "Biscuit_2.0";

// Characteristic 6 Length
static uint8 VersionLen = 11;

// Characteristic 6 User Description
static uint8 txrxServiceChar6UserDesp[17] = "Characteristic 6\0";


// Characteristic 7 Properties
static uint8 txrxServiceChar7Props = GATT_PROP_WRITE | GATT_PROP_READ;

// Characteristic 7 Value
static uint8 TxPower = 2;

// Characteristic 7 User Description
static uint8 txrxServiceChar7UserDesp[17] = "Characteristic 7\0";

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t txrxAttrTbl[] = 
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&txrxService            /* pValue */
  },

    // Characteristic 2 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &txrxServiceChar2Props 
    },

      // Characteristic Value 2
      { 
        { ATT_UUID_SIZE, txDataCharUUID },
        GATT_PERMIT_READ, 
        0, 
        txDataChar 
      },
      
      // Characteristic 2 configuration
      { 
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8 *)txrxServiceChar2Config 
      },
      
      // Characteristic 2 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        txrxServiceChar2UserDesp 
      },           
      
    // Characteristic 3 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &txrxServiceChar3Props 
    },

      // Characteristic Value 3
      { 
        { ATT_UUID_SIZE, rxDataCharUUID },
        GATT_PERMIT_WRITE, 
        0, 
        rxDataChar 
      },

      // Characteristic 3 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        txrxServiceChar3UserDesp 
      },
      
    // Characteristic 4 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &txrxServiceChar4Props 
    },

      // Characteristic Value 4
      { 
        { ATT_UUID_SIZE, BaudrateCharUUID },
        GATT_PERMIT_WRITE | GATT_PERMIT_READ, 
        0, 
        &Baudrate 
      },

      // Characteristic 4 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        txrxServiceChar4UserDesp 
      },
      
    // Characteristic 5 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &txrxServiceChar5Props 
    },

      // Characteristic Value 5
      { 
        { ATT_UUID_SIZE, DevNameCharUUID },
        GATT_PERMIT_WRITE | GATT_PERMIT_READ, 
        0, 
        DevName 
      },

      // Characteristic 5 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        txrxServiceChar5UserDesp 
      },
      
    // Characteristic 6 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &txrxServiceChar6Props 
    },

      // Characteristic Value 6
      { 
        { ATT_UUID_SIZE, VersionCharUUID },
        GATT_PERMIT_READ, 
        0, 
        Version 
      },

      // Characteristic 6 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        txrxServiceChar6UserDesp 
      },
      
      // Characteristic 7 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &txrxServiceChar7Props 
    },

      // Characteristic Value 7
      { 
        { ATT_UUID_SIZE, TxPowerCharUUID },
        GATT_PERMIT_WRITE | GATT_PERMIT_READ, 
        0, 
        &TxPower 
      },

      // Characteristic 7 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        txrxServiceChar7UserDesp 
      },
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 txrx_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t txrx_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );

static void txrx_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t txrxCBs =
{
  txrx_ReadAttrCB,  // Read callback function pointer
  txrx_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      TXRX_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t TXRX_AddService( uint32 services )
{
  uint8 status = SUCCESS;

  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, txrxServiceChar2Config );

  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( txrx_HandleConnStatusCB );  
  
  if ( services & TXRX_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( txrxAttrTbl, 
                                          GATT_NUM_ATTRS( txrxAttrTbl ),
                                          &txrxCBs );
  }

  return ( status );
}


/*********************************************************************
 * @fn      TXRX_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t TXRX_RegisterAppCBs( txrxServiceCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    txrxService_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
  

/*********************************************************************
 * @fn      TXRX_SetParameter
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
bStatus_t TXRX_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  
  switch ( param )
  {
    case TX_DATA_CHAR:
      if ( len <= 20 ) 
      {
        VOID osal_memcpy( txDataChar, value, len );
        txDataLen = len;
        
        GATTServApp_ProcessCharCfg( txrxServiceChar2Config, txDataChar, FALSE,
                      txrxAttrTbl, GATT_NUM_ATTRS( txrxAttrTbl ),
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
        VOID osal_memcpy( rxDataChar, value, len );
        rxDataLen = len;
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    case BAUDRATE_CHAR:
      if ( len == 1 ) 
      {
        VOID osal_memcpy( &Baudrate, value, len );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    case DEV_NAME_CHAR:
      if ( len <= 20 ) 
      {
        VOID osal_memcpy( DevName, value, len );
        DevNameLen = len;
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
      
    case VERSION_CHAR:
      {
        VOID osal_memcpy( Version, value, len );
        VersionLen = len;
      }
      break;
      
    case TX_POWER_CHAR:
      if ( len == 1 ) 
      {
        VOID osal_memcpy( &TxPower, value, len );
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
bStatus_t TXRX_GetParameter( uint8 param, uint8 *len, void *value )
{
  bStatus_t ret = SUCCESS;
  
  switch ( param )
  {
    case TX_DATA_CHAR:
      len[0] = txDataLen;
      VOID osal_memcpy(value, txDataChar, txDataLen);
      break;      

    case RX_DATA_CHAR:
      len[0] = rxDataLen;
      VOID osal_memcpy(value, rxDataChar, rxDataLen);
      break; 

    case BAUDRATE_CHAR:
      VOID osal_memcpy(value, &Baudrate, 1);
      break;
      
    case DEV_NAME_CHAR:
      *len = DevNameLen;
      VOID osal_memcpy(value, DevName, DevNameLen);
      break;
      
    case VERSION_CHAR:
      *len = VersionLen;
      VOID osal_memcpy(value, Version, VersionLen);
      break;
      
    case TX_POWER_CHAR:
      VOID osal_memcpy(value, &TxPower, 1);
      break;
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          txrx_ReadAttrCB
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
static uint8 txrx_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
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
 
  if ( pAttr->type.len == ATT_UUID_SIZE )
  {
    if ( osal_memcmp(pAttr->type.uuid, txDataCharUUID, ATT_UUID_SIZE) )
    {
      *pLen = txDataLen;
      VOID osal_memcpy( pValue, pAttr->pValue, txDataLen );     
    }
    else if ( osal_memcmp(pAttr->type.uuid, BaudrateCharUUID, ATT_UUID_SIZE) )
    {
      //*pLen = 1;
      //VOID osal_memcpy( pValue, pAttr->pValue, 1 );
      switch(Baudrate)
      {
        case 0:   //9600
        {
          *pLen = 9;
          uint8 *str = "BAUD_9600";
          VOID osal_memcpy( pValue, str, 9 );
          break;
        }
        
        case 1:   //19200
        {
          *pLen = 10;
          uint8 *str = "BAUD_19200";
          VOID osal_memcpy( pValue, str, 10 );
          break;
        }
         
        case 2:   //38400
        {
          *pLen = 10;
          uint8 *str = "BAUD_38400";
          VOID osal_memcpy( pValue, str, 10 );
          break;
        }
        
        case 3:   //57600
        {
          *pLen = 10;
          uint8 *str = "BAUD_57600";
          VOID osal_memcpy( pValue, str, 10 );
          break;
        }
        
        case 4:   //115200
        {
          *pLen = 11;
          uint8 *str = "BAUD_115200";
          VOID osal_memcpy( pValue, str, 11 );
          break;
        }
        
        default:
          break;
      }
    }
    else if ( osal_memcmp(pAttr->type.uuid, DevNameCharUUID, ATT_UUID_SIZE) )
    {
      *pLen = DevNameLen;
      VOID osal_memcpy( pValue, pAttr->pValue, DevNameLen );     
    }
    else if ( osal_memcmp(pAttr->type.uuid, VersionCharUUID, ATT_UUID_SIZE) )
    {
      *pLen = VersionLen;
      VOID osal_memcpy( pValue, pAttr->pValue, VersionLen );     
    }
    else if ( osal_memcmp(pAttr->type.uuid, TxPowerCharUUID, ATT_UUID_SIZE) )
    {
      switch(TxPower)
      {
        case 0:   //-23dbm
        {
          *pLen = 7;
          uint8 *str = "-23 dBm";
          VOID osal_memcpy( pValue, str, 7 );
          break;
        }
        
        case 1:   //-6dbm
        {
          *pLen = 6;
          uint8 *str = "-6 dBm";
          VOID osal_memcpy( pValue, str, 6 );
          break;
        }
         
        case 2:   //0dbm
        {
          *pLen = 5;
          uint8 *str = "0 dBm";
          VOID osal_memcpy( pValue, str, 5 );
          break;
        }
        
        case 3:   //+4dbm
        {
          *pLen = 6;
          uint8 *str = "+4 dBm";
          VOID osal_memcpy( pValue, str, 6 );
          break;
        }
        
        default:
          break;
      }     
    }
    else
    {
      // Should never get here!
      *pLen = 0;
      status = ATT_ERR_ATTR_NOT_FOUND;
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      txrx_WriteAttrCB
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
static bStatus_t txrx_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
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
      case GATT_CLIENT_CHAR_CFG_UUID:
        status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                 offset, GATT_CLIENT_CFG_NOTIFY );
        if (status == SUCCESS)
        {
          uint16 charCfg = BUILD_UINT16( pValue[0], pValue[1] );
          txrxService_AppCBs->pfnTXRXServiceChange( (charCfg == GATT_CFG_NO_OPERATION) ?
                                                      TXRX_RX_NOTI_DISABLED :
                                                      TXRX_RX_NOTI_ENABLED );
        }
        break;
        
      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else // 128-bit  
  {
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
        uint8 *pCurValue = (uint8 *)pAttr->pValue;
        osal_memcpy(pCurValue, pValue, len);
        rxDataLen = len;
        
        notifyApp = TXRX_RX_DATA_READY;           
      }
    }
    else if ( osal_memcmp(pAttr->type.uuid, BaudrateCharUUID, ATT_UUID_SIZE) )
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
      if(*pValue > 4)
      {
        status = ATT_ERR_INVALID_VALUE;
      }
        
      //Write the value
      if ( status == SUCCESS )
      {                
        uint8 *pCurValue = (uint8 *)pAttr->pValue;
        osal_memcpy(pCurValue, pValue, len);
        
        notifyApp = BAUDRATE_SET;           
      }
    }
    else if ( osal_memcmp(pAttr->type.uuid, DevNameCharUUID, ATT_UUID_SIZE) )
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
        uint8 *pCurValue = (uint8 *)pAttr->pValue;
        osal_memcpy(pCurValue, pValue, len);
        DevNameLen = len;
        
        notifyApp = DEV_NAME_CHANGED;           
      }
    }
    else if ( osal_memcmp(pAttr->type.uuid, TxPowerCharUUID, ATT_UUID_SIZE) )
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
      if(*pValue > 3)
      {
        status = ATT_ERR_INVALID_VALUE;
      }
        
      //Write the value
      if ( status == SUCCESS )
      {                
        uint8 *pCurValue = (uint8 *)pAttr->pValue;
        osal_memcpy(pCurValue, pValue, len);
        
        notifyApp = TX_POWER_CHANGED;           
      }
    }
    else
    {      
      // Should never get here! (characteristics 2 and 4 do not have write permissions)
      status = ATT_ERR_ATTR_NOT_FOUND;
    }
  }

  // If a charactersitic value changed then callback function to notify application of change
  if ( (notifyApp != 0xFF ) && txrxService_AppCBs && txrxService_AppCBs->pfnTXRXServiceChange )
  {
    txrxService_AppCBs->pfnTXRXServiceChange( notifyApp );  
  }
  
  return ( status );
}

/*********************************************************************
 * @fn          txrx_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void txrx_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset Client Char Config if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
         ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) && 
           ( !linkDB_Up( connHandle ) ) ) )
    { 
      GATTServApp_InitCharCfg( connHandle, txrxServiceChar2Config );
    }
  }
}


/*********************************************************************
*********************************************************************/