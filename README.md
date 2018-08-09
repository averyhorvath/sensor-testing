# sensor-testing

**OVERVIEW**
=====
This study tests the functionality of the Sparkfun VL6180 adn RFD77402 Time of Flight Sensors. Several scenarios were tested - in direct sunlight, in a dark room, etc. - in order to discover the limitations of each sensor.

Operating System: Mac OSx High Sierra Version 10.13.4
Python 3.5.3

Data Sheet Specifications
---
1.  *VL6180*
    * 850 nm IR emitter
    * Can measure up to 10 cm
    * Independent of object reflectance
    * -10°-60°C operating temperature
    * -20°-70°C functional range
2. *RFD77402*
    * IR emitter
    * 100mm to 2000mm measuring range
    * -10°-60°C operating temperature


Arduino Scripts
---
* Each Arduino script prints a distance measurement in milimeters along with a data measurement specific to the library of the sensor that is being tested. 
    * The VL6180 sensor measures and prints the luminosity using the`Serial.println( sensor.getAmbientLight(GAIN_1) );` command. Note: The input gain may be changed to any other the other gains specified in the script. High gains are advatageous for low-light settings.
    * The RFD77402 sensor measures the confidence level using the `confidence = myDistance.getConfidenceValue();` command. The confidence level ranges from 0-2047. Note: If the distance is greater than 2047 mm, the confidence level returns a NaN value, which caused problems for reading and plotting this data. This problem was alleviated by setting the confidence level to zero if the distance was greater than 2 meters (see lines 94-97).
* As shown in the `establishContact()` function, no data will be recorded or printed until a command is sent in the buffer. As shown in the loop function,

    `int numMeasurementFail = 0;`                 
    `int bytesrecd = Serial.available(); `       
    `if (bytesrecd > 0) {  `                      
        `char cmd = Serial.read();  `             
        `if(cmd == 'g') {     `                   
            `recordingData = true;`
        `}`

If a byte is sent in the buffer, then the serial port will read the character. If the command sent is 'g', then a boolean variable `recordingData` is set to true. Once `recordingData=true;`, data will be recorded for 10 seconds and then `recordingData` will be set to false again. 

Python Script
---
* This script reads the Serial data from Arduino and makes an array of the data `arduinoSerialData`. For both sensors, the first element of the array will be the distance measured in milimeters. As stated above, the second element of the array for the VL6180 sensor and the RFD77402 sensor will be luminosity and confidence level, respectively. 

Steps to Collect Data
--- 
* Run the .ino code corresponding to the sensor that is being tested
* Make sure the port detailed in line 30 of the python script is the correct port being used to interface Arduino and the user's computer.
* Make sure the Arduino's Serial Port Monitor is closed. If so, run the `Sensor_Arduino_Py.py` Python script. 
* After it has compiled, the python script will ask a series of questions about the enviroment that the sensors are being tested in. The answers to these questions will be used to create the plot title, the .png file name, and put the .png file in the corresponding folder.
* After the questions have been answered, the user will see `command:`. When the command 'g' is sent, data will begin recording. Data will record for 10 seconds and then the final plot will be shown to the user. This plot will also be saved as a .png file to the user's computer.