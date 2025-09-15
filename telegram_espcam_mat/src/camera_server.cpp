#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>

WebServer server(80);

void handleRoot() {
  String html = "<html><body><h1>ESP32-CAM</h1>";
  html += "<img src='/capture' width='640' height='480'>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleCapture() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }
  
  server.sendHeader("Content-Type", "image/jpeg");
  server.sendHeader("Content-Length", String(fb->len));
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);
  
  esp_camera_fb_return(fb);
}

void startCameraServer() {
  server.on("/", handleRoot);
  server.on("/capture", handleCapture);
  server.begin();
}

void handleClient() {
  server.handleClient();
}