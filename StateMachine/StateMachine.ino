#include <SoftwareSerial.h>

#define CD 15 // нижняя граница коридора
#define FD 30 // верхняя граница коридора

//////////////////////////////////

#define H1 4 // direction
#define H2 7 // direction

#define E1 5 // speed PWM
#define E2 6  // speed PWM

bool forwad_calibrated = false;

bool  LSF = true;
bool  RSF = false;
bool LSB = false;
bool RSB = true;

int iteration = 1;

int speed_left_ratio = 0;
int speed_right_ratio = 0;

int rotation_time_90 = 100;


// Дальномеры
#define PIN_TRIG_LEFT 8
#define PIN_ECHO_LEFT 9

#define PIN_TRIG_FRONT скока
#define PIN_ECHO_FRONT скока

//////////////////////////////////

char state = "STOP";
// States:
  // STOP
  // FORWARD
  // TURN_LEFT
  // TURN_RIGHT
  // ROTATE_LEFT
  // ROTATE_RIGHT

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {}
  // Serial.println("Goodnight, PC!");

  BTSerial.begin(9600);
  // BTSerial.println("Hello, phone?");

  pinMode(H1, OUTPUT);
  pinMode(H2, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
}

void loop() {
  int distance_front = check_left_distance();
  int distance_left = check_front_distance();




  switch(state) {
    case "STOP":
      if (distance_front > CD) { // Спереди пусто, слева стена
        if (distance_left > CD && distance_left < FD) {
          // Едем вперёд
          go_forward(100);
          state = "FORWARD";
        }
        else if (distance_left > FD) { // Слева и спереди ничего нет
          // Поворачиваем налево на 90* 
          rotate_deg(false);
          state = "ROTATE_LEFT";
        }
      }
      else if (distance_left < FD) { // Если спереди препятствие, слева стена 
        // Поворачиваем направо на 90*
        rotate_deg(true);
        state = "ROTATE_RIGHT";
      }
      break;

    case "FORWARD":
      if (distance_front < CD) { // спереди стена
        // STOP
        stop();
        state = "STOP";
      }
      else if (distance_left > FD) { // слева нет стены
        // Плавный поворот налево
        turn_left(100, 0.5);
        state = "TURN_LEFT";
      }
      else if (distance_left < CD) { // Прижались к стене
        // Плавный попворт направо
        turn_right(100, 0.5);
        state = "TURN_RIGHT";
      }
      break;
    
    case "TURN_LEFT":
      if (distance_front < CD) { // спереди стена
        // STOP
        stop();
        state = "STOP";
      }
      else if (distance_left < FD) { // Попали в коридор
        // Едем вперёд
        go_forward(100);
        state = "FORWARD";
      } 
      break;
    
    case "TURN_RIGHT":
      if (distance_front < CD) { // Спереди стена
        // STOP
        stop();
        state = "STOP";
      }
      else if (distance_left > CD) // Попали в коридор
      {
        // Едем вперёд
        go_forward(100);
        state = "FORWARD";
      }
      break;

      case "ROTATE_LEFT":
        // Едем вперёд
        go_forward(100);
        state = "FORWARD";
        break;
      case "ROTATE_RIGHT":
        // Едем вперёд
        go_forward(100);
        state = "FORWARD";
        break;
  }

}


void move(bool dir_left, int speed_left,
          bool dir_right, int speed_right) {
  digitalWrite(H1, dir_left);
  analogWrite(E1, speed_left);

  digitalWrite(H2, dir_right);
  analogWrite(E2, speed_right);
}

void stop() {
  move(0, 0, 0, 0);
}

void go_forward(int speed) {
  move(LSF, speed, RSF, speed);
}

void turn_left(int speed, float ratio) {
  move(LSF, int(speed*ratio), RSF, speed);
}

void turn_right(int speed, float ratio) {
  move(LSF, speed, RSF, int(speed*ratio));
}

void turn_left_onspot(int speed) {
  move(LSB, speed, RSF, speed);
}

void turn_right_onspot(int speed) {
  move(LSF, speed, RSB, speed);
}

void rotate_deg(bool dir) {
  if (dir) {
    turn_right_onspot(200);
  }
  else {
    turn_left_onspot(200);
  }
  long start_time = millis();
  while (millis() - start_time < rotation_time_90) {}
  stop();
}

int check_left_distance() {
  digitalWrite(PIN_TRIG_LEFT, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG_LEFT, LOW);
  long duration = pulseIn(PIN_ECHO_LEFT, HIGH);

  // Теперь осталось преобразовать время в расстояние
  int cm = (duration / 2) / 29.1;

  return cm;
}

int check_front_distance() {
  digitalWrite(PIN_TRIG_FRONT, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG_FRONT, LOW);
  long duration = pulseIn(PIN_ECHO_FRONT, HIGH);

  // Теперь осталось преобразовать время в расстояние
  int cm = (duration / 2) / 29.1;

  return cm;
}
