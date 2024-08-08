/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include <app_x-cube-ai.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "main.h"
#include "its.h"
#include "cryp.h"
#include "fwu.h"
#include "eat.h"
#include "cert.h"

#include "ecdsa.h"
#include "sha256.h"
#include "platform.h"

#include "priv_defines.h"
#include "stm32_lcd.h"
#include "tflm_c.h"

#include "its.h"
#include "psa/internal_trusted_storage.h"
#include "cryp.h"
#include "crypto_tests_common.h"

static GPIO_InitTypeDef  GPIO_InitStruct;

/** @addtogroup STM32H5xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

#define ML_MODEL_PROOF_SIZE   (33U)
#define ML_CHALLANGE_SIZE     (16U)
#define ML_ECDSA_ATTEST_KEY_IDX      (0x46U) // as in ITS_BLOB.bat
#define ML_HASH_SIZE          (32U)
#define ML_SIGNATURE_SIZE     (64U)

#if defined(__ICCARM__)
#include <LowLevelIOInterface.h>
#endif /* __ICCARM__ */

#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int32_t iar_fputc(int32_t ch);
#define PUTCHAR_PROTOTYPE int32_t iar_fputc(int32_t ch)
#elif defined ( __CC_ARM ) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6*/
int io_putchar(int ch);
#define PUTCHAR_PROTOTYPE int io_putchar(int ch)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int32_t uart_putc(int32_t ch)
#endif /* __ICCARM__ */

PUTCHAR_PROTOTYPE
{
  (void)COM_Transmit((uint8_t *)&ch, 1, TX_TIMEOUT);
  return ch;
}

/* Redirects printf to TFM_DRIVER_STDIO in case of ARMCLANG*/
#if defined(__ARMCC_VERSION)
FILE __stdout;

/* __ARMCC_VERSION is only defined starting from Arm compiler version 6 */
int fputc(int ch, FILE *f)
{
  /* Send byte to USART */
  io_putchar(ch);

  /* Return character written */
  return ch;
}
#elif defined(__GNUC__)
/* Redirects printf to TFM_DRIVER_STDIO in case of GNUARM */
int32_t _write(int32_t fd, char *str, int32_t len)
{
  int32_t i;

  for (i = 0; i < len; i++)
  {
    /* Send byte to USART */
    uart_putc(str[i]);
  }

  /* Return the number of characters written */
  return len;
}
#elif defined(__ICCARM__)
size_t __write(int32_t file, unsigned char const *ptr, size_t len)
{
  UNUSED(file);
  size_t idx;
  unsigned char const *pdata = ptr;

  for (idx = 0; idx < len; idx++)
  {
    (void)iar_fputc((int32_t)*pdata);
    pdata++;
  }
  return len;
}

#endif /*  __GNUC__ */

/* Global variables ---------------------------------------------------------*/
extern const uint8_t g_tflm_network_model_data[];
extern const int g_tflm_network_model_data_len;

/* Private variables ---------------------------------------------------------*/
uint8_t *p_UserAppId;
const uint8_t UserAppId = 'A';
TS_Init_t   hTS;

/* ML Attestation */
/* Compressed private point on the curve that represents the encoded model weights. */
unsigned char R_bytes[33]; // 33 bytes: 1 byte for prefix and 64 bytes for the point
/* SHA256 of the TFLITE model. */
unsigned char hash[32];

/* Private function prototypes -----------------------------------------------*/
static void FW_APP_MAIN_PrintMenu(void);
static void FW_APP_MAIN_Run(void);
static void SystemClock_Config(void);
static void CACHE_Enable(void);
static void Error_Handler(void);
static void MX_GPIO_Init(void);

