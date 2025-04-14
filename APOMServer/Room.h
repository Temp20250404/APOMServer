#pragma once
#include <vector>
#include "Player.h"
#include "MatchSystem.h"
#include "ItemSpawner.h"

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
    std::vector<CPlayer*> m_activePlayers;
    std::vector<CPlayer*> m_waitingPlayers;

private:
    static constexpr int MAX_PLAYERS = 10;
    int m_roomId = -1;

    ItemSpawner* m_pItemSpawner;
};