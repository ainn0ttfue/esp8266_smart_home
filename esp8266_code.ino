#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

IPAddress local_IP(192,168,44,100);
IPAddress gateway(192,168,44,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

int connectToWiFi(String ssid, String pwd) { 
  WiFi.begin(ssid, pwd);                
 
  while (WiFi.status() != WL_CONNECTED)       
  { 
    delay(500);                               
    Serial.print(".");                       
  }

  Serial.println("");                        
  Serial.println("WiFi connected");          
 
  Serial.println("Server started");           
  Serial.print("Use this URL to connect: ");
  Serial.println(WiFi.localIP()); 

  return WiFi.status();
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/action", handleForm);
  server.on("/status", handleStatus);
  
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}

void handleForm() {
  String SSID = server.arg("SSID");
  String password = server.arg("password");

  Serial.print("SSID name: ");
  Serial.println(SSID);

  Serial.print("WiFi password: ");
  Serial.println(password);

  connectToWiFi(SSID, password);
  server.send(200, "text/html", String(WiFi.status()));
}

void handleStatus() {
  String result = "";
  result += "WiFi status: " + String(WiFi.status()) + "<br>";
  result += "Local IP: " + WiFi.localIP().toString() + "<br>";

  server.send(200, "text/html", result);
}

void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Настройка точки доступа</title><style type='text/css'>body{font-family:Arial,sans-serif;display:flex;flex-direction:column;justify-content:space-around;align-items:center;min-height:90vh;background:#ccc}.main-form{display:inline-block;background:#fff;padding:20px;margin:0 auto;text-align:center;border-radius:5px}.main-form__inputs{display:flex;justify-content:space-around}input{margin-top:10px;border-radius:2px;border:2px solid transparent;background-color:#e9e9e9}input[type=submit]{font-size:18px;padding:5px 10px;cursor:pointer}input:hover{background-color:#e9e9e9}</style></head><body><div class='main-form'><form class='main-form__form'><h1>Настройка точки доступа</h1><div class='main-form__inputs'><p>Название WiFi (SSID):<br><input name='SSID' required value='temp'></p><p>Пароль:<br><input name='password' required value='temp'></p></div><input type='submit' value='Подключиться к WiFi'></form></div><script type='text/javascript'>function sendAuthData(e){var t=new XMLHttpRequest,o='/action?'+e;console.log(o),t.open('GET',o),t.onreadystatechange=function(){2==this.readyState&&console.log('Waiting for reply...'),4==this.readyState&&200==this.status&&(console.log('Response: '+this.responseText),alert(this.responseText))},t.send(null)}form=document.querySelector('.main-form__form'),console.log(form),form.addEventListener('submit',function(e){e.preventDefault();const t=new FormData(form);var o=new URLSearchParams(t).toString();sendAuthData(o)});</script></body></html>");
}