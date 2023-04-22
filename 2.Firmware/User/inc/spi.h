
#ifndef _SPI_H
#define _SPI_H


#include "headfiles.h"

#define SPI_GPIO                        GPIOA
#define SPI_RCC_GPIO_CLK                RCC_AHB1Periph_GPIOA
#define SENSORS_SPI                     SPI1
#define RCC_SPI_APBx_ClockCmd           RCC_APB2PeriphClockCmd
#define SPI_CLK                         RCC_APB2Periph_SPI1
#define SPI_GPIO_AF                     GPIO_AF_SPI1

#define SPI_SCK_PinSource      GPIO_PinSource5
#define SPI_MOSI_PinSource     GPIO_PinSource7
#define SPI_MISO_PinSource     GPIO_PinSource6

#define SPI_CS_PIN                     GPIO_Pin_4
#define SPI_SCK_PIN                    GPIO_Pin_5
#define SPI_MOSI_PIN                   GPIO_Pin_7
#define SPI_MISO_PIN                   GPIO_Pin_6 

#define SPIT_FLAG_TIMEOUT              ((uint32_t)0x1000)

#define SPI_INFO(fmt,arg...)           printf("<<-SPI-INFO->> "fmt"\n",##arg)

#define SPI_ERROR(fmt,arg...)          printf("<<-SPI-ERROR->> "fmt"\n",##arg)

#define SPI_DEBUG(fmt,arg...)            do{                                        \
                                             if(SPI_DEBUG_ON)                                       \
                                               printf("<<-SPI-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);  \
                                           }while(0)

#define CS_LOW()	  GPIO_ResetBits(SPI_GPIO,SPI_CS_PIN)
#define CS_HIGH()   GPIO_SetBits(SPI_GPIO,SPI_CS_PIN)

void SPI_Config(void);
uint8_t SPI_WR_Byte(uint8_t byte);



#endif
