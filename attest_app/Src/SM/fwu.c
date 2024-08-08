/**
  ******************************************************************************
  * @file    fwu.c
  * @author  MCD Application Team
  * @brief   NS FWU program
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
#include "fwu.h"
#include "com.h"
#include "common.h"
#include "ymodem.h"
#include "string.h"
#include "psa/update.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ARM_DRIVER_OK 0 /* < Operation succeeded */
#define IMAGE_SIZE_MAX 0xA0000
#define FLASH0_PROG_UNIT 0x10
#define IMAGE_ID(slot,type) (((uint8_t)slot) | (type << 8));

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t m_uFileSizeYmodem = 0U;    /* !< Ymodem File size*/
static uint32_t m_uNbrBlocksYmodem = 0U;   /* !< Ymodem Number of blocks*/
static uint32_t m_uPacketsReceived = 0U;   /* !< Ymodem packets received*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void FW_APP_FWU_PrintMenu(void);
static void FW_APP_FWU_DownloadImage(psa_image_id_t image_id);
static void FW_APP_FWU_InstallImage(psa_image_id_t image_id);
static HAL_StatusTypeDef FW_APP_FWU_DownloadNewFirmware(uint32_t uMaxFwSize, psa_image_id_t image_id);
static void FW_APP_FWU_QueryAll(void);
static void FW_APP_FWU_Validation(psa_image_id_t image_id);
static void FW_APP_FWU_Abort(psa_image_id_t image_id);
static void print_image_info(psa_image_info_t info, psa_status_t psa_status);


/**
  * @brief  Display the FWU Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void FW_APP_FWU_PrintMenu(void)
{
  (void)printf("\r\n================= Firmware Update Menu ===================\r\n\r\n");
  (void)printf("  Get all applications Status -------------------------- 1\r\n\r\n");
  (void)printf("  Install all requested Applications (reboot) ---------- 2\r\n\r\n");
  (void)printf("  Download Non-Secure App ------------------------------ 3\r\n\r\n");
  (void)printf("  Request Install Non-Secure App ----------------------- 4\r\n\r\n");
  (void)printf("  Accept Non-Secure App -------------------------------- 5\r\n\r\n");
  (void)printf("  Abort Download Non Secure App ------------------------ 6\r\n\r\n");
  (void)printf("  Download Secure Manager ------------------------------ 7\r\n\r\n");
  (void)printf("  Request Install Secure Manager ----------------------- 8\r\n\r\n");
  (void)printf("  Accept Secure Manager -------------------------------- 9\r\n\r\n");
  (void)printf("  Abort Download Secure Manager ------------------------ A\r\n\r\n");
  (void)printf("  Download STuRoT -------------------------------------- B\r\n\r\n");
  (void)printf("  Request Install STuRoT --------------------------------C\r\n\r\n");
  (void)printf("  Abort Download STuRoT -------------------------------- D\r\n\r\n");
#if NB_MODULE > 0
  (void)printf("  Download Module 0 ------------------------------------ E\r\n\r\n");
  (void)printf("  Request Install Module 0 ----------------------------- F\r\n\r\n");
  (void)printf("  Accept Module 0 -------------------------------------- G\r\n\r\n");
  (void)printf("  Abort Download Module 0 ------------------------------ H\r\n\r\n");
#endif /* NB_MODULE > 0 */
  (void)printf("  Previous menu ---------------------------------------- x\r\n\r\n");
  (void)printf("  Selection :\r\n\r\n");
  (void)printf("  ");
}

/**
  * @brief  Run the FWU example
  * @param  None.
  * @retval None.
  */
