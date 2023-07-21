// https://forum.processing.org/two/discussion/9877/how-to-use-enums
// https://stackoverflow.com/questions/17664445/is-there-an-increment-operator-for-java-enum

enum GameMode { 
  
  OFF,
  CAM,
  DEPTH1_POS,
  DEPTH1_ROT,
  DEPTH2_POS,
  DEPTH2_ROT {
    @Override
    GameMode next() {
      // rollover to the first
      return values()[0];
    };
  };

  GameMode next() {
    // No bounds checking required here, because the last instance overrides
    return values()[ordinal() + 1];
  }
  
}
