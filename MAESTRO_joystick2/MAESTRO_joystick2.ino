#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>
#include <Wire.h>
#include <Servo.h>  // Incluimos la librería para los servos

// Definir los pines de los servos
Servo rueda_d_dere;
Servo rueda_d_izq;
Servo rueda_t_dere;
Servo rueda_t_izq;

int servoPin1 = 3;
int servoPin2 = 5;
int servoPin3 = 6;
int servoPin4 = 9;

// Definir los pines de los motores controlados por el L298N
int motor1_IN1 = 25;
int motor1_IN2 = 27;
int motor1_IN3 = 29;
int motor1_IN4 = 31;

int motor2_IN1 = 33;
int motor2_IN2 = 35;
int motor2_IN3 = 37;
int motor2_IN4 = 39;

int motor3_IN1 = 41;
int motor3_IN2 = 43;
int motor3_IN3 = 45;
int motor3_IN4 = 47;

const int ENA = 2;

// Inicializamos las variables de velocidad y ángulos
int velocidadMotor = 255;  // Velocidad máxima del motor (0-255)

////////////////////////////////////////////
int angulo_ini_d_dere=90;
int angulo_ini_d_izq=90;
int angulo_ini_t_dere=90;
int angulo_ini_t_izq=90;

////////////////////////////////////////////
int angulo_giro_d1_dere=0;
int angulo_giro_d2_dere=0;

////////////////////////////////////////////
int angulo_giro_d1_izq=180;
int angulo_giro_d2_izq=180;
////////////////////////////////////////////////////




String datos_lora[6];

double latitud_min=0.0000000;
double longitud_min=0.0000000;
 int lat_grados= 0;
      int lng_grados= 0;
    int lat_minutos=0;
    int lat_segundos=0;
    int lng_minutos=0;
     int lng_segundos=0;
     int llego=0;
     char l_lat='x';
     char l_lon='X';
   float temperature = 0.000;
  float humidity = 0.000;

   // Variables para almacenar los segmentos de datos
float temperatura = 0.0;
float humedad = 0.0;
float glp = 0.0;          // Cambiado de "valor3" a "glp"
float distancia = 0.0;    // Cambiado de "valor4" a "distancia"
String coordenada1 = "";
String coordenada2 = "";




 
String incomingData = ""; // Variable para almacenar la trama completa
bool receiving = false;   // Indicador de inicio de recepción

