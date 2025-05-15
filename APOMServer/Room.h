#pragma once
#include <vector>
#include "Player.h"
#include "MatchSystem.h"
#include "ItemSpawner.h"

class AIEntity;

class CRoom {
public:
    CRoom(int id);

    void Init(void);

    bool AddPlayer(CPlayer* player);
    void RemovePlayer(int playerId);
    void Update(float deltaTime);
    bool IsFull() const;

    bool OnItemPickupRequest(int playerId, int itemId);

    void MoveToActive(int playerId);
    void MoveToWaiting(int playerId);

    int GetRoomId() const { return m_roomId; }
    int GetPlayerCount() const { return static_cast<int>(m_activePlayers.size() + m_waitingPlayers.size()); }

    CPlayer* FindPlayerById(int playerId);

    const std::vector<CPlayer*>& GetActivePlayers(void) { return m_activePlayers; }

public:
    const std::vector<AIEntity*>& GetEntities(void) { return m_pEntities; }
    void AddEntity(AIEntity* pEntity) { m_pEntities.push_back(pEntity); }
    void RemoveEntity(AIEntity* pEntity);

public:
    std::vector<CPlayer*> m_activePlayers;
    std::vector<CPlayer*> m_waitingPlayers;

    std::vector<AIEntity*> m_pEntities;

public:
    UINT32 authorityPlayerID = 0;

private:
    static constexpr int MAX_PLAYERS = 10;
    int m_roomId = -1;
};