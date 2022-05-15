
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);


//Gas
int Gas_digital = 2;

// DHT22
#include "DHTesp.h"
const int DHT_PIN = 19;
DHTesp dht;



//Rain
#define sensorPower 4
#define sensorPin 16


const char* ssid = "SKYEUQH7";
const char* password = "LLNFeeQbnqjh";

const int greenled = 14;
const int redled = 5;


// Domain name with URL path or IP address with path
String serverName = "http://iot.creatxsoftware.com/api/dataset";


void setup() {

  //  Gas
  pinMode(Gas_digital, INPUT);
  Serial.begin(9600);


  //  Display
  lcd.begin(20, 4);
  lcd.init();
  lcd.backlight();
  //  Serial.begin(115200);

  //  LED
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  digitalWrite(redled, HIGH);


  // DHT22
  dht.setup(19, DHTesp::DHT22);

  //  Rain
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);


  //Wifi
  WiFi.begin(ssid, password);
  lcd.print("Connecting");
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
    Serial.print(".");
    digitalWrite(redled, LOW);
    digitalWrite(greenled, HIGH);
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.print("Connected to WiFi");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {





  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

 lcd.clear();

//Gas
  int gassensorDigital = digitalRead(Gas_digital);
  Serial.print("Gas Class: ");
  Serial.print(gassensorDigital);

  
  if (gassensorDigital > 0) {
    lcd.setCursor(0, 2);
    Serial.println("Gas");
      lcd.print("Smoke: Gas Found");
  }
  else {
    lcd.setCursor(0, 2);
    Serial.println("Gas");
      lcd.print("Smoke: Gas Not Found");
    Serial.println("No Gas");
  }



  


    // DHT22
    float humidity = dht.getHumidity();
    float temprature = dht.getTemperature();

    Serial.print("Humidity: ");
    Serial.println(humidity);

    Serial.print("temprature: ");
    Serial.println(temprature);

   lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.setCursor(0, 1);
    lcd.print("Temprature: ");
    lcd.print(temprature);





    //Rain
    int val = readSensor();
    Serial.print("Digital Output: ");
    Serial.println(val);

    // Determine status of rain
    if (val) {
      lcd.setCursor(0, 3);
      lcd.print("Rain: It's Clear");
      Serial.println("Rain: It's Clear");
    } else {
      lcd.setCursor(0, 3);
      lcd.print("Rain: It's raining");
      Serial.println("Rain: It's raining");

    }

    Serial.println(" ");

  
    String serverPath = serverName + "?rain="+val+"&temprature="+temprature+"&huminity="+humidity+"&gas="+gassensorDigital+"";



    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      //      String payload = http.getString();
      //      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
    digitalWrite(redled, HIGH);
    digitalWrite(greenled, LOW);
  }
  delay(1000);
  Serial.println(" Gas");

}

int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = digitalRead(sensorPin); // Read the sensor output
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return the value
}
