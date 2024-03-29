#include <Arduino.h>
#include <WiFi.h>
 #include <Adafruit_NeoPixel.h>
 #include <Arduino_GFX_Library.h>


#define LED_PIN    4

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 16
 
Arduino_DataBus *bus = new Arduino_ESP32SPI(
     33 /*DC or DE*/, 15 /* CS */,
    14 /* SCK */, 13 /* MOSI */,  12  /*  12 MISO */, HSPI, true);
 

Arduino_ILI9488_18bit *panel = new Arduino_ILI9488_18bit(bus, 26, 3, false);

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}
 

void initTime(String timezone){
  struct tm timeinfo;
  unsigned long elapse=millis();
  Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset
  bool tsuccess=false;
  while(millis()-elapse < 5000){
    if(!getLocalTime(&timeinfo)){
      Serial.println("  Failed to obtain time");
      delay(100);
    }
    else{
      Serial.println("  Got the time from NTP");
      setTimezone(timezone);
      tsuccess=true;
      break;
    }
  }
  if(!tsuccess){
    Serial.println("ntp timed out");
  }
}
 

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time 1");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}


void setup() {
Serial.begin(115200);
WiFi.mode(WIFI_STA);
WiFi.begin("NETGEAR23","rustictree335"); //

while(WiFi.status() != WL_CONNECTED){
  Serial.print(".");
  delay(500);
}
Serial.println();
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
strip.show();            // Turn OFF all pixels ASAP
strip.setBrightness(255); // Set BRIGHTN
panel->begin();
panel->fillScreen(RED);
panel->setCursor(10,10);
panel->setTextColor(BLACK);
panel->println("Shabbat in five minutes")

}
 

void loop() {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, strip.Color(255,   0,   0));         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(200);                           //  Pause for a moment
  }
}