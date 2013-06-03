/*
 * Will Keyser
 * 2013
 */
// desired temperature in deg F
int threshold = 70;
 
//Relay Driver Variables
const int relayPin = 10;

//TMP36 Pin Variables
const int sensorPin = A0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
 
/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
                       
  pinMode(relayPin, OUTPUT);
}
 
void loop()                     // run over and over again
{
 //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);  
 
 // converting that reading to voltage, assuming AREF is 3.3V
 float voltage = reading * 3.3;
 voltage /= 1024.0; 
 
 // find temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((volatge - 500mV) times 100)
 
 // now convert to Fahrenheight
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 
 if (temperatureF > threshold) {
    digitalWrite(relayPin, HIGH);
  } 
  else {
    digitalWrite(relayPin,LOW); 
  }
 
 
 delay(2000);                                     //waiting a second
}
