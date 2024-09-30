#define PIN_LED_1 13
#define PIN_PHOTO_SENSOR_1 A0
#define PIN_LED_2 10
#define PIN_PHOTO_SENSOR_2 A1

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
  
  if (val_1 - val_2 >= 100) {
    digitalWrite(PIN_LED_1, LOW);
    while(val_1 - val_2 >= 100){
      digitalWrite(PIN_LED_2, HIGH);
      pause(1000);
      digitalWrite(PIN_LED_2, LOW);
      pause(1000);
    }
  } else if (val_2 - val_1 >= 100) {
    digitalWrite(PIN_LED_2, LOW);
    while(val_2 - val_1 >= 100){
      digitalWrite(PIN_LED_2, HIGH);
      pause(1000);
      digitalWrite(PIN_LED_2, LOW);
      pause(1000);
    }
  } else {
    digitalWrite(PIN_LED_2, LOW);
    digitalWrite(PIN_LED_1, LOW);
  } 
}
