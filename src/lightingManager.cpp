#include <lightingManager.h>
#include <ArduinoJson.h>
#include <string.h>
#include <FastLED.h>

#define NUM_LEDS      150
#define LED_TYPE   WS2811
#define COLOR_ORDER   GRB
#define DATA_PIN        3
#define VOLTS          12
#define MAX_MA       4000

CRGBArray<NUM_LEDS> leds;

LightingManager::LightingManager() {
    _pattern = LightingPattern();

    delay( 3000 ); //safety startup delay
    FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip);
}

void LightingManager::setLightingPattern( const char* payload ) {
    _pattern = LightingPattern();
    StaticJsonDocument<2048> doc;
    deserializeJson(doc, payload);

    if(doc.containsKey("brightness")){
        int brightness = doc["brightness"].as<int>();
        // Serial.print("Brightness: ");
        // Serial.println(brightness);
        _pattern.brightness = brightness;
    }

    if(doc.containsKey("pattern")){
        JsonArray array = doc["pattern"].as<JsonArray>();
        // Serial.println("Pattern Segments:");
        for(JsonVariant v : array) {
            int start = 0;
            int end = 0;
            int color = 0;
            
            if(v.containsKey("s")) {
                // Serial.print("\t start: ");
                start = v["s"].as<int>();
                // Serial.println(start);
            }
            if(v.containsKey("e")) {
                // Serial.print("\t end: ");
                end = v["e"].as<int>();
                // Serial.println(end);
            }
            if(v.containsKey("c")) {
                // Serial.println("\t color: ");
                String colorString = v["c"].as<String>();
                color = strtol( &colorString[1], NULL, 16);

                // Split them up into r, g, b values for debug
                // int r = color >> 16;
                // int g = color >> 8 & 0xFF;
                // int b = color & 0xFF;
                // Serial.print("\t\t r:");
                // Serial.println(r);
                // Serial.print("\t\t g:");
                // Serial.println(g);
                // Serial.print("\t\t b:");
                // Serial.println(b);
            }


            LightingSegment segment = LightingSegment(start, end, color);

            // Serial.println(segment.getStartLED());
            // Serial.println(segment.getEndLED());
            // Serial.println(segment.getColor());

            _pattern.addSegment(segment);

            
        }
    }
};

void LightingManager::writeLightingToStrip() {
    FastLED.setBrightness(_pattern.brightness);

    for(LightingSegment segment: _pattern.segments) {
        int start = segment.getStartLED();
        int end = segment.getEndLED();

        if(start == 0 && end == 0) {
            continue;
        }

        int segR = segment.getColorR();
        int segG = segment.getColorG();
        int segB = segment.getColorB();

        for(int i = start; i <= end; i++) {
            leds[i].r = segR;
            leds[i].g = segG;
            leds[i].b = segB;
        }
    }

    FastLED.show();

    return;
}

String LightingPattern::toJSONString() {
    StaticJsonDocument<2048> doc;
    doc["brightness"] = brightness;

    JsonArray pattern = doc.createNestedArray("pattern");

    for(LightingSegment segment: segments) {
        int start = segment.getStartLED();
        int end = segment.getEndLED();
        int color = segment.getColor();

        if(start == 0 && end == 0) {
            continue;
        }

        JsonObject segmentObject = pattern.createNestedObject();

        segmentObject["s"]  = start;
        // Serial.println(start);
        segmentObject["e"]    = end;
        // Serial.println(end);
        char colorBuffer[24];
        snprintf(colorBuffer, sizeof(colorBuffer), "#%06X", color);
        segmentObject["c"]        = colorBuffer;
        // Serial.println(colorBuffer);
    }

    String retVal;
    serializeJsonPretty(doc, retVal);
    return retVal;
};
