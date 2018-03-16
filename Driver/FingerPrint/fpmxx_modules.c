#include "fpmxx_modules.h"



unsigned char volatile FPMXX_RECEICE_BUFFER[54];



unsigned char FPMXX_Get_Device[6] = {0x01,0x00,0x03,0x16,0x00,0x1A};
unsigned char FPMXX_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //


unsigned char FPMXX_Get_Img[6] = {0x01,0x00,0x03,0x01,0x0,0x05};                    //录指纹图像
unsigned char FPMXX_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 };        //读有效的模板个数
unsigned char FP_Search[11]={0x01,0x0,0x08,0x04,0x01,0x0,0x0,0x03,0xE7,0x0,0xF8}; //查找起始页0，页数999，即0 - 999,在缓冲区1BUFFER1进行查找
unsigned char FP_Img_To_Buffer1[7]={0x01,0x0,0x04,0x02,0x01,0x0,0x08}; //图像生成特征BUFFER1
unsigned char FP_Img_To_Buffer2[7]={0x01,0x0,0x04,0x02,0x02,0x0,0x09}; //图像生成特征BUFFER2
unsigned char FP_Reg_Model[6]={0x01,0x0,0x03,0x05,0x0,0x09}; //合成特征模板
unsigned char FPMXX_Write_Notepad[39]={0x01,0x0,0x24,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//记事本写入
unsigned char FPMXX_Read_Notepad[7]={0x01,0x0,0x04,0x19,0x00,0x00,0x1e};//记事本读出
unsigned char FPMXX_Delete_All_Model[6]={0x01,0x0,0x03,0x0d,0x00,0x11};//清空指纹库
//volatile 
unsigned char  FP_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x00,0x00,0x19};//存储模板BUFFER1  store
//volatile unsigned char FP_Delete_Model[10]={0x01,0x00,0x07,0x0C,0x0,0x0,0x0,0x1,0x0,0x0}; //删除模板
//volatile unsigned char FINGER_NUM;


void FPM_Running(void)
{
    int id;
    id=FPM_Search();						//指纹查询函数
    
    if(id==-1)
    {
        
    }    
    else 
    {
        
        PC_Usart((unsigned char *)"%d",id);        //用来测试时通过串口打印指纹id
    }
}

/////////////////////////////////////////////
//录入指纹，7S内传感器上无手指或其他错误退出循环
//参数：无
//返回值：  0成功   1失败
/////////////////////////////////////////////
u8 PFM_Get_img(void)
{
    FPMXX_Send_Cmd(6,FPMXX_Get_Img,12);   //录指纹图像
    if(FPMXX_RECEICE_BUFFER[9]==0)			//确认码使接收数组的第10个字节
        return 0;												//录指纹图像成功，返回 0
    else
        return 1;    //录指纹图像失败，返回 1
}


/////////////////////////////////////////////
//生成特征1
//参数：无
//返回值：  0成功   1失败
/////////////////////////////////////////////
u8 FPM_I_TO_B1(void)
{
    FPMXX_Send_Cmd(7,FP_Img_To_Buffer1,12);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//确认码使接收数组的第10个字节
        return 0;
    else return 1;
}


/////////////////////////////////////////////
//生成特征2
//参数：无
//返回值：  0成功   1失败
/////////////////////////////////////////////
u8 FPM_I_TO_B2(void)
{
    FPMXX_Send_Cmd(7,FP_Img_To_Buffer2,12);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//确认码使接收数组的第10个字节
        return 0;
    else return 1;
}






/////////////////////////////////////////////
//特征合成
//参数：无
//返回值：  0成功   1失败
/////////////////////////////////////////////
u8 FPM_Reg_Model(void)
{
    FPMXX_Send_Cmd(6,FP_Reg_Model,12);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//确认码使接收数组的第10个字节
        return 0;
    else return 1;
}




/////////////////////////////////////////////
//记事本存储
//参数：ID值
//返回值：  0成功   1失败
/////////////////////////////////////////////
u8 FPM_Write_pad(u16 id)
{		
    FPMXX_Write_Notepad[5]=id>>8;							//先传高8位
    FPMXX_Write_Notepad[6]=id&0x00ff;					//再传低8位
//    FPMXX_Write_Notepad[5]=8;							//先传高8位
//    FPMXX_Write_Notepad[6]=8;
//    FPMXX_Write_Notepad[37]=0;				//校验和高8位
//    FPMXX_Write_Notepad[38]=77;				//检验和低8位
	
    FPMXX_Write_Notepad[37]=(61+id)/256;				//校验和高8位
    FPMXX_Write_Notepad[38]=(61+id)%256;			
    FPMXX_Send_Cmd(39,FPMXX_Write_Notepad,12);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//确认码使接收数组的第10个字节
        return 0;
    else return 1;
}



