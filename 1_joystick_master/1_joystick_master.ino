int x;
int y;
int motizqvel = 0;
int motdervel = 0;

// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   9
#define CSN_PIN 10

const byte slaveAddress[6] = {'A','N','D','R','E','S'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int valores[3] = {0, 0};

void setup() {
    Serial.begin(9600);
    
    Serial.println("iniciando control");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setPALevel( RF24_PA_MIN );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
}

void loop() {
  x = analogRead(A0);
  y = analogRead(A1);

  if (y >= 510){
      motizqvel = map(y, 505, 1023, 0, 255);
      motdervel = motizqvel;
  }
  
  else {
    motizqvel = 0;
    motdervel = 0;
  }

  if (x <= 510){
    int xMapa = map(x, 510, 0, 0, 255);
    motizqvel = motizqvel - xMapa;
    motdervel = motdervel + xMapa;

    if (motizqvel < 0){
      motizqvel = 0;
    }

    if (motdervel > 255){
        motdervel = 255;
    }
  }

  if (x >= 515){
    int xMapa = map(x, 525, 1023, 0, 255);
    motizqvel = motizqvel + xMapa;
    motdervel = motdervel - xMapa;

    if (motizqvel > 255){
      motizqvel = 255;
    }

    if (motdervel < 0){
      motdervel = 0;
    }
  }
  
  valores[1] = motizqvel;
  valores[0] = motdervel;

  radio.write( &valores, sizeof(valores) );
  
  Serial.print("x ");
  Serial.print(x);
  Serial.print(" y ");
  Serial.print(y);
  Serial.print("   ||   val izq ");
  Serial.print(motizqvel);
  Serial.print(" val der ");
  Serial.println(motdervel);
  
}
