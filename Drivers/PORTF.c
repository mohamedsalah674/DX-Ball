#include "PORTF.h"
#include "../Game/Nokia5110.h"

void PORTF_Init(void){  unsigned long volatile delay;
  SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x11;           // allow changes to PF4-0  0001 0001
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output      
  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct 
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0          
  GPIO_PORTF_PCTL_R &= ~0x000F000F; //  configure PF4 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x11;  //     disable analog functionality 
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4
  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4,PF0 falling edge event
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
	EnableInterrupts();
}


void GPIOPortF_Handler(void){ // called on touch of either SW1 or SW2
  if(GPIO_PORTF_RIS_R&0x10){  // SW1 touch
			GPIO_PORTF_DATA_R &= 0x00;
			GPIO_PORTF_DATA_R |= 0x08;
    sw1flag = 0;  //flag to start the game
		if(playerXcord <63){
		  Nokia5110_PrintBMP(playerXcord, 47, space, 0);
			playerXcord+=4;
		}
		
		GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag0
  }
  if(GPIO_PORTF_RIS_R&0x01){  // SW2 touch
			GPIO_PORTF_DATA_R &= 0x00;
		GPIO_PORTF_DATA_R = 0x02;
    
		
		if(playerXcord > 0){
		Nokia5110_PrintBMP(playerXcord+16, 47, space, 0);
			playerXcord-=4;
		}
		GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag4
	}
	Nokia5110_PrintBMP(playerXcord, 47, PlayerShip0, 0);
		
}
