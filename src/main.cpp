/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <env.h>
#include <ledExample.h>
#include <lightingManager.h>
#include <LittleFS.h>

ESP8266WebServer server ( 80 );

// If true runs example FastLED lighting
const bool exampleLeds = false;

LightingManager lightManager;

void handleLighting () {
	HTTPMethod method = server.method();

	Serial.println (method);

	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
	
	if (method == HTTP_POST) {
		Serial.println ( "" );
		Serial.println ( "Got lighting payload!" );

		//Serial.println(server.arg(0));

		const char* payload = server.arg(0).c_str();

		lightManager.setLightingPattern(payload);

		server.send ( 200, "application/json",  lightManager.getLightingPattern().toJSONString());

		lightManager.writeLightingToStrip();
		return;
	}
	else if (method == HTTP_GET) {
		Serial.println ( "" );
		Serial.println ( "Got lighting request!" );

		String pattern = lightManager.getLightingPattern().toJSONString();

		server.send ( 200, "application/json", pattern );
		return;
	}
	else if (method == HTTP_OPTIONS) {
		server.send (200, "text/plain", "");
	}
	else {
		Serial.println ( "" );
		Serial.println ( "Got bad request!" );

		server.send ( 400, "text/plain");
		return;
	}
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (LittleFS.exists(path)) {                            // If the file exists
    File file = LittleFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );
}

void setup ( void ) {
	ESP.wdtDisable();
	Serial.begin ( 115200 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );
	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

	if ( MDNS.begin ( "esp8266" ) ) {
		Serial.println ( "MDNS responder started" );
	}

	LittleFS.begin();

	server.on ("/lighting", handleLighting );
	
	server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      handleNotFound();
  	});

	server.begin();
	Serial.println ( "HTTP server started" );
	if(exampleLeds) {
		fastLEDExampleSetup();
	}
	else {
		lightManager = LightingManager();
	}
}

void loop ( void ) {
	server.handleClient();
	if(exampleLeds) {
		fastLEDExampleLoop();
	}
}

