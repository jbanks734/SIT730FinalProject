
#include <MQTT.h>

// Set up MQTT
MQTT client("test.mosquitto.org", 1883, callback);
void callback(char *topic, byte *payload, unsigned int length)
{
}

// overall variables
int IFTTTState = LOW;

int ledPin = 7;     // Argon's onboard LED
int inputPin = 2;   // Sensor input
int pirState = LOW; // SensorState
int val = 0;        // variable for pin on pir
int pircount = 15;  // Used to count down so that the pir sensor can check if any movement is made
int piron = LOW;    // Used to see if the

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
      MQTTfunctionOn();
    }
    else if ((lightState == HIGH || lightState == LOW) && pirState == HIGH && IFTTTState == HIGH)
    {
      //  Lights already on
      Serial.println("do nothing");
      IFTTTState = HIGH;
      MQTTfunctionOn();
    }
    else if (lightState == HIGH && pirState == HIGH && IFTTTState == LOW)
    {
      Serial.println("Turn Off");
      // Turn lights off
      Particle.publish("off", "off");
      IFTTTState = LOW;
      // Turn lights off
      MQTTfunctionMovementOnly();
    }
  }
  else
  {
    if (IFTTTState == LOW)
    {
      Serial.println("Already Off");
      // Light alread off
      IFTTTState = LOW;
      MQTTfunctionOff();
    }
    else
    {
      Serial.println("Turn Off");
      Serial.println("no pir");
      // Turn lights off
      Particle.publish("off", "off");
      IFTTTState = LOW;
      MQTTfunctionOff();
    }
  }
}

void MQTTfunctionOff()
{
  client.disconnect();
  // delay(3s);
  client.connect("photonDev");
  // Only try to send messages if we are connected
  if (client.isConnected())
  {
    client.publish("photonLog", "Movement&LightOff");

    Serial.println("Movement & Light Off");
    // delay(1000);

    // client.loop();
  }
}

void MQTTfunctionOn()
{
  client.disconnect();
  // delay(3s);
  client.connect("photonDev");
  // Only try to send messages if we are connected
  if (client.isConnected())
  {
    client.publish("photonLog", "Movement&Light");

    Serial.println("Movement & Light Send");
    // delay(1000);
  }
}

void MQTTfunctionMovementOnly()
{
  client.disconnect();
  // delay(3s);
  client.connect("photonDev");
  // Only try to send messages if we are connected
  if (client.isConnected())
  {
    client.publish("photonLog", "Movement");

    Serial.println("Movement Only");
    // delay(1000);
  }
}

void setup()
{
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(inputPin, INPUT); // declare sensor as input

  // light
  pinMode(photoresistor, INPUT);

  Serial.begin(9600);
  client.connect("photonDev");
  // Connect to the server and call ourselves "photonDev"
}

void loop()
{
  // MQTTfunctionOn();
  togglelights();
  client.loop();
}
