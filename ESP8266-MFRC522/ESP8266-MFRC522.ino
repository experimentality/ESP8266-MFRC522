#include <Arduino.h>

/*
Many thanks to nikxha from the ESP8266 forum
*/

#include <ESP8266WiFi.h>
#include <SPI.h>
#include "MFRC522.h"

/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = D5
SDA(SS) = LedAzulD4 
MOSI    = D13
MISO    = D12
SCK     = D14
GND     = GND
3.3V    = 3.3V
*/

#define RST_PIN	5  // RST-PIN fÃ¼r RC522 - RFID - SPI - Modul D5 
#define SS_PIN	4  // SDA-PIN fÃ¼r RC522 - RFID - SPI - Modul LedAzulD4 


#define LedAzulD4 2
#define LedRojoD3 0
//#define RelayD8 15
#define BUTTON 16
#define BUTTON2 15

int ledPin = 13; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value
int boton=0;
int boton2=0;

const char *ssid =	"yourSSID";	    // change according to your Network - cannot be longer than 32 characters!
const char *pass =	"yourPASSWORD";	// change according to your Network

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance

void setup() {
  pinMode(BUTTON, INPUT);      // INPUT BUTTONS
  pinMode(BUTTON2, INPUT);
  pinMode(LedRojoD3, OUTPUT);
  pinMode(LedAzulD4, OUTPUT);
  //pinMode(RelayD8, OUTPUT);

  
  Serial.begin(9600);    // Initialize serial communications
  delay(250);
  Serial.println(F("Booting...."));
  
  SPI.begin();	         // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  
  WiFi.begin(ssid, pass);
  
  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi connected"));
  }
  
  Serial.println(F("Ready!"));
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));
}

void loop() { 
  boton = digitalRead(BUTTON);   // read the input pin
  boton2 = digitalRead(BUTTON2);
  digitalWrite(LedAzulD4, HIGH);

  if (boton == 1 || boton2 == 1) //Aqui van los UIDs autorizados
  {
    Serial.println("Acceso autorizado");
    Serial.println();
        
        //digitalWrite(RelayD8, HIGH); 
        digitalWrite(LedAzulD4, LOW);       
        delay(2000);
        digitalWrite(LedAzulD4, HIGH); 
        //digitalWrite(RelayD8, LOW);
  }
  

  // Busca nuevas tarjetas
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Selecciona una de las tarjetas
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Muestra el UID en el monitor serial.
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "36 BA 1B 7E" || 
  content.substring(1) == "63 AF 36 6A" ||
  content.substring(1) == "3B 75 74 AF" 
  ) //Aqui van los UIDs autorizados

  {
    Serial.println("Acceso autorizado");
    Serial.println();
        digitalWrite(LedAzulD4, LOW);       
        delay(2000);
        digitalWrite(LedAzulD4, HIGH); 

  }
 
 else   {
    Serial.println(" Acceso negado"); 
    for(int i=0; i<4;i++){
        digitalWrite(LedRojoD3, HIGH);       
        delay(100);
        digitalWrite(LedRojoD3, LOW); 
        delay(100);
    }
  }
}



