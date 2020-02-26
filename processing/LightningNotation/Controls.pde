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
    switch (gameMode) {
      case CAM:
        gameMode = GameMode.OFF;
        break;
      default:
        gameMode = GameMode.CAM;
        break;
    }
  } else if (key == '1') {
    switch (gameMode) {
      case DEPTH1_POS:
        gameMode = GameMode.DEPTH1_ROT;
        break;
      default:
        gameMode = GameMode.DEPTH1_POS;
        break;
    }
  } else if (key == '2') {
    switch (gameMode) {
      case DEPTH2_POS:
        gameMode = GameMode.DEPTH2_ROT;
        break;
      default:
        gameMode = GameMode.DEPTH2_POS;
        break;
    }
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
      if (keyW) frameProjector2.moveForward();
      if (keyS) frameProjector2.moveBack();
      if (keyA) frameProjector2.moveLeft();
      if (keyD) frameProjector2.moveRight();
      if (keyQ) frameProjector2.moveDown();
      if (keyE) frameProjector2.moveUp();
      if (keySpace) frameProjector2.reset();
     break;
   case DEPTH2_ROT:
      if (keyW) frameProjector2.rollUp();
      if (keyS) frameProjector2.rollDown();
      if (keyA) frameProjector2.pitchUp();
      if (keyD) frameProjector2.pitchDown();
      if (keyQ) frameProjector2.yawUp();
      if (keyE) frameProjector2.yawDown();
      if (keySpace) frameProjector2.reset();
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
