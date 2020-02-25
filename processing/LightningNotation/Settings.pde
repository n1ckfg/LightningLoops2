class Settings {
  
  String[] data;
  String name = "settings.txt";
  
  Settings(String _s) {
    name = _s;
    read();
  }
  
  void read() {
    try {
      data = loadStrings(name);
      for (int i=0;i<data.length;i++) {
        if (data[i].equals("Render Width")) texWidth = readInt(i);
        if (data[i].equals("Render Height")) texHeight = readInt(i);
        if (data[i].equals("Volume")) xy1amp = readFloat(i);
        if (data[i].equals("Stroke Lifetime")) globalLifespan = readInt(i);
        if (data[i].equals("Bloom Amount")) bloomMult = readFloat(i);
        if (data[i].equals("Framerate")) realFps = readInt(i);
        if (data[i].equals("Use Website")) useWebsockets = readBoolean(i);
        if (data[i].equals("Activity Threshold")) activityThreshold = readInt(i);
        if (data[i].equals("Cam Position")) cam.pos = readVector(i);
        if (data[i].equals("Cam Point of Interest")) cam.poi = readVector(i);
        if (data[i].equals("Cam Up")) cam.up = readVector(i);
        if (data[i].equals("Cam Pan")) cam.pan = readFloat(i);
        if (data[i].equals("Cam Tilt")) cam.tilt = readFloat(i);
        if (data[i].equals("Depth1 Position")) frameProjector1.pos = readVector(i);
        if (data[i].equals("Depth1 Rotation")) frameProjector1.q = readQuaternion(i);
        if (data[i].equals("Depth2 Position")) frameProjector2.pos = readVector(i);
        if (data[i].equals("Depth2 Rotation")) frameProjector2.q = readQuaternion(i);
      }
    } 
    catch(Exception e) {
      println("Couldn't load settings file. Using defaults.");
    }
  }

  void write() {
    try {
      for (int i=0;i<data.length;i++) {
        if (data[i].equals("Cam Position")) writeVector(cam.pos, i);
        if (data[i].equals("Cam Point of Interest")) writeVector(cam.poi, i);
        if (data[i].equals("Cam Up")) writeVector(cam.up, i);
        if (data[i].equals("Cam Pan")) writeFloat(cam.pan, i);
        if (data[i].equals("Cam Tilt")) writeFloat(cam.tilt, i);
        if (data[i].equals("Depth1 Position")) writeVector(frameProjector1.pos, i);
        if (data[i].equals("Depth1 Rotation")) writeQuaternion(frameProjector1.q, i);
        if (data[i].equals("Depth2 Position")) writeVector(frameProjector2.pos, i);
        if (data[i].equals("Depth2 Rotation")) writeQuaternion(frameProjector2.q, i);
      }
      saveStrings("data/" + name, data);
    } catch (Exception e) {
      println("Failed to write settings file.");
    }
  }
  
  // ~ ~ ~ GET ~ ~ ~

  int readInt(int index) {
    String _s = data[index+1];
    return int(_s);
  }

  float readFloat(int index) {
    String _s = data[index+1];
    return float(_s);
  }

  boolean readBoolean(int index) {
    String _s = data[index+1];
    return boolean(_s);
  }

  String readString(int index) {
    String _s = data[index+1];
    return ""+(_s);
  }
  
  color readColor(int index) {
    String _s = data[index+1];
    
    color endColor = color(0);
    int commaCounter=0;
    String sr = "";
    String sg = "";
    String sb = "";
    String sa = "";
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 0;

    for (int i=0;i<_s.length();i++) {
        if (_s.charAt(i)!=char(' ') && _s.charAt(i)!=char('(') && _s.charAt(i)!=char(')')) {
          if (_s.charAt(i)==char(',')){
            commaCounter++;
          }else{
          if (commaCounter==0) sr += _s.charAt(i);
          if (commaCounter==1) sg += _s.charAt(i);
          if (commaCounter==2) sb += _s.charAt(i); 
          if (commaCounter==3) sa += _s.charAt(i);
         }
       }
     }

    if (sr!="" && sg=="" && sb=="" && sa=="") {
      r = int(sr);
      endColor = color(r);
    }
    if (sr!="" && sg!="" && sb=="" && sa=="") {
      r = int(sr);
      g = int(sg);
      endColor = color(r, g);
    }
    if (sr!="" && sg!="" && sb!="" && sa=="") {
      r = int(sr);
      g = int(sg);
      b = int(sb);
      endColor = color(r, g, b);
    }
    if (sr!="" && sg!="" && sb!="" && sa!="") {
      r = int(sr);
      g = int(sg);
      b = int(sb);
      a = int(sa);
      endColor = color(r, g, b, a);
    }
      return endColor;
  }

  PVector readVector(int index) {
    String _s = data[index+1];

    PVector endPVector = new PVector(0,0,0);
    int commaCounter=0;
    String sx = "";
    String sy = "";
    String sz = "";
    float x = 0;
    float y = 0;
    float z = 0;

    for (int i=0;i<_s.length();i++) {
        if (_s.charAt(i)!=char(' ') && _s.charAt(i)!=char('(') && _s.charAt(i)!=char(')')) {
          if (_s.charAt(i)==char(',')){
            commaCounter++;
          }else{
          if (commaCounter==0) sx += _s.charAt(i);
          if (commaCounter==1) sy += _s.charAt(i);
          if (commaCounter==2) sz += _s.charAt(i); 
         }
       }
     }

    if (sx!="" && sy=="" && sz=="") {
      x = float(sx);
      endPVector = new PVector(x,0);
    }
    if (sx!="" && sy!="" && sz=="") {
      x = float(sx);
      y = float(sy);
      endPVector = new PVector(x,y);
    }
    if (sx!="" && sy!="" && sz!="") {
      x = float(sx);
      y = float(sy);
      z = float(sz);
      endPVector = new PVector(x,y,z);
    }
      return endPVector;
  }
  
  Quaternion readQuaternion(int index) {
    String _s = data[index+1];

    Quaternion q = new Quaternion();
    int commaCounter=0;
    String sw = "";
    String sx = "";
    String sy = "";
    String sz = "";
    float w = 0;
    float x = 0;
    float y = 0;
    float z = 0;

    for (int i=0;i<_s.length();i++) {
        if (_s.charAt(i)!=char(' ') && _s.charAt(i)!=char('(') && _s.charAt(i)!=char(')')) {
          if (_s.charAt(i)==char(',')){
            commaCounter++;
          }else{
          if (commaCounter==0) sw += _s.charAt(i);
          if (commaCounter==1) sx += _s.charAt(i);
          if (commaCounter==2) sy += _s.charAt(i);
          if (commaCounter==3) sz += _s.charAt(i); 
         }
       }
     }

    if (sw!="" && sx!="" && sy!="" && sz!="") {
      w = float(sw);
      x = float(sx);
      y = float(sy);
      z = float(sz);
      q = new Quaternion(w,x,y,z);
    }
    
    return q;
  }
  
  String[] readStringArray(int index) {
    String _s = data[index+1];
      
    int commaCounter=0;
    for (int j=0;j<_s.length();j++) {
        if (_s.charAt(j)==char(',')){
          commaCounter++;
        }      
    }
    //println(commaCounter);
    String[] buildArray = new String[commaCounter+1];
    commaCounter=0;
    for(int k=0;k<buildArray.length;k++){
      buildArray[k] = "";
    }
    for (int i=0;i<_s.length();i++) {
      if (_s.charAt(i)!=char(' ') && _s.charAt(i)!=char('(') && _s.charAt(i)!=char(')') && _s.charAt(i)!=char('{') && _s.charAt(i)!=char('}') && _s.charAt(i)!=char('[') && _s.charAt(i)!=char(']')) {
        if (_s.charAt(i)==char(',')) {
          commaCounter++;
        } else {
          buildArray[commaCounter] += _s.charAt(i);
        }
      }
    }
    println(buildArray);
    return buildArray;
  }
  
  // ~ ~ ~ SET ~ ~ ~

  void writeBoolean(boolean b, int index) {
    data[index+1] = "" + b;
  }
  
  void writeFloat(float f, int index) {
    data[index+1] = "" + f;
  }
  
  void writeInt(int i, int index) {
    data[index+1] = "" + i;
  }

  void writeString(String s, int index) {
    data[index+1] = "" + s;
  }

  void writeVector(PVector p, int index) {
    data[index+1] = p.x + ", " + p.y + ", " + p.z;
  }

  void writeQuaternion(Quaternion q, int index) {
    data[index+1] = q.w + ", " + q.x + ", " + q.y + ", " + q.z;
  }

}
