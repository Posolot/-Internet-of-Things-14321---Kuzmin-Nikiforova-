#define FORWARD_PIN 5
#define BACKWARD_PIN 6
#define PWM_PIN 9

int speed = 200;
bool direction = true;
void setup() {
  Serial.begin(9600);

  pinMode(FORWARD_PIN, OUTPUT);
  pinMode(BACKWARD_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  Serial.println("You can input speed and direction like this:");
  Serial.println("100 0");
  Serial.println("where 100 is speed, 0 or 1 is derection (1 forward, 0 backward)");
}

void loop() {
  if (Serial.available() > 0) {
    speed = Serial.parseInt();
    direction = Serial.parseInt();
  }
  move();
}

void move() {
  if (direction) {
    digitalWrite(FORWARD_PIN, HIGH);
    digitalWrite(BACKWARD_PIN, LOW);
    analogWrite(PWM_PIN, speed);
  } else {
    digitalWrite(FORWARD_PIN, LOW);
    digitalWrite(BACKWARD_PIN, HIGH);
    analogWrite(PWM_PIN, speed);
  }
}
