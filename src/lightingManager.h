#ifndef LIGHTINGMANAGER_H
#define LIGHTINGMANAGER_H
#include <Wstring.h>

class LightingSegment {
    private:
        int _startLED;
        int _endLED;
        int _color;

        void SetSegment(int startLED, int endLED, int color) {
            _startLED = startLED;
            _endLED = endLED;
            _color = color;
        };

    public:
        LightingSegment() {
            _startLED = 0;
            _endLED = 0;
            _color = 0xFFFFFF;

        };
        LightingSegment(int startLED, int endLED, int color) {
            SetSegment(startLED, endLED, color);
        };

        int getStartLED() { return _startLED; }
        int getEndLED() { return _endLED; }
        int getColor()  { return _color; }
        int getColorR() { return _color >> 16; }
        int getColorG() { return _color >> 8 & 0xFF; }
        int getColorB() { return _color & 0xFF; }

};

class LightingPattern {
    private:
        int _segmentCount;

    public:
        LightingPattern() {
            _segmentCount = 0;
        };
        int brightness;
        LightingSegment segments[25];
        void addSegment(LightingSegment segment) {
            if(_segmentCount < 25) {
                segments[_segmentCount++] = segment;
            }
        };
        String toJSONString();
};

class LightingManager {
    private:
        LightingPattern _pattern;
        
    public:
        LightingManager() {
            _pattern = LightingPattern();
        };
        void setLightingPattern( const char* payload );
        LightingPattern getLightingPattern() {
            return _pattern;
        };
};

#endif