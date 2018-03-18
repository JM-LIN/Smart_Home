#include "sim7600.h"


/************************************************************************
* Global Variable Declare Section   
************************************************************************/
//中断缓存串口数据
#define     UART_BUFF_SIZE      255
volatile    uint8_t uart_p = 0;
uint8_t     uart_buff[UART_BUFF_SIZE];

const char num[]=   "13232963434";          							//  电话号码
const char ip[]=    "207.246.97.145";									//  IP地址 207.246.97.145
const char port[]=  "33333";											//  端口 33333


extern volatile unsigned char APP_buffer[];		                        // 用于存储APP显示信息


void SIM7600_Init(void)
{   
    SIM7600_Reset_Init();
    
	SIM7600_Config();	                        //初始化并检测模块响应是否正常
	SIM900A_DELAY(2000);

    // 拨号
// 	SIM7600_Call((char *)num);
	
	server_connect((char *)ip,(char *)port);
	SIM7600_GPRS_Send("TMThis_HMis_SMSa_LSstarting_WEus_SSOtest_SST!_HTtest_ARby_PMJM.Lin");

    // 发送短信
// 	sim900a_sms((char *)num,"Hello World JM.Lin");
// 	PC_Usart((unsigned char*)"\r\n短信已经发送至：%s\r\n",num);
    
}

void SIM7600_IRQHandler_Routine(void)
{
    if(uart_p<UART_BUFF_SIZE)       // 判断是否缓存是否溢出
    {
//        char ch;
        if(USART_GetITStatus(SIM7600_Usart_Port, USART_IT_RXNE) != RESET)
        {
            uart_buff[uart_p] = USART_ReceiveData(SIM7600_Usart_Port);
//            ch = uart_buff[uart_p];
//            USART_SendData(PC_Usart_Port, ch);      // 转发给 PC串口
            uart_p++;     
        }
    }
}

static void SIM7600_Reset_Init(void)											// 蜂鸣器
{
	GPIO_InitTypeDef GPIO_Initstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_Initstructure);
		
    SIM7600_RST_HIGH_LEVEL();
    SIM900A_DELAY(50);
    SIM7600_RST_LOW_LEVEL();    
}

void SIM7600_Data_Send(void)
{
	char *redata;
	SIM900A_CLEAN_RX();
	SIM7600_Usart( (unsigned char*)"AT+CIPSEND=0,\r");
  
    redata=SIM7600_Waitask(0);
	PC_Usart((unsigned char*)"%s\r\n",redata);
	
	SIM900A_CLEAN_RX();
    SIM7600_Usart( (unsigned char*)"%s",APP_buffer);
	while((SIM7600_Usart_Port->SR&0X40)==0);//等待上一次数据发送完成  
	SIM7600_Usart_Port->DR=(u32)0x1A;		//发送十六进制数：0X1A,信息结束符号
	
	redata=SIM7600_Waitask(0);	
	SIM900A_DELAY(1000);
	bufcheck();
	return;
}

//0表示成功，1表示失败
uint8_t SIM7600_CMD(char *cmd, char *reply,uint32_t waittime )
{    
    SIM900A_CLEAN_RX();                 //清空了接收缓冲区数据

    SIM900A_TX(cmd);                    //发送命令

    if(reply == 0)                      //不需要接收数据
    {
        return SIM900A_TRUE;
    }
    
    SIM900A_DELAY(waittime);                 //延时

    return SIM7600_CMD_Check(reply);    //对接收数据进行处理
}


//0表示成功，1表示失败
uint8_t SIM7600_CMD_Check(char *reply)
{
    uint8_t len;
    uint8_t n;
    uint8_t off;
    char *redata;
    
    redata = SIM900A_RX(len);   //接收数据
    n = 0;
    off = 0;
    while((n + off)<len)
    {
        if(reply[n] == 0)                 //数据为空或者比较完毕
        {
            return SIM900A_TRUE;
        }
        
        if(redata[ n + off]== reply[n])
        {
            n++;                //移动到下一个接收数据
        }
        else
        {
            off++;              //进行下一轮匹配
            n=0;                //重来
        }
    }

    if(reply[n]==0)   //刚好匹配完毕
    {
        return SIM900A_TRUE;
    }
    
    return SIM900A_FALSE;       //跳出循环表示比较完毕后都没有相同的数据，因此跳出
}

char * SIM7600_Waitask(uint8_t waitask_hook(void))      //等待有数据应答
{
    uint8_t len=0;
    char *redata;
    do{
        redata = SIM900A_RX(len);   //接收数据
        if(waitask_hook!=0)
        {
            if(waitask_hook()==SIM900A_TRUE)           //返回 SIM900A_TRUE 表示检测到事件，需要退出
            {
                redata = 0;
                return redata;               
            }
        }
    }while(len==0);                 //接收数据为0时一直等待
    
    
    SIM900A_DELAY(20);              //延时，确保能接收到全部数据（115200波特率下，每ms能接收11.52个字节）
    return redata;
}


//检测模块是否正常
void SIM7600_Config(void)
{
	while(SIM7600_CMD("AT\r","OK",100) != SIM900A_TRUE)
	{
		PC_Usart((unsigned char*)"AT_ERROR\r\n");
	}
	PC_Usart((unsigned char*)"AT_SUC\r\n");
	
	while(SIM7600_CMD("AT+CPIN?\r","READY",400) != SIM900A_TRUE)
	{
		PC_Usart((unsigned char*)"CPIN_ERROR\r\n");
	}
	PC_Usart((unsigned char*)"CPIN_SUC\r\n");
	
	while(SIM7600_CMD("AT+CREG?\r","0,1",400) != SIM900A_TRUE)
	{
		PC_Usart((unsigned char*)"CREG_ERROR\r\n");
	}
	PC_Usart((unsigned char*)"CREG_SUC\r\nSIM_OK\r\n\r\n");
}

