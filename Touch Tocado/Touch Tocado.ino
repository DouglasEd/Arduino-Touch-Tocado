#include <WiFi.h>

WiFiServer server(80);
String header;

unsigned long CT = millis();
unsigned long PT = 0;
const long TT = 2000;

const char* SSID = "VOCEULTRA_14328_2.4G";
const char* Senha = "64036642200";

int a;

#define touch1 15
void setup() {
  Serial.begin(115200);
  pinMode(touch1, INPUT);
  Serial.print("Conectando a ");
  Serial.println(SSID);
  WiFi.begin(SSID,Senha);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  a = touchRead(touch1);
  Serial.println(a);
  if (client){
    CT = millis();
    PT = CT;
    Serial.println("Novo Usuario");
    String currentLine = "";
    while (client.connected() && CT - PT <= TT){
      CT= millis();
      if (client.available()){
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c != '\n'){
          if (currentLine.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTIPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".tocado{background-color: green;}");
            client.println("h1{text-align: center; font-size:200px}");
            client.println(".ntocado{cabkground-color: red;}</style></head>");

            if(a<50){
              client.println("<body class=\"tocado\"><h1>Touch tocado</h1></body></html>");
            }
            else{
              client.println("<body class=\"ntocado\"><h1>Touch Não tocado</h1></body></html>");
            }
            client.println();
            break;
          } else{
            currentLine = "";
          }
        }else if(c != '\r'){
          currentLine += c;
        }
      }
    }

    header = "";
  }
}