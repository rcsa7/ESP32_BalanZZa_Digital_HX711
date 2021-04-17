#include "HX711.h"
#include "soc/rtc.h"// LIBRARY SENSO USADA PARA REDUZIR A VELOCIDADE DE CLOCK DO ESP32
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 #include <WiFi.h>
HX711 scale;
LiquidCrystal_I2C lcd(0x27,20,4);// SDA-D21----SCL-D22
const char WEBSITE[] = "api.pushingbox.com";    //pushingbox API server
const String devid1 = "v210947D67032C9B";       //ALARME GAS DA COZINHA ACABANDO

const char* ssid = "2.4GHz_Acher";
const char* password =  "cadeafaca999";
  int pinLedPesoAlto =18; //D18 gpio18 indicação de bujão cheio
  int pinLedPesoBaixo= 19; //D19  gpio19 alarme de bujão vazio
  
//////int sensor = 26;// 
void alarmegas();// alarme gas vazio
 
 
void setup() {
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);// REDUÇÃO DA VELOCIDADE DO CLOCK INTERNO DO ESP32 PARA PERMITIR A LEITURA DA CELULA DE CARGA
   lcd.init();
  //--- Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
 
      pinMode(pinLedPesoAlto,OUTPUT);
      pinMode(pinLedPesoBaixo,OUTPUT); 
 
  //---- parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT  - pin #A1
  // HX711.PD_SCK - pin #A0
  scale.begin(26, 25);// DT--GPIO26--// SCK--GPIO25
 
/*
  //---- Descomentar este trecho do codigo para calibrar a celula de carga
  //---- Com o valor obtido no monitor serial da tara da celula de carga, obter o fator para calibração.
  //-----usar a formula E=V/P
  //---V= VALOR OBTIDO NO MONITOR SERIAL,---VALOR OBTIDO FOI (70.979)
  //---P= PESO PADRÃO USADO PARA CALIBRAR A CELULA,---- FOI USADO UM PESO CONHECIDO DE (4,9Kg) 
  //-- E= ESCALA (FATOR)
  //--- COLOCAR ESTE VALOR NA LINHA-- scale.set_scale(-14.485)
  scale. set_scale();
  scale.tare();
  Serial.println("Put known weight on ");

  delay(2500);
  Serial.print(scale.get_units(10));
  Serial.print(" Divide this value to the weight and insert it in the scale.set_scale() statement");
 
  while(1==1);
*/
  

 

  scale.set_scale(-14.485);    // escreva aqui o valor obtido da tara calibração 
  scale.tare();               // reset the scale to 0

 
   
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) //not connected,..waiting to connect
    {
      delay(500);
      Serial.print(".");
    }
      Serial.println(WiFi.localIP());
      delay(500);
}
void printValues()
{
   Serial.println("Diska");
  Serial.println("Initializing the scale");
   Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());     // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  // by the SCALE parameter (not set yet)
 Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
 Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
  //--- by the SCALE parameter set with set_scale
   Serial.println("Readings:");
   Serial.println(scale.get_units(10), 1);
    Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 1);
   Serial.println("alarmegas");
}
void loop() {
   int sensorValue = analogRead(scale.get_units(10) );
  /// int sensorValue = analogRead(sensor );
     alarmegas();
     printValues();
    
   
  //  if (scale.get_units(10) >=11000.00)// peso do gas cheio
 ///////  if (sensorValue >=11000.00)// peso do gas cheio
  
 ///// {
 ///////  digitalWrite(pinLedPesoAlto,HIGH);// indicação do gas cheio
 
  //////// }
  //////// else
 ////////  {
  ///////////   digitalWrite(pinLedPesoAlto,LOW);
 //////////////   }
     if (scale.get_units(10) <=4000.00)// peso do gas vazio
/////   if (sensorValue <=4000.00)// peso do gas vazio
  {
   digitalWrite(pinLedPesoBaixo,HIGH); // indicação do gas vazio
   
   }
   else
   {
     digitalWrite(pinLedPesoBaixo,LOW);
    }
    
     
 
  
  
  int ww = (int)(scale.get_units(10) + 0.5);
  scale.power_down();             // put the ADC in sleep mode
  delay(100);
  scale.power_up();
 // alarmegas();// alarme gas vazio
  lcd.setCursor(0,0);
  lcd.print("BalanZZa DIGITAL");
  lcd.setCursor(0,1); 
  lcd.print(scale.get_units(10), 1);
  lcd.setCursor(14,1);
  lcd.print(" kg");
 
}
void alarmegas(){
  WiFiClient client;  //------Instantiate WiFi object

    //-------Start or API service using our WiFi Client through PushingBox
    if (client.connect(WEBSITE, 80))
      { 
         client.print("GET /pushingbox?devid=" + devid1
         );

      client.println(" HTTP/1.1"); 
      client.print("Host: ");
      client.println(WEBSITE);
      client.println("User-Agent: ESP8266/1.0");
      client.println("Connection: close");
      client.println();
      }
       
}
