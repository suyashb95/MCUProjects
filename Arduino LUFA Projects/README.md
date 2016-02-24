Modification of the LUFA demos to work with the Arduino Uno R3.
Based on code from [this](https://github.com/harlequin-tech/arduino-usb) repo. 
Modified to work with a newer version of LUFA.

## Sketch report formats
Joystick interface
* 3 axis, signed 8 bit values -100 to 100
* 5 buttons
```c
struct {
	int8_t x;
	int8_t y;
	int8_t z;
	uint8_t Button; /*Bit mask for buttons, uses 5 out of the 8 bits.*/
}joyReport;
```

## usage
* Download the AVR-GCC toolchain 
* `cd` to the directory
* run `make`
* Put your Uno in DFU mode([help](https://www.arduino.cc/en/Hacking/DFUProgramming8U2))
* run dfu-programmer atmega16u2 erase
* run dfu-programmer atmega16u2 flash Joystick.hex
* run dfu-programmer atmega16u2 reset
* Disconnect the Uno and connect again. The computer should detect it as a Joystick now
* Flash Arduino-usbserial.hex to restore the Uno to its original state