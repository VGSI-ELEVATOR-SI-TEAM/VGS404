
#define	MAIN_C

//#define	_DEBUG
#include "AllHeader.h"

#pragma config FOSC = HS2			// Main oscillator high speed resonator
#pragma config XINST = OFF
#pragma config MCLRE = ON

#ifdef _DEBUG
#pragma config WDTEN = OFF		// Watchdog timer off
#pragma config CP0 = OFF
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config CP3 = OFF
#pragma config CPB = OFF
#else
#pragma config PWRTEN = ON
#pragma config WDTEN = SWDTDIS//ON			// Watchdog timer off
#pragma config CP0 = ON
#pragma config CP1 = ON
#pragma config CP2 = ON
#pragma config CP3 = ON
#pragma config CPB = ON
#endif
#pragma config BOREN = ON		// Brown out reset enabled
#pragma config BORV = 1			// Brown out voltage 2.7V
#pragma config WDTPS = 1024//1024//256		// watchdog postscaler 1 : 256
//#pragma config LVP = OFF		// Low voltage programming off
#pragma config CANMX = PORTB
#pragma config SOSCSEL = DIG

#define vref 30 /*Reference Voltage is 5V*/
void ADC_Init();
int ADC_Read(int);

void Device_init(void){
  IOCB 	= 0;		// pb�ڣ���ƽ�仯�жϽ�ֹ
	WPUB 	= 0;		// pb�ڣ�����������
	ODCON 	= 0;		// spi,uart��©����·����
  HLVDCON	= 0;   		// �ߵ�ѹ���	
	SLRCON	= 0;		// ѹ�ڿ��ƼĴ���
  ANCON0 = 0x00;	// port A and E pins as digital I/O
	ANCON1 = 0x00;
    
	PORTA  = 0x00;
	LATA   = 0x00;	
	TRISA  = 0xFF;//RA0--RA3 SW1 ���뿪������
		// RA0: SW2.4 input
		// RA1: SW2.3 input
		// RA2: SW2.2 input
		// RA3: SW2.1 input
		// RA4: Not used in
		// RA5: Not used in
		// RA6-RA7: OSC
		
	PORTB = 0xFF; 		// switch off (important for CAN!)
	LATB	= 0x00;
	TRISB = 0xFB; 
		// RB0-RB1: Not used input
		// RB2-RB3: CAN
		// RB4-RB5; Not used input
		// RB6-RB7: PGC-PGD;
    
  TRISC = 0xBF;	
		//RC0: MP3 Busy check  input
		//RC1: DI1      input
		//RC2: DI2      input
		//RC3: DI3      input
		//RC4: DI4      input
		//RC5: Not Used input
		//RC6: TX			 output
		//RC7; RX			 input
	PORTC = 0x00;
	LATC	= 0x00;
	
	PORTD = 0x00;
	LATD  = 0x00;	
	TRISD = 0xF0;
		//RD0: LED-RUN			output
		//RD1: ������ʹ��		output
		//RD2: ������ʹ��		output
		//RD3: ������оƬ����		output
		//RD4-RD7: Not used input	
	LATDbits.LATD1 = 1;		//�������������ֹ
	LATDbits.LATD2 = 1;
	MUSIC_POWER_OUTPUT();

	PORTE = 0x00;
	TRISE = 0x07;
		// RE0-RE2: Not used input
	LATE  = 0x00;
}

/**********************************************************************************
 - ���������� ��ʱ��0��ʼ��
 - ����ģ�飺 �ⲿ
 - ����˵���� ��
 - ����˵���� ��
 - ע��	      
**********************************************************************************/
void Timer0_init(void){	
	INTCONbits.T0IF  = 0;		//���TMR0�жϱ�־
	INTCONbits.T0IE  = 1;		//TMR0�ж�����λ
	INTCON2bits.T0IP = 0;		//����Timer0�ж�Ϊ�����ȼ�
	TMR0L = T0L_500ms;				//��ʱ����ֵ
	TMR0H = T0H_500ms;				//��ʱ����ֵ
	T0CON = T0START_1TO32;
}

