#include "FastLED.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#define NUM_LEDS 400
#define DATA_PIN 14
#define FORWARD 0
#define BACKWARD 1
#define SLOW 100
#define MEDIUM 50
#define FAST 5
#define LO 35
#define MID 70
#define HI 150

// LED strip array.
CRGB leds[NUM_LEDS];
boolean direction = FORWARD;

// Replace with your network credentials
const char *ssid = "MySpectrumWiFi78-2G";
const char *password = "famousspoon624";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos)
{
    if (WheelPos < 85)
    {
        return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

// Theater-style crawling lights with rainbow effect
void theaterChaseRainbow(int cycles, int speed)
{
    for (int j = 0; j < 256 * cycles; j++)
    {
        // cycle all 256 colors in the wheel
        for (int q = 0; q < 3; q++)
        {
            for (int i = 0; i < NUM_LEDS; i = i + 3)
            {
                int pos = i + q;
                leds[pos] = Wheel((i + j) % 255); // turn every third pixel on
            }
            FastLED.show();

            delay(speed);

            for (int i = 0; i < NUM_LEDS; i = i + 3)
            {
                leds[i + q] = CRGB::Black; // turn every third pixel off
            }
        }
    }
}

// Random flashes of lightning
void flash(CRGB c, int simultaneous, int cycles, int speed)
{
    int flashes[simultaneous];

    for (int i = 0; i < cycles; i++)
    {
        for (int j = 0; j < simultaneous; j++)
        {
            int idx = random(NUM_LEDS);
            flashes[j] = idx;
            leds[idx] = c ? c : randomColor();
        }
        FastLED.show();
        delay(speed);
        for (int s = 0; s < simultaneous; s++)
        {
            leds[flashes[s]] = CRGB::Black;
        }
        delay(speed);
    }
}

// Rainbow colors that slowly cycle across LEDs
void rainbow(int cycles, int speed)
{
    if (cycles == 0)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = Wheel(((i * 256 / NUM_LEDS)) & 255);
        }
        FastLED.show();
    }
    else
    {
        for (int j = 0; j < 256 * cycles; j++)
        {
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
            }
            FastLED.show();
            delay(speed);
        }
    }
}

// Changes all LEDS to given color
void allColor(CRGB c)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = c;
    }
    FastLED.show();
}

void _setBrightness(int level)
{
    FastLED.setBrightness(level);
    FastLED.show();
}


CRGB randomColor()
{
    return Wheel(random(256));
}

void setup()
{
    // Serial port for debugging purposes
    Serial.begin(115200);

    // Initialize LED strip
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

    // Seed random
    randomSeed(analogRead(0));

    // Initialize SPIFFS
    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());

    // Routing for button presses
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html"); });
    server.on("/red", HTTP_GET, [](AsyncWebServerRequest *request)
              { allColor(CRGB::Red); });
    server.on("/green", HTTP_GET, [](AsyncWebServerRequest *request)
              { allColor(CRGB::Green); });
    server.on("/blue", HTTP_GET, [](AsyncWebServerRequest *request)
              { allColor(CRGB::Blue); });
    server.on("/purple", HTTP_GET, [](AsyncWebServerRequest *request)
              { allColor(CRGB::Purple); });
    server.on("/flash", HTTP_GET, [](AsyncWebServerRequest *request)
              { theaterChaseRainbow(50, MEDIUM); });
    server.on("/low", HTTP_GET, [](AsyncWebServerRequest *request)
              { _setBrightness(LO); });
    server.on("/mid", HTTP_GET, [](AsyncWebServerRequest *request)
              { _setBrightness(MID); });
    server.on("/high", HTTP_GET, [](AsyncWebServerRequest *request)
              { _setBrightness(HI); });

    // Start server
    server.begin();
}

void loop()
{
}
