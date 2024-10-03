#include <Wire.h>

#define WHO_AM_I_REG 0x00  // WHO_AM_I register, change if necessary based on device

void setup() {
  delay(5000);
  Serial.begin(115200);
  while (!Serial);  // Leonardo: wait for serial monitor
  // Serial.println("\n\nI2C Scanner to scan for devices on each port pair D0 to D7");
  scanPorts();
}

uint8_t portArray[] = {16, 5, 4, 0, 2, 14, 12, 13};
String portMap[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"};  // for Wemos
// String portMap[] = {"GPIO16", "GPIO5", "GPIO4", "GPIO0", "GPIO2", "GPIO14", "GPIO12", "GPIO13"};

void scanPorts() {
  for (uint8_t i = 0; i < sizeof(portArray); i++) {
    for (uint8_t j = 0; j < sizeof(portArray); j++) {
      if (i != j) {
        // Serial.print("Scanning (SDA : SCL) - " + portMap[i] + " : " + portMap[j] + " - ");
        Wire.begin(portArray[i], portArray[j]);
        check_if_exist_I2C();
      }
    }
  }
}

void check_if_exist_I2C() {
  byte error, address;
  int nDevices;
  nDevices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      // Try reading WHO_AM_I register
      byte whoami = readWhoAmI(address);
      if (whoami != 0xFF) {
        Serial.print("WHO_AM_I register: 0x");
        Serial.println(whoami, HEX);
      } else {
        Serial.println("WHO_AM_I register not accessible");
      }

      nDevices++;
    }
    // Commented out error reporting
    // else if (error == 4) {
    //   Serial.print("Unknown error at address 0x");
    //   if (address < 16)
    //     Serial.print("0");
    //   Serial.println(address, HEX);
    // }
  }

  // Commented out no devices found message
  // if (nDevices == 0)
  //   Serial.println("No I2C devices found");
  // else
  // Serial.println("**********************************\n");
}

byte readWhoAmI(byte address) {
  Wire.beginTransmission(address);
  Wire.write(WHO_AM_I_REG);  // Request WHO_AM_I register
  if (Wire.endTransmission() != 0) {
    return 0xFF;  // Return 0xFF if WHO_AM_I register is not accessible
  }

  Wire.requestFrom(address, (byte)1);  // Read one byte from WHO_AM_I register
  if (Wire.available()) {
    return Wire.read();  // Return the WHO_AM_I value
  }
  return 0xFF;  // Return 0xFF if no response
}

void loop() {
  // Nothing to do in loop
}
