




#include "headfiles.h"
/**
  * @brief  Main Entrance
  * @param  none
  * @retval Never returns if OS starts  
	* @date   2020-06-28
  */
int main(void)
{	
	BSP_Init();
	
	Parameters_Init();
	
	
	if(READ_KEY()==ON)
	 com_console_ui();

	
  OS_Start(); 
	
}















