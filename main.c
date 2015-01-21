/* Motor H-bridge connections


H1
PC6 2P
PC7 1P
PC8 2N TM_PWM_Channel_3
PC9 1N TM_PWM_Channel_4

H2
PB4 2N TM_PWM_Channel_1
PB5 1N TM_PWM_Channel_2
PB7 1P
PD7 2P

*/


/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_servo.h"
#include "tm_stm32f4_adc.h"
#include "ACSE_math.h"
#include "infra_path.h"

#include <stdio.h>
//#include "tm_stm32f4_stdio.h"


typedef enum {
        tank_direction_forward= 0,
        tank_direction_backward= 1,
        tank_direction_turnleft= 2,
        tank_direction_turnright= 3,
        tank_direction_stop= 4

} tank_direction_t;

TM_PWM_TIM_t TIM3_Data;
int tank_speed_default = 45;
int tank_speed;
char str[40];
int i = 0;
uint8_t c;
int scanned = 0;



void tank_move(tank_direction_t direction, int speed){

switch (direction){
   case tank_direction_forward:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_3, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_4, speed);
      GPIO_SetBits(GPIOB, GPIO_Pin_7);
      //GPIO_SetBits(GPIOD, GPIO_Pin_7);
      //GPIO_SetBits(GPIOC, GPIO_Pin_6);
      GPIO_SetBits(GPIOC, GPIO_Pin_7);
      //GPIO_ResetBits(GPIOB, GPIO_Pin_7);
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      GPIO_ResetBits(GPIOC, GPIO_Pin_6);
      //GPIO_ResetBits(GPIOC, GPIO_Pin_7);

      break;

   case tank_direction_backward:
	   TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, speed);
	   TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, 0);
	   TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_3, speed);
	   TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_4, 0);
	   //GPIO_SetBits(GPIOB, GPIO_Pin_7);
	   GPIO_SetBits(GPIOD, GPIO_Pin_7);
	   GPIO_SetBits(GPIOC, GPIO_Pin_6);
	   //GPIO_SetBits(GPIOC, GPIO_Pin_7);
	   GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	   //GPIO_ResetBits(GPIOD, GPIO_Pin_7);
	   //GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	   GPIO_ResetBits(GPIOC, GPIO_Pin_7);


      break;

   case tank_direction_turnright:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_3, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_4, speed);
      //GPIO_SetBits(GPIOB, GPIO_Pin_7);
      GPIO_SetBits(GPIOD, GPIO_Pin_7);
      //GPIO_SetBits(GPIOC, GPIO_Pin_6);
      GPIO_SetBits(GPIOC, GPIO_Pin_7);
      GPIO_ResetBits(GPIOB, GPIO_Pin_7);
      //GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      GPIO_ResetBits(GPIOC, GPIO_Pin_6);
      //GPIO_ResetBits(GPIOC, GPIO_Pin_7);


      break;

   case tank_direction_turnleft:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_3, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_4, 0);
      GPIO_SetBits(GPIOB, GPIO_Pin_7);
      //GPIO_SetBits(GPIOD, GPIO_Pin_7);
      GPIO_SetBits(GPIOC, GPIO_Pin_6);
      //GPIO_SetBits(GPIOC, GPIO_Pin_7);
      //GPIO_ResetBits(GPIOB, GPIO_Pin_7);
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      //GPIO_ResetBits(GPIOC, GPIO_Pin_6);
      GPIO_ResetBits(GPIOC, GPIO_Pin_7);


      break;

   case tank_direction_stop:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_3, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_4, 0);
      GPIO_ResetBits(GPIOB, GPIO_Pin_7);
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      GPIO_ResetBits(GPIOC, GPIO_Pin_6);
      GPIO_ResetBits(GPIOC, GPIO_Pin_7);
   
	break;

   default:
      GPIO_ResetBits(GPIOB, GPIO_Pin_4 | GPIO_Pin_5);
      GPIO_ResetBits(GPIOC, GPIO_Pin_13 | GPIO_Pin_14);
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
    /* Initialize PWM on TIM3, Channel 3 and PinsPack 2 = PC8 */
    TM_PWM_InitChannel(TIM3, TM_PWM_Channel_3, TM_PWM_PinsPack_2);
    /* Initialize PWM on TIM3, Channel 4 and PinsPack 2 = PC9 */
    TM_PWM_InitChannel(TIM3, TM_PWM_Channel_4, TM_PWM_PinsPack_2);
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

    GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
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

        GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
        GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
        //Initialize pins
        GPIO_Init(GPIOD, &GPIO_InitDef);


}
void tank_speedsetup (){
TM_USART_Puts(USART2, "Set up tank speed! \n\r + increase - decrease speed by 5% \n\r r to end setup \n\r");

while (i !=110 ){
c = TM_USART_Getc(USART2);
switch (c){
                        case '+':
                                if (tank_speed <=95) tank_speed+=5;
                                itoa(tank_speed, str);
                                TM_USART_Puts(USART2, str);
                                TM_USART_Puts(USART2, "   \r");
                                break;
                        case '-':
                                if (tank_speed >=5) tank_speed-=5;
                                itoa(tank_speed, str);
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
 Delayms(10);
 i++;
 }
}

}



int main(void) {
	tank_speed = tank_speed_default;
	infra_path_init () ;
    /* Initialize system */
    SystemInit();
    //Initialize USART2 at 9600 baud, TX: PA2, RX: PA3
    TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);
    init_tank_pwm ();
    init_tank_gpio ();
    TM_DELAY_Init();
    //tank_speedsetup ();
    init_tank_speed ();
    //tank_speed = tank_speed_default;



    TM_USART_Puts(USART2, "chose move \n\r f forward \n\r b backward \n\r r turn right \n\r l turn left \n\r s stop \n\r");






    while (1) {
    	//tank_move(tank_direction_forward, 70);
    	//Get character from internal buffer
    			if (infra_path () & (scanned==0)){
    				tank_move(tank_direction_stop, 0);
    				freeway_detect ();
    				scanned = 1;
    			}


    			c = TM_USART_Getc(USART2);
    	        if (c) {

    	        	switch (c){
    	        	case 'f':
    	        		tank_move(tank_direction_forward, tank_speed);
    	        		TM_USART_Puts(USART2, "going forward \r");
    	        		scanned = 0;
    	        		break;
    	        	case 'b':
    	        		tank_move(tank_direction_backward, tank_speed);
    	        		TM_USART_Puts(USART2, "going backward \r");
    	        		scanned = 0;
    	        		break;
    	        	case 'r':
    	        		tank_move(tank_direction_turnright, tank_speed);
    	        		TM_USART_Puts(USART2, "turning right \r");
    	        		scanned = 0;
    	        		break;
    	        	case 'l':
    	        		tank_move(tank_direction_turnleft, tank_speed);
    	        		TM_USART_Puts(USART2, "turning left \r");
    	        		scanned = 0;
    	        		break;

    	        	case 's':
                        tank_move(tank_direction_stop, 0);
                        TM_USART_Puts(USART2, "stop              \r");
                        scanned = 0;
                        break;

    	        	}
    	        	//If anything received, put it back to terminal
    	            //TM_USART_Putc(USART2, c);
    	        }

    }
}

