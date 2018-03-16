#include "SYN7318.h"
#include "usart.h"
#include "string.h"
#include "OLED.h"

unsigned char Back[4] = {0};   //接收命令回传的数组
unsigned char ASR[6] = {0};    //接收识别结果回传的数组
unsigned char S[4] = {0};      //接收模块当前工作状态回传的数组

char D_MP3_1[] = {"D:\\Mp3\\Wifi_首次开机.mp3"};
char D_MP3_2[] = {"D:\\Mp3\\Wifi_我在这.mp3"}; 
char D_MP3_3[] = {"D:\\Mp3\\Prompt_嘀.mp3"}; 
char D_MP3_4[] = {"D:\\Mp3\\Prompt_我在呢.mp3"};
char TF_MP3_1[] = {"E:\\1-歌曲\\一条大鱼不见了.mp3"};  

char Test[] = {"雷猴"};

char *Dict0[] = {"音量加","音量减","最大音量","最小音量","打开提示音","关闭提示音","请开灯","请关灯",
                "调暗一点","调亮一点","最小亮度","最大亮度"};           //00词典

char ** DICT[10] = {Dict0}; //可以添加其他词典


void Voice_Control(void)
{
    const char num[]=   "15768088230";   
    const char mynum[]= "13232963434";
    
    static uint16_t fan_speed = 0;
    
    
    USART3_Config();	//去掉发现不好用
    
    // 0x00 云宝
    // 0x1F 你在干嘛
    //唤醒名,唤醒成功后播放“我在呢”,之后可以识别0x00词典中的词条
    Three_One(0x03, 0x1F, 0x02, D_MP3_4);   
    switch (ASR[3])
    {
        case 0x06:  // 请开灯
            TIM_Cmd(TIM1, ENABLE);              // 打开定时器1
            Pwm_led_status = OPEN;              // 开灯
            SYN_TTS("为你开灯");
            delay_ms(2000);
            break;
       
        case 0x07:  // 请关灯
            TIM_Cmd(TIM1, ENABLE);
            Pwm_led_status = CLOSE;              // 关灯
            SYN_TTS("为你关灯");
            delay_ms(2000);
            break;
       
        case 0x11:  // 打开风扇
            TIM_Cmd(TIM4, ENABLE);
            SYN_TTS("打开换气");
            TIM_SetCompare2(TIM4, fan_speed);
            delay_ms(2000);
            break;
        case 0x13:  // 关闭风扇
            TIM_Cmd(TIM4, ENABLE);
            SYN_TTS("关闭换气");
            TIM_SetCompare2(TIM4, 0);
            delay_ms(2000);
            break;
        case 0x16:  // 中速转动
            SYN_TTS("风力加大");
            TIM_SetCompare2(TIM4, fan_speed+500);
            delay_ms(2000);
            break;

        case 0x1D:  // 开窗
            TIM_SetCompare3(TIM4, 50);		// 窗帘舵机转动
            SYN_TTS("窗户正在打开");		// 语音播报 窗户正在打开
            delay_ms(2000); 
            TIM_SetCompare3(TIM4, 53);		// 窗帘舵机停止
            break;
        case 0x1E:  // 关窗
            TIM_SetCompare3(TIM4, 56); 		// 窗帘舵机转动
            SYN_TTS("窗户正在关闭");		// 语音播报 窗户正在关闭
            delay_ms(2000);
            TIM_SetCompare3(TIM4, 53);		// 窗帘舵机停止
            break;

        case 0x1F:  // 开门
            TIM_SetCompare4(TIM4, 15);
            SYN_TTS("正在开门");
            delay_ms(1500);	break;         
        case 0x20:  // 关门
            TIM_SetCompare4(TIM4, 7);
            SYN_TTS("正在关门");
            delay_ms(1500);	break;
            break;
        case 0x21:  // 太干燥了
            SYN_TTS("打开加湿器");
            delay_ms(2000);
            break;
        case 0x22:  // 关闭加湿器
            SYN_TTS("关闭加湿器");
            delay_ms(2000);
            break;
        
        case 0x23:  // 我回来了
            SYN_TTS("欢迎回家");
            delay_ms(2000);
            break;
        case 0x24:  // 我出门啦
            SYN_TTS("好的主人");
         	
            delay_ms(2000);
            break;
        case 0x25:  // 没事了
            SYN_TTS("好的");
            delay_ms(1000);
            break;
        case 0x26:  // 模块重启
            SYN_TTS("正在为你重启");
            delay_ms(2500);
            break;
        case 0x27:  // 添加指纹
            SYN_TTS("请把手指放正");
            delay_ms(2000);
            break;
        case 0x28:  // 打个电话
            sim900a_call((char *)mynum);
            SYN_TTS("正在拨号");
            delay_ms(2000);
            break;
        case 0x29:  // 预警
            SYN_TTS("预警模式");
            delay_ms(2000);
            break;
        case 0x2A:  // 警报
            SYN_TTS("警报模式");
            delay_ms(2000);
            break;
        case 0x2B:  // 发条信息
            SYN_TTS("信息发送中");
            sim900a_sms((char *)mynum,"This is a test");
            delay_ms(2000);
            break;
        case 0x2C:  // 数据联网测试
            SYN_TTS("GPRS正在打开");
            sim900a_gprs_send("TMwhat_HMthe_SMSfuck_LSare_WEyou_SSOtalking_SSTabout_HT?_ARlll_PMccc");
            delay_ms(2000);
            break;
        default:
            SYN_TTS("你的普通话还挺普通的");
            delay_ms(2500);
            break;
    }
}

