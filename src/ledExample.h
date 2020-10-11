#ifndef LED_H
#define LED_H

#include "FastLED.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

void fastLEDExampleSetup();
void fastLEDExampleLoop();

#endif