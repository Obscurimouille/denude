/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "stm32f1_mpu6050.h"
#include "WS2812S.h"

#define COLOR_BLUE	0x000077
#define COLOR_RED	0x007700
#define COLOR_GREEN	0x770000
#define COLOR_BLACK	0x000000

void writeLED(bool_e b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

bool_e readButton(void)
{
	return !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN);
}

static volatile uint32_t t = 0;
void process_ms(void)
{
	if(t)
		t--;
}


int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la premi�re �tape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2 � la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reli�es jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirig�es vers la sonde de d�bogage, la liaison UART �tant ensuite encapsul�e sur l'USB vers le PC de d�veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le p�riph�rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port de la matrice Led (carte Nucleo)
	LED_MATRIX_init();

	//Initialise la liaison I2C, ainsi que l'acc�l�rom�tre.
	//MPU6050_Init(&datas, GPIOB, GPIO_PIN_5, MPU6050_Device_0, MPU6050_Gyroscope_2000s);

	//On ajoute la fonction process_ms � la liste des fonctions appel�es automatiquement chaque ms par la routine d'interruption du p�riph�rique SYSTICK
	Systick_add_callback_function(&process_ms);

	/*
	uint32_t pixels[64];
	uint8_t i;
	for(i=0;i<64;i++)
	{
		pixels[i] = COLOR_RED;
	}
	LED_MATRIX_display(pixels, 64);
	*/

	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 1);
	HAL_Delay(1000);

	while(1)	//boucle de t�che de fond
	{
		//MPU6050_demo();
		if(MPU6050_test())
		{
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 0);
		}
		HAL_Delay(1000);
		HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 1);

		/*if(readButton())
		{
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 1);
		}
		else
		{
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 0);
		}*/
		/*
		if(!t)
		{
			t = 200;
			HAL_GPIO_TogglePin(LED_GREEN_GPIO, LED_GREEN_PIN);
		}
		*/
	}
}
