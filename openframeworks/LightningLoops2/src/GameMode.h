#pragma once
#include "ofMain.h"

// Which object the WASDQE keys control.
enum class GameMode {
    OFF,
    CAM,
    DEPTH1_POS,
    DEPTH1_ROT,
    DEPTH2_POS,
    DEPTH2_ROT
};

// Cycles through the modes, rolling over from the last to the first.
inline GameMode nextGameMode(GameMode mode) {
    if (mode == GameMode::DEPTH2_ROT) return GameMode::OFF;
    return GameMode(int(mode) + 1);
}

inline string gameModeName(GameMode mode) {
    switch (mode) {
        case GameMode::OFF: return "OFF";
        case GameMode::CAM: return "CAM";
        case GameMode::DEPTH1_POS: return "DEPTH1_POS";
        case GameMode::DEPTH1_ROT: return "DEPTH1_ROT";
        case GameMode::DEPTH2_POS: return "DEPTH2_POS";
        case GameMode::DEPTH2_ROT: return "DEPTH2_ROT";
    }
    return "";
}
