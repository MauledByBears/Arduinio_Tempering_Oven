#include "max6675.h"

//Values for different heat treat schemes can be saved here
//Data should be taken at room temp with the same gear
//For 300C max: HI = 295, LO = 285, HOLDON = 5000, HOLDOFF = 1000;

int RELAY = 13;
int CLK = 4, CS = 5, DO = 6; //SPI setup
int stat = 0; //Is the relay on or off?

MAX6675 thermocouple(CLK, CS, DO);
int VCC = 3;
int GND = 2;
double SetrangeHIGH = 295; //Adjust to needs
double SetrangeLOW = 285; //Adjust to needs

void setup(){
	Serial.begin(9600);

	pinMode(RELAY, OUTPUT);
	pinMode(VCC, OUTPUT); digitalWrite(VCC, HIGH);
	pinMode(GND, OUTPUT); digitalWrite(GND, LOW);

	delay(500); //wait for chip to stabilize
	if(digitalRead(DO) == 0){
		Serial.println("Thermocouple not connected!");
	}
}

void loop(){
	double Input = thermocouple.readCelsius();
	Serial.println(thermocouple.readCelsius());
	
	if (Input < SetrangeLOW){
		digitalWrite(RELAY, HIGH);
    if (stat == 0){
      Serial.println("HIGH");
    }
    stat = 1;
    delay(1000);
	}
  else if (Input > SetrangeLOW && Input < SetrangeHIGH){
    Serial.println("HOLDING");
    digitalWrite(RELAY, HIGH);
    stat = 1;
    delay(5000); //Coil needs time to warm up
    Serial.println(thermocouple.readCelsius());
    digitalWrite(RELAY, LOW);
    stat = 0;
    delay(1000); //Heat lost increases w/ temp, adjust to needs

    }
	else if (Input > SetrangeHIGH){
	digitalWrite(RELAY, LOW);
  if (stat == 1){
    Serial.println("LOW");
  }
  stat = 0;
  delay(1000);
	}
  else delay(1000);
	}
