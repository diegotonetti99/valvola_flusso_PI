#include "Motor.h"
#include "Process.h"
#include "PIController.h"

// x stepper driver pints trigorilla board
#define STEP  54
#define DIR 55
#define ENABLE  38

#define MICROSTEPS 16 // locked by the board
#define MOTOR_STEPS 200 // steps per revolution NEMA17

#define RPM 100

#define DT 10 // ms
#define MAX_TRAVEL 10 // mm
#define PITCH 1.25 // screw pitch
#define STEP_MM float(MOTOR_STEPS) * float(MICROSTEPS) / float(PITCH)

int menu(){
  Serial.println("----------");
  Serial.println("MENU:");
  Serial.println("1) Set a setpoint flow");
  Serial.println("2) Set PI parameters");
  Serial.println("0) Return");
  Serial.println("----------");
  while(!Serial.available()){}
  String entry = Serial.readString();
  return entry.toInt();
}

float getSetpoint(){
  Serial.println("Set wanted flow: ");
  while(!Serial.available()){}
  String setpoint_str = Serial.readString();
  Serial.print("Q [l/m]: ");
  Serial.println(setpoint_str.toFloat());
  return setpoint_str.toFloat();
}

void setPIParams(PIController *controller){
  Serial.println("Kp: ");
  while(!Serial.available()){}
  String Kp_str = Serial.readString();
  controller->setKp(Kp_str.toFloat());
  Serial.println("Ki: ");
  while(!Serial.available()){}
  String Ki_str = Serial.readString();
  controller->setKi(Ki_str.toFloat());
}

void setup() {
  
  // create the serial connection with baudrate 115200
  Serial.begin(115200);
  // wait for the serial connection
  while(!Serial){}
  Serial.println("Ready");
  Motor motor(DIR,STEP,ENABLE, MICROSTEPS, MOTOR_STEPS, RPM, DT);
  motor.disable();
  float setpoint = getSetpoint();
  motor.enable();

  PIController controller(9.0f, 3.0f, DT, 10.0f);
  Process P;

  while(1){

    // send a character to the serial
    if (Serial.available()){
      // turn of motor
      motor.disable();
      Serial.readString();
      // print the menu
      int entry = menu();
      switch (entry){
        case 1: setpoint = getSetpoint(); break;
        case 2: setPIParams(&controller); break;
        default: break;
      }
    }
    motor.enable();
    float y = P.read();
    float u = controller.update(setpoint, y);
    P.update(u);
    long turns = long(u * STEP_MM); // steps to do
    motor.move(turns);
    Serial.print(u);
    Serial.print(",");
    Serial.print(setpoint);
    Serial.print(",");
    Serial.println(y);
  }
  
}

void loop() {
  delay(1);
}
