#include "MatchSystem.h"

MatchSystem::MatchSystem()
{
}

MatchSystem::MatchSystem(int roomId) : m_roomId(roomId) {
    m_red.reserve(5);
    m_blue.reserve(5);

    m_red.clear();
    m_blue.clear();

    // ���� �����Ϳ� 0 ~ 4���� �ε��� ����
    for (int i = 0; i < 5; ++i)
    {
        m_redSpawnIndex.push(i);
        m_blueSpawnIndex.push(i);
    }
}

void MatchSystem::OnPlayerJoin(CPlayer* player) {
    // ���� ���� ���
    if (m_red.size() <= m_blue.size()) {
        m_red.push_back(player);
        player->SetTeamId(0);   // ���� ���� ��� �� ID = 0

        player->SetSpawnPosIndex(m_redSpawnIndex.front());
        m_redSpawnIndex.pop();
    }

    // ��� ���� ���
    else {
        m_blue.push_back(player);
        player->SetTeamId(1);   // ��� ���� ��� �� ID = 1

        player->SetSpawnPosIndex(m_blueSpawnIndex.front());
        m_blueSpawnIndex.pop();
    }
}

void MatchSystem::OnPlayerLeave(int playerId) {
    auto removePlayer = [playerId](std::vector<CPlayer*>& team) {
        team.erase(std::remove_if(team.begin(), team.end(), [playerId](CPlayer* p) {
            return p->GetId() == playerId;
            }), team.end());
        };

    for (auto& redPlayer : m_red)
    {
        if (redPlayer)
        {
            // �÷��̾ ã���� ���
            if (redPlayer->m_ID == playerId)
            {
                // �ڽ��� ���� ��ġ�� ��ȯ
                m_redSpawnIndex.push(redPlayer->GetSpawnPosIndex());
            }
        }
    }
    for (auto& bluePlayer : m_blue)
    {
        if (bluePlayer)
        {
            // �÷��̾ ã���� ���
            if (bluePlayer->m_ID == playerId)
            {
                // �ڽ��� ���� ��ġ�� ��ȯ
                m_blueSpawnIndex.push(bluePlayer->GetSpawnPosIndex());
            }
        }
    }

    removePlayer(m_red);
    removePlayer(m_blue);
}

int MatchSystem::GetPlayerCount() const {
    return static_cast<int>(m_red.size() + m_blue.size());
}

const std::vector<CPlayer*>& MatchSystem::GetRedTeam() const { return m_red; }
const std::vector<CPlayer*>& MatchSystem::GetBlueTeam() const { return m_blue; }
