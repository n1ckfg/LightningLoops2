import ddf.minim.*; 
import xyscope.*;
  
int activityThreshold = 1;

XYscope xy1;
//XYscope xy2;

float xy1ampOrig = 0.01;
//float xy2ampOrig = 1;
float xy1amp = xy1ampOrig;
//float xy2amp = xy2ampOrig;
float deltaAmp = 0.0025;
boolean fadeOutAmp1 = false;
//boolean fadeOutAmp2 = false;

void soundOutSetup() {
  // initialize XYscope with default sound out
  xy1 = new XYscope(this);
  //xy2 = new XYscope(this);

  // smooth waves to reduce visible points
  //xy1.smoothWaves(true);
  //xy2.smoothWaves(true);
  
  // test best smoothAmount, default 12
  //xy1.smoothWavesAmount(12);
  //xy2.smoothWavesAmount(12);
}

void soundOutDraw() {
  if (frameProjector1.frame.strokes.size() > activityThreshold) {
    try {
    xy1.amp(xy1amp);
    //xy2.amp(xy2amp);
      
    // build audio from shapes
    xy1.buildWaves();
    //xy2.buildWaves();
    
    if (fadeOutAmp1) {
      xy1amp -= deltaAmp;
      if (xy1amp < 0) xy1amp = 0;
    } 
    /*
    if (fadeOutAmp2) {
      xy2amp -= deltaAmp;
      if (xy2amp < 0) xy2amp = 0;
    }
    */
    } catch (Exception e) { }
  }
}
