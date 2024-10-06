#define PIN_LED_1 13
#define PIN_PHOTO_SENSOR_1 A0

#define SET_ON 'u'
#define SET_OFF 'd'
#define SET_PHOTO 'p'

unsigned long previousMillis = 0;
bool isPaused = false;
void pause(unsigned long duration) {
  previousMillis = millis();
  isPaused = true;
  while (millis() - previousMillis < duration) {
  }
  isPaused = false;
}

void getting_command(){
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'u') {
      digitalWrite(PIN_LED_1, HIGH);
    } else if (cmd == 'd') {
      digitalWrite(PIN_LED_1, LOW);
    } else if (cmd == 'p') {
      int val = analogRead(PIN_PHOTO_SENSOR_1);
      byte compressedValue = val / 4;
      Serial.write(compressedValue);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED_1, OUTPUT);
}

void loop() {
  getting_command();
}