void setup() {
  Serial.begin(9600); // Configura la velocidad del puerto serial
    Serial3.begin(9600); // Configura la velocidad del puerto serial
   pinMode(A15, OUTPUT); // Configura A15 como salida 

 // Inicializar los servos
 rueda_d_dere.attach(servoPin1);
 rueda_d_izq.attach(servoPin2);
 rueda_t_dere.attach(servoPin3);
 rueda_t_izq.attach(servoPin4);

  // Inicializar los pines de control de los motores como salida
  pinMode(motor1_IN1, OUTPUT);
  pinMode(motor1_IN2, OUTPUT);
  pinMode(motor1_IN3, OUTPUT);
  pinMode(motor1_IN4, OUTPUT);

  pinMode(motor2_IN1, OUTPUT);
  pinMode(motor2_IN2, OUTPUT);
  pinMode(motor2_IN3, OUTPUT);
  pinMode(motor2_IN4, OUTPUT);

  pinMode(motor3_IN1, OUTPUT);
  pinMode(motor3_IN2, OUTPUT);
  pinMode(motor3_IN3, OUTPUT);
  pinMode(motor3_IN4, OUTPUT);
  
  pinMode(ENA, OUTPUT);

  

digitalWrite(motor1_IN1, LOW);
digitalWrite(motor1_IN2, LOW);
digitalWrite(motor1_IN3, LOW);
digitalWrite(motor1_IN4, LOW);

digitalWrite(motor2_IN1, LOW);
digitalWrite(motor2_IN2, LOW);
digitalWrite(motor2_IN3, LOW);
digitalWrite(motor2_IN4, LOW);

digitalWrite(motor3_IN1, LOW);
digitalWrite(motor3_IN2, LOW);
digitalWrite(motor3_IN3, LOW);
digitalWrite(motor3_IN4, LOW);

  // Mover los servos a su posición inicial
  rueda_d_dere.write(angulo_ini_d_dere);
  rueda_d_izq.write(angulo_ini_d_izq);
  rueda_t_dere.write(angulo_ini_t_dere);
  rueda_t_izq.write(angulo_ini_t_izq);

  
   while (!Serial);
  Serial.println("LoRa Sender12");
  
  if (!LoRa.begin(433E6)) 
  { // o 915E6, dependiendo de la frecuencia de tu módulo
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  
  //Serial.println("paso");
}



/////////////////////////////////////////////
//
////////////////////////////////////////////
void parar(void)
{


digitalWrite(motor1_IN1, LOW);
digitalWrite(motor1_IN2, LOW);
digitalWrite(motor1_IN3, LOW);
digitalWrite(motor1_IN4, LOW);

digitalWrite(motor2_IN1, LOW);
digitalWrite(motor2_IN2, LOW);
digitalWrite(motor2_IN3, LOW);
digitalWrite(motor2_IN4, LOW);

digitalWrite(motor3_IN1, LOW);
digitalWrite(motor3_IN2, LOW);
digitalWrite(motor3_IN3, LOW);
digitalWrite(motor3_IN4, LOW);

delay (200);
angulo_ini_d_dere=90;
angulo_ini_d_izq=90;
angulo_ini_t_dere=90;
angulo_ini_t_izq=90;


  rueda_d_dere.write(angulo_ini_d_dere);
  rueda_d_izq.write(angulo_ini_d_izq);
  rueda_t_dere.write(angulo_ini_t_dere);
  rueda_t_izq.write(angulo_ini_t_izq);

  
}




/////////////////////////////////////////
//
//////////////////////////////////////

void fadelante(void)
{angulo_ini_d_dere=90;
angulo_ini_d_izq=90;
angulo_ini_t_dere=90;
angulo_ini_t_izq=90;


  rueda_d_dere.write(angulo_ini_d_dere);
  rueda_d_izq.write(angulo_ini_d_izq);
  rueda_t_dere.write(angulo_ini_t_dere);
  rueda_t_izq.write(angulo_ini_t_izq);
delay (200);

digitalWrite(motor1_IN2, HIGH);
digitalWrite(motor1_IN1, LOW);

digitalWrite(motor1_IN4, HIGH);
digitalWrite(motor1_IN3, LOW);
////////////////////////////////////////

digitalWrite(motor2_IN2, HIGH);
digitalWrite(motor2_IN1, LOW);

digitalWrite(motor2_IN4, HIGH);
digitalWrite(motor2_IN3, LOW);
/////////////////////////////////////////////

digitalWrite(motor3_IN2, HIGH);
digitalWrite(motor3_IN1, LOW);

digitalWrite(motor3_IN4, HIGH);
digitalWrite(motor3_IN3, LOW);

analogWrite(ENA, 128);
}
/////////////////////////////////////////
//
//////////////////////////////////////

void derecha(void)
{
digitalWrite(motor1_IN1, LOW);
digitalWrite(motor1_IN2, LOW);
digitalWrite(motor1_IN3, LOW);
digitalWrite(motor1_IN4, LOW);

digitalWrite(motor2_IN1, LOW);
digitalWrite(motor2_IN2, LOW);
digitalWrite(motor2_IN3, LOW);
digitalWrite(motor2_IN4, LOW);

digitalWrite(motor3_IN1, LOW);
digitalWrite(motor3_IN2, LOW);
digitalWrite(motor3_IN3, LOW);
digitalWrite(motor3_IN4, LOW);

delay (500);
rueda_d_dere.write(135);//
rueda_d_izq.write(135);
rueda_t_dere.write(45);//
rueda_t_izq.write(45);

  
}
//////////////////////////////////////////
////////
/////////////////////////////////////////
void izquierda(void)
{
   
digitalWrite(motor1_IN1, LOW);
digitalWrite(motor1_IN2, LOW);

digitalWrite(motor1_IN3, LOW);
digitalWrite(motor1_IN4, LOW);
////////////////////////////////////////

digitalWrite(motor2_IN1, LOW);
digitalWrite(motor2_IN2, LOW);

digitalWrite(motor2_IN3, LOW);
digitalWrite(motor2_IN4, LOW);
/////////////////////////////////////////////

digitalWrite(motor3_IN1, LOW);
digitalWrite(motor3_IN2, LOW);

digitalWrite(motor3_IN3, LOW);
digitalWrite(motor3_IN4, LOW);

delay (500);
rueda_d_dere.write(45);//
rueda_d_izq.write(45);
rueda_t_dere.write(135);//
rueda_t_izq.write(135);
  }

/////////////////////////////////////////
//
//////////////////////////////////////
void fatras(void)
{
angulo_ini_d_dere=90;
angulo_ini_d_izq=90;
angulo_ini_t_dere=90;
angulo_ini_t_izq=90;


  rueda_d_dere.write(angulo_ini_d_dere);
  rueda_d_izq.write(angulo_ini_d_izq);
  rueda_t_dere.write(angulo_ini_t_dere);
  rueda_t_izq.write(angulo_ini_t_izq);
delay (200);

digitalWrite(motor1_IN1, HIGH);
digitalWrite(motor1_IN2, LOW);

digitalWrite(motor1_IN3, HIGH);
digitalWrite(motor1_IN4, LOW);
////////////////////////////////////////

digitalWrite(motor2_IN1, HIGH);
digitalWrite(motor2_IN2, LOW);

digitalWrite(motor2_IN3, HIGH);
digitalWrite(motor2_IN4, LOW);
/////////////////////////////////////////////

digitalWrite(motor3_IN1, HIGH);
digitalWrite(motor3_IN2, LOW);

digitalWrite(motor3_IN3, HIGH);
digitalWrite(motor3_IN4, LOW);

}
/////////////////////////////////////////
//
//////////////////////////////////////
void fizquierda(void)
{

digitalWrite(motor1_IN1, LOW);
digitalWrite(motor1_IN2, LOW);
digitalWrite(motor1_IN3, LOW);
digitalWrite(motor1_IN4, LOW);

digitalWrite(motor2_IN1, LOW);
digitalWrite(motor2_IN2, LOW);
digitalWrite(motor2_IN3, LOW);
digitalWrite(motor2_IN4, LOW);

digitalWrite(motor3_IN1, LOW);
digitalWrite(motor3_IN2, LOW);
digitalWrite(motor3_IN3, LOW);
digitalWrite(motor3_IN4, LOW);

delay (200);
//angulo_ini_d_dere=180;
//angulo_ini_d_izq=180;
  }
/////////////////////////////////////////
//
//////////////////////////////////////
void gderecha(void)
{ 
angulo_ini_d_dere=0;
angulo_ini_d_izq=0;
angulo_ini_t_dere=0;
angulo_ini_t_izq=0;


  rueda_d_dere.write(angulo_ini_d_dere);
  rueda_d_izq.write(angulo_ini_d_izq);
  rueda_t_dere.write(angulo_ini_t_dere);
  rueda_t_izq.write(angulo_ini_t_izq); 

delay (200);

digitalWrite(motor1_IN1, HIGH);
digitalWrite(motor1_IN2, LOW);

digitalWrite(motor1_IN3, HIGH);
digitalWrite(motor1_IN4, LOW);
////////////////////////////////////////

digitalWrite(motor2_IN1, HIGH);
digitalWrite(motor2_IN2, LOW);

digitalWrite(motor2_IN3, HIGH);
digitalWrite(motor2_IN4, LOW);
/////////////////////////////////////////////

digitalWrite(motor3_IN1, HIGH);
digitalWrite(motor3_IN2, LOW);

digitalWrite(motor3_IN3, HIGH);
digitalWrite(motor3_IN4, LOW);

  }
/////////////////////////////////////////
//
//////////////////////////////////////

void gizquierda(void)
{ 
angulo_ini_d_dere=180;
angulo_ini_d_izq=180;
angulo_ini_t_dere=180;
angulo_ini_t_izq=180;


  rueda_d_dere.write(angulo_ini_d_dere);
  rueda_d_izq.write(angulo_ini_d_izq);
  rueda_t_dere.write(angulo_ini_t_dere);
  rueda_t_izq.write(angulo_ini_t_izq); 

delay (200);

digitalWrite(motor1_IN1, HIGH);
digitalWrite(motor1_IN2, LOW);

digitalWrite(motor1_IN3, HIGH);
digitalWrite(motor1_IN4, LOW);
////////////////////////////////////////

digitalWrite(motor2_IN1, HIGH);
digitalWrite(motor2_IN2, LOW);

digitalWrite(motor2_IN3, HIGH);
digitalWrite(motor2_IN4, LOW);
/////////////////////////////////////////////

digitalWrite(motor3_IN1, HIGH);
digitalWrite(motor3_IN2, LOW);

digitalWrite(motor3_IN3, HIGH);
digitalWrite(motor3_IN4, LOW);

  }
/////////////////////////////////////////
//
//////////////////////////////////////
  
String Jx, Ax, Bx, Cx, Dx, Xx,  Yx;

/////////////

void espera_lora(void)
{
  // Verifica si hay datos disponibles para recibir
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // Recibir los datos y almacenarlos en una variable
    String receivedData = "";
    while (LoRa.available()) 
    {
      receivedData += (char)LoRa.read();
    }

    // Mostrar la cadena recibida
 
    
    Serial.println("Datos lora: " + receivedData);


  
 
  // Extraer los valores separados por comas
    int pos1 = receivedData.indexOf(',');
    int pos2 = receivedData.indexOf(',', pos1 + 1);
    int pos3 = receivedData.indexOf(',', pos2 + 1);
    int pos4 = receivedData.indexOf(',', pos3 + 1);
    int pos5 = receivedData.indexOf(',', pos4 + 1);
    int pos6 = receivedData.indexOf(',', pos5 + 1);
    

    Jx = receivedData.substring(0, pos1);
    Ax = receivedData.substring(pos1 + 1, pos2);
    Bx = receivedData.substring(pos2 + 1, pos3);
    Cx = receivedData.substring(pos3 + 1, pos4);
    Dx = receivedData.substring(pos4 + 1, pos5);
    Xx = receivedData.substring(pos5 + 1, pos6);
    Yx = receivedData.substring(pos6 +1); 

     // Mostrar resultados en el Serial Monitor
    
   Serial.println("Jx: " + Jx);
    Serial.println("Ax: " + Ax);
    Serial.println("Bx: " + Bx);
    Serial.println("Cx: " + Cx);
    Serial.println("Dx: " + Dx);
    Serial.println("Xx: " + Xx);
    Serial.println("Yx: " + Yx);

    compara_estados();


    
  if (!LoRa.begin(433E6)) 
      { // o 915E6, la frecuencia en MHz de tu módulo
    Serial.println("Starting LoRa failed!");
  }

  }

   
}
//////////////////////////////////////////
//
///////////////////////////////////////////


