#include <ESP8266WiFi.h>
#include "HX711.h"

const int DT_PIN = 0;
const int SCK_PIN = 2;
const char* ssid = "ssidを入力しよう";
const char* password = "passwordを入力しよう";

boolean Ini_html_on;

WiFiServer server(80);
WiFiClient client;

HX711 scale;

void setup() {
 
  Serial.begin(9600);
  // Connect to WiFi network
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
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.println(WiFi.localIP());

   //HX711 setup start
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
//HX711 Setting end

}


void loop() {
  
  Serial.println(scale.get_units(10), 1);//たんま

  scale.power_down();
  delay(500);
  scale.power_up();

  if(Ini_html_on == false){
      Ini_HTTP_Response(scale.get_units(10));
  }else if(client.available()){
    Serial.print(client.read());
  }
  delay(1);//これは重要かも。
}


float Ini_HTTP_Response(float data1)
{
  
  client = server.available();//クライアント生成
  delay(1);
  String req;
 
  while(client){
    if(client.available()){
      req = client.readStringUntil('\n');
      Serial.println(req);
      if (req.indexOf("GET / HTTP") >= 0 || req.indexOf("GET /favicon") >= 0){//ブラウザからリクエストを受信したらこの文字列を検知する
        //Google Chromeの場合faviconリクエストが来るのでそれも検出する
        Serial.println("-----from Browser FirstTime HTTP Request---------");
        Serial.println(req);
        //ブラウザからのリクエストで空行（\r\nが先頭になる）まで読み込む
        while(req.indexOf("\r") != 0){
          req = client.readStringUntil('\n');//\nまで読み込むが\n自身は文字列に含まれず、捨てられる
          Serial.println(req);
        }
        req = "";
        delay(10);//10ms待ってレスポンスをブラウザに送信
 
        //メモリ節約のため、Fマクロで文字列を囲う
        //普通のHTTPレスポンスヘッダ
        client.print(F("HTTP/1.1 200 OK\r\n"));
        client.print(F("Content-Type:text/html\r\n"));
        client.print(F("Connection:close\r\n\r\n"));//１行空行が必要
        //ここからブラウザ表示のためのHTML吐き出し
        client.println(F("<!DOCTYPE html>"));
        client.println(F("<html>"));
        client.println(F("<body>"));
        client.println(F("<font size=30>"));
        client.println(data1);
        client.println(F("</font>"));
        client.println(F("</body>"));
        client.println(F("</html>"));
 
        delay(10);//これが重要！これが無いと切断できないかもしれない。
        client.stop();//一旦ブラウザとコネクション切断する。
        delay(10);
        Serial.println("\nGET HTTP client stop--------------------");
        req = "";
        Ini_html_on = false;  //一回切りの接続にしたい場合、ここをtrueにする。                
      }
    }
  }
}
