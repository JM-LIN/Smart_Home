#ifndef _FPMXX_H_
#define _FPMXX_H_

//#include "type.h"
#include "stm32f10x.h"
#include "usart.h"
#include "Systick.h"

//void Finger_Test(void);

u8      FPM_I_TO_B1             (void);
u8      FPM_I_TO_B2             (void);
u8      FPM_Reg_Model           (void);
u8      FPMXX_Input             (void);
u8      PFM_Get_img             (void);
u8      FPM_Write_pad           (u16 id);
u8      FPMXX_Cmd_Save_Finger   (void);
int     FPM_Search              (void);
u16     FPM_Read_pad            (void);
void    FPMXX_Send_Cmd          (unsigned char length,unsigned char *address,unsigned char returnLength); 
void    FPM_Running             (void);

#endif
