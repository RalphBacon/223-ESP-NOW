#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0xCC, 0x50, 0xE3, 0xB5, 0x9D, 0x40 };

// Structure example to send data
// Must match the receiver structure
struct struct_message {
  char a_char[32];
  int b_int;
  float c_float;
  bool d_bool;
};

// Create a struct_message called myData
struct_message myData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success\n");
  } else {
    Serial.println("Delivery fail\n");
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(300);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
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

  // Allow data to get out of the UART
  delay(2000);
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  delay(5000);
}