

#include "common.h"

const char table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };


/**
  *@brief  convert float to string
  * 
  *@param  str: target string
  *        data: float data to be converted
  *		   decimal: eps value
  *				 
  *@retval none  
  *@date   2021-04-24
  */
void float2str(char *str, float data,  uint8_t decimal)
{
	int8_t i;
	uint32_t temp;
	float t2 = 0.0f;
	uint8_t digit;
	uint8_t minus_flag = 0;

	
	if(data<0.0f)
	{
		data = -data;
    minus_flag = 1;
	}
	
	if (data < 10.0f)
		digit = 1;

	else if (data >= 10.0f && data < 100.0f)
		digit = 2;
	else
		digit = 3;
	
	temp = (int)data;


	for (i = 1; i <= digit; i++)
	{
		if (temp == 0)
			str[digit - i] = table[0];
		else
			str[digit - i] = table[temp % 10];
		temp = temp / 10;
	}


	*(str + digit) = '.';


	temp = 0;
	t2 = data;
	for (i = 1; i <= decimal; i++)
	{
		temp = t2 * 10;
		str[digit + i] = table[temp % 10];
		t2 = t2 * 10;
	}
	*(str + digit + decimal + 1) = '\0';
	

	
	if(minus_flag)
	{
		for(i=10;i>0;i--)
		 str[i] = str[i-1];
	 
		str[0] = '-';
	}
	
}


/**
  *@brief  copy array 
  * 
  *@param  dst: dst array
  *        src: src array
  * 		   len: length of array
  *			 times: copy times
  *@retval none  
  *@date   2022-01-06
  */
void myarrcpy(char* dst,const char* src,uint16_t len,uint16_t times)
{
	uint8_t i = 0,j = 0,num = len*times;
	
  while(j<num)
	{
		while(i<len)
		 dst[j++] = src[i++];
		i = 0;
	}
}

/**
  *@brief  replace 0 with 0x80
  *@param  s: string to be processed
  *      len: length of string including 0
  *@retval none  
  *@date   2022-01-07
  */
void myreplace0(char* s, uint16_t len)
{
	while(len--)
	{
		if(s[len]==0)
			s[len] = 0x80;
	}
		
}
	
	
	
	
	
	
	
