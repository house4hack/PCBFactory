#include <Arduino.h>

// PCB Factory - Tool control board
// using Pronteer.nz Grbl CNC shield v 3.00 clone




// ================================================
// INTERFACE TO GRBL CNC CONTROLLER
// ================================================

// the below pins are connected to the Grbl cnc controller board

// NOTE : Not reading individual limit pins into the tool controller
// as these are all connected in parallel, and echoing them to the main
// grbl controller can be done by connecting these DIRECTLY to the z-axis
// limit input on the main board.

// INPUTS ================

#define PWM_IN 2      // interupt pin
#define STEP_IN 3     // interupt pin
#define DIR_IN 4
#define AXIS_SELECT_BIT_0 A3
#define AXIS_SELECT_BIT_1 8

// OUTPUTS

#define GANTRY_STEP_OUT A2
#define TOOL2_STEP_OUT A0
#define TOOL1_STEP_OUT A1
#define TOOL3_STEP_OUT 12

#define GANTRY_DIR_OUT 5
#define TOOL1_DIR_OUT 6
#define TOOL2_DIR_OUT 7
#define TOOL3_DIR_OUT  13

#define PWM_TOOL_1_OUT  9
#define PWM_TOOL_2_OUT 10
#define PWM_TOOL_3_OUT 11



#define LCD_SDA  A4
#define LCD_SCL  A5

// STATE ==============================================

#define ACTIVE_MOTOR_GANTRY 0
#define ACTIVE_MOTOR_TOOL_1 1
#define ACTIVE_MOTOR_TOOL_2 2
#define ACTIVE_MOTOR_TOOL_3 3


// DEFINE what the "off" or unselected states are for the PWM outputs
#define PWM_INACTIVE  LOW

// default start up
int activeMotor, oldActiveMotor = ACTIVE_MOTOR_GANTRY;



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Connected");



  // inputs ========================================

  pinMode(STEP_IN, INPUT);
  pinMode(DIR_IN, INPUT);
//  pinMode(EN_IN, INPUT);

  pinMode(PWM_IN, INPUT);

  pinMode(AXIS_SELECT_BIT_0, INPUT_PULLUP); // need to set the invert pin for COOLANT_MIST in grbl config.h
  pinMode(AXIS_SELECT_BIT_1, INPUT_PULLUP); // need to set the invert pin for COOLANT_FLOOD in grbl config.h

  // outputs  =======================================
  pinMode(PWM_TOOL_1_OUT, OUTPUT);
  pinMode(PWM_TOOL_2_OUT, OUTPUT);
  pinMode(PWM_TOOL_3_OUT, OUTPUT);

  //steppers
  pinMode(GANTRY_STEP_OUT, OUTPUT);
  pinMode(GANTRY_DIR_OUT, OUTPUT);

  pinMode(TOOL_1_STEP_OUT, OUTPUT);
  pinMode(TOOL_1_DIR_OUT, OUTPUT);

  pinMode(TOOL_2_STEP_OUT, OUTPUT);
  pinMode(TOOL_2_DIR_OUT, OUTPUT);

  pinMode(TOOL_3_STEP_OUT, OUTPUT);
  pinMode(TOOL_3_DIR_OUT, OUTPUT);


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
  if(oldActiveMotor != activeMotor){
    switch (acttveMotor)
    {
      case ACTIVE_MOTOR_GANTRY:
        Serial.println("ACTIVE : GANTRY  DAUGHTER X ");
      break;
      case ACTIVE_MOTOR_TOOL_1:
      Serial.println("ACTIVE : TOOL 1  DAUGHTER Y ");
      break;
      case ACTIVE_MOTOR_TOOL_2:
      Serial.println("ACTIVE : TOOL 2 DAUGHTER Z");
      break;
      case ACTIVE_MOTOR_TOOL_3:
      Serial.println("ACTIVE : TOOL 3 DAUGHTER A ");
      break;
    }
    oldActiveMotor = activeMotor;
  }



}

void loop() {
  // put your main code here, to run repeatedly:

  // TODO: change stepper echo to use pin change interupt

  // TODO: Phase 2 -

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