void FW_APP_FWU_Run(void)
{
  uint8_t key = 0U;
  uint8_t exit = 0U;

  psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
  uint32_t image_id = 0U;

  /* Print Menu message*/
  FW_APP_FWU_PrintMenu();

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
          /* Display status of all code images (active and download slot) */
          FW_APP_FWU_QueryAll();
          break;
        case '2':
          /* Installation of images in pending install (swap) by sturot */
          (void)printf("\r\nPSA API tested : \r\n");
          (void)printf("psa_fwu_request_reboot() \r\n");
          psa_status = psa_fwu_request_reboot();
          (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
          break;
        case '3':
          /* Download Non Secure image in download slot */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_NONSECURE);
          FW_APP_FWU_DownloadImage(image_id);
          break;
        case '4':
          /* Request install Non Secure image in download slot */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_NONSECURE);
          FW_APP_FWU_InstallImage(image_id);
          break;
        case '5':
          /* Confirm Non Secure image */
          image_id =IMAGE_ID(PSA_FWU_SLOT_ID_ACTIVE, FWU_IMAGE_TYPE_NONSECURE);
          FW_APP_FWU_Validation(image_id);
          break;
        case '6':
          /* Erase Non Secure Secondary slot after download only */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_NONSECURE);
          FW_APP_FWU_Abort(image_id);
          break;
        case '7':
          /* Download Secure Manager image in download slot */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_SECURE);
          FW_APP_FWU_DownloadImage(image_id);
          break;
        case '8':
          /* Request install Secure Manager image in download slot */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_SECURE);
          FW_APP_FWU_InstallImage(image_id);
          break;
        case '9':
          /* Confirm Secure Manager image */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_ACTIVE, FWU_IMAGE_TYPE_SECURE);
          FW_APP_FWU_Validation(image_id);
          break;
        case 'a':
        case 'A':
          /* Erase Secure Manager Secondary slot after download only */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_SECURE);
          FW_APP_FWU_Abort(image_id);
          break;
        case 'b':
        case 'B':
          /* Download STuRoT image in download slot */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_BL);
          FW_APP_FWU_DownloadImage(image_id);
          break;
        case 'c':
        case 'C':
          /* Request install STuRoT image in download slot */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_BL);
          FW_APP_FWU_InstallImage(image_id);
          break;
        case 'd':
        case 'D':
          /* Erase STuRoT Secondary slot after download only */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_BL);
          FW_APP_FWU_Abort(image_id);
          break;
#if NB_MODULE > 0
        case 'e':
        case 'E':
          /* Download Module0 image in download slot */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_MODULE_0);
          FW_APP_FWU_DownloadImage(image_id);
          break;
        case 'f':
        case 'F':
          /* Request install Module0 image in download slot */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_MODULE_0);
          FW_APP_FWU_InstallImage(image_id);
          break;
        case 'g':
        case 'G':
          /* Confirm Module0 image */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_ACTIVE, FWU_IMAGE_TYPE_MODULE_0);
          FW_APP_FWU_Validation(image_id);
          break;
        case 'h':
        case 'H':
          /* Erase Module0 Secondary slot after download only */
          image_id = IMAGE_ID(PSA_FWU_SLOT_ID_DL, FWU_IMAGE_TYPE_MODULE_0);
          FW_APP_FWU_Abort(image_id);
          break;
#endif /* NB_MODULE > 0 */

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
        FW_APP_FWU_PrintMenu();
      }
    }
  }
}

/**
  * @brief  print image info
  * @param  psa_image_info_t info
  * @retval None.
  */
static void print_image_info(psa_image_info_t info, psa_status_t psa_status)
{
  if ((psa_status == PSA_SUCCESS) && (info.state != PSA_IMAGE_UNDEFINED))
  {
    (void)printf("State = 0x%x\r\n", info.state);
    (void)printf("Image ID = 0x%x\r\n", (int)info.image_id);
    if ((info.version.iv_build_num == 0U) || (info.version.iv_build_num == 0xFFFFFFFF))
    {
    (void)printf("Version (Major.Minor.Revision) = %d.%d.%d\r\n", \
                  info.version.iv_major, info.version.iv_minor, info.version.iv_revision);
    }
    else
    {
      (void)printf("Version (Major.Minor.Revision.BuildNum) = %d.%d.%d.%d\r\n", \
                    info.version.iv_major, info.version.iv_minor, info.version.iv_revision, \
                    (int)info.version.iv_build_num);
    }
    (void)printf("Size Max = 0x%x\r\n", info.staging.max_size);
  }
  else
  {
    (void)printf("State = 0x%x\r\n", info.state);
  }
}

