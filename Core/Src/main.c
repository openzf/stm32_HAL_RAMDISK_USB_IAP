/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stmflash.h"
#include "kymodem.h"
#include "dev_com.h"
#include "drv_com.h"
#include "iap.h"
#include "flash_if.h"



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


#define KAPP_ADDR  APPLICATION_ADDRESS

// -usb fatfs测试


FATFS fs;    // Work area (file system object) for logical drive
FIL fil;     // file objects
             /* File read buffers */
char filename[] = "0:/STM32cube.txt";
unsigned int count = 0;

unsigned char write_buf[50] = "skt t1 faker123\r\n";

uint8_t read_buf[8192];


#define WORK_BUFFER_SIZE  512
static char work_buffer[WORK_BUFFER_SIZE];
void ram_disk_test(void)
{
	FRESULT retSD;
	
	retSD = f_mkfs("0:/",FM_ANY,0,work_buffer,WORK_BUFFER_SIZE);
	if(retSD != FR_OK)
	{
			printf("f_mkfs sdcard error,err = %d\r\n", retSD);
			while(1);

	}else{
			printf("f_mk sdcard ok 1\r\n");

	}
		
		
		 printf("\r\n ****** FatFs Example ******\r\n\r\n");
 
    /*##-1- Register the file system object to the FatFs module ##############*/
	retSD = f_mount(&fs, "0:/", 0);
    if(retSD)
    {
        printf(" mount error : %d \r\n",retSD);
        Error_Handler();
    }
    else
        printf(" mount sucess!!! \r\n");
     


    // 挂载spi flash
    retSD = f_open(&USERFile, "0:/sudaroot.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if(retSD == FR_OK)
    {
        printf("open ok\r\n");
        retSD = f_write(&USERFile, write_buf, strlen((const char *)write_buf), &count);
        if(retSD != FR_OK)
        {
            printf("f_write ,err = %d\r\n", retSD);
            count = 0;
            f_close(&USERFile);
        }
        else
        {
            printf("write ok:%d\n", count);
            printf("write buf :\r\n%s\r\n", write_buf);
            count = 0;
            f_close(&USERFile);
        }
    }
    else
    {
        printf("open sudaroot.txt,err = %d\r\n", retSD);
    }


    retSD = f_open(&USERFile, "0:/sudaroot.txt", FA_OPEN_EXISTING | FA_READ);
    if(retSD == FR_OK)
    {
        printf(" open sudaroot ok\r\n");
        retSD = f_read(&USERFile, read_buf, sizeof(read_buf), &count);
        if(retSD != FR_OK)
        {
            printf("f_read ,err = %d\r\n", retSD);
            f_close(&USERFile);
        }
        else
        {
            printf("open ok:%d\n", count);
            printf("read buf:\r\n%s\r\n", read_buf);
            f_close(&USERFile);
        }
    }
    else
    {
        printf("read sudaroot.txt ,err = %d\r\n", retSD);
    }
}


// usb完成之后读取bin
void system_usb_finish_read()
{
		FRESULT retSD;
	
		retSD = f_mount(&fs, "0:/", 0);
    if(retSD)
    {
        printf(" mount error : %d \r\n",retSD);
        Error_Handler();
    }
    else
        printf(" mount sucess!!! \r\n");

	
    retSD = f_open(&USERFile, "0:/update.bin", FA_OPEN_EXISTING | FA_READ);
    if(retSD == FR_OK)
    {
        printf(" open sudaroot ok\r\n");
        retSD = f_read(&USERFile, read_buf, sizeof(read_buf), &count);
        if(retSD != FR_OK)
        {
            printf("f_read ,err = %d\r\n", retSD);
            f_close(&USERFile);
        }
        else
        {
            printf("open ok: len == %d\n", count);
					  FLASH_If_Erase(KAPP_ADDR);
					  printf("flash erase ok\n");
						if (FLASH_If_Write(KAPP_ADDR,(uint32_t*)  read_buf, count/4) == FLASHIF_OK)
						{
							printf("data write ok\r\n");
							iap_load_app(KAPP_ADDR);
						}else{
								printf("data write error\r\n");
						}
	
            printf("read buf:\r\n%s\r\n", read_buf);
            f_close(&USERFile);
        }
    }
    else
    {
        printf("read sudaroot.txt ,err = %d\r\n", retSD);
    }
}


// fatfs init
void fatfs_init()
{
	FRESULT retSD;
	retSD = f_mkfs("0:/",FM_ANY,0,work_buffer,WORK_BUFFER_SIZE);
	if(retSD != FR_OK)
	{
			printf("f_mkfs sdcard error,err = %d\r\n", retSD);
			while(1);

	}else{
			printf("f_mk sdcard ok 1\r\n");

	}
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	FLASH_If_Init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
	
	 MX_FATFS_Init();

						
	// 格式化fatfs
	 fatfs_init();
	 //ram_disk_test();
	 MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

	LL_USART_EnableIT_RXNE(USART1);
	LL_USART_EnableIT_PE(USART1);   
	
	dev_comctrl_init();
	printf("init\r\n");
	
	uint8_t old_usb_state = 0;
		uint8_t over_time = 0;
		uint8_t usb_input_state = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		dev_comctrl_handle();
		#ifdef USE_RX_MODE
		ymodem_rx_time_handle();
		#endif
		
		if(hUsbDeviceFS.dev_state != old_usb_state){
			if(hUsbDeviceFS.dev_state== USBD_STATE_CONFIGURED){
				usb_input_state = 1;
				printf("usb 已经连接\r\n");
			}else if(hUsbDeviceFS.dev_state== USBD_STATE_SUSPENDED){
				printf("usb 断开成功\r\n");
				system_usb_finish_read();
			}else{
			}
			old_usb_state = hUsbDeviceFS.dev_state;
		}
		
		// 如果插入超时,
		if(over_time++>3){
			// 没检查到USB 直接运行APP
			if(usb_input_state == 0){
				printf("---system run -app\r\n");
				iap_load_app(KAPP_ADDR);
				printf("---system run -app error!!!!\r\n");
			}
		}
		HAL_Delay(1000);
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
