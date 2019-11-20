#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include <DHT.h>;
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


int status = WL_IDLE_STATUS;
#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)

char serverAddress[] = "130.15.85.65";  // server address
int port = 80;
WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

int chk;
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
float temp; //Stores temperature value


void setup() {
    // check for the WiFi module:
    Serial.begin(9600);
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
         // declare LED as output
  pinMode(inputPin, INPUT);
  dht.begin();
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
 

}

void loop() {
  // put your main code here, to run repeatedly:
val = digitalRead(inputPin);  // read input value
  if (val == HIGH) { 
    // check if the input is HIGH
    wifiPost(9, 1, "Trigger12", "Trigger2", "Trigger3");
     // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    
    if (pirState == HIGH){
      wifiPost(9, 1, "Trigger12", "Trigger2", "Trigger3");
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }

delay(2000);
    //Read data and store it to variables hum and temp
    
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
   
    Serial.print("  Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    delay(10000); //Delay 2 sec.



}



//function to write post request to server
void wifiPost(int groupNum, int alertVar, String Trigger_1Var, String Trigger_2Var, String Trigger_3Var){
  int alert = (groupNum*10)+alertVar;
  
  //Post Request
  String postLine ="http://130.15.85.65/data.php?";
  Serial.println("making POST request");
  // Post request format ("serverLocation.php?Alert="+alertVar+"&Trigger_1="+Trigger_1Var+"&Trigger_2="+Trigger_2Var+"&Trigger_3="+Trigger_3Var)
  //"http://130.15.85.65/data.php?Alert=2&Trigger_1=Temp32&Trigger_2=PersonYes&Trigger_3=MotionYes"
  postLine += F("Alert=");
  postLine += alert;
  postLine += F("&Trigger_1=");
  postLine += Trigger_1Var;
  postLine += F("&Trigger_2=");
  postLine += Trigger_2Var;
  postLine += F("&Trigger_3=");
  postLine += Trigger_3Var;
  Serial.println(postLine);
  client.post(postLine);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
 
}
