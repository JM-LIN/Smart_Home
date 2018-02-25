
#include "wifi.h"


/************************************************************************
* Global Variable Declare Section   
************************************************************************/

//extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART_RX_STA;         		//����״̬���	

//static void                   ESP8266_GPIO_Config                 ( void );
//static void                   ESP8266_USART_Config                ( void );
//static void                   ESP8266_USART_NVIC_Configuration    ( void );


struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };


void WiFi_Init(ENUM_Net_ModeTypeDef mode)							//ESP_8266��ʼ��
{																	// mode : AP ; STA ; STA_AP
	
//	while(!ESP8266_Cmd ( "AT+RESTORE", "OK", NULL, 1000 ));
	switch(mode)
	{
		case(STA):
		{
			/******************************/
			while(!ESP8266_Cmd ( "AT+CWMODE=1", "OK", NULL, 1000 ));
			while(!ESP8266_Cmd ( "AT+RST", "OK", NULL, 2500 ));
			while(!ESP8266_Cmd ( "AT+CWJAP=\"420sys\",\"420dzcxsys\"", "OK", NULL, 10000 ));
			delay_ms (1800);
			delay_ms (1800);
			delay_ms (1800);
			delay_ms (1800);
			delay_ms (1800);
			while(!ESP8266_Cmd ( "AT+CIPMUX=0", "OK", NULL, 1000 ));
			while(!ESP8266_Cmd ( "AT+CIPSTART=0,\"TCP\",\"144.168.61.37\",12346", "OK", NULL, 1000 ));
			while(!ESP8266_Cmd ( "AT+CWMODE=1", "OK", NULL, 1000 ));
			/******************************/
			break;
		}
		case(AP):
		{
			/******************************/
			while(!ESP8266_Cmd ( "AT+CWMODE=2", "OK", NULL, 1000 ));
			while(!ESP8266_Cmd ( "AT+RST", "OK", NULL, 2500 ));
			delay_ms (1800);
			delay_ms (1800);
			while(!ESP8266_Cmd ( "AT+CIPAP=\"192.168.4.1\"", "OK", NULL, 2500 ));
			while(!ESP8266_Cmd ( "AT+CWSAP=\"Smart_Home\",\"123321556\",1,4", "OK", NULL, 1000 ));
			while(!ESP8266_Cmd ( "AT+CIPMUX=1", "OK", NULL, 1000 ));
			while(!ESP8266_Cmd ( "AT+CIPSERVER=1,8080", "OK", NULL, 1000 ));
			while(!ESP8266_Cmd ( "AT+CIPSTO=180", "OK", NULL, 2500 ));
			while(!ESP8266_Cmd ( "AT+CIFSR", "OK", NULL, 2500 ))			// ��ѯ����IP�����Լ��ģ���Ƿ�����
            {				
				LCD_P6x8Str(64,1,(unsigned char*)"Fail !");
			}				
			/******************************/
			break;
		}
		// TMAre_HMyou_SMSout_LSof_WEyour_SSOfucking_SSTmind_HT?_ARAss_PMHole!
		case(STA_AP):
		{
			/******************************/
//			ESP8266_AT_Test();
//			while(!ESP8266_Cmd ( "+++", NULL, NULL, 1800 ));
//			while(!ESP8266_Cmd ( "+++", NULL, NULL, 1800 ));
//			while(!ESP8266_Cmd ( "AT+RST", "OK", NULL, 1800 ));
			delay_ms (1800);
			delay_ms (1800);
			delay_ms (1800);
			delay_ms (1800);
			delay_ms (1800);
//			while(!ESP8266_Cmd ( "AT+CWMODE_DEF=3", "OK", NULL, 1000 ));			
//			while(!ESP8266_Cmd ( "AT+CWSAP=\"Smart_Home\",\"123321556\",1,4", "OK", NULL, 1000 ));
//			while(!ESP8266_Cmd ( "AT+CWJAP=\"420sys\",\"420dzcxsys\"", "OK", NULL, 10000 ));
//			while(!ESP8266_Cmd ( "AT+CWJAP_DEF=\"Smart_Phone\",\"123321556abc\"", "OK", NULL, 10000 ));
	
//			while(!ESP8266_Cmd ( "AT+CIPMUX=0", "OK", NULL, 1000 ));
			while (!ESP8266_Link_Server( enumTCP, User_ESP8266_TcpServer_IP, User_ESP8266_TcpServer_Port, Single_ID_0 ));	
			delay_ms (1800);
			while (!ESP8266_UnvarnishSend());
			delay_ms (1800);
			
//			while(!ESP8266_Cmd ( "AT+CIPSTART=\"TCP\",\"144.168.61.37\",12346", "OK", NULL, 1000 ));
//			while(!ESP8266_Cmd ( "AT+CIPMODE=1", "OK", NULL, 1000 ));
//			while(!ESP8266_Cmd ( "AT+CIPSEND", "OK", NULL, 1000 ));
			//	while(!ESP8266_Cmd ( ">setid=Smart_Home", "OK", NULL, 1000 ));	
			/******************************/
			break;
		}	
		default:
			break;		
	}
}

