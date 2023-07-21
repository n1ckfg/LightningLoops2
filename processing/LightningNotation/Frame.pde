class Frame {

  ArrayList<Stroke> strokes;
  int timestamp;
  
  Frame(ArrayList<Stroke> _strokes) {
    strokes = _strokes;
    timestamp = millis();
  }
  
  void draw() {  
    xy1.clearWaves();
    
    for (int i=0; i<strokes.size(); i++) {
      try {
        strokes.get(i).draw();
      } catch (Exception e) {
        println("Error drawing stroke " + i + " in frame " + timestamp + ".");
      }
    }
  }
}
