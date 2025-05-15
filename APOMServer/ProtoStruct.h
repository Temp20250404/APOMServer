#pragma once

#include "Protobuf/Protocol.pb.h"

struct PlayerInfo {
    std::string playerNickname;
    UINT32 playerJob;
    UINT32 level;

    PlayerInfo& operator=(const game::PlayerInfo& other) {
        playerNickname = other.playernickname();
        playerJob = other.playerjob();
        level = other.level();

        return *this;
    }
};

struct Position {
    float posX;
    float posY;
    float posZ;

    // game::Position ���κ��� �����ϴ� ���� ������
    Position& operator=(const game::Position& other) {
        posX = other.posx();
        posY = other.posy();
        posZ = other.posz();

        return *this;
    }
};