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
      strokes.get(i).draw();
    }
  }
}
