/*==================================CPEG222=============================
* Program:		Project2.c
* Authors: 		Yuchen Tang & Mingrui Liu
* Date: 		3/15/2017
* Inputs : On board Btn1 & Btn2
* Outputs: One on board LED (LD1 to 4) and Pmod 8LD on Port B of the MX7 board
========================================================================*/ 
#include <plib.h>                   //include peripheral library
#include <math.h>                   //include math library
#pragma config ICESEL = ICS_PGx1	//configure on board licensed debugger
#pragma config FNOSC = PRIPLL		//configure system clock 80 MHz
#pragma config POSCMOD = EC, FPLLIDIV = DIV_2,FPLLMUL = MUL_20,FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_8		//configure peripheral bus clock to 10 MHz
#define SYS_FREQ    (80000000L)   // 80MHz syste1 a second

/* ------------------------------------------------------------ */
/*				Forward Declarations                    		*/
/* ------------------------------------------------------------ */

int pause1 = 1;//Variable used to track whether counting or paused, intially paused
int pause2 = 1;//Variable used to track whether counting or paused, intially paused
int one = 0;//Variable to track status of first SSD
int ten = 0;//Variable to track status of 2nd SSD
int hun = 0;//Variable to track status of 3rd SSD
int tho = 0;//Variable to track status of 3rd SSD
int one1 = 0;
int mode = 1;
void DelayCount(int count);
void DisplayChar(int num,int SSD);

//#define REFRESH_SSD  (SYS_FREQ/2/100)   //Counter to set refresh rate of displays 100/sec

/* ------------------------------------------------------------ */
/*				Definitions                                     */
/* ------------------------------------------------------------ */
#define DELAY_CNT   125000          //Counter to set delay length (1250000 ~ 1sec)
#define DELAY_SSD   1250000/200
#define Btn1        PORTGbits.RG6   //Define Btn1 to appropriate port bit
#define Btn2        PORTGbits.RG7   //Define Btn2 to appropriate port bit
#define Btn3        PORTAbits.RA0
#define Led1        LATGbits.LATG12 //Define Led1 to appropriate port bit
#define Led2        LATGbits.LATG13 //Define Led2 to appropriate port bit
#define Led3        LATGbits.LATG14  //Define Led3 to appropriate port bit
#define Led4        LATGbits.LATG15  //Define Led4 to appropriate port bit
// SSD Pmod1 (2 rightmost SSDs)using the bottom rows of JA & JB jumpers
#define SegA1       LATBbits.LATB7
#define SegB1       LATBbits.LATB8
#define SegC1       LATBbits.LATB9
#define SegD1       LATBbits.LATB10
#define SegE1       LATEbits.LATE4
#define SegF1       LATEbits.LATE5
#define SegG1       LATEbits.LATE6
#define DispSel1    LATEbits.LATE7 //Select between the cathodes of the 2 SSDs
// SSD Pmod2 (2 leftmost SSDs)using the bottom rows of JC & JD jumpers
#define SegA2       LATBbits.LATB15
#define SegB2       LATDbits.LATD5
#define SegC2       LATDbits.LATD4
#define SegD2       LATBbits.LATB14
#define SegE2       LATDbits.LATD1
#define SegF2       LATDbits.LATD2
#define SegG2       LATDbits.LATD3
#define DispSel2    LATDbits.LATD12 //Select between the cathodes of the 2 SSDs
#define DELAY 3000000 // 39062 = 1second 

void __ISR(_TIMER_1_VECTOR, ipl2) _Timer1Handler(void){//Use timer 1
if (!pause1 && mode == 4){//auto count up
    
    one++;
        if (one>9){
            one = 0;
            ten++; 
        }
        if (ten>5){
            ten = 0;
            hun++;
        }
        if (hun>9){
            hun = 0;
            tho++;
        }
        if (tho>9){
            tho = 0;
        }
       one1=one;
}
if (!pause2 && mode == 5){//auto count down
       one--;
       if (one<0){
           one = 9;
           ten--;
        }
        if (ten<0){
               ten = 5;
               hun--;
           }   
        if (hun<0){
           hun = 9;
           tho--;
           }
           if (tho<0){
               tho = 9;
           }
    } 
      

    mT1ClearIntFlag();  // clear the interrupt flag
}




