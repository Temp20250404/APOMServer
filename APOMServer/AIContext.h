#pragma once

#include "ProtoStruct.h"
#include "Protobuf/Protocol.pb.h"

class CRoom;
class CPlayer;

// 언리얼에서 blackboard라 불리는 AI 정보를 컨트롤하는 구조체
struct AIContext {
    // 고유 ID
    UINT32 ID = -1;  // 보스 또는 AI의 고유 식별자

    // HP 관련
    UINT32 currentHP;    // 현재 HP
    UINT32 maxHP;        // 최대 HP
    bool bDeath;         // 사망 여부

    // 스킬 사용 여부
    bool usedSkill1;     // 스킬1 사용 여부
    bool usedSkill2;     // 스킬2 사용 여부
    bool usedSkill3;     // 스킬3 사용 여부

    // 애니메이션 시간 (초)
    float attackAnimTime;   // 공격 애니메이션 재생 시간
    float dieAnimTime;      // 사망 애니메이션 재생 시간
    float skill1AnimTime;   // 스킬1 애니메이션 재생 시간
    float skill2AnimTime;   // 스킬2 애니메이션 재생 시간
    float skill3AnimTime;   // 스킬3 애니메이션 재생 시간
    float phaseTimer;       // 현재 진행 중인 애니메이션 타이머

    // 타겟 정보
    bool hasTargetPlayer;   // 타겟 플레이어 존재 여부
    float playerDistance;   // 타겟 플레이어와의 거리


    // Idle 관련
    float idleTime;         // 대기 상태(Idle) 시간

    // 상수 (각 Tick 간 시간 등)
    float attackRange;      // 공격 범위
    float moveSpeed;        // 이동 속도 (각 Tick 당 감소하는 거리)
    float idleResetTime;    // 이동 후 Idle 시간 재설정 값
    float deltaTime;        // 각 Tick 간 시간 (초)
    float detectionRange;   // 플레이어 인지 범위. 인지 범위 내에 들어온 플레이어가 있다면 해당 플레이어를 타게팅


    // 추가 정보: 방 및 타겟 플레이어 정보
    CRoom* ptargetRoom = nullptr;      // 현재 AI가 속한 방 (방 정보를 통해 방 안의 플레이어 목록 등 접근)
    CPlayer* pTargetPlayer = nullptr;    // 현재 AI가 타겟팅한 플레이어의 정보


    // 위치 정보
    Position currentPos{ 0.f, 0.f, 0.f };   // 현재 AI(보스)의 위치 (월드 좌표)
    Position targetPos{ 0.f, 0.f, 0.f };    // 보스의 이동 목표 위치

    // 상태 정보
    game::BOSS_STATE eCurState = game::BOSS_STATE::BOSS_STATE_IDLE;
};
