/* Includes ------------------------------------------------------------------*/
#include "init.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
/* Private typedef -----------------------------------------------------------*/
#define PERIOD_VALUE		0xFFFF				/* Period Value  */
#define PWM_PERIOD_VALUE	100
/* Private define ------------------------------------------------------------*/
#define PWM_PULSE_VALUE    (PWM_PERIOD_VALUE/2)	/* Duty cycle 50%  */

/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/

/* PWM variables */
TIM_HandleTypeDef tim_pwm_handle;
TIM_OC_InitTypeDef pwm_conf;

/* TIM_Base variables */
TIM_HandleTypeDef tim_base_handle;

/* Delay variable */
uint16_t tickstart;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
void error_handling(const char *error_string, uint8_t error_code);

void time_base_init()
{
	/* Compute the prescaler value to have TIM2 counter clock equal to 1742 Hz, period Time 574 micro sec */
	uint32_t prescalervalue = (uint32_t)((SystemCoreClock) / 1742) - 1;

	/* Set TIM2 instance */
	tim_base_handle.Instance = TIM2;

	/* Initialize TIMx peripheral as follows:
	   + Period = 65535
	   + Prescaler = ((SystemCoreClock)/1742) - 1
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	tim_base_handle.Init.Period            = PERIOD_VALUE;
	tim_base_handle.Init.Prescaler         = prescalervalue;
	tim_base_handle.Init.ClockDivision     = 0;
	tim_base_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	tim_base_handle.Init.RepetitionCounter = 0;

	if (HAL_TIM_Base_Init(&tim_base_handle) != HAL_OK) {
		error_handling("TIM Base initialization has failed!", HAL_ERROR);
	}
	if (HAL_TIM_Base_Start(&tim_base_handle) != HAL_OK)	{
		error_handling("Starting TIM Base has failed!", HAL_ERROR);
	}
}

void delay(uint16_t delay_value)
{
	tickstart = __HAL_TIM_GET_COUNTER(&tim_base_handle);

	if ((delay_value + tickstart) > 0xFFFF) {
		while (__HAL_TIM_GET_COUNTER(&tim_base_handle) != 0);
		while (__HAL_TIM_GET_COUNTER(&tim_base_handle) != delay_value - (0xFFFF - tickstart));
	} else {
		while ((__HAL_TIM_GET_COUNTER(&tim_base_handle) - tickstart) < delay_value);
	}
}

void pwm_init()
{
	/* Compute the prescaler value to have TIM3 counter clock equal to 38 kHz */
	uint8_t prescalervalue = 20;

	tim_pwm_handle.Instance = TIM3;

	/* Initialize TIMx peripheral as follows:
	   + Prescaler = (SystemCoreClock / 380000) - 1
	   + Period = 100
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	tim_pwm_handle.Init.Prescaler         = prescalervalue;
	tim_pwm_handle.Init.Period            = PWM_PERIOD_VALUE;
	tim_pwm_handle.Init.ClockDivision     = 0;
	tim_pwm_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	tim_pwm_handle.Init.RepetitionCounter = 0;

	if (HAL_TIM_PWM_Init(&tim_pwm_handle) != HAL_OK) {
		error_handling("TIM PWM initialization has failed!", HAL_ERROR);
	}
	pwm_conf.OCFastMode = TIM_OCFAST_DISABLE;
	pwm_conf.OCIdleState = TIM_OCIDLESTATE_RESET;
	pwm_conf.OCMode = TIM_OCMODE_PWM1;
	pwm_conf.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_conf.Pulse = PWM_PULSE_VALUE;

	if (HAL_TIM_PWM_ConfigChannel(&tim_pwm_handle, &pwm_conf, TIM_CHANNEL_1) != HAL_OK)	{
		error_handling("TIM PWM channel configuration has failed!", HAL_ERROR);
	}
}

void error_handling(const char *error_string, uint8_t error_code)
{
	printf("Error: %s Error code: %d", error_string, error_code);
	BSP_LED_On(LED2);
}
