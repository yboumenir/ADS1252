
#include <SPI.h>
// #include <math.h>

// Will try to use minimal libraries

#define MISOPIN 12
#define SCLKPIN 13

byte byte1; byte byte2; byte byte3;
byte flipper = 0b11111111; // 8 bit number to flip bits of a byte
byte addone = 0b1; // to add one to the two's complement


int check = byte3 << 1;

double ads_output =0;

// declare 3 bytes = 24 bits

// Other Stuff
// #include <StandardCplusplus.h>
// #include <serstream>
#include <avr/pgmspace.h>


// using namespace std;

// <iostream> declares cout/cerr, but the application must define them
// because it's up to you what to do with them.

/*
namespace std
{
  ohserialstream cout(Serial);
}

extern void more(void);
*/

void setup()
{
 Serial.begin(9600);
 
 pinMode(SCLKPIN, OUTPUT); pinMode(MISOPIN, INPUT);
 // corresponding to SCK pin and DRDY/DOUT pin on ADC
 
 reset_adc();
 // put ADC on reset at the outset
 
 SPI.begin();
 // initialize SPI (with default settings, including...
 // CPOL = 0: so that SCLK is normally LOW
 // CPHA = 0: data sampled on rising edge (LOW to HIGH)
 // perhaps try changing CPHA ??
 
 digitalWrite(SCLKPIN, LOW);
 // release ADC from reset; now we're at a known point
 // in the timing diagram, and just have to wait for
 // the beginning of a conversion cycle
}

void loop()
{
 if (digitalRead(MISOPIN) == HIGH) read_adc();
 // "sort of" an interrupt to go to read_adc routine;
 // can use hardware interrupt in future but now just poll
}

void reset_adc()
// to reset ADC, we need SCLK HIGH for min of 4 CONVCYCLES
// so here, hold SCLK HIGH for 5 CONVCYCLEs = 1440 usec
{
 digitalWrite(SCLKPIN, HIGH);
 delayMicroseconds(1440);
}

void read_adc()
{
 drdy_wait();
 // go to drdy_wait routine, where we wait for
 // DRDY phase to pass, and thus for DOUT phase to begin

 byte3 = SPI.transfer(0x00);
 byte2 = SPI.transfer(0x00);
 byte1 = SPI.transfer(0x00);
 // read in adc data (sending out don't care bytes)
 // and store read data into three bytes */
/*
 Serial.println(byte1, DEC);
 Serial.println(byte2, DEC);
 Serial.println(byte3, DEC);
 Serial.println();
 */
 
        if (check == 1){
// Negative number in two's complement form. need to flip bytes. and add one to them.
//		negative = "-";
// flip the bits
	
            byte3 = byte3^flipper;
            byte2 = byte2^flipper;
            byte1 = byte1^flipper;
       	}
         else{
// negative = "+";
// no need to flip or add one
        addone = 0b0;
        }
        
        ads_output = 5*(((byte3 << 16) + (byte2 << 8) + (byte1 + addone))) >> 24; 

        Serial.println(ads_output);
        
        //cout << " two's Complement : " <<  5*(((byte3 << 16) + (byte2 << 8) + (byte1 + addone))/pow(2,24)) << endl;

 
 // print out data;
 // will these instructions eat into time significantly?
 // possible improvement: store all data from multiple cycles
 // into array, and print out only later at end.
}

void drdy_wait()
// wait for DRDY to pass and to reach start-point of DOUT
{
 delayMicroseconds(30);
 // to be safe, 30 usec, instead of 27 usec, which is
 // the expected period of DRDY phase
}
