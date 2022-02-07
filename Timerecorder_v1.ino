#include <M5Stack.h>
#include <WiFi.h>
#include <driver/adc.h>
#define TFT_GREY 0x5AEB
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "TLAB_Wi-Fi";
const char *password = "#u,3jcdXJ~SS";
const char* published_url = "https://script.google.com/macros/s/AKfycbxhIoNO-uY6tIQ5g6g5phrAlaQ8q6kMyr17IKkWw5G7ofbxxXWV/exec";
uint8_t hh;
uint8_t mm;
uint8_t ss;
byte omm = 60;
byte oss = 60;
byte xcolon;
byte xsecs;
struct tm timeInfo;
int cnt=0;
String name[] = {"Boss","Asai","Saito","Takahashi","Fujimoto","Ikuta","Ota","Fukuda","Katsura","Shimomura","Higashiguchi","Guest"};
int num=0;
char s[20];

void setup(void) {
  adc_power_acquire(); // ADC Power ON
  M5.begin();
  while (!Serial) continue;
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setBrightness(10);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.print(name[0]);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_RED);
  M5.Lcd.setCursor(20, 220);
  M5.Lcd.print("Attend");
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
  M5.Lcd.setCursor(130, 220);
  M5.Lcd.print("Leave");
  M5.Lcd.setTextColor(TFT_WHITE, TFT_GREEN);
  M5.Lcd.setCursor(230, 220);
  M5.Lcd.print("Name");
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Speaker.setVolume(1);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if(WiFi.begin(ssid, password) != WL_DISCONNECTED) {
    ESP.restart();
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  configTime(9*3600L,0,"ntp.nict.jp","time.google.com","ntp.jst.mfeed.ad.jp");
}

void loop() {
  getLocalTime(&timeInfo);
  hh = timeInfo.tm_hour;
  mm = timeInfo.tm_min;
  ss = timeInfo.tm_sec;
  StaticJsonDocument<500> doc;
  char pubMessage[256];
  if (oss != ss) {
    int xpos = 0;
    int ypos = 85;
    int ysecs = ypos + 24;
    if (omm != mm) {
      omm = mm;
      if (hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8);
      xpos += M5.Lcd.drawNumber(hh, xpos, ypos, 8);
      xcolon = xpos;
      xpos += M5.Lcd.drawChar(':', xpos, ypos - 8, 8);
      if (mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8);
      xpos += M5.Lcd.drawNumber(mm, xpos, ypos, 8);
      xsecs = xpos;
    }
    if (oss != ss) {
      oss = ss;
      xpos = xsecs;
      M5.Lcd.drawChar(':', xcolon, ypos - 8, 8);
      xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6);
      if (ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ysecs, 6);
      M5.Lcd.drawNumber(ss, xpos, ysecs, 6);
    }
  }
    // Aボタンが押されたら
  if(M5.BtnA.wasPressed())
  {
    //M5.Speaker.tone(659, 200);
    //delay(200);
    //M5.Speaker.tone(523, 200);
    //delay(200);
    M5.Lcd.fillRect(0, 0, 320, 70,BLACK); // 塗りつぶし left, top, witdh, height
    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(TFT_RED,TFT_BLACK);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.print(name[num]);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.setTextSize(1);
    // JSONメッセージの作成
    JsonArray idValues = doc.createNestedArray("ID");
    idValues.add(name[num]);

    JsonArray dataValues = doc.createNestedArray("temp");
    dataValues.add("in");

    serializeJson(doc, pubMessage);

    // HTTP通信開始
    HTTPClient http;

    Serial.print(" HTTP通信開始　\n");
    http.begin(published_url);
   
    Serial.print(" HTTP通信POST　\n");
    int httpCode = http.POST(pubMessage);
   
    if(httpCode > 0){
      //M5.Lcd.printf(" HTTP Response:%d\n", httpCode);
   
      if(httpCode == HTTP_CODE_OK){
        //M5.Lcd.println(" HTTP Success!!");
        String payload = http.getString();
        Serial.println(payload);
      }
    }else{
      //M5.Lcd.println(" FAILED");
      Serial.printf("　HTTP　failed,error: %s\n", http.errorToString(httpCode).c_str());
    }
   
    http.end();
    M5.Lcd.fillRect(0, 0, 320, 70,BLACK); // 塗りつぶし left, top, witdh, height
    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.print(name[num]);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.setTextSize(1);
  }

  // Bボタンが押されたら
  if(M5.BtnB.wasPressed())
  {
    //M5.Speaker.tone(523, 200);
    //delay(200);
    //M5.Speaker.tone(659, 200);
    M5.Lcd.fillRect(0, 0, 320, 70,BLACK); // 塗りつぶし left, top, witdh, height
    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(TFT_BLUE,TFT_BLACK);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.print(name[num]);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.setTextSize(1);
    // JSONメッセージの作成
    JsonArray idValues = doc.createNestedArray("ID");
    idValues.add(name[num]);

    JsonArray dataValues = doc.createNestedArray("temp");
    dataValues.add("out");

    serializeJson(doc, pubMessage);

    // HTTP通信開始
    HTTPClient http;

    Serial.print(" HTTP通信開始　\n");
    http.begin(published_url);
   
    Serial.print(" HTTP通信POST　\n");
    int httpCode = http.POST(pubMessage);
   
    if(httpCode > 0){
      //M5.Lcd.printf(" HTTP Response:%d\n", httpCode);
   
      if(httpCode == HTTP_CODE_OK){
        //M5.Lcd.println(" HTTP Success!!");
        String payload = http.getString();
        Serial.println(payload);
      }
    }else{
      //M5.Lcd.println(" FAILED");
      Serial.printf("　HTTP　failed,error: %s\n", http.errorToString(httpCode).c_str());
    }
   
    http.end();
    M5.Lcd.fillRect(0, 0, 320, 70,BLACK); // 塗りつぶし left, top, witdh, height
    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.print(name[num]);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.setTextSize(1);
    
  }

  // Cボタンが押されたら
  if(M5.BtnC.wasPressed())
  {
    //M5.Speaker.tone(440, 100);
    //delay(100);
    //M5.Speaker.mute();
    //delay(100);
    //M5.Speaker.tone(440, 100);
    //delay(100);
    //M5.Speaker.mute();
    //delay(100);
    //M5.Speaker.tone(440, 100);
    num++;
    M5.Lcd.fillRect(0, 0, 320, 70,BLACK); // 塗りつぶし left, top, witdh, height
    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.print(name[num]);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.setTextSize(1);
    if(num==11)
    {
      num=-1;
    } 
  }
  M5.update();  // ボタン操作の状況を読み込む関数(ボタン操作を行う際は必須)
}
