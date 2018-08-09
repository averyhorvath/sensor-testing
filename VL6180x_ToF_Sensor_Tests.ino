
/*
  Getting distance readings from the VL6180 Time of Flight Sensor
  By: Avery Horvath
  SparkFun Electronics
  Date: August 6th, 2018

  Device: Arduino Uno
  
  Sensor to Arduino Interface:
  GND --> GND
  3V3 --> 3.3 V
  SDA --> A4
  SCL --> A5


*/
#include <Wire.h>
#include <SparkFun_VL6180X.h>
#include <Adafruit_VL6180X.h>

// #define DEBUG 1

#define VL6180X_ADDRESS 0x29

VL6180xIdentification identification;
VL6180x sensor(VL6180X_ADDRESS);

Adafruit_VL6180X errorStatus = Adafruit_VL6180X();

bool recordingData = false;


void setup() {
  Serial.begin(115200);
  Wire.begin(); //Start I2C library
  errorStatus.begin();
  delay(100);

  sensor.VL6180xDefautSettings(); //Load default settings to get started.
  
  delay(1000); // delay 1s
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
    int bytesrecd = Serial.available();         // record #bytes in buffer (if any)
    if (bytesrecd > 0) {                        // do something only if bytes in buffer
        char cmd = Serial.read();               // read one byte only (removes byte from buffer)
        if(cmd == 'g') {                        // execute only if a "g" is rec'd
            recordingData = true;
        }
    }

    long startTime = millis(); // Start 10 second counter - We only want to record data for 10 seconds

    while (recordingData) {

        uint8_t status = errorStatus.readRangeStatus();

        unsigned int distance = sensor.getDistance();
        unsigned int lux = sensor.getAmbientLight(GAIN_1);
          
        Serial.print(distance);
        Serial.print(",");
        Serial.println(lux);

        if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
          Serial.println("System error");
        }
        else if (status == VL6180X_ERROR_ECEFAIL) {
          Serial.println("ECE failure");
        }
        else if (status == VL6180X_ERROR_NOCONVERGE) {
          Serial.println("No convergence");
        }
        else if (status == VL6180X_ERROR_RANGEIGNORE) {
          Serial.println("Ignoring range");
        }
        else if (status == VL6180X_ERROR_SNR) {
          Serial.println("Signal/Noise error");
        }
        else if (status == VL6180X_ERROR_RAWUFLOW) {
          Serial.println("Raw reading underflow");
        }
        else if (status == VL6180X_ERROR_RAWOFLOW) {
          Serial.println("Raw reading overflow");
        }
        else if (status == VL6180X_ERROR_RANGEUFLOW) {
          Serial.println("Range reading underflow");
        }
        else if (status == VL6180X_ERROR_RANGEOFLOW) {
          Serial.println("Range reading overflow");
        }
        long elapsedTime = millis() - startTime;
        if (elapsedTime >= 10000) {
          recordingData = false;
        }
        
        
        delay(100);
    } 
    Serial.println("Ten Second Count is Complete");
    establishContact();
};



