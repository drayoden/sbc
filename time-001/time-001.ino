#include <WiFiManager.h>
#include <time.h>

const long GMT7HRS = -25200;   // offset for pacific timezone
const int DLS_OFFSET =  0;  
const char* NTP_SERVER = "pool.ntp.org";

struct tm timeinfo;


void setup() {

  Serial.begin(115200);
  
  wifiInit();

  configTime(GMT7HRS, DLS_OFFSET, NTP_SERVER);
}



void loop() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to get time...");
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("HH:MM -- ");
  Serial.println(&timeinfo,"%H:%M"); 
  delay(5000);
  
}

void wifiInit() {

  WiFi.mode(WIFI_STA);  // station mode first to present wifi config site:
  WiFiManager wm;       // initialize the object...
  // wm.resetSettings();   // reset it each time for now...
  
  Serial.println("autoConnect...");

  if (! wm.autoConnect("MonlithAP","password")) {
    Serial.println("Cannot connect to wifi...");
    Serial.println("Execution stopped...");
    exit(2);
  } else {
    Serial.print("Connected to SSID: ");
    Serial.println(wm.getConfigPortalSSID());
  }

}
