#pragma once

struct PlayerInfo {
    std::string playerNickname;
    UINT32 playerMaxHp;
    UINT32 playerMaxMp;
    UINT32 playerJobIcon;
};

struct Position {
    float posX;
    float posY;
    float posZ;
};
