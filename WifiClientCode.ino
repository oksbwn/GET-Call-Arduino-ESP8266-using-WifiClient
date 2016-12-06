#include <ESP8266WiFi.h>

const char* ssid     = "oksbwn-world"; //Wi-Fi SSID
const char* password = "omisoksbwn"; //Wi-Fi Password

//Only used if using Static IP
IPAddress ip(192, 168, 0, 6); //IP
IPAddress gatewayDNS(192, 168, 0, 1);//DNS and Gewateway
IPAddress netmask(255, 255, 255,0); //Netmask

//Server IP or domain name
const char* host = "192.168.0.1";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip,gatewayDNS,netmask,gatewayDNS); //Only used if using Static IP 
  WiFi.begin(ssid, password); //Connecting to the network
  
  while (WiFi.status() != WL_CONNECTED) { //Wait till connects
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  //Serial.println(WiFi.localIP()); //Use if using DHCP to know the IP
}

void loop() {
  delay(5000);

  Serial.print("connecting to ");
  Serial.println(host);
  
  WiFiClient client; //Client to handle TCP Connection
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) { //Connect to server using port httpPort
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/test/hello.php?data1=787348&data2=7387"; //File or Server page you want to communicate with. along with data
  
  // This will send the request to the server 
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 25000) { //Try to fetch response for 25 seconds
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop(); //Close Connection
}


