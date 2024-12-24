#define DATA_LEVEL LOW
#define SPACE_LEVEL HIGH
#define DASH_DURATION 3
#define DOT_DURATION 1
#define TU 100
#define DATA_PIN 9

String CODES[] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
char LETTERS[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int NLETTERS = 27;

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(DATA_PIN, SPACE_LEVEL);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() >= 1){
    char letter = Serial.read();
    if(letter == ' '){
      send_word();
    }
    else{
      for(int iletter; iletter<NLETTERS; iletter++){
        if(letter == LETTERS[iletter]){
          String code = CODES[iletter];
          send_letter(code);
        }
      }
    }
  }
}

void send_letter(String code){
  int code_len = code.length();
        for(int i=0; i<code_len; i++){
          char symbol = code[i];
          if(symbol == '.'){
            digitalWrite(DATA_PIN, DATA_LEVEL);
            delay(TU);
          }
          if(symbol == '-'){
            digitalWrite(DATA_PIN, DATA_LEVEL);
            delay(DASH_DURATION*TU);
          }
          digitalWrite(DATA_PIN, SPACE_LEVEL);
          delay(TU);
        }
        delay((DASH_DURATION - 1)* TU );
}

void send_word(){
  digitalWrite(DATA_PIN, SPACE_LEVEL);
  delay((DOT_DURATION + 2 * DASH_DURATION) * TU);
  Serial.print(" ");
}
