#include <SoftwareSerial.h>
#include <Servo.h> //Servo 라이브러리를 추가
#include <Wire.h>
#define SLAVE 4 // 슬레이브 주소

//배열 순서대로 도,레,미,파,솔,라,시,도
int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};

// Servo
Servo servo;      //Servo 클래스로 servo객체 생성
int degree_value = 0;    // 각도를 조절할 변수 value


SoftwareSerial BTSerial(8, 7); //보드의 RX, TX     //그 BTS아님….bluetooth serial약자임….
char buffer[300];
int buffer_cnt = 0;
int search_cnt = 1;

int col[2] = { 0,0 };

int animal_num = 0;
String animal_data[5][2];

void setup() {
   // string 초기화
   for (int i = 0; i < 5; i++) {
      animal_data[i][0] = "";
      animal_data[i][1] = "";
   }
   // 블루투
   Serial.begin(9600);
   BTSerial.begin(9600);
   // 서보
  pinMode(10,OUTPUT);
   servo.attach(10);     //servo 서보모터 7번 핀에 연결
   //i2c
   Wire.begin();
   Serial.begin(9600);
}

void i2c_sender(char c) { // 슬레이브로 데이터 전송
   Wire.beginTransmission(SLAVE); // 전송 시작
   Wire.write(c);
   Wire.endTransmission(SLAVE); // 실제 데이터 전송
}

void servo_move(int val) {
   servo.write(val);
   delay(50);
}

void loop() {
   if (BTSerial.available()) {      //블루투스 모듈에서 신호가 입력되면
      char test = BTSerial.read();    //아두이노 시리얼로 출력해라
      i2c_sender(test);
    Serial.print(test);
      if (test == ';') {
    buffer[buffer_cnt] = ';';
      buffer_cnt = 0;
    search_cnt = 0;
      // 파싱 슈루룩
      while (buffer[search_cnt] <= 48 || buffer[search_cnt] >= 57) {  //숫자면 인덱스 멈추기
         search_cnt++;
      }
      animal_num = buffer[search_cnt++] - '0';

    int idx = -1;
    // while(buffer[idx++]!=';'){
    //   Serial.print(idx);
    //   Serial.print(":");
    //   Serial.print(buffer[idx]);
    // }

         if (animal_num == 0) {
            animal_data[0][0] = "None";
            animal_data[0][0] = "0";
         }
         else {
            for (int animal = 0; animal < animal_num; animal++) {
               String name = "";
               String degree = "";
               animal_data[animal][0] = "";
               animal_data[animal][1] = "";

               if (buffer[search_cnt] == ',') {
                  col[0] = search_cnt;  //start col
                  while (buffer[++search_cnt] != ',') {
              //idx skipper
                  }
                  col[1] = search_cnt;  //end col
                  // 이름 가져오기
                  for (int find_idx = col[0] + 1; find_idx < col[1]; find_idx++) {
                     name += buffer[find_idx];
                  }
                  name[col[1]] = '\0';
               }

               if (buffer[search_cnt] == ',') {
                  col[0] = search_cnt;  //start col
                  while (buffer[++search_cnt] != ',') {

                  }
                  col[1] = search_cnt;  //end col
                  // 이름 가져오기
                  for (int find_idx = col[0] + 1; find_idx < col[1]; find_idx++) {
                     degree += buffer[find_idx];
                  }
                  degree[col[1]] = '\0';
                  Serial.println("||||||||");
                  Serial.println(name);
                  Serial.println(degree.toInt());

               }
               animal_data[animal][0] = name;
               animal_data[animal][1] = String(degree.toInt());
               int degree_data = animal_data[animal][1].toInt();
               Serial.println(degree_data);
            }
      // 동물의 숫자가 0이 아닐 때 할 행동
        // 첫 번쨰 동물만 본다고 했을 때
        //animal_data[0][0] >> 이름
        //animal_data[0][1] >> 각도
        } 
      
        if(animal_num != 0){
          int degree_servo = -animal_data[0][1].toInt() + 90;
          Serial.print(degree_servo);
          servo_move(degree_servo+10);
          
          for (int i = 0; i < 2; i++) {
            tone(11, melody[i], 250);  //tone함수를 이용해 11번핀으로 주파수 연주
            delay(100);    
            noTone(8);      //음 종료
          }
          }
      }
      else {
         buffer[buffer_cnt++] = test;
      }
   }
}