#include <Servo.h>

#define ECHO_PIN 2   // Broche ECHO du capteur HC-SR04
#define TRIG_PIN 3   // Broche TRIG du capteur HC-SR04
#define SERVO_PIN 9  // Broche de contrôle du servomoteur

Servo servo;

void setup() {
  Serial.begin(9600);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  
  servo.attach(SERVO_PIN);
  servo.write(0);  // Initialiser la position du servomoteur à 0 degrés
}

void loop() {
  long duration, distance_cm;

  // Envoie une impulsion ultrasonique
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Mesure la durée de l'écho
  duration = pulseIn(ECHO_PIN, HIGH);

  // Convertit la durée en distance
  distance_cm = duration * 0.034 / 2;

  // Affiche la distance mesurée
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  // Condition pour ouvrir la poubelle lorsque la distance atteint 20 cm
  if (distance_cm <= 20) {
    servo.write(90);  // Position du servomoteur pour ouvrir la poubelle
    delay(1000);      // Temps d'ouverture de la poubelle
    servo.write(0);   // Revenir à la position initiale du servomoteur
    delay(1000);      // Temps de fermeture de la poubelle
  }

  delay(1000);
}


