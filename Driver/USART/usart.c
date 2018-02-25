#include "usart.h"	



int fgetc(FILE *f)								// 重定向c库函数scanf到USART1
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);	/* 等待串口1输入数据 */
	return (int)USART_ReceiveData(USART3);
}

int fputc(int ch, FILE *f)						// 重定向c库函数printf到USART1
{
	USART_SendData(USART3, (uint8_t) ch);		/* 发送一个字节数据到USART1 */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);			/* 等待发送完毕 */
	return (ch);
}

int GetKey (void)  
{ 
    while (!(USART1->SR & USART_FLAG_RXNE));
    return ((int)(USART1->DR & 0x1FF));
}

void USART_Configuration(void)						//串口1、2、3初始化
{				
	USART1_Config();    							// 用作与ESP8266通信
	USART2_Config();								// 用作与ZigBee通信
	USART3_Config();								// 用作与PC机通信
	USART1_NVIC_Configuration();
	USART2_NVIC_Configuration();
	USART3_NVIC_Configuration();
}
  
/*
 * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * @param  无
 * @retval 无
 */
static void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	/* config USART1 clock */
	
	/* USART1 GPIO config */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能串口1接收中断 */
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_Cmd(USART1, ENABLE);
}


/*
 * @brief  USART2 GPIO 配置,工作模式配置。115200 8-N-1
 * @param  无
 * @retval 无
 */
static void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;      
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	/* USART2 GPIO config */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;					/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);				/* 使能串口2接收中断 */
	USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_Cmd(USART2, ENABLE);
}

/*
 * @brief  USART3 GPIO 配置,工作模式配置。115200 8-N-1
 * @param  无
 * @retval 无
 */
static void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;      
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);   	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	/* USART1 GPIO config */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* 使能串口3接收中断 */
	USART_ClearFlag(USART3,USART_FLAG_TC);
	USART_Cmd(USART3, ENABLE);
}

/*
 * @brief  配置USART1接收中断
 * @retval 无
 */
static void USART1_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			/* Configure the NVIC Preemption Priority Bits */  

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			/* Enable the USART1 Interrupt */
	NVIC_Init(&NVIC_InitStructure);
}


/*
 * @brief  配置USART2接收中断
 * @retval 无
 */
static void USART2_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			/* Configure the NVIC Preemption Priority Bits */  
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			/* Enable the USART2 Interrupt */
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * @brief  配置USART3接收中断
 * @retval 无
 */
static void USART3_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			/* Configure the NVIC Preemption Priority Bits */  
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			/* Enable the USARTy Interrupt */
	NVIC_Init(&NVIC_InitStructure);
}

static char *itoa( int value, char *string, int radix )
{
    int     i, j, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix == 10)
    {
			if (!value)
			{
					*ptr++ = 0x30;
					*ptr = 0;
					return string;
			}

			/* if this is a negative value insert the minus sign. */
			if (value < 0)
			{
					*ptr++ = '-';

					/* Make the value positive. */
					value *= -1;
			}

			for (i = 10000; i > 0; i /= 10)
			{
					d = value / i;

					if (d || flag)
					{
							*ptr++ = (char)(d + 0x30);
							value -= (d * i);
							flag = 1;
					}
			}

			/* Null terminate the string. */
			*ptr = 0;

			return string;
		}
		// hex
		else if(radix==16)
		{
			if (!value)
			{
					*ptr++ = 0x30;
					*ptr++ = 0x30;
					*ptr = 0;
					return string;
			}
			
			if(value<10)
			{
					*ptr++ = 0x30;
					*ptr++ = '0' + value;
					*ptr = 0;
					return string;
			}
			
			i=0;
			while(value!=0)
			{
				i++;
				
				d = value % 16;
				value /= 16;
				if(d<10)
					*ptr++ = '0' + d;
				
				if(d>=10 && d<=15)
					*ptr++ = 'A' + d - 10;				
			}
			
			// 反转字符串
			for(j=i/2-1; j>=0; j--)
			{
				d = string[j];
				string[j] = string[i-j-1];
				string[i-j-1] = d;
			}
			
			/* Null terminate the string. */
			*ptr = 0;
			return string;			
		}
		else
		{
			/* Null terminate the string. */
			*ptr = 0;

			return string;
		}			
} /* NCL_Itoa */

/*
 * 函数名：USART_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART_printf( USART_TypeDef* USARTx, uint8_t *Data, ... )
{
	const char *s;
	int d;   
	char buf[16];
	va_list ap;
	va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
          d = va_arg(ap, int); 
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;
					
				case 'x':										//十六进制
          d = va_arg(ap, int); 
          itoa(d, buf, 16);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	}
}

