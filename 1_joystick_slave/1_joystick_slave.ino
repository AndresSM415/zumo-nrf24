const int motizq = 5;
const int motder = 3;
int motizqvel;
int motdervel;

// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

const byte thisSlaveAddress[6] = {'A','N','D','R','E','S'};

RF24 radio(CE_PIN, CSN_PIN);

int valor[3];

void setup() {

    Serial.begin(9600);

    Serial.println("iniciando carrito");
    
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setPALevel( RF24_PA_MIN );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();

    pinMode(motizq, OUTPUT);
    pinMode(motder, OUTPUT);
}

void loop() {
  if ( radio.available() ) {
    radio.read( &valor, sizeof(valor) );
    motizqvel = valor[0];
    motdervel = valor[1];
    Serial.print("izq ");
    Serial.print(motizqvel);
    Serial.print(" der ");
    Serial.println(motdervel);
  }
  
  else{
    Serial.println("no disponible");
  }

  analogWrite(motizq, motizqvel);
  analogWrite(motder, motdervel);
}
