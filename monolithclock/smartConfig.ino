#include <Adafruit_NeoPixel.h>
#include "WiFi.h"

// NTP settings
#define TIMEZONE 9 // timezone (GMT = 0, Japan = 9)
#define NTP_SERVER "pool.ntp.org"

#define WIFI_SMARTCONFIG true

#if !WIFI_SMARTCONFIG
// if you do not use smartConfifg, please specify SSID and password here
#define WIFI_SSID "SSID" // your WiFi's SSID
#define WIFI_PASS "PASS" // your WiFi's password
#endif

// LED color setting
#define LED_R 0
#define LED_G 255
#define LED_B 0

// starting LED number of each digit
#define MIN1 0
#define MIN10 7
#define HOUR1 15
#define COLON 14
#define HOUR10A 22
#define HOUR10B 24

#define TOTAL_LED 25
#define LED_CONTROL 16
Adafruit_NeoPixel pixels(TOTAL_LED, LED_CONTROL, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  pixels.begin();
  pixels.setPixelColor(HOUR1 + 3, pixels.Color(LED_R, LED_G, LED_B));
  pixels.setPixelColor(MIN10 + 3, pixels.Color(LED_R, LED_G, LED_B));
  pixels.setPixelColor(MIN1  + 3, pixels.Color(LED_R, LED_G, LED_B));
  pixels.show();

  wifiSetup();

  getNTP();
}

void loop() {
  static int prevMin = -1;
  static int prevCol = 0;
  static int prevDay = -1;
  int col, hour;
  struct tm tmtime;
    
  col = (millis() / 500) % 2;
  if(prevCol == col)
    return;
  prevCol = col;

  pixels.clear();
  if(col) {
    pixels.setPixelColor(COLON, pixels.Color(LED_R, LED_G, LED_B));
  }
  getLocalTime(&tmtime);
  if(prevDay > 0 && tmtime.tm_mday != prevDay) {
    // ESP.restart(); // reset itself at 0:00 to get accurate time
    getNTP();
  }
  prevDay = tmtime.tm_mday;
  hour = tmtime.tm_hour % 12;
  if(hour == 0) hour = 12;
  
  setDigit(HOUR1, hour % 10);
  setDigit(MIN10, tmtime.tm_min / 10);
  setDigit(MIN1 , tmtime.tm_min % 10);

  if(hour >= 10) {
    pixels.setPixelColor(HOUR10A, pixels.Color(LED_R, LED_G, LED_B));
    pixels.setPixelColor(HOUR10B, pixels.Color(LED_R, LED_G, LED_B));
  }
  pixels.show();
  delay(400);
}

#define NUM 10
#define SEG 7
int seg[NUM][SEG] = {
  {1, 1, 1, 0, 1, 1, 1},
  {0, 1, 0, 0, 1, 0, 0},
  {1, 1, 0, 1, 0, 1, 1},
  {1, 1, 0, 1, 1, 0, 1},
  {0, 1, 1, 1, 1, 0, 0},
  {1, 0, 1, 1, 1, 0, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 0, 1}
};

void setDigit(int pos, int n) {
  for(int j = 0; j < SEG; j++) {
    if(seg[n][j]) {
      pixels.setPixelColor(j + pos, pixels.Color(LED_R, LED_G, LED_B));
    }        
  }  
}

//////////////////////////////
// WiFi and NTP section
//////////////////////////////

void getNTP(void) {
  for(int i = 0; WiFi.status() != WL_CONNECTED; i++) {
    if(i > 30) {
      ESP.restart();
    }
    Serial.println("Waiting for WiFi connection..");
    delay(1000);
  }

  configTime(TIMEZONE * 3600L, 0, NTP_SERVER);
  printLocalTime();
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %Y-%m-%d %H:%M:%S");
}


void wifiSetup() {
  WiFi.mode(WIFI_STA);
#if WIFI_SMARTCONFIG
  WiFi.begin();
#else
  WiFi.begin(WIFI_SSID, WIFI_PASS);
#endif

  for (int i = 0; ; i++) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
#if WIFI_SMARTCONFIG
  if(i > 30)
    break;
#endif    
  }

#if WIFI_SMARTCONFIG
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

    //Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      Serial.print(".");
      pixels.clear();
      setDigit(MIN10, 5); //s
      pixels.setPixelColor(MIN1, pixels.Color(LED_R, LED_G, LED_B));
      pixels.setPixelColor(MIN1+2, pixels.Color(LED_R, LED_G, LED_B));
      pixels.setPixelColor(MIN1+5, pixels.Color(LED_R, LED_G, LED_B));
      pixels.setPixelColor(MIN1+6, pixels.Color(LED_R, LED_G, LED_B));
      pixels.show();
      delay(1000);
    }

    Serial.println("");
    Serial.println("SmartConfig received.");

    //Wait for WiFi to connect to AP
    Serial.println("Waiting for WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(",");
    }
  }
  Serial.println("WiFi Connected.");
#endif

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
