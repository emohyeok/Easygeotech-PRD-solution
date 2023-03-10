//********************************************************************
//**B 아두이노 코딩**
//1. B는 공기압 센서를 실시간으로 확인한다
//2. 변수 a값을 저장하여 B의 신호를 받을 때마다 확인하는 시그널을 추가한다

//** A아두이노 필요한 모듈 및 부품**
//1. 2채널 B타입 릴레이 1개 {BS1, BS2 2개 릴레이 채널 필요}
//2. 아두이노 및 점퍼선
//3. BLDC 모터 및 BLDC 드라이버 (가변 저항 조절기 포함)
//4. 공기압 센서 1개 필요 (BMP388 or DPS310)
//********************************************************************
//********************************************************************
//** 값 저장용 및 확인용 변수 선언**
#include <SoftwareSerial.h> //또 다른 가상 시리얼 통신주파수 설정 
#define BT_RX 10 //블루투스 RX 통신 핀을 10번으로 설정
#define BT_TX 11 //블루투스 TX 통신 핀을 11번으로 설정

SoftwareSerial bt(BT_RX,BT_TX); // 가상 시리얼 통신 주파수를 블루투스 주파수로 맞춤

char data; //data는 블루투스 데이터를 받아오기 위한 함수임

int RED = 5; // RED LED  5번 핀 변수 저장
int GREEN = 3; // GREEN LED 3번 핀 변수 저장
int READ = A5; // READ (센서 값) 받아들이는 값 변수 A5(아날로그만이 센서를 측정 가능) 에 저장

int BS1 = 6; //|SIGNAL 1| 2-1 B to A 신호용 릴레이 변수 값 저장 (합격)
int BS2 = 4; //|SIGNAL 2| 2-2 B to A 신호용 릴레이 변수 값 저장 (불합격)
int AS1 = 7; //|A to B SIGNAL 1| A에게서 받은 신호 값 저장용 변수 값 저장

int COF = 0; //|Correct or False| AS1으로 부터 입력 값을 받을 때 검사 할 때 저장용 변수 

int SENS = 0; //|공기압 센서| 공기압 센서 값 저장용 변수

int TIME1 = 0; //|시간 변수| 공기압 센서 시간용 변수 1 - 2-1 시그널 관련 (합격)
int TIME2 = 0; //|시간 변수| 공기압 센서 시간용 변수 2 - 2-2 시그널 관련 (불합격)

int STATUS = 0 ; // |상황 값 저장| 어떤 모드인지 변화하는지에 따라 STATUS가 0 이고 1 이 된다.
//********************************************************************
//********************************************************************


void setup() {

Serial.begin(9600); //시리얼 통신 BAUD RATE 9600 주파수 선언
bt.begin(9600); // 블루투스 시리얼 통신도 BAUD RATE 9600으로 주파수 선언

pinMode(AS1,INPUT_PULLUP); //|A to B SIGNAL 1| A to B 릴레이에서 온 신호 INPUT 값 풀업 저항으로 선언
pinMode(BS1,OUTPUT); //|B to A SIGNAL 1| B to A 릴레이 2-1 아웃풋 선언 (합격)
pinMode(BS2,OUTPUT); //|B to A SIGNAL 2| B to A 릴레이 2-2 아웃풋 선언 (불합격)

pinMode(RED,OUTPUT); // RED핀 OUTPUT 선언
pinMode(GREEN,OUTPUT); // GREEN핀 OUTPUT 선언

}

