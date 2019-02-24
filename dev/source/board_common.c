#include "main.h"

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
	while (1) { }
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(char *file, uint32_t line)
{
	/* User can add his own implementation to report the file name
	 * and line number ex:
	 * printf("Wrong parameters value: file %s on line %d\r\n", file, line)
	 */

	/* XXX: Infinite loop */
	while (1) { }
}

#endif

