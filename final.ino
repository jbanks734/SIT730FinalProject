
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
int piron = LOW;    // Used to see if there is movement

// light
int photoresistor = A0; // The analog input for the light sensor
int analogValue;        // Stores the value of the light sensor
int lightcount = 4;     // Used to make sure the is light or not
int lightState = LOW;   // The state of the light

// Function to sense if there is light
void lightfunction()
{
  while (lightcount != 0 && lightcount != 7)
  {
    analogValue = analogRead(photoresistor);
    if (analogValue > 6)
    {
      Serial.println("Is Light");
      Serial.println(lightcount);
      lightcount++;
    }
    else
    {
      Serial.println("No Light");
      Serial.println(lightcount);
      lightcount--;
    }
    delay(2s);
  }
}

// Function to set the states of the lights
void getlightfunction()
{
  lightfunction();

  if (lightcount == 7)
  {
    if (lightState == LOW)
    {
      lightState = HIGH;
    }
  }
  else
  {
    if (lightState == HIGH)
    {
      lightState = LOW;
    }
  }

  lightcount = 4;

  delay(2s);
}

// Function to see if there is movement using the PIR sensor
void pirfunction()
{
  while (pircount != 0 && pircount < 20)
  {
    val = digitalRead(inputPin);
    if (val == 1)
    {
      Serial.println("Is Movement");
      Serial.println(pircount);

      pircount = 25;
      piron = HIGH;
    }
    else
    {
      Serial.println("No Movement");
      Serial.println(pircount);
      pircount--;
    }
    delay(2s);
  }
}

// Function to set the state of the PIR sensor
void getpirfunction()
{
  pirfunction();

  if (piron == HIGH)
  {                             // check if the input is HIGH
    digitalWrite(ledPin, HIGH); // turn LED ON

    if (pirState == LOW)
    {
      Serial.println("Motion detected!");
      pirState = HIGH;
    }
  }
  else
  {
    digitalWrite(ledPin, LOW); // turn LED OFF

    if (pirState == HIGH)
    {
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }

  pircount = 15;
  piron = LOW;
  delay(4s);
}

// Function to toggle the lights
void togglelights()
{
  getpirfunction();

  if (pirState == HIGH)
  {
    getlightfunction();

    if (lightState == LOW && pirState == HIGH && IFTTTState == LOW)
    {
      Serial.println("Turn ON");
      Particle.publish("on", "on");
      IFTTTState = HIGH;
      MQTTfunctionOn();
    }
    else if ((lightState == HIGH || lightState == LOW) && pirState == HIGH && IFTTTState == HIGH)
    {
      Serial.println("do nothing");
      IFTTTState = HIGH;
      MQTTfunctionOn();
    }
    else if (lightState == HIGH && pirState == HIGH && IFTTTState == LOW)
    {
      Serial.println("Turn Off");
      Particle.publish("off", "off");
      IFTTTState = LOW;
      MQTTfunctionMovementOnly();
    }
  }
  else
  {
    if (IFTTTState == LOW)
    {
      Serial.println("Already Off");
      IFTTTState = LOW;
      MQTTfunctionOff();
    }
    else
    {
      Serial.println("Turn Off");
      Particle.publish("off", "off");
      IFTTTState = LOW;
      MQTTfunctionOff();
    }
  }
}

// Function to tell the raspberry pi the lights are off as well as movement using MQTT
void MQTTfunctionOff()
{
  client.disconnect();
  client.connect("photonDev");
  if (client.isConnected())
  {
    client.publish("photonLog", "Movement&LightOff");

    Serial.println("Movement & Light Off");
  }
}

// Function to tell the raspberry pi the lights are on and there is movement using MQTT
void MQTTfunctionOn()
{
  client.disconnect();
  client.connect("photonDev");
  if (client.isConnected())
  {
    client.publish("photonLog", "Movement&Light");

    Serial.println("Movement & Light Send");
  }
}

// Function to tell the raspberry pi the lights are off using MQTT
void MQTTfunctionMovementOnly()
{
  client.disconnect();
  client.connect("photonDev");
  if (client.isConnected())
  {
    client.publish("photonLog", "Movement");

    Serial.println("Movement Only");
  }
}

void setup()
{
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(inputPin, INPUT); // declare sensor as input
  pinMode(photoresistor, INPUT);

  Serial.begin(9600);          // Start serial
  client.connect("photonDev"); // Connect to the server
}

void loop()
{
  togglelights();
  client.loop();
}