void SYN7318_Reset_Init(void)											// 蜂鸣器
{
	GPIO_InitTypeDef GPIO_Initstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	GPIO_Initstructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_Initstructure);
	
    SYN7318_RST_LOW_LEVEL();
    delay_ms(50);
    SYN7318_RST_HIGH_LEVEL();
}

/****************************************************************************************
【函 数 名】：SYN_TTS----语音合成播放函数
【参数说明】：Pst：存放要合成播放的文本的数组名
【简    例】：SYN_TTS("北京龙邱"); //合成播放北京龙邱              
****************************************************************************************/
void SYN_TTS(char *Pst)
{
	uint16_t Length;
	char Frame[5];   //保存发送命令的数组
	
	Length = strlen(Pst);
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x01;      //语音合成播放命令
	Frame[4] = 0x00;      //播放编码格式为“GB2312”
	
    UART_Put_String(Frame,5);	
	UART_Put_String(Pst, Length);	
}


/*******************************************************************
【函 数 名】：SYN_MP3----MP3播放函数
【参数说明】：Pst：存放要播放的MP3的数组名
【简    例】：char D_MP3_2[] = {"D:\\Mp3\\Wifi_我在这.mp3"}; 
              SYN_MP3(D_MP3_2); //播放MP3
*******************************************************************/
void SYN_MP3(char *Pst)
{
	uint16_t Length;
	char Frame[5];   //保存发送命令的数组
	
	Length = strlen(Pst);
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x61;      //MP3播放命令
	Frame[4] = 0x00;      //播放编码格式为“GB2312”
	
    UART_Put_String(Frame, 5);	
	UART_Put_String(Pst, Length);	
}


/*********************************************************************
【函 数 名】：Start_ASR----开始语音识别函数
【参数说明】：Dict：词典编号
【简    例】：Start_ASR(0x00); //识别0x00词典中的词条
*********************************************************************/
void Start_ASR(char Dict)
{
	char Frame[5];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x10;      //开始语音识别命令
	Frame[4] = Dict;      //词典编号  在这里修改想要识别的词典编号
	
	ASR[3] = 0;
	
  UART_Put_String(Frame, 5);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		 UART_Get_String(Back,3);  //语音识别命令回传结果
		if(Back[0] == 0xfc) 
		{
			 UART_Get_String(ASR,Back[2]);
		}
	}
}

