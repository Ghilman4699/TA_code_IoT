#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

ESP8266WebServer server(80);
#define SS_PIN D10
#define RST_PIN D9
#define led1 D7
#define led2 D6
#define led3 D5
#define LPG A0
#define solenoid D8
int sensorValue = 0;
int outputValue = 0;
const byte interrupt1 = D3;
const byte interrupt2 = D4;
MFRC522 mfrc522(SS_PIN, RST_PIN); 
String data = "";

void web_root_handle(){
  String web = "";
  web += "<a href=\"/buka\">bukakunci</a>\n";
  web += "<a href=\"/tutup\">tutupkunci</a>\n"; 
  web += "<a href=\"/bacaLPG\">sensorValue</a>\n"; 
  web += "<a href=\"/hidup1\">Lampu1ON</a>\n"; 
  web += "<a href=\"/mati1\">Lampu1OFF</a>\n"; 
  web += "<a href=\"/hidup2\">Lampu2ON</a>\n"; 
  web += "<a href=\"/mati2\">Lampu2OFF</a>\n"; 
  web += "<a href=\"/hidup3\">Lampu3ON</a>\n"; 
  web += "<a href=\"/mati3\">Lampu3OFF</a>\n"; 
  web += "<a href=\"/semuaON\">nyalakan semua lampu</a>\n"; 
  web += "<a href=\"/semuaOFF\">matikan semua lampu</a>\n"; 

  server.send(200, "text/html", web);
  }  
  
void buka_kunci(){
  
  String web = "";
  
  web += "<strong>Berhasil dibuka</strong>";

  Serial.println("ON");
  Serial.println("Tombol dihidupkan..");
  digitalWrite(D8, HIGH);
  server.send(200, "text/html", web);
  } 

void gas(){
  String web = "";

  web += "<strong>MQ 2 ON</strong>";

  sensorValue = analogRead(A0); 
  Serial.print("pembacaan sensor = ");
  Serial.println(sensorValue);
  outputValue = map(sensorValue, 0, 1023, 0, 5);
  
  switch(outputValue){
    case 0:
      Serial.println("tidak ada kebocoran");
      break;
    case 1:
      Serial.println("ada kebocoran");
      break;
    case 2:
      Serial.println("bahaya");
      break;
    case 3:
      Serial.println("sangat bahaya");
      break;
    case 4:
      Serial.println("warning!!");
      break;
    case 5:
      Serial.println("sangat warning!!");
      break;
    }
  delay(1);
  
  server.send(200, "text/html", web);
}

void lampu_1_ON(){
  String web = "";

  web += "<strong>lampu satu dihidupkan/strong>";

  Serial.println("Lampu 1 ON");
  digitalWrite(D7, HIGH);

  server.send(200, "text/html", web);
  }  

void lampu_1_OFF(){
  String web = "";

  web += "<strong>lampu satu dimatikan/strong>";

  Serial.println("Lampu 1 OFF");
  digitalWrite(D7, LOW);

  server.send(200, "text/html", web);
  }  

void lampu_2_ON(){
  String web = "";

  web += "<strong>lampu dua dihidupkan/strong>";

  Serial.println("Lampu 2 ON");
  digitalWrite(D6, HIGH);

  server.send(200, "text/html", web);
  }  

void lampu_2_OFF(){
  String web = "";

  web += "<strong>lampu dua dimatikan/strong>";

  Serial.println("Lampu 2 OFF");
  digitalWrite(D6, LOW);

  server.send(200, "text/html", web);
  }  

void lampu_3_ON(){
  String web = "";

  web += "<strong>lampu tiga dihidupkan/strong>";

  Serial.println("Lampu 3 ON");
  digitalWrite(D5, HIGH);

  server.send(200, "text/html", web);
  }  

void lampu_3_OFF(){
  String web = "";

  web += "<strong>lampu tiga dimatikan/strong>";

  Serial.println("Lampu 3 OFF");
  digitalWrite(D5, LOW);

  server.send(200, "text/html", web);
  }  

void nyala_semua(){
  String web = "";

  web += "<strong>semua lampu menyala/strong>";

  Serial.println("Semua lampu ON");
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);

  server.send(200, "text/html", web);
  }  

void mati_semua(){
  String web = "";

  web += "<strong>semua lampu dimatikan/strong>";

  Serial.println("Semua lampu OFF");
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);

  server.send(200, "text/html", web);
  }  

void baca_rfid(){
   //   Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  data = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     data = data + String(mfrc522.uid.uidByte[i]);
     Serial.print(mfrc522.uid.uidByte[i], DEC);
  }
  Serial.println("\nreaded : " + data);
  if(data == "18589242176"){
      Serial.println("kartu dikenali..");
      digitalWrite(D8, HIGH);
      delay(2000);
    }else{
      Serial.println("kartu tdk dikenal..");
      digitalWrite(D8, LOW);
      delay(2000);
      }
  }

void tutup_kunci(){
  String web = "";
  web += "<strong>Berhasil dikunci</strong>";
  
  Serial.println("OFF");
  Serial.println("Tombol Dimatikan Ditekan!");
  digitalWrite(D8, LOW);

  server.send(200, "text/html", web);
  }   

void buka_kunci2(){
  String web = "";
  web += "<strong>kunci dibuka</strong>";

  digitalWrite(D8, HIGH);
  Serial.println("Kunci rumah dibuka");

  server.send(200, "text/html", web);
  }
    
void setup() {
  pinMode(interrupt1, INPUT);   //interrupt pin1
  pinMode(interrupt2, INPUT);   //interrupt pin2
  pinMode(D7, OUTPUT);          //lampu1
  pinMode(D6, OUTPUT);          //lampu2
  pinMode(D5, OUTPUT);          //lampu3
  pinMode(D8, OUTPUT);          //solenoid
  pinMode(A0,OUTPUT);           //sensor gas LPG
  Serial.begin(115200);
  WiFi.begin("Rumah", "Khususrahmabayar2000");
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  Serial.println("\n\nPlease wait Device is Traying to Connecting.. \n[");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("*");
    delay(100);
    }
    Serial.println("\t[OK]");
    Serial.println("\nConnected");
    Serial.println(WiFi.localIP());

    server.on("/", web_root_handle);
    server.on("/buka", buka_kunci);
    server.on("/bacaLPG", gas);
    server.on("/hidup1", lampu_1_ON);
    server.on("/mati1", lampu_1_OFF);
    server.on("/hidup2", lampu_2_ON);
    server.on("/mati2", lampu_2_OFF);
    server.on("/hidup3", lampu_3_ON);
    server.on("/mati3", lampu_3_OFF);
    server.on("/semuaON", nyala_semua);
    server.on("/semuaOFF", mati_semua);
    server.on("/tutup", tutup_kunci);
    server.begin();
    attachInterrupt(digitalPinToInterrupt(interrupt1), baca_rfid, RISING);
    attachInterrupt(digitalPinToInterrupt(interrupt2), buka_kunci2, RISING);
}

void loop() {
server.handleClient();
}
