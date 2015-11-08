#include <LiquidCrystalFast.h>

int connected = 1;
long lastNextPress = 0;
long lastPausePress = 0;
long lastUpdate = 0;
LiquidCrystalFast lcd(14, 15, 16, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(13, OUTPUT);
  pinMode(21, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
}

void loop() {
  long now = millis();

  // Check if we are connected.
  if (Serial.dtr()) {
    connected = 1;
  } else if (connected) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("Not Connected");
    connected = 0;
  }

  digitalWrite(13, connected);
  if (!connected) return;
  
  if (!digitalRead(21) && now > lastNextPress + 500)
  {
    Serial.println("next");
    lastNextPress = now;
    lastUpdate = 0;
  }

  if (!digitalRead(22) && now > lastPausePress + 200)
  {
    Serial.println("pause");
    lastPausePress = now;
    lastUpdate = 0;
  }
  
  if (now > lastUpdate + 1000) {
    Serial.println("currentsong");
    Serial.flush();
    delay(10);

    while (Serial.available()) {
      byte sb;

      // Skip to the next newline
      while ((sb = Serial.read()) != '\n');
      
      // Skip the line if it will match nothing
      if (Serial.read() != 'T' ||
          Serial.read() != 'i')
        continue;
      
      // Switch based on the third letter after it
      sb = Serial.read();

      if (sb == 't' &&
          Serial.read() == 'l' &&
          Serial.read() == 'e' &&
          Serial.read() == ':' &&
          Serial.read() == ' ')
      {
        byte b;
        int count = 0;

        lcd.setCursor(0,0);
        while ((b = Serial.read()) != '\n' && count < 16) {
          lcd.write(b);
          count++;
        }
        for (; count < 16; count++) {
          lcd.write(' ');
        }
      }

      if (sb == 'm' &&
          Serial.read() == 'e' &&
          Serial.read() == ':' &&
          Serial.read() == ' ')
      {
        byte b;
        int count = 0;

        lcd.setCursor(10,1);
        lcd.write('/');
        while ((b = Serial.read()) != '\n' && count < 5) {
          lcd.write(b);
          count++;
        }
        for (; count < 5; count++) {
          lcd.write(' ');
        }
      }
    }

    Serial.println("status");
    Serial.flush();
    delay(5);

    while (Serial.available()) {
      byte sb;

      // Skip to the next newline
      while ((sb = Serial.read()) != '\n');
      
      //Switch based on the first letter after it
      sb = Serial.read();
      
      if (sb == 's' &&
          Serial.read() == 't' &&
          Serial.read() == 'a' &&
          Serial.read() == 't' &&
          Serial.read() == 'e' &&
          Serial.read() == ':' &&
          Serial.read() == ' ') {
        byte b1 = Serial.read();
        byte b2 = Serial.read();
        if (b1 == 'p' &&
            b2 == 'l') {
          lcd.setCursor(0, 1);
          lcd.write('P');
        }
        if (b1 == 'p' &&
            b2 == 'a') {
          lcd.setCursor(0, 1);
          lcd.write('p');
        }
        if (b1 == 's' &&
            b2 == 't') {
          lcd.setCursor(0, 1);
          lcd.write('S');
        }
      }

      if (sb == 'e' &&
          Serial.read() == 'l' &&
          Serial.read() == 'a' &&
          Serial.read() == 'p' &&
          Serial.read() == 's' &&
          Serial.read() == 'e' &&
          Serial.read() == 'd' &&
          Serial.read() == ':' &&
          Serial.read() == ' ') {
        byte b;
        int count = 0;

        lcd.setCursor(5, 1);
        while ((b = Serial.read()) != '.' && count < 5) {
          lcd.write(b);
          count++;
        }
        for (; count < 5; count++) {
          lcd.write(' ');
        }
      }
    }
    lastUpdate = now;
  }
}
