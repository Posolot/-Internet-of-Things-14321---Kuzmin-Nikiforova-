#define LED_PIN_GREEN 13
#define LED_PIN_YELLOW 12
#define LED_PIN_RED 11

#define SET_GREEN_ON 'g'
#define SET_YELLOW_BLINK 'y'
#define SET_RED_ON 'r'

#define MODE_MANUAL 'm'
#define MODE_AUTO 'a'

bool isAutoMode = true;
int autoState = 0;

char manualCommand = 0;  
int i;
void update_mode() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == MODE_MANUAL) {
      isAutoMode = false;
      digitalWrite(LED_PIN_GREEN, LOW);
      digitalWrite(LED_PIN_YELLOW, LOW);
      digitalWrite(LED_PIN_RED, LOW); 
      Serial.println("Manual mode activated");
    } else if (cmd == MODE_AUTO) {
      isAutoMode = true;
      Serial.println("Automatic mode activated");
    }
    if (!isAutoMode) {
      manualCommand = cmd;
    }
  }
}

void process_auto_mode() {
  if (autoState == 0) {
    digitalWrite(LED_PIN_GREEN, HIGH);
    digitalWrite(LED_PIN_YELLOW, LOW);
    digitalWrite(LED_PIN_RED, LOW);
    delay(2400);
    for (int i = 0; i < 4; i++) {
      digitalWrite(LED_PIN_GREEN, HIGH);
      delay(300);
      digitalWrite(LED_PIN_GREEN, LOW);
      delay(300);
    }
    autoState = 1;
  } else if (autoState == 1) {
    digitalWrite(LED_PIN_GREEN, LOW);
    for (int i = 0; i < 4; i++) {
      digitalWrite(LED_PIN_YELLOW, HIGH);
      delay(400);
      digitalWrite(LED_PIN_YELLOW, LOW);
      delay(400);
    }
    autoState = 2;
  } else if (autoState == 2) {
    digitalWrite(LED_PIN_YELLOW, LOW);  
    digitalWrite(LED_PIN_RED, HIGH);
    delay(3000);
    autoState = 0;
  }
}

void process_manual_mode() {
  if (manualCommand == SET_GREEN_ON) {
    digitalWrite(LED_PIN_GREEN, HIGH);
    digitalWrite(LED_PIN_YELLOW, LOW);
    digitalWrite(LED_PIN_RED, LOW);
  } else if (manualCommand == SET_YELLOW_BLINK) {
    digitalWrite(LED_PIN_GREEN, LOW);
    digitalWrite(LED_PIN_RED, LOW); 
    for (i = 0; i < 4; i++) {
      digitalWrite(LED_PIN_YELLOW, HIGH);
      delay(250);
      digitalWrite(LED_PIN_YELLOW, LOW);
      delay(250);
    }
    digitalWrite(LED_PIN_RED, LOW);
  } else if (manualCommand == SET_RED_ON) {
    digitalWrite(LED_PIN_GREEN, LOW);
    digitalWrite(LED_PIN_YELLOW, LOW);
    digitalWrite(LED_PIN_RED, HIGH);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_YELLOW, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  Serial.println("Automatic mode activated");
}

void loop() {
  update_mode();

  if (isAutoMode) {
    process_auto_mode();
  } else {
    process_manual_mode();
  }
}
