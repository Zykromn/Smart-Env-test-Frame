// Libs
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>




// Consts and vars
const String SYSTEM_ID = "000000001";
const char* SSID = "20";                                  // HTTP client
const char* PASSWORD = "79227940";                        // HTTP client
String HTTP_ServerAdress = "http://192.168.0.138:1337/";  // HTTP server
unsigned long HTTP_LastTime = 0;                          // HTTP delay
unsigned long HTTP_TimerDelay = 10000;                    // HTTP delay
int SYS_PinUlrasoundTRIG = 15;                            // Sensor pin
int SYS_PinUlrasoundECHO = 2;                             // Sensor pin
DynamicJsonDocument JSON_VL (1024);                       // JSON VL Creating




// Main voids
void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  pinMode(SYS_PinUlrasoundTRIG, OUTPUT);
  pinMode(SYS_PinUlrasoundECHO, INPUT);

  Serial.println("SMART ENV. HOME SYSTEM");
  Serial.println("Based on ESP32");
  Serial.println();

  delay(3000);

  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("Starting...");
  Serial.println();

  Serial.println("--------------------------------------------------");
  Serial.print("Connecting to the WiFi network");
  int WiFi_ConnectTry = 0;
  while(WiFi.status() != WL_CONNECTED) {
    if (WiFi_ConnectTry == 20) {
      while(WiFi.status() != WL_CONNECTED) {
        Serial.println("An error occurred while trying to establish a Wi-fi connection. Trying again ...");
        delay(500);
      }
      break;
    }
    delay(500);
    Serial.print(".");
    WiFi_ConnectTry += 1;
  }
  Serial.println();
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  delay(3000);
  Serial.println("--------------------------------------------------");
  Serial.println("Connecting to the server and checking the system registration...");
  JSON_VL["type"] = "entrance";
  JSON_VL["systemid"] = SYSTEM_ID;
  String JSON_StringVL;
  serializeJson(JSON_VL, JSON_StringVL);
  if(WiFi.status()== WL_CONNECTED) {
    HTTPClient http;

    String HTTP_ServerPath = HTTP_ServerAdress + JSON_StringVL;
    http.begin(HTTP_ServerPath.c_str());
    int HTTP_ResponseCode = http.GET();
    
    Serial.println(HTTP_ResponseCode);

    if (HTTP_ResponseCode == 200 ) {
      Serial.println("SERVER CONNECTING SUCCESSFUL");
    
      String HTTP_PayloadResponse = http.getString();

      if (HTTP_PayloadResponse == "1") {
        Serial.println("SERVER REGISTRATION SUCCSESSFUL. CONTROL POINT CONNECTION SUCCSESSFUL. DBC ACCESS OPENED.");

        JSON_VL.remove("type");
      } else if (HTTP_PayloadResponse == "2") {
        while (true) {
          Serial.println("SERVER REGISTRATION SUCCSESSFUL. CONTROL POINT CONNECTION FAILED. DBC ACCESS BLOCKED.");
          delay(1000);
        }
      } else {
        while (true) {
          Serial.println("SERVER REGISTRATION FAILED. CONTROL POINT CONNECTION FAILED. DBC ACCESS BLOCKED.");
          delay(1000);
        }
      }
    } else {
      while (true) {
        Serial.println("SERVER CONNECTING FAILED");
        Serial.print("Error code: ");
        Serial.println(HTTP_ResponseCode);
        delay(1000);
      }
    }
  }
  Serial.println("--------------------------------------------------");

  Serial.println();
  Serial.println("System started succsessful");
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}

void loop() {
  delay(15000);  


  JSON_VL["type"] = "infobj";
  int DISTANCE = SYS_SensorULTRASOUND(SYS_PinUlrasoundTRIG, SYS_PinUlrasoundECHO);
  JSON_VL["123123"] = DISTANCE;
  
  String JSON_StringVL;
  serializeJson(JSON_VL, JSON_StringVL);
  String HTTP_Response = HTTP_Request(HTTP_ServerAdress, JSON_StringVL);
}



// Additional voids
int SYS_SensorULTRASOUND(int SYS_PinUlrasoundTRIG, int SYS_PinUlrasoundECHO) {
  // Prepairing ...
  digitalWrite(SYS_PinUlrasoundTRIG, LOW);
  delay(5);
  digitalWrite(SYS_PinUlrasoundTRIG, HIGH);

  // Main impulse
  delayMicroseconds(10);
  digitalWrite(SYS_PinUlrasoundTRIG, LOW);
  int DURATION = pulseIn(SYS_PinUlrasoundECHO, HIGH);
  int DISTANCE = (DURATION / 2) / 29.1;

  return DISTANCE;
} 

