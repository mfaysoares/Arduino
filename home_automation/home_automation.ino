#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Ultrasonic.h>
#include <SoftwareSerial.h>

/*Pinos*/
#define trig_pin 11
#define echo_pin 12
#define DHTPIN 8
#define DHTTYPE DHT22

// Inicializa o display no endereco 0x3F
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

/*Inicialização dos sensores*/
Ultrasonic ultrassom(trig_pin, echo_pin);
DHT rht03(DHTPIN, DHTTYPE);
SoftwareSerial bluetooth(0, 1);

//Símbolo º
byte grau[8] = { B00001100,
                 B00010010,
                 B00010010,
                 B00001100,
                 B00000000,
                 B00000000,
                 B00000000,
                 B00000000,
               };

void setup()
{
  pinMode(13, OUTPUT); // Define pino como saída
  digitalWrite(13, LOW); // Seta pino com nível baixo
  lcd.begin (20, 4);
  lcd.clear();
  lcd.createChar(0, grau);
  rht03.begin();
}

void loop()
{
  long time;
  float temperatura, umidade, distancia;
  float temp1, temp2, temp3, umid1, umid2, umid3;
  char data1[50], data2[50], data3[50];
  String data_bluetooth = "";
  String sensor = "SENSOR";
  String split = "*";

  /*Sensor de Ultrassom*/
  time = ultrassom.timing();
  distancia = ultrassom.convert(time, Ultrasonic::CM);

  /*Sensor de Temperatura e Umidade*/
  temperatura = rht03.readTemperature();
  umidade = rht03.readHumidity();

  /*Converte de float para String*/
  dtostrf(temperatura, 6, 2, data1);
  dtostrf(umidade, 6, 2, data2);
  dtostrf(distancia, 6, 2, data3);
  data_bluetooth = sensor + split + data1[1] + split + data1[2] + split + data1[4] + split + data2[1] + split + data2[2] + split + data2[4] + split + data3;

  /*Envio dos dados via bluetooth*/
  /*bluetooth.print(data_bluetooth);
    delay(500);*/
  lcd.setCursor(0, 0);
  lcd.print("T2-DISP. MOVEIS");
  lcd.setCursor(0, 1);
  lcd.print("Distancia: ");
  lcd.print(distancia);
  lcd.setCursor(16, 1);
  lcd.print("cm");
  lcd.setCursor(0,2);
  lcd.print("Temperatura: ");
  lcd.print(temperatura);
  lcd.setCursor(18,2);
  lcd.write((byte)0);
  lcd.print("C");
  lcd.setCursor(0,3);
  lcd.print("Umidade: ");
  lcd.print(umidade);
  lcd.setCursor(14,3);
  lcd.print("%ur");
  delay(1000);
}
