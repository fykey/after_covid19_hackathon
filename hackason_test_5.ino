#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
// https://github.com/bogde/HX711
#include "HX711.h"

const int DT_PIN = 0;
const int SCK_PIN = 2;
int count = 0;
int sit_judge = 5;
int overwork_judge = 25;
int sit_end;
String send_data_API;
float weight = 50.0;//重くなったらintに変えて

const char* ssid = "i phone XXX";
const char* password = "abcdefs07";

HX711 scale;

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
   Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 

  Serial.println("connected!");
}

void disconnectWifi() {
  WiFi.disconnect();
  Serial.println("disconnected!");
}


String getPageSource2(char host[],char root[]) {
  WiFiClient client;

  if ( !client.connect(host, 80) ) {
    return String("");
  }

  client.print(String("GET ") + root +
               " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  client.println();

  delay(1000);

  String body = "";

  while (client.available()) {
    body += client.readStringUntil('\r');
  }

  return body;
}

void setup() {
  Serial.begin(9600);
//wifi start
Serial.println();
   connectWifi();

//wifi end
  
  Serial.println("start");
  scale.begin(DT_PIN, SCK_PIN);

  Serial.print("read:");
  Serial.println(scale.read());

  scale.set_scale();
  scale.tare();

  Serial.print("calibrating...");
  delay(5000);
  Serial.println(scale.get_units(10));

  scale.set_scale(-1536.00);
  scale.tare();

  Serial.print("read (calibrated):");
  Serial.println(scale.get_units(10));

  pinMode(12, OUTPUT);
  
}
void loop() {
  float data = scale.get_units(10);//ロードセルの値を変数dataに移す
  Serial.print(data, 1);//後でprint→printlnに直しておく

  scale.power_down();
  scale.power_up();
//着席確認プログラムstart dataが50以上ならbの値を追加して，下回ったら0とする．
  if (data > weight){
    count += 1;
    
  }else if (data < weight){
    count = 0;
  }
  Serial.print("\t");
  Serial.println(count);
  if (count == sit_judge){
    Serial.println("着席しました");//postリクエスト
    String result2 = getPageSource2("after-covid-hackathon-api.herokuapp.com","/test");
    Serial.println(result2);
  }
  
  
  if (count > sit_judge){//着席スタート合図＊＊ここから，postのプログラム書けばいいのでは
    Serial.println("着席中です");
    sit_end = count;
  }
  if (count > overwork_judge){
    Serial.println("座りすぎです");//バイブレーション
    digitalWrite(12, HIGH);
    delay(500);
    digitalWrite(12, LOW);
  }

   
   if ((sit_end - count) >=sit_judge){
    



    
    Serial.print("席を立ったためデータを送信します！！");//このタイミングでpost送信
    //send_data = String(data);
    sit_end = 0;

    String result2 = getPageSource2("after-covid-hackathon-api.herokuapp.com","/");
    Serial.println(result2);
   }
   
    delay(500);
}