/* ML Attestation */
static void ML_Attestation_ComputeCurrentModelProof(void);
static bool ML_Attestation_Validate_Model_Proof(uint8_t *pModelProof);
static void ML_Attestation_Model_Attestation(void);
static bool ML_Attestation_Generate_Model_Proof(uint8_t *pChallange);
static void ML_Attestation_GetDevicePublicKey(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*  set example to const : this const changes in binary without rebuild */
  p_UserAppId = (uint8_t *)&UserAppId;

  /* STM32H5xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  if (HAL_Init() != HAL_OK)
  {
    Error_Handler();
  }

  /* This project template calls CACHE_Enable() in order to enable the Instruction
     and Data Cache. This function is provided as template implementation that
     User may integrate in his application in order to enhance performance */

  /* Enable the Instruction Cache */
  CACHE_Enable();

  /* Configure the System clock to have a frequency of 250 MHz */
  SystemClock_Config();

  MX_GPIO_Init();

//
//  /* -1- Enable GPIO Clock (to be able to program the configuration registers) */
//   LED1_GPIO_CLK_ENABLE();
//   LED2_GPIO_CLK_ENABLE();
//   LED3_GPIO_CLK_ENABLE();
//   LED4_GPIO_CLK_ENABLE();
//
//   /* -2- Configure IO in output push-pull mode to drive external LEDs */
//   GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
//   GPIO_InitStruct.Pull  = GPIO_NOPULL;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//
//   GPIO_InitStruct.Pin = LED1_PIN;
//   HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
//
//   GPIO_InitStruct.Pin = LED2_PIN;
//   HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
//
//   GPIO_InitStruct.Pin = LED3_PIN;
//   HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);
//
//   GPIO_InitStruct.Pin = LED4_PIN;
//   HAL_GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStruct);
//
//   /* Initialize LCD */
//   BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
//   /* Set GUI functions */
//   UTIL_LCD_SetFuncDriver(&LCD_Driver);
//   UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
//
//   hTS.Orientation = TS_ORIENTATION_LANDSCAPE;
//   hTS.Accuracy = 5;
//   hTS.Width = 240;
//   hTS.Height = 240;
//
//   BSP_TS_Init(0, &hTS);
//   BSP_TS_EnableIT(0);

  /* Configure Communication module */
  if (COM_Init() != HAL_OK)
  {
    Error_Handler();
  }

  /* Compute the hash of the model data in the init section */
  mbedtls_sha256(g_tflm_network_model_data, g_tflm_network_model_data_len, hash, 0);

  ML_Attestation_ComputeCurrentModelProof();

  size_t data_length = ML_MODEL_PROOF_SIZE;
  uint8_t dataout[ML_MODEL_PROOF_SIZE] = {0U};

  (void)psa_its_get(0x40, 0u, data_length, (void *)&dataout, &data_length);
  /* Check the current model amprent is the same as the one stored in ITS */
  if(memcmp(dataout, R_bytes, ML_MODEL_PROOF_SIZE) != 0)
  {
    /* Do not allow any other model to run.
     * E.g Now the model in embedded in flash and signed. But let's suppose that the model is loaded from SD card and it is not authenticated. */
    Error_Handler();
  }

  //MX_X_CUBE_AI_Init();

  //MX_X_CUBE_AI_Process();

  /* Add your application code here */
  HAL_Delay(100);

  FW_APP_MAIN_Run();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Display the Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
void FW_APP_MAIN_PrintMenu(void)
{
  (void)printf("\r\n=================== ML model attestation ============================\r\n\r\n");
  (void)printf("  Start ML Attestation process ----------------------------- 1\r\n\r\n");
  (void)printf("  Get device public key ------------------------------------------- 2\r\n\r\n");
  (void)printf("  Selection :\r\n\r\n");
  (void)printf("  ");
}

/**
  * @brief  Run the Main menu
  * @param  None.
  * @retval None.
  */
void FW_APP_MAIN_Run(void)
{
  uint8_t key = 0U;

  /* Print Menu message*/
  FW_APP_MAIN_PrintMenu();

  while (1U)
  {
    /* Clean the input path */
    (void)COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, RX_TIMEOUT) == HAL_OK)
    {
      (void)printf("%c\r\n", key);
      switch (key)
      {
        case '1' :
          ML_Attestation_Model_Attestation();
          break;
        case '2' :
          ML_Attestation_GetDevicePublicKey();
          break;
        case '3':
        default:
          (void)printf("\rInvalid Number !\r\n");
          break;
      }

      /* Print Main Menu message */
      FW_APP_MAIN_PrintMenu();
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 250000000  (CPU Clock)
  *            HCLK(Hz)                       = 250000000  (Bus matrix and AHBs Clock)
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1 (APB1 Clock  250MHz)
  *            APB2 Prescaler                 = 1 (APB2 Clock  250MHz)
  *            APB3 Prescaler                 = 1 (APB3 Clock  250MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 100
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  if (HAL_RCC_DeInit() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Use HSE in bypass mode and activate PLL with HSE as source. Enable HSI48. */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS_DIGITAL;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* Select PLL as system clock source and configure the HCLK, PCLK1, PCLK2 and PCLK3
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                 RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/* Compute H(w)*G on the secp256k1 curve. */
static void ML_Attestation_ComputeCurrentModelProof(void)
{
  int ret;
  mbedtls_ecp_group group;
  mbedtls_ecp_point P;
  mbedtls_mpi k;
  mbedtls_ecp_point R;

  mbedtls_ecp_group_init(&group);
  mbedtls_ecp_point_init(&P);
  mbedtls_mpi_init(&k);
  mbedtls_ecp_point_init(&R);

  // 2. Set up the SECP256k1 curve
  ret = mbedtls_ecp_group_load(&group, MBEDTLS_ECP_DP_SECP256K1);
  if (ret != 0) {
      mbedtls_printf("Failed to load SECP256k1 curve: -0x%04x\n", -ret);
      return 1;
  }

  // 3. Load the generator point (G)
  ret = mbedtls_ecp_copy(&P, &group.G); // Group.G is the generator point
  if (ret != 0) {
      mbedtls_printf("Failed to copy generator point: -0x%04x\n", -ret);
      return 1;
  }

  // Convert hash to mbedtls_mpi (scalar)
  ret = mbedtls_mpi_read_binary(&k, hash, sizeof(hash));
  if (ret != 0) {
      mbedtls_printf("Failed to convert hash to MPI: -0x%04x\n", -ret);
      return 1;
  }

  // 5. Perform scalar multiplication (k * G)
  ret = mbedtls_ecp_mul(&group, &R, &k, &P, NULL, NULL);
  if (ret != 0) {
      mbedtls_printf("Failed to multiply scalar with point: -0x%04x\n", -ret);
      return 1;
  }

  // 6. Print the result
  size_t olen;

  ret = mbedtls_ecp_point_write_binary(&group, &R, MBEDTLS_ECP_PF_COMPRESSED, &olen, R_bytes, sizeof(R_bytes));
  if (ret != 0) {
      mbedtls_printf("Failed to write point to binary: -0x%04x\n", -ret);
      return 1;
  }

  // Clean up
  mbedtls_ecp_group_free(&group);
  mbedtls_ecp_point_free(&P);
  mbedtls_mpi_free(&k);
  mbedtls_ecp_point_free(&R);
}

static bool ML_Attestation_Validate_Model_Proof(uint8_t *pModelProof)
{
  size_t data_length = ML_MODEL_PROOF_SIZE;
  psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
  uint8_t dataout[ML_MODEL_PROOF_SIZE] = {0U};

  psa_status = psa_its_get(0x40, 0u, data_length, (void *)&dataout, &data_length);
  if(psa_status != PSA_SUCCESS)
  {
    (void)printf("\r\nNo provisioned model.\r\n");
    return false;
  }
  else
  {
    /* Check that received data is the same as in the internal storage. */
    if(memcmp(dataout, pModelProof, ML_MODEL_PROOF_SIZE) == 0)
    {
      /* POC: check if current model matches. */
      (void)printf("\r\nModel proof is valid.\r\n");
      return true;
    }
    else
    {
      (void)printf("\r\nModel proof is invalid.\r\n");
      return false;
    }
  }
}

static void ML_Attestation_EcdsaSignChallange
(
  const uint8_t *pBlock, uint16_t blockLen, psa_key_id_t id_key, uint8_t *pSignature, size_t *pSignatureLen
)
{
  psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
  psa_algorithm_t alg = PSA_ALG_ECDSA(PSA_ALG_SHA_256);
  uint8_t signature[ML_SIGNATURE_SIZE] = {0U};
  size_t signature_length = 0x0U;
  uint8_t data[1 + 2 * PSA_BITS_TO_BYTES(256)] = {0};
  size_t data_size = 1 + 2 * PSA_BITS_TO_BYTES(256);
  size_t data_length = 0;

  if (pBlock == NULL || pSignature == NULL || pSignatureLen == NULL)
  {
    return;
  }

  if (*pSignatureLen < ML_SIGNATURE_SIZE)
  {
    return;
  }
  psa_status = psa_sign_message(id_key, alg, pBlock, blockLen, &signature[0], ML_SIGNATURE_SIZE, &signature_length);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rGenerating attestation...\r\n") : (void)printf("\nService not available:%d\r\n", (int)psa_status);

  memcpy(pSignature, signature, signature_length);
  *pSignatureLen = signature_length;

}

static void ML_Attestation_GetDevicePublicKey(void)
{
  psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
  uint8_t data[1 + 2 * PSA_BITS_TO_BYTES(256)] = {0};
  size_t data_size = 1 + 2 * PSA_BITS_TO_BYTES(256);
  size_t data_length = 0;

  psa_status = psa_export_public_key(ML_ECDSA_ATTEST_KEY_IDX, &data[0], data_size, &data_length);
  if(psa_status != PSA_SUCCESS)
  {
    (void)printf("\r\nDevice does not support attestation..\r\n");
    return false;
  }

  printf("Public key:\n");
  for(uint8_t i = 0; i < data_length; i++)
  {
    printf("%02X", data[i]);
  }
}

static bool ML_Attestation_Generate_Model_Proof(uint8_t *pChallange)
{
    uint8_t signature[ML_SIGNATURE_SIZE] = {0U};
    size_t sigLen = ML_SIGNATURE_SIZE;

    ML_Attestation_EcdsaSignChallange(pChallange, ML_CHALLANGE_SIZE + ML_HASH_SIZE, ML_ECDSA_ATTEST_KEY_IDX, signature, &sigLen);

    printf("Signature:\n");
    for(uint8_t i = 0; i < sigLen; i++)
    {
        printf("%02X", signature[i]);
    }
    printf("\n\n\n\n");
}


static void ML_Attestation_Model_Attestation(void)
{
  uint8_t challange[ML_CHALLANGE_SIZE + ML_MODEL_PROOF_SIZE] = {0U};
  uint8_t *pModelProof = &challange[ML_CHALLANGE_SIZE];
  bool model_proof_valid = false;

  (void)COM_Flush();

  printf("Please enter the challange:\n");

  /* Receive challange. Wait 20 seconds. */
  if (COM_Receive(&challange, ML_CHALLANGE_SIZE + ML_MODEL_PROOF_SIZE, 10 * RX_TIMEOUT) == HAL_OK)
  {
    printf("Starting the authentication process...\n");
    model_proof_valid = ML_Attestation_Validate_Model_Proof(pModelProof);
    if(model_proof_valid != true)
    {
      (void)printf("\r\nRequest denied.\r\n");
      return;
    }

    /* Construct payload to be signed. */
    memcpy(pModelProof, hash, ML_HASH_SIZE);
    ML_Attestation_Generate_Model_Proof(challange);
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, ARD_D2_Pin|ARD_D8_Pin|LCD_CTP_RST_Pin|ARD_D7_Pin
                          |ARD_D4_Pin|UCPD_PWR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, MEMS_LED_Pin|DETECTN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STMOD_17_GPIO_Port, STMOD_17_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, STMOD_11_INT_Pin|STMOD_14_PWM_Pin|STMOD_20_Pin|STMOD_12_Pin
                          |STMOD_19_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ARD_D3_Pin */
  GPIO_InitStruct.Pin = ARD_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(ARD_D3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D2_Pin ARD_D8_Pin LCD_CTP_RST_Pin ARD_D7_Pin
                           ARD_D4_Pin UCPD_PWR_Pin */
  GPIO_InitStruct.Pin = ARD_D2_Pin|ARD_D8_Pin|LCD_CTP_RST_Pin|ARD_D7_Pin
                          |ARD_D4_Pin|UCPD_PWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : SAI2_SD_B_Pin */
  GPIO_InitStruct.Pin = SAI2_SD_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_SAI2;
  HAL_GPIO_Init(SAI2_SD_B_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D13_Pin ARD_D12_Pin */
  GPIO_InitStruct.Pin = ARD_D13_Pin|ARD_D12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : MEMS_LED_Pin DETECTN_Pin */
  GPIO_InitStruct.Pin = MEMS_LED_Pin|DETECTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : I2C1_SCL_Pin I2C1_SDA_Pin */
  GPIO_InitStruct.Pin = I2C1_SCL_Pin|I2C1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_TXD1_Pin RMII_TX_EN_Pin */
  GPIO_InitStruct.Pin = RMII_TXD1_Pin|RMII_TX_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_SAI1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_FS_P_Pin USB_FS_N_Pin */
  GPIO_InitStruct.Pin = USB_FS_P_Pin|USB_FS_N_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_USB;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : RMII_RX_ER_Pin */
  GPIO_InitStruct.Pin = RMII_RX_ER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(RMII_RX_ER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D9_Pin */
  GPIO_InitStruct.Pin = ARD_D9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(ARD_D9_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : STMOD_17_Pin */
  GPIO_InitStruct.Pin = STMOD_17_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STMOD_17_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
  GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin RMII_CRS_DV_Pin */
  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin|RMII_MDIO_Pin|RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : STMOD_18_Pin */
  GPIO_InitStruct.Pin = STMOD_18_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(STMOD_18_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_A1_Pin ARD_A2_Pin ARD_A4_Pin ARD_A3_Pin */
  GPIO_InitStruct.Pin = ARD_A1_Pin|ARD_A2_Pin|ARD_A4_Pin|ARD_A3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D1_Pin ARD_D0_Pin */
  GPIO_InitStruct.Pin = ARD_D1_Pin|ARD_D0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : STMOD_11_INT_Pin STMOD_14_PWM_Pin STMOD_20_Pin STMOD_12_Pin
                           STMOD_19_Pin */
  GPIO_InitStruct.Pin = STMOD_11_INT_Pin|STMOD_14_PWM_Pin|STMOD_20_Pin|STMOD_12_Pin
                          |STMOD_19_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D11_Pin */
  GPIO_InitStruct.Pin = ARD_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(ARD_D11_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_SAI1_SD3_Pin */
  GPIO_InitStruct.Pin = PDM_SAI1_SD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_SAI1;
  HAL_GPIO_Init(PDM_SAI1_SD3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D10_Pin */
  GPIO_InitStruct.Pin = ARD_D10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(ARD_D10_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_A5_Pin IBUS_SENSE_Pin STMOD_13_Pin VBUS_SENSE_Pin */
  GPIO_InitStruct.Pin = ARD_A5_Pin|IBUS_SENSE_Pin|STMOD_13_Pin|VBUS_SENSE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : STMOD_9_MISOs_Pin STMOD_12_RST_Pin */
  GPIO_InitStruct.Pin = STMOD_9_MISOs_Pin|STMOD_12_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D6_Pin ARD_D5_Pin */
  GPIO_InitStruct.Pin = ARD_D6_Pin|ARD_D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : UCPD_CC2_Pin ARD_A0_Pin UCPD_CC1_Pin */
  GPIO_InitStruct.Pin = UCPD_CC2_Pin|ARD_A0_Pin|UCPD_CC1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : UCPD_FLT_Pin */
  GPIO_InitStruct.Pin = UCPD_FLT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(UCPD_FLT_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/**
  * @brief  Enable ICACHE with 2-ways set-associative configuration.
  * @param  None
  * @retval None
  */
static void CACHE_Enable(void)
{
  /* Enable instruction cache (default 2-ways set associative cache) */
  (void)HAL_ICACHE_Enable();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *p_file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */
