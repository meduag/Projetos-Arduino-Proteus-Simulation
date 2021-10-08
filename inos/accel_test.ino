int XPin = A0;    // select the input pin for the potentiometer
int YPin = A1;
int ZPin = A2;

float accel_x, accel_y, accel_z;
float mean_x, mean_y, mean_z;
int mean_counter;

float getFilteredSignal(int pin);
int detectMovement(float accel);

void setup()
{
  pinMode(13, OUTPUT);
  analogReference(EXTERNAL);
  mean_x = 0;
  mean_y = 0;
  mean_z = 0;
  mean_counter = 0;
  Serial.begin(9600);

  // Get mean value for threshold
  Serial.println("Starting callibration...");
  int t0 = millis();
  while (millis() - t0 < 4000)
  {
    mean_x += getFilteredSignal(XPin);
    mean_y += getFilteredSignal(YPin);
    mean_z += getFilteredSignal(ZPin);
    mean_counter++;
  }
  
  mean_x /= mean_counter;
  mean_y /= mean_counter;
  mean_z /= mean_counter;

  Serial.print("Callibration done! With samples: ");
  Serial.println(mean_counter);
}

void loop() {

  accel_x = getFilteredSignal(XPin);
  accel_y = getFilteredSignal(YPin);
  accel_z = getFilteredSignal(ZPin);

  detectMovement(accel_x, 'x');
  detectMovement(accel_y, 'y');
  detectMovement(accel_z, 'z');
}

boolean detectMovement(float accel, char axis)
{
  int threshold = 5;
  
  switch (axis)
  {
    case 'x':
      if (abs(accel - mean_x > threshold))
      {
        digitalWrite(13, HIGH);
        delay(100);
        digitalWrite(13,LOW);
        return true;
      }
      else 
        return false;
      
    case 'y':
      if (abs(accel - mean_y > threshold))
      {
        digitalWrite(13, HIGH);
        delay(100);
        digitalWrite(13,LOW);
        return true;
      }
      else 
        return false;

    case 'z':
      if (abs(accel - mean_z > threshold))
      {
        digitalWrite(13, HIGH);
        delay(100);
        digitalWrite(13,LOW);
        return true;
      }
      else 
        return false;
  }
}

float getFilteredSignal(int pin)
{
  float sensor_value = analogRead(pin);

//  delay(10);

  return sensor_value;
}

// under development
float movingWindowFilter(float data)
{
  
}

// under development
void kalmanFilter(float *mean, float *cov, float *u, float *data, float *A, float *B, float *C, float *R, float *Q)
{
  // Prediction phase
//  int m = A*mean + B*u;
//  int S = A*cov*A’ + R;

  // Update phase
//  K = S*C’*pinv(C*S*C’ + Q);
//  new_mean = m + K*(z - C*m);
//  I = eye(size(K, 1), size(C, 2));
//  new_cov = (I - K*C)*S;
}
