
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

char ssid[] = "vertigo";          // your network SSID (name)
char pass[] = "CreateIoT2018";     // your network password
//char ssid[] = "SEXED2.4G";          // your network SSID (name)
//char pass[] = "666devilHAMHAM";     // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(192, 168, 2, 2); // remote IP (not needed for receive)
const unsigned int outPort = 10000;     // remote port (not needed for receive)
const unsigned int localPort = 4210;    // local port to listen for UDP packets
const unsigned int superCollider = 57120; 

OSCErrorCode error;

// from irCommander
byte data[8];
// byte byteData[4];
unsigned long mergedNumber;
int identifier;
unsigned int id; // 8bit 0~255
int op; // 2bit 0~3, 0[==] 1[!=] 2[<] 3[>]
int address; // 3bit 0~7
unsigned long value; // 16bit 0~65535

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  wifisetup();

  // tell system that it establich the connection
  delay(1000);
  for (int i = 0; i < 2; i++) {
    OSCMessage msg("/s");
    msg.add(int(WiFi.localIP()[0]));
    msg.add(int(WiFi.localIP()[1]));
    msg.add(int(WiFi.localIP()[2]));
    msg.add(int(WiFi.localIP()[3]));
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(1000);
  }
  // Serial.println("OSC Message sent");
}

void wifisetup() {
  // Connect to WiFi network
  // Serial.println();
  // Serial.println();
  // Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
  }
  // Serial.println("");

  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  // Serial.println("Starting UDP");
  Udp.begin(localPort);
  // Serial.print("Local port: ");

#ifdef ESP32
  // Serial.println(localPort);
#else
  // Serial.println(Udp.localPort());
#endif
  delay(8000);
  id = int(WiFi.localIP()[3]);
  Serial.write(id);
  Serial.write(B01000000); // means this message is ID info
}

void loop() {
  serialUpdate();
  oscUpdate();
}
