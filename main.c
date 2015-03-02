/* Motor H-bridge connections

grey STBY PD6
blue PWMA PB4 TIM3 PP2 CH1
green AIN2 PB6
yellow AIN1 PB7
orange BIN1 PD7
red BIN2 PB3
brown PWMB PB5 TIM3 PP2 CH2


*/

#define ARM_MATH_CM4
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_pwm.h"
//#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_servo.h"
#include "tm_stm32f4_adc.h"
#include "ACSE_math.h"
//#include "infra_path.h"
//#include "arm_math.h"

#include "stm32f4_discovery.h"
#include <stdio.h>
//#include "tm_stm32f4_stdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

uint64_t u64IdleTicksCnt=0; // Counts when the OS has no task to execute.
uint64_t tickTime=0;        // Counts OS ticks (default = 1000Hz).

typedef enum {
        tank_direction_forward= 0,
        tank_direction_backward= 1,
        tank_direction_turnleft= 2,
        tank_direction_turnright= 3,
        tank_direction_stop= 4

} tank_direction_t;

TM_PWM_TIM_t TIM3_Data;
int tank_speed_default = 75;
int tank_speed_left,tank_speed_right;
char str[40];
int i = 0;
uint8_t c;
int scanned = 0;
int block=0;
TM_SERVO_t Servo1, Servo2;

FILE USART2_Stream;

void vFlashTask (void * pvparameters);
void vControlTask (void * pvparameters);
void vDetectTask (void * pvparameters);
void vScanTask( void *pvparameters );
int USART2_Stream_OutputFunction(int ch, FILE* f);


/* USART2_Stream function that will send char by char to stream */
int USART2_Stream_OutputFunction(int ch, FILE* f) {
    /* Send char via USART2 */
    TM_USART_Putc(USART2, (char) ch);

    /* Return ch, it means OK */
    return ch;
    /* If you want to return error, then you have to send EOF (-1) */
    //return -1;
}

void tank_move(tank_direction_t direction){

switch (direction){
   case tank_direction_forward:

	   GPIO_SetBits(GPIOB, GPIO_Pin_6);
	   GPIO_SetBits(GPIOD, GPIO_Pin_7);
	   GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	   GPIO_ResetBits(GPIOB, GPIO_Pin_3);

	  //set STBY high enabling H bridges
	  GPIO_SetBits(GPIOD, GPIO_Pin_6);





      break;

   case tank_direction_backward:
	   GPIO_SetBits(GPIOB, GPIO_Pin_7);
	   GPIO_ResetBits(GPIOD, GPIO_Pin_7);
	   GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	   GPIO_SetBits(GPIOB, GPIO_Pin_3);

	   	  //set STBY high enabling H bridges
	   	  GPIO_SetBits(GPIOD, GPIO_Pin_6);


      break;

   case tank_direction_turnright:
	   GPIO_SetBits(GPIOB, GPIO_Pin_6);
	   GPIO_ResetBits(GPIOD, GPIO_Pin_7);
	   GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	   GPIO_SetBits(GPIOB, GPIO_Pin_3);

	   	  //set STBY high enabling H bridges
	   	  GPIO_SetBits(GPIOD, GPIO_Pin_6);


      break;

   case tank_direction_turnleft:
	   GPIO_SetBits(GPIOB, GPIO_Pin_7);
	   GPIO_SetBits(GPIOD, GPIO_Pin_7);
	   GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	   GPIO_ResetBits(GPIOB, GPIO_Pin_3);

	   	  //set STBY high enabling H bridges
	   	  GPIO_SetBits(GPIOD, GPIO_Pin_6);


      break;

   case tank_direction_stop:

	  GPIO_ResetBits(GPIOD, GPIO_Pin_6);



   
	break;

   default:

	  GPIO_ResetBits(GPIOD, GPIO_Pin_6);

    break;
}
}

