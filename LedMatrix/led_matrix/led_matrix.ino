#define PIN_LED_3 3
#define PIN_LED_4 4
#define PIN_LED_5 5
#define PIN_LED_6 6
#include <ArduinoJson.h>

String inputText;
String ledState;
StaticJsonDocument<200> bincode;

unsigned long previousMillis = 0;
bool isPaused = false;
void pause(unsigned long duration) {
  previousMillis = millis();
  isPaused = true;
  while (millis() - previousMillis < duration) {
  }
  isPaused = false;
}
void parcer(String ledState) {
  String val1 = ledState.substring(0, 4);
  String val2 = ledState.substring(4, 8);

  digitalWrite(PIN_LED_3, (val1[0] == '1') ? HIGH : LOW);
  digitalWrite(PIN_LED_4, (val1[1] == '1') ? HIGH : LOW);
  digitalWrite(PIN_LED_5, (val1[2] == '1') ? HIGH : LOW);
  digitalWrite(PIN_LED_6, (val1[3] == '1') ? HIGH : LOW);
  pause(10);
  digitalWrite(PIN_LED_3, (val2[0] == '1') ? HIGH : LOW);
  digitalWrite(PIN_LED_4, (val2[1] == '1') ? HIGH : LOW);
  digitalWrite(PIN_LED_5, (val2[2] == '1') ? HIGH : LOW);
  digitalWrite(PIN_LED_6, (val2[3] == '1') ? HIGH : LOW);
  pause(10);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Введите, какие пины необходимо включить (например, 1,3,4):");
  bincode["off"] = "00000000";       
  bincode["on"] = "11000000";      
  bincode["1"] = "10010000";        
  bincode["2"] = "10100000";       
  bincode["3"] = "01010000";       
  bincode["4"] = "01100000";        
  bincode["1,2"] = "10000000";      
  bincode["2,4"] = "11100000";      
  bincode["3,4"] = "01000000";      
  bincode["1,3"] = "11010000";     
  bincode["1,4"] = "10010110";      
  bincode["2,3"] = "10100101";      
  bincode["1,2,4"] = "10000110";    
  bincode["2,3,4"] = "10100100";   
  bincode["1,3,4"] = "01001001";
  bincode["1,2,3"] = "10000101";

  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
  pinMode(PIN_LED_5, OUTPUT);
  pinMode(PIN_LED_6, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    inputText = Serial.readStringUntil('\n');
    if (bincode.containsKey(inputText)) {
      ledState = bincode[inputText].as<String>();
      Serial.print("Состояние LED: ");
      Serial.println(ledState);
      parcer(ledState);
    } else {
      Serial.println("Ошибка: Неверный ввод.");
    }
  }
  if (ledState.length() > 0) {
    parcer(ledState);
  }
}
