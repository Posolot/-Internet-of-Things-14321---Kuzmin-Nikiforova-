#include <SoftwareSerial.h>
#define H1 4 
#define H2 7 
#define E1 5 
#define E2 6  
#define LEFT_SIDE_FORWARD HIGH 
#define RIGHT_SIDE_FORWARD HIGH 

bool forwad_calibrated = false;

bool  LSF = true;
bool  RSF = false;
bool LSB = false;
bool RSB = true;

int iteration = 1;

int speed_left_ratio = 0;
int speed_right_ratio = 0;

int rotation_time_90 = 100;
int rotation_time_180 = 150;
int rotation_time_270 = 200;
int rotation_time_360 = 250;

#define LEFT_SIDE_BACKSIDE LOW 
#define RIGHT_SIDE_BACKSIDE LOW 
#define PIN_TRIG 8
#define PIN_ECHO 9
long duration, cm;
SoftwareSerial BTSerial(10, 11);

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  BTSerial.begin(9600);

  pinMode(H1, OUTPUT);
  pinMode(H2, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
}

void calibrate_direction() {
  while(true) {
    if (BTSerial.available() > 0) {
      char value = BTSerial.read();
      if (value == '^') {
        go_forward(100);
      } else if (value == 'q') {
        stop();
      } else if (value == 'A') { 
        return;
      } else if (value == 'Y') {
        if (iteration == 1) { 
          LSF = true;
          RSF = true;
          LSB = false;
          RSB = false;
          iteration++;
        } else if (iteration == 2) {
          LSF = false;
          RSF = true;
          LSB = true;
          RSB = false;
          iteration++;
        } else if (iteration == 3) {
          LSF = false;
          RSF = false;
          LSB = true;
          RSB = true;
          iteration = 1;
        }
      }
    }
  }
}

void calibrate_wheels_speed() {
  while(true) {
    if(BTSerial.available() > 0) {
      char value = BTSerial.read();

      if (value == "^") {
        go_forward(100);
      }
      else if (value == "q") { 
        stop();
      } 
      else if (value == "A") { 
        return;
      }
      else if (value == ">") { 
        speed_left_ratio += 10;
      }
      else if (value == "<") { 
        speed_left_ratio -= 10;
      }
    }
  }
}

void calibrate_rotation() {
  int step = 1;
  while(true) {
    if (step == 5) { 
      return;
    }
    if (BTSerial.available() > 0) {
      char value = BTSerial.read();
      if (value == "R") {
        rotate_deg(90 * step);
      }
      else if (value == "A") {
        step += 1;
      }
      else if (value == ">") {
        if (step == 1) {
          rotation_time_90 += 20;
        }
        if (step == 2) {
          rotation_time_180 += 20;
        }
        if (step == 3) {
          rotation_time_270 += 20;
        }
        if (step == 4) {
          rotation_time_360 += 20;
        }
      }
      else if (value == "<") { 
        if (step == 1) {
          rotation_time_90 -= 20;
        }
        if (step == 2) {
          rotation_time_180 -= 20;
        }
        if (step == 3) {
          rotation_time_270 -= 20;
        }
        if (step == 4) {
          rotation_time_360 -= 20;
        }
      }
    }
  }
}

void calibration() {
  calibrate_direction();
  calibrate_wheels_speed();
  calibrate_rotation();
}


void move(bool dir_left, int speed_left,
          bool dir_right, int speed_right) {
  
  speed_left += speed_left_ratio; 
  speed_right += speed_right_ratio;

  digitalWrite(H1, dir_left);
  analogWrite(E1, speed_left);

  digitalWrite(H2, dir_right);
  analogWrite(E2, speed_right);
}

void go_forward(int speed) {
  move(LSF, speed, RSF, speed);
}

void go_backside(int speed) {
  move(LEFT_SIDE_BACKSIDE, speed, RIGHT_SIDE_BACKSIDE, speed);
}

void move_only_left(int speed) {
  move(LEFT_SIDE_BACKSIDE, 0, RIGHT_SIDE_FORWARD, speed);
}

void move_only_right(int speed) {
  move(LEFT_SIDE_FORWARD, speed, LEFT_SIDE_BACKSIDE, 0);
}

void turn_left(int speed, float ratio) {
  move(LEFT_SIDE_FORWARD, int(speed*ratio), RIGHT_SIDE_FORWARD, speed);
}

void turn_right(int speed, float ratio) {
  move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_FORWARD, int(speed*ratio));
}

void turn_left_onspot(int speed) {
  move(LEFT_SIDE_BACKSIDE, speed, RIGHT_SIDE_FORWARD, speed);
}

void turn_right_onspot(int speed) {
  move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_BACKSIDE, speed);
}

void rotate_deg(int degree) {
  long rotation_time = 0;
  if (degree == 90) {
    rotation_time = rotation_time_90;
  } 
  else if (degree == 180) {
    rotation_time = rotation_time_180;
  }
  else if (degree == 270) {
    rotation_time = rotation_time_270;
  }
  else {
    rotation_time = rotation_time_360;
  }
  turn_right_onspot(200);
  long start_time = millis();
  while (millis() - start_time < rotation_time) {
  }
  stop();
}

void stop() {
  move(0, 0, 0, 0);
}

void loop() {
  calibration();
  stop();
}
