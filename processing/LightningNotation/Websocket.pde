import websockets.*;

WebsocketClient wsc;
int wsNow;
int wsInterval = 5000;
String wsUrl = "ws://vr.fox-gieg.com/socket.io/?EIO=3&transport=websocket";

void wsSetup(){  
  wsc = new WebsocketClient(this, wsUrl);
  wsNow = millis();
}

void wsUpdate(){
  if(millis() > wsNow + 5000) {
    wsc.sendMessage("Client message");
    wsNow = millis();
  }
}

void webSocketEvent(String msg) {
  println(msg);
}