void main()
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    TRISG = 0xc0;
    DDPCONbits.JTAGEN = 0; //Shutoff JTAG
    PORTSetPinsDigitalOut (IOPORT_G, BIT_12|BIT_13| BIT_14|BIT_15); //Set LD1 through LD4 as digital output
	PORTClearBits (IOPORT_G, BIT_12|BIT_13| BIT_14|BIT_15); //Turn all 4 LEDs OFF
    PORTSetPinsDigitalIn (IOPORT_G, BIT_6 |BIT_7); //Set Btn1 and Btn2 as inputs
    PORTSetPinsDigitalOut (IOPORT_B,BIT_7|BIT_8| BIT_9| BIT_10| BIT_14| BIT_15);// Set MX7 Port A as output for SSD1R
   	PORTSetPinsDigitalOut (IOPORT_E,BIT_4|BIT_5| BIT_6|BIT_7);// Set MX7 Port B as output for SSD1R
   	PORTSetPinsDigitalOut (IOPORT_D,BIT_1|BIT_2| BIT_3|BIT_4 |BIT_5| BIT_12);// Set MX7 Port C and D as output for SSD2
    
    TRISB = 0x0000; // all PORTB as output
    T1CON = 0x8030; // Set  timer 1 prescale to 1:256,
    TMR1 = 0x0; // Clear timer register
    PR1 = 0xA000; // Load period register
    mT1SetIntPriority(2);
    mT1ClearIntFlag();  // clear the interrupt flag  
    INTEnableSystemMultiVectoredInt();  // enable multi-vector interrupts
    mT1IntEnable(1);
    int buttonLock = 0;   //Variable to "lock" the button
    Led1=Led2=Led3=Led4=1;
    while (1)
	{
        DisplayChar(one,0);
        DisplayChar(hun,2);  
        DelayCount(DELAY_SSD);
        DisplayChar(ten,1);
        DisplayChar(tho,3); 
        DelayCount(DELAY_SSD);  
       
        //mode2
        if(Btn3 && !buttonLock && mode==1){ //when in mode 1 and press btn3, enter mode 2
            mode=2;
            Led2= 1;
            Led1=Led3=Led4=0;
            buttonLock = 1;
        }  
        
        if(Btn2 && !buttonLock && mode==2){//when in mode 2, press btn2, increase the number on hundreds by one
           hun++;
           buttonLock=1;
           if (hun>9){//when the number on hundreds reaches 10, set it to 0
                hun = 0;
                tho++;
           }
           if (tho>9){//when the number on thousands reaches 10, set it to 0
                tho = 0;
           }
        }
        if(Btn1 && !buttonLock && mode==2){//when in mode 2, press btn1, decrease the number on hundreds by one
           hun--;
           buttonLock=1;
           if (hun<0){//when the number on hundreds reaches -1, set it to 9
                hun = 9;
                tho--;
           }
           if (tho<0){//when the number on thousands reaches -1, set it to 9
               tho = 9;
           }
     
        } 
    
      //mode3
        if(Btn3 && !buttonLock && mode==2){//when in mode 2 and press btn3, enter mode 3
            mode=3;
            Led1= 1;
            Led2=Led3=Led4=0;
            buttonLock = 1;
        }  
        
        if(Btn2 && !buttonLock && mode==3){//when in mode 3, press btn2, increase the number on ones by one
           one++;
           buttonLock=1;
           if (one>9){//when the number on ones reaches 10, set it to 0
              one = 0;
              ten++;
           }
           if (ten>5){//when the number on tens reaches 6, set it to 0
              ten = 0;
           }
        }
        if(Btn1 && !buttonLock && mode==3){//when in mode 3, press btn1, decrease the number on ones by one
           one--;
           buttonLock=1;
           if (one<0){//when the number on ones reaches -1, set it to 9
              one = 9;
              ten--;
           }
           if (ten<0){//when the number on tens reaches -1, set it to 5
              ten = 5;
           }        
        } 
        
        if(Btn3 && !buttonLock && mode==3){//when in mode 3 and press btn3, enter mode 1
            mode=1;
            Led1=Led2=Led3=Led4=1;
            one=ten=hun=tho=0;
            buttonLock=1;           
        }
         
        
        //mode = 4
        if(Btn2 && !buttonLock && mode==1){//when in mode 1 and press btn2, enter mode 4 
            mode=4;
            pause1= !pause1;
            Led1=Led2=1;
            Led3=Led4=0;
            buttonLock = 1;
            one1=one;  
        }
        if(Btn2 && !buttonLock && mode==4){//when in mode 4 and press btn2 will stop or continue
            pause1=(pause1+1)%2;//Set pause to 1 or 0 when press btn2
            buttonLock = 1;}
        
       
        
        if (Btn3 && !buttonLock && mode==4){//when in mode 4 and press btn3, enter mode 1 
            mode = 1;
            pause1 = 1;
            buttonLock=1;
            Led1=Led2=Led3=Led4=1;
            one=ten=hun=tho=0;
        }

        //mode5
        if(Btn1 && !Btn3 && !buttonLock && mode==1){//when in mode 1 and press btn1, enter mode 5 
            mode=5;
            Led1=Led2=0;
            Led3=Led4=1;
            buttonLock = 1;
            pause2=!pause2;
            one1=one; 
        }
        
        if(Btn1 && !buttonLock && mode==5){
            pause2=(pause2+1)%2;////Set pause to 1 or 0 when press btn1
            buttonLock = 1;
        }
        
       
        
        if (Btn3 && !buttonLock && mode==5){//when in mode 5 and press btn3, enter mode 1
            pause2 =1;
            mode = 1;
            buttonLock = 1;
            Led1=Led2=Led3=Led4=1;
            one=ten=hun=tho=0;
        }

        
        
        else if(!Btn2 &!Btn1&!Btn3 &buttonLock)//Check if both buttons are off but "locked"
        {
            DelayCount(2000);             //Wait about 100ms for switch to settle
            buttonLock = 0;                 //"unlock" button  
        }
    
	}
}




