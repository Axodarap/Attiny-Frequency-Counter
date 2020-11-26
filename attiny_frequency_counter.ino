#define INTERRUPT_PIN PCINT1  // This is PB1 per the schematic
#define INT_PIN PB2           // PB3 (same as PCINT3) - Pin 2
#define LED_PIN PB4           // Pin 3



void setup() 
{
	pinMode(LED_PIN, OUTPUT);
	pinMode(INT_PIN, INPUT_PULLUP);
	pinMode(PB1, OUTPUT);
	
	//configuring Pin-Change-Interrupt
	cli();						// clear global interrupt flag (disable interrupts)	
	GIMSK |= (1 << INT0);    	// enable external pin interrupt
	MCUCR |= (1 << ISC01);		// set rising edge to trigger interrupt
	MCUCR |= (1 << ISC00);	
	sei();                   	// enable global interrupt flag in SREG

}

void loop() 
{
	
}


// ISR external interrupt
ISR(INT0_vect)
{
	if( digitalRead(LED_PIN)) //toggle led
	{
		digitalWrite(LED_PIN, LOW);
	}
	else
	{
		digitalWrite(LED_PIN, HIGH);
	} 
}