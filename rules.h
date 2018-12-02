#ifndef RULES_H
#define RULES_H

#include "FS.h"
#include <ArduinoJson.h>
#include "common.h"

#define RULES_FILE "/rules.json"
#define JSON_BUFFER_SIZE 3000
#define MAX_RULES_NUM 20

extern JsonObject* jsonRules;

bool rules_load();
uint8_t rules_add(const int x_hour, const int x_minute, bool x_action);
bool rules_remove(const String x_name);
bool rules_save();
bool rules_modified();
void rules_set_modified(bool x_modified);
//void rules_dbg_print();
bool rules_get_list(String& x_sJsonRules);


#endif
