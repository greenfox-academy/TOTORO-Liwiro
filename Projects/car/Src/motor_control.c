/* Includes ------------------------------------------------------------------*/
#include "motor_control.h"
#include "stm32l4xx_hal.h"
#include "init.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/

/* PWM variables */
extern TIM_HandleTypeDef tim2_pwm_handle;
extern TIM_HandleTypeDef tim3_pwm_handle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void go_forward()
{
	gpio_m1_p1_on();
	gpio_m1_p2_off();
	tim2_pwm_handle.Instance->CCR1 = 50;
	HAL_TIM_PWM_Start(&tim2_pwm_handle, TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&tim3_pwm_handle, TIM_CHANNEL_1);
	printf("PWM started\r\n");
	while(1){
		HAL_Delay(10000);
		tim2_pwm_handle.Instance->CCR1 = 50;
		printf("100\r\n");
		printf("PWM started\r\n");
		HAL_Delay(10000);
		tim2_pwm_handle.Instance->CCR1 = 40;
		printf("80\r\n");
		HAL_Delay(10000);
		tim2_pwm_handle.Instance->CCR1 = 35;
		printf("70\r\n");
		HAL_Delay(10000);
		tim2_pwm_handle.Instance->CCR1 = 20;
		printf("40\r\n");
	}
	HAL_TIM_PWM_Stop(&tim2_pwm_handle, TIM_CHANNEL_1);
}

void go_backward()
{
	HAL_TIM_PWM_Start(&tim3_pwm_handle, TIM_CHANNEL_1);
	while(1){

	}
	HAL_TIM_PWM_Stop(&tim3_pwm_handle, TIM_CHANNEL_1);
}

void go_left()
{
	HAL_TIM_PWM_Start(&tim3_pwm_handle, TIM_CHANNEL_1);
	while(1){

	}
	HAL_TIM_PWM_Stop(&tim3_pwm_handle, TIM_CHANNEL_1);
}

void go_right()
{
	HAL_TIM_PWM_Start(&tim3_pwm_handle, TIM_CHANNEL_1);
	while(1){

	}
	HAL_TIM_PWM_Stop(&tim3_pwm_handle, TIM_CHANNEL_1);
}

void ctrl_stop()
{

}

void gpio_m1_p1_on()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
}

void gpio_m1_p1_off()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
}

void gpio_m1_p2_on()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}

void gpio_m1_p2_off()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

void gpio_m2_p1_on()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void gpio_m2_p1_off()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void gpio_m2_p2_on()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
}

void gpio_m2_p2_off()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
}