import oscP5.*;
import netP5.*;

PVector dot1 = new PVector(0,0);
PVector dot2 = new PVector(0,0);

String ipNumber = "127.0.0.1";
int sendPort = 9998;
int receivePort = 7110;
int datagramSize = 1000000;
OscP5 oscP5;
NetAddress myRemoteLocation;

void oscSetup() {
  OscProperties op = new OscProperties();
  op.setListeningPort(receivePort);
  op.setDatagramSize(datagramSize);
  oscP5 = new OscP5(this, op);
  myRemoteLocation = new NetAddress(ipNumber, sendPort);
}

void sendOscContour(String hostname, int index, color col, ArrayList<PVector> points) {
  OscMessage msg = new OscMessage("/contour");
  
  msg.add(hostname);
  msg.add(index);
  
  float[] colFloats = { red(col)/255.0, green(col)/255.0, blue(col)/255.0, alpha(col)/255.0 };
  msg.add(floatsToBytes(colFloats));
  
  float[] pointFloats = new float[points.size()*3];
  for (int i=0; i<pointFloats.length; i+=3) {
    PVector p = points.get(int(i/3));
    pointFloats[i] = p.x;
    pointFloats[i+1] = p.y;
    pointFloats[i+2] = p.z;
  }
  msg.add(floatsToBytes(pointFloats));

  oscP5.send(msg, myRemoteLocation); 
}

// Receive message example
void oscEvent(OscMessage msg) {
  //println(msg);
  if ((msg.checkAddrPattern("/contour") || msg.checkAddrPattern("/scanline")) && msg.checkTypetag("ssibbi")) {    
    String hostname = msg.get(0).stringValue();
    //String uniqueId = msg.get(1).stringValue();
    int index = msg.get(2).intValue();
    byte[] readColorBytes = msg.get(3).blobValue();
    byte[] readLNPointsBytes = msg.get(4).blobValue();
   
    byte[] bytesR = { readColorBytes[0], readColorBytes[1], readColorBytes[2], readColorBytes[3] };
    byte[] bytesG = { readColorBytes[4], readColorBytes[5], readColorBytes[6], readColorBytes[7] };
    byte[] bytesB = { readColorBytes[8], readColorBytes[9], readColorBytes[10], readColorBytes[11] };

    float r = asFloat(bytesR);
    float g = asFloat(bytesG);
    float b = asFloat(bytesB);
    color c = color(255);
    if (!Float.isNaN(r) && !Float.isNaN(g) && !Float.isNaN(b)) {
      //println("COLOR: " + r + " " + g + " " + b);
      c = color(r, g, b);
    }
 
    ArrayList<PVector> points = new ArrayList<PVector>();
    for (int i = 0; i < readLNPointsBytes.length; i += 12) { //+=16) { 
      byte[] bytesX = { readLNPointsBytes[i], readLNPointsBytes[i+1], readLNPointsBytes[i+2], readLNPointsBytes[i+3] };
      byte[] bytesY = { readLNPointsBytes[i+4], readLNPointsBytes[i+5], readLNPointsBytes[i+6], readLNPointsBytes[i+7] };
      byte[] bytesZ = { readLNPointsBytes[i+8], readLNPointsBytes[i+9], readLNPointsBytes[i+10], readLNPointsBytes[i+11] };
      //byte[] bytesW = { readLNPointsBytes[i+12], readLNPointsBytes[i+13], readLNPointsBytes[i+14], readLNPointsBytes[i+15] };

      float x = asFloat(bytesX);
      float y = asFloat(bytesY);
      float z = asFloat(bytesZ) * 10;
      //float w = asFloat(bytesW);
      if (!Float.isNaN(x) && !Float.isNaN(y)) { // && !Float.isNaN(z)) {
        PVector p = new PVector(x, y, z);
        points.add(p);
        //println(p.x + ", " + p.z + ", " + p.y);
      }
    } 
    
    boolean foundNewHost = true;
    for (int i=0; i<frameProjector.size(); i++) {
      if (frameProjector.get(i).hostname.equals(hostname)) {
        frameProjector.get(i).createStroke(index, c, points);
        frameProjector.get(i).hostnameConfirmed = true;
        foundNewHost = false;
        break;
      }
    }
    
    if (foundNewHost) {
      for (int i=0; i<frameProjector.size(); i++) {
        if (!frameProjector.get(i).hostnameConfirmed) {
          frameProjector.get(i).hostname = hostname;
          frameProjector.get(i).createStroke(index, c, points);
          frameProjector.get(i).hostnameConfirmed = true;
          break;
        }
      }
    }
  }
}
