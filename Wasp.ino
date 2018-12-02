#include "Arduino.h"
#include "webserver.h"
#include "relay.h"
#include "rules.h"
//#include "rtc.h"
#include "ntp.h"

long previousTimer = 0;
uint8_t today = 0;

void setup(void) {
  
	//ntp client init
	ntp_init();
	//init relay shield
	rel_init();
	//init web server
	srv_init();

}

String lastRuleTriggered = "";


void loop(void) {
	
	srv_handle_client();
	ntp_refresh();

	//check the rules and eventually perfoms action
	if (!rules_load()){
		DBG_OUTPUT_PORT.println(F("wasp|Can't load rules file!"));
		while(1);
	}
	
	long l_currentTimer = millis(); 

	//check rules each CHECK_RULES_DELAY ms if server available
	if (l_currentTimer - previousTimer > CHECK_RULES_INTERVAL){

		//ntp_print_date_time();
		String l_currentRule = "";

		for(size_t l_nIndex = 1; l_nIndex < MAX_RULES_NUM; l_nIndex++){
			
			l_currentRule = "rule"+String(l_nIndex);
			if(jsonRules->containsKey(l_currentRule)){

				JsonVariant l_rule = jsonRules->get<JsonVariant>(l_currentRule);
			
				int l_ruleHour = atoi(l_rule["hour"]);
				int l_ruleMinute = atoi(l_rule["minute"]);
				bool l_ruleAction = l_rule["action"];

				//it's time to perform an action?
				if (l_ruleHour == ntp_hour_now() 
					&& l_ruleMinute == ntp_minute_now() 
					&& l_currentRule != lastRuleTriggered){
					
						DBG_OUTPUT_PORT.println(F("wasp|time to rule!"));
						//toggle output if actual state is the high and rule action is false (or viceversa)
						if ((rel_get_status() == HIGH && !l_ruleAction)
						|| (rel_get_status() == LOW && l_ruleAction)){
							rel_toggle_output();	
						}
						lastRuleTriggered = l_currentRule;
				}

			}
		}

		if (today != ntp_day_now()){

			lastRuleTriggered = "";
			today = ntp_day_now();
		}	

		previousTimer = l_currentTimer;		
	}	

}


