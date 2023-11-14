


// Adafruit Motor shield library
// for ESP8266 Wemos D1 R2

#ifndef _AFMotor_h_
#define _AFMotor_h_

#include <inttypes.h>
//#include <analogWrite.h>


#if defined(ESP8266)

  //#define MOTORDEBUG 1

  #include "stdlib_noniso.h"

  #define MICROSTEPS 16

  #define DC_MOTOR_PWM_RATE 5
  #define STEPPER1_PWM_RATE 5
  #define STEPPER2_PWM_RATE 5
  
  // Arduino pin names for interface to 74HCT595 latch
  #define MOTORLATCH D8 //for Wemos D1 R2 // <<D10 for Wemos D1 R1>>
  #define MOTORCLK D2 //for Wemos D1 R2 // <<D4 for Wemos D1 R1>>
  #define MOTORENABLE D5 //for Wemos D1 R2 // <<D7 for Wemos D1 R1>>
  #define MOTORDATA   D6 //for Wemos D1 R2 // <<D8 for Wemos D1 R1>>

  //logic pins for "PWM"
  #define MOTOR1_EN D1 //for Wemos D1 R2 // <<D3 for Wemos D1 R1>>
  #define MOTOR2_EN D1 //for Wemos D1 R2 // <<D3 for Wemos D1 R1>>
  #define MOTOR3_EN D3 //for Wemos D1 R2 // <<D5 for Wemos D1 R1>>
  #define MOTOR4_EN D3 //for Wemos D1 R2 // <<D5 for Wemos D1 R1>>
  
#elif defined(ESP32)

  #include "stdlib_noniso.h"

  #define MICROSTEPS 16

  #define DC_MOTOR_PWM_RATE 5
  #define STEPPER1_PWM_RATE 5
  #define STEPPER2_PWM_RATE 5
  
  // Arduino pin names for interface to 74HCT595 latch
  #define MOTORLATCH 5 
  #define MOTORCLK 17 
  #define MOTORENABLE 14 
  #define MOTORDATA   12 

  //logic pins for "PWM"
  #define MOTOR1_EN 23 
  #define MOTOR2_EN 25 
  #define MOTOR3_EN 27 
  #define MOTOR4_EN 16 

#elif defined(__AVR__)

 #include <avr/io.h>

    //#define MOTORDEBUG 1

    #define MICROSTEPS 16                       // 8 or 16

    #define MOTOR12_64KHZ _BV(CS20)             // no prescale
    #define MOTOR12_8KHZ _BV(CS21)              // divide by 8
    #define MOTOR12_2KHZ _BV(CS21) | _BV(CS20)  // divide by 32
    #define MOTOR12_1KHZ _BV(CS22)              // divide by 64

    #define MOTOR34_64KHZ _BV(CS00)             // no prescale
    #define MOTOR34_8KHZ _BV(CS01)              // divide by 8
    #define MOTOR34_1KHZ _BV(CS01) | _BV(CS00)  // divide by 64
    
    #define DC_MOTOR_PWM_RATE   MOTOR34_8KHZ    // PWM rate for DC motors
    #define STEPPER1_PWM_RATE   MOTOR12_64KHZ   // PWM rate for stepper 1
    #define STEPPER2_PWM_RATE MOTOR34_64KHZ // PWM rate for stepper 2

    // Arduino pin names for interface to 74HCT595 latch
    #define MOTORLATCH 10
    #define MOTORCLK 4
    #define MOTORENABLE 7
    #define MOTORDATA 8

//logic pins for "PWM"
#define MOTOR1_EN 3
#define MOTOR2_EN 3
#define MOTOR3_EN 5
#define MOTOR4_EN 5



#endif



// Bit positions in the 74HCT595 shift register output
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

// Constants that the user passes in to the motor calls
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

// Constants that the user passes in to the stepper calls
#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4

/*

// Arduino pin names for interface to 74HCT595 latch
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 13 //used to be 7
#define MOTORDATA 0 //used to be 8

*/

class AFMotorController
{
  public:
    AFMotorController(void);
    void enable(void);
    friend class AF_DCMotor;
    void latch_tx(void);
    uint8_t TimerInitalized;
};

class AF_DCMotor
{
 public:
  AF_DCMotor(uint8_t motornum, uint8_t freq = DC_MOTOR_PWM_RATE);
  void run(uint8_t);
  void setSpeed(uint8_t);

 private:
  uint8_t motornum, pwmfreq;
};

class AF_Stepper {
 public:
  AF_Stepper(uint16_t, uint8_t);
  void step(uint16_t steps, uint8_t dir,  uint8_t style = SINGLE);
  void setSpeed(uint16_t);
  uint8_t onestep(uint8_t dir, uint8_t style);
  void release(void);
  uint16_t revsteps; // # steps per revolution
  uint8_t steppernum;
  uint32_t usperstep, steppingcounter;
 private:
  uint8_t currentstep;

};

uint8_t getlatchstate(void);

#endif
