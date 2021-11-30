/*
  Simple GET client for ArduinoHttpClient library
  Connects to server once every five seconds, sends a GET request
  created 14 Feb 2016
  modified 22 Jan 2019
  by Tom Igoe
  
  this example is in the public domain
  https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/SimpleGet/SimpleGet.ino
 */
#include <ArduinoHttpClient.h>
#include <WiFi.h>

#include "arduino_secrets.h"


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char serverAddress[] = "192.168.1.153";  // server address
int port = 8080;

WiFiClient wifi;
//HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);

  //====== For testings =====
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  //=============
  
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

String Read_Sensors(){
    long temp = random(0, 40);
    long hum = random(10, 80);
    long co = random(10, 100);

    String res = "temp="+String(temp)+"&hum="+String(hum)+"&co2="+String(co);
    return res;
}

void loop() {
  HttpClient client = HttpClient(wifi, serverAddress, port);
  
  Serial.println("making GET request");
  //String req = "/proj/submit.php?temp=5&hum=5&co2=5";
  String sensors = Read_Sensors();
  String req = "/proj/submit.php?"+sensors;
  client.get(req);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println("Wait five seconds");
  client.stop();
  delay(5000);
}
