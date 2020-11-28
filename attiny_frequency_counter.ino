#define INTERRUPT_PIN PCINT1  // This is PB1 per the schematic
#define INT_PIN PB2           // PB3 (same as PCINT3) - Pin 2
#define LED_PIN PB4           // Pin 3

volatile unsigned long _freq;
volatile unsigned long pulse_count;

void setup() 
{
	pinMode(LED_PIN, OUTPUT);
	pinMode(INT_PIN, INPUT_PULLUP);
	
	configurePinChangeInterrup();
	configureTimer0();	
}

void loop() 
{
	
}

void configurePinChangeInterrup()
{
	cli();						// clear global interrupt flag (disable interrupts)	
	
	GIMSK |= (1 << PCIE);    	// enable pin change interrupt
	PCMSK |= (1 << PCINT1);		// set interrupt pin: pin 6 (PB1)
	
	sei();                   	// enable global interrupt flag in SREG
}

void configureTimer0()
{
	cli();						// global interrupt disable

	TCCR0B |=  (1 << CS02);		// Prescaler: 256 (p.80) --> 32.25 kHz
	TCCR0B &=~ (1 << CS01);		// 
	TCCR0B &=~ (1 << CS00);		//

	TCCR0A &=~ (1 << WGM00);	// CTC mode - clear timer on compare (p.79)
	TCCR0A |=  (1 << WGM01);	//
	TCCR0A &=~ (1 << WGM02);	//

	OCR0A = 124;				// Output compare Register, 2^8: 32.25 kHz -> 250Hz
	TIMSK |= (1 << OCIE0A);		// enable Timer0 Interrupt

	GTCCR |= (1 << PSR0);		// reset prescaler counting
	TCNT0 = 0;					// Counter0 = 0
	
	sei();						// global interrupt enable
}

// TIMER0 ISR
ISR(TIMER0_COMPA_vect) 
{
	//_freq = pulse_count * 250;	//i assume at least
	
	//debugging stuff
	if( digitalRead(LED_PIN)) //toggle led
	{
		digitalWrite(LED_PIN, LOW);
	}
	else
	{
		digitalWrite(LED_PIN, HIGH);
	} 
}

// ISR pin change interrupt
ISR(PCINT0_vect)
{
	if(digitalRead(INTERRUPT_PIN))	//rising edge, could potentially be ignored
	{
		pulse_count++; 
	}
	else
	{
		// do nothing
	}
}