String HTTP_ServerPathFormulate(String HTTP_ServerAdress, String HTTP_PayloadRequest) {
  String HTTP_ServerPath = "";
  String HTTP_EncodePayloadRequest = HTTP_UrlEncode(HTTP_PayloadRequest);
  HTTP_ServerPath = HTTP_ServerPath + HTTP_ServerAdress + HTTP_EncodePayloadRequest;

  return HTTP_ServerPath;
}

String HTTP_Request(String HTTP_ServerAdress, String HTTP_PayloadRequest) {
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("===========================================================");
  Serial.println("HTTP request starting...");
  Serial.println();

  Serial.println("--------------------------------------------------");
  Serial.println("Formulating server path...");
  String HTTP_ServerPath = HTTP_ServerPathFormulate(HTTP_ServerAdress, HTTP_PayloadRequest);
  Serial.print("Server path - ");
  Serial.println(HTTP_ServerPath);

  Serial.println("--------------------------------------------------");
  Serial.println("Sending HTTP request...");
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    Serial.print("Payload request- ");
    Serial.println(HTTP_PayloadRequest);
    Serial.print("Server request - ");
    Serial.println(HTTP_ServerPath);
  
    Serial.println("--------------------------------------------------");
    Serial.println("Request processing ...");
    http.begin(HTTP_ServerPath.c_str());
    int HTTP_ResponseCode = http.GET();
    
    if (HTTP_ResponseCode > 0 ) {
      if (HTTP_ResponseCode == 200) {
        Serial.println("HTTP REQUEST SENDED SUCCESSFUL");
        Serial.println("HTTP REQUEST READING SUCCESSFUL");
        Serial.print("Response code: ");
        Serial.println(HTTP_ResponseCode);

        Serial.println("--------------------------------------------------");
        Serial.println("Server response has been recevied");
        String HTTP_EncodePayloadResponse = http.getString();
        String HTTP_PayloadResponse = HTTP_UrlDecode(HTTP_EncodePayloadResponse);
        Serial.print("Server response - ");
        Serial.println(HTTP_EncodePayloadResponse);
        Serial.print("Payload response - ");
        Serial.println(HTTP_PayloadResponse);
        Serial.println("--------------------------------------------------");

        Serial.println("===========================================================");
        return HTTP_PayloadResponse;
      } else {

        Serial.println("HTTP REQUEST SENDED SUCCESSFUL");
        Serial.println("HTTP REQUEST READING FAILED");
        Serial.print("Response code: ");
        Serial.println(HTTP_ResponseCode);
        Serial.println("--------------------------------------------------");

        Serial.println("===========================================================");
        return "0";
      }
    }
    else {

      Serial.println("HTTP REQUEST SEND FAILED");
      Serial.print("Error code: ");
      Serial.println(HTTP_ResponseCode);
      Serial.println("--------------------------------------------------");

      Serial.println("===========================================================");
      return "-1";
    }

    http.end();
  }
  else {
    Serial.println("HTTP REQUEST OPERATION FAILED");
    Serial.println("Reason - Wifi disconnected");
    Serial.println("--------------------------------------------------");

    Serial.println("===========================================================");
    return "-2";
  }
  HTTP_LastTime = millis();
}

String HTTP_UrlEncode(String str) {
  String encodedString="";
  char c;
  char code0;
  char code1;
  char code2;

  for (int i =0; i < str.length(); i++) {
    c=str.charAt(i);
    if (c == ' ') {
      encodedString+= '+';
    } else if (isalnum(c)) {
      encodedString+=c;
    } else{
      code1=(c & 0xf)+'0';
      if ((c & 0xf) >9){
          code1=(c & 0xf) - 10 + 'A';
      }
      c=(c>>4)&0xf;
      code0=c+'0';
      if (c > 9){
          code0=c - 10 + 'A';
      }
      code2='\0';
      encodedString+='%';
      encodedString+=code0;
      encodedString+=code1;
    }
    yield();
  }

  return encodedString;
}

String HTTP_UrlDecode(String str) {
  String encodedString="";
  char c;
  char code0;
  char code1;

  for (int i =0; i < str.length(); i++) {
      c=str.charAt(i);
    if (c == '+') {
      encodedString+=' ';  
    } else if (c == '%') {
      i++;
      code0=str.charAt(i);
      i++;
      code1=str.charAt(i);
      c = (HTTP_UrlDecodeH2Int(code0) << 4) | HTTP_UrlDecodeH2Int(code1);
      encodedString+=c;
    } else{
      encodedString+=c;  
    }  
    yield();
  }
  
  return encodedString;
}

unsigned char HTTP_UrlDecodeH2Int(char c) {
    if (c >= '0' && c <='9') {
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f') {
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F') {
        return((unsigned char)c - 'A' + 10);
    }

    return(0);
}