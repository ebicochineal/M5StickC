#include <M5StickC.h>
#include <WiFiUdp.h>
#include <WiFi.h>
const char* ssid = "mywifi";
const char* password = "password";
int port = 25001;
IPAddress pc_ipaddress(192, 168, 1, 0);
IPAddress m5stick_ipaddress(192, 168, 1, 1);
WiFiUDP udp;

byte drawbuff[25600];
byte buff[1281];

bool redraws[20];
byte sendbuff[8];

void connectWiFi() {
    //WiFi.softAPdisconnect(true);
    M5.Lcd.drawString("Connecting", 8, 8);
    //M5.Lcd.println("Connecting");
    // M5.Lcd.println(ssid);
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        // Serial.print(".");
    }
    // M5.Lcd.println("");
    // M5.Lcd.println("Connected");
    M5.Lcd.drawString("Connected   ", 8, 8);
    WiFi.config(m5stick_ipaddress, WiFi.gatewayIP(), WiFi.subnetMask());
    // M5.Lcd.println("IP address: ");
    // M5.Lcd.println(WiFi.localIP());
    udp.begin(port);
}

void sendReDraw () {
    udp.beginPacket(pc_ipaddress, port);
    
    int p = 7;
    byte t = 0;
    int len = 0;
    for (int i = 0; i < 20; ++i) {
        if (redraws[i]) { t = t | (1 << p); }
        
        p -= 1;
        if (p < 0) {
            sendbuff[len] = (byte)t;
            len += 1;
            t = 0;
            p = 7;
        }
    }
    sendbuff[len] = (byte)t;
    udp.write(sendbuff, len+1);
    udp.endPacket();
}

void setup() {
    // pinMode(10, OUTPUT);
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(8);
    connectWiFi();
    
    for (int i = 0; i < 20; ++i) {
        redraws[i] = true;
    }
}

void loop() {
    int buffsize = udp.parsePacket();
    if (buffsize) {
        int len = udp.read(buff, 1281);
        
        const int k = buff[1280];
        if (redraws[k] == true) {
            for (int i = 0; i < 1280; ++i) {
                drawbuff[1280*k+i] = buff[i];
            }
            
            redraws[k] = false;
            int cnt = 0;
            for (int i = 0; i < 20; ++i) {
                if (redraws[i] == true) { cnt += 1; }
            }
            if (cnt == 0) {
                M5.Lcd.setAddrWindow(0, 0, 160, 80); 
                M5.Lcd.pushColors(drawbuff, 25600);
                
                for (int i = 0; i < 20; ++i) { redraws[i] = true; }
            }
            
        }
        sendReDraw();
        
    }
}