#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

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

#define PWM_IN  19          // PIN INTERRUPT  // from Master Board Laser out (PIN 11)
#define STEP_IN 21          // PIN INTERRUPT  // from Mster Board Z motor Step Pin
#define DIR_IN 20          //   from Master Z motor Dir pin
#define EN_IN 18              //  from Master Z motor En pin
#define AXIS_SELECT_BIT_0  22 //  From Master Board - MIST & FLOOD
#define AXIS_SELECT_BIT_1  24  // From Master Board - MIST & FLOOD

#define LIMIT_BACK 26       // To Z-Limit on Master Board

// LIMIT PINS  =  input on CNC daughter board
#define LIMIT_GANTRY 37
#define LIMIT_TOOL_1  35
#define LIMIT_TOOL_2  33
#define LIMIT_TOOL_3  31


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

#define STEP_EN 8           //  daughter board pin to all daughter board drivers

// STATE ==============================================

#define ACTIVE_MOTOR_GANTRY 0
#define ACTIVE_MOTOR_TOOL_1 1
#define ACTIVE_MOTOR_TOOL_2 2
#define ACTIVE_MOTOR_TOOL_3 3


// DEFINE what the "off" or unselected states are for the PWM outputs
#define PWM_INACTIVE  LOW

// MACHINE STATES =========================
int activeMotor = ACTIVE_MOTOR_GANTRY;
int oldActiveMotor = -1;
int oldLimitOut = -1;
int oldEnableIn = -1;

// state holders for limit switches ( so we can display triggered limits on display)
int limGantry = digitalRead(LIMIT_GANTRY);
int limTool1 = digitalRead(LIMIT_TOOL_1);
int limTool2 = digitalRead(LIMIT_TOOL_2);
int limTool3 = digitalRead(LIMIT_TOOL_3);


LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display


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

void checkLimits(){
  // check if any limit pins are pulled low | triggered

  limGantry = digitalRead(LIMIT_GANTRY);
  limTool1 = digitalRead(LIMIT_TOOL_1);
  limTool2 = digitalRead(LIMIT_TOOL_2);
  limTool3 = digitalRead(LIMIT_TOOL_3);


  if (limGantry == LOW || limTool1 == LOW || limTool2 == LOW || limTool3 == LOW){
    if (oldLimitOut == HIGH) {
      digitalWrite(LIMIT_BACK, LOW);
      oldLimitOut = LOW;
    }
  } else {
    if (oldLimitOut == LOW) {
      digitalWrite(LIMIT_BACK, HIGH);
      oldLimitOut = HIGH;
    }
  }
}

void checkDriverEnable(){
  if(digitalRead(EN_IN) == HIGH) {   // working state is pulled low
    // drivers disabled
    if (oldEnableIn == LOW){
      digitalWrite(STEP_EN, HIGH);
      oldEnableIn = HIGH;
    }
  } else  {
    if(oldEnableIn == HIGH){
      digitalWrite(STEP_EN, LOW);
      oldEnableIn = LOW;
    }
  }
}

void getActiveMotor(){

  if (digitalRead(AXIS_SELECT_BIT_0) == LOW) {
    if(digitalRead(AXIS_SELECT_BIT_1) == LOW){
      activeMotor = ACTIVE_MOTOR_GANTRY;      // 0 0
    }else{
      activeMotor = ACTIVE_MOTOR_TOOL_1;      // 0 1
    }
  } else {
    if(digitalRead(AXIS_SELECT_BIT_1) == LOW){
      activeMotor = ACTIVE_MOTOR_TOOL_2;      //  1 0
    }else{
      activeMotor = ACTIVE_MOTOR_TOOL_3;      //  1 1
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

  // initialize the LCD
	lcd.begin();

  lcd.clear();
  lcd.backlight();      // Make sure backlight is on
  //lcd.noBacklight();

  // Print a message on both lines of the LCD.
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("H4H PCB FACTORY");

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

  checkLimits();
  if(oldLimitOut == LOW) {
    //  TODO: put startup limit triggered warning here
    lcd.setCursor(0,1);
    lcd.print("START ERR LIM:");

    if( limGantry == LOW) lcd.print(" G");
    if( limTool1 == LOW) lcd.print(" T1");
    if( limTool2 == LOW) lcd.print(" T2");
    if( limTool3 == LOW) lcd.print(" T3");
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  getActiveMotor();
  // read all limit switches and send to limti back if one is LOW
  checkLimits();
  checkDriverEnable();

  // TODO: decide when to update display

}