void Timer1_init (void){
	PIR1bits.TMR1IF = 0;
	IPR1bits.TMR1IP = 0;
	PIE1bits.TMR1IE = 1;
	TMR1H = T1H_5ms;
	TMR1L = T1L_5ms;
	T1CON = T1START_1TO1;
}

void Load_DefaultPara(void){
	
	if(mValumn_Switch == 1)
		p.music_volumn = 25;
	else		
		p.music_volumn = 15;
	p.music_timer = 60;
	p.floor_enable = ENABLE;
	p.arrow_enable = ENABLE;
	p.door_enable = ENABLE;
	p.input_enable = ENABLE;
	p.special_enable = DISABLE;
}

void ADC_Init()
{
    ANCON1 = 0x08;

    ADCON2bits.ADFM=1; // Result format 1= Right justified
    ADCON2bits.ACQT=1; // Acquisition time 7 = 20TAD 2 = 4TAD 1=2TAD
    ADCON2bits.ADCS=2; // Clock conversion bits 6= FOSC/64 2=FOSC/32
    // ADCON1
    ADCON1bits.VCFG0 =0; // Vref+ = AVdd
    ADCON1bits.VCFG1 =0; // Vref+ = AVdd
    ADCON1bits.VNCFG = 0; // Vref- = AVss
    ADCON1bits.CHSN = 4; // Select ADC channel
    ADCON0bits.ADON = 1; // Turn on ADC 
    
    //PIE1.ADIE = 1; //  Enables the A/D interrupt 
}
int ADC_Read(int channel)
{
    int digital;
    ADCON0 =(ADCON0 & 0b10000011)|((channel<<2) & 0b01111100); /*channel 0 is selected i.e (CHS3CHS2CHS1CHS0=0000)
    and ADC is disabled i.e ADON=0*/
    ADCON0 |= ((1<<ADON)|(1<<GO)); /*Enable ADC and start conversion*/
    while(ADCON0bits.GO_nDONE==1); /*wait for End of conversion i.e. Go/done'=0 conversion completed*/
    digital = (ADRESH*256) | (ADRESL); /*Combine 8-bit LSB and 2-bit MSB*/
    printf("ADC digital %d", digital);
    return(digital);
}

/****************************************************************************************************/
/* sET ALL USER VARIABLES																			*/
/****************************************************************************************************/
void Init_UserVar(void){
	BYTE i;
	
	for (i = 0; i < MAX_IO_TYPE; i++)
		virt_out[i] = 0;
	Function_Config();
	Load_DefaultPara();
	if ( p.music_language > MAX_LANGUAGE)
		p.music_language = ENGLISH;
	if(p.music_back_type > (MAX_PLAY_MODE - 1))
		p.music_back_type = PLAY_ALL;
}

