#define STROKE_LED_2 9
#define STROKE_LED_3 6
#define STROKE_LED_4 10
#define STROKE_LED_5 12
#define STROKE_LED_6 4
#define STROKE_LED_7 7
#define Column_LED_2 11
#define Column_LED_3 3
#define Column_LED_4 13
#define Column_LED_5 5
#define Column_LED_6 8
#define Column_LED_7 2
#define STROKE_LED_A5 A5
#define STROKE_LED_A4 A4
#define Column_LED_B1 1
#define Column_LED_B0 0

String array[8][8] = {
    {"0", "1", "0", "0", "1", "0", "0", "0"},
    {"1", "1", "1", "1", "1", "1", "0", "0"},
    {"1", "1", "1", "1", "1", "1", "0", "0"},
    {"0", "1", "1", "1", "1", "0", "0", "0"},
    {"0", "0", "1", "1", "0", "0", "0", "0"},
    {"0", "0", "0", "0", "0", "1", "1", "1"},
    {"0", "0", "0", "0", "0", "1", "0", "1"},
    {"0", "0", "0", "0", "0", "1", "1", "1"},
};
unsigned long previousMillis = 0;
bool isPaused = false;
int currentRow = 0;
void pause(unsigned long duration) {
  previousMillis = millis();
  isPaused = true;
  while (millis() - previousMillis < duration) {
  }
  isPaused = false;
}


void setup() {
  
  pinMode(STROKE_LED_2, OUTPUT);
  pinMode(STROKE_LED_3, OUTPUT);
  pinMode(STROKE_LED_4, OUTPUT);
  pinMode(STROKE_LED_5, OUTPUT);
  pinMode(STROKE_LED_6, OUTPUT);
  pinMode(STROKE_LED_7, OUTPUT);
  pinMode(STROKE_LED_A4, OUTPUT);
  pinMode(STROKE_LED_A5, OUTPUT);

  pinMode(Column_LED_2, OUTPUT);
  pinMode(Column_LED_3, OUTPUT);
  pinMode(Column_LED_4, OUTPUT);
  pinMode(Column_LED_5, OUTPUT);
  pinMode(Column_LED_6, OUTPUT);
  pinMode(Column_LED_7, OUTPUT);
  pinMode(Column_LED_B0, OUTPUT);
  pinMode(Column_LED_B1, OUTPUT);
}


void processRow(String row, int num) {
    switch (num) {
        case 0:
            digitalWrite(STROKE_LED_A5, HIGH);
            break;
        case 1:
            digitalWrite(STROKE_LED_2, HIGH);
            break;
        case 2:
            digitalWrite(STROKE_LED_3, HIGH);
            break;
        case 3:
            digitalWrite(STROKE_LED_4, HIGH);
            break;
        case 4:
            digitalWrite(STROKE_LED_5, HIGH);
            break;
        case 5:
            digitalWrite(STROKE_LED_6, HIGH);
            break;
        case 6:
            digitalWrite(STROKE_LED_7, HIGH);
            break;
        case 7:
            digitalWrite(STROKE_LED_A4, HIGH);
            break;
        default:
            Serial.println("Неверные данные");
    }

  digitalWrite(Column_LED_B0, (row[0] == '0') ? HIGH : LOW);
  digitalWrite(Column_LED_2, (row[1] == '0') ? HIGH : LOW);
  digitalWrite(Column_LED_3, (row[2] == '0') ? HIGH : LOW);
  digitalWrite(Column_LED_4, (row[3] == '0') ? HIGH : LOW);
  digitalWrite(Column_LED_5, (row[4] == '0') ? HIGH : LOW);
  digitalWrite(Column_LED_6, (row[5] == '0') ? HIGH : LOW);
  digitalWrite(Column_LED_7, (row[6] == '0') ? HIGH : LOW);
  digitalWrite(Column_LED_B1, (row[7] == '0') ? HIGH : LOW);
  pause(1);
  digitalWrite(STROKE_LED_2, LOW);
  digitalWrite(STROKE_LED_3, LOW);
  digitalWrite(STROKE_LED_4, LOW);
  digitalWrite(STROKE_LED_5, LOW);
  digitalWrite(STROKE_LED_6, LOW);
  digitalWrite(STROKE_LED_7, LOW);
  digitalWrite(STROKE_LED_A4, LOW);
  digitalWrite(STROKE_LED_A5, LOW);
}

void loop() {
  String row = "";
  for (int i = 0; i < 8; i++) {
    row += array[currentRow][i];
  }
  processRow(row,currentRow);
  currentRow = (currentRow + 1) % 8;
}
