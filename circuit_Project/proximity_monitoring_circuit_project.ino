/*
 * Sistema embebido de monitoreo de proximidad y alerta multimodal
 * Universidad de San Buenaventura, sede Bogota
 * Plataforma: Arduino Uno (ATmega328P)
 *
 * A1 -> HC-SR04 Trig              D2 -> LED verde (220 ohm)
 * A2 -> HC-SR04 Echo              D3 -> LED rojo (220 ohm)
 * A0 -> Potenciometro (duracion)  D4 -> Zumbador piezoelectrico
 *                                 D5 -> Pulsador con pull-down
 */
const int trigPin     = A1;
const int echoPin     = A2;
const int greenLedPin = 2;
const int redLedPin   = 3;
const int piezoPin    = 4;
const int buttonPin   = 5;
const int potPin      = A0;

// Distance threshold (cm) to enter / exit the "inside" state
const int enterThreshold = 5;
const int exitThreshold  = 5;

bool isInside = false;
bool wasInside = false;
bool firstReading = true;

// Buzzer mute/unmute button with debounce
bool buzzerEnabled = true;
int buttonState = LOW;
int lastButtonReading = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 150;

// Non-blocking buzzer: it is turned off with millis(), not delay()
unsigned long buzzerStartTime = 0;
unsigned long activeBeepDuration = 1000;
bool isBeeping = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT);   // External pull-down resistor used
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(piezoPin, OUTPUT);

  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);

  Serial.begin(9600);
}

void loop() {
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        buzzerEnabled = !buzzerEnabled;
        if (!buzzerEnabled) {
          noTone(piezoPin);
          isBeeping = false;
        }
      }
    }
  }
  lastButtonReading = reading;

  // The potentiometer sets the alert duration (0.3 s to 3 s)
  int potValue = analogRead(potPin);
  unsigned long selectedDuration = map(potValue, 0, 1023, 300, 3000);

  if (isBeeping && (millis() - buzzerStartTime >= activeBeepDuration)) {
    noTone(piezoPin);
    isBeeping = false;
  }

  long distance = measureDistanceCM(trigPin, echoPin);

  if (distance > 0 && distance <= enterThreshold) {
    isInside = true;
  } else if (distance >= exitThreshold) {
    isInside = false;
  }

  // Telemetry over UART
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Set Duration: ");
  Serial.print(selectedDuration / 1000.0, 1);
  Serial.print(" s | Buzzer: ");
  Serial.print(buzzerEnabled ? "ENABLED" : "DISABLED");
  Serial.print(" -> ");
  Serial.println(isInside ? "RED LED" : "GREEN LED");

  // firstReading avoids a false alert at startup
  if (firstReading) {
    wasInside = isInside;
    firstReading = false;
  } else if (isInside != wasInside) {
    if (isInside) {
      digitalWrite(redLedPin, HIGH);
      digitalWrite(greenLedPin, LOW);
    } else {
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, HIGH);
    }

    if (buzzerEnabled) {
      activeBeepDuration = selectedDuration;
      tone(piezoPin, 1000);
      buzzerStartTime = millis();
      isBeeping = true;
    }

    wasInside = isInside;
  }

  delay(50);
}

long measureDistanceCM(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);   // 10 us trigger pulse for the HC-SR04
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 20000);   // 20 ms timeout
  if (duration == 0) return 999;
  return duration / 29 / 2;
}
