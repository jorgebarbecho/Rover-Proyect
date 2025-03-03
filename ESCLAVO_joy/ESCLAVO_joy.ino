#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <LoRa.h>


#define PULSADOR_1 3
#define PULSADOR_2 4
#define PULSADOR_3 5
#define PULSADOR_4 6
#define PULSADOR_5 7


#define BATERIA A3
#define BUZZER 8


int menuActual = 0;  // Variable para almacenar el menú actual
bool enMenu = true; // Variable para saber si estamos en el menú
bool pantallaActualizada = false; // Controla si la pantalla necesita actualización
bool lcdEncendido = false; 
unsigned long tiempoEncendido = 0;  // Variable para almacenar el tiempo en que se encendió la pantalla
const unsigned long tiempoEncendidoMaximo = 15000;  // Tiempo que la pantalla permanecerá encendida (10 segundos)
bool buzzerActivado = false;



double latitud_min=0.0000000;
double longitud_min=0.0000000;
int lat_grados= 0;
int lng_grados= 0;
int lat_minutos=0;
int lat_segundos=0;
int lng_minutos=0;
int lng_segundos=0;    int llego=0;
char l_lat='x';
char l_lon='X';
float temperature = 0.000;
float humidity = 0.000;
float glp = 0.000;
float distancia = 0.000;
String gpsLocation = "No data";
String receivedText = "";

const int JOY_X_PIN = A2;  // Eje X
const int JOY_Y_PIN = A1;// Eje Y


String estado_jx="J1";

String estado_Ax="A0";
String estado_Bx="B0";
String estado_Cx="C0";
String estado_Dx="D0";
String estado_Xx = "X0";  // eje X
String estado_Yx = "Y0"; //eje Y
String estado_Mx = "M0";  // Centro

int timmer=0;    
String variables[6];  // Array para almacenar las variables
char actualiza=0;

// Dirección I2C de tu LCD, puede ser 0x27 o 0x3F dependiendo de tu modelo
LiquidCrystal_I2C lcd(0x27, 20, 4);  // (dirección I2C, columnas, filas)


void setup() {
  // Inicializamos el LCD
  lcd.begin(20, 4); // El LCD tiene 20 columnas y 4 filas

  Serial.begin(9600);

  
  // Muestra más texto en las 4 líneas
   lcd.setCursor(5, 0);
   lcd.print("MECATRONICA");
   lcd.setCursor(2, 2);
   lcd.print("#DEL COLE AL TEC");// Texto en la primera fila
 lcd.backlight();


  if (!LoRa.begin(433E6)) { // o 915E6, la frecuencia en MHz de tu módulo
    Serial.println("Starting LoRa failed!");

  }


 
    
    pinMode(BATERIA, INPUT);
   

    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
    pinMode(PULSADOR_1, INPUT_PULLUP); // Habilita resistencia interna de pull-up
    pinMode(PULSADOR_2, INPUT_PULLUP);
    pinMode(PULSADOR_3, INPUT_PULLUP);
    pinMode(PULSADOR_4, INPUT_PULLUP);
    pinMode(PULSADOR_5, INPUT_PULLUP);

}

char escoge=1;
char escoge_ant=0;
char bandera=0;

/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////  
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



    // Encuentra las posiciones de los caracteres '*' y '/'
    int startPos = receivedData.indexOf('*') + 1;  // Posición después del '*'
    int endPos = receivedData.indexOf('/');        // Posición del '/'

// Extrae la subcadena entre '*' y '/'
String extracted = receivedData.substring(startPos, endPos);
//Serial.println(extracted);

int index = 0;

 // Separar el string por comas
    startPos = 0;
   for (int i = 0; i < extracted.length(); i++) 
   {
    if (extracted.charAt(i) == ',') 
    {
      variables[index++] = extracted.substring(startPos, i);
      startPos = i + 1;
    }
  }
  // Agregar la última variable después de la última coma
  variables[index] =extracted.substring(startPos);

    float temperatura = 0.0;
    float humedad = 0.0;
    float glp = 0.0;
    float distancia = 0.0;
    String coordenada1 = "";
    String coordenada2 = "";
 
    // Mostrar los valores extraídos
    Serial.println("Temperatura: " + variables[0]);
    Serial.println("Humedad: " + variables[1]);
    Serial.println("GLP: " + variables[2]);
    Serial.println("Distancia: " + variables[3]);
    Serial.println("Coordenada 1: " + variables[4]);
    Serial.println("Coordenada 2: " + variables[5]);

    pantallaActualizada = false;
    actualiza=1;

 
      if (!LoRa.begin(433E6)) { // o 915E6, la frecuencia en MHz de tu módulo
    Serial.println("Starting LoRa failed!");
    //while (1);

    
       }

  }

   

}


