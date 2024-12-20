#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <EEPROM.h>

// Pin Definitions
#define DHTPIN 4
#define DHTTYPE DHT22
#define GREEN_LED 14
#define YELLOW_LED 12
#define RED_LED 27
#define BUTTON_PIN 5  // Button pin to toggle graph view
#define BUZZER_PIN 13 // Pin for buzzer

// OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT Sensor
DHT dht(DHTPIN, DHTTYPE);

// Variables
float airQuality = 0;            // Simulated Air Quality
int airQualityLevel = 0;         // 0: Good, 1: Moderate, 2: Poor
float airQualityHistory[10];     // Stores the last 10 air quality readings
int viewGraph = false;           // Flag to toggle between data view and graph view

void setup() {
  Serial.begin(115200);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.clearDisplay();

  // Initialize DHT Sensor
  dht.begin();

  // Initialize LEDs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Initialize Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Load previous air quality history from EEPROM
  loadHistory();

  display.println("Setup Complete");
  display.display();
}

void loop() {
  // Check button state
  if (digitalRead(BUTTON_PIN) == LOW) { // Button pressed
    delay(300); // Debounce delay
    viewGraph = !viewGraph; // Toggle the graph view
    while (digitalRead(BUTTON_PIN) == LOW); // Wait for button release
  }

  if (viewGraph) {
    displayGraph(); // Show the graph when viewGraph is true
  } else {
    displayRealTimeData(); // Show the real-time data when viewGraph is false
  }

  delay(500); // Delay before next iteration
}

void displayRealTimeData() {
  // Read Temperature and Humidity
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Simulate Air Quality based on Temperature
  if (temp < 40) {
    airQuality = random(50, 100); // Good air quality
    airQualityLevel = 0;          // Good
  } else if (temp < 50) {
    airQuality = random(100, 150); // Moderate air quality
    airQualityLevel = 1;           // Moderate
  } else {
    airQuality = random(150, 200); // Poor air quality
    airQualityLevel = 2;           // Poor
  }

  // Update LEDs based on air quality
  digitalWrite(GREEN_LED, airQualityLevel == 0);  // Green LED for Good air quality
  digitalWrite(YELLOW_LED, airQualityLevel == 1); // Yellow LED for Moderate
  digitalWrite(RED_LED, airQualityLevel == 2);    // Red LED for Poor

  // Update OLED Display
  display.clearDisplay();
  display.setTextSize(1); // Smaller text size for values
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Display Temperature and Humidity
  display.printf("Temp: %.1f C\n", temp);
  display.printf("Humidity: %.1f %%\n", humidity);

  // Display Air Quality value
  display.setCursor(0, 40);
  display.printf("Air Quality: %.1f\n", airQuality);

  // Add message for air quality levels
  display.setCursor(0, 55);
  if (airQualityLevel == 0) {
    display.println("Best Air Quality!");
    playTikTokTone(); // Play TikTok tone for good air quality
  } else if (airQualityLevel == 1) {
    display.println("Moderate Air Quality!");
    noTone(BUZZER_PIN); // No tone for moderate air quality
  } else if (airQualityLevel == 2) {
    display.println("Danger: Poor Air!");
    playDangerTone(); // Play Danger tone for poor air quality
  }

  display.display();

  // Save air quality to history
  saveToHistory(airQuality);
}

void displayGraph() {
  display.clearDisplay();

  // Draw Grid Lines
  drawGridLines();

  // Draw Graph
  for (int i = 0; i < 9; i++) {
    int y1 = 63 - map(airQualityHistory[i], 50, 200, 0, 63); // Map air quality value to Y-axis
    int y2 = 63 - map(airQualityHistory[i + 1], 50, 200, 0, 63);
    display.drawLine(i * 14, y1, (i + 1) * 14, y2, SSD1306_WHITE);
  }

  // Draw X and Y axis Labels
  drawXAxisLabels();
  drawYAxisLabels();

  display.display();
}

void drawGridLines() {
  for (int y = 0; y < 64; y += 10) {
    display.drawLine(0, y, 127, y, SSD1306_WHITE); // Horizontal grid lines
  }
}

void drawXAxisLabels() {
  for (int i = 0; i < 10; i++) {
    display.setCursor(i * 14, 63);
    display.print("T" + String(i)); // Example: T0, T1, T2, ..., T9
  }
}

void drawYAxisLabels() {
  for (int y = 0; y < 64; y += 10) {
    int airQualityValue = map(y, 0, 63, 200, 50); // Map Y position back to air quality value
    display.setCursor(0, y);
    display.print(airQualityValue);
  }
}

void saveToHistory(float airQualityValue) {
  // Shift air quality values in history
  for (int i = 0; i < 9; i++) {
    airQualityHistory[i] = airQualityHistory[i + 1];
  }
  airQualityHistory[9] = airQualityValue;

  // Save to EEPROM
  for (int i = 0; i < 10; i++) {
    EEPROM.put(i * sizeof(float), airQualityHistory[i]);
  }
}

void loadHistory() {
  // Load air quality history from EEPROM
  for (int i = 0; i < 10; i++) {
    EEPROM.get(i * sizeof(float), airQualityHistory[i]);
  }
}

void playTikTokTone() {
  tone(BUZZER_PIN, 1000, 200); // 1000 Hz for 200 ms
  delay(300);
  tone(BUZZER_PIN, 1500, 200); // 1500 Hz for 200 ms
  delay(300);
  noTone(BUZZER_PIN);
}

void playDangerTone() {
  for (int i = 0; i < 5; i++) {
    tone(BUZZER_PIN, 500, 300); // 500 Hz for 300 ms
    delay(400);
    noTone(BUZZER_PIN);
    delay(200);
  }
}
