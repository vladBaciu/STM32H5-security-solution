/**
  ******************************************************************************
  * @file    eat.c
  * @author  MCD Application Team
  * @brief   NS EAT program
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
#include "eat.h"
#include "com.h"
#include "psa/initial_attestation.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t TokenBuf[2048];
static uint8_t AuthChallenge[] = { \
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                                  };
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void FW_APP_EAT_PrintMenu(void);
static void FW_APP_EAT_DumpEatToken(uint8_t *token, size_t token_size);

/**
  * @brief  Display the EAT Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void FW_APP_EAT_PrintMenu(void)
{
  (void)printf("\r\n============== Initial Attestation Menu ==================\r\n\r\n");
  (void)printf("  Token ------------------------------------------------ 1\r\n\r\n");
  (void)printf("  Previous menu ---------------------------------------- x\r\n\r\n");
  (void)printf("  Selection :\r\n\r\n");
  (void)printf("  ");
}

/**
  * @brief  Run the EAT example
  * @param  None.
  * @retval None.
  */
void FW_APP_EAT_Run(void)
{
  uint8_t key = 0U;
  uint8_t exit = 0U;
  psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;

  size_t chall_size     = 32;
  size_t token_buf_size = 0U;
  size_t token_size     = 0U;

  /* Print Menu message*/
  FW_APP_EAT_PrintMenu();

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
        case '1':
          (void)printf(" ---- To decode the token : scripts are available in \r\n");
          (void)printf("   'Utilities >> PC_Software >> IAT_Verifier >> st_tools' \r\n");
          (void)printf("\r\nPSA API tested : \r\n");
          (void)printf("psa_initial_attest_get_token_size() \r\n");
          (void)printf("psa_initial_attest_get_token() \r\n");
          psa_status = psa_initial_attest_get_token_size(chall_size, &token_buf_size);
          (void)printf("\r\n Get token size ");
          (psa_status == PSA_SUCCESS) ? (void)printf("SUCCESS\r\n") : (void)printf("FAILED: %d\r\n", (int)psa_status);
          psa_status = psa_initial_attest_get_token(AuthChallenge, chall_size, TokenBuf, token_buf_size, &token_size);
          (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
          if (psa_status == PSA_SUCCESS)
          {
            (void)printf("\r\nToken Response: \r\n");
            FW_APP_EAT_DumpEatToken(TokenBuf, token_buf_size);
          }
          break;

        case 'X' :
        case 'x' :
          exit = 1U;
          break;

        default :
          (void)printf("\rInvalid Number !\r\n");
          break;
      }

      if (exit == 0U)
      {
        /* Print Main Menu message */
        FW_APP_EAT_PrintMenu();
      }
    }
  }
}

static void FW_APP_EAT_DumpEatToken(uint8_t *p_token, size_t token_size)
{
  int32_t len = (int32_t)token_size;
  int32_t n_item_per_line;
  int32_t i;
  int32_t index = 0;
  uint8_t *p_byte = (uint8_t *)p_token;
  while (index < len)
  {
    n_item_per_line = ((len - index) >= (int32_t)20) ? 20 : (len - index);
    for (i = 0; i < n_item_per_line; i++)
    {
      (void)printf("%2.2x", p_byte[index + i]);
    }
    (void)printf("\r\n");
    index += n_item_per_line;

  }
  (void)printf("\r\n");

}
