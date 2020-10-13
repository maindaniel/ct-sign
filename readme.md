#Hacktoberfest Light-up Sign

### About
Ever wanted to make a color changing sign? How about one that you can control with a few clicks on a website? What about a sign in the shape of the Hacktoberfest logo?

Those are the questions that led us to start this project. We are a couple of software developers that discovered Hacktoberfest a few years ago and have been trying to think of a perfect, month-long project for us to challenge ourselves.

Our goal with this project is to create an LED Hacktoberfest sign that can be wirelessly programmed to display any color for the different sections of the sign. We plan on using an ESP8266 to host a web server to allow users to select the colors for the sign. The device will also power a strip of LEDs and set each one to the appropriate color. Once we have these created, we'll design a sign to create using our 3D printers and CNC.

Made by [zsimpso](https://github.com/zsimpso) and [maindaniel](https://github.com/maindaniel).

### Setup
To get started copy `include/env.dist.h` to `include/env.h` and make sure to populate the variables properly.

Also, you may need to add and set a new env in the platform.io file for whatever board you are using locally

#### Hardware
For this project you'll need somne kind of ESP8266 based or compatible microcontroller (Adafruit Huzzah or NodeMCU for instance), as well as WS2812B leds/strips. Refer to [this adafruit guide](https://learn.adafruit.com/adafruit-neopixel-uberguide/basic-connections) for wiring informtation.