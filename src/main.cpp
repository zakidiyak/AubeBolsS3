#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#include <EthernetUdp.h>

#include <OSCBundle.h>

#define W5500_CS   14
#define W5500_RST  9
#define W5500_INT  10
#define W5500_MISO 12
#define W5500_MOSI 11
#define W5500_SCK  13

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Bol 1
//byte mac[] = { 0xDE, 0xBD, 0xBE, 0xEF, 0xFE, 0xED }; //Bol 2
//byte mac[] = { 0xDE, 0xCD, 0xBE, 0xEF, 0xFE, 0xED }; //Bol 3

IPAddress ip(192, 168, 0, 51); //Bol 1
//IPAddress ip(192, 168, 0, 52); //Bol 2
//IPAddress ip(192, 168, 0, 53); //Bol 3

EthernetUDP Udp;

int bulb1 = 33;
int bulb2 = 34;
int bulb3 = 35;
int bulb4 = 36;
int bulb5 = 37;

const unsigned int localPort = 8888;

void setBulb(int bulbIndex, float intensity);

void setup() {


  setBulb(1, 0.0);
  setBulb(2, 0.0);
  setBulb(3, 0.0);
  setBulb(4, 0.0);
  setBulb(5, 0.0);

  // Reset W5500
  pinMode(W5500_RST, OUTPUT);
  digitalWrite(W5500_RST, LOW);
  delay(100);
  digitalWrite(W5500_RST, HIGH);
  delay(200);

  // Init SPI with explicit pins
  SPI.begin(W5500_SCK, W5500_MISO, W5500_MOSI, W5500_CS);

  // Tell Ethernet lib which CS to use
  Ethernet.init(W5500_CS);

  // Start Ethernet
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  
}

void setBulb(int bulbIndex, float intensity) {
  
  if(bulbIndex == 1) {
    analogWrite(bulb1, intensity * 255);
  }
  else if(bulbIndex == 2) {
    analogWrite(bulb2, intensity * 255);
  }
  else if(bulbIndex == 3) {
    analogWrite(bulb3, intensity * 255);
  }
  else if(bulbIndex == 4) {
    analogWrite(bulb4, intensity * 255);
  }
  else if(bulbIndex == 5) {
    analogWrite(bulb5, intensity * 255);
  }
  
}

void setBulb(int bulbIndex, OSCMessage &msg) {
  
  float intensity = msg.getFloat(0);

  if(bulbIndex == 1) {
    analogWrite(bulb1, intensity * 255);
  }
  else if(bulbIndex == 2) {
    analogWrite(bulb2, intensity * 255);
  }
  else if(bulbIndex == 3) {
    analogWrite(bulb3, intensity * 255);
  }
  else if(bulbIndex == 4) {
    analogWrite(bulb4, intensity * 255);
  }
  else if(bulbIndex == 5) {
    analogWrite(bulb5, intensity * 255);
  }
  
}

void handleBulbs(OSCMessage &msg, int addrOffset) {
  // addrOffset points to what's after "/bulb"
  // e.g. "/1", "/2", etc.

  if (msg.match("/1", addrOffset)) {
    setBulb(1, msg);
    //Serial.println("Bulb 1 : " + String(msg.getFloat(0)));
  } 
  else if (msg.match("/2", addrOffset)) {
    setBulb(2, msg);
    //Serial.println("Bulb 2 : " + String(msg.getFloat(0)));
  }
  else if (msg.match("/3", addrOffset)) {
    setBulb(3, msg);
    //Serial.println("Bulb 3 : " + String(msg.getFloat(0)));
  }
  else if (msg.match("/4", addrOffset)) {
    setBulb(4, msg);
    //Serial.println("Bulb 4 : " + String(msg.getFloat(0)));
  }
  else if (msg.match("/5", addrOffset)) {
    setBulb(5, msg);
    //Serial.println("Bulb 5 : " + String(msg.getFloat(0)));
  }
}


void loop() {
  OSCBundle bundle;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }

    if (!bundle.hasError()) {
      bundle.route("/bulb", handleBulbs);
    }
  }

  delay(1);
}