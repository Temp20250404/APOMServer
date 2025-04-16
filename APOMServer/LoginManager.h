#pragma once

#include "pch.h"
#include "Singleton.h"
#include "json.hpp"
#include "ProtoStruct.h"
#include "LoginInfo.h"

using json = nlohmann::json;

class CPlayer;

class CLoginManager : public SingletonBase<CLoginManager> {
    friend class SingletonBase<CLoginManager>;

public:
    explicit CLoginManager() noexcept;
    ~CLoginManager() noexcept;

    // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
    CLoginManager(const CLoginManager&) = delete;
    CLoginManager& operator=(const CLoginManager&) = delete;

public:
    // JSON ���Ͽ��� ����� ������ �ҷ���
    bool LoadFromFile(const std::string& filename);

    // ����� ������ JSON ���Ϸ� ����
    bool SaveToFile(const std::string& filename) const;

    // ����� ������ �߰�
    bool AddUser(const LoginInfo& login, const PlayerInfo& player);

    // �α��� ������ �������� �÷��̾� ������ �˻�
    const PlayerInfo* FindPlayerByLogin(const LoginInfo& login) const;

    // ID�� �������� �÷��̾� ������ �˻�
    const PlayerInfo* FindPlayerById(const std::string& id) const;

    // ID�� �������� �÷��̾� ������ ����
    bool UpdatePlayerInfo(const std::string& id, const PlayerInfo& newInfo);

    // �̸��� �ߺ� ���θ� �˻�
    bool IsDuplicateEmail(const std::string& email) const; 

    // ID �ߺ� ���θ� �˻�
    bool IsDuplicateId(const std::string& id) const;

public:
    // ����� ��ü �ʿ� ���� (�б� ����)
    const std::unordered_map<LoginInfo, PlayerInfo>& GetUserMap() const {
        return m_userMap;
    }

private:
    // �α��� ������ key��, �÷��̾� ������ value�� ����
    std::unordered_map<LoginInfo, PlayerInfo> m_userMap;

    // ID�� key��, �÷��̾� ������ value�� ���� (���� �ε���)
    std::unordered_map<std::string, PlayerInfo> m_playerMap;
};