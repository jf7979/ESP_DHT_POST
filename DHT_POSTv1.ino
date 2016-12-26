#include <ESP8266WiFi.h>
#include <DHT.h>

const char* ssid     = "SSID";
const char* password = "password";

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);



void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  }
  dht.begin();
}




void loop() {
  

  // Read humidity
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  WiFiClient client;

  String PostData = "{\"node\": \"LivingRoom\",\"humidity\": ";
  PostData += (float)h, 2;
  PostData += ",";
  PostData += "\"temp_c\": ";
  PostData += (float)t, 2;
  PostData += ",";
  PostData += "\"temp_f\": ";
  PostData += (float)f, 2;
  PostData += "}";


  const uint16_t port = 8080;
  const char* host = "192.168.1.18";

  if (client.connect(host, port)) {
    digitalWrite(LED_BUILTIN, LOW);
    client.println("POST /posts HTTP/1.1");
    client.println("Host: 192.168.1.18");
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    client.stop();
    digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(60000);

  
}

