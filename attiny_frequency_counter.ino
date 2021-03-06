#include <TinyWire.h>

#define INT_PIN PB1           // PB1 (same as PCINT1) - Pin 6
#define LED_PIN PB4           // Pin 3

volatile unsigned long _freq;
volatile unsigned long pulse_count;

byte attiny_address = 10;

union buffer_union{
	char my_byte[4];
	long my_long;
}payload; 


void setup() 
{
	pinMode(LED_PIN, OUTPUT);
	pinMode(INT_PIN, INPUT_PULLUP);
	
	configurePinChangeInterrup();
	configureTimer0();	
	
	TinyWire.begin(attiny_address);				// config TinyWire library for I2C slave functionality
	TinyWire.onRequest(onI2CRequest);			// register a handler function in case of a request from a master
	TinyWire.PCINT0Handler(PinChangeISR);		// insert pin change ISR
}

void loop() 
{
	
}

// -------------------------------------------------------------------------------------------------------------

void onI2CRequest() 
{
	payload.my_long = _freq;

	// sends one byte with content 'b' to the master, regardless how many bytes he expects
	// if the buffer is empty, but the master is still requesting, the slave aborts the communication
	// (so it is not blocking)
	TinyWire.send(payload.my_byte, 4); //sizeof(test));
}

void configureTimer0()
{
	cli();						// global interrupt disable

	TCCR0B |=  (1 << CS02);		// Prescaler: 1024 (p.80) --> 7.8125 kHz
	TCCR0B &=~ (1 << CS01);		// 
	TCCR0B |=  (1 << CS00);		//

	TCCR0A &=~ (1 << WGM00);	// CTC mode - clear timer on compare (p.79)
	TCCR0A |=  (1 << WGM01);	//
	TCCR0A &=~ (1 << WGM02);	//

	OCR0A = 255	;				// Output compare Register, 7.1825 kHz / 255 = ~30 Hz
	TIMSK |= (1 << OCIE0A);		// enable Timer0 Interrupt

	GTCCR |= (1 << PSR0);		// reset prescaler counting
	TCNT0 = 0;					// Counter0 = 0
	
	sei();						// global interrupt enable
}

void configurePinChangeInterrup()
{
	cli();						// clear global interrupt flag (disable interrupts)	
	
	GIMSK |= (1 << PCIE);    	// enable pin change interrupt
	PCMSK |= (1 << PCINT1);		// set interrupt pin: pin 6 (PB1)
	
	sei();                   	// enable global interrupt flag in SREG
}

void PinChangeISR()
{								//check if interrupt was triggered by clk source
	if (digitalRead(INT_PIN))	//also check for rising edge
	{							
		pulse_count++;
	}
	else
	{
		//do nothing
	}
}

// TIMER0 ISR
ISR(TIMER0_COMPA_vect) 
{
	static int prescale_counter = 0;	//further dividing the counter down to 1Hz
	prescale_counter++;

	if(prescale_counter == 30)
	{
		_freq = pulse_count;
		pulse_count = 0;
		if(digitalRead(LED_PIN))
			digitalWrite(LED_PIN, LOW);
		else
			digitalWrite(LED_PIN, HIGH);
		
		prescale_counter = 0;
	}

}