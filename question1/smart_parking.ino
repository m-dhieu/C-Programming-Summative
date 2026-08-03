/*
 Smart Parking Lot Monitoring System
 
 This is a 4-space smart parking shield for Arduino Uno tracking
 vehicle occupancy and displaying availability.
 */

struct ParkingSpace {
    uint8_t id;
    uint8_t buttonPin;
    uint8_t ledPin;
    bool isOccupied;
    bool lastButtonState;
};

const uint8_t TOTAL_SPACES = 4;
unsigned long lastDisplayTime = 0;
const unsigned long DISPLAY_INTERVAL = 2000; 

ParkingSpace* parkingLot = nullptr;
uint8_t occupiedCount = 0;
uint8_t availableCount = TOTAL_SPACES;

void setup() {
    Serial.begin(9600);
    parkingLot = new ParkingSpace[TOTAL_SPACES];
    
    for (uint8_t i = 0; i < TOTAL_SPACES; i++) {
        (parkingLot + i)->id = i + 1;
        (parkingLot + i)->buttonPin = i + 2;  
        (parkingLot + i)->ledPin = i + 8;     
        (parkingLot + i)->isOccupied = false;
        (parkingLot + i)->lastButtonState = LOW;
        
        pinMode((parkingLot + i)->buttonPin, INPUT);
        pinMode((parkingLot + i)->ledPin, OUTPUT);
        digitalWrite((parkingLot + i)->ledPin, LOW);
    }
    Serial.println(F("Smart Parking Lot Monitoring System is Active."));
}

void loop() {
    unsigned long currentMillis = millis();
    
    for (uint8_t i = 0; i < TOTAL_SPACES; i++) {
        ParkingSpace* space = parkingLot + i;
        int reading = digitalRead(space->buttonPin);
        
        // fire on click
        if (reading == HIGH && space->lastButtonState == LOW) {
            if (!space->isOccupied) {
                if (occupiedCount < TOTAL_SPACES) {
                    space->isOccupied = true;
                    digitalWrite(space->ledPin, HIGH); 
                    occupiedCount++;
                    availableCount--;
                    Serial.print(F("ALERT: Vehicle entered Space "));
                    Serial.println(space->id);
                } else {
                    Serial.println(F("ERROR: Capacity Exceeded! Cannot park vehicle."));
                }
            } else {
                if (occupiedCount > 0) {
                    space->isOccupied = false;
                    digitalWrite(space->ledPin, LOW); 
                    occupiedCount--;
                    availableCount++;
                    Serial.print(F("ALERT: Vehicle departed Space "));
                    Serial.println(space->id);
                } else {
                    Serial.println(F("ERROR: System discrepancy detected!"));
                }
            }
            printParkingStatus();
            delay(250);
        }
        space->lastButtonState = reading;
    }
    
    if (currentMillis - lastDisplayTime >= DISPLAY_INTERVAL) {
        lastDisplayTime = currentMillis;
        printParkingStatus();
    }
}

void printParkingStatus() {
    Serial.println(F("\n--- PARKING LOT STATUS TELEMETRY ---"));
    Serial.print(F("Occupied Spaces : ")); Serial.println(occupiedCount);
    Serial.print(F("Available Spaces: ")); Serial.println(availableCount);
    Serial.print(F("Dashboard  : [ "));
    for (uint8_t i = 0; i < TOTAL_SPACES; i++) {
        Serial.print(F("S")); Serial.print((parkingLot + i)->id);
        Serial.print(F(":"));
        Serial.print((parkingLot + i)->isOccupied ? F("OCCUPIED") : F("FREE"));
        if (i < TOTAL_SPACES - 1) Serial.print(F(" | "));
    }
    Serial.println(F(" ]"));
    Serial.println(F(" "));
}

