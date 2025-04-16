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

    // 복사 생성자와 대입 연산자를 삭제하여 복사 방지
    CLoginManager(const CLoginManager&) = delete;
    CLoginManager& operator=(const CLoginManager&) = delete;

public:
    // JSON 파일에서 사용자 정보를 불러옴
    bool LoadFromFile(const std::string& filename);

    // 사용자 정보를 JSON 파일로 저장
    bool SaveToFile(const std::string& filename) const;

    // 사용자 정보를 추가
    bool AddUser(const LoginInfo& login, const PlayerInfo& player);

    // 로그인 정보를 기준으로 플레이어 정보를 검색
    const PlayerInfo* FindPlayerByLogin(const LoginInfo& login) const;

    // ID를 기준으로 플레이어 정보를 검색
    const PlayerInfo* FindPlayerById(const std::string& id) const;

    // ID를 기준으로 플레이어 정보를 수정
    bool UpdatePlayerInfo(const std::string& id, const PlayerInfo& newInfo);

    // 이메일 중복 여부를 검사
    bool IsDuplicateEmail(const std::string& email) const; 

    // ID 중복 여부를 검사
    bool IsDuplicateId(const std::string& id) const;

public:
    // 사용자 전체 맵에 접근 (읽기 전용)
    const std::unordered_map<LoginInfo, PlayerInfo>& GetUserMap() const {
        return m_userMap;
    }

private:
    // 로그인 정보를 key로, 플레이어 정보를 value로 저장
    std::unordered_map<LoginInfo, PlayerInfo> m_userMap;

    // ID를 key로, 플레이어 정보를 value로 저장 (보조 인덱스)
    std::unordered_map<std::string, PlayerInfo> m_playerMap;
};