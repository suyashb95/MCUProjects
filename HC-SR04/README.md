A C project to interface the HC-SR04 sensor with an Arduino Uno R3

##connections
*	Trigger - PORTD pin 4 (Digital pin 4)
*	Echo - PORTB pin 0 (Digital pn 8)
	
## usage
* `cd` to the directory
* run `make all`
* run  `avrdude -p m328p -c arduino -P 'your-COM-port' -U flash:w:sonarTest.hex:i`
* Open RealTerm or TeraTerm to view the values from the sensor 