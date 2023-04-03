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
  if (WiFi.status() == WL_CONNECTED) {
    String result = "";
    result += "WiFi status: " + String(WiFi.status()) + "<br>";
    result += "Local IP: " + WiFi.localIP().toString() + "<br>";

    server.send(200, "text/html", result);  
  } else {
    // See full page code in form.html file
    server.send(200, "text/html", "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Настройка точки доступа</title><style type='text/css'>.lds-roller{display:inline-block;position:relative;width:80px;height:80px}.lds-roller div{animation:lds-roller 1.2s cubic-bezier(.5,0,.5,1) infinite;transform-origin:40px 40px}.lds-roller div:after{content:' ';display:block;position:absolute;width:7px;height:7px;border-radius:50%;background:#fff;margin:-4px 0 0 -4px}.lds-roller div:nth-child(1){animation-delay:-.036s}.lds-roller div:nth-child(1):after{top:63px;left:63px}.lds-roller div:nth-child(2){animation-delay:-.072s}.lds-roller div:nth-child(2):after{top:68px;left:56px}.lds-roller div:nth-child(3){animation-delay:-.108s}.lds-roller div:nth-child(3):after{top:71px;left:48px}.lds-roller div:nth-child(4){animation-delay:-.144s}.lds-roller div:nth-child(4):after{top:72px;left:40px}.lds-roller div:nth-child(5){animation-delay:-.18s}.lds-roller div:nth-child(5):after{top:71px;left:32px}.lds-roller div:nth-child(6){animation-delay:-.216s}.lds-roller div:nth-child(6):after{top:68px;left:24px}.lds-roller div:nth-child(7){animation-delay:-.252s}.lds-roller div:nth-child(7):after{top:63px;left:17px}.lds-roller div:nth-child(8){animation-delay:-.288s}.lds-roller div:nth-child(8):after{top:56px;left:12px}@keyframes lds-roller{0%{transform:rotate(0)}100%{transform:rotate(360deg)}}</style><style type='text/css'>body{font-family:Arial,sans-serif;display:flex;flex-direction:column;justify-content:space-around;align-items:center;min-height:90vh;background:#ccc}.main-form{display:inline-block;background:#fff;padding:20px;margin:0 auto;text-align:center;border-radius:5px;position:relative}.main-form__inputs{display:flex;justify-content:space-around}input{margin-top:10px;border-radius:2px;border:2px solid transparent;background-color:#e9e9e9}input[type=submit]{font-size:18px;padding:5px 10px;cursor:pointer}input:hover{background-color:#e9e9e9}.main-form__loading{position:absolute;z-index:9;background:rgba(0,0,0,.5);width:100%;height:100%;top:0;left:0;display:flex;justify-content:center;align-items:center}.hidden{display:none}</style></head><body><div class='main-form'><form class='main-form__form'><h1>Настройка точки доступа</h1><div class='main-form__inputs'><p>Название WiFi (SSID):<br><input name='SSID' required value='temp'></p><p>Пароль:<br><input name='password' required value='temp'></p></div><input type='submit' value='Подключиться к WiFi'></form><div class='main-form__loading hidden'><div class='lds-roller'><div></div><div></div><div></div><div></div><div></div><div></div><div></div><div></div></div></div></div><script type='text/javascript'>function sendAuthData(e){var t=new XMLHttpRequest,o="/action?"+e;document.querySelector(".main-form__loading").classList.remove("hidden"),t.open("GET",o),t.onreadystatechange=function(){2==this.readyState&&console.log("Waiting for reply..."),4==this.readyState&&200==this.status&&("3"==this.responseText&&window.location.reload(),alert(this.responseText),console.log("Response: "+this.responseText),console.log(this.responseText),console.log(this))},t.send(null)}form=document.querySelector(".main-form__form"),console.log(form),form.addEventListener("submit",function(e){e.preventDefault();const t=new FormData(form);var o=new URLSearchParams(t).toString();sendAuthData(o)});</script></body></html>");
  }
}