/*********************************************************************************************************//**
 * @file    i2c_eeprom.h
 * @version $Rev:: 214          $
 * @date    $Date:: 2015-03-31 #$
 * @brief   The header file of i2c_eeprom.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) 2014 Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32_board.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup I2C_EEPROM I2C_EEPROM
  * @{
  */


/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup I2C_EEPROM_Exported_Constants I2C EEPROM exported constants
  * @{
  */
#define I2C_EEPROM_SPEED         (400000)                                       /*!< I2C speed              */
#define I2C_EEPROM_CAPACITY      (4096)                                         /*!< capacity in bytes      */
#define I2C_EEPROM_PAGE_SIZE     (32)                                           /*!< page size in bytes     */
#define I2C_EEPROM_ADDR_MASK     (I2C_EEPROM_CAPACITY - 1)                      /*!< address mask           */
#define I2C_EEPROM_PAGE_MASK     (I2C_EEPROM_PAGE_SIZE - 1)                     /*!< page size mask         */
#define I2C_EEPROM_DEV_ADDR      (0x50)                                         /*!< I2C device address     */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup I2C_EEPROM_Exported_Functions I2C EEPROM exported functions
  * @{
  */
void I2C_EEPROM_Init(void);
void I2C_EEPROM_PageWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EEPROM_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EEPROM_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);
/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_EEPROM_H -----------------------------------------------------------------------------------*/
