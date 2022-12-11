#include <DHT.h>  // Including library for dht
#include <WiFi.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 25
#define SECRET_SSID "Nashaat"    // replace MySSID with your WiFi network name
#define SECRET_PASS "N12345678"  // replace MyPassword with your WiFi password

#define SECRET_CH_ID 1957148     // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "I5WO1S8RQ7Z1XU57"   // replace XYZ with your channel write API Key
char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values

String myStatus = "";

#define DHTPIN 14          //pin where the dht11 is connected

DHT dht(DHTPIN, DHT22);


OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float Celcius = 0;
float Fahrenheit = 0;


void setup()
{
  Serial.begin(115200);
  //delay(10);
  dht.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop()
{

  float t = dht.readTemperature();

  if ( isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  sensors.requestTemperatures();
  Celcius = Celcius - 1;
  Celcius = sensors.getTempCByIndex(0);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print("TEMPERATURE OF WATER");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, Celcius);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  Serial.print("Temperature of air: ");
  Serial.print(t);
  Serial.print(" degrees Celcius");
  Serial.println("%. Send to Thingspeak.");
  delay(5000); // Wait 20 seconds to update the channel again

}
