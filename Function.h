/********************펠티어함수*****************/
void TEMPcalcul(){//(온도)펠티어동작여부판단
    if(TEMP >= t1+t2){//냉각필요
        CoolMode();
    }else if(TEMP <= t1-t2){//가열필요
        HotMode();
    }else{//적정온도
        PauseMode();
    }
}
void CoolMode(){//펠티어냉각모드
    digitalWrite(RelayA,LOW);
    digitalWrite(RelayB,HIGH);
    digitalWrite(pinPFAN,HIGH);
    lcd.setCursor(0,2);lcd.write(6);lcd.print("COOL");
    FanMode=0;FanMoving();
}
void HotMode(){//펠티어가열모드
    digitalWrite(RelayA,HIGH);
    digitalWrite(RelayB,LOW);
    digitalWrite(pinPFAN,HIGH);
    lcd.setCursor(0,2);lcd.write(6);lcd.print("HOT ");
    FanMode=0;FanMoving();
}
void PauseMode(){//펠티어정지모드
    digitalWrite(RelayA,HIGH);
    digitalWrite(RelayB,HIGH);
    digitalWrite(pinPFAN,LOW);
    lcd.setCursor(0,2);lcd.write(6);lcd.print("OFF ");
    FanMode=1;FanMoving();
}
/********************LED함수********************/
void LEDon(){//재배등LED켜기
    digitalWrite(pinLED,HIGH);
    lcd.setCursor(4,3);lcd.write(7);lcd.print("ON ");
    _timer.setTimeout(10000,LEDoff);
}
void LEDoff(){//재배등LED끄기
    lcd.setCursor(4,3);lcd.write(7);lcd.print("OFF");
    digitalWrite(pinLED,LOW);
}
/********************워터펌프함수***************/
void MOIScalcul(){//(수분)워터펌프동작여부판단
    if(MOIS<50){
        WATERPUMPon();
    }
}
void WATERPUMPon(){//워터펌프켜기
    digitalWrite(pinWATERPUMP,LOW);
    lcd.setCursor(8,3);lcd.write(8);lcd.print("ON ");
    _timer.setTimeout(2000,WATERPUMPoff);
}
void WATERPUMPoff(){//워터펌프끄기
    digitalWrite(pinWATERPUMP,HIGH);
    lcd.setCursor(8,3);lcd.write(8);lcd.print("OFF");
}
/********************FAN함수********************/
void FanMoving(){//환기팬온앤오프
    if(FanMode==0){
        digitalWrite(pinFAN,LOW);
    }else if(FanMode==1){
        digitalWrite(pinFAN,HIGH);
    }
}
/********************LCD깜박임******************/
void Blink(){//LCD부분지우기
    if(rotaryMode!=0){
        switch(rotaryMode){
            case 1:lcd.setCursor(13,0);break;
            case 2:lcd.setCursor(13,1);break;
            case 3:lcd.setCursor(13,2);break;
            case 4:lcd.setCursor(13,3);break;
        }
        lcd.print("       ");
    }
}
/********************값측정함수*****************/
void Value(){//값측정(온도,습도,수분)
    t = dht.readTemperature();//온도
    h = dht.readHumidity();//습도
    if(abs(t)>=0) TEMP = t+t4;//t값이 nan일땐 지정안함
    if(abs(h)>=0) HUMI = h;   //h값이 nan일땐 지정안함
    w = analogRead(pinMOIS);//수분
    MOIS = w;
    MOIS = map(w,4000,3000,0,100);
    MOIS = constrain(MOIS,0,100);
}
