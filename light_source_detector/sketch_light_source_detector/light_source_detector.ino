#define PIN_LED_1 13
#define PIN_PHOTO_SENSOR_1 A0
#define PIN_LED_2 10
#define PIN_PHOTO_SENSOR_2 A1

const int sensitivity = 100;
bool isBlinking = false;

bool led1State = LOW;
bool led2State = LOW;

unsigned long previousMillis = 0;
bool isPaused = false;
void pause(unsigned long duration) {
  previousMillis = millis();
  isPaused = true;
  while (millis() - previousMillis < duration) {
  }
  isPaused = false;
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
}

void loop() {
  int val_1 = analogRead(PIN_PHOTO_SENSOR_1);
  int val_2 = analogRead(PIN_PHOTO_SENSOR_2);
  
  if (val_2 - val_1 >= sensitivity) {
    digitalWrite(PIN_LED_1, led1State);
    led1State = !led1State;
    digitalWrite(PIN_LED_2, LOW);
    isBlinking = true;
  } else if (val_1 - val_2 >= sensitivity) {
    digitalWrite(PIN_LED_2, led2State);
    led2State = !led2State;
    digitalWrite(PIN_LED_1, LOW);
    isBlinking = true;
  } else {
    isBlinking = false;
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, HIGH);
  }

  if (isBlinking) {
    pause(1000);
  }

}
