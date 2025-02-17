# Fingerprint-Based Door Lock System

## Description
The **Fingerprint-Based Door Lock System** provides secure, keyless access using fingerprint recognition. The system uses an AS608 fingerprint sensor connected to an Arduino to verify the fingerprint of authorized users before granting access. This automated system enhances security by ensuring only registered fingerprints can unlock the door, making it ideal for homes, offices, and other secure areas.

## Components Used
- **Arduino Uno**: Microcontroller for controlling the fingerprint sensor and door lock.
- **AS608 Fingerprint Sensor**: Captures and stores fingerprint data for user verification.
- **Servo Motor**: Controls the locking mechanism of the door.
- **Jumper wires**: For wiring the components together.
- **Power Supply (Battery)**: Powers the entire system.

## Key Features
- **Fingerprint Recognition**: Unlocks the door only for registered fingerprints.
- **Keyless Access**: Eliminates the need for physical keys.
- **Secure**: Provides a high level of security by allowing only authorized users.
- **Automated Control**: Door locks and unlocks automatically based on fingerprint input.

## Installation & Setup
1. **Hardware Setup**:
   - Connect the fingerprint sensor to the Arduino (TX, RX pins).
   - Attach the servo motor to the door lock mechanism.
   - Power the system using a suitable battery.

2. **Software Setup**:
   - Download and install the Arduino IDE if not already installed.
   - Open the Arduino IDE and upload the code to your Arduino Uno.

3. **Fingerprint Registration**:
   - Use the Arduino serial monitor to register your fingerprint in the system.
   - Save multiple fingerprints for different users.

4. **Run the System**:
   - Once setup is complete, place your finger on the sensor to authenticate and unlock the door.

![ezbot install](ezbot_install.png)

## Source Code:

```cpp
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>

LiquidCrystal_I2C dis(0x27, 16, 2);
SoftwareSerial mySerial(2, 3); // TX/RX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup() {
  Serial.begin(9600);
  dis.init();
  dis.backlight();
  dis.setCursor(1, 0);
  dis.print("Place your");
  dis.setCursor(7, 1);
  dis.print("finger->");
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getParameters();
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop() { // run over and over again
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
    return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);

  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  dis.clear();
  dis.setCursor(0, 0);
  dis.print("ID: ");
  dis.print(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    dis.print(" Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}
```

## Final Setup:
![ezbot install](ezbot_install.png)

## How It Works:
1. The fingerprint sensor scans the user's fingerprint.
2. The Arduino compares the scanned fingerprint with the stored templates.
3. If a match is found, the servo motor unlocks the door.
4. After a few seconds, the servo motor locks the door again.

## Future Scope:
- Add more users by storing additional fingerprints.
- Integrate with a mobile app for remote access control.
- Implement advanced features like email notifications for access attempts.
