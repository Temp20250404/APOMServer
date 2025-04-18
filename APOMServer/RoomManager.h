#pragma once
#include "Room.h"
#include "Singleton.h"

class CRoomManager : public SingletonBase<CRoomManager> {
    friend class SingletonBase<CRoomManager>;

public:
    explicit CRoomManager() noexcept;
    ~CRoomManager() noexcept;

    // 복사 생성자와 대입 연산자를 삭제하여 복사 방지
    CRoomManager(const CRoomManager&) = delete;
    CRoomManager& operator=(const CRoomManager&) = delete;

public:
    void InitRooms() {
        m_rooms.reserve(MAX_ROOM_COUNT);
        CRoom* pRoom = nullptr;
        for (int i = 0; i < MAX_ROOM_COUNT; ++i) {
            pRoom = new CRoom(i);
            pRoom->Init();

            m_rooms.push_back(pRoom);
        }
    }

    CRoom* FindAvailableDungeon() {
        for (int i = 0; i < MAX_ROOM_COUNT; ++i) {
            if (!m_rooms[i]->IsFull()) {
                return m_rooms[i];
            }
        }
        return nullptr;
    }

    CRoom* GetRoomById(int roomId) {
        if (roomId < 0 || roomId >= MAX_ROOM_COUNT)
            return nullptr;
        return m_rooms[roomId];
    }

    int GetRoomCount() const { return MAX_ROOM_COUNT; }

private:
    static constexpr int MAX_ROOM_COUNT = 10;
    std::vector<CRoom*> m_rooms;
};