/***************************************************************************
【函 数 名】：Dict_TTS----识别词典结果播报函数
【参数说明】：Dict：词典编号
【简    例】：Start_ASR(0x00); //识别0x00词典中的词条
              Dict_TTS(0x00);  //如果识别成功,把识别到的词条播报出来 
***************************************************************************/
void Dict_TTS(char Dict)
{
	switch(ASR[0])
	{
		case 0x01:
		case 0x02:
		{
			SYN_TTS(DICT[Dict][ASR[3]]);  //ASR[3]中存放词条ID
		}break;
		case 0x03:
		{
			SYN_TTS("用户静音超时"); 
		}break;
		case 0x04:
		{
			SYN_TTS("用户语音超时");
		}break;
		case 0x05:
		case 0x07:
		{
			SYN_TTS("识别拒识");
		}break;
		case 0x06:
		{
			SYN_TTS("识别内部错误");
		}break;
	}
}

/*********************************************************************
【函 数 名】：Stop_ASR----停止语音识别函数
【参数说明】：无参
*********************************************************************/
void Stop_ASR(void)
{
	char Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x11;      //停止语音识别命令
	
  UART_Put_String(Frame, 4);
}



/*********************************************************************************************************************************************************
【函 数 名】：Start_WakeUp----开始语音唤醒函数
【参数说明】：Wake_ID：唤醒名ID号----0x00：云宝  0x02：小播   0x04：百灵  0x06：叮当管家  0x07：百灵管家  0x08：小播管家  0x09：大管家  0x1F：自定义
【简    例】：Start_WakeUp(0x09);//唤醒名为大管家,唤醒成功之后播报“我在这”
**********************************************************************************************************************************************************/
void Start_WakeUp(char Wake_ID)
{
	char Frame[5];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x51;      //开始语音唤醒命令
	Frame[4] = Wake_ID;   
	
  UART_Put_String(Frame, 5);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 UART_Get_String(Back,4);  //语音识别命令回传结果
		if(Back[3] == 0x21)
		{
			 SYN_MP3("D:\\Mp3\\Wifi_我在这.mp3");
		}
	}
}

/*********************************************************************************************************************************************************
【函 数 名】：Three_One----三合一函数：开启语音唤醒，唤醒后播放提示音，播完后开启语音识别
【参数说明】：Dict：词典编号
              Wake_ID：唤醒名ID号----0x00：云宝  0x02：小播   0x04：百灵  0x06：叮当管家  0x07：百灵管家  0x08：小播管家  0x09：大管家  0x1F：自定义
              Sound：提示音类型  0x00：无提示音  0x01：文本类型  0x02：内置MP3类型
              Pst：提示音的内容
【简    例】：Three_One(0x00, 0x00, 0x02, D_MP3_2);//唤醒名为云宝,唤醒成功后播放“我在这”,之后可以识别0x00词典中的词条 
              Dict_TTS(0x00); //结合Dict_TTS函数把识别结果播报出来
**********************************************************************************************************************************************************/
void Three_One(char Dict, char Wake_ID, char Sound, char *Pst)
{
	char Frame[100] = {0};   //保存发送命令的数组
	uint16_t Length,i;
	
	Frame[0] = 0xFD;      //帧头
//	Frame[1] = 0x00;
//	Frame[2] = 0x07;    //由于不确定数据区的长度，所以选择暂时屏蔽
	Frame[3] = 0x15;      //三合一识别命令
	Frame[4] = Dict;      //词典编号
	Frame[5] = Wake_ID;   //唤醒名ID号
	Frame[6] = Sound;     //提示音类型
	if(Sound == 0x00)     //无提示音
	{
		Frame[1] = 0x00;
		Frame[2] = 0x04;
	  UART_Put_String(Frame, 7);	
	}
	else                 //有提示音
	{
		Frame[7] = 0x01;      //文本或内置MP3提示音编码格式：0x00：GB2312编码  0x01：GBK编码  0x02：BIG5编码  0x03：Unicode小头  0x04nicode大头
		Length = strlen(Pst); //文本或内置MP3提示音的长度
		Frame[1] = 0x00;
		Frame[2] = Length + 5;
		for(i = 0; i < Length; i++)
		{
			Frame[8 + i] = Pst[i];
		}
	  UART_Put_String(Frame, Length + 8);	
	}
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		UART_Get_String(Back,3);  //语音识别命令回传结果
		if(Back[0] == 0xfc)
		{
			 UART_Get_String(ASR,Back[2]);
		}
	}
}

