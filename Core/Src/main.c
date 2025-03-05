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
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "atc.h"
#include <stdio.h>
#include <string.h>
#include "station.h"

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
//#############################################################
//UART_HandleTypeDef huart5; - RS485
//UART_HandleTypeDef huart1; - GSM
//UART_HandleTypeDef huart2; - Bluetooth
//UART_HandleTypeDef huart3; - uart-cpu

// Add atc.c str.365 =>     hAtc->Events = ev;

uint32_t  timer1 = 0;
uint32_t  timer2 = 0;
uint32_t  timer3 = 0;
uint32_t  timer4 = 0;

ATC_HandleTypeDef gsm;
int resp = 0;
int resp2 = 0;
int flag_sim800_error = 0;
int flagCommand2 = 0;
char command2[15] = {0,};
char strStation[15] = {0,};

void cbCommand2Receive(const char *str){
	flagCommand2 = 1;
	strncpy( command2, str, 14);
}

const ATC_EventTypeDef events[] = {
		{ "$2|", cbCommand2Receive },

};

struct telemetriaData telData;
char telString[130] = {0,};
char tempString[15] = {0,};

uint8_t rx_buff_fromStation[50] = {0,};
uint8_t temp_buff_fromStation[50] = {0,};
volatile uint8_t flagReceiveFromStation = 0;

//bluetooth module
uint8_t rx_buff_fromBluetooth[50] = {0,};

//flag adc
volatile uint8_t flagAdcDmaComplit = 0;
volatile uint16_t adc[3] = {0,}; //3 results
int u_bat = 0;
int u_5v = 0;
int temperature = 0;

//220
volatile uint8_t flagPower220 = 0;
volatile int u220 =0;
volatile int count_u220 =0;
int startAdcGetU220();


void sim800_power_on();
void sim800_power_off();
int reset_sim800_connect_to_server();

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if( huart->Instance == USART1 ){
		ATC_IdleLineCallback(&gsm, Size);
	}

	if(huart->Instance == UART5){
		  uint16_t i;
		  for( i = 0; i < Size; i++){
			  temp_buff_fromStation[i] = rx_buff_fromStation[i];
		  }
		  temp_buff_fromStation[i] = '\n';
		  i++;
		  temp_buff_fromStation[i] = '\0';
		  flagReceiveFromStation = 1;

		  //delete
		  //printf( (char *)temp_buff_rs485 );

		  HAL_UARTEx_ReceiveToIdle_IT(&huart5, rx_buff_fromStation, 50);
	}

	// from Bluetooth
	if(huart->Instance == USART2){

		  //delete
		  printf( (char *)rx_buff_fromBluetooth );

		  HAL_UARTEx_ReceiveToIdle_IT(&huart2, rx_buff_fromBluetooth, 50);
	}
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if(hadc->Instance == ADC1) {
        flagAdcDmaComplit = 1;
    }
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//        if(htim->Instance == TIM2) //check if the interrupt comes from TIM1
//        {
//                HAL_ResumeTick();
//                HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//        }
//}



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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_UART5_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_UART4_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */


//------ Off all device ---------------------------------------------------------------

	printf( "\n =====> Start! \n\n" );


  	while(1){
  		u220 = startAdcGetU220();
  		if (u220 > 40) {
  			break;
  		}
  		HAL_Delay(500);
  		HAL_PWR_EnterSTANDBYMode();
  	}


//-------------------------------------------------------------------------------------
  //  //Ethernet on!
  //  HAL_GPIO_WritePin(ON_ETHERNET_GPIO_Port, ON_ETHERNET_Pin, GPIO_PIN_RESET);

    //Bluetooth Reset!
    HAL_GPIO_WritePin(GPIOD, BLUETOOTH_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOD, BLUETOOTH_RES_Pin, GPIO_PIN_SET);

    //RS485 on!
    HAL_GPIO_WritePin(GPIOD, GATE_485_Pin, GPIO_PIN_RESET);
    //RS485 off!
    //HAL_GPIO_WritePin(GPIOD, GATE_485_Pin, GPIO_PIN_SET);

    //On Uart CPU
    HAL_GPIO_WritePin(ON_UART_CPU_GPIO_Port, ON_UART_CPU_Pin, GPIO_PIN_SET);



  ////  WIFI on
  //  HAL_GPIO_WritePin(GPIOC, WIFI_ON_OFF_Pin, GPIO_PIN_SET);
