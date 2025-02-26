#include <Servo.h>

Servo myservo;
int pos = 0;   // Начальная позиция

void moveServo(int speedValue, bool direction) {
  int step = map(speedValue, 0, 100, 1, 5);
  
  if (direction) {
    for (pos = 0; pos <= 180; pos += step) {
      myservo.write(pos);
      delay(map(speedValue, 0, 100, 50, 5));
    }
  } else {
    for (pos = 180; pos >= 0; pos -= step) {
      myservo.write(pos);
      delay(map(speedValue, 0, 100, 50, 5));
    }
  }
}

void userInputControl() {
  Serial.println("Введите скорость (0-100) и направление (0 - назад, 1 - вперед):");
  while (!Serial.available());
  int speedValue;
  bool direction;
  if (sscanf(Serial.readStringUntil('\n').c_str(), "%d %d", &speedValue, &direction) == 2) {
    moveServo(speedValue, direction);
  } else {
    Serial.println("Ошибка ввода. Повторите попытку.");
  }
}

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  Serial.println("Выберите режим: 1 - Авто, 2 - Ручной ввод");
}

void loop() {
  while (!Serial.available());
  int mode = Serial.readStringUntil('\n').toInt();
  
  if (mode == 1) {
    for (int i = 0; i < 5; i++) {
      moveServo(100, true);
      moveServo(100, false);
    }
  } else if (mode == 2) {
    userInputControl();
  } else {
    Serial.println("Неверный режим. Повторите ввод.");
  }
}
