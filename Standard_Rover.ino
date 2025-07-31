#include <BluetoothSerial.h>
#define ledPin 2

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to enable it
#endif

BluetoothSerial SerialBT;

// Define motor control pins
// Left motor (simplified with parallel connection)
const int leftMotorPin1 = 12;  // IN1 on L298N
const int leftMotorPin2 = 13;  // IN2 on L298N
const int leftMotorPWM = 14;   // ENA on L298N

// Right motor (simplified with parallel connection)
const int rightMotorPin1 = 27; // IN3 on L298N
const int rightMotorPin2 = 26; // IN4 on L298N
const int rightMotorPWM = 25;  // ENB on L298N


int valSpeed = 200;     // Default motor speed (0-255)
int minTurnSpeed = 100; // Minimum speed for effective turning

// LEDC parameters
const int freq = 5000;
const int resolution = 8;  // 8-bit resolution (0-255)

bool isConnected= false;
unsigned long lastBlinkTime=0;
bool ledState=false;

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32_CarBot"); // Bluetooth device name
    Serial.println("Bluetooth device started, you can pair it with your smartphone now!");
    
    // Set up motor control pins
    pinMode(leftMotorPin1, OUTPUT);
    pinMode(leftMotorPin2, OUTPUT);
    pinMode(rightMotorPin1, OUTPUT);
    pinMode(rightMotorPin2, OUTPUT);
    
    // Configure PWM using the new LEDC API
    // For left motor PWM
    if(!ledcAttach(leftMotorPWM, freq, resolution)) {
        Serial.println("Failed to attach left motor PWM!");
    }
    
    // For right motor PWM
    if(!ledcAttach(rightMotorPWM, freq, resolution)) {
        Serial.println("Failed to attach right motor PWM!");
    }
    
    // Initialize  LED
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    
    // Stop all motors initially
    stopMotors();
    
    Serial.println("Ready to receive commands...");
}

void loop(){ 
    char command = 0;

    // Blink LED while not connected
    if (!isConnected) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastBlinkTime >= 300) { // Blink every 300ms
            lastBlinkTime = currentMillis;
            ledState = !ledState;
            digitalWrite(ledPin, ledState);
        }
    }

    // Check Serial
    if (Serial.available() > 0) {
        command = Serial.read();
        Serial.println(command);
    }

    // Check Bluetooth
    if (SerialBT.available() > 0) {
        command = SerialBT.read();
        Serial.print("Bluetooth command: ");
        Serial.println(command);

        if (!isConnected) {
            isConnected = true;
            digitalWrite(ledPin, HIGH);  // Solid LED once connected
        }
    }

    // Process command
    if (command != 0) {
        processCommand(command);
    }
}


void processCommand(char command) {
    switch(command) {
        case 'F': {  // Move forward
            leftMotorForward();
            rightMotorForward();
            setMotorSpeeds(valSpeed, valSpeed);
            break;
        }
        case 'B': {  // Move backward
            leftMotorBackward();
            rightMotorBackward();
            setMotorSpeeds(valSpeed, valSpeed);
            break;
        }
        case 'R':{   // Turn right
            // Use higher speed for turning if current speed is too low
            int rightTurnSpeed = max(valSpeed, minTurnSpeed);
            leftMotorForward();
            rightMotorBackward();
            setMotorSpeeds(rightTurnSpeed, rightTurnSpeed);
            break;
        }
        case 'L':{   // Turn left
            // Use higher speed for turning if current speed is too low
            int leftTurnSpeed = max(valSpeed, minTurnSpeed);
            leftMotorBackward();
            rightMotorForward();
            setMotorSpeeds(leftTurnSpeed, leftTurnSpeed);
            break;
        }
        case 'G':{   // Forward left (curve)
            leftMotorForward();
            rightMotorForward();
            setMotorSpeeds(valSpeed/2, valSpeed);
            break;
        }
        case 'H':{   // Forward right (curve)
            leftMotorForward();
            rightMotorForward();
            setMotorSpeeds(valSpeed, valSpeed/2);
            break;
        }
        case 'S':{   // Stop all motors
            stopMotors();
            break;
        }

        // Set motor speed using single characters (0-9)
        case '0': valSpeed = 0; break;
        case '1': valSpeed = 50; break;   // Increased minimum speeds
        case '2': valSpeed = 75; break;
        case '3': valSpeed = 100; break;
        case '4': valSpeed = 125; break;
        case '5': valSpeed = 150; break;
        case '6': valSpeed = 175; break;
        case '7': valSpeed = 200; break;
        case '8': valSpeed = 225; break;
        case '9': valSpeed = 255; break;
    }
}

// New function to set individual motor speeds
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    ledcWrite(leftMotorPWM, leftSpeed);
    ledcWrite(rightMotorPWM, rightSpeed);
}

// Simplified setSpeed function for forward/backward movement
void setSpeed(int speed) {
    valSpeed = speed;
    setMotorSpeeds(speed, speed);
}

void leftMotorForward() {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
}

void leftMotorBackward() {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
}

void rightMotorForward() {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
}

void rightMotorBackward() {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
}

void stopMotors() {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
    setMotorSpeeds(0, 0);
}