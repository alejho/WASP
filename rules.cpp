#include "rules.h"

StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
JsonObject* jsonRules = NULL;

bool bRulesModified = true;

bool rules_load(){
	
	//minimize file access
	if (!rules_modified()) return true;

	if (SPIFFS.exists(RULES_FILE)) {
		File file = SPIFFS.open(RULES_FILE, "r");
		
		jsonBuffer.clear();
		JsonObject& l_jsonToRet = jsonBuffer.parseObject(file);
		jsonRules = &l_jsonToRet;

		if (!l_jsonToRet.success()){
			DBG_OUTPUT_PORT.println(F("rules_load()|Failed to read rules file!"));
			file.close();
			return false;
		}
		file.close();
		rules_set_modified(false);
		return true;
	}
	else{
		DBG_OUTPUT_PORT.print(F("rules_load()|Unable To Find rules file!"));
		DBG_OUTPUT_PORT.println(RULES_FILE);
		DBG_OUTPUT_PORT.println();
		return false;
	}
	return false;
}

uint8_t rules_add(const int x_hour, const int x_minute, bool x_action){
	
	
	if (!rules_load()){
		DBG_OUTPUT_PORT.println(F("rules_add()|Can't load rules!"));
		return RULES_ADD_LOAD_FAILED;
	}
	
	int l_nIndex = 1;
	//get the first rules free index
	while(jsonRules->containsKey("rule"+String(l_nIndex))){
		l_nIndex++;
	}
	//check if rules are all used
	if (l_nIndex > MAX_RULES_NUM){
		DBG_OUTPUT_PORT.println(F("rules_add()|max rules num excedeed!"));
		return RULES_ADD_MAX_RULES;
	}

	//check if exists another rule at the same time
	for(size_t l_nIndex = 1; l_nIndex < MAX_RULES_NUM; l_nIndex++){
			
			if(jsonRules->containsKey("rule"+String(l_nIndex))){

				JsonVariant l_rule = jsonRules->get<JsonVariant>("rule"+String(l_nIndex));
			
				int l_ruleHour = atoi(l_rule["hour"]);
				int l_ruleMinute = atoi(l_rule["minute"]);
				if (l_ruleHour == x_hour && l_ruleMinute == x_minute) return RULES_ADD_ALREADY_EXISTS;
			}
	}

	
	JsonObject& l_newRule = jsonRules->createNestedObject("rule"+String(l_nIndex));
	
	l_newRule["hour"] = x_hour;
	l_newRule["minute"] = x_minute;
	l_newRule["action"] = x_action;
	bRulesModified = true;
	 
	return RULES_ADD_OK;
	
}

bool rules_remove(const String x_name){
	
	if (!rules_load()){
		DBG_OUTPUT_PORT.println(F("rules_remove()|Can't load rules!"));
		return false;
	}
	
	jsonRules->remove(x_name);
	
	if(!rules_save()){
		DBG_OUTPUT_PORT.println(F("rules_remove()|Can't save rules!"));
		return false;
	}
	
	bRulesModified = true;
	return true;
}

bool rules_save(){
	
	SPIFFS.remove(RULES_FILE);
	
	File file = SPIFFS.open(RULES_FILE, "w");

	if (!file){
		DBG_OUTPUT_PORT.println(F("rules_save()|Failed to create rules file!"));
		return false;
	}
	if (jsonRules->prettyPrintTo(file) == 0){
		DBG_OUTPUT_PORT.println(F("rules_save()|Failed to write to rules file!"));
	}
	file.close();
	return true;
}

bool rules_modified(){

	return bRulesModified;
}

void rules_set_modified(bool x_modified){
	
	bRulesModified = x_modified;
}

void rules_dbg_print(){
	
	if (!rules_load()){
		DBG_OUTPUT_PORT.println(F("rules_dbg_print()|Can't load rules!"));
		return;
	}
	jsonRules->prettyPrintTo(DBG_OUTPUT_PORT);	
	DBG_OUTPUT_PORT.println(F(""));
}

bool rules_get_list(String& x_sJsonRules){

	//DBG_OUTPUT_PORT.println(F("rules_get()|execute"));

	if (!rules_load()){
		DBG_OUTPUT_PORT.println(F("rules_get()|Can't load rules!"));
		x_sJsonRules = "{}";
		return false;
	}
	jsonRules->prettyPrintTo(x_sJsonRules);	
	return true;
}