void init_tank_pwm (){
    /* Set PWM to 20kHz frequency on timer TIM3 */

    TM_PWM_InitTimer(TIM3, &TIM3_Data, 20000);

    /* Initialize PWM on TIM3, Channel 1 and PinsPack 2 = PB4 */
    TM_PWM_InitChannel(TIM3, TM_PWM_Channel_1, TM_PWM_PinsPack_2);
    /* Initialize PWM on TIM3, Channel 2 and PinsPack 2 = PB5 */
    TM_PWM_InitChannel(TIM3, TM_PWM_Channel_2, TM_PWM_PinsPack_2);

    TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, tank_speed_left);
    TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, tank_speed_right);

    /*
    // Initialize PWM on TIM3, Channel 3 and PinsPack 2 = PC8
    TM_PWM_InitChannel(TIM3, TM_PWM_Channel_3, TM_PWM_PinsPack_2);
    // Initialize PWM on TIM3, Channel 4 and PinsPack 2 = PC9
    TM_PWM_InitChannel(TIM3, TM_PWM_Channel_4, TM_PWM_PinsPack_2);
    */
}

void init_tank_gpio (){
	GPIO_InitTypeDef GPIO_InitDef;

	//Enable clock for GPOIA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//Pin 0
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	//Mode output
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	//Output type push-pull
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	//With pull down resistor
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	//50MHz pin speed
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	//Initialize pin on GPIOA port
	GPIO_Init(GPIOA, &GPIO_InitDef);



    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitDef.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
    //Initialize pins
    GPIO_Init(GPIOB, &GPIO_InitDef);

    //GPIO_InitTypeDef GPIO_InitDef;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
    //Initialize pins
    GPIO_Init(GPIOC, &GPIO_InitDef);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

        GPIO_InitDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
        GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
        //Initialize pins
        GPIO_Init(GPIOD, &GPIO_InitDef);


}
void tank_speedsetup (){
TM_USART_Puts(USART2, "Set up tank speed! \n\r + increase - decrease  left speed by 5% \n\r p increase m decrease  right speed by 5% \n\r r to end setup \n\r");

while (i !=110 ){
c = TM_USART_Getc(USART2);
switch (c){
                        case '+':
                                if (tank_speed_left <=95) tank_speed_left+=5;
                                itoa(tank_speed_left, str);
                                TM_USART_Puts(USART2, str);
                                TM_USART_Puts(USART2, "   \r");
                                break;
                        case '-':
                                if (tank_speed_left >=5) tank_speed_left-=5;
                                itoa(tank_speed_left, str);
                                TM_USART_Puts(USART2, str);
                                TM_USART_Puts(USART2, "   \r");
                                break;
                        case 'p':
                                if (tank_speed_right <=95) tank_speed_right+=5;
                                itoa(tank_speed_right, str);
                                TM_USART_Puts(USART2, str);
                                TM_USART_Puts(USART2, "   \r");
                                break;
                        case 'm':
                                if (tank_speed_right >=5) tank_speed_right-=5;
                                itoa(tank_speed_right, str);
                                TM_USART_Puts(USART2, str);
                                TM_USART_Puts(USART2, "   \r");
                                break;


                        case 'r':
                                i=110;
                                TM_USART_Puts(USART2, "setup ended \r");
                                break;
}
}
}

void init_tank_speed (){


 if (i < 100){
 if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
	 tank_speedsetup ();
 }
 else{
 //Delayms(10);
 i++;
 }
}

}

void tank_control (){
	c = TM_USART_Getc(USART2);
	    	        if (c) {

	    	        	switch (c){
	    	        	case 'f':
	    	        		tank_move(tank_direction_forward);
	    	        		TM_USART_Puts(USART2, "going forward \r");
	    	        		scanned = 0;
	    	        		break;
	    	        	case 'b':
	    	        		tank_move(tank_direction_backward);
	    	        		TM_USART_Puts(USART2, "going backward \r");
	    	        		scanned = 0;
	    	        		break;
	    	        	case 'r':
	    	        		tank_move(tank_direction_turnright);
	    	        		TM_USART_Puts(USART2, "turning right \r");
	    	        		scanned = 0;
	    	        		break;
	    	        	case 'l':
	    	        		tank_move(tank_direction_turnleft);
	    	        		TM_USART_Puts(USART2, "turning left \r");
	    	        		scanned = 0;
	    	        		break;

	    	        	case 's':
	                        tank_move(tank_direction_stop);
	                        TM_USART_Puts(USART2, "stop              \r");
	                        scanned = 0;
	                        break;

	    	        	}
	    	        	//If anything received, put it back to terminal
	    	            //TM_USART_Putc(USART2, c);
	    	        }
}

