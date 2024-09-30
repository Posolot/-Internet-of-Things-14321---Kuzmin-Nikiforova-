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
unsigned long previousMillis = 0;
bool isPaused = false;
unsigned long manualModeStartTime = 0; // время начала ручного режима
bool manualModeTimeout = false;

void pause(unsigned long duration) {
  previousMillis = millis();
  isPaused = true;
  while (millis() - previousMillis < duration) {
  }
  isPaused = false;
}

void update_mode() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == MODE_MANUAL) {
      isAutoMode = false;
      digitalWrite(LED_PIN_GREEN, LOW);
      digitalWrite(LED_PIN_YELLOW, LOW);
      digitalWrite(LED_PIN_RED, LOW); 
      Serial.println("Manual mode activated");
      manualModeStartTime = millis();
      manualModeTimeout = false;
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
    pause(10000);
    for (int i = 0; i < 4; i++) {
      digitalWrite(LED_PIN_GREEN, HIGH);
      pause(250);
      digitalWrite(LED_PIN_GREEN, LOW);
      pause(250);
    }
    autoState = 1;
  } else if (autoState == 1) {
    digitalWrite(LED_PIN_GREEN, LOW);
    digitalWrite(LED_PIN_YELLOW, HIGH);
    digitalWrite(LED_PIN_RED, LOW);
    pause(1000);
    autoState = 2;
  } else if (autoState == 2) {
    digitalWrite(LED_PIN_YELLOW, LOW); 
    digitalWrite(LED_PIN_RED, HIGH);
    pause(7000);
    autoState = 3;
  }
  else if(autoState==3){
    digitalWrite(LED_PIN_GREEN, LOW);
    digitalWrite(LED_PIN_YELLOW, HIGH);
    digitalWrite(LED_PIN_RED, LOW);
    pause(1000);
    autoState = 0;
  }
}

void process_manual_mode() {
  unsigned long currentMillis = millis();
  if (currentMillis - manualModeStartTime >= 60000) {
    Serial.println("Manual mode timeout. Returning to automatic mode.");
    isAutoMode = true;
    return;
  }

  if (manualCommand == SET_GREEN_ON) {
    if (digitalRead(LED_PIN_RED) == HIGH) {
      digitalWrite(LED_PIN_RED, LOW);
      digitalWrite(LED_PIN_YELLOW, HIGH);
      pause(3000);
    }
    digitalWrite(LED_PIN_YELLOW, LOW);
    digitalWrite(LED_PIN_GREEN, HIGH);
  } else if (manualCommand == SET_RED_ON) {
    if (digitalRead(LED_PIN_GREEN) == HIGH) {
      digitalWrite(LED_PIN_GREEN, LOW);
      digitalWrite(LED_PIN_YELLOW, HIGH);
      pause(3000);
    }
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