void compara_estados(void)
{// Parar
  if ((Xx == "X0") && (Yx == "Y0/")) {
    parar();
  }
  // Adelante
  else if ((Xx == "X0") && (Yx == "Y4/")) {
    fadelante();
  }
  // Derecha
  else if ((Xx == "X4") && (Yx == "Y0/")) {
    derecha();
  }
  // Atrás
  else if ((Xx == "X0") && (Yx == "Y2/")) {
    fatras();
  }
  // Izquierda
  else if ((Xx == "X2") && (Yx == "Y0/")) {
    izquierda();
  }
  // Giro derecha-adelante
  else if ((Xx == "X4") && (Yx == "Y4/")) {
    derecha();
    adelante();
  }
  // Giro izquierda-adelante
  else if ((Xx == "X2") && (Yx == "Y4/")) {
    izquierda();
    adelante();
  }
  // Giro derecha-atrás
  else if ((Xx == "X4") && (Yx == "Y2/")) {
    derecha();
    atras();
  }
  // Giro izquierda-atrás
  else if ((Xx == "X2") && (Yx == "Y2/")) {
    izquierda();
    atras();
  } 

  
}


  
///////////////////////////////
//
///////////////////////////////////

void prueba_servo (void)
{
while(1){

rueda_t_dere.write(90);
delay(6000);

rueda_t_dere.write(0);
delay(3000);
rueda_t_dere.write(180);
delay(3000);

  

  
}
}


