#define X_ACCEL A0
#define Y_ACCEL A1
#define Z_ACCEL A2
#define ledPin 13
#define RMS_window 1000

class  FilterBuHp2
{
	public:
		FilterBuHp2()
		{
			v[0]=0.0;
			v[1]=0.0;
		}
	private:
		float v[3];
	public:
		float step(float x) //class II 
		{
			v[0] = v[1];
			v[1] = v[2];
			v[2] = (9.823854385260919475e-1 * x)
				 + (-0.96508117389913528061 * v[0])
				 + (1.96446058020523239840 * v[1]);
			return 
				 (v[0] + v[2])
				- 2 * v[1];
		}
};

float mean_value;
int iterator;
float accel;
float accel_values[RMS_window];
int queue_position;
int mean_counter;
float mean_x, mean_y, mean_z;
FilterBuHp2* filtered_value;
int t0;
int state;

void setup() {
  analogReference(EXTERNAL);
  pinMode(X_ACCEL, INPUT);
  pinMode(Y_ACCEL, INPUT);
  pinMode(Z_ACCEL, INPUT);
  pinMode(ledPin, OUTPUT);

  queue_position = 0;

  Serial.begin(9600);
  Serial1.begin(9600);

  mean_x = 0;
  mean_y = 0;
  mean_z = 0;
  mean_counter = 0;
//  Serial.begin(9600);

  filtered_value = new FilterBuHp2();

  // Get mean value for threshold
//  Serial.println("Starting callibration...");
  t0 = millis();
  while (millis() - t0 < 4000)
  {
    mean_x += getSeparatedValues(X_ACCEL);
    mean_y += getSeparatedValues(Y_ACCEL);
    mean_z += getSeparatedValues(Z_ACCEL);
    mean_counter++;
  }
  
  mean_x /= mean_counter;
  mean_y /= mean_counter;
  mean_z /= mean_counter;

//  Serial.print("Callibration done! With samples: ");
//  Serial.println(mean_counter);
  // Get mean value for threshold
  /*Serial.println("Starting accel callibration. Please stand still for 5 seconds.");
  int t0 = millis();
  while (millis() - t0 < 4000)
  {
    mean_value = getFilteredSignal(X_ACCEL, Y_ACCEL, Z_ACCEL);
  }*/
  
//  Serial.print("Callibration done! With samples: ");
//  Serial.println(mean_counter);

  state = 0;
  t0 = millis();
}



void loop() {
  if (millis() - t0 > 1000)
  {
    state++;
    t0 = millis();
  }
  accel = getSimpleValue(X_ACCEL, Y_ACCEL, Z_ACCEL);
//  Serial.print("Simple value: ");
//  Serial.print(accel);
  float accel_rms = getFilteredSignal(X_ACCEL, Y_ACCEL, Z_ACCEL);
  Serial.print(accel_rms);
  Serial.print(",");
  accel_rms = filtered_value->step(accel_rms);
//  Serial.print(" - RMS value: ");
  Serial.print(accel_rms);
  Serial1.println(accel_rms);
  Serial.print(",");
  Serial.println(state);
  // Serial.print("X: ");
  // Serial.print(getSeparatedValues(X_ACCEL) - mean_x);
  // Serial.print(" - Y: ");
  // Serial.print(getSeparatedValues(Y_ACCEL) - mean_y);
  // Serial.print(" - Z: ");
  // Serial.println(getSeparatedValues(Z_ACCEL) - mean_z);
  delay(1);
}

/* Accelerometer functions */
float getFilteredSignal(int pin_x, int pin_y, int pin_z)
{
  float instant_value_x = analogRead(pin_x) - mean_x;
  float instant_value_y = analogRead(pin_y) - mean_y;
  float instant_value_z = analogRead(pin_z) - mean_z;

  float instant_value = sqrt(pow(instant_value_x,2) + pow(instant_value_y,2) + pow(instant_value_z,2));
  accel_values_put(instant_value);
  float result = RMS(accel_values);

  return result;
}

void accel_values_put(float value_in)
{
  if (queue_position == RMS_window)
    queue_position = 0;
  accel_values[queue_position] = value_in;
  queue_position++;
}

float RMS(float values[])
{
  float result;
  result = 0;
  
  for(int i = 0; i < RMS_window; i++)
  {
    result += pow(values[i], 2);
  }

  result = sqrt(result/10);

  return result;
}

float getSimpleValue(int pin_x, int pin_y, int pin_z)
{
  float instant_value_x = analogRead(pin_x);
  float instant_value_y = analogRead(pin_y);
  float instant_value_z = analogRead(pin_z);

  float result = sqrt(pow(instant_value_x,2) + pow(instant_value_y,2) + pow(instant_value_z,2));

  return result;
}

float getSeparatedValues(int pin)
{  
  float instant_value = analogRead(pin);
  return instant_value;
}

