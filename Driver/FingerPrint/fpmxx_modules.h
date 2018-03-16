#ifndef _FPMXX_H_
#define _FPMXX_H_
#include "type.h"
#include "sys.h"
u8 FPM_I_TO_B1(void);
u8 FPM_I_TO_B2(void);
u8 FPM_Reg_Model(void);
u8 FPM_Write_pad(u16 id);
int FPM_Search(void);
u16 FPM_Read_pad(void);
void FPMXX_Send_Cmd(unsigned char length,unsigned char *address,unsigned char returnLength); 
u8 FPMXX_Cmd_Save_Finger(void);
u8 FPMXX_Input(void);
//void Finger_Test(void);
u8 PFM_Get_img(void);
#endif



