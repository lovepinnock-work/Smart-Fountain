#define NUM_SENSORS 4 //Number of ultrasonic sensors we have

const int ledRelayPin = 12 ;
const int relayPin = 6;
const int waterPin = 7;
const int trigPins[] = {2, 4, 8, 10};
const int echoPins[] = {3, 5, 9, 11};

const int distanceThreshold = 15; //15cm to be safe. 
const unsigned long fountainRunTime = 30000; //30s

bool drakeInFront;

// False detection prevention. If object stuck in front of sensor(s) within threshold, we ignore sensor(s) until object is moved.
// We will keep and update an array of prevDistances. -1 represents no prev. We will take distance of each sensor and check if:
//                                                                                                  abs(curr - prev) <= 1 --> Sensor blocked
//                                                                                                                   > 1 --> Sensor unblocked
long prevDistances[NUM_SENSORS] = {-1, -1, -1, -1};
bool ledOn = false;
bool waterRelayOn = false;
unsigned long turnOffWaterAt = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //waterSensor
  pinMode(waterPin, INPUT);

  //Sensors
  for (int i=0; i<NUM_SENSORS; i++) {
    pinMode(echoPins[i], INPUT);
    pinMode(trigPins[i], OUTPUT);
  }

  //Relays
  pinMode(ledRelayPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  //Ensure relays are off to start. Active high to prevent false ONs
  digitalWrite(ledRelayPin, LOW);
  digitalWrite(relayPin, LOW);
}

bool sensorBlocked(int sensorNumber, long currDistance) {
  bool isBlocked = false;
  if (sensorNumber < NUM_SENSORS) {
    if (prevDistances[sensorNumber] != -1) { //Ensure we have a prev measurement to compare to
      if (currDistance <= distanceThreshold && abs(currDistance - prevDistances[sensorNumber]) <= 1) { //Ensure no false positives and that object has moved further than our threshold
        isBlocked = true;
      }
    } //1st distance measurement. Can't tell if blocked 
    prevDistances[sensorNumber] = currDistance;
  } else {
    Serial.println("Invalid sensor number given, exceeds system capacity. Was given: ");
    Serial.print(sensorNumber);
  }
  return isBlocked;
}

bool detectDrake() {
  bool drakeDetected = false;
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Checking sensor #"); Serial.println(i+1);
    long distance = readSensor(trigPins[i], echoPins[i]);
    if (distance >= 0) { 
      if (!sensorBlocked(i, distance)) {
        if (distance <= distanceThreshold) { //Sensor unblocked and distance within threshold drake is detected!
          drakeDetected = true;
          Serial.print("Drake detected by sensor #"); Serial.println(i);
          break; //No need to continue checking
        }
      } else { //Object stuck in front sensor, ignore reading until not stuck
        Serial.print("Ignoring sensor #"); Serial.print(i); Serial.println(" it is blocked");
      }
    } else {//If less than 0, sensor is blocked or too far, ignore
      Serial.print("Sensor #"); Serial.print(i+1); Serial.println(" is blocked or no object within detectable range.");
    }
    delay(80); //Give ultrasonic waves a few seconds to disperse
  }
  //Returns true if drake is in front of fountain
  return drakeDetected;
}

long readSensor(int trigPin, int echoPin) {
  long echoDuration = -1;
  int distance = -1;

  digitalWrite(trigPin, LOW); //Clear pin
  delayMicroseconds(2);
  
  //Set trigPin to high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //Read echoPin, pulseIn will return sound wave travel time in microseconds
  echoDuration = pulseIn(echoPin, HIGH);

  /*if (echoDuration == 0) {//Blocked or too far, ignore this. 6/16/25 Drake probably will be right up against it most likely will give zero. 
    return -1;
  }*/
  //Calculate distance
  distance = (echoDuration * 0.0343)/ 2; //Distance = speed of sound(converted to CMs)/2 since round trip
  Serial.print("Distance detected "); Serial.print(distance); Serial.print(" cms"); Serial.println("");
  return distance;
}

int waterPresent() {
  return digitalRead(waterPin) == HIGH;
}

void loop() {
  // put your main code here, to run repeatedly:

  if (waterPresent()) { //Run only when there is water.
    //Turn off no water led if still on
    if (ledOn) {
      digitalWrite(ledRelayPin, LOW);
      ledOn = false;
    } 
    /*
    <----------millis--------------------------millis+turnOffWaterAt------->
     detectDrake      don't detectDrake                             detectDrake
     relay LOW        relayHigh                relayLow                  
     Timeline like this is helpful somewhat, the real insight is the number of rows/conditions tells you how many if statements you'll need to have this logic!                             
    */
    //Detection Code. Turns relay on for "fountainRunTime" seconds allowing water to flow then turns it off after.
    if (millis() >= turnOffWaterAt) {
      if (waterRelayOn) {
        Serial.println("Turning off water");
        digitalWrite(relayPin, LOW);
        turnOffWaterAt = 0;
        waterRelayOn = false;
      }
      if(detectDrake()) {
        Serial.println("Turning on water");
        digitalWrite(relayPin, HIGH);
        turnOffWaterAt = millis() + fountainRunTime;
        waterRelayOn = true;
      }
    }
  } else { //No water, turn blue led on
    if (!ledOn) {
      Serial.println("No water, will stop relay and turn on 'no water LED'!");
      ledOn = true;
      digitalWrite(relayPin, LOW);
      digitalWrite(ledRelayPin, HIGH);
    }
  }
  delay(100); //Adjust to increase rate of detection 
  
}
