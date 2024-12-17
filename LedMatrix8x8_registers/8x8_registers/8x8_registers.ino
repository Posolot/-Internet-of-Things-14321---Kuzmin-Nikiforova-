const byte images[][8] = {
  {
    B10000001,
    B00000000,
    B00111100,
    B00111111,
    B10000111,
    B11100111,
    B11111111,
    B11100111
  },
  {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  }
};

int currentRow = 0;
int currentImage = 0;
volatile int newImage = 0;

void setup() {
  cli();
  DDRD = B11111111;
  DDRB = B00111111;
  DDRC = B00110000;

  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2B = TCCR2B | (1 << CS12);
  TIMSK2 = TIMSK2 | (1 << TOIE2); 
  sei();
}

ISR(TIMER2_OVF_vect) {
  row_off();
  PORTD |= (1 << currentRow);
  newImage++;

  for (int col = 0; col < 8; col++) {
    int ledState = (images[currentImage][currentRow] >> col) & 1;

    if (ledState == 1) {
      if (col < 6) {
        PORTB |= (1 << col);
      } else if (col == 6) {
        PORTC |= (1 << 4);
      } else if (col == 7) {
        PORTC |= (1 << 5);
      }
    } else {
      if (col < 6) {
        PORTB &= ~(1 << col);
      } else if (col == 6) {
        PORTC &= ~(1 << 4);
      } else if (col == 7) {
        PORTC &= ~(1 << 5);
      }
    }
  }

  currentRow = (currentRow + 1) % 8;

  if (newImage >= 2000) {
    currentImage = (currentImage == 0) ? 1 : 0;
    newImage = 0;
  }
}

void row_off() {
  PORTD = B00000000;
  PORTB = B00000000;
  PORTC = B00000000;
}

void loop() {
}
