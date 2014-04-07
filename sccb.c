//Code for SCCB communication
//ME433
#include "NU32.h"        // plib.h, config bits, constants, funcs for startup and UART
#include <math.h>
#define FULL_DUTY 8


// Define  prototypes
void setup_camera();
void isr2_setup();
void write_sccb();
void write_bit();
void setup_sccb();
void camera_config();
void delay();

// Setup ISR for camera setup();
void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Timer2ISR(void) {
  IFS0bits.T2IF = 0;
}
int main() {

	NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
	
	camera_config();

	isr2_setup();
	setup_sccb();
	//Setup the PWM
	T3CONbits.TCKPS = 0; // Timer 3 pre-scaler N = 1 (1:1), thus it ticks at 80 Mhz (PBCLK/N)
	PR3 = FULL_DUTY - 1; // This makes run at 80 Mhz / (N * (PR2+1)) == 10 MHz
	TMR3 = 0; // Set the initial timer count to 0
	OC1CONbits.OCM = 0b110; // PWM mode without the failsafe for OC1
	OC1CONbits.OCTSEL = 1; // use timer 3
	OC1RS = FULL_DUTY/2; // Next duty duty cycle is 0
	OC1R = FULL_DUTY/2; // Initial duty cycle of 0
	T3CONbits.ON = 1; // Turn on timer 3
	OC1CONbits.ON = 1; // Turn on output compare 1

	while(1){
		write_bit(1);
		write_bit(0);
	}
	
}

void isr2_setup() {

	INTDisableInterrupts();
        //Code to setup Timer2 and Timer2's ISR
        PR2 =39999;// the timer stops
        TMR2 = 0;//Initial timer count 
        T2CONbits.TCKPS = 1; // step 3: Set Prescaler 
        T2CONbits.TGATE = 0; // Not gated input
        T2CONbits.TCS = 0; // PBCLK input
        T2CONbits.ON = 1; // Turn on timer2
        IPC2bits.T2IP = 2; // step 4: set priority to 2
        IPC2bits.T2IS = 0; // step 4:  subpriority to 1
        IFS0bits.T2IF = 0; // step 5: clear the int flag
        IEC0bits.T2IE = 1; // step 6: enable INT2
        INTEnableSystemMultiVectoredInt();

}

void camera_config() {

	// First lets try to get the camera to say ACK

}

// Take a string of binary and write it via sccb to the camera
void write_sccb() {
// PSEUDOCODE
// Write control byte
// Write command byte
// Get ACK?
int address = 0x42;

}

void write_bit(int b) {
//Make sure SOID is writing out
TRISDbits.TRISD3 = 0;
	if (b>0){
	//Data pin high
	LATDbits.LATD3 = 1;
	}
	else
	{
	//Data pin low
	LATDbits.LATD3 = 0;
	}
	
	//Toggle clock on
	LATDbits.LATD2 = 1;
	//Delay
	delay();
	//Toggle clock off
	LATDbits.LATD2 = 0;

	delay();
}


// Initialize the digital in and out
void setup_sccb() {
	TRISDbits.TRISD2 = 0;// This is the clock, always out
	TRISDbits.TRISD3 = 0;// This is the digital out/in  

}


// This is about a ms delay
void delay() {
	int j;
	for (j=0; j<10000; j++) {}
}
