#include "constants.h"
#include "commandFunctions.h"

//Servo library
#include <Servo.h>
Servo horizontal_arm;        //initialize a servo object for the connected servo
Servo horizontal_wrist; 
Servo vertical_arm;        //initialize a servo object for the connected servo
Servo vertical_wrist;
 
int state = STALLED;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(FORWARD_MOTOR_PIN, OUTPUT);
  pinMode(BACKWARD_MOTOR_PIN, OUTPUT);
  pinMode(TURN_MOTOR_LEFT_PIN, OUTPUT);
  pinMode(TURN_MOTOR_RIGHT_PIN, OUTPUT);

  pinMode(REPLY_ACK_OUT_PIN, OUTPUT);
  pinMode(REPLY_DONE_OUT_PIN, OUTPUT);
 
  horizontal_arm.attach(HORIZONTAL_ARM_PIN);
  horizontal_wrist.attach(HORIZONTAL_WRIST_PIN);
  vertical_arm.attach(VERTICAL_ARM_PIN);
  vertical_wrist.attach(VERTICAL_WRIST_PIN);

  stopMotors();
  
  // Setup serial debug channel
  Serial.begin(9600);

  // Pause before startup;
  delay(1000);
}

void turn(Servo s, int angle)
{
  int c = s.read();

  Serial.print("Turning from "); Serial.print(c); Serial.print(" to "); Serial.println(angle);
  
  if (angle > c) 
    for (int x = s.read(); x <= angle; x++) { s.write(x); delay(50); }
  else 
    for (int x = s.read(); x >= angle; x--) { s.write(x); delay(50); }
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // sets the LED on
  delay(500);                  // waits for a second
  digitalWrite(LED_PIN, LOW);    // sets the LED off
  delay(500);  

  //Serial.println("zeroing");
    //turn(horizontal_arm, 0);
    //turn(vertical_arm, 80);
    //delay(20000);

  int harvestInRead = analogRead(COMMAND_HARVEST_IN_PIN);
  Serial.print("harvest command: "); Serial.println(harvestInRead);
  if (harvestInRead > AHIGH)
  {
    Serial.println("Harvest Command Recieved");
    state = HARVESTING;
    digitalWrite(REPLY_ACK_OUT_PIN, HIGH);
    delay(1000);
    Serial.println("Harvest Ack Sent");
    digitalWrite(REPLY_ACK_OUT_PIN, LOW);
  }

  int forwardInRead = analogRead(COMMAND_FORWARD_IN_PIN);
  Serial.print("forward command: "); Serial.println(forwardInRead);
  if (forwardInRead > AHIGH)
  {
    Serial.println("Forward Command Recieved");
    state = MOVING_FORWARD;
    digitalWrite(REPLY_ACK_OUT_PIN, HIGH);
    delay(1000);
    Serial.println("Forward Ack Sent");
    digitalWrite(REPLY_ACK_OUT_PIN, LOW);
  }
  
  if (state == STALLED)
  {
    Serial.println("Stalled...");
    turn(horizontal_arm, 0);
    turn(vertical_arm, VERTICAL_ARM_MIN_ANGLE);
    turn(vertical_wrist, 0);
    turn(horizontal_wrist, 0);
    
    digitalWrite(FORWARD_MOTOR_PIN, LOW);
    digitalWrite(BACKWARD_MOTOR_PIN, LOW);
  
    // analog read until something happens
  }

  if (state == MOVING_FORWARD)
  {
    Serial.println("Moving Forward...");
    moveForward();

    state = STALLED;
    digitalWrite(REPLY_DONE_OUT_PIN, HIGH);
    delay(2000);
    digitalWrite(REPLY_DONE_OUT_PIN, LOW);
  }

  if (state == MOVING_BACKWARD)
  {
    moveForward();

    state = STALLED;
    digitalWrite(REPLY_DONE_OUT_PIN, HIGH);
    delay(2000);
    digitalWrite(REPLY_DONE_OUT_PIN, LOW);
  }

  if (state == HARVESTING)
  {
    Serial.println("Harvesting...");

    // make sure left arm is up and left wrist is open
    Serial.print("horizontal_arm 0");    turn(horizontal_arm, 0);
    Serial.print("vertical_wrist 0");  turn(vertical_wrist, 0);

    // make sure right arm is out and right wrist is open
    Serial.print("vertical_arm "); Serial.print(VERTICAL_ARM_MIN_ANGLE); turn(vertical_arm, VERTICAL_ARM_MIN_ANGLE);
    Serial.print("horizontal_wrist 0");  turn(horizontal_wrist, 0);

    // now move the arms closer to the plant
    Serial.print("horizontal_arm 90");    turn(horizontal_arm, 90);
    Serial.print("vertical_arm "); Serial.print(VERTICAL_ARM_MAX_ANGLE); turn(vertical_arm, VERTICAL_ARM_MAX_ANGLE);

    // pinch the gripping arm
    Serial.print("horizontal_wrist 90"); turn(horizontal_wrist, 90);
    
    // pinch the cutting arm twice
    Serial.print("vertical_wrist 90");  turn(vertical_wrist, 90);
    Serial.print("vertical_wrist 0");   turn(vertical_wrist, 0);
    Serial.print("vertical_wrist 90");  turn(vertical_wrist, 90);
    Serial.print("vertical_wrist 0");   turn(vertical_wrist, 0);

    // with you arms wide open
    Serial.print("horizontal_arm 0");    turn(horizontal_arm, 0);
    Serial.print("vertical_arm "); Serial.print(VERTICAL_ARM_MIN_ANGLE); turn(vertical_arm, VERTICAL_ARM_MIN_ANGLE);

    // open the gripping wrist
    Serial.print("horizontal_wrist 0"); turn(horizontal_wrist, 0);

    //for (int x = vertical_wrist.read(); vertical_wrist_direction == 1 ? x <= 90 : x >= 0; x += vertical_wrist_direction) { vertical_wrist.write(x); delay(50); } vertical_wrist_direction *= -1;
    //for (int x = horizontal_arm.read(); horizontal_arm_direction == 1 ? x <= 90 : x >= 0; x += horizontal_arm_direction) { horizontal_arm.write(x); delay(50); } horizontal_arm_direction *= -1;

    //for (int x = horizontal_wrist.read(); horizontal_wrist_direction == 1 ? x <= 90 : x >= 0; x += horizontal_wrist_direction) { horizontal_wrist.write(x); delay(50); } horizontal_wrist_direction *= -1;
    //for (int x = vertical_arm.read(); vertical_arm_direction == 1 ? x <= 90 : x >= 0; x += vertical_arm_direction) { vertical_arm.write(x); delay(50); } vertical_arm_direction *= -1;
    
    state = STALLED;
    digitalWrite(REPLY_DONE_OUT_PIN, HIGH);
    delay(2000);
    digitalWrite(REPLY_DONE_OUT_PIN, LOW);
  }
 
  /*
  for(vertical_wrist_angle = 0; vertical_wrist_angle < MAX_SERVO_ANGLE; vertical_wrist_angle += 5)    // command to move from 0 degrees to 180 degrees
  {                                 
    vertical_wrist.write(vertical_wrist_angle);                 //command to rotate the servo to the specified angle
    delay(4000);                      
  }
 
  for(horizontal_arm_angle = 0; horizontal_arm_angle < MAX_SERVO_ANGLE; horizontal_arm_angle += 5)    // command to move from 0 degrees to 180 degrees
  {                                 
    horizontal_arm.write(horizontal_arm_angle);                 //command to rotate the servo to the specified angle
    delay(4000);                      
  }*/
}

