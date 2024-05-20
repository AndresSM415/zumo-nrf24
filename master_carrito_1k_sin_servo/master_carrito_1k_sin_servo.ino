int dir;
int x, y;
int motizqvel;
int motdervel;

const int bsprint = 3;
const int este = 2;

int vsprint;// activar o no sprint
int veste;
int vueltacorta; //voltear corto, adelante o atras full.

// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   9
#define CSN_PIN 10

const byte slaveAddress[6] = {'A','N','D','R','E','S'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int valores[5] = {0, 0, 0, 0};

void setup() {
    Serial.begin(9600);
    
    Serial.println("iniciando control");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setPALevel( RF24_PA_MIN );//MIN, LOW, HIGH, MAX
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);

    pinMode(bsprint, INPUT);
    pinMode(este, INPUT);
}

void loop() {
  veste = digitalRead(este);
  vsprint = digitalRead(bsprint);
  x = analogRead(A1);
  y = analogRead(A0);

  if (vsprint == LOW){
    if (y > 900){
      vueltacorta = 4;
    }
    else if (y < 200){
      vueltacorta = 3;
    }
    else{
      vueltacorta = 0;
    }
  }
  else if (veste == LOW){
    if (x < 400 && y < 200){
      vueltacorta = 1;//izq: atras, der: adelante.
    }
    else if (x > 640 && y < 200){
      vueltacorta = 2;//izq: adelante, der: atras.
    }
    else if (x > 640 && y > 200){
      vueltacorta = 1;//izq: atras, der: adelante.
    }
    else if (x < 400 && y > 200){
      vueltacorta = 2;//izq: adelante, der: atras.
    }
    else{
      vueltacorta = 0;
    }
  }
  
  else{
    vueltacorta = 0;
    if (y <= 220){//atras
      motizqvel = map(y, 520, 0, 0, 255);
      motdervel = motizqvel;
      dir = 1;
    }
    
    else {
      motizqvel = 0;
      motdervel = 0;
    }
    
    if(y >= 520){//adelante
        motizqvel = map(y, 520, 1023, 0, 255);
        motdervel = motizqvel;
        dir = 0;
      }
    
    if (x <= 510){
      int xMapa = map(x, 510, 0, 0, 255);
      motizqvel = motizqvel - xMapa;
      motdervel = motdervel + xMapa;

      if (motizqvel < 100){
        motizqvel = 0;
      }

      if (motdervel > 255){
        motdervel = 255;
      }
    }

    if (x >= 540){
      int xMapa = map(x, 540, 1023, 0, 255);
      motizqvel = motizqvel + xMapa;
      motdervel = motdervel - xMapa;

      if (motizqvel > 255){
        motizqvel = 255;
      }

      if (motdervel < 100){
        motdervel = 0;
      }
    }
  }
  valores[3] = vueltacorta;
  valores[2] = dir;
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
  Serial.print(motdervel);
  Serial.print(" || ");
  Serial.println(veste);
  
}
