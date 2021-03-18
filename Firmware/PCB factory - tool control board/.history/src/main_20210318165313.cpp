#include <Arduino.h>

// PCB Factory - Tool control board
// using Pronteer.nz Grbl CNC shield v 3.00 clone


// ================================================
// INTERFACE TO GRBL CNC CONTROLLER
// ================================================

// the below pins are connected to the Grbl cnc controller board

// INPUT From Grbl Z Axis Step ( stepper driver step & dir pins )
#define STEP_IN A0   // abort shield pin
#define DIR_IN A1     // hold shield pin
#define EN_IN A2        //  resume pin on shield
// OUTPUT - echo of limit limit pins
// connected to Z limit switch input on grbl controller.

// NOTE : Not reading individual limit pins into the tool controller
// as these are all connected in parallel, and echoing them to the main
// grbl controller can be done by connecting these DIRECTLY to the z-axis
// limit input on the main board.

// INPUT Axis Select (chip select) pins
#define AXIS_SELECT_BIT_0 A3  //  coolant enable pin on shield - - Connected to A3 (Coolant Mist M7)
#define AXIS_SELECT_BIT_1 A4    // SDA pin on shield -   Connected to A4 (Coolant Flood M8)

// PWM Input from Grbl
#define PWM_IN A5     // SCL pin on shield -  connected to pwm output pin on GRBL controller

// ===============================================

// ================================================
// INTERFACE TO TOOLS / GANTRY
// ================================================

// PWM OUTPUT ECHO
#define PWM_TOOL_1  11  //  Digital pin 11 - Z limit pin on Shield
#define PWM_TOOL_2  10  //  Digital pin 10 - Y limit pin on Shield
#define PWM_TOOL_3  9   //  Digital pin 9 - X limit pin on Shield

// OUTPUTS TO STEPPER MOTORS
#define MOTOR_EN 8
// X AXIS == GANTRY MOTOR
#define GANTRY_STEP 2
#define GANTRY_DIR  5
// Y AXIS == TOOL 1 MOTOR
#define TOOL_1_STEP 3
#define TOOL_1_DIR  6
// Z AXIS == TOOL 2 MOTOR
#define TOOL_2_STEP 3
#define TOOL_2_DIR  7
// A AXIS == TOOL 3 MOTOR
#define TOOL_3_STEP 12
#define TOOL_3_DIR  13


#define ACTIVE_MOTOR_GANTRY 0
#define ACTIVE_MOTOR_TOOL_1 1
#define ACTIVE_MOTOR_TOOL_2 2
#define ACTIVE_MOTOR_TOOL_3 3


// DEFINE what the "off" or unselected states are for the PWM outputs
#define PWM_INACTIVE  LOW

int activeMotor = ACTIVE_MOTOR_GANTRY;


void setup() {
  // put your setup code here, to run once:
  // inputs ========================================

  pinMode(STEP_IN, INPUT);
  pinMode(DIR_IN, INPUT);
  pinMode(EN_IN, INPUT);

  pinMode(PWM_IN, INPUT);

  pinMode(AXIS_SELECT_BIT_0, INPUT_PULLUP); // need to set the invert pin for COOLANT_MIST in grbl config.h
  pinMode(AXIS_SELECT_BIT_1, INPUT_PULLUP); // need to set the invert pin for COOLANT_FLOOD in grbl config.h

  // outputs  =======================================
  pinMode(PWM_TOOL_1, OUTPUT);
  pinMode(PWM_TOOL_2, OUTPUT);
  pinMode(PWM_TOOL_3, OUTPUT);

  //steppers
  pinMode(GANTRY_STEP, OUTPUT);
  pinMode(GANTRY_DIR, OUTPUT);

  pinMode(TOOL_1_STEP, OUTPUT);
  pinMode(TOOL_1_DIR, OUTPUT);

  pinMode(TOOL_2_STEP, OUTPUT);
  pinMode(TOOL_2_DIR, OUTPUT);

  pinMode(TOOL_3_STEP, OUTPUT);
  pinMode(TOOL_3_DIR, OUTPUT);


  // Read Motor select pins =========================
  getActiveMotor();

}

void getActiveMotor(){
  if (digitalRead(AXIS_SELECT_BIT_0) == HIGH) {
    if(digitalRead(AXIS_SELECT_BIT_1) == HIGH){
      activeMotor = ACTIVE_MOTOR_GANTRY;
    }else{
      activeMotor = ACTIVE_MOTOR_TOOL_1;
    }
  } else {
    if(digitalRead(AXIS_SELECT_BIT_1) == HIGH){
      activeMotor = ACTIVE_MOTOR_TOOL_3;
    }else{
      activeMotor = ACTIVE_MOTOR_TOOL_2;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // TODO: change stepper echo to use pin change interupt

  getActiveMotor();

  int stp = digitalRead(STEP_IN);
  int dir = digitalRead(DIR_IN);
  int pwm = digitalRead(PWM_IN);

  // enable pins on drivers are all joined to pin D8 on the shield
  digitalWrite(MOTOR_EN, digitalRead(EN_IN));

  switch(activeMotor){
    case ACTIVE_MOTOR_GANTRY:
      digitalWrite(GANTRY_STEP, stp);
      digitalWrite(GANTRY_DIR, dir);
      digitalWrite(PWM_TOOL_1, PWM_INACTIVE);
      digitalWrite(PWM_TOOL_2, PWM_INACTIVE);
      digitalWrite(PWM_TOOL_3, PWM_INACTIVE);

    break;
    case ACTIVE_MOTOR_TOOL_1:
      digitalWrite(TOOL_1_STEP, stp);
      digitalWrite(TOOL_1_DIR, dir);
      digitalWrite(PWM_TOOL_1, pwm);
      digitalWrite(PWM_TOOL_2, PWM_INACTIVE);
      digitalWrite(PWM_TOOL_3, PWM_INACTIVE);

    break;
    case ACTIVE_MOTOR_TOOL_2:
      digitalWrite(TOOL_2_STEP, stp);
      digitalWrite(TOOL_2_DIR, dir);
      digitalWrite(PWM_TOOL_2, pwm);
      digitalWrite(PWM_TOOL_1, PWM_INACTIVE);
      digitalWrite(PWM_TOOL_3, PWM_INACTIVE);

    break;
    case ACTIVE_MOTOR_TOOL_3:
      digitalWrite(TOOL_3_STEP, stp);
      digitalWrite(TOOL_3_DIR, dir);
      digitalWrite(PWM_TOOL_3, pwm);
      digitalWrite(PWM_TOOL_1, PWM_INACTIVE);
      digitalWrite(PWM_TOOL_2, PWM_INACTIVE);

    break;
  }

}