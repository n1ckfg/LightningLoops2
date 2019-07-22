import websockets.*;

WebsocketClient wsc;
int wsNow;
int wsInterval = 1000;
String wsUrl = "ws://localhost:8090"; //"ws://vr.fox-gieg.com:8080";

void wsSetup() {  
  wsc = new WebsocketClient(this, wsUrl);
  wsNow = millis();
}

void wsUpdate() {
  if(millis() > wsNow +  wsInterval) {
    wsc.sendMessage("clientRequestFrame");
    wsNow = millis();
  }
}

void webSocketEvent(String msg) {
  println(msg);
}
