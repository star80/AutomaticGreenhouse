/********************핀*************************/
#define RelayA 18      //릴레이1
#define RelayB 25      //릴레이2
#define pinPFAN 17     //펠티어팬(연결위치:환기팬)
#define pinLED 2       //재배등LED(연결위치:재배등2)
#define pinFAN 5       //환기팬(연결위치:재배등)
#define pinMOIS 4      //수분센서(연결위치:수위센서)
#define pinWATERPUMP 26//워터펌프(연결위치:스프링쿨러)
/********************변수***********************/
SimpleTimer _timer;
#define DHTPIN 27//온습도센서(연결위치:온습도센서)
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float t;//측정온도
float h;//측정습도
int w;  //측정수분
double TEMP;//사용온도
double HUMI;//사용습도
int MOIS;   //사용수분
volatile int t1 = 1;    //희망온도
volatile double t2 = 1;//편차온도
volatile int t3 = 1;     //지연시간(초)
volatile double t4 = 1; //보정온도

int FanMode = 0;      //팬동작모드(0:정지/1:동작)
int FanMotionFlag = 0;//팬마크상태(0:팬마크1/1:팬마크2)
int rotaryMode = 0;//로터리설정모드
int MD=35000;//수분측정주기= (MD-5000)/1000 초
/********************사용자정의문자*************/
byte h_temp[8] = {0b01110,0b01010,0b01010,0b01010,0b01010,0b10001,0b11111,0b01110};//온도마크
byte h_humi[8] = {0b10001,0b10001,0b10001,0b10001,0b11011,0b11111,0b11111,0b11111};//습도마크
byte h_cels[8] = {0b10000,0b00000,0b01110,0b10001,0b10000,0b10000,0b00001,0b01110};//℃마크
byte h_fan1[8] = {0b00000,0b10111,0b11110,0b11011,0b01111,0b11101,0b00000,0b00000};//팬마크1
byte h_fan2[8] = {0b00000,0b00110,0b10100,0b11011,0b00101,0b01100,0b00000,0b00000};//팬마크2
byte h_pelt[8] = {0b00001,0b00011,0b00111,0b01111,0b10110,0b00100,0b01000,0b10000};//펠티어마크
byte h_led[8] = {0b01110,0b11101,0b11011,0b10111,0b11111,0b01110,0b01110,0b00100}; //재배등LED마크
byte h_wate[8] = {0b00000,0b00100,0b01110,0b01110,0b11111,0b11111,0b11111,0b01110};//수분,워터펌프마크
/***************로터리엔코더초기설정관련*********/
#define ROTARY_ENCODER_A_PIN pinENC2    //A
#define ROTARY_ENCODER_B_PIN pinENC3    //B
#define ROTARY_ENCODER_BUTTON_PIN pinENC1   // BUTTON
#define ROTARY_ENCODER_VCC_PIN -1   
#define ROTARY_ENCODER_STEPS 4
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder1 = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder2 = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder3 = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder4 = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
