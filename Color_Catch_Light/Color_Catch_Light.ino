#define light_R A0
#define light_G A1
#define light_B A2
#define light_All A3
#define Button 2
int value_R = 0;
int value_G = 0;
int value_B = 0;
int value_All = 0;

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(light_R, INPUT);
  pinMode(light_G, INPUT);
  pinMode(light_B, INPUT);
  pinMode(light_All, INPUT);
  pinMode(Button, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
    int reading = digitalRead(Button);
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
  
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
  
        if (buttonState == HIGH) {
          value_R = analogRead(light_R);
          value_G = analogRead(light_G);
          value_B = analogRead(light_B);
          value_All = analogRead(light_All);
          Serial.print("R:");
          Serial.print(value_R);
          Serial.print("\tG:");
          Serial.print(value_G);
          Serial.print("\tB:");
          Serial.print(value_B);
          Serial.print("\tAll:");
          Serial.print(value_All);
          Serial.println();
        }
      }
    }
  
  
    lastButtonState = reading;
}
