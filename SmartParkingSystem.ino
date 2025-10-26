#include <WiFi.h>
#include <WebServer.h>


#define TRIG1 13 
#define ECHO1 12
#define TRIG2 14 
#define ECHO2 27

#define IR_ENTRY 33 
#define IR_EXIT 32  


#define GREEN1 18 
#define RED1 19
#define GREEN2 21 
#define RED2 22


const char* ssid = "parshva2005-5G";
const char* password = "Parshva26";

WebServer server(80);


int vehicleCount = 0;
const int MAX_SLOTS = 2; 
const int slotDistance = 10; 


bool entryGateBlocked = false;
bool exitGateBlocked = false;


long readUltrasonic(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH, 30000); 
    if (duration == 0) return 999; 
    return duration * 0.034 / 2; 
}


String getHTMLPage() {
    int dist1 = readUltrasonic(TRIG1, ECHO1);
    int dist2 = readUltrasonic(TRIG2, ECHO2);

   
    bool isSlot1Occupied = (dist1 < slotDistance && dist1 > 0);
    bool isSlot2Occupied = (dist2 < slotDistance && dist2 > 0);

    String slot1Status = isSlot1Occupied ? "Occupied 🔴" : "Available 🟢";
    String slot2Status = isSlot2Occupied ? "Occupied 🔴" : "Available 🟢";
    
    
    int slotsAvailable = (isSlot1Occupied ? 0 : 1) + (isSlot2Occupied ? 0 : 1);
    
   
    String overallStatus = (slotsAvailable > 0) ? "AVAILABLE" : "FULL";
    String statusColor = (slotsAvailable > 0) ? "green" : "red";

    
    String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='3'/>"
                  "<title>Smart Parking System</title>"
                  "<style>"
                  "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; background: #e0f7fa; color: #333; }"
                  ".container { max-width: 600px; margin: 40px auto; padding: 20px; background: white; border-radius: 15px; box-shadow: 0px 8px 15px rgba(0,0,0,0.1); }"
                  "h1 { color: #00796b; border-bottom: 2px solid #00796b; padding-bottom: 10px; }"
                  ".status-box { padding: 15px; margin: 10px 0; border-radius: 8px; font-size: 1.2em; font-weight: bold; display: flex; justify-content: space-between; align-items: center; }"
                  ".available { background: #e8f5e9; color: #388e3c; border-left: 5px solid #388e3c; }"
                  ".occupied { background: #ffebee; color: #d32f2f; border-left: 5px solid #d32f2f; }"
                  ".count-box { background: #00bcd4; color: white; padding: 20px; border-radius: 10px; margin: 20px 0; font-size: 1.5em; }"
                  ".overall-status { padding: 15px; border-radius: 8px; font-size: 1.5em; font-weight: bold; background: #" + statusColor + "; color: white; }"
                  ".dist-info { font-size: 0.8em; color: #777; margin-top: 15px; }"
                  "</style></head><body><div class='container'>"
                  "<h1>🚗 Smart Parking Dashboard</h1>"
                  
                  "<div class='overall-status' style='background-color: " + statusColor + ";'>System Status: " + overallStatus + " (" + String(slotsAvailable) + " Available)</div>"

                  "<div class='status-box " + (isSlot1Occupied ? "occupied" : "available") + "'>"
                  "<span>Parking Slot 1:</span><span>" + slot1Status + "</span></div>"
                  
                  "<div class='status-box " + (isSlot2Occupied ? "occupied" : "available") + "'>"
                  "<span>Parking Slot 2:</span><span>" + slot2Status + "</span></div>"
                  
                  "<div class='count-box'>Total Vehicles Inside (IR Count): " + String(vehicleCount) + "</div>"
                  
                  "<div class='dist-info'>Debug Distances: Slot 1: " + String(dist1) + " cm | Slot 2: " + String(dist2) + " cm</div>"
                  
                  "</div></body></html>";
    return html;
}

void handleRoot() {
    server.send(200, "text/html", getHTMLPage());
}


void setup() {
    Serial.begin(115200);

   
    pinMode(TRIG1, OUTPUT); pinMode(ECHO1, INPUT);
    pinMode(TRIG2, OUTPUT); pinMode(ECHO2, INPUT);

   
    pinMode(IR_ENTRY, INPUT_PULLUP);
    pinMode(IR_EXIT, INPUT_PULLUP);

    
    pinMode(GREEN1, OUTPUT); pinMode(RED1, OUTPUT);
    pinMode(GREEN2, OUTPUT); pinMode(RED2, OUTPUT);

    
    Serial.print("Connecting to WiFi ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); 

   
    server.on("/", handleRoot);
    server.begin();
}


void loop() {
   
    server.handleClient();

   
    int dist1 = readUltrasonic(TRIG1, ECHO1);
    int dist2 = readUltrasonic(TRIG2, ECHO2);

   
    if (dist1 < slotDistance && dist1 > 0) {
        digitalWrite(RED1, HIGH);
        digitalWrite(GREEN1, LOW);
    } else {
        digitalWrite(RED1, LOW);
        digitalWrite(GREEN1, HIGH); 
    }

    
    if (dist2 < slotDistance && dist2 > 0) {
        digitalWrite(RED2, HIGH);   
        digitalWrite(GREEN2, LOW);
    } else {
        digitalWrite(RED2, LOW);
        digitalWrite(GREEN2, HIGH); 
    }

   
    if (digitalRead(IR_ENTRY) == LOW && !entryGateBlocked) {
        vehicleCount++;
        entryGateBlocked = true; 
        Serial.println("Vehicle Entered. Count: " + String(vehicleCount));
    } else if (digitalRead(IR_ENTRY) == HIGH && entryGateBlocked) {
        
        entryGateBlocked = false;
    }

  
    if (digitalRead(IR_EXIT) == LOW && !exitGateBlocked && vehicleCount > 0) {
      
        vehicleCount--;
        exitGateBlocked = true; 
        Serial.println("Vehicle Exited. Count: " + String(vehicleCount));
    } else if (digitalRead(IR_EXIT) == HIGH && exitGateBlocked) {
       
        exitGateBlocked = false;
    }

    // Optional: Add a small delay to prevent excessive loop speed, though it's usually fine without it
    // delay(50); 
}
