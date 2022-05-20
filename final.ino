
#include <MQTT.h>

MQTT client("test.mosquitto.org", 1883, callback);

// This is called when a message is received. However, we do not use this feature in
// this project so it will be left empty
void callback(char *topic, byte *payload, unsigned int length)
{
}

// overall
int IFTTTState = LOW;

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
  while (lightcount != 0 && lightcount != 7)
  {
    analogValue = analogRead(photoresistor);
    if (analogValue > 6)
    {
      Serial.println("IsLight");
      Serial.println(lightcount);
      lightcount++;
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

  if (lightcount == 7)
  {
    if (lightState == LOW)
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
    if (lightState == HIGH)
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
      Serial.println(pirState);

      pircount = 25;
      piron = HIGH;
    }
    else
    {
      Serial.println("NoMovement");
      Serial.println(pircount);
      Serial.println(piron);
      Serial.println(pirState);
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

void togglelights()
{
  getpirfunction();

  if (pirState == HIGH)
  {
    getlightfunction();

    if (lightState == LOW && pirState == HIGH && IFTTTState == LOW)
    {
      Serial.println("Turn ON");
      // Turn lights on
      Particle.publish("on", "on");
      IFTTTState = HIGH;
    }
    else if (lightState == HIGH || lightState == LOW && pirState == HIGH && IFTTTState == HIGH)
    {
      //  Lights already on
      Serial.println("do nothing");
      IFTTTState = HIGH;
    }
    else
    {
      Serial.println("Turn Off");
      // Turn lights off
      Particle.publish("off", "off");
      IFTTTState = LOW;
    }
  }
  else
  {
    if (IFTTTState == LOW)
    {
      Serial.println("Already Off");
      // Light alread off
      IFTTTState = LOW;
    }
    else
    {
      Serial.println("Turn Off");
      Serial.println("no pir");
      // Turn lights off
      Particle.publish("off", "off");
      IFTTTState = LOW;
    }
  }
}

void MQTTfunction()
{
  // Only try to send messages if we are connected
  if (client.isConnected())
  {

    client.publish("photonLog", "Button has been pressed");

    // If the button is pressed it will be read as 0V since the button is
    // in an inverting configuation.

    // if (digitalRead(0) == 0)
    // {
    //   // Publish our message to the test server
    //   client.publish("photonLog", "Button has been pressed");
    //   delay(1000);
    // }

    // CALL THIS at the end of your loop
    client.loop();
  }
}

void setup()
{
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(inputPin, INPUT); // declare sensor as input

  // light
  pinMode(photoresistor, INPUT);

  Serial.begin(9600);

  // Connect to the server and call ourselves "photonDev"
  client.connect("photonDev");
}

void loop()
{
  // togglelights();
  MQTTfunction();
}
