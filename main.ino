/********************함수원형선언***************/
void TEMPcalcul();  //(온도)펠티어동작여부판단
void CoolMode();    //펠티어냉각모드
void HotMode();     //펠티어가열모드
void PauseMode();   //펠티어정지모드
void LEDon();       //재배등LED켜기
void LEDoff();      //재배등LED끄기
void MOIScalcul();  //(수분)워터펌프동작여부판단
void WATERPUMPon(); //워터펌프켜기
void WATERPUMPoff();//워터펌프끄기
void FanMoving();   //환기팬온앤오프
void Blink();       //LCD부분지우기
void Value();       //값측정(온도,습도,수분)
/********************라이브러리*****************/
#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"
#include "basicFunction.h"
#include "InitData.h"
#include "Function.h"
/*******************로터리엔코더****************/
void rotary_onButtonClickChatt(){//채터링방지 100밀리이하
    static unsigned long lastTimePressed = 0;
    if (millis() - lastTimePressed < 100)   return;
    lastTimePressed = millis();
}
void rotary_loop(){//루프에서 반복됨
    
    if (rotaryEncoder.isEncoderButtonClicked()){//로터리 버튼이 클릭되었을때
        rotary_onButtonClickChatt();
        rotaryMode++;       
        if(rotaryMode>4) rotaryMode = 0;
        
        if(rotaryMode==0){     
        }else if(rotaryMode==1) {
            rotaryEncoder1.setEncoderValue(EEPROM.read(10));
        }else if(rotaryMode==2) {
            rotaryEncoder2.setEncoderValue(EEPROM.read(20));
        }else if(rotaryMode==3) {
            rotaryEncoder3.setEncoderValue(EEPROM.read(30));
        }else if(rotaryMode==4) {
            rotaryEncoder4.setEncoderValue(EEPROM.read(40));
        }
        level_up_melody(n32);
        
    }else if(rotaryEncoder.encoderChanged()){//로터리값이 변경되었을때    
        if(rotaryMode==1){  //설정온도
            rotaryEncoder1.setBoundaries(0,40,false); 
            t1 = rotaryEncoder1.readEncoder();
            EEPROM.write(10,rotaryEncoder1.readEncoder());
            
        }else if(rotaryMode==2){ //편차온도
            rotaryEncoder2.setBoundaries(0,100,false); //(0~10)
            t2 = double(rotaryEncoder2.readEncoder())/10;
            EEPROM.write(20,rotaryEncoder2.readEncoder());
            
        }else if(rotaryMode==3){ //지연시간
            rotaryEncoder3.setBoundaries(1,10,false);
            t3=rotaryEncoder3.readEncoder();
            EEPROM.write(30,rotaryEncoder3.readEncoder());
            
        }else if(rotaryMode==4){ //보정온도
            rotaryEncoder4.setBoundaries(10,190,false);//(-9~9)
            t4 = double(rotaryEncoder4.readEncoder())/10-10;
            EEPROM.write(40,rotaryEncoder4.readEncoder());   
            
        }
        play(NOTE_G3,n16);//삐-소리
        EEPROM.commit();
    }
}
void IRAM_ATTR readEncoderISR(){//인터럽트
    rotaryEncoder.readEncoder_ISR();
    rotaryEncoder1.readEncoder_ISR();
    rotaryEncoder2.readEncoder_ISR();
    rotaryEncoder3.readEncoder_ISR();
    rotaryEncoder4.readEncoder_ISR();
}
/********************셋업***********************/
void setup(){
    start(1);//start버튼동작시작
    
    rotaryEncoder.begin();rotaryEncoder.setup(readEncoderISR);rotaryEncoder.setAcceleration(0);
    rotaryEncoder1.begin();rotaryEncoder1.setup(readEncoderISR);rotaryEncoder1.setAcceleration(0);
    rotaryEncoder2.begin();rotaryEncoder2.setup(readEncoderISR);rotaryEncoder2.setAcceleration(0);
    rotaryEncoder3.begin();rotaryEncoder3.setup(readEncoderISR);rotaryEncoder3.setAcceleration(0);
    rotaryEncoder4.begin();rotaryEncoder4.setup(readEncoderISR);rotaryEncoder4.setAcceleration(0);
    dht.begin();
    
    //사용자정의문자선언
    lcd.createChar(1,h_temp);
    lcd.createChar(2,h_humi);
    lcd.createChar(3,h_cels);
    lcd.createChar(4,h_fan1);
    lcd.createChar(5,h_fan2);
    lcd.createChar(6,h_pelt);
    lcd.createChar(7,h_led);
    lcd.createChar(8,h_wate);
    //핀모드설정
    pinMode(RelayA,OUTPUT);
    pinMode(RelayB,OUTPUT);
    pinMode(pinPFAN,OUTPUT);
    pinMode(pinLED,OUTPUT);
    pinMode(pinFAN,OUTPUT);
    pinMode(pinWATERPUMP,OUTPUT);
    WATERPUMPoff();
    pinMode(DHTPIN,INPUT);
    pinMode(pinMOIS,INPUT);

    //상시반복함수
    _timer.setInterval(20000,LEDon);       //LED동작
    _timer.setInterval(t3*1000,TEMPcalcul);//온도에따른제어명령
    _timer.setInterval(500,ShowLCD);       //LCD출력
    _timer.setInterval(1000,Blink);        //로터리 변경대상 깜박임
    _timer.setInterval(1000,Value);        //값측정
    _timer.setInterval(MD,MOIScalcul);     //수분에따른WP제어명령

    //EEPROM저장값 불러오기
    t1=EEPROM.read(10);       //0~40
    t2=double(EEPROM.read(20))/10;    //0~10
    t3=EEPROM.read(30);       //1~10
    t4=double(EEPROM.read(40))/10-10; //-9~9
}
/********************루프***********************/
void loop(){
    rotary_loop();
    _timer.run();
    delay(10);
}
/********************LCD출력********************/
void ShowLCD(){
    lcd.setCursor(0,0);lcd.write(1);lcd.print(TEMP,1);lcd.write(3);               //온도출력
    lcd.setCursor(0,1);lcd.write(2);lcd.print(HUMI,1);lcd.print("% ");            //습도출력
    lcd.setCursor(7,1);lcd.write(8);lcd.print(MOIS);lcd.print("%");lcd.print(" ");//수분출력
    lcd.setCursor(13,0);lcd.print("t1:");lcd.print(t1);    //희망온도출력
    lcd.setCursor(13,1);lcd.print("t2:");lcd.print(t2,1);  //편차온도출력
    lcd.setCursor(13,2);lcd.print("t3:");lcd.print(t3);    //지연시간출력
    lcd.setCursor(13,3);lcd.print("t4:");lcd.print(t4,1);  //보정온도출력
    if(FanMode==1){
        if(FanMotionFlag==0){
            FanMotionFlag++;lcd.setCursor(0,3);lcd.write(4);lcd.print("ON ");
        }else{
            FanMotionFlag=0;lcd.setCursor(0,3);lcd.write(5);lcd.print("ON ");
        }
    }else{
        lcd.setCursor(0,3);lcd.write(4);lcd.print("OFF");
    }
}