int contador=0;

void loop() 
{

  //prueba_servo();
   contador++;
   if(contador==2500){ Serial3.write('e');contador=0;}
  espera_lora();
  delay(1);
  while (Serial3.available() > 0) 
  {
    char incomingChar = Serial3.read(); // Lee el carácter entrante

    // Si se detecta el inicio de la trama
    if (incomingChar == '*') 
    {
      receiving = true;      // Activa la recepción
      incomingData = "";     // Limpia el buffer
    }

    // Si estamos recibiendo datos, almacénalos
    if (receiving) 
    {
      incomingData += incomingChar;
      // Si se detecta el fin de la trama
      if (incomingChar == '/') 
      {
        receiving = false;    // Detiene la recepción
        pita();pita();pita();
        processFrame(incomingData.substring(1, incomingData.length() - 1)); 
        enviar_lora();
        incomingChar=0;
      }
    }
    
  }


  if (Serial.available() > 0) 
  {
    // Leemos el caracter del serial
    char receivedChar = Serial.read();

    // Comprobamos si el caracter es 'e'
    if (receivedChar == 'e') {
      // Hacemos titilar el LED
         Serial.write('o');
         Serial3.write('e');
receivedChar=0;
    }
  }

  
}


//////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

void adelante (void){
  
digitalWrite(motor1_IN1, LOW);
digitalWrite(motor1_IN2, HIGH);

digitalWrite(motor1_IN3, LOW);
digitalWrite(motor1_IN4, HIGH);
////////////////////////////////////////

digitalWrite(motor2_IN1, LOW);
digitalWrite(motor2_IN2, HIGH);

digitalWrite(motor2_IN3, LOW);
digitalWrite(motor2_IN4, HIGH);
/////////////////////////////////////////////

digitalWrite(motor3_IN1, LOW);
digitalWrite(motor3_IN2, HIGH);

digitalWrite(motor3_IN3, LOW);
digitalWrite(motor3_IN4, HIGH);
  

  return;
  }



