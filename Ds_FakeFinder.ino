/*
Parts of this code I found on the internet, other parts I wrote myself. It provided as is.
It works fine for me.
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>

#define ONE_WIRE_BUS 0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address


//display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display
// reset pin
Adafruit_SSD1306 display(-1);


uint8_t deviceCount = 0;
const uint8_t PARASITE = 0;
uint8_t highAlarmValue = 25;
uint8_t lowAlarmValue = 10;

uint32_t timeCon = 0;

uint8_t resol = 12;

uint32_t millChngRes = 0;



void setup(void)
{
  Serial.begin(19200);
  Wire.begin(5, 14);
  while (!Serial);

//   SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(22,10);             // Start at top-left corner
    display.println("DS18B20");
    display.setCursor(5,40);             // Start at top-left corner
    display.println("FakeFinder");
    display.display();
     delay(1700);


 // ============== find all i2c adresses
  Serial.println("====================================");
  Serial.println(F("Finding all i2c adresses !"));
  byte error, address;
  int nDevices;
  nDevices = 0;
   for (address = 1; address < 127; address++ )  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0){
      Serial.println("-");
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.println("-");
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  } //for loop
  if (nDevices == 0){
    Serial.println("No I2C devices found");

  }
  delay(100);
 // ============== i2c test end.

 
  Serial.print("DallasTemperature library version: ");
  Serial.println(DALLASTEMPLIBVERSION);
}// setup end



void loop(void){

    if(resol==13){
      resol=9;
     }

  sensors.begin();
  //deviceCount = sensors.getDS18Count();
  deviceCount = sensors.getDeviceCount();


    Serial.println(""); 
    
    display.clearDisplay();
    display.setTextSize(1);             
    display.setTextColor(WHITE);        
    display.setCursor(0,0);             
    display.println("Dev.#: " + String(deviceCount));
    display.setCursor(0,11);             
    display.println(SetResolution(resol));
    display.setCursor(0,22);             
    
    delay(50);
  uint32_t startMillis = millis();
  sensors.requestTemperatures(); // Request temperature mesurments
  timeCon = millis()-startMillis; // calculate time
  Serial.println("************************************************************");
  Serial.print("TimeCon:");
  Serial.print(timeCon); 
  Serial.println(" ms");
  Serial.println("************************************************************");
   delay(50);

    display.println("SET alarm: " + method2());
    display.setCursor(0,33);             
    display.println("GOT alarm: " + readDevicesMethod2());
    display.setCursor(0,44);             
    display.println("TEMP: " + String(sensors.getTempCByIndex(0)) + " C");
    display.setCursor(0,55);             
    display.println("TimeCon: " + String(timeCon) + " ms");
    display.display();

    Serial.println(""); 

    delay(50);



// change resolution every xx sec
if (millis() >  millChngRes + 5000 && millis()>12000){
    resol++;
    millChngRes = millis();
}
    

}// === loop end



//================================================
String method2(){   // using direct OneWire write commands
  String msg;
  Serial.println("============================================================"); 
  Serial.print("Setting New Hi/Lo alarm values: ");
  highAlarmValue++;
  lowAlarmValue++;
  Serial.print(highAlarmValue);
  Serial.print("/");
  Serial.println(lowAlarmValue);
  
  Serial.println("Writing to devices");
  oneWire.reset_search();
  uint8_t addr[8];
  while (oneWire.search(addr)){
    if (OneWire::crc8(addr, 7) == addr[7]){
      Serial.print(".");
      oneWire.reset();
      oneWire.select(addr);
      oneWire.write(0x4E);           // Write to scratchpad
      oneWire.write(highAlarmValue); // Write high alarm value
      oneWire.write(lowAlarmValue);  // Write low alarm value
      oneWire.write(0x7F);           // Write configuration register, 12 bit temp res
      delay(30);  // dallas temp lib doesn't delay

      oneWire.reset();
      oneWire.select(addr);
      oneWire.write(0x48, PARASITE);  //copy scratchpad to eeprom, 1 - parasite power
      delay(20);                      // need at least 10ms eeprom write delay
      if (PARASITE) delay(10);
      msg = String(highAlarmValue) + " / " + String(lowAlarmValue);
    } else {
      Serial.println("Bad device addr!");
      msg = "Bad device addr!";
    }
  }

  Serial.println("Done");
  Serial.println("------------------------------------------------------------");
  return msg; 
}
//================================================



//================================================
String readDevicesMethod2(){   // using direct OneWire write commands
  Serial.println("Reading...");
  uint8_t hAlarmValue;
  uint8_t lAlarmValue;
  uint8_t addr[8];
  uint8_t data[9];
  String msg;
  oneWire.reset_search();
  while (oneWire.search(addr)){
    printAddress(addr);
    if (OneWire::crc8(addr, 7) == addr[7]){
      oneWire.reset();
      oneWire.select(addr);    
      oneWire.write(0xB8);        // Copy eeprom to scratchpad cmd, missing from DallasTemperature library
      delay(50);
      oneWire.reset();
      oneWire.select(addr);
      oneWire.write(0xBE);        // Read scratchpad cmd
      delay(50);
      for (uint8_t i = 0; i < 9; i++){
        data[i] = 0;
        data[i] = oneWire.read();
      }
      hAlarmValue = data[2];   // byte 2 is high temp alarm
      lAlarmValue = data[3];    // byte 3 is low temp alarm
      Serial.print("Hi/Lo now is: ");
      Serial.print(hAlarmValue);
      Serial.print("/");
      Serial.println(lAlarmValue);
      msg = String(hAlarmValue) + " / " + String(lAlarmValue);
    } else {
      Serial.println("Bad device addr!");
      msg = "Bad device addr!";
    }
  } 
 Serial.println("============================================================"); 
 return msg;
}
//================================================




//================================================
  // Loop through each device, print out address
String SetResolution(uint8_t resSet){
  String msg;
  for(int i=0;i<deviceCount; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
  { Serial.println("");
    Serial.print("Found device ");
    Serial.print(i, DEC);
    Serial.print(" with address: ");
    printAddress(tempDeviceAddress);
    Serial.println();
   
    Serial.print("Setting resolution to ");
    Serial.println(resSet, DEC);
    
    // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
    sensors.setResolution(tempDeviceAddress, resSet);
    
    
     Serial.print("Resolution actually set to: ");
    Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
    
    msg += "SetTo/Real: ";
    msg +=String(resSet, DEC);
    msg += " / ";
    msg += String(sensors.getResolution(tempDeviceAddress), DEC);
    
    Serial.println();
  }else{
    Serial.print("Found ghost device at ");
    Serial.print(i, DEC);
    Serial.print(" but could not detect address. Check power and cabling");
    msg = "Err.";
  }
  }

  return msg;
}
//================================================



//================================================
  void printAddress(DeviceAddress deviceAddress){
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0"); // zero pad the address if necessary
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) Serial.print(":");
  }

  Serial.println(" | " + String(sensors.getTempC(deviceAddress))+ " C");
}
//================================================
