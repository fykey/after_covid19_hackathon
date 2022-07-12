#include <ESP8266WiFi.h>
// https://github.com/bogde/HX711
#include "HX711.h"

const int DT_PIN = 0;
const int SCK_PIN = 2;
int count = 0;
int sit_judge = 15;
int overwork_judge = 25;
int sit_end;
String send_data;
float weight = 50.0;//重くなったらintに変えて

const char* ssid = "ssid";
const char* password = "password";


const char* host1 = "https://after-covid-hackathon-api.herokuapp.com/SitDown";//後で入力！！！！！！！

const char* host2 = "https://after-covid-hackathon-api.herokuapp.com/StandUp";


HX711 scale;

void setup() {
  Serial.begin(9600);
//wifi start
Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 

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
  if (count == 0){
    
  }
   
   if ((sit_end - count) >=sit_judge){//二重かっこがどうなるのかわからない
    
    Serial.print("席を立ったためデータを送信します！！");//このタイミングでpost送信
    send_data = String((sit_end - sit_judge)*2)+"秒";
    sit_end = 0;

    WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host2, httpPort)) {
    Serial.println("connection failed");
    return;
  }

   Serial.print("Requesting POST: ");
   // Send request to the server:
   client.println(String("POST") / "HTTP/1.1");
   client.println("Host": host2);
   client.println("Accept: */*");
   client.println("Content-Type: https://after-covid-hackathon-api.herokuapp.com/");
   client.print("Content-Length: ");
   client.println(send_data.length());
   client.println();
   client.print(send_data);

   delay(500); // Can be changed
  if (client.connected()) { 
    client.stop();  // DISCONNECT FROM THE SERVER
  }
  Serial.println();
  Serial.println("closing connection");
  delay(5000);

    
   }
    delay(500);
}