//////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

void atras (void){
  
  
digitalWrite(motor1_IN2, LOW);
digitalWrite(motor1_IN1, HIGH);

digitalWrite(motor1_IN4, LOW);
digitalWrite(motor1_IN3, HIGH);
////////////////////////////////////////

digitalWrite(motor2_IN2, LOW);
digitalWrite(motor2_IN1, HIGH);

digitalWrite(motor2_IN4, LOW);
digitalWrite(motor2_IN3, HIGH);
/////////////////////////////////////////////

digitalWrite(motor3_IN2, LOW);
digitalWrite(motor3_IN1, HIGH);

digitalWrite(motor3_IN4, LOW);
digitalWrite(motor3_IN3, HIGH);
  

  return;
  }



//////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

void fderecha (void){
  
  
digitalWrite(motor1_IN1, HIGH);
digitalWrite(motor1_IN2, LOW);

digitalWrite(motor1_IN3, LOW);
digitalWrite(motor1_IN4, HIGH);
////////////////////////////////////////

digitalWrite(motor2_IN1, HIGH);
digitalWrite(motor2_IN2, LOW);

digitalWrite(motor2_IN3, LOW);
digitalWrite(motor2_IN4, HIGH);
/////////////////////////////////////////////

digitalWrite(motor3_IN1, HIGH);
digitalWrite(motor3_IN2, LOW);

digitalWrite(motor3_IN3, LOW);
digitalWrite(motor3_IN4, HIGH);
  

  return;
  }



