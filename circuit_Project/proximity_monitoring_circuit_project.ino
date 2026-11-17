 /*
 * D2 -> Sensor PING))) (SIG)     D5 -> LED rojo (220 ohm)
 * D3 -> Pulsador con pull-down   D6 -> Zumbador piezoelectrico
 * D4 -> LED verde (220 ohm)      A0 -> Potenciometro (umbral)
 */
const int ultrasonicPin = 2;
const int buttonPin     = 3;
const int greenLedPin   = 4;
const int redLedPin     = 5;
const int piezoPin      = 6;
const int potPin        = A0;

// State variables for threshold crossing
bool wasInside = false;
bool firstReading = true;

// State variables for buzzer toggle button (Mute / Unmute)
bool buzzerEnabled = true;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;

void setup() {
  pinMode(buttonPin, INPUT);   // External pull-down resistor used
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(piezoPin, OUTPUT);

  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);

  Serial.begin(9600);
}

void loop() {
  // Pushbutton (external pull-down): enable or mute ONLY the buzzer
  int buttonReading = digitalRead(buttonPin);
  if (buttonReading == HIGH && lastButtonState == LOW && (millis() - lastDebounceTime > 50)) {
    buzzerEnabled = !buzzerEnabled;
    lastDebounceTime = millis();

    if (!buzzerEnabled) {
      noTone(piezoPin);
    }
  }
  lastButtonState = buttonReading;

  long distance = measureDistanceCM(ultrasonicPin);

  // Read the potentiometer to adjust the threshold (from 0 cm to 100 cm)
  int potValue = analogRead(potPin);
  int threshold = map(potValue, 0, 1023, 0, 100);

  // Determine if the target is within the threshold
  bool isInside = (distance > 0 && distance <= threshold);

  // Display telemetry on the Serial Monitor (UART protocol)
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Threshold: ");
  Serial.print(threshold);
  Serial.print(" cm | Buzzer: ");
  Serial.print(buzzerEnabled ? "ON" : "MUTED");
  Serial.print(" -> ");
  Serial.println(isInside ? "INSIDE (RED)" : "OUTSIDE (GREEN)");

  // LED control (always active regardless of buzzer state)
  if (isInside) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  }

  // Detect if the target just ENTERED or EXITED the threshold
  if (firstReading) {
    wasInside = isInside;
    firstReading = false;
  } else if (isInside != wasInside) {
    // Sound the buzzer (tone 125 ms + 300 ms wait, < 1 s) ONLY if buzzerEnabled is true
    if (buzzerEnabled) {
      tone(piezoPin, 1000, 125);
      delay(300);
      noTone(piezoPin);
    }
    wasInside = isInside;
  }

  delay(50);
}

long measureDistanceCM(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5); // Minimum 5 us for the sensor to trigger
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  long duration = pulseIn(pin, HIGH, 30000); // 30000 us timeout
  if (duration == 0) return 999;
  return duration / 29 / 2;
}
