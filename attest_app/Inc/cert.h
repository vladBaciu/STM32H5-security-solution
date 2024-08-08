/**
  ******************************************************************************
  * @file    cert.h
  * @author  MCD Application Team
  * @brief   Header for cert.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CERT_H
#define CERT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FW_APP_CERT_Run(void);

#endif /* CERT_H */