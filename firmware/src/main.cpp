/**
 * @file main.cpp
 * @brief Main program body
 * @version 0.1
 * @date 2023-11-05
 * @author b401
 *
 * This is an OTA firmware for ESP8266
 * The ESP8266 will act as an access point without password
 * To update the firmware, connect to the access point and open the IP address in the browser
 * The IP address is 11.4.0.1
 * Just upload the firmware located on ./.pio/build/[board]/firmware.bin and wait for the ESP8266 to reboot
 *
 */

#include <Arduino.h>
#include "LittleFS.h"
#include <DNSServer.h>
#include <ElegantOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Private macro -------------------------------------------------------------*/
#define NETMASK_24 IPAddress(255, 255, 255, 0)
#define LED_PIN 2
#define RGB_LED_PIN_RED 15
#define RGB_LED_PIN_GREEN 12
#define RGB_LED_PIN_BLUE 13
#define BUTTON_PIN 3

/* USER CODE BEGIN PM */
#define AP_SSID "Kelompok_01"
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
const byte DNS_PORT = 53;
IPAddress esp_32_ip(11, 4, 0, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

/* USER CODE BEGIN PV */
uint8_t led_color_rgb[3] = {0, 0, 0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void init_pin();

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

void setup()
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(esp_32_ip, esp_32_ip, NETMASK_24);
  WiFi.softAP(AP_SSID);

  // if DNSServer is started with "*" for domain name, it will reply with provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", esp_32_ip);

  // route handle here
  webServer.onNotFound([]()
                       {      
                        webServer.sendHeader("Content-Encoding", "gzip");
                        webServer.send_P(200, "text/html", (const char*)ELEGANT_HTML, sizeof(ELEGANT_HTML)); });

  ElegantOTA.begin(&webServer);

  webServer.begin();

  /* USER CODE BEGIN 2 */
  pinMode(LED_PIN, OUTPUT);
  /* USER CODE END 2 */
}

void loop()
{
  ElegantOTA.loop();
  dnsServer.processNextRequest();
  webServer.handleClient();

  /* USER CODE BEGIN 3 */
  static uint32_t last_time = 0;
  if (millis() - last_time > 1000)
  {
    last_time = millis();
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  /* USER CODE END 3 */
}

void init_pin()
{
  pinMode(RGB_LED_PIN_RED, OUTPUT);
  pinMode(RGB_LED_PIN_GREEN, OUTPUT);
  pinMode(RGB_LED_PIN_BLUE, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  delay(1);
  digitalWrite(RGB_LED_PIN_RED, LOW);
  digitalWrite(RGB_LED_PIN_GREEN, LOW);
  digitalWrite(RGB_LED_PIN_BLUE, LOW);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */