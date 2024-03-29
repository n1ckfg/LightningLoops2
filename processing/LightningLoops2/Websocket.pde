import websockets.*;

WebsocketClient wsc;
int wsNow;
int wsInterval = 83;
String wsUrl = "ws://vr.fox-gieg.com:8080";
float wsGlobalScale = 1;
PVector wsGlobalOffset = new PVector(0, 0, 0);
boolean useWebsockets = false;
int targetProjector = 0;

void wsSetup() {
  wsc = new WebsocketClient(this, wsUrl);
  wsNow = millis();
}

void wsUpdate() {
  if(millis() > wsNow +  wsInterval) {
    try {
      wsc.sendMessage("clientRequestFrame");
    } catch (Exception e) {
      println("Error sending ws message: " + e);
    }
    wsNow = millis();
  }
}

void webSocketEvent(String msg) {
  try {
    //println(msg);
    JSONArray json = parseJSONArray(msg);
    
    for (int i=0; i<json.size(); i++) {
      JSONObject jsonStroke = (JSONObject) json.get(i);
      
      int r = int(255.0 * jsonStroke.getJSONArray("color").getFloat(0));
      int g = int(255.0 * jsonStroke.getJSONArray("color").getFloat(1));
      int b = int(255.0 * jsonStroke.getJSONArray("color").getFloat(2));
      color c = color(r,g,b);
      
      ArrayList<PVector> points = new ArrayList<PVector>();
      for (int m=0; m<jsonStroke.getJSONArray("points").size(); m++) {
        JSONObject jsonLNPoint = (JSONObject) jsonStroke.getJSONArray("points").get(m);
        float x = jsonLNPoint.getJSONArray("co").getFloat(0);
        float y = jsonLNPoint.getJSONArray("co").getFloat(1);
        float z = jsonLNPoint.getJSONArray("co").getFloat(2);
        PVector p = new PVector(x, y, -z).mult(wsGlobalScale).add(wsGlobalOffset);
        //println(p.x + " " + p.y + " " + p.z);
        points.add(p);
      }
      
      Stroke newStroke = new Stroke(i, c, points, globalLifespan);
      frameProjector.get(targetProjector).strokesBuffer.add(newStroke);
    
      /*
      int time = millis();
      for (int j=0; j<strokesBuffer.size(); j++) {
        Stroke s = strokesBuffer.get(i);
        if (time > s.timestamp + s.lifespan) {
          strokesBuffer.remove(j);
        }
      }
      */
    }
  } catch (Exception e) {
    println("Error receiving ws message: " + e);
  }
}
