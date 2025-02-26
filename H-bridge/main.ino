#include <Servo.h>

Servo myservo;
int pos = 0;

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

void autoMode() {
  int speedValue = 100;
  for (int i = 0; i < 5; i++) {
    moveServo(speedValue, true);
    moveServo(speedValue, false);
    speedValue -= 20;
    if (speedValue < 0) speedValue = 0;
  }
}

void manualMode() {
  Serial.println("Введите скорость (0-100) и направление (0 - назад, 1 - вперед):");
  while (true) {
    if (Serial.available()) {
      int speedValue, direction;
      if (sscanf(Serial.readStringUntil('\n').c_str(), "%d %d", &speedValue, &direction) == 2) {
        if (speedValue >= 0 && speedValue <= 100 && (direction == 0 || direction == 1)) {
          moveServo(speedValue, direction);
        } else {
          Serial.println("Ошибка: Введите корректные значения.");
        }
      } else {
        Serial.println("Ошибка ввода. Повторите попытку.");
      }
      Serial.println("Введите новую команду или перезапустите для выхода из ручного режима.");
    }
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
    autoMode();
  } else if (mode == 2) {
    manualMode();
  } else {
    Serial.println("Неверный режим. Повторите ввод.");
  }
}
