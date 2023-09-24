#include "DHT.h"
#include <WiFi.h>
#include <ESP32Servo.h>
#include <Adafruit_Fingerprint.h>
#include <BlynkSimpleEsp32.h> 
#include <LiquidCrystal_I2C.h>
#include <PZEM004Tv30.h>

#define BLYNK_TEMPLATE_ID "TMPL2syJsqco"
#define BLYNK_TEMPLATE_NAME "Bùi Xuân Danh"
#define BLYNK_AUTH_TOKEN "MHqE61OVvfXlocwfWX70cZ_buBCa_kh6"
#define PZEM_RX_PIN 9
#define PZEM_TX_PIN 10
#define PZEM_SERIAL Serial1

  
#define mySerial Serial2

#define dhttype DHT11
#define dhtpin 25



Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const char *ssid = "HoaCuc";
const char *password = "danh1412";
char auth[] = BLYNK_AUTH_TOKEN;
Servo myservo;
Servo myservo1;

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(dhtpin, dhttype);
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
WidgetTerminal terminal(V13);

int gas = 27;
int flame = 26;
int rain = 14;
int servo = 12;
int relay1 = 23;
int relay2 = 19;
int relay3 = 18;
int relay4 = 4;
int relay5 = 2;
int relay6 = 15;
int warning = 32;
int servo1 = 13;


int prev;
void setup() {
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, password);
  prev = 1;
  

  
  pinMode(gas, INPUT);
  pinMode(flame, INPUT);
  pinMode(rain, INPUT);
  
  pinMode(warning, OUTPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  myservo.attach(servo);
  myservo1.attach(servo1);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  finger.begin(57600);
}

//Điền khiển bằng App
BLYNK_WRITE(V3){
  if (param.asInt() == 1){
    digitalWrite(relay1, HIGH);
  } else{
    digitalWrite(relay1, LOW);
  }
}
BLYNK_WRITE(V4){
  if (param.asInt() == 1){
    digitalWrite(relay2, HIGH);
  } else{
    digitalWrite(relay2, LOW);
  }
}
BLYNK_WRITE(V5){
  if (param.asInt() == 1){
      digitalWrite(relay3, HIGH);
  } else{
    digitalWrite(relay3, LOW);
  }
}