void DisplayChar(int num,int SSD)
{
    if(SSD > 1)
    {      
        DispSel2 = SSD-2;
        switch(num)
        {
            case 0:
                SegA2=1; SegB2=1; SegC2=1; SegD2=1;SegE2=1; SegF2=1; SegG2=0;break;
            case 1:
                SegA2=0; SegB2=1; SegC2=1; SegD2=0;SegE2=0; SegF2=0; SegG2=0;break;
            case 2:
                SegA2=1; SegB2=1; SegC2=0; SegD2=1;SegE2=1; SegF2=0; SegG2=1;break;
            case 3:
                SegA2=1; SegB2=1; SegC2=1; SegD2=1;SegE2=0; SegF2=0; SegG2=1;break;
            case 4:
                SegA2=0; SegB2=1; SegC2=1; SegD2=0;SegE2=0; SegF2=1; SegG2=1;break;
            case 5:
                SegA2=1; SegB2=0; SegC2=1; SegD2=1;SegE2=0; SegF2=1; SegG2=1;break;
            case 6:
                SegA2=1; SegB2=0; SegC2=1; SegD2=1;SegE2=1; SegF2=1; SegG2=1;break;
            case 7:
                SegA2=1; SegB2=1; SegC2=1; SegD2=0;SegE2=0; SegF2=0; SegG2=0;break;
            case 8:
                SegA2=1; SegB2=1; SegC2=1; SegD2=1;SegE2=1; SegF2=1; SegG2=1;break;
            case 9:
                SegA2=1; SegB2=1; SegC2=1; SegD2=1;SegE2=0; SegF2=1; SegG2=1;break;
            default:
                break;
        }   
    }
    else
    {
        DispSel1 = SSD;
        switch(num)
        {
            case 0:
                SegA1=1; SegB1=1; SegC1=1; SegD1=1;SegE1=1; SegF1=1; SegG1=0;break;
            case 1:
                SegA1=0; SegB1=1; SegC1=1; SegD1=0;SegE1=0; SegF1=0; SegG1=0;break;
            case 2:
                SegA1=1; SegB1=1; SegC1=0; SegD1=1;SegE1=1; SegF1=0; SegG1=1;break;
            case 3:
                SegA1=1; SegB1=1; SegC1=1; SegD1=1;SegE1=0; SegF1=0; SegG1=1;break;
            case 4:
                SegA1=0; SegB1=1; SegC1=1; SegD1=0;SegE1=0; SegF1=1; SegG1=1;break;
            case 5:
                SegA1=1; SegB1=0; SegC1=1; SegD1=1;SegE1=0; SegF1=1; SegG1=1;break;
            case 6:
                SegA1=1; SegB1=0; SegC1=1; SegD1=1;SegE1=1; SegF1=1; SegG1=1;break;
            case 7:
                SegA1=1; SegB1=1; SegC1=1; SegD1=0;SegE1=0; SegF1=0; SegG1=0;break;
            case 8:
                SegA1=1; SegB1=1; SegC1=1; SegD1=1;SegE1=1; SegF1=1; SegG1=1;break;
            case 9:
                SegA1=1; SegB1=1; SegC1=1; SegD1=1;SegE1=0; SegF1=1; SegG1=1;break;
            default:
                break;
        }   
    }
    
}


void DelayCount(int count)
{
	int		counter;
	for (counter=0; counter<count; counter++){}
}


