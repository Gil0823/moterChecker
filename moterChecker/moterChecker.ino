#include <Servo.h>
 
const int slider = A2; // 슬라이더 포트
const int button = 4;  // 버튼 포트
int mapping = 0;  // 슬라이더값 전용 변수
Servo moter;

void hi(void);
void checkMoter(void);
void controlling(void);
void set(int zero, int lim);
void setZero(void);

void setup() {  // 프로그램 초기설정
  Serial.begin(9600); // 시리얼 시작(컴퓨터 - 아두이노 간 통신시작) 
  pinMode(button, INPUT);
  hi();
}
 
void loop() {
  if(Serial.available()) {
    char c = Serial.read();

    switch(c) {
      case '1':
        checkMoter();
        controlling();
        break;
      case '2':
        checkMoter();
        setZero();
        break;
      default:
        ;
    }
  delay(100);
  }
}

void hi(void) {
  Serial.println("********POS CHECKER********");
  Serial.println("1. Checking");
  Serial.println("2. Testing");
  Serial.println("");
}

void checkMoter(void) {
  static int tmp = 0;
  static int pin = 0;
  static bool count = false;
  
  Serial.print("What port want to attach? : "); 
  for(tmp = 0;;) {
    if(Serial.available()) {
      char c = Serial.read();
      
      if(count) {
        tmp = atoi(&c);
        count = false;
        
        Serial.print("A"); Serial.println(tmp);
        switch(tmp) {
          case 0:
            pin = A0;
            break;
          case 1:
            pin = A1;
            break;
          case 2:
            pin = A2;
            break;
          case 3:
            pin = A3;
            break;
          case 4:
            pin = A4;
            break;
          case 5:
            pin = A5;
            break;
          default:
            ;
        }
        Serial.print("Pin? : "); Serial.println(pin);
        moter.attach(pin);
        Serial.println("Done!");
        break;
      }
      else {
        count = true;
      }
    }
  }
}

void controlling(void) {
  for(;;) {
    if(!(digitalRead(button))) {
      moter.detach();
      Serial.println("Escape!!");
      hi();
      break;
    }
    mapping = map(analogRead(slider), 0, 1023, 0, 180);
    if(mapping >= 160) {
      mapping = 160;
    }
    moter.write(mapping);
    Serial.print("moter "); Serial.println(mapping);
  }
}

void set(int zero, int lim, Servo * m) {
  m->write(zero);
  delay(500);
  m->write(lim);
  delay(300);
}

void setZero(void) {
  static bool recieve = false;
  static int check = 0;
  static int count = 0;
  int zero = 0;
  int lim = 0;
  int buf[3] = {0, };
  
  Serial.print("set zero : ");
  for(check = 0, count = 0, recieve = false;;) {
    if(Serial.available()) {
    recieve = true;
    char c = Serial.read();
    buf[check] = atoi(&c);
    check++;
    delay(10);  // Serial이 씹힘
    }
    if(recieve) {
      count++;
    }
    if(check != count) {
      switch(check) {
        case 1:
          zero = buf[0];
          Serial.println(zero);
          break;
        case 2:
          zero = (buf[0]*10) + (buf[1]);
          Serial.println(zero);
          break;
        case 3:
          zero = (buf[0]*100) + (buf[1]*10) + (buf[2]);
          Serial.println(zero);
          break;
        default:
          ;
      }
      break;
    }
  }
  Serial.print("set lim : ");
  for(check = 0, count = 0, recieve = false;;) {
    if(Serial.available()) {
    recieve = true;
    char c = Serial.read();
    buf[check] = atoi(&c);
    check++;
    delay(10);  // Serial이 씹힘
    }
    if(recieve) {
      count++;
    }
    if(check != count) {
      switch(check) {
        case 1:
          lim = buf[0];
          Serial.println(lim);
          break;
        case 2:
          lim = (buf[0]*10) + (buf[1]);
          Serial.println(lim);
          break;
        case 3:
          lim = (buf[0]*100) + (buf[1]*10) + (buf[2]);
          Serial.println(lim);
          break;
        default:
          ;
      }
      break;
    }
  }
  Serial.println("After 5 second later, test will start");
  for(int i = 5; i > 0; i--) {
    Serial.print(i); Serial.println(" second leftt");
    delay(1000);
  }
  Serial.println("Activate!!");
  moter.write(zero);
  set(zero, lim, &moter);
  delay(500);
  set(zero, lim, &moter);
  hi();
}

