/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hc05_lib.hpp"
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
bool checker_AT = false;
bool checker_BR = false;
bool checker_Role = false;
bool checker_Name = false;
bool checker_Init = false;

std::string getter_BR;
std::string getter_INQM;
std::vector<Slave_info> getter_DevInfo;

std::string state1;
std::string state2;

std::string connections_response;

bool response;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void SendStructUART(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
  HC05 master(&huart4, Key_GPIO_Port, Key_Pin, Enable_GPIO_Port, Enable_Pin, 115200);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	// Pin default configuration for AT Mode:
	  master.initPinConfig();

	  for (uint8_t i = 0; i < 5; i++)
	  {
		  checker_AT = master.isConnectionOk();
	  }

	  HAL_Delay(100);
	  checker_Role = master.setRole(Role::Role_master);

	  master.setBaudRate(BaudRate_115200, StopBit_1, Parity_None);

	  master.connectDevicesViaName("hc-05 SLAVE", 3, 6);

	  master.disconnectDevices();

	  HAL_Delay(5000);

//	  HAL_GPIO_WritePin(Key_GPIO_Port, Key_Pin, GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(Enable_GPIO_Port, Enable_Pin, GPIO_PIN_RESET);


//	  state1 = master.getState();

	  //HAL_GPIO_WritePin(Key_GPIO_Port, Key_Pin, GPIO_PIN_RESET);

//	  connections_response = master.connectDevicesViaMACaddr("98D3:32:30CB23");
//
//	  HAL_Delay(2000);
//
//	  state2 = master.getState();
//
//	  HAL_Delay(5000);
//
//	  master.restoreToFactoryState();
//
//	  std::string name = master.getNameFromMAC("98D3:32:30CB23");
//
//	  state1 = master.getState(); //   <- tutaj się klinuje
//
//	  getter_DevInfo = master.getBluetoothDevices(5, 5);
//
//	  SendStructUART();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_UART4;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void SendStructUART(void)
{
	  size_t how_many_dev_found = getter_DevInfo.size(); // check how many devices was found

	  std::string dev_found = std::to_string(how_many_dev_found);

	  std::string start_message = "Founded " + dev_found + " Devices:\r\n \r\n";
	  HAL_UART_Transmit(&huart2, (uint8_t*)start_message.c_str(), start_message.length(), 1000);
	  HAL_Delay(1000);

	  for (const auto& device : getter_DevInfo)
	  {
		  // Convert data to string:
		  std::string dev_name = device.slave_name + " :\r\n";
		  std::string mac = "MAC ADDRESS: " + device.slave_address + " \r\n";
		  std::string type = "TYPE: " + device.slave_type + "\r\n";
		  std::string rssi = "RSSI: " + device.slave_rssi + "\r\n\r\n";

		  // Send Data:
		  HAL_UART_Transmit(&huart2, (uint8_t*)dev_name.c_str(), dev_name.length(), 1000);
		  HAL_Delay(1000);

		  HAL_UART_Transmit(&huart2, (uint8_t*)mac.c_str(), mac.length(), 1000);
		  HAL_Delay(1000);

		  HAL_UART_Transmit(&huart2, (uint8_t*)type.c_str(), type.length(), 1000);
		  HAL_Delay(1000);

		  HAL_UART_Transmit(&huart2, (uint8_t*)rssi.c_str(), rssi.length(), 1000);
		  HAL_Delay(1000);
	  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */