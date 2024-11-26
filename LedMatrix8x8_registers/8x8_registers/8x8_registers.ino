#define STROKE_LED_1 1
#define STROKE_LED_0 0
#define STROKE_LED_2 2
#define STROKE_LED_3 3
#define STROKE_LED_4 4
#define STROKE_LED_5 5
#define STROKE_LED_6 6
#define STROKE_LED_7 7

#define Column_LED_2 8
#define Column_LED_3 9
#define Column_LED_4 10
#define Column_LED_5 11
#define Column_LED_6 12
#define Column_LED_7 13
#define Column_LED_A5 A4
#define Column_LED_A4 A5

String array[8][8] = {
    {"0", "1", "1", "0", "0", "1", "1", "0"},
    {"1", "1", "1", "1", "1", "1", "1", "1"},
    {"1", "1", "1", "1", "1", "1", "1", "1"},
    {"1", "1", "1", "1", "1", "1", "1", "1"},
    {"0", "1", "1", "1", "1", "1", "1", "0"},
    {"0", "0", "1", "1", "1", "1", "0", "0"},
    {"0", "0", "0", "1", "1", "0", "0", "0"},
    {"0", "0", "0", "1", "1", "0", "0", "1"},
};

int currentRow = 0;

void setup() {
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 100;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);
  sei();

  DDRD |= (1 << 0);  
  DDRD |= (1 << 1);
  DDRD |= (1 << 2);
  DDRD |= (1 << 3);
  DDRD |= (1 << 4);
  DDRD |= (1 << 5);
  DDRD |= (1 << 6);
  DDRD |= (1 << 7);   
  
  DDRB |= (1 << 0);
  DDRB |= (1 << 1);
  DDRB |= (1 << 2);
  DDRB |= (1 << 3);
  DDRB |= (1 << 4);
  DDRB |= (1 << 5);
  DDRC |= (1 << 4);
  DDRC |= (1 << 5);

  
}

ISR(TIMER1_COMPA_vect) {
  PORTD &= ~(0xFF);
  PORTB &= ~(0xFF);
  PORTC &= ~((1 << 4) | (1 << 5));
  PORTD |= (1 << currentRow);
  
  for (int i = 0; i < 8; i++) {
    int ledState = (array[currentRow][i] == "1") ? 1 : 0;

    if (ledState == 1) {
      if (i == 6) {
        PORTC |= (1 << 4);
      }
      else if (i == 7) {
        PORTC |= (1 << 5);
      }
      else {
        PORTB |= (1 << i);
        }
    } else {
      if (i == 6) {
        PORTC &= ~(1 << 4);
      }
      else if (i == 7) {
        PORTC &= ~(1 << 5);
      }
      else {
        PORTB &= ~(1 << i);
      }
    }
  }

  currentRow = (currentRow + 1) % 8;
}

void loop() {
}
