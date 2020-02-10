#include <Servo.h>
#include <Adafruit_GPS.h>
#include <math.h>
#define GPSSerial Serial2
#define Radio Serial3
Adafruit_GPS GPS(&GPSSerial);
uint32_t timer = millis();
#define Enable_RAW  false
Servo ESC;     
int maxRpm;
int minRpm;
uint32_t timeStamp = 0;
int tc = 1000;

void setup() {
  Serial.begin(115200); //communicate with seriel at this speed
  Serial3.begin(115200);
  InitializeMotor();
  InitializeGPS();
  Serial.println("Time, Throttle Percentage, Throttle RPM, var,vary");  //Setup Column Headers for easy matlab import.
}

void loop() {
  readRadio();
  takeInput();
}

void readRadio(){
  if (Serial3.available()) {
    Serial.println("Radio Received");
    Serial.println(Serial3.read());
    }
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
}

void InitializeMotor(){
  ESC.attach(2,8000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  ESC.write(0);
  }

void InitializeGPS(){
  Serial.println("GPS Initializing");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  delay(500);
  GPSSerial.println(PMTK_Q_RELEASE);  
  }
  
void takeInput(){
  if(Serial.available()){
    if(Serial.parseInt () == 1){
      readGPS();
      }
    }
  }

void readGPS(){
  char c = GPS.read();
  if (Enable_RAW)
    {
      if (c) Serial.print(c);
    }
  if (GPS.newNMEAreceived()) {
    //Serial.println(GPS.lastNMEA()); 
    if (!GPS.parse(GPS.lastNMEA()))
      return; 
  }
  if (timer > millis()) timer = millis();
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("Fix: "); Serial.println((int)GPS.fix);
  if (GPS.fix) {    
//      Radio.write("Lat: ");
//      Radio.write(convertDegMinToDecDeg(GPS.latitude),6);
//      Radio.write(", ");
//      Radio.write("Long: ");
//      Radio.write(convertDegMinToDecDeg(GPS.longitude),6);
//      Radio.write(", ");
//      Radio.write("Alt: ");
//      Radio.write(convertDegMinToDecDeg(GPS.altitude),6);

      Radio.write("Lat: ");
      Radio.write((long)GPS.latitude);
      Radio.write(", ");
      Radio.write("Long: ");
      Radio.write((long)GPS.longitude);
      Radio.write(", ");
      Radio.write("Alt: ");
      Radio.write((long)GPS.latitude);
   //   Radio.print("Speed (knots): "); Serial.println(GPS.speed);
  }
  }
}

double convertDegMinToDecDeg (float degMin) {
  double min = 0.0;
  double decDeg = 0.0;
 
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}
