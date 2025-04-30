
#include "LoginManager.h"
#include <fstream>
#include <functional>


// LoginInfo를 JSON으로 변환
void to_json(json& j, const LoginInfo& info) {
    j = json{ {"id", info.id}, {"email", info.email}, {"pw", info.pw} };
}

// JSON을 LoginInfo로 변환
void from_json(const json& j, LoginInfo& info) {
    j.at("id").get_to(info.id);
    j.at("email").get_to(info.email);
    j.at("pw").get_to(info.pw);
}

// PlayerInfo를 JSON으로 변환
void to_json(json& j, const PlayerInfo& info) {
    j = json{
        {"playerNickname", info.playerNickname},
        {"level", info.level},
        {"playerJob", info.playerJob}
    };
}

// JSON을 PlayerInfo로 변환
void from_json(const json& j, PlayerInfo& info) {
    j.at("playerNickname").get_to(info.playerNickname);
    j.at("level").get_to(info.level);
    j.at("playerJob").get_to(info.playerJob);
}




CLoginManager::CLoginManager() noexcept
{
}

CLoginManager::~CLoginManager() noexcept
{
}

bool CLoginManager::AddUser(const LoginInfo& login, const PlayerInfo& player) {
    if (m_userMap.contains(login)) return false;
    if (m_playerMap.contains(login.id)) return false;

    m_userMap[login] = player;
    m_playerMap[login.id] = player;
    return true;
}

const PlayerInfo* CLoginManager::FindPlayerByLogin(const LoginInfo& login) const {
    auto it = m_userMap.find(login);
    if (it == m_userMap.end()) return nullptr;
    return &it->second;
}

const PlayerInfo* CLoginManager::FindPlayerById(const std::string& id) const {
    auto it = m_playerMap.find(id);
    if (it == m_playerMap.end()) return nullptr;
    return &it->second;
}

bool CLoginManager::UpdatePlayerInfo(const std::string& id, const PlayerInfo& newInfo) {
    // 보조 인덱스를 먼저 갱신
    auto it = m_playerMap.find(id);
    if (it == m_playerMap.end()) return false;

    it->second = newInfo;

    // _userMap에서도 id에 해당하는 LoginInfo를 찾아 업데이트
    for (auto& [login, player] : m_userMap) {
        if (login.id == id) {
            player = newInfo;
            return true;
        }
    }

    return false;
}

bool CLoginManager::IsDuplicateEmail(const std::string& email) const {
    // 모든 로그인 정보에 대해 이메일 비교
    for (const auto& [login, _] : m_userMap) {  // 구조적 분해, 신기함...
        if (login.email == email) {
            return true;
        }
    }
    return false;
}

bool CLoginManager::IsDuplicateId(const std::string& id) const
{
    return m_playerMap.contains(id);
}

bool CLoginManager::SaveToFile(const std::string& filename) const {
    json j = json::array();

    for (const auto& [login, player] : m_userMap) {
        j.push_back(json{
            {"login", login},
            {"player", player}
            });
    }

    std::ofstream out(filename);
    if (!out.is_open()) return false;

    out << j.dump(4);
    return true;
}

bool CLoginManager::LoadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    json j;
    in >> j;

    m_userMap.clear();
    m_playerMap.clear();

    for (const auto& item : j) {
        LoginInfo login = item.at("login").get<LoginInfo>();
        PlayerInfo player = item.at("player").get<PlayerInfo>();
        m_userMap[login] = player;
        m_playerMap[login.id] = player;
    }

    return true;
}