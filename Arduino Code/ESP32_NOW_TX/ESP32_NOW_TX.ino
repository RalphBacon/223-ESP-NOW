#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0xCC, 0x50, 0xE3, 0xB5, 0x9D, 0x40 };

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a_char[32];
  int b_int;
  float c_float;
  bool d_bool;
};

// Create a struct_message called myData
struct_message myData;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(300);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Set values to send
  strcpy(myData.a_char, "THIS IS A CHAR");
  myData.b_int = random(1, 20);
  myData.c_float = (float)random(10.0, 100.0) / 10.0;
  myData.d_bool = random(1, 10) < 5 ? true : false;

  // Send message via ESP-NOW
  Serial.println("Bytes transmitted: ");
  Serial.print("Char: ");
  Serial.println(myData.a_char);
  Serial.print("Int: ");
  Serial.println(myData.b_int);
  Serial.print("Float: ");
  Serial.println(myData.c_float);
  Serial.print("Bool: ");
  Serial.println(myData.d_bool ? "true" : "false");
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(5000);
}