//////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////





void giro_derecha(void)
{


  rueda_d_dere.write(angulo_giro_d1_dere);
  rueda_d_izq.write(angulo_giro_d2_dere);
  rueda_t_dere.write(angulo_ini_t_dere);
  rueda_t_izq.write(angulo_ini_t_izq);

}



void giro_izquierda(void)
{

  rueda_d_dere.write(angulo_giro_d1_izq);
  rueda_d_izq.write(angulo_giro_d2_izq);
  rueda_t_dere.write(angulo_ini_t_dere);
  rueda_t_izq.write(angulo_ini_t_izq);

}


  
void reinicio_lora(void){
  
  
  
  if (!LoRa.begin(433E6)) 
  { // o 915E6, dependiendo de la frecuencia de tu módulo
    Serial.println("Starting LoRa failed!");
    while (1);
  }
Serial.println("LORA REINICIADO");
  
  
  }


void depura(void)
{  Serial.println("funca");
  while(1){
   digitalWrite(A15, LOW);
   delay(300);
   
  digitalWrite(A15, HIGH);
   delay(300);
  }
  }

  
void pita() {
  // Ejemplo: alternar el estado del pin A15
  digitalWrite(A15, HIGH); // Enciende
  delay(50);             // Espera 1 segundo
  digitalWrite(A15, LOW);  // Apaga
  delay(50);             // Espera 1 segundo
}




// Función para procesar la trama
void processFrame(String frame) {
  Serial.println("Trama recibida: " + frame);

  // Dividir la trama en segmentos por comas
  int startIndex = 0;
  int commaIndex = frame.indexOf(',');
  int segmentCounter = 0;

  while (commaIndex != -1) {
    String dataSegment = frame.substring(startIndex, commaIndex);

    // Asignar los datos a variables según el segmento
    switch (segmentCounter) {
      case 0: temperatura = dataSegment.toFloat(); break;
      case 1: humedad = dataSegment.toFloat(); break;
      case 2: glp = dataSegment.toFloat(); break;
      case 3: distancia = dataSegment.toFloat(); break;
      case 4: coordenada1 = dataSegment; break;
      case 5: coordenada2 = dataSegment; break;
      default: break; // Ignorar segmentos adicionales
    }

    segmentCounter++;
    startIndex = commaIndex + 1;
    commaIndex = frame.indexOf(',', startIndex);
  }

  // Procesar el último segmento después de la última coma
  String lastSegment = frame.substring(startIndex); // Obtiene el último segmento
  switch (segmentCounter) {
    case 0: temperatura = lastSegment.toFloat(); break;
    case 1: humedad = lastSegment.toFloat(); break;
    case 2: glp = lastSegment.toFloat(); break;
    case 3: distancia = lastSegment.toFloat(); break;
    case 4: coordenada1 = lastSegment; break;
    case 5: coordenada2 = lastSegment; break;
    default: break; // Ignorar si hay más segmentos
  }

  // Mostrar los valores almacenados en las variables
  Serial.println("Temperatura: " + String(temperatura));
  Serial.println("Humedad: " + String(humedad));
  Serial.println("GLP: " + String(glp));
  Serial.println("Distancia: " + String(distancia));
  Serial.println("Coordenada 1: " + coordenada1);
  Serial.println("Coordenada 2: " + coordenada2);

}



  void enviar_lora(void)
  {


    String dataToSend = "*"+String(temperatura, 2) + "," + 
                      String(humedad, 2) + "," + 
                      String(glp, 2) + "," + 
                      String(distancia, 2) + "," + 
                      coordenada1 + "," + 
                      coordenada2 + "/";

//   Enviar la cadena por LoRa
  Serial.println("Transmitiendo:"+dataToSend);
  LoRa.beginPacket();
  LoRa.print(dataToSend);
  LoRa.endPacket();

  Serial.println("envie por lora");
    }
