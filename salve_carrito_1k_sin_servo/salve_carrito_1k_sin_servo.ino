const int in1 = 2;
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;
int dir;
int motizqvel;
int motdervel;
int vueltacorta;
int vbt1, vbt2;

// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   49
#define CSN_PIN 53

const byte thisSlaveAddress[6] = {'A','N','D','R','E','S'};

RF24 radio(CE_PIN, CSN_PIN);

int valor[5];

void setup() {
  Serial.begin(9600);
  

  Serial.println("iniciando carrito");
  
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setPALevel( RF24_PA_MIN );
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}


void loop() {
  if ( radio.available() ) {
    radio.read( &valor, sizeof(valor) );
    motizqvel = valor[0];
    motdervel = valor[1];
    dir = valor[2];
    vueltacorta = valor[3];
    Serial.print("izq ");
    Serial.print(motizqvel);
    Serial.print(" der ");
    Serial.println(motdervel);
  }
  
  //else{
    //Serial.println("no disponible");
  //}

  if (vueltacorta == 0){
    if (dir == 0){
      digitalWrite(in1, LOW);
      analogWrite(in2, motizqvel);
      digitalWrite(in3, LOW);
      analogWrite(in4, motdervel);
    }
    else if (dir == 1){
        analogWrite(in1, motizqvel);
        digitalWrite(in2, LOW);
        analogWrite(in3, motdervel);
        digitalWrite(in4, LOW);
      }
    }
    else if (vueltacorta != 0){
      if (vueltacorta == 1){
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
      }
      else if (vueltacorta == 2){
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
      }
      else if (vueltacorta == 3){
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
      }
      else if (vueltacorta == 4){
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
      }
    }
  }