/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////

void enviar_lora(void)
{
    String dataToSend = "*"+estado_jx + "," + 
                      estado_Ax + "," + 
                      estado_Bx + "," + 
                      estado_Cx + "," + 
                      estado_Dx + "," +
                      estado_Xx + "," +
                      estado_Yx + "/" ;
                    

  // Enviar la cadena por LoRa
  Serial.println("Transmitiendo:"+dataToSend);
  LoRa.beginPacket();
  LoRa.print(dataToSend);
  LoRa.endPacket();
 Serial.println("envie por lora");
  return;
}


int envio=0;
/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////


void muestra_todo(void){

        
  
        lcd.clear();

          
          if(escoge==1){
                       lcd.setCursor(0, 0);
            lcd.print("TEMPERATURA: " + variables[0]);
          }
           if(escoge==2){ 
            lcd.setCursor(0, 0);
            lcd.print("GLP: " + variables[2]);
}

     if(escoge==3){
            lcd.setCursor(0, 0);
            String distanciaStr = variables[3];
            float distan= distanciaStr.toFloat();
            distan=-0.1031*distan+49.7952;
            lcd.print("DISTANCIA: " + variables[3]);
            lcd.setCursor(1, 1);
             lcd.print("DISTANCIA: ");
            lcd.print(distan, 2);
            lcd.print("cm.");           
     }


      if(escoge==4){
  lcd.setCursor(0, 0);
            lcd.print("GPS1: " + variables[4]);
            lcd.setCursor(0, 1);
            lcd.print("GPS2: " + variables[5]);  
                   
      }

  
  
}


void escoge_menu(void)
{
  
        // Mostrar el menú
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Seleccione Menu:");
         lcd.setCursor(0, 1);
        switch (escoge) 
        {
          case 1:
          lcd.setCursor(4, 2);
          lcd.print("TEMPERATURA");
            break;
          case 2:
          lcd.setCursor(8, 2);
            lcd.print("GLP");
            break;
          case 3:
          lcd.setCursor(5, 2);
            lcd.print("DISTANCIA");
            break;
          case 4:
          lcd.setCursor(8, 2);
            lcd.print("GPS");    
             Serial.println("----------------------------------");

            break;
        }
  }


void loop() 
{

   // Leer estado de los pulsadores
    int estado1 = digitalRead(PULSADOR_1);
    int estado2 = digitalRead(PULSADOR_2);
    int estado3 = digitalRead(PULSADOR_3);
    int estado4 = digitalRead(PULSADOR_4);
    int estado5 = digitalRead(PULSADOR_5);
    int estado6 = analogRead(JOY_X_PIN);
    int estado7 = analogRead(JOY_Y_PIN);

  ////////////////////////////
  //Leer valores de pulsantes y joystic
  ///////////////////////////////////  

             
if((estado2==LOW)&&(bandera==0))
{
  
  escoge++;
  if(escoge==5){escoge=1;}
  escoge_menu();
  delay(500);
              
}



if((estado4==LOW)&&(bandera==0))
{
  
  escoge--;
  if(escoge==0){escoge=4;}
  escoge_menu();
  delay(500);
              
}


   
 
if((estado3==LOW))
{
 bandera=1;muestra_todo();
  delay(500);
  delay(500);
   
}


if((estado1==LOW))
{
  bandera=0;escoge_menu();
  delay(500);
  delay(500);
   
}


delay(1);
if((actualiza==1)&&(bandera==1)){actualiza=0;muestra_todo();}

  
// Clasificación para el eje Y con 5 rangos
  if (estado7 >= 0 && estado7 <= 5 ) 
  {                // 0 a 5
    estado_Yx = "Y4";
    lcdEncendido=true;
    tiempoEncendido = millis();;
  } 
  
  else if (estado7 <= 540) 
  {        
    estado_Yx = "Y0";
  } 
 
  else if (estado7 >= 1021 && estado7 <= 1023 )
  {                         
    estado_Yx = "Y2";
    lcdEncendido=true;
     tiempoEncendido = millis();
  }


  // Clasificación para el eje X con 5 rangos
  if (estado6 >= 0 && estado6 <= 5 ) 
  {                // 0 a 5
    estado_Xx = "X4";
    lcdEncendido=true;
    tiempoEncendido = millis();
  }
 
  else if (estado6 <= 540) 
  {        
    estado_Xx = "X0";
  }

  else if (estado6 >= 1021 && estado6 <= 1023 )
  {                         
    estado_Xx = "X2";
    lcdEncendido=true;
     tiempoEncendido = millis();
  }
    

espera_lora();
timmer++;
char buffer[16];
if(timmer==500)
{
  enviar_lora();
  timmer=0;
  envio++;


}

  }