int main(void) {
    /* Initialize system */
    SystemInit();
	tank_speed_left = tank_speed_default;
	tank_speed_right = tank_speed_default;
//	infra_path_init () ;
	/* Initialize ADC1 on channel 12, this is pin PC2 */
	TM_ADC_Init(ADC1, ADC_Channel_12);
	/* Initialize servo 1, TIM2, Channel 2, Pinspack 1 = PA1 */
	TM_SERVO_Init(&Servo1, TIM2, TM_PWM_Channel_2, TM_PWM_PinsPack_1);

    //Initialize USART2 at 9600 baud, TX: PA2, RX: PA3
    TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);

    init_tank_speed ();
    init_tank_gpio ();
    init_tank_pwm ();
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED5);
    STM_EVAL_LEDInit(LED6);




    TM_USART_Puts(USART2, "chose move \n\r f forward \n\r b backward \n\r r turn right \n\r l turn left \n\r s stop \n\r");

    xTaskCreate( vControlTask, ( const char * ) "Control Task", 100, NULL, 2, NULL );
    xTaskCreate( vFlashTask, ( const char * ) "Flash Task", 100, NULL, 3, NULL );
    xTaskCreate( vDetectTask, ( const char * ) "Detect Task", 100, NULL, 1, NULL );
    xTaskCreate( vScanTask, ( const char * ) "Scan Task", 100, NULL, 3, NULL );

    vTaskStartScheduler(); // This should never return.

    while (1) {



    }
    return 1;
}

void vFlashTask( void *pvparameters )
{

	for (;;){

		if (tickTime % 100 == 0){
			STM_EVAL_LEDToggle(LED3);
				}

		vTaskDelay(5 / portTICK_RATE_MS);
	}
}

void vControlTask( void *pvparameters )
{



	for (;;){



		tank_control ();
		if (tickTime % 500 == 0){
		STM_EVAL_LEDToggle(LED6);
		}
		vTaskDelay(5 / portTICK_RATE_MS);
	}
}


void vDetectTask( void *pvparameters )
{
int x=0;
float distance;
char dist[40];

	for (;;){
		STM_EVAL_LEDOff(LED4);
		if (tickTime % 100 == 0){
			x=TM_ADC_Read(ADC1, ADC_Channel_12);
			//distance = (x*3)/4096;
			if ((x > 2700) && (block == 0)){
				tank_move(tank_direction_stop);
				STM_EVAL_LEDOn(LED4);
				block = 1;
			}
			itoa(x, dist);
			TM_USART_Puts(USART2, dist);
			TM_USART_Puts(USART2, "    \r");
				}

		vTaskDelay(50 / portTICK_RATE_MS);
	}
}

void vScanTask( void *pvparameters )
{
int temp=0;
int u=0;
int x=10, var, deg;
int num[x];


for (;;){
if ((block == 1)) {


	for (deg =45 ;deg <= 135; deg++){
		temp=0;
				for (var = 0; var < x; ++var) {
					num[var] = TM_ADC_Read(ADC1, ADC_Channel_12);
					temp+=num[var];

					vTaskDelay(1 / portTICK_RATE_MS);
				}
				u=(temp/x);/*(temp/x)*arm_cos_f32(((deg-90)/180)*3.1415926);*/


				itoa(u, str);
				TM_USART_Puts(USART2, str);
				TM_USART_Puts(USART2, "\n\r");


				TM_SERVO_SetDegrees(&Servo1, deg);


				vTaskDelay(4 / portTICK_RATE_MS);
	}

	TM_USART_Puts(USART2, "end of scan\n\r");
	TM_SERVO_SetDegrees(&Servo1, 90);
	block = 0;
}
	vTaskDelay(50 / portTICK_RATE_MS);

}



}

// This FreeRTOS callback function gets called once per tick (default = 1000Hz).
// ----------------------------------------------------------------------------
void vApplicationTickHook( void ) {
    ++tickTime;
}

// This FreeRTOS call-back function gets when no other task is ready to execute.
// On a completely unloaded system this is getting called at over 2.5MHz!
// ----------------------------------------------------------------------------
void vApplicationIdleHook( void ) {
    ++u64IdleTicksCnt;
}

// A required FreeRTOS function.
// ----------------------------------------------------------------------------
void vApplicationMallocFailedHook( void ) {
    configASSERT( 0 );  // Latch on any failure / error.
}
