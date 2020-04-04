#include <SPI.h>
#include <MFRC522.h>
//#define led1 D12
#define led2 13
#define IRQ 2
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); 
String data = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(13, OUTPUT);
  SPI.begin();
  mfrc522.PCD_Init();
//pinMode(D12, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(IRQ), baca_rfid, RISING);
}

//
//void program(){
//  digitalWrite(D12, HIGH);
//  delay(1000);
//  digitalWrite(D12, LOW);
//  delay(1000);
//  Serial.println("interrupt on");
//  }

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
  Serial.println("LOOP JALAN");
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
      digitalWrite(8, HIGH);
      delay(2000);
    }else{
      Serial.println("kartu tdk dikenal..");
      digitalWrite(8, LOW);
      delay(2000);
      }
  }