void loop() {
  if(STATUS==0){   //상황 STATUS가 0일 때 문제가 없을 떄
    
    analogWrite(GREEN,130); //초록 LED가 255면 터지므로 130의 밝기로 빛난다
    analogWrite(RED,0); // 빨간 LED는 0의 값을 낸다 (꺼진다)
  }
  char data; // 다시 한 번 data 함수를 호출한다
  data = bt.read(); // data는 블루투스에서 읽는 값이 될 것이다 (문자 혹은 숫자)
  if(data == 's'){ //그 data가 "s"가 호출된다면
    STATUS=1; // 상황은 STATUS는 1이 된다
  } 
  if(STATUS==1){ //상황이 1일때 (긴급상황일때) 
    bt.println("!  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !");  // 블루투스 화면에 현재 줄 부터 아래줄 까지의 텍스트를 띄운다
    bt.println("작업을 중지 하였습니다");
    bt.println("작업을 다시 원하신다면");
    bt.println("재부팅 하여주세요");
    bt.println("!  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !  !"); // 여기까지.
    analogWrite(GREEN,0); // 초록불은 꺼지고
    analogWrite(RED,130);  // 빨간 LED는 130의 밝기로 빛난다
    delay(3000000000000); // 작동 중지
  }
  
 //---------------------------------------------------------------------------------------------
 //                       COF 변 수 상 태 , 공 기 압 센 서 변 수 상 태 확 인 용
 //---------------------------------------------------------------------------------------------
 SENS = analogRead(A0); //|공기압 센서| 공기압 센서 값 저장용 변수 analogRead A0 포트로 부터 값 읽기 
                        //****후에 계산식 추가 및 digitalRead로 변동 유무 있음****
                        
 Serial.print("합격 :  "); ////센서 값 확인용 시리얼 통신 9600 시리얼 모니터 기능 활용하여 확인
 Serial.println(TIME1); //|시간 변수|공기압 센서 시간용 변수 1 - 2-1 시그널 관련 (합격) 저장용 변수 값 확인

 Serial.print("불합격 :  "); ////센서 값 확인용 시리얼 통신 9600 시리얼 모니터 기능 활용하여 확인
 Serial.println(TIME2); //|시간 변수| 공기압 센서 시간용 변수 2 - 2-2 시그널 관련 (불합격) 저장용 변수 값 확인
 
 Serial.print("COF :  "); ////센서 값 확인용 시리얼 통신 9600 시리얼 모니터 기능 활용하여 확인
 Serial.println(COF); //|Correct or False| AS1으로 부터 입력 값을 받을 때 검사 할 때 저장용 변수 확인
 Serial.println("------------------------------"); //시리얼 모니터 가독성 증가용 줄

  // 아래는 어플로 데이터를 받고 분리하기 위에 999임의의 숫자와 "," 쉼표 SENS 값
  // ","쉼표 888임의의 숫자로 마무리 하여 나중에 어플에서 데이터 보내기 위험 용
  bt.print("999");// 블루투스 화면에 현재 줄 부터 아래줄 까지의 텍스트를 띄운다 (임의의 숫자로 대입하여 시도) 
  bt.print(","); //블루투스 시리얼 모니터로 ","로 보낸다
  bt.print(SENS); //SENS값 데이터 값 플로팅 전송
  bt.print(","); //블루투스 시리얼 모니터로 ","로 보낸다
  bt.print(analogRead(READ)); // anlaogRead(READ) 함수를 블루투스 시리얼 모니터로 보낸다
  bt.print(","); //블루투스 시리얼 모니터로 ","로 보낸다
  bt.println("888"); //블루투스 시리얼 모니터로 "888"로 보낸다 (임의의 숫자로 대입하여 시도)
 delay(500); //아두이노 값 안정화 delay
 //-----------------------------------------------------------------------------------------------
 //                                      COF 변 수 저 장 용
 //-----------------------------------------------------------------------------------------------
 if (digitalRead(AS1) ==LOW){ //|A to B SIGNAL 1| A to B 릴레이에서 온 신호 INPUT 값이 들어올 때,
 COF=1; //|Correct or False|의 변수 저장 상태가 1이 된다.
 delay(100); //아두이노 값 안정화 delay
 
} //닫힘


 //-----------------------------------------------------------------------------------------------
 //         ▶▶▶▶ 센 서 확 인 기 압 이 " 20 ↓ " 이 하 " 5 초 ↑ " 이 상 이 라 면 " 합 격 " ◀◀◀◀
 //---------------------------------------------------------------------------------------------
 
 if(COF==1 && SENS <= 150){ //"COF 상태가 1일 때(A 아두이노로 부터 확인 요청일 때)와 공기압 센서가 '20' 이하 일 때,"
  TIME1++; //TIME1의 시간 저장용 변수가 정수 1씩 상승한다.
  delay(600); //1초 후 반복한다. 즉 1초에 TIME1은 1,2,3...씩 1씩 ++가 된다
 }  else { //위의 변수들이 아니라면,
  TIME1--; //TIME1의 시간 저장용 변수는 정수 1씩 줄어든다.
  delay(10); //0.5초 후 반복한다. 즉 1초에 TIME1은 3,2,1..씩 1씩 --가 된다
 }
 if(TIME1 >= 5){ //TIME1의 변수가 5 이상이라면 (5초 이상이라면)
 OK();
 COF = 0; //|Correct or False| 변수는 역할을 끝냈기에 0으로 돌린다.
 TIME1 = 0; //마찬가지로 역할을 끝냈기에 0으로 돌린다.
 delay(100); //아두이노 값 안정화 delay
 }
 
 if(TIME1 < 0){ // 위의 TIME1의 변수가 -로 무한히 떨어지는 것을 방지한다.
 TIME1 = 0; // TIME1이 0이하 라면 다시 0 으로 맞춘다.
 delay(100); // 아두이노 값 안정화 delay
 }
 
 //-----------------------------------------------------------------------------------------------
 //       ▶▶▶▶ 센 서 확 인 기 압 이 " 22 ↑ " 이 상 " 3 초 ↑ " 이 상 이 라 면 " 불 합 격 " ◀◀◀◀
 //-----------------------------------------------------------------------------------------------
 
 if(COF==1 && SENS >= 250){ //"COF 상태가 1일 때(A 아두이노로 부터 확인 요청일 때)와 공기압 센서가 '22' 이상 일 때,"
  TIME2++; //TIME2의 시간 저장용 변수가 정수 1씩 상승한다.
  delay(600); // // 1초 후 반복한다. (즉 1초에 TIME2은 1,2,3...씩 1씩 ++가 된다)
 } else { //위의 변수들이 아니라면,
  TIME2--; //TIME2의 시간 저장용 변수는 정수 1씩 줄어든다.
  delay(10); //0.5초 후 반복한다. 즉 1초에 TIME2은 3,2,1..씩 1씩 --가 된다
 }
 if(TIME2 < 0){ // 위의 TIME2의 변수가 -로 무한히 떨어지는 것을 방지한다.
 TIME2 = 0; // TIME2이 0이하 라면 다시 0 으로 맞춘다.
 delay(100); // 아두이노 값 안정화 delay
 }
 if(TIME2 >= 4){ //TIME1의 변수가 5 이상이라면 (5초 이상이라면)
 NOK();
 COF = 0; //|Correct or False| 변수는 역할을 끝냈기에 0으로 돌린다.
 TIME2 = 0; //마찬가지로 역할을 끝냈기에 0으로 돌린다.
 delay(100); //아두이노 값 안정화 delay
 }
} //▲▲▲▲▲▲VOID LOOP 함수 닫힘▲▲▲▲▲▲

 //-----------------------------------------------------------------------------------------------
 //                     ▶▶▶▶  " 합 격 " 과 " 불 합 격 " 함 수 저 장 구 역 ◀◀◀◀
 //-----------------------------------------------------------------------------------------------
 
void NOK(){ //불합격 함수
digitalWrite(BS2,255); //BS2는 즉 불합격을 의미하는 릴레이 신호를 의미
delay(2500); //1.7초 정도 켜졌다가 혹시나 모를 A 아두이노가 받아들이지 못할 때를 대비
digitalWrite(BS2,0); //BS2 다음 시그널이 겹치지 않게 꺼진다
delay(100); //아두이노 값 안정화 delay 
}

void OK(){ //합격 함수

digitalWrite(BS1,255); //BS1는 즉 합격을 의미하는 릴레이 신호를 의미
delay(2500); //1.7초 정도 켜졌다가 혹시나 모를 A 아두이노가 받아들이지 못할 때를 대비
digitalWrite(BS1,0); //BS1 다음 시그널이 겹치지 않게 꺼진다
delay(100); //아두이노 값 안정화 delay
}
