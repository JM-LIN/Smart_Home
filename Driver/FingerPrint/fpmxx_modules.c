#include "fpmxx_modules.h"



unsigned char volatile FPMXX_RECEICE_BUFFER[54];



unsigned char FPMXX_Get_Device[6] = {0x01,0x00,0x03,0x16,0x00,0x1A};
unsigned char FPMXX_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //


unsigned char FPMXX_Get_Img[6] = {0x01,0x00,0x03,0x01,0x0,0x05};                    //¼ָ��ͼ��
unsigned char FPMXX_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 };        //����Ч��ģ�����
unsigned char FP_Search[11]={0x01,0x0,0x08,0x04,0x01,0x0,0x0,0x03,0xE7,0x0,0xF8}; //������ʼҳ0��ҳ��999����0 - 999,�ڻ�����1BUFFER1���в���
unsigned char FP_Img_To_Buffer1[7]={0x01,0x0,0x04,0x02,0x01,0x0,0x08}; //ͼ����������BUFFER1
unsigned char FP_Img_To_Buffer2[7]={0x01,0x0,0x04,0x02,0x02,0x0,0x09}; //ͼ����������BUFFER2
unsigned char FP_Reg_Model[6]={0x01,0x0,0x03,0x05,0x0,0x09}; //�ϳ�����ģ��
unsigned char FPMXX_Write_Notepad[39]={0x01,0x0,0x24,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//���±�д��
unsigned char FPMXX_Read_Notepad[7]={0x01,0x0,0x04,0x19,0x00,0x00,0x1e};//���±�����
unsigned char FPMXX_Delete_All_Model[6]={0x01,0x0,0x03,0x0d,0x00,0x11};//���ָ�ƿ�
//volatile 
unsigned char  FP_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x00,0x00,0x19};//�洢ģ��BUFFER1  store
//volatile unsigned char FP_Delete_Model[10]={0x01,0x00,0x07,0x0C,0x0,0x0,0x0,0x1,0x0,0x0}; //ɾ��ģ��
//volatile unsigned char FINGER_NUM;


void FPM_Running(void)
{
    int id;
    id=FPM_Search();						//ָ�Ʋ�ѯ����
    
    if(id==-1)
    {
        
    }    
    else 
    {
        
        PC_Usart((unsigned char *)"%d",id);        //��������ʱͨ�����ڴ�ӡָ��id
    }
}

/////////////////////////////////////////////
//¼��ָ�ƣ�7S�ڴ�����������ָ�����������˳�ѭ��
//��������
//����ֵ��  0�ɹ�   1ʧ��
/////////////////////////////////////////////
u8 PFM_Get_img(void)
{
    FPMXX_Send_Cmd(6,FPMXX_Get_Img,12);   //¼ָ��ͼ��
    if(FPMXX_RECEICE_BUFFER[9]==0)			//ȷ����ʹ��������ĵ�10���ֽ�
        return 0;												//¼ָ��ͼ��ɹ������� 0
    else
        return 1;    //¼ָ��ͼ��ʧ�ܣ����� 1
}


/////////////////////////////////////////////
//��������1
//��������
//����ֵ��  0�ɹ�   1ʧ��
/////////////////////////////////////////////
u8 FPM_I_TO_B1(void)
{
    FPMXX_Send_Cmd(7,FP_Img_To_Buffer1,12);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//ȷ����ʹ��������ĵ�10���ֽ�
        return 0;
    else return 1;
}


/////////////////////////////////////////////
//��������2
//��������
//����ֵ��  0�ɹ�   1ʧ��
/////////////////////////////////////////////
u8 FPM_I_TO_B2(void)
{
    FPMXX_Send_Cmd(7,FP_Img_To_Buffer2,12);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//ȷ����ʹ��������ĵ�10���ֽ�
        return 0;
    else return 1;
}






/////////////////////////////////////////////
//�����ϳ�
//��������
//����ֵ��  0�ɹ�   1ʧ��
/////////////////////////////////////////////
u8 FPM_Reg_Model(void)
{
    FPMXX_Send_Cmd(6,FP_Reg_Model,12);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//ȷ����ʹ��������ĵ�10���ֽ�
        return 0;
    else return 1;
}




