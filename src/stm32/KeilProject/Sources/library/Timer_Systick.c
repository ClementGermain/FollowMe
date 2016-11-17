#include "stm32f10x.h" 
#include "clock.h"
//=======================================================================================
// 	Auteur T.R.
//  02 Oct 2010
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure le timer systéme Systick
//=======================================================================================

static void (*Ptr_Systick)(void);	

//========================================================================================
// Fixer la priorité de l'IT coeur Systick
//=======================================================================================

void Systick_Prio_IT(char Prio,void (*Systick_function) (void))
{
	Ptr_Systick=Systick_function;
	SCB->SHP[11]=(SCB->SHP[11])& 0x00;
	SCB->SHP[11]=(SCB->SHP[11])| (Prio<<4);
}

//========================================================================================
// Fixer la periode du Systick et renvoie de la valeur effective
//=======================================================================================

float Systick_Period(float Duree_us)

// Retourne la duree calculée, 0 si impossible
{
vu32 Nb_Reload;
float Nb_Reload_Float; 
float Duree_reelle_us;
float Freq_In;

	Freq_In = (float) CLOCK_GetHCLK();

	Nb_Reload_Float= Duree_us*Freq_In/1000000.0;
	Nb_Reload=(float)Nb_Reload_Float;
	Duree_reelle_us=((float)Nb_Reload) / Freq_In*1000000.0; 

	// test de limites
	// Pas de prescaler :
	SysTick->CTRL=(SysTick->CTRL)|1<<2;
	if (Nb_Reload_Float	>=16777215.0) // 2^24-1 maxi
	{
		 // fixer le prescaler spécifique au systick à 8
		 SysTick->CTRL=(SysTick->CTRL)& ~(1<<2);
		 Nb_Reload_Float= Duree_us*Freq_In/8000000.0;
		 Nb_Reload=(float)Nb_Reload_Float;
		 Duree_reelle_us=((float)Nb_Reload) / Freq_In*8000000.0; 
	}

	if (Nb_Reload_Float	>=16777215.0) // 2^24-1 maxi
	{
		 Duree_reelle_us=0.0;
	}

	SysTick->LOAD = Nb_Reload;

	return Duree_reelle_us;
}

void My_SysTick_Handler (void) 
{
	Ptr_Systick();	
}
