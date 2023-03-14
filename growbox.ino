
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "DHTesp.h" 


#ifdef ESP32

#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

DHTesp dht;


ESP8266WebServer    server(80);





struct settings {
  char svetvkl[30];
  char svetvikl[30];
  char tempmin[30];
  char tempmax[30];
  char vlajmin[30];
  char vlajmax[30];
  char token3[30];
  char times[30];
  char avaria[30];
  char sava[30];
} user_wifi = {};

int kan0 = D0;//датчика огня все реле откл.fire ogon   s ognem  = 0
int kan1 = D1;//temp
int kan2 = D2;
int kan3 = D3;////датчик света  svet    vtemnote = 1
int kan4 = D4;//датчик света
int kan5 = D5;//Реле для лампы //датчик огня //fire ogon   s ognem  = 0
int kan6 = D6;//винтилятор d1
int kan7 = D7;//Реле для огня
int kan8 = D8;//z---c


int sig0;// nachalo svet
int sig1;//svetvkl 
int sig2;//svetvikl


int tmr;
int tmax;
int tmin;
int treal;

int z1;

void setup() {


Serial.begin(9600);
Serial.println("startt");
delay(1000);

 Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
 dht.setup(D1, DHTesp::DHT22);
 

  
  pinMode(kan0, INPUT);
  pinMode(kan1, INPUT_PULLUP);
  pinMode(kan2, INPUT);
  pinMode(kan3, INPUT);
  pinMode(kan4, OUTPUT);
  pinMode(kan5, OUTPUT);
  pinMode(kan6, OUTPUT);
  pinMode(kan7, OUTPUT);
  pinMode(kan8, OUTPUT);


 
  
  EEPROM.begin(sizeof(struct settings) );
  EEPROM.get( 0, user_wifi );

  WiFi.mode(WIFI_STA);
 WiFi.begin("yourSSID", "yourPASS");

  delay(1000);
 server.on("/",  handlePortal);
 server.begin();
  
  sig1=2;
  sig2=3;
  sig0=0;
  //tmax=26;



  z1=(String(user_wifi.sava)).toInt();


}

void loop() {
delay(1000);


tmax=(String(user_wifi.tempmax)).toInt();
Serial.println(tmax);
sig1=(String(user_wifi.svetvkl)).toInt()*60;
sig2=(String(user_wifi.svetvikl)).toInt()*60;

int tmp=millis()/1000;
if(tmp%2==0){


  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
  treal=20;
if (treal>tmax){
 
  digitalWrite(kan7, HIGH);
  
}else{

   digitalWrite(kan7, LOW);
}

}



  
 
 Serial.println(digitalRead(kan0));//fire ogon   s ognem  = 0
if (digitalRead(kan0)==0){
 digitalWrite(kan7, LOW);
}else{
 digitalWrite(kan7, HIGH);
}


  Serial.println(digitalRead(kan3));//svet    v temnote = 1
if (digitalRead(kan3)==1){
   digitalWrite(kan4, LOW);
}else{
  digitalWrite(kan4, HIGH);
}

//  Serial.println(digitalRead(kan5));// s vodoy =0 bez vodi =1
if (digitalRead(kan5)==0){
 digitalWrite(kan1, LOW);
}else{
 digitalWrite(kan1, HIGH);
}

  
tmr=millis()/1000;

Serial.println(sig0+z1);
  Serial.println(sig0);
  Serial.println(sig1);
  Serial.println(sig2);

  if(sig1>sig0+z1){
    digitalWrite(kan2, HIGH);
    Serial.println("on");
  }else if(sig1<sig0+z1<sig1+sig2){
    digitalWrite(kan2, LOW);
    //sig0=tmr%(sig1+sig2);
    Serial.println("off");
    
    //Serial.println(sig0);
  }
int z1=(String(user_wifi.sava)).toInt();
sig0=tmr%(sig1+sig2);
if (sig0%10==0 & sig0!=0){
  strncpy(user_wifi.sava, String(sig0).c_str(), sizeof(user_wifi.sava) );
  EEPROM.put(0, user_wifi);
  EEPROM.commit();
}

 
  server.handleClient();
 




Serial.println(WiFi.localIP());



  }
   
void handlePortal() {

  if (server.method() == HTTP_POST) {
    if (server.arg("tochka") != 0){
    strncpy(user_wifi.tempmax,     server.arg("tochka").c_str(),     sizeof(user_wifi.tempmax) );
    }
    if (server.arg("tochkapass") != 0){
    strncpy(user_wifi.tempmin,     server.arg("tochkapass").c_str(),     sizeof(user_wifi.tempmin) );
    }
     if (server.arg("token1") != 0){
    strncpy(user_wifi.vlajmin,     server.arg("token1").c_str(),     sizeof(user_wifi.vlajmin) );
     }
     if (server.arg("token2") != 0){
    strncpy(user_wifi.vlajmax,     server.arg("token2").c_str(),     sizeof(user_wifi.vlajmax) );
     }
     if (server.arg("token3") != 0){
    strncpy(user_wifi.token3,     server.arg("token3").c_str(),     sizeof(user_wifi.token3) );
     }
     if (server.arg("token4") != 0){
    strncpy(user_wifi.sava,     server.arg("token4").c_str(),     sizeof(user_wifi.sava) );
     }
     if (server.arg("token5") != 0){
    strncpy(user_wifi.avaria,     server.arg("token5").c_str(),     sizeof(user_wifi.avaria) );
     }
     if (server.arg("ssid") != 0){
    strncpy(user_wifi.svetvkl,     server.arg("ssid").c_str(),     sizeof(user_wifi.svetvkl) );
    
    strncpy(user_wifi.sava, String(sig0).c_str(), sizeof(user_wifi.sava) );
     }
     if (server.arg("password") != 0){
    strncpy(user_wifi.svetvikl, server.arg("password").c_str(), sizeof(user_wifi.svetvikl) );
        
    strncpy(user_wifi.sava, String(sig0).c_str(), sizeof(user_wifi.sava) );
     }
  
    EEPROM.put(0, user_wifi);
    EEPROM.commit();

    server.send(200,   "text/html", "<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>GROW BOX</title><style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}</style> </head> <body><main class='form-signin'> <h1>GROW BOX</h1> <br/> <p>Your settings have been saved successfully!<br />Please restart the device.</p></main></body></html>" );
  } else {

    server.send(200,   "text/html", "<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>GROW BOX</title> <style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1{text-align: center}</style> </head> <body><main class='form-signin'> <form action='/' method='post'> <h1 class=''> GROW BOX<br>AUTOMATIK </h1><br/><div class='form-floating'><label>Light minute</label><input type='text' class='form-control' name='ssid'> </div><div class='form-floating'><br/><label>Dark minute</label><input type='text' class='form-control' name='password'></div><br/><br/><div class='form-floating'><label>Temperature max</label><input type='tel' class='form-control' name='tochka'><div class='form-floating'><div class='form-floating'><label>Start in hours</label><input type='tel' class='form-control' name='token4'><div class='form-floating'><button type='submit'>Save</button></p></form></main> </body></html>" );
  }
  
}