/////////////////////////////////////////////
//���±��洢
//������IDֵ
//����ֵ��  0�ɹ�   1ʧ��
/////////////////////////////////////////////
u8 FPM_Write_pad(u16 id)
{		
    FPMXX_Write_Notepad[5]=id>>8;							//�ȴ���8λ
    FPMXX_Write_Notepad[6]=id&0x00ff;					//�ٴ���8λ
//    FPMXX_Write_Notepad[5]=8;							//�ȴ���8λ
//    FPMXX_Write_Notepad[6]=8;
//    FPMXX_Write_Notepad[37]=0;				//У��͸�8λ
//    FPMXX_Write_Notepad[38]=77;				//����͵�8λ
	
    FPMXX_Write_Notepad[37]=(61+id)/256;				//У��͸�8λ
    FPMXX_Write_Notepad[38]=(61+id)%256;			
    FPMXX_Send_Cmd(39,FPMXX_Write_Notepad,12);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//ȷ����ʹ��������ĵ�10���ֽ�
        return 0;
    else return 1;
}



/////////////////////////////////////////////
//���±����� 
//������IDֵ
//����ֵ��  IDֵ�ɹ�   -1ʧ��
/////////////////////////////////////////////
u16 FPM_Read_pad(void)
{		
    int id;
    FPMXX_Send_Cmd(7,FPMXX_Read_Notepad,44);
    if(FPMXX_RECEICE_BUFFER[9]==0)			//ȷ����ʹ��������ĵ�10���ֽ�
    {
        id  =   FPMXX_RECEICE_BUFFER[10];        
        id +=   FPMXX_RECEICE_BUFFER[11];
        return id;
    }
    else return -1;
}


/////////////////////////////////////////////
//ָ�ƴ洢
//��������
//����ֵ��  0�ɹ�   1ʧ��
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
    if(FPMXX_RECEICE_BUFFER[9]==0)			//ȷ����ʹ��������ĵ�10���ֽ�
        return 0;
    else return 1;
}


/////////////////////////////////////////////
//ָ��Ѱ��
//��������
//����ֵ��  id�ɹ�   -1ʧ��
/////////////////////////////////////////////
int FPM_Search(void)
{		
    u8 i,j=50;
    int id;

	//¼��ָ��ͼ��	GET����IMG 
	while(j--)			//��ŵȴ�6����7��ȴ�ָ��ͼ���ȡ
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
    if(FPMXX_RECEICE_BUFFER[9]==0)			//ȷ����ʹ��������ĵ�10���ֽ�
    {
        id=FPMXX_RECEICE_BUFFER[10]*256+FPMXX_RECEICE_BUFFER[11];
        return id;
    }
    else return -1;
}


/////////////////////////////////////////////
//ָ��¼����������
//��������                     //�������a��ֵ̫������޸�Ϊ2����Ϊ�ܹ�a�λ�ȡָ�ƣ�һ��6-7��
//����ֵ��  0�ɹ�   1ʧ��
/////////////////////////////////////////////
u8 FPMXX_Input(void)
{
    u8  i,j,a;
    //����ָ���������
    a=4;                        
    while(--a)
    {
        j=50;
        //¼��ָ��ͼ��	GET����IMG 
        while(--j)			//��ŵȴ�6����7��ȴ�ָ��ͼ���ȡ
        {		
            i=PFM_Get_img();
            if(i==0)
                break;
        }
        if(j==0)				            //����ȴ�6~7����Ȼû��ָ��¼�룬�����δ˲��裬ͬʱa--���ܵļ�������һ
            continue;
        if(FPM_I_TO_B1()){;}                //��������1	//�����ڼ�ʹ�����ָ��ͼ��û¼ȡ��Ҳ������������������ʱ�޷������//�ж�j����0����˴�BUG
        
        while(--j)                          //¼��ָ��ͼ��	GET����IMG 
        {		
            i=PFM_Get_img();
            if(i==0)
                break;
        }
        if(j==0)				            //����ȴ�6~7����Ȼû��ָ��¼�룬�����δ˲��裬ͬʱa--���ܵļ�������һ
            continue;
        if(FPM_I_TO_B2()){;}                //��������2
        if(FPM_Reg_Model()){continue;}      //�����ļ��ϲ�������ϲ�ʧ��,��öָ�Ʋ���ͬһ����ָ�ģ�����ִ������,����ִ������Ĵ洢���������¿�ʼ¼��ָ��
        else {FPMXX_Cmd_Save_Finger();break;}//�˺��������ǽ������ļ�������1��ָ�ƴ洢�����Ա�Ȼ��ִ����ȷ��
    }
    
    if(a)
        return 0;
    else
        return 1;
}


/////////////////////////////////////////////
//ָ�ƴ���������ָ����к����Ļ���
//��������
//����ֵ����
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
	