/***************************************************************************
【函 数 名】：Status_Query----模块状态查询函数
【参数说明】：无参
【简    例】：Status_Query(); 
***************************************************************************/
void Status_Query(void)
{
	char Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;    //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x21;   //状态查询命令
	
  UART_Put_String(Frame, 4);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 UART_Get_String(S,4);  //模块当前工作状态的回传结果
	}
}


/*****************************************以下为新增的功能（待测试）*************************************************/

/************************SYN语音合成缓存保存************************/
/*功能：针对一些比较长的内容可以先存储到缓存里面，再进行缓存播放      */
/*注意：缓存存储和缓存播放之间不允许有其他指令。                     */
/*输入值：Pst：存放要合成播放的文本的数组名                         */
/*       Block：语音合成缓存存储块选取，取值0--15                  */
/*****************************************************************/
void SYN_Buffer_Save(char *Pst, unsigned char Block)
{
	uint16_t Length;
	char Frame[5];   //保存发送命令的数组
	
	Length = strlen(Pst);
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x31;      //语音合成缓存存储指令
	Frame[4] = Block;      //语音合成缓存存储块选取，取值0--15
	
  UART_Put_String(Frame,5);			//发送
	UART_Put_String(Pst, Length);	
}

/************************SYN语音合成缓存播放************************/
/*功能：针对一些比较长的内容可以先存储到缓存里面，再进行缓存播放      */
/*注意：缓存存储和缓存播放之间不允许有其他指令。                     */
/*输入值：Times：播放次数                                         */
/*****************************************************************/
void SYN_Buffer_TTS(unsigned char Times)
{
	char Frame[5];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;
	Frame[3] = 0x32;      //语音合成缓存存储播放指令
	Frame[4] = ((Times<<4)|0x00);      //高4位是播放次数 低四位是编码格式
	
  UART_Put_String(Frame,5);			//发送
}

/************************SYN播放控制命令***************************/
/*功能：可以对以下三类命令控制停止、暂停和恢复播放                  */
/*      1、语音合成播放命令                                       */
/*      2、文本缓存播放命令                                       */
/*      3、MP3播放命令                                           */
/*输入值：command：                                              */
/*  			1、Stop_Play  停止播放                                 */
/*        2、Pause_Play  暂停播放                                */
/*				3、Recover_Play  恢复播放                              */
/****************************************************************/
void SYN_Play_Control(unsigned char command)
{
	char Frame[4];		//保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;
	Frame[3] = command;      //播放命令
	
	UART_Put_String(Frame,4);			//发送
}

/**********************SYN播放音量设置命令*************************/
/*功能：可以对以下三类命令的播放音量进行更改                       */
/*      1、语音合成播放命令                                       */
/*      2、文本缓存播放命令                                       */
/*      3、MP3播放命令                                           */
/*输入值：Vol：音量大小级别，取值0x00-0x09	                       */
/*****************************************************************/
void SYN_VOL_Control(unsigned char Vol)
{
	char Frame[5];		//保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;
	Frame[3] = 0x05;      //设置音量命令
	Frame[4] = Vol;				//音量设置0x00--0x09
	
	UART_Put_String(Frame,5);			//发送
}

/*************************************关于停止的功能（待测试）******************************************/

/**************************停止语言唤醒****************************/
/*功能：停止当前的语言唤醒                                         */
/*输入值：无                                                      */
/*****************************************************************/
void Stop_WakeUp(void)
{
	char Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;
	Frame[3] = 0x52;      //停止语言唤醒命令
	
  UART_Put_String(Frame,4);			//发送
}

/**************************停止三合一识别***************************/
/*功能：停止当前的三合一识别                                       */
/*输入值：无                                                      */
/*****************************************************************/
void Stop_Three_One(void)
{
	char Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;
	Frame[3] = 0x16;      //停止三合一命令
	
  UART_Put_String(Frame,4);			//发送
}