BLYNK_WRITE(V11){
  if (param.asInt() == 1){
    digitalWrite(relay5, HIGH);
  } else{
    digitalWrite(relay5, LOW);
  }
}
BLYNK_WRITE(V10){
  if (param.asInt() == 1){
    digitalWrite(relay6, HIGH);
  } else{
    digitalWrite(relay6, LOW);
  }
}
BLYNK_WRITE(V12){
  if (param.asInt() == 1){
    myservo1.write(0);
    lcd.setCursor(0, 1);
    lcd.print("Unlocked");
    delay(3000);
    myservo1.write(90);
    lcd.setCursor(0, 1);
    lcd.print("Locked");
    delay(1000);
    lcd.clear();
    Blynk.virtualWrite(V12, 0);
  }
}
BLYNK_WRITE(V13){
  if (String("Dang ky van tay") == param.asStr()){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("dat van tay len");
    int p = -1;
    int id = random(1,127);
    while (finger.loadModel(id) == FINGERPRINT_OK)
    {
      id = random(1,127);
    }
    while (p != FINGERPRINT_OK){
      p = finger.getImage();
      if (p == FINGERPRINT_OK){
        p = finger.image2Tz(1);
        if (p == FINGERPRINT_OK){
          p = finger.fingerFastSearch();
          if (p == FINGERPRINT_OK){
            lcd.clear();
            lcd.print("Van tay da co");
            delay(2000);
            break;
          } else
          lcd.clear();
          lcd.print("bo tay ra");
          p = finger.getImage();
          while (p == FINGERPRINT_OK){
            p = finger.getImage();
          }
          lcd.clear();
          lcd.print("xac thuc van tay");
          p = finger.getImage();
          while (p != FINGERPRINT_OK){
            p = finger.getImage();
          }
          if (p == FINGERPRINT_OK){
            p = finger.image2Tz(2);
            if (p == FINGERPRINT_OK){
              p = finger.createModel();
              if (p == FINGERPRINT_OK) {
                p = finger.storeModel(id);
                if (p == FINGERPRINT_OK){
                  lcd.clear();
                  lcd.print("thanh cong");
                  delay(2000);
                } else {
                  lcd.clear();
                  lcd.print("that bai");
                  delay(2000);
                  break; 
                } 
              } else {
                lcd.clear();
                lcd.print("Khong hop le");
                delay(3000);
                break;
              }
            } else break; 
          } else break;
        } else break;
      } 
    }
  } else 
   if (String("Xoa van tay") == param.asStr()) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("dat van tay len");
    int p = -1;
    while (p != FINGERPRINT_OK){
      p = finger.getImage();
      if (p == FINGERPRINT_OK){
        p = finger.image2Tz();
        if (p == FINGERPRINT_OK){
          p = finger.fingerFastSearch();
          if (p == FINGERPRINT_OK){
            p = finger.deleteModel(finger.fingerID);
            if (p == FINGERPRINT_OK){
              lcd.clear();
              lcd.print("Xoa thanh cong");
              delay(2000);
            }
          } else{
            lcd.clear();
            lcd.print("Khong ton tai");
            delay(2000);
            break;
          }
        } else break;
      }
    }
  } else
   if (String("Truy van") == param.asStr()){
    int p = -1;
    p = finger.getTemplateCount();
    if (p == FINGERPRINT_OK){
      lcd.clear();
      lcd.print("Co ");
      lcd.setCursor(3,0);
      lcd.print(finger.templateCount);
      Serial.print(finger.templateCount);
      lcd.setCursor(6,0);
      lcd.print("van tay");
      delay(2000);
      lcd.clear();
    }
   }else if (String("Xoa het") == param.asStr()){
    lcd.clear();
    int p = finger.emptyDatabase();
    if (p == FINGERPRINT_OK){
      lcd.print("Xoa hoan tat ");
      delay(2000);
    }
   }
}
  
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V7);
  Blynk.syncVirtual(V8);
}
void loop() {
    if (WiFi.status() != WL_CONNECTED){
      prev = 0;
    } else {
    if (prev == 0){
        Blynk.begin(auth, ssid, password);
        prev = 1;
      }
    }
//Kiểm tra tình trạng hoạt động của vân tay và xác thực mở cửa vân tay. 
  if (finger.verifyPassword()) {
    lcd.setCursor(0,0);
    lcd.print("Fingerprint?????");
  } else {
    lcd.setCursor(0,0);
    lcd.print("Sensor Broken");
  }

  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_OK){
    p = finger.image2Tz();
    lcd.setCursor(0,0);
    lcd.print("Wait for it...");
    if (p == FINGERPRINT_OK){
      p = finger.fingerFastSearch();
      if (p == FINGERPRINT_OK){
        myservo1.write(0);
        lcd.setCursor(0, 1);
        lcd.print("Unlocked");
        delay(3000);
        myservo1.write(90);
        lcd.setCursor(0, 1);
        lcd.print("Locked");
        delay(1000);
        lcd.clear();
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Wrong finger");
        delay(1500);
        lcd.clear();
      }     
    } 
  }
// Đọc dữ liệu nhiệt độ, độ ẩm và gửi lên App
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  
//Cảnh báo lửa cháy và rò rỉ khí gas
  if ((digitalRead(gas) == 0)) {
    Blynk.virtualWrite (V6, 1);
    digitalWrite(relay4, HIGH);
    digitalWrite(warning, HIGH);
  } else 
  {
    Blynk.virtualWrite (V6, 0);
    digitalWrite(relay4, LOW);
    digitalWrite(warning, LOW);
  }
  if ((digitalRead(flame) == 0)) {
    digitalWrite(warning, HIGH);
  } else 
  {
    digitalWrite(warning, LOW);
  }

  
  
//Cảnh báo mưa
  if (digitalRead(rain) == 0)
  {
    myservo.write(90);
  } else 
  {
    myservo.write(0);
  }
//Giám sát công suất tiêu thụ
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    Serial.println(voltage);
    Serial.println(current);
    Serial.println(power);
    Serial.println(energy);
    if(isnan(voltage)){
        Blynk.virtualWrite(V0, 0);
        Blynk.virtualWrite(V9, 0);
        Blynk.virtualWrite(V7, 0);
        Blynk.virtualWrite(V8, 0);
    }else
    {
      Blynk.virtualWrite(V0, voltage);
      Blynk.virtualWrite(V9, current);
      Blynk.virtualWrite(V7, energy);
      Blynk.virtualWrite(V8, power);
    }
}