//-----------------------------------------------------------------------------------------


	HAL_UARTEx_ReceiveToIdle_IT(&huart5, rx_buff_fromStation, 50);




    //gsm modem
    ATC_Init(&gsm, &huart1, 512, "GSM");
    ATC_SetEvents(&gsm, events);

    for(int i = 0; i < 10; i++) {
    	if( reset_sim800_connect_to_server() == 1 ){
    		printf( "reset_sim800_connect_to_server(pos0) == 1  => connection Ok!\n" );
    		break;
    	} else {
    		printf( "reset_sim800_connect_to_server(pos0) != 1  => connection Err!\n" );
    	}
    }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		if((HAL_GetTick() - timer1) > 500) { //  500ms

			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			timer1 = HAL_GetTick();
		}



		if((HAL_GetTick() - timer3) > 200) { //  200ms
			//is '$2|' command receive?
			ATC_Loop(&gsm);

			//check command receive
			if (flagCommand2 == 1){
				flagCommand2 = 0;

 				if ( strServerToStation(strStation, command2, 15) == 0 ){
 					// query data from station change current
 					HAL_UART_Transmit(&huart5, (uint8_t *)strStation, strlen(strStation), 400);
 				}

			}

			timer3 = HAL_GetTick();
		}




		if((HAL_GetTick() - timer2) > 30000) { //  30sec
			//
			// query data from station
			HAL_UART_Transmit(&huart5, (uint8_t *)"$?;", 3, 200);
			HAL_Delay(200);

			if( flagReceiveFromStation ) {
				decodeStrToStruct(&telData, (char *)temp_buff_fromStation);
				flagReceiveFromStation = 0;
			} else{
				defaultStrToStruct(&telData);
			}

			// signal quality
			resp = ATC_SendReceive(&gsm, "AT+CSQ\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
			if(resp) {
				//printf( (char *)gsm.pReadBuff );
				if (gsm.pReadBuff[5] == 'Q'){
					gsm.pReadBuff[10] = '\0';
					strcpy( telData.sq, (char *)&gsm.pReadBuff[8]);
				}
			}

			// u_bat; 220v; temperature (run adc)
			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc, 3); // run ADC
			HAL_Delay(1);
			if( flagAdcDmaComplit ) {

				flagAdcDmaComplit = 0;

				HAL_ADC_Stop_DMA(&hadc1);

				u_bat = (int)(((adc[0] * 6.6) / 409.5) + 0.5) ;	// batery
				u_5v = (int)(((adc[1] * 6.6) / 409.5) + 0.5) ;	// 5 volt
				temperature = (int)((((adc[2] * 3.3) / 4095) - 0.76) / 0.0025 + 25.0) ;	// temperature
				//
				sprintf(tempString, "%i", u_bat);
				strcpy( telData.u_bat, tempString);
				//
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//todo del:
				//strcpy( telData.u_bat, "42");
				//
				if( u_5v > 40){
					sprintf(tempString, "%s", "1");
					count_u220 = 0;
				}
				else{
					sprintf(tempString, "%s", "0");
					count_u220++;
				}
				strcpy( telData.u220, tempString);
				//
				sprintf(tempString, "%+i", temperature);
				strcpy( telData.temperature, tempString);

				adc[0] = 0;
				adc[1] = 0;
				adc[2] = 0;
			}

			// door
			if ( HAL_GPIO_ReadPin(DOOR_GPIO_Port, DOOR_Pin) ) {
				//close
				sprintf(tempString, "%s", "0");
			}else {
				//open
				sprintf(tempString, "%s", "1");
			}
			strcpy( telData.door, tempString);


			sprintf(telString, "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|",
					telData.id,
					telData.i,
					telData.u,
					telData.p,
					telData.u_bat,
					telData.sq,
					telData.u220,
					telData.door,
					telData.temperature,
					telData.power,
					telData.i_max,
					telData.t_max
					);
			uint8_t crc = crc_8(telString);
			sprintf(tempString, "%u;\x1A", crc);
			strcat(telString, tempString);


			resp = ATC_SendReceive(&gsm, "AT+CIPSEND\r\n", 100, NULL, 100, 2, "\r\n> ", "\r\nERROR\r\n");

			resp2 = ATC_SendReceive(&gsm, telString, 100, NULL, 2000, 2, "\r\nSEND OK\r\n", "\r\nERROR\r\n");

			if ( (resp == 1) && (resp2 == 1) ){
				flag_sim800_error = 0;
			} else {
				flag_sim800_error = 1;
			}

			timer2 = HAL_GetTick();
		}


		if ( flag_sim800_error == 1 ){
			flag_sim800_error = 0;

		    for(int i = 0; i < 10; i++) {
		    	if( reset_sim800_connect_to_server() == 1 ){
		    		printf( "reset_sim800_connect_to_server(pos1) == 1  => connection Ok!\n" );
		    		break;
		    	} else {
		    		printf( "reset_sim800_connect_to_server(pos1) != 1  => connection Err!\n" );
		    	}
		    }
		}


