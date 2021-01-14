# Ds_Fake_Tester
DS18B20 Fake tester sketch
This sketch tests all resolutions of DS18B20, writes values in alarm trigger registers, and measures conversion time, it uses OLED display as Output (or serial.).
The fake sensor doesn't support 9, 10-bit resolution, and doesn't store values in alarm registers. Also, my fake sensor has a very short conversion time of 35 ms at 12bit, the normal sensor needs about 600 ms at 12bit

Watch how it works https://youtu.be/gzyqUVxOktY

[![IMAGE ALT TEXT](http://img.youtube.com/vi/gzyqUVxOktY/0.jpg)](http://www.youtube.com/watch?v=gzyqUVxOktY "Video Title")

# Components
ESP8266: https://s.click.aliexpress.com/e/_97j7kp

OLED Display: https://s.click.aliexpress.com/e/_9AdhMZ
