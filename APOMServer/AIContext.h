#pragma once

#include "ProtoStruct.h"
#include "Protobuf/Protocol.pb.h"

class CRoom;

// 언리얼에서 blackboard라 불리는 AI 정보를 컨트롤하는 구조체
struct AIContext {
    // 고유 ID
    UINT32 ID = -1;  // 보스 또는 AI의 고유 식별자

    bool bDeath;         // 사망 여부

    // 추가 정보: 방 및 타겟 플레이어 정보
    CRoom* ptargetRoom = nullptr;      // 현재 AI가 속한 방 (방 정보를 통해 방 안의 플레이어 목록 등 접근)
};
