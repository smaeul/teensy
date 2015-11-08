#include <limits.h>

#define DELAY_USECS 1000000
#define DIGITS 4
#define SEGMENTS 7

int currentValues[DIGITS] = { 0, 0, 0, 0 };
int font[10][SEGMENTS] = {
  { 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 0, 1, 1, 0, 1 },
  { 1, 1, 1, 1, 0, 0, 1 },
  { 0, 1, 1, 0, 0, 1, 1 },
  { 1, 0, 1, 1, 0, 1, 1 },
  { 1, 0, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 0, 0, 1, 1 }
};
unsigned long lastChange = 0;
int numericValue = 0;
int pinDigits[DIGITS] = { 9, 10, 11, 12 };
int pinSegments[SEGMENTS] = { 1, 2, 3, 4, 5, 6, 7 };

void setup() {
  int digit, segment;

  for (digit = 0; digit < DIGITS; ++digit) {
    pinMode(pinDigits[digit], OUTPUT);
    digitalWrite(pinDigits[digit], 1);
  }
  for (segment = 0; segment < SEGMENTS; ++segment) {
    pinMode(pinSegments[segment], OUTPUT);
    digitalWrite(pinSegments[segment], 0);
  }
}

void loop() {
  int digit, segment;
  unsigned long usecs;
  
  for (digit = 0; digit < DIGITS; digit++) {
    for (segment = 0; segment < SEGMENTS; segment++) {
      digitalWrite(pinSegments[segment], font[currentValues[digit]][segment]);
    }
    digitalWrite(pinDigits[digit], 0);
    delay(3);
    digitalWrite(pinDigits[digit], 1);
  }
  
  usecs = micros();
  if (usecs - lastChange >= DELAY_USECS || ULONG_MAX - lastChange + usecs >= DELAY_USECS) {
    int minutes, seconds;
    
    numericValue++;
    seconds = numericValue % 60;
    minutes = numericValue / 60;
    currentValues[3] = seconds % 10;
    currentValues[2] = seconds / 10;
    currentValues[1] = minutes % 10;
    currentValues[0] = minutes / 10;
    lastChange += DELAY_USECS;
  }
}
