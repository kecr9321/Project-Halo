#include <Servo.h>
Servo ESC;     
int maxRpm;
int minRpm;
uint32_t timeStamp = 0;
int tc = 1000;

void setup() {
  InitializeMotor();
  Serial.begin(9600); //communicate with seriel at this speed
  Serial.println("Time, Throttle Percentage, Throttle RPM, var,vary");  //Setup Column Headers for easy matlab import.

}

void loop() {
  

 // timeStamp = millis();
 // Serial.print(timeStamp);
 // Serial.print(", ");
  SmoothStepTest();
}


void SmoothStepTest(){
  for (int i = 1; i <= 100; i++){
    ESC.write(i);    // Send the signal to the ESC
    Serial.print(i);
    Serial.print(", \n");
    delay(5*tc); //Run each step for 20 seconds. 

    Serial.println(" ");
    Serial.print("\n");  
  }
  
void InitializeMotor(){
  ESC.attach(2,8000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  ESC.write(0);
  }
  

}