/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	
	char cStr [ 100 ] = { 0 };
	
//	printf ( "\r\n�������� ESP8266 ......\r\n" );

	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n���� ESP8266 ���\r\n" );
	
	
	while ( 1 )
	{		

//		printf ( "%s", cStr );                                             //��ӡ��ȡ DHT11 ��ʪ����Ϣ
	
		
	}	
}

/*
 * ��������ESP8266_Rst
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ���� ESP8266_AT_Test ����
 */
void ESP8266_Rst ( void )
{
	#if 0
	 ESP8266_Cmd ( "AT+RST", "OK", "ready", 1800 );   	
	
	#else
	 ESP8266_RST_LOW_LEVEL();
	 delay_ms ( 500 ); 
	 ESP8266_RST_HIGH_LEVEL();
	#endif

}


/*
 * ��������ESP8266_Cmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               	//���¿�ʼ�����µ����ݰ�

	ESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      	//����Ҫ��������
		return true;
	if(waittime <= 1800)
		delay_ms ( waittime );                 							//��ʱ
	else
	{
		delay_ms(1800);delay_ms(1800);delay_ms(1800);
	}
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

//	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}


/*
 * ��������ESP8266_AT_Test
 * ����  ����WF-ESP8266ģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
//void ESP8266_AT_Test ( void )
//{
//	macESP8266_RST_HIGH_LEVEL();
//	
//	Delay_ms ( 1000 ); 
//	
//	while ( ! ESP8266_Cmd ( "AT", "OK", NULL, 500 ) ) ESP8266_Rst ();  	

//}
void ESP8266_AT_Test ( void )
{
	char count=0;
	
	ESP8266_RST_HIGH_LEVEL();	
	delay_ms ( 1000 );
	while ( count < 10 )
	{
		if( ESP8266_Cmd ( "AT", "OK", NULL, 500 ) ) return;
		ESP8266_Rst();
		++ count;
	}
}


/*
 * ��������ESP8266_Net_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��enumMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	switch ( enumMode )
	{
		case STA:
			return ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 1800 ); 
		
	  case AP:
		  return ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 1800 ); 
		
		case STA_AP:
		  return ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 1800 ); 
		
	  default:
		  return false;
  }
	
}


/*
 * ��������ESP8266_JoinAP
 * ����  ��WF-ESP8266ģ�������ⲿWiFi
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd ( cCmd, "OK", NULL, 5000 );	
}


/*
 * ��������ESP8266_BuildAP
 * ����  ��WF-ESP8266ģ�鴴��WiFi�ȵ�
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 *       ��enunPsdMode��WiFi���ܷ�ʽ�����ַ���
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
	
}


/*
 * ��������ESP8266_Enable_MultipleId
 * ����  ��WF-ESP8266ģ������������
 * ����  ��enumEnUnvarnishTx�������Ƿ������
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd ( cStr, "OK", 0, 500 );
	
}


/*
 * ��������ESP8266_Link_Server
 * ����  ��WF-ESP8266ģ�������ⲿ������
 * ����  ��enumE������Э��
 *       ��ip��������IP�ַ���
 *       ��ComNum���������˿��ַ���
 *       ��id��ģ�����ӷ�������ID
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

	switch (  enumE )
	{
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
	}

	if ( id < 5 )
		sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

	else
		sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 1800 );		
}


/*
 * ��������ESP8266_StartOrShutServer
 * ����  ��WF-ESP8266ģ�鿪����رշ�����ģʽ
 * ����  ��enumMode������/�ر�
 *       ��pPortNum���������˿ں��ַ���
 *       ��pTimeOver����������ʱʱ���ַ�������λ����
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
	}
	
}


/*
 * ��������ESP8266_Get_LinkStatus
 * ����  ����ȡ WF-ESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
 * ����  ����
 * ����  : 2�����ip
 *         3����������
 *         3��ʧȥ����
 *         0����ȡ״̬ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Get_LinkStatus ( void )
{
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )			// ������WiFi�����IP��ַ
			return 2;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) ) 		// �ѽ����������������
			return 3;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )		// ������Ͽ�����
			return 4;		

	}
	LCD_P6x8Str(97,1,(unsigned char*)"Fail ");
	return 0;
}


/*
 * ��������ESP8266_Get_IdLinkStatus
 * ����  ����ȡ WF-ESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
 * ����  ����
 * ����  : �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1���Id���������ӣ�������0���Idδ��������
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Get_IdLinkStatus ( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	

	}
	
	return ucIdLinkStatus;
	
}


/*
 * ��������ESP8266_Inquire_ApIp
 * ����  ����ȡ F-ESP8266 �� AP IP
 * ����  ��pApIp����� AP IP ��������׵�ַ
 *         ucArrayLength����� AP IP ������ĳ���
 * ����  : 0����ȡʧ��
 *         1����ȡ�ɹ�
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength )
{
	char uc;
	
	char * pCh;
	
	
  ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 500 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "APIP,\"" );
	
	if ( pCh )
		pCh += 6;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pApIp [ uc ] = * ( pCh + uc);
		
		if ( pApIp [ uc ] == '\"' )
		{
			pApIp [ uc ] = '\0';
			break;
		}
		
	}
	
	return 1;
	
}


/*
 * ��������ESP8266_UnvarnishSend
 * ����  ������WF-ESP8266ģ�����͸������
 * ����  ����
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_UnvarnishSend ( void )
{
	if ( ! ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) )
		return false;
	
	return 
	  ESP8266_Cmd ( "AT+CIPSEND", "OK", ">", 500 );	
}


/*
 * ��������ESP8266_ExitUnvarnishSend
 * ����  ������WF-ESP8266ģ���˳�͸��ģʽ
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_ExitUnvarnishSend ( void )
{
//	delay_ms ( 1000 );
	
	ESP8266_Usart ( "+++" );
	
	delay_ms ( 1800 );
	delay_ms ( 1800 );
	LCD_P6x8Str(97,2,(unsigned char*)"Exit ");
}


/*
 * ��������ESP8266_SendString
 * ����  ��WF-ESP8266ģ�鷢���ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 *       ��pStr��Ҫ���͵��ַ���
 *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
 *       ��ucId���ĸ�ID���͵��ַ���
 * ����  : 1�����ͳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, unsigned char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
//	char count = 0;
		
	if ( enumEnUnvarnishTx )
	{
		ESP8266_Usart ( pStr );
//		ESP8266_Cmd ( pStr, 0, 0, 1000 );
		bRet = true;		
	}

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );
		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength);
		
		ESP8266_Cmd ( cStr, "> ", 0, 1000 );
		
		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );
		if(bRet)
			LCD_P6x8Str(97,1,(unsigned char*)"S OK ");
		else
			LCD_P6x8Str(97,1,(unsigned char*)"SFail");
//		while( count < 2 )
//		{
//			if( ESP8266_Cmd ( pStr, "SEND OK", 0, 500 ) )	
//			{ 
//				LCD_P6x8Str(97,5,(unsigned char*)"S OK ");
//				bRet = true; 
//				break;
//			}
//			++ count;
//		}		
//		if(bRet == false)
//		{
//			LCD_P6x8Str(97,5,(unsigned char*)"SFail");
//			bRet = true;
//		}
	}
	
	return bRet;
}


/*
 * ��������ESP8266_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
 */
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}

	return pRecStr;
	
}

