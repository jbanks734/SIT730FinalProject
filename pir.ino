int ledPin = 7;     // Argon's onboard LED
int inputPin = 2;   // Sensor input
int pirState = LOW; // we start, assuming no motion detected
int val = 0;        // variable for reading the pin status

int photoresistor = A0;
int analogValue;
int i = 4;
int high = 0;
int low = 0;

void setup()
{
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(inputPin, INPUT); // declare sensor as input

  Serial.begin(9600);
  pinMode(photoresistor, INPUT);
}

void light()
{
  while (i != 0 && i != 7)
  {
    analogValue = analogRead(photoresistor);
    if (analogValue > 6)
    {
      Serial.println("IsLight");
      Serial.println(i);
      i++;
    }
    else
    {
      Serial.println("NoLight");
      Serial.println(i);
      i--;
    }
    delay(2s);
  }
}

void loop()
{
  val = digitalRead(inputPin); // read input value
  if (val == HIGH)
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
}
