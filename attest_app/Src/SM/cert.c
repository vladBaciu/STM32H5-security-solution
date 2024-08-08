/**
  ******************************************************************************
  * @file    cert.c
  * @author  MCD Application Team
  * @brief   NS Certificate DUA
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
#include "cert.h"
#include "com.h"
#include "stm32_cert.h"
#include "mbedtls/base64.h"

/* Private typedef -----------------------------------------------------------*/
#define MAX_LENGTH_LINE 64U
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void FW_APP_CERT_PrintMenu(void);
static void FW_APP_CERT_Test(const CERT_Id_t certificate_id);
static void FW_APP_CERT_Print(const char* pem_string, const size_t pem_cert_size);

/**
  * @brief  Display the CERT Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void FW_APP_CERT_PrintMenu(void)
{
  (void)printf("\r\n=============== Certificates Menu ==================\r\n\r\n");
  (void)printf("  DUA USER X509 certificate ---------------------------- 1\r\n\r\n");
  (void)printf("  DUA Initial Attestation X509 certificate ------------- 2\r\n\r\n");
  (void)printf("  Previous menu ---------------------------------------- x\r\n\r\n");
  (void)printf("  Selection :\r\n\r\n");
  (void)printf("  ");
}

/**
  * @brief  Run the CERTIFICATE example
  * @param  None.
  * @retval None.
  */
void FW_APP_CERT_Run(void)
{
  uint8_t key = 0U;
  uint8_t exit = 0U;

  /* Print Menu message*/
  FW_APP_CERT_PrintMenu();

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
          /* Test Certificates utility for DUA USER  */
          (void)printf("\r---DUA USER Certificate example---\r\n\r\n");
          FW_APP_CERT_Test(DUA_USER);
          break;

        case '2':
          /* Test Certificates utilitu for DUA Initial Attestation  */
          (void)printf("\r---DUA Initial Attestation Certificate example---\r\n\r\n");
          FW_APP_CERT_Test(DUA_INITIAL_ATTEST);
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
        FW_APP_CERT_PrintMenu();
      }
    }
  }
}

static void FW_APP_CERT_Test(const CERT_Id_t certificate_id)
{
  uint8_t chip_certificate[DUA_PUBLIC_PART_MAX_SIZE_BYTES]={0};
  CERT_Status_t cert_status = CERT_HW_ERROR;
  uint32_t certificate_size = 0U;
  uint32_t pem_string_len = 0;
  unsigned char pem_string[3000];
  size_t pem_cert_len = 0;

  cert_status = UTIL_CERT_GetCertificateSize(certificate_id, &certificate_size);
  (void)printf("\rUTIL_CERT_GetCertificateSize : \r\n\r\n");
  if (cert_status == CERT_OK)
  {
    (void)printf("\rTEST PASSED\r\n\r\n \r Certificate Size : %d \r\n\r\n", (int)certificate_size);
  }
  else
  {
    (void)printf("\rTEST FAILED:%d\r\n", (int)cert_status);
  }
  cert_status = UTIL_CERT_GetCertificate(certificate_id, chip_certificate);
  (void)printf("\rUTIL_CERT_GetCertificate : \r\n\r\n");
  if (cert_status == CERT_OK)
  {
    (void)printf("\rTEST PASSED\r\n\r\n");
    (void)printf("\rTo visualize the certificate in a terminal : openssl x509 -in certificate.pem -text\r\n\r\n");
    pem_string_len = sizeof(pem_string) - 1;
    if (mbedtls_base64_encode(pem_string, pem_string_len, &pem_cert_len, (unsigned char *) chip_certificate, certificate_size) != 0 )
    {
      (void)printf("\rError : Buffer to small to convert the certificate in pem format \r\n\r\n");
    }
    else
    {
      FW_APP_CERT_Print((const char*)&pem_string[0], pem_cert_len);
    }
  }
  else
  {
    (void)printf("\rTEST FAILED:%d\r\n", (int)cert_status);
  }
}

static void FW_APP_CERT_Print(const char* pem_string, const size_t pem_cert_size)
{
    uint32_t len = (uint32_t)pem_cert_size;
    uint32_t iter_max = len / MAX_LENGTH_LINE;
    uint32_t div_remainder = len % MAX_LENGTH_LINE;
    uint32_t i = 0;
    (void)printf("-----BEGIN CERTIFICATE-----\r\n");
    while (i < iter_max) {
      (void)printf("%.*s\r\n", MAX_LENGTH_LINE, pem_string+MAX_LENGTH_LINE*i);
      i++;
    }
    if (div_remainder != 0U)
    {
      (void)printf("%s\r\n", &pem_string[len-div_remainder]);
    }
    (void)printf("-----END CERTIFICATE-----\r\n");
}