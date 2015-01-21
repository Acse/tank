/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"



typedef enum {
        tank_direction_forward= 0,
        tank_direction_backward= 1,
        tank_direction_turnleft= 2,
        tank_direction_turnright= 3

} tank_direction_t;

TM_PWM_TIM_t TIM3_Data;

void tank_move(tank_direction_t direction, int speed){

switch (direction){
   case tank_direction_forward:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, 0);
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

   case tank_direction_backward:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, speed);
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
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, 0);
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

   case tank_direction_turnright:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, speed);
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
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
    //Initialize pins
    GPIO_Init(GPIOB, &GPIO_InitDef);

    //GPIO_InitTypeDef GPIO_InitDef;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
    //Initialize pins
    GPIO_Init(GPIOC, &GPIO_InitDef);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

        GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
        GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
        //Initialize pins
        GPIO_Init(GPIOD, &GPIO_InitDef);


}

int main(void) {

    uint8_t c;
    /* Initialize system */
    SystemInit();
    init_tank_pwm ();
    init_tank_gpio ();
    TM_DELAY_Init();
    //Initialize USART2 at 9600 baud, TX: PA9, RX: PA10
    TM_USART_Init(USART2, TM_USART_PinsPack_1, 9600);

    TM_USART_Puts(USART2, "chose move \n\r f forward \n\r b backward \n\r r turn right \n\r l turn left \n\r");






    while (1) {
    	//tank_move(tank_direction_forward, 70);
    	//Get character from internal buffer
    	        c = TM_USART_Getc(USART2);
    	        if (c) {

    	        	switch (c){
    	        	case 'f':
    	        		tank_move(tank_direction_forward, 70);
    	        		TM_USART_Puts(USART2, "going forward \r");
    	        		break;
    	        	case 'b':
    	        		tank_move(tank_direction_backward, 70);
    	        		TM_USART_Puts(USART2, "going backward \r");
    	        		break;
    	        	case 'r':
    	        		tank_move(tank_direction_turnright, 70);
    	        		TM_USART_Puts(USART2, "turning right \r");
    	        		break;
    	        	case 'l':
    	        		tank_move(tank_direction_turnleft, 70);
    	        		TM_USART_Puts(USART2, "turning left \r");
    	        		break;


    	        	}
    	        	//If anything received, put it back to terminal
    	            //TM_USART_Putc(USART2, c);
    	        }

    }
}

