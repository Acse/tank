/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_pwm.h"


typedef enum {
        tank_direction_forward= 0,
        tank_direction_backward= 1,
        tank_direction_turnleft= 2,
        tank_direction_turnright= 3

} tank_direction_t;

void tank_move(tank_direction_t direction, int speed){

switch (direction){
   case tank_direction_forward:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_3, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_4, 0);
      //GPIO_SetBits(GPIOB, GPIO_Pin_4);
      GPIO_SetBits(GPIOB, GPIO_Pin_5);
      //GPIO_SetBits(GPIOC, GPIO_Pin_8);
      GPIO_SetBits(GPIOC, GPIO_Pin_9);
      GPIO_ResetBits(GPIOB, GPIO_Pin_4);
      //GPIO_ResetBits(GPIOB, GPIO_Pin_5);
      GPIO_ResetBits(GPIOC, GPIO_Pin_8);
      //GPIO_ResetBits(GPIOC, GPIO_Pin_9);

      break;

   case tank_direction_backward:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_3, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_4, speed);
      GPIO_SetBits(GPIOB, GPIO_Pin_4);
      //GPIO_SetBits(GPIOB, GPIO_Pin_5);
      GPIO_SetBits(GPIOC, GPIO_Pin_8);
      //GPIO_SetBits(GPIOC, GPIO_Pin_9);
      //GPIO_ResetBits(GPIOB, GPIO_Pin_4);
      GPIO_ResetBits(GPIOB, GPIO_Pin_5);
      //GPIO_ResetBits(GPIOC, GPIO_Pin_8);
      GPIO_ResetBits(GPIOC, GPIO_Pin_9);

      break;

   case tank_direction_turnleft:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_3, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_4, 0);
      //GPIO_SetBits(GPIOB, GPIO_Pin_4);
      GPIO_SetBits(GPIOB, GPIO_Pin_5);
      GPIO_SetBits(GPIOC, GPIO_Pin_8);
      //GPIO_SetBits(GPIOC, GPIO_Pin_9);
      GPIO_ResetBits(GPIOB, GPIO_Pin_4);
      //GPIO_ResetBits(GPIOB, GPIO_Pin_5);
      //GPIO_ResetBits(GPIOC, GPIO_Pin_8);
      GPIO_ResetBits(GPIOC, GPIO_Pin_9);

      break;

   case tank_direction_turnright:
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_1, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_2, speed);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_3, 0);
      TM_PWM_SetChannelPercent(TIM3, &TIM3_Data, TM_PWM_Channel_4, speed);
      GPIO_SetBits(GPIOB, GPIO_Pin_4);
      //GPIO_SetBits(GPIOB, GPIO_Pin_5);
      //GPIO_SetBits(GPIOC, GPIO_Pin_8);
      GPIO_SetBits(GPIOC, GPIO_Pin_9);
      //GPIO_ResetBits(GPIOB, GPIO_Pin_4);
      GPIO_ResetBits(GPIOB, GPIO_Pin_5);
      GPIO_ResetBits(GPIOC, GPIO_Pin_8);
      //GPIO_ResetBits(GPIOC, GPIO_Pin_9);

      break;

   default:
      GPIO_ResetBits(GPIOB, GPIO_Pin_4 | GPIO_Pin_5);
      GPIO_ResetBits(GPIOC, GPIO_Pin_13 | GPIO_Pin_14);
    break;
}
}

void init_tank_pwm (){
    /* Set PWM to 20kHz frequency on timer TIM3 */
    /* 20 kHz =50us */
    TM_PWM_InitTimer(TIM3, &TIM3_Data, 50);

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

    GPIO_InitDef.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
    //Initialize pins
    GPIO_Init(GPIOB, &GPIO_InitDef);

    GPIO_InitTypeDef GPIO_InitDef;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitDef.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
    //Initialize pins
    GPIO_Init(GPIOC, &GPIO_InitDef);


}

int main(void) {
    TM_PWM_TIM_t TIM3_Data;

    /* Initialize system */
    SystemInit();

    init_tank_pwm ();
    init_tank_gpio ();









    while (1) {

    }
}

