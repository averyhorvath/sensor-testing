**PRELIMINARY RESULTS**
=====
Inside
--
1. *Aluminum Rigid Wall*

    * *VL6180*
        
        * The VL6180 provided more accurate readings inside than the RFD77402 sensor and constantly provided a lower standard deviation

        * The maximum distance the sensor was able to measure was about 250 mm despite the datasheet stating it could measure up to 1000mm. To troubleshoot this, whether or not the sensor has multiple modes will be investigated.

    * *RFD77402*

        * This sensor could sense an object was near but only sometimes returned an accurate distance reading. Sometimes the sensor returned over 100% error. 
            * Another RFD77402 sensor will be tested to make sure the error is not due to a faulty RFD77402 sensor. 
            * In addition, the sensor was not facing directily normal to the object but rather a little bit down towards the floor. The sensor & arduino setup was placed higher up to alleviate this; however, this could still have affected measurements.
2. Black Iphone with LED Flashlight
    * *VL6180*
        * Any measurements under 100mm produced accurate results. However, during the first trial, any distances 100m+ produced a constant measurment of 42.00mm. Seeing that the standard deviation was 0.0 and this happened for multiple distances above 100mm, it is safe to say the sensor was malfunctioning. Whether or not the LED flashlight was responsible for this malfunction will be determined with future tests.  

Outside
--
1. *In the Sun*
    * *VL6180*
        * Whether the sensor was in the shade and the object was in the sun or vise versa, the sensor returned nonsensical data. This test was done before the `uint8_t status = errorStatus.readRangeStatus();` was put in the Arudino code. This line returns any problems with the sensor.
    * *RFD77402* 
        * After about a minute of being in the sun, the sensor returned "Sensor pixels saturated".
2. *In the Shade*
    * *VL6180*
        * Was working fine. Though the objects were much cooler than those in direct sunlight.
    
