
const int row[8] = {
  13, 18, 9, 16, 2, 8, 3, 6
};

const int col[8] = {
  17, 4, 5, 14, 7, 15, 19, 12
};

const int motionPin = 10;
const int buzzerPin = 11;
const int tapPin = A6;
const int tapThreshold = 100;

int pixels[8][8];

// cursor position:
int x = 3;
int y = 3;
int the_delay = 100;
boolean sticky = false;
boolean tail = false;
int xd = 1;
int yd = 0;
unsigned long timer;
int cherry_x;
int cherry_y;
unsigned frequency = 200;


void setup() {
  pinMode(motionPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(tapPin, INPUT);

  for (int thisPin = 0; thisPin < 8; thisPin++) {
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);

    digitalWrite(col[thisPin], HIGH);
    randomSeed(analogRead(A7));
  }
  timer = millis();
  new_cherry();
}

void loop() {
  if (millis()-timer > the_delay) {
    move();
    timer = millis();
  }
  render();
}
void move() {
  detect_tap();
  int motion = digitalRead(motionPin);
  if (motion == LOW) return;

  if (!sticky) {
    pixels[x][y] = false;
  }

  x = x + xd;
  y = y + yd;
  if (x >= 8 || x < 0) {
    if (x < 0) x = 0;
    if (x > 7) x = 7;
    turn();
  }
  if (y >= 8 || y < 0) {
    if (y < 0) y = 0;
    if (y > 7) y = 7;
    turn();
  }
  pixels[x][y] = true;
  if (cherry_x == x && cherry_y == y) new_cherry();
}
void render() {
  for (int thisRow = 0; thisRow < 8; thisRow++) {
    digitalWrite(row[thisRow], HIGH);
    for (int thisCol = 0; thisCol < 8; thisCol++) {
      if (pixels[thisRow][thisCol]) {
        digitalWrite(col[thisCol], LOW);
      }
    
      digitalWrite(col[thisCol], HIGH);
    }
    digitalWrite(row[thisRow], LOW);
  }
}
void new_cherry() {
  buzz();
  cherry_x = random(0, 8);
  cherry_y = random(0, 8);
  pixels[cherry_x][cherry_y] = true;
}
void detect_tap() {
  int sensorReading = analogRead(tapPin);
  if (sensorReading > tapThreshold) {
    delay(100); // debounce
    turn();
  }
}
void turn() {
  xd = random(-1, 2);
  yd = random(-1, 2);
  if (xd == 0 && yd == 0) xd = 1;

  the_delay = random(10, 300);
}
void buzz() {
  tone(buzzerPin, frequency);
  delay(100);
  noTone(buzzerPin);
  frequency += 50;
  if (frequency >= 20000) frequency = 20000;
}
