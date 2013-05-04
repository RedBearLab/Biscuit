
Files required for compiling:
 

1.Copy file from SDK 1.3 to SDK 1.2.1

From: BLE-CC254x-1.3\Projects\ble\common\cc2540\cc254x_f256_ubl_msd.xcl
To  : BLE-CC254x-1.2.1\Project\ble\common\cc2540\new_cc254x_f256_ubl_msd.xcl


2. Copy UBL files from SDK 1.3 to SDK 1.2.1

From: BLE-CC254x-1.3\Projects\ble\util\UBL


3. Modify OnBoard.c

File:
C:\Texas Instruments\BLE-CC254x-1.2.1\Projects\ble\common\cc2540\OnBoard.c

After this line:

extern uint8 LL_PseudoRand( uint8 *randData, uint8 dataLen );

Add the following lines:

#if   defined FEATURE_ABL
#include "..\..\util\ABL\app\sbl_app.c"
#elif defined FEATURE_SBL
#include "..\..\util\SBL\app\sbl_app.c"
#elif defined FEATURE_EBL
#include "..\..\util\EBL\app\sbl_app.c"
#elif defined FEATURE_UBL_MSD
#include "..\..\util\UBL\soc_8051\usb_msd\app\ubl_app.c"
#else
void appForceBoot(void);
#endif

After this function:

__near_func void Onboard_soft_reset( void )
{
  HAL_DISABLE_INTERRUPTS();
  asm("LJMP 0x0");
}

Add the following lines:

#if   defined FEATURE_ABL
#elif defined FEATURE_SBL
#elif defined FEATURE_EBL
#elif defined FEATURE_UBL_MSD
#else
void appForceBoot(void)
{
}
#endif


4. Modify hal_board_cfg.h

File 1: BLE-CC254x-1.2.1\Components\hal\target\CC2540EB
File 2: BLE-CC254x-1.2.1\Components\hal\target\CC2540USB

Search and add:

#if defined NON_BANKED
#define HAL_FLASH_LOCK_BITS            16
#define HAL_NV_PAGE_END                30

// Added for BLE Mini MSD  
#elif defined FEATURE_UBL_MSD
#define HAL_FLASH_LOCK_BITS            16
#define HAL_NV_PAGE_END                121 

#else
#define HAL_FLASH_LOCK_BITS            16
#define HAL_NV_PAGE_END                126
#endif

5. Project->Clean and Make

