#include <Wire.h>

byte slave_address = 10;

union BufferUnion{
	char myByte[4];
	long myLong;
}buffer; 

void setup() {
	setupPWMTimer();
	Wire.begin();        // join i2c bus (address optional for master)
	Serial.begin(9600);  // start serial for output
}

void loop() {
	Wire.requestFrom(slave_address, 4);    // request 1 byte from slave device #10
	
	for(int i=0; (i<4 || Wire.available()); i++)
	{
		buffer.myByte[i] = Wire.read();
	}
	Serial.print(buffer.myLong);
	Serial.println();

  delay(500);
}

void setupPWMTimer()	//generates 5kHz clock
{
  TCCR2A = (1<<WGM21); // Wave Form Generation Mode 2: CTC, OC2A disconnected
  TCCR2B = (1<<CS22); // prescaler = 128
  TIMSK2 = (1<<OCIE2A); // interrupt when Compare Match with OCR2A
  OCR2A = 24;
  DDRD |= (1<<PD7);
}

ISR(TIMER2_COMPA_vect)
{
	PORTD ^= (1<<PD7); // toggle PD7
}