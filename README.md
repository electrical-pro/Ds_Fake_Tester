# Ds_Fake_Tester
DS18B20 Fake tester sketch
This sketch tests all resolutions of DS18B20, writes values in alarm trigger registers, and measures conversion time, it uses OLED display as Output (or serial.).
The fake sensor doesn't support 9, 10-bit resolution, and doesn't store values in alarm registers. Also, my fake sensor has a very short conversion time of 35 ms at 12bit, the normal sensor needs about 600 ms at 12bit
# Components
ESP8266: https://s.click.aliexpress.com/e/_97j7kp

OLED Display: https://s.click.aliexpress.com/e/_9AdhMZ

# Demo
Watch how it works https://youtu.be/gzyqUVxOktY

[![IMAGE ALT TEXT](http://img.youtube.com/vi/gzyqUVxOktY/0.jpg)](http://www.youtube.com/watch?v=gzyqUVxOktY "Video Title")

# Compile with 1.2.0 version of Adafruit SSD1306 library !
It doesn't work correctly with version higher than 1.2.0

Adafruit SSD1306 library: https://github.com/adafruit/Adafruit_SSD13
![image](https://user-images.githubusercontent.com/31592485/145067027-0089f67d-9602-4342-bc47-b28c946707d9.png)

Tested with ESP8266 Core 2.7.4

# Update
Modified sketch that supports Adafruit SSD1306 library 2.5.0 and ESP8266/ESP32 you can find here:
https://github.com/electrical-pro/Ds_Fake_Tester/issues/1#issuecomment-988925914

# Connections
SDA to D1 (GPIO5)

SCK to D5 (GPIO14)
