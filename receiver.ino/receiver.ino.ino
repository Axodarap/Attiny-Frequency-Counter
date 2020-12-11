#include <Wire.h>

byte slave_address = 10;

union BufferUnion{
	char myByte[4];
	long myLong;
}buffer; 

void setup() {
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
