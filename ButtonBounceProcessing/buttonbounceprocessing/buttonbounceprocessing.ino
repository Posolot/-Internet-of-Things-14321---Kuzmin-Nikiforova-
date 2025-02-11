#define BUTTON_PIN 2
#define BUTTON_TIME 50  // Антидребезг (мс)
#define PRESSED HIGH

volatile long press_time = 0;
volatile bool pressed_candidate = false;
volatile long hold_time = 0;
volatile int press_count = 0;
bool button_press = false;
bool button_released = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), process_button_click, RISING);
}

void loop() {
  if (pressed_candidate && millis() - press_time >= BUTTON_TIME) {
    if (digitalRead(BUTTON_PIN) == PRESSED) {
      Serial.println("Button Pressed!");
      button_press = true;
      pressed_candidate = false;
      press_count++;
    }
  }

  if (button_press) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      hold_time = millis() - press_time;

      if (hold_time > BUTTON_TIME) {
        Serial.print("Hold time: ");
        Serial.println(hold_time);
      }

      button_press = false;
      button_released = true;
      Serial.print("Total Presses: ");
      Serial.println(press_count);
    }
  }
  if (button_released) {
    Serial.println("Button Released!");
    button_released = false;
  }
}

void process_button_click() {
  if (!pressed_candidate) {
    press_time = millis();
    pressed_candidate = true;
  }
}
