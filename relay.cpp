#include "relay.h"
#include "common.h"
  
void rel_init(){

	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, HIGH);
}

int rel_get_status(){

  //DBG_OUTPUT_PORT.println("rel_get_status|invoked");
  
	if(digitalRead(RELAY_PIN) == HIGH){
		//DBG_OUTPUT_PORT.println("output is ON");
		return HIGH;
	}
	else{
		//DBG_OUTPUT_PORT.println("output is OFF");
		return LOW;
	}
}

void rel_toggle_output(){

	DBG_OUTPUT_PORT.println("rel_toggle_output|invoked");
	
	if(rel_get_status() == HIGH){
		//DBG_OUTPUT_PORT.println("output set to OFF");
		digitalWrite(RELAY_PIN, LOW);
	}
	else{
		//DBG_OUTPUT_PORT.println("output set to ON");
		digitalWrite(RELAY_PIN, HIGH);
	}
}


