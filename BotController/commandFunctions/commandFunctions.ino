void moveForwardSingle()
{ 
  int sensorLeft = analogRead(IR_SENSOR_LEFT_PIN);
  int sensorRight = analogRead(IR_SENSOR_RIGHT_PIN);
  bool moveForward = false;

  // Below this and we are off the tape
  #define SENSOR_OFF_TAPE_THRESHOLD 500

  // TODO: put these into #define statements
  if (sensorLeft < SENSOR_OFF_TAPE_THRESHOLD && sensorRight > SENSOR_OFF_TAPE_THRESHOLD)
  {
    // Left - sensor off tape, right - sensor on tape
    // Turn right
    digitalWrite(TURN_MOTOR_LEFT_PIN, LOW);
    digitalWrite(TURN_MOTOR_RIGHT_PIN, HIGH);
    moveForward = true;
  }
  else if (sensorLeft > SENSOR_OFF_TAPE_THRESHOLD && sensorRight < SENSOR_OFF_TAPE_THRESHOLD)
  {
    // Left - sensor on tape, right - sensor off tape
    // Turn left
    digitalWrite(TURN_MOTOR_LEFT_PIN, HIGH);
    digitalWrite(TURN_MOTOR_RIGHT_PIN, LOW);
    moveForward = true;
  }
  else if (sensorLeft > SENSOR_OFF_TAPE_THRESHOLD && sensorRight > SENSOR_OFF_TAPE_THRESHOLD)
  {
    // Left - sensor on tape, right - sensor on on tape
    // Straight
    digitalWrite(TURN_MOTOR_LEFT_PIN, LOW);
    digitalWrite(TURN_MOTOR_RIGHT_PIN, LOW);
    moveForward = true;
  }
  else
  {
    // Left - sensor off, right - sensor off tape
    // Stop
    digitalWrite(TURN_MOTOR_LEFT_PIN, LOW);
    digitalWrite(TURN_MOTOR_RIGHT_PIN, LOW);
    moveForward = false;
  }
    
  if (moveForward)
  {
    digitalWrite(FORWARD_MOTOR_PIN, HIGH);
    Serial.print("move: ");
  }
  else
  {
    digitalWrite(FORWARD_MOTOR_PIN, LOW);
    Serial.print("stop: ");
  }
  
  Serial.print(sensorLeft);
  Serial.print("  ");
  Serial.print(sensorRight);
  Serial.println("");
}

void moveForward() 
{
  for (int i = 0; i < 10; ++i)
  {
    moveForwardSingle();
    delay(2);
  }
  
  // stop all motors
  stopMotors();
}

void stopMotors()
{
  digitalWrite(FORWARD_MOTOR_PIN, LOW);
  digitalWrite(BACKWARD_MOTOR_PIN, LOW);
  digitalWrite(TURN_MOTOR_RIGHT_PIN, LOW);
  digitalWrite(TURN_MOTOR_LEFT_PIN, LOW);
}

void moveBackwardSingle()
{
  stopMotors();  
}

void moveBackward()
{
  moveBackwardSingle();
}

