int ledPin = 7;     // Argon's onboard LED
int inputPin = 2;   // Sensor input
int pirState = LOW; // we start, assuming no motion detected
int val = 0;        // variable for reading the pin status
int pircount = 15;
int piron = LOW;
int piroff = LOW;

// light
int photoresistor = A0;
int analogValue;
int lightcount = 4;
int lightState = LOW;

// light
void lightfunction()
{
  while (lightcount != 0 && lightcount < 8)
  {
    analogValue = analogRead(photoresistor);
    if (analogValue > 6)
    {
      Serial.println("IsLight");
      Serial.println(lightcount);
      lightcount = 15;
    }
    else
    {
      Serial.println("NoLight");
      Serial.println(lightcount);
      lightcount--;
    }
    delay(2s);
  }
}

void getlightfunction()
{
  lightfunction();

  Serial.println("BreakOut");

  if (lightcount == 15)
  {
    if (lightState == HIGH)
    {
      Serial.println("Notify light on");
      // Particle.publish("on", "on");
      lightState = HIGH;
    }
    else
    {
      Serial.println("Already Notified light on");
    }
  }
  else
  {
    if (lightState == LOW)
    {
      Serial.println("Notify light off");
      // Particle.publish("off", "off");
      lightState = LOW;
    }
    else
    {
      Serial.println("Already Notified light off");
    }
  }

  lightcount = 4;

  delay(2s);
}

// pir

void pirfunction()
{
  while (pircount != 0 && pircount < 20)
  {
    val = digitalRead(inputPin);
    if (val == 1)
    {
      Serial.println("IsMovement");
      Serial.println(pircount);
      Serial.println(piron);
      pircount = 25;
      piron = HIGH;
    }
    else
    {
      Serial.println("NoMovement");
      Serial.println(piroff);
      pircount--;
      piroff = HIGH;
    }
    delay(2s);
  }
}

void getpirfunction()
{
  pirfunction();

  if (piron == HIGH)
  {                             // check if the input is HIGH
    digitalWrite(ledPin, HIGH); // turn LED ON

    if (pirState == LOW)
    {
      Serial.println("Motion detected!"); // Only show message on change
      pirState = HIGH;
    }
  }
  else
  {
    digitalWrite(ledPin, LOW); // turn LED OFF

    if (pirState == HIGH)
    {
      Serial.println("Motion ended!"); // Only show message on change
      pirState = LOW;
    }
  }

  pircount = 15;
  piron = LOW;
  piroff = LOW;
  delay(4s);
}

void setup()
{
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(inputPin, INPUT); // declare sensor as input

  // light
  pinMode(photoresistor, INPUT);

  Serial.begin(9600);
}

void loop()
{
  getpirfunction();
  getlightfunction();
}
