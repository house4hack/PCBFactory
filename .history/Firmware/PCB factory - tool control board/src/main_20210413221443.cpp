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

#define PWM_IN  19          // interupt pin  mega 18, 19, 20, 21
#define STEP_IN 21          // interupt pin
#define DIR_IN 20          //
#define EN_IN 18              //
#define AXIS_SELECT_BIT_0  22 //
#define AXIS_SELECT_BIT_1  24  //

#define LIMIT_BACK 26

// LIMIT PINS  =  input on CNC daughter board




// OUTPUTS

#define GANTRY_STEP_OUT 2  //
#define TOOL_1_STEP_OUT 3  //
#define TOOL_2_STEP_OUT 4  //
#define TOOL_3_STEP_OUT 12  //

#define GANTRY_DIR_OUT 5    //
#define TOOL_1_DIR_OUT 6    //
#define TOOL_2_DIR_OUT 7    //
#define TOOL_3_DIR_OUT  13  //

#define TOOL_1_PWM_OUT  41   //
#define TOOL_2_PWM_OUT 43   //
#define TOOL_3_PWM_OUT 45   //



#define LCD_SDA  A4         //
#define LCD_SCL  A5         //

#define STEP_EN 8           //

// STATE ==============================================

#define ACTIVE_MOTOR_GANTRY 0
#define ACTIVE_MOTOR_TOOL_1 1
#define ACTIVE_MOTOR_TOOL_2 2
#define ACTIVE_MOTOR_TOOL_3 3


// DEFINE what the "off" or unselected states are for the PWM outputs
#define PWM_INACTIVE  LOW

// default start up
int activeMotor = ACTIVE_MOTOR_GANTRY;
int oldActiveMotor = -1;

// =========== FUNCTIONS ============================
void ISR_STEP(){
  // the pin changed!
  byte dir = digitalRead(DIR_IN);
  byte step = digitalRead(STEP_IN);
  switch(activeMotor){
    case ACTIVE_MOTOR_GANTRY:
      digitalWrite(GANTRY_STEP_OUT, step);
      digitalWrite(GANTRY_DIR_OUT, dir);
      break;
    case ACTIVE_MOTOR_TOOL_1:
      digitalWrite(TOOL_1_STEP_OUT, step);
      digitalWrite(TOOL_1_DIR_OUT, dir);
      break;
    case ACTIVE_MOTOR_TOOL_2:
      digitalWrite(TOOL_2_STEP_OUT, step);
      digitalWrite(TOOL_2_DIR_OUT, dir);
      break;
    case ACTIVE_MOTOR_TOOL_3:
      digitalWrite(TOOL_3_STEP_OUT, step);
      digitalWrite(TOOL_3_DIR_OUT, dir);
      break;
  }

}

void ISR_PWM(){
  byte pwm = digitalRead(PWM_IN);
  switch (activeMotor)
  {
  case ACTIVE_MOTOR_TOOL_1:
    digitalWrite(TOOL_1_PWM_OUT, pwm);
    break;
  case ACTIVE_MOTOR_TOOL_2:
    digitalWrite(TOOL_2_PWM_OUT, pwm);
    break;
  case ACTIVE_MOTOR_TOOL_3:
    digitalWrite(TOOL_2_PWM_OUT, pwm);
    break;
  }
}

void getActiveMotor(){

  if (digitalRead(AXIS_SELECT_BIT_0) == LOW) {
    if(digitalRead(AXIS_SELECT_BIT_1) == LOW){
      activeMotor = ACTIVE_MOTOR_GANTRY;

    }else{
      activeMotor = ACTIVE_MOTOR_TOOL_1;
    }
  } else {
    if(digitalRead(AXIS_SELECT_BIT_1) == LOW){
      activeMotor = ACTIVE_MOTOR_TOOL_2;
    }else{
      activeMotor = ACTIVE_MOTOR_TOOL_3;
    }
  }
  if(oldActiveMotor != activeMotor){
    switch (activeMotor)
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

// =============================================

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Connected");

  // setup pin interrupts
  pinMode(STEP_IN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STEP_IN), ISR_STEP, CHANGE);

  pinMode(PWM_IN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PWM_IN), ISR_PWM, CHANGE);


  // inputs ========================================


  pinMode(DIR_IN, INPUT_PULLUP);

  pinMode(AXIS_SELECT_BIT_0, INPUT_PULLUP); // need to set the invert pin for COOLANT_MIST in grbl config.h
  pinMode(AXIS_SELECT_BIT_1, INPUT_PULLUP); // need to set the invert pin for COOLANT_FLOOD in grbl config.h

  // outputs  =======================================
  pinMode(TOOL_1_PWM_OUT, OUTPUT);
  pinMode(TOOL_2_PWM_OUT, OUTPUT);
  pinMode(TOOL_3_PWM_OUT, OUTPUT);

  //steppers
  pinMode(GANTRY_STEP_OUT, OUTPUT);
  pinMode(GANTRY_DIR_OUT, OUTPUT);

  pinMode(TOOL_1_STEP_OUT, OUTPUT);
  pinMode(TOOL_1_DIR_OUT, OUTPUT);

  pinMode(TOOL_2_STEP_OUT, OUTPUT);
  pinMode(TOOL_2_DIR_OUT, OUTPUT);

  pinMode(TOOL_3_STEP_OUT, OUTPUT);
  pinMode(TOOL_3_DIR_OUT, OUTPUT);

  pinMode(STEP_EN, OUTPUT);

  // Read Motor select pins =========================
  getActiveMotor();

}


void loop() {
  // put your main code here, to run repeatedly:
  getActiveMotor();
  // read all limit switches and send to limti back if one is LOW

}