/****************************************************************************************************/
/* main function																					*/
/****************************************************************************************************/
void main(void){	
	BYTE baudrate = 0;
	int digital;
    float voltage;
    int volume;
    int old_volume;
	delay_ms(1000);
	Device_init();	
	
#ifndef _DEBUG
	WDTCON = 0x01;		//ʹ���������Ź�
#endif

	
	INTCON = 0;
	RCONbits.IPEN = 1;
	ClrWdt();	
	Timer0_init();	
	Timer1_init();	
	UART_Init(BAUD_9600);
	Init_UserVar();		

	nmtstate = BOOT_UP;
	nmtwait  = 1000;				/* waiting time to 1. heartbeat			*/
	speech_lift = LIFT1;						/* read lift for speech from EEPROM		*/	

	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;	
	while (nmtwait)
		ClrWdt();
	delay_ms(1000);		
	MP3_init();
	music_init_finish = 1;	
	baudrate = BPS_125K;
	Can_init (baudrate);								/* initialize Onchip CAN		l		*/
	nmtstate = OPERATIONAL;	
	
	ClrWdt();												/* reset watchdog timer					*/
	music_back_timer = p.music_timer;						/* start music timer					*/

	ADC_Init();
    digital = ADC_Read(4);
    voltage = digital*((float)vref/(float)4095); /*Convert digital value into analog voltage*/
    volume = abs((int)voltage);
	if (volume < 5) volume = 5;
    old_volume = volume;
    UART_SendCommand(CMD_VOLUMN_SETTING , FEED_BACK , volume); // my test

	while(1) 
		{		
			ClrWdt();
			digital = ADC_Read(4);
            voltage = digital*((float)vref/(float)4095); /*Convert digital value into analog voltage*/
            volume = abs((int)voltage);
            if (volume % 2 == 0)
            {
                if (volume != old_volume)
                {
					if (volume < 5) volume = 5;
                    UART_SendCommand(CMD_VOLUMN_SETTING , FEED_BACK , volume); // my test
                    old_volume = volume;
                    delay_ms(500);
                }
            }
			if(bTime_Fg.time_100ms)
				{
					if(music_led_no_good == 0)
						LedWork();										/* handle LED							*/
					Play_Input();									/* handle digital inputs				*/
					bTime_Fg.time_100ms = 0;
				}			
			if(bTime_Fg.time_500ms)
				{
					UART_ReSend();									/* handle UART errors					*/	
					UART_ResetCheck();
					bTime_Fg.time_500ms = 0;
				}
			if(uart1_status_ok)
				{
					UART_Handle(sReceive_status);					/* check UART feedback					*/
					uart1_status_ok = 0;
				}
			if (rc) 											/* Message in receive buffer			*/
				read_rx (); 									/* read and handle CAN message			*/
			if (p.music_back_type == PLAY_DEMO)						/* enter test mode						*/
				MP3_Demo ();
			else if(nmtstate == OPERATIONAL)
				{
					if(!play_timer)
						Play_Task();
					if(p.music_back_type == PLAY_BACKMUSIC_TIMER)
						{
							if ((music_back_timer || (!p.music_timer)) && (!music_back_status)) 
								{// no time out, but music is off
									UART_SendCommand(CMD_PLAY_FILE_LOOP , FEED_BACK , ((p.music_language % (MAX_LANGUAGE + 1)) + 1));
									delay_ms(500);
									music_back_status = 1;
								}
							else if ((!music_back_timer) && p.music_timer && music_back_status)
								{//time out, but music is on	
									UART_SendCommand(CMD_PLAY_FILE_LOOP , FEED_BACK , MUSIC_BACK_FILE_NO);
									delay_ms(500);
									music_back_status = 0;
								}				
							if(updown_msg & 0x33)
								{
									music_back_timer = p.music_timer;
								}
						}
					else if (p.music_back_type == PLAY_ALL)
						{
							if (!music_back_status)					/* music is off							*/	
								{
									UART_SendCommand(CMD_PLAY_FILE_LOOP , FEED_BACK , ((p.music_language % (MAX_LANGUAGE + 1)) + 1));
									delay_ms(500);
									music_back_status = 1;
								}
						}
					else if (p.music_back_type == PLAY_NO_BACKMUSIC)
						{
							if (music_back_status)						/* music is on							*/	
								{
									UART_SendCommand(CMD_PLAY_FILE_LOOP , FEED_BACK , MUSIC_BACK_FILE_NO);
									delay_ms(500);
									music_back_status = 0;
								}
						}
				}
			if((CanErr_Flag && (!can_inittime)) || (com_can_work > 20))
				{
					CanErr_Flag = 0;
					com_can_work = 0;
					Can_init(baudrate);
				}
			if(!PlayOverFlag)
				{
					LATDbits.LATD1 = 0;
					LATDbits.LATD2 = 0;
				}
		}	
}



