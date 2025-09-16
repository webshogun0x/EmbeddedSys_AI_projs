//..
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET -1  // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include <HardwareSerial.h>

// ------------------ Pin Configuration ------------------
#define SIM800_RX 20  // ESP32 RX ← SIM800 TX
#define SIM800_TX 21  // ESP32 TX → SIM800 RX
#define BUTTON_PIN 5  // Updated: Button moved to GPIO 7

#define LED_PIN 8     // Onboard LED or GPS fix indicator

// ------------------ Global Instances ------------------
HardwareSerial sim800l(1);     // SIM800L on UART2

// ------------------ Button Debounce ------------------
bool buttonPressed = false;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// ------------------ Status Flags ------------------
bool simReady = false;

// ------------------ Function: Check SIM800L ------------------
bool checkSIM800L() {
  Serial.println("Checking SIM800L...");
  for (int i = 0; i < 5; i++) {
    sim800l.println("AT");
    delay(500);
    while (sim800l.available()) {
      String res = sim800l.readStringUntil('\n');
      res.trim();
      if (res == "OK") return true;
    }
    Serial.println("Retrying SIM800L...");
    delay(1000);
  }
  return false;
}


// ------------------ Function: Send SMS to Multiple Numbers ------------------
void sendSMS() {
  String sms = "VISITOR ALERT!!!";

  // List of recipient phone numbers
  String phoneNumbers[] = {
    "+2347043773235"
  };

  // Send SMS to each number
  for (int i = 0; i < 3; i++) {
    Serial.print("Sending to: ");
    Serial.println(phoneNumbers[i]);

    sim800l.println("AT+CMGF=1");  // Text mode
    delay(500);
    sim800l.print("AT+CMGS=\"");
    sim800l.print(phoneNumbers[i]);
    sim800l.println("\"");
    delay(500);
    sim800l.print(sms);
    sim800l.write(26);  // Ctrl+Z to send
    delay(6000);        // Wait to send
  }

  // Blink LED to indicate SMS sent
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, LOW);
    delay(200);
    digitalWrite(LED_PIN, HIGH);
    delay(200);
  }

  Serial.println("All SMS sent successfully.");
}

// ------------------ Setup ------------------
void setup() {
  Serial.begin(115200);
  esp_reset_reason_t reason = esp_reset_reason();
  Serial.print("Reset reason: ");
  Serial.println(reason);

  sim800l.begin(9600, SERIAL_8N1, SIM800_RX, SIM800_TX);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // Start with LED off
  Serial.println("System initializing...");

  simReady = checkSIM800L();

  if (!simReady) Serial.println("SIM800L not responding.");

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // Clear the buffer.
  display.clearDisplay();

  // Display Text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 28);
  display.println("SMART-MAT ...");
  display.display();
  delay(2000);
  display.clearDisplay();
  Serial.println("System ready.");
  // initialize the OLED object
}

// ------------------ Loop ------------------
void loop() {

  // Debounced button check
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && !buttonPressed) {
      buttonPressed = true;
      sendSMS();
    } else if (reading == HIGH) {
      buttonPressed = false;
    }
  }
  lastButtonState = reading;
}