/**
  * @brief  Run FW download process.
  * @param  psa_image_id_t image_id
  * @retval HAL Status.
  */
static void FW_APP_FWU_DownloadImage(psa_image_id_t image_id)
{
  HAL_StatusTypeDef ret;
  psa_image_info_t info  = { 0 };

  /* Print PSA API info */
  (void)printf("PSA API tested : \r\n");
  (void)printf("psa_fwu_write() \r\n");
  (void)printf("psa_fwu_install() \r\n");
  (void)printf("psa_fwu_query() \r\n");

  /* Print Firmware Update welcome message */
  (void)printf("Download Image\r\n");

  /* Download new firmware image*/
  ret = FW_APP_FWU_DownloadNewFirmware(IMAGE_SIZE_MAX, image_id);

  if (ret == HAL_OK)
  {
    (void)printf("  -- Image correctly downloaded \r\n\r\n");
    HAL_Delay(1000U);
    (void)printf("\r\nTEST PASSED\r\n");
  }
  else
  {
    (void)printf("\r\nTEST FAILED : %d \r\n", ret);
  }

  (void)psa_fwu_query(image_id, &info);
  (void)printf("\rDownload slot State after download : 0x%x \r\n", info.state);
}

/**
  * @brief  Run FW install request process.
  * @param  psa_image_id_t image_id
  * @retval HAL Status.
  */
static void FW_APP_FWU_InstallImage(psa_image_id_t image_id)
{
  psa_status_t psa_status;
  psa_image_id_t dependency_uuid;
  psa_image_version_t dependency_version;
  psa_image_info_t info  = { 0 };

  /* Print PSA API */
  (void)printf("\r\nPSA API tested : \r\n");
  (void)printf("psa_fwu_install() \r\n");
  (void)printf("psa_fwu_query() \r\n");

  (void)psa_fwu_query(image_id, &info);
  (void)printf("\rDownload slot State before install request : 0x%x \r\n", info.state);

  /* In the currently implementation, image verification is deferred to
   * reboot, so PSA_SUCCESS_REBOOT is returned when success.  */
  psa_status = psa_fwu_install(image_id, &dependency_uuid, &dependency_version);
  (psa_status == PSA_SUCCESS_REBOOT) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);

  (void)psa_fwu_query(image_id, &info);
  (void)printf("\rDownload slot State after install request : 0x%x \r\n", info.state);

  /* Check the image state. */
  if (info.state != (uint8_t)PSA_IMAGE_REBOOT_NEEDED)
  {
    (void)printf("\rWrong Image state\r\n\r\n");
  }
}

/**
  * @brief Download a new Firmware from the host.
  * @retval HAL status
  */
static HAL_StatusTypeDef FW_APP_FWU_DownloadNewFirmware(uint32_t uMaxFwSize, psa_image_id_t image_id)
{
  HAL_StatusTypeDef ret;
  COM_StatusTypeDef e_result;

  uint32_t u_fw_size;

  (void)printf("  -- Send Firmware \r\n\r\n");

  /* Download binary */
  (void)printf("  -- -- The binary name with extension must not exceed 64 characters \r\n\r\n");
  (void)printf("  -- -- File> Transfer> YMODEM> Send \t\n");

  /*Init of Ymodem*/
  Ymodem_Init();

  u_fw_size = uMaxFwSize;
  /*Receive through Ymodem*/
  e_result = Ymodem_Receive(&u_fw_size, image_id);
  (void)printf("\r\n\r\n");

  if ((e_result == COM_OK))
  {
    (void)printf("  -- -- Programming Completed Successfully!\r\n\r\n");
    (void)printf("  -- -- Bytes: %u\r\n\r\n", (int)u_fw_size);
    ret = HAL_OK;
  }
  else if (e_result == COM_ABORT)
  {
    (void)printf("  -- -- !!Aborted by user!!\r\n\r\n");
    (void)COM_Flush();
    ret = HAL_ERROR;
  }
  else
  {
    (void)printf("  -- -- !!Error during file download!!\r\n\r\n");
    ret = HAL_ERROR;
    HAL_Delay(500U);
    (void)COM_Flush();
  }

  return ret;
}


