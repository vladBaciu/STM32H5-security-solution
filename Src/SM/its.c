/**
  ******************************************************************************
  * @file    its.c
  * @author  MCD Application Team
  * @brief   NS ITS program
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

/* Includes ------------------------------------------------------------------*/
#include "com.h"
#include "its.h"
#include "psa/internal_trusted_storage.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DATA_LENGTH          33U // secpk1 compressed key length
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void FW_APP_ITS_PrintMenu(void);

/**
  * @brief  Display the ITS Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void FW_APP_ITS_PrintMenu(void)
{
  (void)printf("\r\n============= Internal Trusted Storage Menu ==============\r\n\r\n");
  (void)printf("  Factory data are provisioned during SM installation and are not updatable\r\n\r\n");
  (void)printf("  New data: Set ---------------------------------------- 1\r\n\r\n");
  (void)printf("  New data: Get ---------------------------------------- 2\r\n\r\n");
  (void)printf("  New data: Get info ----------------------------------- 3\r\n\r\n");
  (void)printf("  New data: Remove ------------------------------------- 4\r\n\r\n");
  (void)printf("  Factory data: Set ------------------------------------ 5\r\n\r\n");
  (void)printf("  Factory data: Get ------------------------------------ 6\r\n\r\n");
  (void)printf("  Factory data: Get info ------------------------------- 7\r\n\r\n");
  (void)printf("  Factory data: Remove --------------------------------- 8\r\n\r\n");
  (void)printf("  Previous menu ---------------------------------------- x\r\n\r\n");
  (void)printf("  Selection :\r\n\r\n");
  (void)printf("  ");
}

/**
  * @brief  Run the ITS example
  * @param  None.
  * @retval None.
  */
void FW_APP_ITS_Run(void)
{
  uint8_t key = 0U;
  uint8_t exit = 0U;
  psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
  size_t data_length = DATA_LENGTH;
  uint8_t datain[DATA_LENGTH] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11,
                                 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
  uint8_t dataout[DATA_LENGTH];

  psa_storage_create_flags_t flags = PSA_STORAGE_FLAG_NO_CONFIDENTIALITY;
  struct psa_storage_info_t  storage_info;

  /* Print Menu message*/
  FW_APP_ITS_PrintMenu();

  while (exit == 0U)
  {
    /* Clean the input path */
    (void)COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, RX_TIMEOUT) == HAL_OK)
    {
      (void)printf("%c\r\n", key);
      switch (key)
      {
        case '6' :
          psa_status = psa_its_get(0x40, 0u, data_length, (void *)&dataout, &data_length);
          (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
          if (psa_status == PSA_SUCCESS)
          {
            (void)printf("\r\nData :\r\n");
            for (size_t i = 0; i < DATA_LENGTH; i++)
            {
              (void)printf("%02x", (uint8_t)dataout[i]);
            }
            (void)printf("\r\n");
          }
          break;

        case 'X' :
        case 'x' :
          exit = 1U;
          break;

        default:
          (void)printf("\r\nInvalid Number !\r\n");
          break;
      }

      if (exit == 0U)
      {
        /* Print Main Menu message */
        FW_APP_ITS_PrintMenu();
      }
    }
  }
}