/////////////////////////////////////////////
//记事本读出 
//参数：ID值
//返回值：  ID值成功   -1失败
/////////////////////////////////////////////
u16 FPM_Read_pad(void)
{		
    int id;
    FPMXX_Send_Cmd(7,FPMXX_Read_Notepad,44);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//确认码使接收数组的第10个字节
    {
        id  =   FPMXX_RECEICE_BUFFER[10];        
        id +=   FPMXX_RECEICE_BUFFER[11];
        return id;
    }
    else return -1;
}


/////////////////////////////////////////////
//指纹存储
//参数：无
//返回值：  0成功   1失败
/////////////////////////////////////////////
u8 FPMXX_Cmd_Save_Finger(void)
{
    u16 id;
    id=FPM_Read_pad();
    id++;
    if(id==1000)
    id=200;
    FPM_Write_pad(id);
    FP_Save_Finger[5]=(id/256+id%256)/256;
    FP_Save_Finger[6]=(id/256+id%256)%256;
    FP_Save_Finger[7]=(14+id/256+id%256)/256;
    FP_Save_Finger[8]=(14+id/256+id%256)%256;
    FPMXX_Send_Cmd(9,FP_Save_Finger,12);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//确认码使接收数组的第10个字节
        return 0;
    else return 1;
}


/////////////////////////////////////////////
//指纹寻找
//参数：无
//返回值：  id成功   -1失败
/////////////////////////////////////////////
int FPM_Search(void)
{		
    u8 i,j=50;
    int id;

	//录入指纹图像	GET——IMG 
	while(j--)			//大概等待6——7秒等待指纹图像获取
	{		
		i=PFM_Get_img();
		if(i==0)
			break;
	}
	if(j==0)
		return -1;
		
	
	if(FPM_I_TO_B1())
		return -1;
	
    FPMXX_Send_Cmd(11,FP_Search,16);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//确认码使接收数组的第10个字节
    {
        id=FPMXX_RECEICE_BUFFER[10]*256+FPMXX_RECEICE_BUFFER[11];
        return id;
    }
    else return -1;
}


/////////////////////////////////////////////
//指纹录入整个过程
//参数：无                     //如果觉得a的值太大可以修改为2，因为总共a次获取指纹，一次6-7秒
//返回值：  0成功   1失败
/////////////////////////////////////////////
u8 FPMXX_Input(void)
{
    u8  i,j,a;
    //三次指纹输入机会
    a=4;                        
    while(--a)
    {
        j=50;
        //录入指纹图像	GET——IMG 
        while(--j)			//大概等待6——7秒等待指纹图像获取
        {		
            i=PFM_Get_img();
            if(i==0)
                break;
        }
        if(j==0)				            //假如等待6~7秒依然没有指纹录入，跳过次此步骤，同时a--即总的检查次数减一
            continue;
        if(FPM_I_TO_B1()){;}                //生成特征1	//，存在即使上面的指纹图像没录取到也能正常生成特征，暂时无法解决，//判断j等于0解决此次BUG
        
        while(--j)                          //录入指纹图像	GET——IMG 
        {		
            i=PFM_Get_img();
            if(i==0)
                break;
        }
        if(j==0)				            //假如等待6~7秒依然没有指纹录入，跳过次此步骤，同时a--即总的检查次数减一
            continue;
        if(FPM_I_TO_B2()){;}                //生成特征2
        if(FPM_Reg_Model()){continue;}      //特征文件合并，假如合并失败,两枚指纹不是同一根手指的，将会执行语句块,即不执行下面的存储函数，重新开始录入指纹
        else {FPMXX_Cmd_Save_Finger();break;}//此函数功能是将特征文件缓冲区1的指纹存储，所以必然是执行正确的
    }
    
    if(a)
        return 0;
    else
        return 1;
}


/////////////////////////////////////////////
//指纹传感器发送指令，所有函数的基础
//参数：无
//返回值：无
/////////////////////////////////////////////
void FPMXX_Send_Cmd(unsigned char length,unsigned char *address,unsigned char returnLength) 
{ 
	uint8_t i = 0;

	for(i=0;i<6;i++)        // 
    {
        Usart_Send_Byte(UART4, FPMXX_Pack_Head[i]);   
    }
	
	for(i=0;i<length;i++)
	{       
        Usart_Send_Byte(UART4, *address);        
        address++;
	}		
	
	for (i=0;i<returnLength;i++)
	{
        FPMXX_RECEICE_BUFFER[i] = Usart_Receive_Byte(UART4);
	}
}
	

