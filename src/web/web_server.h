/*
 * ECG Web Server Class Header
 * 
 * Handles WiFi connection, web server setup, and HTTP endpoints
 * for real-time ECG data visualization
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

class ECGWebServer {
private:
  WebServer server;
  bool wifiConnected;
  bool serverStarted;
  
  // Current data
  int currentECGValue;
  int currentHeartRate;
  int currentSignalQuality;
  bool leadsConnected;
  unsigned long lastDataUpdate;
  
  // Internal methods
  bool connectToWiFi();
  void setupRoutes();
  String generateHTML();
  
  // Route handlers
  void handleRoot();
  void handleData();
  void handleStatus();
  void handleNotFound();
  
public:
  // Constructor
  ECGWebServer();
  
  // Initialize WiFi and web server
  bool begin();
  
  // Handle client requests
  void handleClient();
  
  // Update ECG data
  void updateECGData(int ecgValue, int heartRate, int signalQuality);
  
  // Update lead connection status
  void updateLeadStatus(bool connected);
  
  // Get connection status
  bool isWiFiConnected() { return wifiConnected; }
  bool isServerRunning() { return serverStarted; }
  
  // Get IP address
  String getIPAddress();
  
  // Get connection info
  String getConnectionInfo();
};