/**
  * @}
  */

/** @defgroup FW_UPDATE_Callback_Functions Callback Functions
  * @{
  */

/**
  * @brief  Ymodem Header Packet Transfer completed callback.
  * @param  uFileSize Dimension of the file that will be received (Bytes).
  * @retval None
  */
HAL_StatusTypeDef Ymodem_HeaderPktRxCpltCallback(psa_image_id_t imageId, uint32_t uOffset, uint32_t uFileSize)
{
  UNUSED(uOffset);
  UNUSED(imageId);
  /*Reset of the ymodem variables */
  m_uFileSizeYmodem = 0U;
  m_uPacketsReceived = 0U;
  m_uNbrBlocksYmodem = 0U;

  /*Filesize information is stored*/
  m_uFileSizeYmodem = uFileSize;

  /* compute the number of 1K blocks */
  m_uNbrBlocksYmodem = (m_uFileSizeYmodem + (PACKET_1K_SIZE - 1U)) / PACKET_1K_SIZE;

  /* NOTE : delay inserted for Ymodem protocol*/
  HAL_Delay(1000);
  return HAL_OK;
}

extern uint32_t total_size_received;
/**
  * @brief  Ymodem Data Packet Transfer completed callback.
  * @param  pData Pointer to the buffer.
  * @param  uSize Packet dimension (Bytes).
  * @retval None
  */
HAL_StatusTypeDef Ymodem_DataPktRxCpltCallback(uint8_t *pData, psa_image_id_t imageId, uint32_t uOffset, uint32_t uSize)
{
  int32_t ret;
  uint32_t size = uSize;
  m_uPacketsReceived++;
  psa_status_t status;

  /*Increase the number of received packets*/
  if (m_uPacketsReceived == m_uNbrBlocksYmodem) /*Last Packet*/
  {
    /*Extracting actual payload from last packet*/
    if ((m_uFileSizeYmodem % PACKET_1K_SIZE) == 0U)
    {
      /* The last packet must be fully considered */
      size = PACKET_1K_SIZE;
    }
    else
    {
      /* The last packet is not full, drop the extra bytes */
      size = m_uFileSizeYmodem - ((uint32_t)(m_uFileSizeYmodem / PACKET_1K_SIZE) * PACKET_1K_SIZE);
    }

    m_uPacketsReceived = 0U;
  }

  /* Write the data block with block_offset 0. */
  status = psa_fwu_write(imageId, uOffset, pData, size);
  if (status != PSA_SUCCESS)
  {
    (void)printf("psa_fwu_write() failure");
    return HAL_ERROR;
  }
  else
  {
    ret = ARM_DRIVER_OK;
  }
  if (ret != ARM_DRIVER_OK)
  {
    /*Reset of the ymodem variables */
    m_uFileSizeYmodem = 0U;
    m_uPacketsReceived = 0U;
    m_uNbrBlocksYmodem = 0U;
    return HAL_ERROR;
  }
  else
  {
    return HAL_OK;
  }
}

/**
  * @brief  retrieve images status in active and download slot
            (Non Secure, Secure Manager, STuRoT and Module0)
  * @param  pData Pointer to the buffer.
  * @param  uSize Packet dimension (Bytes).
  * @retval None
  */

