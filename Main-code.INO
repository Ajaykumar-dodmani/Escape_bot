// === Pin Definitions ===
#define trigPin 2
#define echoPin 3
#define irLeft 5
#define irRight 6
#define relayPin 4
#define buzzerPin 7

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(relayPin, LOW);    // Motors OFF at start
  digitalWrite(buzzerPin, LOW);   // Buzzer OFF

  Serial.begin(9600);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;  // in cm
}

void loop() {
  long distance = getDistance();
  int leftEdge = digitalRead(irLeft);   // 1 = edge, 0 = surface
  int rightEdge = digitalRead(irRight); // 1 = edge, 0 = surface

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | IR Left: ");
  Serial.print(leftEdge);
  Serial.print(" | IR Right: ");
  Serial.println(rightEdge);

  if (leftEdge == 1 || rightEdge == 1) {
    // IR edge → STOP
    digitalWrite(relayPin, LOW);    // Motors OFF
    digitalWrite(buzzerPin, LOW);   // Buzzer OFF
    Serial.println("🟥 Edge detected — stopping.");
  }
  else if (distance < 20) {
    // Object detected → ESCAPE!
    digitalWrite(relayPin, HIGH);   // Motors ON
    digitalWrite(buzzerPin, HIGH);  // Buzzer ON
    Serial.println("🚨 Escaping!");
  }
  else {
    // Idle
    digitalWrite(relayPin, LOW);    // Motors OFF
    digitalWrite(buzzerPin, LOW);   // Buzzer OFF
    Serial.println("🟩 Standing by...");
  }

  delay(100);
}
/*
| **Component**       | **Connect To**                                     | **Purpose / Behavior**                     |
| ------------------- | -------------------------------------------------- | ------------------------------------------ |
| Ultrasonic TRIG     | Arduino D2                                         | Sends pulse to measure distance            |
| Ultrasonic ECHO     | Arduino D3                                         | Receives pulse back                        |
| IR Sensor Left OUT  | Arduino D5                                         | Detects left edge (1 = edge, 0 = surface)  |
| IR Sensor Right OUT | Arduino D6                                         | Detects right edge (1 = edge, 0 = surface) |
| Buzzer (+ve)        | Arduino D7                                         | Buzzes when escaping                       |
| Relay IN            | Arduino D4                                         | Controls motor ON/OFF                      |
| Relay VCC           | Arduino 5V                                         | Powers relay logic                         |
| Relay GND           | Arduino GND                                        | Common ground                              |
| Relay COM           | Li-ion +ve                                         | Power input to motor                       |
| Relay NO            | Motor +ve                                          | Power output when relay is ON              |
| Motor –ve           | Li-ion –ve                                         | Motor return path                          |
| Arduino GND         | Shared with IRs, Ultrasonic, Relay, Li-ion –ve     | Required for signal reference              |
| Arduino Power       | 9V battery                                         | Powers all sensors and Arduino itself      |
*/