//		if((HAL_GetTick() - timer4) > 43200000) { //  12hours
//
//		    for(int i = 0; i < 10; i++) {
//		    	if( reset_sim800_connect_to_server() == 1 ){
//		    		printf( "reset_sim800_connect_to_server(pos2) == 1  => connection Ok!\n" );
//		    		break;
//		    	} else {
//		    		printf( "reset_sim800_connect_to_server(pos2) != 1  => connection Err!\n" );
//		    	}
//		    }
//			timer4 = HAL_GetTick();
//		}


		HAL_IWDG_Refresh(&hiwdg); //reset watchdog

		if(count_u220 > 20){
			count_u220 = 0;
			// standby
			HAL_PWR_EnterSTANDBYMode();
		}


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    ITM_SendChar(*ptr++);
  }
  return len;
}


void sim800_power_on() {

    //Sim800 PWRKEY = hight
    HAL_GPIO_WritePin(GPIOD, GATE_PWRKEY_Pin, GPIO_PIN_RESET);
    //Sim800 PowerOn = On
    HAL_GPIO_WritePin(GPIOD, GATE_V_SIM_Pin, GPIO_PIN_SET);
    HAL_Delay(700);
    HAL_GPIO_WritePin(GPIOD, GATE_PWRKEY_Pin, GPIO_PIN_SET);
    HAL_Delay(1200);
    HAL_GPIO_WritePin(GPIOD, GATE_PWRKEY_Pin, GPIO_PIN_RESET);
}


void sim800_power_off() {
    //Sim800 PowerOn = Off
    HAL_GPIO_WritePin(GPIOD, GATE_V_SIM_Pin, GPIO_PIN_RESET);
}


int reset_sim800_connect_to_server() {

	int resp = 0;

	HAL_IWDG_Refresh(&hiwdg); //reset wdg

    sim800_power_off();
    HAL_Delay(5000);
    sim800_power_on();
    HAL_Delay(5000);

    HAL_IWDG_Refresh(&hiwdg); //reset wdg

    for(int i = 0; i < 1000; i++) {
    	HAL_Delay(100);
    	HAL_IWDG_Refresh(&hiwdg); //reset wdg
    	resp = ATC_SendReceive(&gsm, "AT\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    	if( resp == 1 ) break;
    }

    HAL_IWDG_Refresh(&hiwdg); //reset wdg

    HAL_Delay(25000);

    HAL_IWDG_Refresh(&hiwdg); //reset wdg

    resp = ATC_SendReceive(&gsm, "AT\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // echo off
    resp = ATC_SendReceive(&gsm, "ATE0\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // off all calls
    resp = ATC_SendReceive(&gsm, "AT+GSMBUSY=1\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // sms not in TE
    resp = ATC_SendReceive(&gsm, "AT+CNMI=0,0\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // sms text mode
    resp = ATC_SendReceive(&gsm, "AT+CMGF=1\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // signal quality
    resp = ATC_SendReceive(&gsm, "AT+CSQ\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // phone activity status
    resp = ATC_SendReceive(&gsm, "AT+CPAS\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;


    // deactivate gprs context
    resp = ATC_SendReceive(&gsm, "AT+CIPSHUT\r\n", 100, NULL, 5000, 2, "\r\nSHUT OK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // single ip conection
    resp = ATC_SendReceive(&gsm, "AT+CIPMUX=0\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // attach gprs
    resp = ATC_SendReceive(&gsm, "AT+CGATT=1\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // apn
    resp = ATC_SendReceive(&gsm, "AT+CSTT=\"wap.orange.md\"\r\n", 100, NULL, 100, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // bring wireless connection
    resp = ATC_SendReceive(&gsm, "AT+CIICR\r\n", 100, NULL, 5000, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    if (resp != 1) return resp;
    // get local ip address => resp==0 no OK
    resp = ATC_SendReceive(&gsm, "AT+CIFSR\r\n", 100, NULL, 1000, 2, "\r\nOK\r\n", "\r\nERROR\r\n");
    //if (resp != 1) return resp;

    resp = ATC_SendReceive(&gsm, "AT+CIPSTART=\"TCP\",\"scz.pge.md\",\"16992\"\r\n", 100, NULL, 5000, 2, "\r\nCONNECT OK\r\n", "\r\nERROR\r\n");

    HAL_IWDG_Refresh(&hiwdg); //reset wdg

    return resp;
}

int startAdcGetU220() {
	// u_bat; 220v; temperature (run adc)
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc, 3); // run ADC
	HAL_Delay(1);
	if( flagAdcDmaComplit ) {

		flagAdcDmaComplit = 0;

		HAL_ADC_Stop_DMA(&hadc1);

		u_5v = (int)(((adc[1] * 6.6) / 409.5) + 0.5) ;	// 5 volt

		return u_5v;
	} else{
		return -1;
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
