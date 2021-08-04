#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SoftwareSerial.h>
//DICHIARIAMO TUTTE LE VARIABILI E LE COSTANTI
#define trigPin 6
#define echoPin 5
#define Led1 4
#define Led2 7
#define Led3 8
#define RST_PIN 9
#define SS_PIN 10
#define ServoPin 3
Servo ServoSerratura;
long durata, cm;
int Centimetri=0;
int Verifica=0;
bool ControllaLettera=false;
int T_Attesa = 2500;//Regola il tempo di attesa tra la serratura aperta e poi chiusa.
String Chiave = "C2 C5 67 1A"; //Sostituire con il codice della vostra chiave d'accesso
String Chiave2 = "17 47 83 34";
String content= "";
MFRC522 RFID(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600); //Inizializza la comunicazione Seriale
  pinMode(ServoPin, OUTPUT);
  ServoSerratura.attach(ServoPin);  
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  ServoSerratura.write(150);
  SPI.begin(); //Inizializzia il bus per la connessione SPI
  RFID.PCD_Init();  //Inizializza Il modulo RFID 
}

void loop() {
  //Controllo
   Verifica++;
      if(Verifica==230)
      {
        ControllaLettere();
        Verifica=0;
      }
    //Fine Controllo
    
    //Tiene sotto controllo il modulo RFID-RC522 e verifica se ci sono nuove chiavi
      if(!RFID.PICC_IsNewCardPresent())
      {
        return;
      }
    //Selezione la carta letta
    if(!RFID.PICC_ReadCardSerial())
      {
        return;
      }
  
  //Stampa sul monitor seriale il codice esadecimale della chiave
  //Serial.print("Codice chiave: ");
      content= "";
      for (byte i = 0; i < RFID.uid.size; i++)
      {
        content.concat(String(RFID.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(RFID.uid.uidByte[i], HEX));
      }
      content.toUpperCase();
 
  //Se la stringa letta dal modulo corrisponde a quella memorizzata esegue l'accesso altrimenti lo nega
  if(content.substring(1) == Chiave||content.substring(1) == Chiave2)
  {
    ServoSerratura.write(50);
    digitalWrite(Led2,HIGH);
    digitalWrite(Led1,LOW);
    delay(T_Attesa);
    digitalWrite(Led2,LOW);
    digitalWrite(Led1,HIGH);
    ServoSerratura.write(150);
  }
  else
  {
    Serial.write("b");
    digitalWrite(Led1,LOW);
    digitalWrite(Led3,HIGH);
    delay(T_Attesa);
    digitalWrite(Led1,HIGH);
    digitalWrite(Led3,LOW);
   }
}

void ControllaLettere()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  durata = pulseIn(echoPin, HIGH);
  cm = durata / 58; // per i pollici la formula è durata / 148;
  
  if(cm<21&&ControllaLettera==false)
  {
    Serial.write("a"); 
    ControllaLettera=true;
  }
   if(cm>21&&ControllaLettera==true)
  {
    Serial.write("c"); 
    ControllaLettera=false;
  }
return 0;

}
