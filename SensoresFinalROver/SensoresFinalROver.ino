// Compilado captura datos de sensores

#include <SoftwareSerial.h>
#include <TinyGPS.h>


// Configura los pines de conexión del GPS
const int RXPin = 3, TXPin = 4;

TinyGPS gps;
SoftwareSerial ss(4, 3);

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);


const uint32_t GPSBaud = 9600;
const int paso= 8;
double latitud_min=0.0000000;
double longitud_min=0.0000000;
 int lat_grados= 0;
      int lng_grados= 0;
    int lat_minutos=0;
    int lat_segundos=0;
    int lng_minutos=0;
     int lng_segundos=0;
     int llego=0;
// Inicializa las bibliotecas de SoftwareSerial y TinyGPS++
//SoftwareSerial ss(RXPin, TXPin);
//TinyGPSPlus gps;




#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 9         // Pin connected to the DHT sensor
#define DHTTYPE DHT11    // DHT 11

// MQ-2 sensor
const int MQ_PIN = A1;    // MQ sensor pin
const int RL_VALUE = 5;    // RL resistor in kilo ohms
const int R0 = 10;         // R0 resistance in kilo ohms
const int READ_SAMPLE_INTERVAL = 100;  // Time between samples
const int READ_SAMPLE_TIMES = 5;      // Number of samples

// Sharp 2Y0A21 sensor
const int sensorPin = A0; // Pin for Sharp sensor

// DHT sensor initialization
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
const float X0 = 200, Y0 = 1.7, X1 = 10000, Y1 = 0.28;
const float punto0[] = { log10(X0), log10(Y0) };
const float punto1[] = { log10(X1), log10(Y1) };
const float scope = (punto1[1] - punto0[1]) / (punto1[0] - punto0[0]);
const float coord = punto0[1] - punto0[0] * scope;


///////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  // Corrected sensor initialization
  sensor_t sensor;
  dht.temperature().getSensor(&sensor); // Pass the pointer to sensor_t
  delayMS = sensor.min_delay / 1000;

  // Inicia la comunicación serial con el GPS
  ss.begin(GPSBaud);
  Serial.println(F("Iniciando..."));
  pinMode(paso, OUTPUT);
  digitalWrite(paso, HIGH);

}


void loop() {

  
  // Verificamos si hay datos disponibles en el buffer serial
  if (Serial.available() > 0) {
    // Leemos el caracter del serial
    char receivedChar = Serial.read();

    // Comprobamos si el caracter es 'e'
    if (receivedChar == 'e') {
      // Hacemos titilar el LED
         envia_datos();
    }
  }





}

////////////////////////////////////////////////////////////////////////////////////////
void actu_gps2(void)
{
  
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
 
  gps.f_get_position(&flat, &flon, &age);
 /*
  Serial.print(flat);
  Serial.print(',');
  Serial.print(flon);
  Serial.print(',');

  Serial.println();
*/

   
      lat_grados= abs((int)flat);
      lng_grados= abs((int)flon);
      Serial.print(lat_grados);Serial.print("°");

      float lat_minutos_c=((abs(flat)-(float)lat_grados)*60);
      lat_minutos=lat_minutos_c;
      
      Serial.print((int)lat_minutos);Serial.print("'");
      
      float lat_segundos_c=((lat_minutos_c-(float)lat_minutos)*60);
      lat_segundos=lat_segundos_c;
      Serial.print(lat_segundos);Serial.print('"');

      if(flat<0){Serial.print("S");}else{Serial.print("N");}
 Serial.print(",");
   Serial.print(lng_grados);Serial.print("°");

      float lng_minutos_c=((abs(flon)-(float)lng_grados)*60);
      lng_minutos=lng_minutos_c;
      
      Serial.print((int)lng_minutos);Serial.print("'");
      
      float lng_segundos_c=((lng_minutos_c-(float)lng_minutos)*60);
      lng_segundos=lng_segundos_c;
      Serial.print(lng_segundos);Serial.print('"');

      if(flon<0){Serial.print("W");}else{Serial.print("O");}
     Serial.println("/");
      
  smartdelay(1000);
  
  
  
  }



static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}





//////////////////////////////////////////////////////////////////////////
void envia_datos(void) 
{

  // DHT readings
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) 
  {
    Serial.print("*");
    Serial.print(event.temperature);
    Serial.print(",");
  } else {
    Serial.print(",0");
  }

  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) 
  {
    Serial.print(event.relative_humidity);
    Serial.print(",");
  } else {
    Serial.print("0,");
  }

  // MQ-2 sensor readings
  float rs_med = analogRead(MQ_PIN);
  float concentration = analogRead(MQ_PIN);

  Serial.print(concentration);

  // Sharp 2Y0A21 sensor readings
  float distance = analogRead(sensorPin);
  Serial.print(",");
  Serial.print(distance, 2);
  Serial.print(",");

  actu_gps2();

  delay(delayMS);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////


void actu_gps(void)

{
  
  
  }





float readMQ(int mq_pin) {
  float rs = 0;
  for (int i = 0; i < READ_SAMPLE_TIMES; i++) {
    rs += getMQResistance(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
  return rs / READ_SAMPLE_TIMES;
}

float getMQResistance(int raw_adc) {
  return (RL_VALUE / 1000.0 * (1023 - raw_adc) / raw_adc);
}

float getConcentration(float rs_ro_ratio) {
  return pow(10, coord + scope * log(rs_ro_ratio));
}
