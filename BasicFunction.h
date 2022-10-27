#define pinBUZZER 15    //pinA1 보드에 장창된 패시브버져 
#define pinSTART 0      //pinA3 보드 시작버튼 
#define pinENC1 39      //pinD5
#define pinENC2 32      //pinD2
#define pinENC3 33      //pinD1
#define n32     1*3     //3  32분음표 
#define n16     2*3     //6  16분음표
#define NOTE_G3  196
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_G5  784
#define NOTE_C6  1047
#define  bpm_offset  2561//1550//올라갈수록 느려짐       
#define START_KEY   (pinMode(pinSTART,INPUT_PULLUP), delay(10),((digitalRead(pinSTART))==0))
unsigned int bpm = 200;
unsigned char lcdAddress = 0x3f;

#include <EEPROM.h>
#include <ESP32Servo.h>           //esp32보드에서 톤함수 서보함수 아날로그 출력에 활용한다
#include <DHT.h>                 //온습도 센서 라이브러리
#include <Wire.h>                //I2C 통신 라이브러리
#include <I2C_LCD.h>
#include <SimpleTimer.h>
I2C_LCD lcd(lcdAddress, 20, 4);

void play(unsigned int sound, unsigned int note); //박자수만큼 음을 연주
void boot_melody(int count);                // 부팅멜로디 함수 
void level_up_melody(unsigned int note);    //도미솔도

void lcd_init();

void play(unsigned int sound, unsigned int note){
    note*=(bpm_offset/bpm);
    tone(pinBUZZER, sound, note);
    delay(note*1.00);
    noTone(pinBUZZER);
}
void start(bool Start_Key){
    pinMode(pinSTART,INPUT_PULLUP);
    pinMode(pinENC1,INPUT_PULLUP);
    pinMode(pinENC2,INPUT);
    pinMode(pinENC3,INPUT); 
    Wire.begin();
    EEPROM.begin(1023);
    lcd_init();
    boot_melody(1);
    delay(200);
    pinMode(pinBUZZER,OUTPUT); 
    digitalWrite(pinBUZZER,LOW);
    if (Start_Key){
        while(1){
        pinMode(pinSTART,OUTPUT);
        digitalWrite(pinSTART,HIGH);
        delay(50);
        digitalWrite(pinSTART,LOW);
        delay(50);
        digitalWrite(pinSTART,HIGH);
        pinMode(pinSTART,INPUT_PULLUP);
        if(START_KEY){
            delay(50);
            break;
            }
        }
    }
    boot_melody(2);
    lcd.clear();
    Serial.begin(57600);
}
void boot_melody(int count){
    for(int j=0;j<count;j++){
      for(int i=1800;i<2200;i=i+50){  
        tone(pinBUZZER,i,15);delay(8);
      }
      delay(60);
    }
    noTone(pinBUZZER);
    delay(100);
}
void level_up_melody(unsigned int note){
    int melody[]={NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6};
    note*=(bpm_offset/bpm);
    for (int thisNote = 0; thisNote < 4; thisNote++) {
        tone(pinBUZZER, melody[thisNote], note*1.3);
        delay(note);
    }
}
void lcd_init() {//시작이전 lcd
  lcdAddress = 0x3f;
  lcd.setAddress(lcdAddress);
  lcd.init();lcd.backlight();
  lcd.setCursor(0,1);lcd.print("| Auto  | Green |");
  lcd.setCursor(0,2);lcd.print("| matic | house |");
}
