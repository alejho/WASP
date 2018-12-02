#include "webserver.h"

const char* ssid = "yourWiFi_ssid";
const char* password = "yourWiFi_password";
const char* host = "wasp";

IPAddress  ip(192,168,1,132);
IPAddress  netmask(255,255,255,0);
IPAddress  gateway(192,168,1,254);
IPAddress  dns(8,8,8,8);


ESP8266WebServer server(80);


void srv_listFiles(){
	
	Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      DBG_OUTPUT_PORT.printf("FS File: %s", fileName.c_str());
    }
    DBG_OUTPUT_PORT.printf("\n");

}
void srv_init(){

	SPIFFS.begin();
	srv_listFiles();

	DBG_OUTPUT_PORT.begin(115200);
	DBG_OUTPUT_PORT.print(F("\n"));
	DBG_OUTPUT_PORT.setDebugOutput(true);
	
	//WIFI INIT
	WiFi.config(ip, dns, gateway, netmask);
	DBG_OUTPUT_PORT.printf("Connecting to %s\n", ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED){
		delay(1000);
	}	
	WiFi.mode(WIFI_STA);
	

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		DBG_OUTPUT_PORT.print(F("."));
	}
	DBG_OUTPUT_PORT.println(F(""));
	   
	DBG_OUTPUT_PORT.println(WiFi.localIP());

	MDNS.begin(host);
	DBG_OUTPUT_PORT.print(F("Open http://"));
	DBG_OUTPUT_PORT.print(host);
	DBG_OUTPUT_PORT.println(F(".local"));
	
	//called when the url is not defined here
	//use it to load content from SPIFFS
	server.onNotFound([]() {
	if (!srv_handle_file_read(server.uri())) {
	  server.send(404, "text/plain", "FileNotFound");
	}
	});
	//toggle output
	server.on("/toggleOutput", HTTP_GET, []() {
		rel_toggle_output();
		server.send(200, "text", "");
	});
	//get status
	server.on("/getOutputState", HTTP_GET, []() {

		if (rel_get_status() == HIGH){
      //DBG_OUTPUT_PORT.println("webserver|relay is ON");
			server.send(200, "text", "ON");
		}
		else{
			//DBG_OUTPUT_PORT.println("webserver|relay is OFF");
			server.send(200, "text", "OFF");
		}
		
	});
	//get rules list
	server.on("/getRulesList", HTTP_GET, []() {
		
		String l_sJsonRules;
		rules_get_list(l_sJsonRules);
		//DBG_OUTPUT_PORT.println(l_sJsonRules);
		server.send(200, "text/json", l_sJsonRules);
	});
	//remove rule
	server.on("/removeRule", HTTP_GET, []() {
		if (server.hasArg("index")) { 
			rules_remove(server.arg("index"));
		}
		server.send(200, "text", "");		
	});
	//add rule
	server.on("/addRule", HTTP_POST, []() {
		
		//DBG_OUTPUT_PORT.println("add rule:");
		bool l_bValidData = true;

		if (server.hasArg("rule_time") && server.arg("rule_time") != "" ) { 
			//DBG_OUTPUT_PORT.println(server.arg("rule_time"));
		}
		else{
			l_bValidData = false;
		}
		if (server.hasArg("action")) { 
			//DBG_OUTPUT_PORT.println(server.arg("action"));
		}
		else{
			l_bValidData = false;		
		}

		if(l_bValidData){
			String l_ruleTime = server.arg("rule_time");
			bool l_action = server.arg("action") == "ON" ? true : false;
			
			int l_hour = atoi(l_ruleTime.substring(0,2).c_str());
			int l_minute = atoi(l_ruleTime.substring(3,5).c_str());

			uint8_t l_res = rules_add(l_hour, l_minute, l_action);
			if (l_res == RULES_ADD_ALREADY_EXISTS){
				server.send(200, "text", "RULES_ADD_ALREADY_EXISTS");
				return;				
			}
			else if(l_res == RULES_ADD_LOAD_FAILED){
				server.send(200, "text", "RULES_ADD_LOAD_FAILED");
				return;		
			}
			else if(l_res == RULES_ADD_MAX_RULES){
				server.send(200, "text", "RULES_ADD_MAX_RULES");
				return;
			}

			if (!rules_save()){
				server.send(200, "text", "RULES_SAVE_FAILED");
				return;			
			}
		}
		server.send(200, "text", "");		
	});

	//get ntp server availability
	server.on("/getNtpStatus", HTTP_GET, []() {

		//DBG_OUTPUT_PORT.println("getNtpStatus");
		
		if (ntp_available()){
			server.send(200, "text", "ON");
		}
		else{
			server.send(200, "text", "OFF");
		}
		
	});

	server.begin();
	DBG_OUTPUT_PORT.println(F("HTTP server started"));

}

String srv_get_content_type(String x_filename) {

  if (server.hasArg("download")) {
    return "application/octet-stream";
  } else if (x_filename.endsWith(".htm")) {
    return "text/html";
  } else if (x_filename.endsWith(".html")) {
    return "text/html";
  } else if (x_filename.endsWith(".css")) {
    return "text/css";
  } else if (x_filename.endsWith(".js")) {
    return "application/javascript";
  } else if (x_filename.endsWith(".png")) {
    return "image/png";
  } else if (x_filename.endsWith(".gif")) {
    return "image/gif";
  } else if (x_filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (x_filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (x_filename.endsWith(".xml")) {
    return "text/xml";
  } else if (x_filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (x_filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (x_filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}


bool srv_handle_file_read(String x_path){

  //DBG_OUTPUT_PORT.println("handleFileRead: " + x_path);
  if (x_path.endsWith("/")) {
    x_path += "index.html";
  }
  String contentType = srv_get_content_type(x_path);
  String pathWithGz = x_path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(x_path)) {
    if (SPIFFS.exists(pathWithGz)) {
      x_path += ".gz";
    }
    File file = SPIFFS.open(x_path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;

}

void srv_handle_client(){
	server.handleClient();
}
