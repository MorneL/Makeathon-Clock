# Clock Makeathon

## This repo includes the code, installation instructions and schematics for the Clock Makeathon

### **Install and set up Ardiuno IDE**
The development board we are using for this project is a NodeMCU 8266 which is not a standard Arduino development board and thus, some additional installation is required.

1. Download the Ardiuno IDE for your OS from https://www.arduino.cc/en/software.
2. Install and open the IDE
3. Under the **Preferences** menu, look for **Additional Boards Manager URLs** and paste the following line `https://arduino.esp8266.com/stable/package_esp8266com_index.json` 
*DO NOT open the link and copy the actual JSON data, just the link.*
4. Now under the **Tools** menu navigate to **Board > Boards Manager..** and search for `esp8266` by ESP8266 Community and press install. 
5. Lastly go back to the **Tools** menu and navigate to **Board > ESP8266 Boards** and select **NodeMCU 1.0**

### **Check if NodeMCU is detected**
Before plugging in NodeMCU device.

1. Navigate to **Tools > Port** and count how many devices.
2. Plug the device into your usb port.
3. Follow step 1 and see if one extra device was detected.

If no new device was detected, please message me on slack ASAP so we can debug it quickly.

### **Download code and install libraries**
This project requires a few libraries to be compiled

1. Clone the code from this repo if you have not done so yet and open the MakeathonClock.ino file.
2. Navigate to **Tools > Manage Libraries..** and search for the following libraries (allow additional dependant libraries to be installed when prompted):
* RTClib - by Adafruit
* LCD_I2C - by Blackhack
* NTPClient - by Fabrice Weinberg
3. Look for the verify checkmark at the top left of the arduino window and try to verify if all libraries have been installed successfully.

### **Compile and upload code to development board**
TODO

### **Assemble project**
TODO
