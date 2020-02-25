float delta = 10;

boolean keyW = false;
boolean keyA = false;
boolean keyS = false;
boolean keyD = false;
boolean keyQ = false;
boolean keyE = false;
boolean keySpace = false;

void keyPressed() {
  checkKeyChar(key, true);
  
  if (keyCode==TAB) {
    gameMode = gameMode.next();
    cam.displayText = "" + gameMode;
    println(gameMode);
  } else if (key == 'c') {
    gameMode = GameMode.CAM;
  } else if (key == '1') {
    gameMode = GameMode.DEPTH1_POS;
  } else if (key == '2') {
    gameMode = GameMode.DEPTH2_POS;
  } else if (key == 'o') {
    settings.write();
  }
}

void keyReleased() {
  checkKeyChar(key, false);
}

boolean checkKeyChar(char k, boolean b) {
  switch (k) {
    case 'w':
      return keyW = b;
    case 'a':
      return keyA = b;
    case 's':
      return keyS = b;      
    case 'd':
      return keyD = b;
    case 'q':
      return keyQ = b;
    case 'e':
      return keyE = b;
    case ' ':
      return keySpace = b;
    default:
      return b;
  }
}

/*
boolean checkKeyCode(int k, boolean b) {
  switch (k) {
    case SHIFT:
      return keyShift = b;
    default:
      return b;
  }
}
*/

void updateControls() {
  
  switch (gameMode) {
    case CAM:
      if (keyW) cam.moveForward();
      if (keyS) cam.moveBack();
      if (keyA) cam.moveLeft();
      if (keyD) cam.moveRight();
      if (keyQ) cam.moveDown();
      if (keyE) cam.moveUp();
      if (keySpace) cam.reset();
      break;
   case DEPTH1_POS:
      if (keyW) frameProjector1.moveForward();
      if (keyS) frameProjector1.moveBack();
      if (keyA) frameProjector1.moveLeft();
      if (keyD) frameProjector1.moveRight();
      if (keyQ) frameProjector1.moveDown();
      if (keyE) frameProjector1.moveUp();
      if (keySpace) frameProjector1.reset();
     break;
   case DEPTH1_ROT:
      if (keyW) frameProjector1.rollUp();
      if (keyS) frameProjector1.rollDown();
      if (keyA) frameProjector1.pitchUp();
      if (keyD) frameProjector1.pitchDown();
      if (keyQ) frameProjector1.yawUp();
      if (keyE) frameProjector1.yawDown();
      if (keySpace) frameProjector1.reset();
     break;     
   case DEPTH2_POS:
     break;
   case DEPTH2_ROT:
     break;
  }

}

void keysOff() {
  keyW = false;
  keyA = false;
  keyS = false;
  keyD = false;
  keyQ = false;
  keyE = false;
  keySpace = false;
}
