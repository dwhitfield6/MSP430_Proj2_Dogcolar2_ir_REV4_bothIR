#include <msp430.h>

#define LED0 BIT0
#define LED1 BIT2
#define IRrec BIT1
#define IRrec1 BIT4
#define IRLED1 BIT0
#define IRLED2 BIT7
#define debug BIT3

int bit=0;
int checksum =0;
unsigned int count=0;
int data =0;
int one =0;
int two =0;
int three =0;
int four =0;
int five = 0;
int six =0;
int i =0;
int ii=0;
int loopcount =0;
int resetcount =0;
int realdata =0;
int nowdata =0;




void tx_send(int irdata);

int led0count=200;
int led1count=200;

int main(void)
{

WDTCTL = WDTPW + WDTHOLD;                            // Stop watchdog timer
P2DIR |= (LED0 + LED1); // Set P1.0 to output direction
P1DIR |= (IRLED2 + IRLED1 + debug);
TA1CCTL0 |= CCIE;                             // CCR0 interrupt enabled
TA1CTL |= TASSEL_2 + MC_1 + ID_2;           // SMCLK/8, upmode
TA1CCR0 =  30;
// P1.3 must stay at input
P2OUT &= ~(LED0 + LED1); // set P1.0 to 0 (LED OFF)
P1OUT &= ~(IRLED2 + IRLED1);
P2IE |= IRrec; // P1.3 interrupt enabled
P2IES |= IRrec; // P1.3 interrupt enabled
P2IFG &= ~IRrec; // P1.3 IFG cleared
P2IE |= IRrec1; // P1.3 interrupt enabled
P2IES &= ~IRrec1; // P1.3 interrupt enabled
P2IFG &= ~IRrec1; // P1.3 IFG cleared

__enable_interrupt(); // enable all interrupts
while(1)
{
	resetcount++;
if(resetcount == 300)
{
loopcount =0;
checksum =0;
}
if(resetcount>1000)
{
	resetcount = 990;
}
	_delay_cycles(200);
		if(checksum == 2)
		{
			realdata = data;
			nowdata = data;
			checksum =0;
			P2OUT |= (LED1); // set P1.0 to 0 (LED OFF)
			loopcount =0;
			led1count=0;
			P2OUT |= (LED1);
		}
if(realdata == 1)
{
	_delay_cycles(20000);
	P2OUT |= (LED0);
	tx_send(2);
	realdata = 0;
	led0count=0;
}
if(realdata == 3)
{
	_delay_cycles(20000);
	P2OUT |= (LED0);
	realdata = 0;
}


led0count++;
led1count++;
if(led0count > 200)
{
	led0count =199;
			P2OUT &= ~(LED0); // set P1.0 to 0 (LED OFF)

}
if(led1count > 200)
{
	led0count =199;
			P2OUT &= ~(LED1); // set P1.0 to 0 (LED OFF)

}
}
}


// Port 1 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	P1OUT ^= debug;
	TA1CTL |= TACLR;
if(loopcount == 0)
{
	data =0;
}
loopcount++;
if (loopcount == 2)
{
	one = count;
	if (one >40 && one <60)
	{
		checksum++;
	}
}
else if (loopcount ==4)
{
	two = count;
	if(two > 18 && two < 30)
	{
		data+=1;
	}

}
else if(loopcount ==6)
{
	three = count;
	if(three > 18 && three < 30)
	{
		data+=2;
	}
}
else if(loopcount ==8)
{
	four = count;
	if(four > 18 && four < 30)
	{
		data+=4;
	}
}
else if(loopcount == 10)
{
checksum++;
	five = count;
	if(five > 18 && five < 30)
	{
		data+=8;
	}
}


count=0;
resetcount=0;
P2IFG &= ~IRrec; // P1.3 IFG cleared
P2IFG &= ~IRrec1; // P1.3 IFG cleared
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{

count++;
if(count >5000)
{
count=0;
}
}



void tx_send(int irdata)
{


	  __disable_interrupt(); // enable all interrupts
	    for (i = 200;i>0;i--)
	    {
    	      P1OUT |= IRLED1;
    	      P1OUT |= IRLED2;
    	      P1OUT &= ~IRLED1;
    	      P1OUT &= ~IRLED2;

	    }
	    for (i = 10;i>0;i--)
	    {

	      __delay_cycles(18);

	    }
    for(ii = 4; ii>0;ii--)
    {
      bit = irdata & 0x0001;
      if(bit == 1)
      {
  	    for (i = 100;i>0;i--)
  	    {
    	      P1OUT |= IRLED1;
    	      P1OUT |= IRLED2;
    	      P1OUT &= ~IRLED1;
    	      P1OUT &= ~IRLED2;

  	    }
  	    for (i = 10;i>0;i--)
  	    {

  	      __delay_cycles(18);

  	    }
      }
      else
      {
    	  for (i = 50;i>0;i--)
    	    	    {
    	      	      P1OUT |= IRLED1;
    	      	      P1OUT |= IRLED2;
    	      	      P1OUT &= ~IRLED1;
    	      	      P1OUT &= ~IRLED2;

    	    	    }
    	    	    for (i = 10;i>0;i--)
    	    	    {

    	    	      __delay_cycles(18);

    	    	    }

      }
     irdata >>= 1;
    }

  __enable_interrupt(); // enable all interrupts
}

