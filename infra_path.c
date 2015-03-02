/**
 *	Keil project for servo example
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_servo.h"
#include "tm_stm32f4_adc.h"
#include "tm_stm32f4_usart.h"
#include "ACSE_math.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

int temp=0;
int temp2=0;
	int u=0;
	int i2=0;
	int var = 0;
	int var2 = 0;
	int deg=0;
	char str[40];

	/* Servo structs */
		TM_SERVO_t Servo1, Servo2;
		
	
void freeway_detect (){
			temp=0;
			u=0;
			int x=100;
			int num[x];
			TM_SERVO_SetDegrees(&Servo1, 45);
//			Delayms(300);
			vTaskDelay(300 / portTICK_RATE_MS);
for (deg =45 ;deg <= 135; deg++){
	temp=0;
			for (var = 0; var < x; ++var) {
				num[var] = TM_ADC_Read(ADC1, ADC_Channel_12);
				temp+=num[var];
//				Delay(100);
				vTaskDelay(1);
			}
			u=(temp/x);/*(temp/x)*arm_cos_f32(((deg-90)/180)*3.1415926);*/


			itoa(u, str);
			TM_USART_Puts(USART2, str);
			TM_USART_Puts(USART2, "\n\r");


			TM_SERVO_SetDegrees(&Servo1, deg);

//			Delayms(4);
			vTaskDelay(4 / portTICK_RATE_MS);
}
//Delayms(100);
vTaskDelay(100 / portTICK_RATE_MS);
TM_USART_Puts(USART2, "end of scan\n\r");
TM_SERVO_SetDegrees(&Servo1, 90);
}	



void infra_path_init (){
	
	/* Initialize system */
	SystemInit();

	/* Initialize delay */
//	TM_DELAY_Init();

	TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);

	/* Initialize ADC1 on channel 12, this is pin PC2 */
	TM_ADC_Init(ADC1, ADC_Channel_12);

	/* Initialize servo 1, TIM2, Channel 2, Pinspack 1 = PA1 */
	TM_SERVO_Init(&Servo1, TIM2, TM_PWM_Channel_2, TM_PWM_PinsPack_1);

	/* Initialize servo 2, TIM2, Channel 2, Pinspack 2 = PB3 */
	TM_SERVO_Init(&Servo2, TIM2, TM_PWM_Channel_2, TM_PWM_PinsPack_2);
	
	TM_SERVO_SetDegrees(&Servo1, 90);
}

int infra_path () {

temp2=0;
i2=0;
int x2=200;
int num2[x2];
int ret=0;

for (var2 = 0; var2 < x2; ++var2) {
					num2[var2] = TM_ADC_Read(ADC1, ADC_Channel_12);
					temp2+=num2[var2];
//					Delay(5);
					//vTaskDelay(1);
				}

i2=(temp2/x2);

if (i2>2000) {
	ret = 1;
}
else ret = 0;

return ret;
}
	