static void FW_APP_FWU_QueryAll(void)
{
  psa_status_t psa_status;
  psa_image_info_t info  = { 0 };
  uint32_t image_id_active = (((uint8_t)PSA_FWU_SLOT_ID_ACTIVE) | (FWU_IMAGE_TYPE_NONSECURE << 8));

  (void)printf("\r\n Non-Secure Active Slot : \r\n");
  psa_status = psa_fwu_query(image_id_active, &info);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
  print_image_info(info, psa_status);

  (void)printf("\r\n Non-Secure Download Slot : \r\n");
  image_id_active = (((uint8_t)PSA_FWU_SLOT_ID_DL) | (FWU_IMAGE_TYPE_NONSECURE << 8));
  psa_status = psa_fwu_query(image_id_active, &info);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
  print_image_info(info, psa_status);

  (void)printf("\r\n Secure Manager Active Slot : \r\n");
  image_id_active = (((uint8_t)PSA_FWU_SLOT_ID_ACTIVE) | (FWU_IMAGE_TYPE_SECURE << 8));
  psa_status = psa_fwu_query(image_id_active, &info);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
  print_image_info(info, psa_status);

  (void)printf("\r\n Secure Manager Download Slot : \r\n");
  image_id_active = (((uint8_t)PSA_FWU_SLOT_ID_DL) | (FWU_IMAGE_TYPE_SECURE << 8));
  psa_status = psa_fwu_query(image_id_active, &info);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
  print_image_info(info, psa_status);

  (void)printf("\r\n STuRoT Active Slot : \r\n");
  image_id_active = (((uint8_t)PSA_FWU_SLOT_ID_ACTIVE) | (FWU_IMAGE_TYPE_BL << 8));
  psa_status = psa_fwu_query(image_id_active, &info);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
  print_image_info(info, psa_status);

  (void)printf("\r\n STuRoT Download Slot : \r\n");
  image_id_active = (((uint8_t)PSA_FWU_SLOT_ID_DL) | (FWU_IMAGE_TYPE_BL << 8));
  psa_status = psa_fwu_query(image_id_active, &info);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
  print_image_info(info, psa_status);

#if NB_MODULE > 0
  (void)printf("\r\n Module 0 Active Slot : \r\n");
  image_id_active = (((uint8_t)PSA_FWU_SLOT_ID_ACTIVE) | (FWU_IMAGE_TYPE_MODULE_0 << 8));
  psa_status = psa_fwu_query(image_id_active, &info);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
  print_image_info(info, psa_status);

  (void)printf("\r\n Module 0 Download Slot : \r\n");
  image_id_active = (((uint8_t)PSA_FWU_SLOT_ID_DL) | (FWU_IMAGE_TYPE_MODULE_0 << 8));
  psa_status = psa_fwu_query(image_id_active, &info);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);
  print_image_info(info, psa_status);
#endif /* NB_MODULE > 0 */

}

/**
  * @brief  Confirm updated image
  * @param  psa_image_id_t image_id of the image to confirm
  * @retval None
  */
static void FW_APP_FWU_Validation(psa_image_id_t image_id)
{
  psa_status_t psa_status;
  psa_image_info_t info  = { 0 };

  /* Print PSA API */
  (void)printf("\nPSA API tested : \r\n");
  (void)printf("psa_fwu_accept() \r\n");
  (void)printf("psa_fwu_query() \r\n");

  (void)psa_fwu_query(image_id, &info);
  (void)printf("\rActive slot State before accept : 0x%x \r\n", info.state);

  psa_status = psa_fwu_accept(image_id);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);

  (void)psa_fwu_query(image_id, &info);
  (void)printf("\rActive slot State after accept : 0x%x \r\n", info.state);
}

/**
  * @brief  Abort downloaded image
  * @param  psa_image_id_t image_id of the image to abort
  * @retval None
  */
static void FW_APP_FWU_Abort(psa_image_id_t image_id)
{
  psa_status_t psa_status;
  psa_image_info_t info  = { 0 };

  /* Print PSA API */
  (void)printf("\r\nPSA API tested : \r\n");
  (void)printf("psa_fwu_abort() \r\n");

  (void)psa_fwu_query(image_id, &info);
  (void)printf("\rDownload slot State before abort : 0x%x \r\n", info.state);

  psa_status = psa_fwu_abort(image_id);
  (psa_status == PSA_SUCCESS) ? (void)printf("\rTEST PASSED\r\n") : (void)printf("\rTEST FAILED:%d\r\n", (int)psa_status);

  (void)psa_fwu_query(image_id, &info);
  (void)printf("\rDownload slot State after abort : 0x%x \r\n", info.state);
}

