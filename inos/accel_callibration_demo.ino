#define X_ACCEL A0
#define Y_ACCEL A1
#define Z_ACCEL A2
#define ledPin 13


float mean_values[3];
int mean_counter;
int iterator;
float accel;

void setup() {
  pinMode(X_ACCEL, INPUT);
  pinMode(Y_ACCEL, INPUT);
  pinMode(Z_ACCEL, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(9600);
  // Get mean value for threshold
  Serial.println("Starting accel callibration. Please stand still for 5 seconds.");
  for (iterator = 0; iterator < 3; iterator++)
    mean_values[iterator] = 0;
  mean_counter = 0;
  int t0 = millis();
  while (millis() - t0 < 4000)
  {
    mean_values[0] += getFilteredSignal(X_ACCEL);
    mean_values[1] += getFilteredSignal(Y_ACCEL);
    mean_values[2] += getFilteredSignal(Z_ACCEL);
    mean_counter++;
  }
  mean_values[0] /= mean_counter;
  mean_values[1] /= mean_counter;
  mean_values[2] /= mean_counter;

  Serial.print("Callibration done! With samples: ");
  Serial.println(mean_counter);
}

void loop() {
  accel = getFilteredSignal(Z_ACCEL);
  if(detectMovement(accel, mean_values[2]))
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}

/* Accelerometer functions */
float getFilteredSignal(int pin)
{
  float sensor_value = analogRead(pin);

  //  filter it

  return sensor_value;
}

boolean detectMovement(float accel, float mean)
{
  int threshold = 150;

  if (abs(accel - mean) > threshold)
  {
    Serial.println(String(accel-mean) + ">" + String(threshold));
    Serial1.println(String(accel-mean));
    return true;
  }
  else 
    return false;
}
