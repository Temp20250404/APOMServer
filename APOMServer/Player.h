#pragma once

#include "pch.h"
#include "Object.h"
//#include "DamageTracker.h"
#include <array>
#include "ProtoStruct.h"

void SendCreationPacketBetween(CObject*, CObject*);
void SendDestructionPacketBetween(CObject*, CObject*);

enum class PLAYER_GAME_STATE {
    WAITING,
    ACTIVE,
    END
};

enum class PRESS_KEY {
    W,
    A,
    S,
    D,
    JUMP,
    END
};

class CPlayer : public CObject {
public:
    explicit CPlayer() noexcept;
    virtual ~CPlayer();

    // 기타 멤버 함수 선언
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;

    void Init(float _x, float _y, float _z, UINT8 _hp, UINT8 _mp);

public:
    int GetId() const { return m_ID; }
    int GetRoomId() const { return m_roomId; }
    int GetSpawnPosIndex() const { return m_spawnPosIndex; }

    void SetRoomId(int roomId) { m_roomId = roomId; }
    void SetSpawnPosIndex(int spawnPosIndex) { m_spawnPosIndex = spawnPosIndex; }

public:
    PLAYER_GAME_STATE GetCurGameState(void) { return m_eCurPlayerGameState; }
    void SetCurGameState(PLAYER_GAME_STATE eState) { m_eCurPlayerGameState = eState; }

public:
    std::array<bool, (int)PRESS_KEY::END> GetMovementKeys() const;
    void SetMovementKeys(const std::array<bool, (int)PRESS_KEY::END>& keys);

public:
    void GetRotationAxisXY(float& AxisX, float& AxisY) const { AxisX = m_rotationAxisX; AxisY = m_rotationAxisY; }
    void SetRotationAxisXY(float AxisX, float AxisY) { m_rotationAxisX = AxisX, m_rotationAxisY = AxisY; }

public:
    UINT8 GetCurHp(void) { return m_curHp; }
    UINT8 GetMaxHp(void) { return m_maxHp; }
    void SetCurHp(UINT8 curHp) { m_curHp = curHp; }

    UINT8 GetCurMp(void) { return m_curMp; }
    UINT8 GetMaxMp(void) { return m_maxMp; }
    void SetCurMp(UINT8 curMp) { m_curMp = curMp; }

public:
    const std::string& GetName(void) { return m_playerName; }
    void SetName(const std::string& name) { m_playerName = name; }

private:
    UINT8 m_curHp = 0;          // 체력
    UINT8 m_maxHp = 0;

    UINT8 m_curMp = 0;          // 마력
    UINT8 m_maxMp = 0;

    float m_rotationAxisX = 0;      // x, y축 회전값
    float m_rotationAxisY = 0;

    int m_roomId = -1;
    int m_spawnPosIndex = -1;

    bool m_pressedKey[(int)PRESS_KEY::END];

    PLAYER_GAME_STATE m_eCurPlayerGameState;  // 시작 관련 정보, 처음이나 죽었을 때 WAITING 상태, 게임 플레이 중이면 ACTIVE로 변경됨

    std::string m_playerName;

    UINT32 playerJobID;     // 임시로 정의한 플레이어 직업 정보
};