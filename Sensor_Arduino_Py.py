# -------------------------------------------------------
# Testing the volatility and accuracy of the SparkFun
# RFD77402 Sensor and the Sparkfun VL6180 Sensor 
#
# Author: Avery Horvath
# email: avery.horvath1@gmail.com
#
# Reads Serial Port from Arduino. When a command 'g'
# is sent, this script live plots distance and additional
# data specific to the sensor being used and its given  
# functions. For the VL6180 sensor, the extra data
# that was measured and plotted was the luminosity. The
# confidence level was measured and plotted for the 
# RFD77402 sensor. 
# 
# The plot will then be saved to a specific
# folder with the parameters that are inputed when the
# script is run. If this folder does not exist, it will
# be created. Future iterations of this script will then
# save the data to an excel file.
# -------------------------------------------------------

import serial, time
import numpy as np 
import matplotlib.pyplot as plt 
import re
import os
from drawnow import *

arduinoSerialData = serial.Serial(port='/dev/tty.usbmodem1411',baudrate=115200)
plt.style.use('ggplot')
plt.ion()

def make_figure(distance, sensorSpecificData, SENSOR_TYPE):
    plt.subplot(211)
    plt.plot(distance, 'o-', c = 'm', label = 'Distance (mm)')
    avgDistance = np.ones(len(distance)) * np.mean(distance)
    avdStd      = np.ones(len(distance)) * np.std(distance)
    plt.plot(avgDistance, '--', c= '.45', linewidth = 1.5, alpha = .7, label = 'Average Distance = %s mm' % str(round(avgDistance[0],1)))
    minDist     = min(distance)
    plt.text(0,minDist,"Standard Deviation = %s" % str(round(avdStd[0],1)),fontweight='bold',bbox={'facecolor':'grey', 'alpha':0.5, 'pad':10}) # put in bottom left
    plt.subplot(212)
    if SENSOR_TYPE == "RFD77402":
        label = 'Confidence Level (0-2018)'
    if SENSOR_TYPE == "VL6180":
        label = 'Luminosity'
    plt.plot(sensorSpecificData,'+-', c = 'c',label = label)
    
 
def plot_filepath(testSensorType,testLocation,testDistance,testMaterial,additionalInfo):
    
    plotFilename = "%s_%s_%s%s_%s" % (testSensorType,testLocation,testMaterial,testDistance,additionalInfo)
    plotFilepath = "./%s/%s/%s/%s/" % (testSensorType,testLocation,testMaterial,testDistance)
    
    if not os.path.exists(plotFilepath):
        os.makedirs(plotFilepath)

    plotFilepath += plotFilename

    fileNum = ""
    i = 0
    while os.path.exists(plotFilepath + str(fileNum) + '.png'):
        i+=1
        fileNum="(%s)" % i
    plotFilepath += fileNum

    return plotFilepath


def plot_title(testSensorType,testLocation,testDistance,testMaterial,additionalInfo):
    plotTitle = "%s %s %s %s %s" % (testSensorType,testLocation,testDistance,testMaterial,additionalInfo)
    plotTitle = plotTitle.replace("_"," ")
    plotTitle = re.sub(r"(\w)([A-Z])", r"\1 \2", plotTitle)
    return plotTitle


def setting_inputs():
    sensorType   = input("Which Sensor are you testing? A) Sparkfun RFD77402 or B) Sparkfun VL6180 "
                         "Please answer A or B. \n Answer: ")
    if sensorType == "A":
        sensorType = "RFD77402"
    elif sensorType == "B":
        sensorType = "VL6180"

    loc             = input("Are you testing inside or outside? (Please capitilize first letter)")
    dist            = input("What is the true distance of the object that you are measuring? "
                            "Please answer with units, preferably mm. \n Answer: ")
    material        = input("What is the color and material/object that you are testing the " 
                            "distance of? Ex) BlueConcreteWall \n Answer: ")
    extraParamInput = input("Are there any other parameters that are important to consider"
                            " for the functionality of the sensor? For example, if the sensor" 
                            " has been exposed to an oven, if the sun or some light source is " 
                            "blinding the sensor, etc. PLEASE ANSWER Y or N . \n Answer: ")
    if extraParamInput == "Y":
        extraParam  = input("In one a couple words (without spaces) please state each additional "
                            "parameter that is being considered with a underscore between each" 
                            " parameter. Ex) AfterOven_FacingSun, Darkroom, etc. Please capitilize "
                            " the first letter of each word. \n Answer: ")
    else:
        extraParam  = ""

    return sensorType, loc, dist, material, extraParam



def plot_serial_data(title,filepath, SENSOR_TYPE):

    while 1:
        
        cmd = input("command: ")
        bytes_sent = arduinoSerialData.write(cmd.encode())  # write cmd to serial buffer as a single byte
        time.sleep(1)                                       # wait for Arduino to write to serial buffer

        distance   = []
        sensorSpecificData = []

        if arduinoSerialData.inWaiting() >> 0:

            while 1:
                arduinoStr = arduinoSerialData.readline().decode()
                
                if 'Ten Second Count is Complete' in arduinoStr:
                    plt.xlabel("Measurement Number")
                    plt.subplot(211)
                    plt.title(title)
                    plt.legend()
                    plt.subplot(212)
                    plt.legend()
                    plt.show()
                    plt.savefig(filepath)
                    break
                if ',' in arduinoStr:   
                    arduinoSerialDataArr = arduinoStr.split(',')
                    distance.append(float(arduinoSerialDataArr[0]))
                    sensorSpecificData.append((arduinoSerialDataArr[1]))
                
                    drawnow(make_figure,distance=distance,sensorSpecificData=sensorSpecificData, SENSOR_TYPE=SENSOR_TYPE)
                    plt.pause(0.000001)
                else:
                    print(arduinoStr)

def main():
    
    sensorType, loc, dist, material, extraParam = setting_inputs()
    plotTitle    = plot_title(sensorType, loc, dist, material, extraParam)
    plotFilepath = plot_filepath(sensorType, loc, dist, material, extraParam)
    plot_serial_data(plotTitle,plotFilepath,sensorType)

main()