/*---------------------------------------------------------------------*/

//发起拨打电话
void SIM7600_Call(char *num)
{
    SIM900A_CLEAN_RX();                 //清空了接收缓冲区数据
    SIM7600_Usart( (unsigned char*)"ATD%s;\r",num);
}

//发送短信
void SIM7600_SMS(char *num,char *smstext)
{
	SIM900A_CLEAN_RX();                 //清空了接收缓冲区数据
	
	SIM7600_Usart( (unsigned char*)"AT+CMGF=1\r");     //文本模式
	SIM900A_DELAY(100);
	
	SIM7600_Usart( (unsigned char*)"AT+CSCS=\"GSM\"\r");           
	SIM900A_DELAY(100);
	
	SIM7600_Usart( (unsigned char*)"AT+CSCA?\r");           
	SIM900A_DELAY(100);
	
	SIM7600_Usart( (unsigned char*)"AT+CSMP=17,167,0,241\r");           
	SIM900A_DELAY(100);
	
	SIM7600_Usart( (unsigned char*)"AT+CMGS=\"%s\"\r",num);  //电话号码
	SIM900A_DELAY(100);

	SIM7600_Usart( (unsigned char*)"%s",smstext);            //短信内容        

    SIM900A_DELAY(1);   
	SIM7600_Usart_Port->DR=(u32)0x1A;		//发送十六进制数：0X1A,信息结束符号
}

void SIM7600_GPRS_Send(char * str)
{
	char *redata;
	SIM900A_CLEAN_RX();
	SIM7600_Usart( (unsigned char*)"AT+CIPSEND=0,\r");
  
    redata=SIM7600_Waitask(0);
	PC_Usart((unsigned char*)"%s\r\n",redata);
	
	SIM900A_CLEAN_RX();
    SIM7600_Usart( (unsigned char*)"%s",str);
	while((SIM7600_Usart_Port->SR&0X40)==0);//等待上一次数据发送完成  
	SIM7600_Usart_Port->DR=(u32)0x1A;		//发送十六进制数：0X1A,信息结束符号
	
	redata=SIM7600_Waitask(0);	
	SIM900A_DELAY(1000);
	bufcheck();
	return;
}

void server_connect(char *ipaddr,char *port)
{
	char i;
	i=server_connect_tcp(ipaddr,port);
	while(i)
	{
		i=server_connect_tcp(ipaddr,port);
	}
	PC_Usart((unsigned char*)"connect_suc\r\n");
}

char server_connect_tcp(char *ipaddr,char *port)
{
	SIM900A_CLEAN_RX();
	SIM7600_Usart( (unsigned char*)"AT+CGREG?\r");    
	SIM900A_DELAY(200);
	if(SIM7600_CMD_Check("0,1"))
	{
		PC_Usart((unsigned char*)"CGREG_ERROR\r\n");
		return 1;
	}
	
	SIM900A_CLEAN_RX();
	SIM7600_Usart( (unsigned char*)"AT+CGSOCKCONT=1,\"IP\",\"cmnet\"\r");    
	SIM900A_DELAY(200);
	if(SIM7600_CMD_Check("OK"))
	{
		PC_Usart((unsigned char*)"CGSOCKCONT_ERROR\r\n");
		return 2;
	}
	
	SIM900A_CLEAN_RX();
	SIM7600_Usart( (unsigned char*)"AT+CSOCKSETPN=1\r");    
	SIM900A_DELAY(600);
	if(SIM7600_CMD_Check("OK"))
	{
		PC_Usart((unsigned char*)"CSOCKSETPN_ERROR\r\n");
		return 3;
	}
	
	SIM900A_CLEAN_RX();
	SIM7600_Usart( (unsigned char*)"AT+NETOPEN\r");    
	SIM900A_DELAY(1000);
		bufcheck();
	if(SIM7600_CMD_Check("+NETOPEN: 0"))
	{
		bufcheck();
		SIM900A_CLEAN_RX();
		SIM7600_Usart( (unsigned char*)"AT+NETCLOSE\r"); 
		SIM900A_DELAY(1000);
		bufcheck();
		if(SIM7600_CMD_Check("+NETCLOSE: 0"))
		{
			bufcheck();
		}
		return 4;
	}
	
	SIM900A_CLEAN_RX();
	SIM7600_Usart( (unsigned char*)"AT+IPADDR\r");    
	SIM900A_DELAY(500);
	if(SIM7600_CMD_Check("OK"))
	{
		bufcheck();
		return 5;
	}
	
	SIM900A_CLEAN_RX();
	SIM7600_Usart( (unsigned char*)"AT+CIPOPEN=0,\"TCP\",\"%s\",%s\r",ipaddr,port);  
	SIM900A_DELAY(1000);
	if(SIM7600_CMD_Check("+CIPOPEN: 0,0"))
	{
		PC_Usart((unsigned char*)"CIPOPEN_ERROR\r\n");
		return 6;
	}
	return 0;
}


static void bufcheck()
{
	char *redata;
    uint8_t len;  
    redata = SIM900A_RX(len);   //接收数据
 	PC_Usart((unsigned char*)"%s\r\n",redata);
}


static char *get_rebuff(uint8_t *len)       //获取接收到的数据和长度
{
    *len = uart_p;
    return (char *)&uart_buff;
}

static void clean_rebuff(void)
{
    uint16_t i=UART_BUFF_SIZE+1;
    uart_p = 0;
	while(i)
		uart_buff[--i]=0;
}

/*---------------------------------------------------------------------*/
