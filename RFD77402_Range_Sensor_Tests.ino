/*
  Getting distance readings from the RFD77402 Range Sensor
  By: Avery Horvath
  SparkFun Electronics
  Date: August 6th, 2018

  Device: Arduino Uno
  
  Sensor to Arduino Interface:
  GND --> GND
  3V3 --> 3.3 V
  SDA --> A4
  SCL --> A5

  
  Avaliable Functions:
  takeMeasurement() - Initiates measurement. Read via getDistance(), etc

  getDistance() - Returns the last measurement value
  getValidPixels() - Returns number of valid pixels
  getConfidenceValue() - A qualitative value of the distance measurement

  getPeak/setPeak(byte) - Gets/sets the vertical-cavity surface-emitting laser (VCSEL) peak
  getThreshold/setThreshold(byte) - Gets/sets the VCSEL threshold
  getFrequency/setFrequency(byte) - Gets/sets the modulation frequency

  goToOffMode() - Turn off MCPU
  goToOnMode() - Wake MCPU to ON Mode
  goToStandbyMode() - Low power standby mode
  getMeasurement() - Once sensor is configured, issue this command to take measurement

  getCalibrationData() - reads 27 messages of MPU mailbox data and loads calibration data array
  getMailbox() - returns the 16-bits in the MPU mailbox

  reset() - Resets IC to initial POR settings

*/

#include <SparkFun_RFD77402_Arduino_Library.h> //Use Library Manager or download here: https://github.com/sparkfun/SparkFun_RFD77402_Arduino_Library
// #define DEBUG 1

bool recordingData = false;

RFD77402 myDistance; //Hook object to the library

void print_headers() {
    Serial.println("RFD77402 Range Sensor Test");
    Serial.println("Readings taken using Arduino");
    Serial.print("\n");  
    Serial.println("Data Set");
    Serial.print("Distance (mm)");       //long name
    Serial.print("\t");         //tab character
    Serial.print ("Pixels");   //change to match sensor
    Serial.print("\t");         //tab character
    Serial.println ("Confidence Level");   //change to match sensor 
  
}

void setup() {
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  Serial.begin(115200);
  while (!Serial);


  if (myDistance.begin() == false){
    Serial.println("Sensor failed to initialize. Check wiring.");
    while (1); //Freeze!
  }

  Serial.flush();
  establishContact();
}


void establishContact() {
    while (Serial.available() <= 0) {         // do nothing unless serial buffer has data
    
    #ifdef DEBUG
    Serial.println("waiting for command");    // debugging only: send a starting message
    #endif
    
    delay(1000);
    }
}


void loop() {
    int numMeasurementFail = 0;                 // Keep track of how many failed measurements
    int bytesrecd = Serial.available();         // record #bytes in buffer (if any)
    if (bytesrecd > 0) {                        // do something only if bytes in buffer
        char cmd = Serial.read();               // read one byte only (removes byte from buffer)
        if(cmd == 'g') {                        // execute only if a "g" is rec'd
            //print_headers();
            recordingData = true;
        }
    }

    long startTime = millis(); // Start 10 second counter - We only want to record data for 10 seconds

    while (recordingData) {

        byte errorCode = myDistance.takeMeasurement(); // Check if Measurement Data are valid

        if (errorCode == CODE_VALID_DATA) {
          unsigned int distance = myDistance.getDistance();
          unsigned int confidence = myDistance.getConfidenceValue();

      
          Serial.print(distance);
          Serial.print(",");  
          if(distance > 2000) confidence = 0;
          Serial.print(confidence);
            
        }
        
        else if (errorCode == CODE_FAILED_PIXELS)
        {
          Serial.print("Not enough pixels valid.");
        }
        else if (errorCode == CODE_FAILED_SIGNAL)
        {
          Serial.print("Not enough signal");
        }
        else if (errorCode == CODE_FAILED_SATURATED)
        {
          Serial.print("Sensor pixels saturated");
        }
        else if (errorCode == CODE_FAILED_NOT_NEW)
        {
          Serial.print("Measurement Faied");
          numMeasurementFail += 1;
        }
        else if (errorCode == CODE_FAILED_TIMEOUT)
        {
          Serial.print("Sensors timed out");
        }
      
        Serial.println();

        long elapsedTime = millis() - startTime;
        if (elapsedTime >= 10000) {
          recordingData = false;
        }
        delay(100);
    }
    Serial.print("Number of Failed Measurements = ");
    Serial.print(numMeasurementFail);
    Serial.println("Ten Second Count is Complete");
    establishContact();
}
