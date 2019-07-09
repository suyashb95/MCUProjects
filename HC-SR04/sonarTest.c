#define F_CPU 16000000
#define clockCyclesPerMicrosecond() (F_CPU/1000000)
#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 
#define MAX_SENSOR_TIME 5800
#define MAX_ECHO_TIME 17129
#define IDLE 0
#define RUNNING 1
#define SUCCESS 2
#define FAIL 3

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<stdlib.h>
#include<stdio.h>

/*
Connections 
-----------
Trigger - PORTD pin 4 (Digital pin 4)
Echo - PORTB pin 0 (Digital pn 8)

Trigger sensor if status is 0, set maximum timeout, status = 1
poll while status = 1
Wait for interrupt, if timeout then status = 3 else status = 2 after falling edge
*/
 
volatile unsigned long timer1_overflow_count = 0, echo_start, echo_finish, _max_time;
volatile uint8_t up = 1, status = IDLE;
unsigned long micros(void);
void timer1_init(void);
void USART0Init(void);
void sonar_init(void);
int sonar_trigger(void);
unsigned long sonar_ping(void);
int USART0SendByte(char, FILE*);
int USART0ReceiveByte(FILE*);

FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

ISR(TIMER1_COMPA_vect) {
    timer1_overflow_count++;
}

ISR(PCINT0_vect) {
    if(up) {
        echo_start = micros();
        if(echo_start > _max_time) status = FAIL;
        up = 0;
    }
    else {
        echo_finish = micros();
        status = SUCCESS;
        up = 1;
    }
}

int main() {
    timer1_init();
    USART0Init();
    sonar_init();
    stdin=stdout=&usart0_str;
    sei();
    float distance = 0;
    unsigned int number, decimal;
    while(1) {
        distance = ((float)sonar_ping())/58;
        number = (unsigned int)distance;
        decimal = ((unsigned int)(distance*100))%100;
        printf("%d.%d\n", number, decimal);
        _delay_ms(50);
    }
}

unsigned long micros() {
    // Overflow_count * 1024 + current_timer_count/16
	return (timer1_overflow_count << 10) + (TCNT1 >> 4); 
}

void timer1_init() {
    //TCCR1A |= _BV(COM1A0);
    TCCR1B |= _BV(CS10) | _BV(WGM12);
    TIMSK1 |= _BV(OCIE1A);
    OCR1A = 16383;
}

void USART0Init() {
    // Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    // Set frame format to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    //enable transmission and reception
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

void sonar_init() {  
    DDRB &= ~(1<<0);
    DDRD |= (1<<4);
    PORTD &= ~(1<<4);
    PORTB &= ~(1<<0);
    PCICR |= _BV(PCIE0);
    PCMSK0 |= _BV(PCINT0);
}

int sonar_trigger() {
    if(status != IDLE) 
       return 0;
    PORTD &= ~(1<<4);
    _delay_us(3);
    PORTD |= (1<<4);
    _delay_us(10);
    PORTD &= ~(1<<4);
    _max_time = micros() + MAX_ECHO_TIME + MAX_SENSOR_TIME;
    status = RUNNING;
    return 1;
}

unsigned long sonar_ping() {
    if(!sonar_trigger()) return -1;
    while(status == RUNNING);
    if(status == SUCCESS) {
        status = IDLE;
        return echo_finish - echo_start;
    }
    else {
        status = IDLE;
        return -2;
    }
}

int USART0SendByte(char u8Data, FILE *stream) {
   if(u8Data == '\n')
   {
      USART0SendByte('\r', 0);
   } 
    //wait while previous byte is completed
    while(!(UCSR0A&(1<<UDRE0))){};
    // Transmit data
    UDR0 = u8Data;
    return 0;
}

int USART0ReceiveByte(FILE *stream) {
    uint8_t u8Data;
    // Wait for byte to be received
    while(!(UCSR0A&(1<<RXC0))){};
    u8Data=UDR0;
    //echo input data
    USART0SendByte(u8Data,stream); 
    // Return received data
    return u8Data;
}