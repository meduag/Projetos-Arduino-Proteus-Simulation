int XPin = A0;    // select the input pin for the potentiometer
int YPin = A1;
int ZPin = A2;

float accel_x, accel_y, accel_z;
float mean_x, mean_y, mean_z;
int mean_counter;

//float getFilteredSignal(int pin);

void setup()
{
  pinMode(13, OUTPUT);

  Serial.begin(9600);

  // Get mean value for threshold
  Serial.println("Starting callibration...");
}

void loop() {

  accel_x = getFilteredSignal(XPin);
  Serial.print("Eje X: ");
  Serial.println(accel_x);
  
  accel_y = getFilteredSignal(YPin);
  Serial.print("Eje Y: ");
  Serial.println(accel_y);

  accel_z = getFilteredSignal(ZPin);
  Serial.print("Eje Z: ");
  Serial.println(accel_z);
  delay(200);
}

float getFilteredSignal(int pin)
{
  float sensor_value = analogRead(pin);
  return sensor_value;
}



