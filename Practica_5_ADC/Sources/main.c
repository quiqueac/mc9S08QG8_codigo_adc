#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define CH0PTA0 0x00
#define CH1PTA1 0x01
#define CH2PTA2 0x02
#define CH3PTA3 0x03
#define CH4PTB0 0x04
#define CH5PTB1 0x05
#define CH6PTB2 0x06
#define CH7PTB3 0x07
#define ADCIRQ 0x40

void irqadc (void);
void int_adc (unsigned char CANAL);
int lee_adc (unsigned char CANAL);

int resultado; // Variable global para IRQ y ver resultado del ADC

void int_adc (unsigned char CANAL)
{
	ADCSC1 = 0x20|CANAL|ADCIRQ;
	ADCSC2 = 0x00;
	ADCCFG = 0x00; // ADLPC = 0 High Speed; ADIV= 00; ADLSMP = SHORT TIME; MODE = 8 BITS; ADICLK = BUS_CLK;
	
if (CANAL==CH0PTA0)
	APCTL1 = 0x01; // ADPC0=PTA0 como canal de ADCO
else
	APCTL1 = CANAL<<1;// Checar para Canal=CH3PTA3 y CH7PTB3, pues no funciona
}

int lee_adc (unsigned char CANAL)
{
	int Resultado;
	
	ADCSC1 = ADCSC1|CANAL;
	while (ADCSC1_COCO==0);
	
	Resultado = ADCR; //Son los 10 bits
	(void) ADCSC1; //; Dummy Read AP
	
	return Resultado;
}
					
void main(void) // Main para usarse por Consulta.
{
 SOPT1=0x12;
 int_adc(CH0PTA0);
 while(1){
	 resultado=lee_adc(CH0PTA0); 
 }
} 

interrupt 19 void irqadc (void)
{	
	EnableInterrupts;
	resultado=ADCR;
	(void) ADCSC1; // Dummy Read
	DisableInterrupts;
}
