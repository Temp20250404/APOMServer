#pragma once

#include "pch.h"

// 로그인 정보 구조체
struct LoginInfo {
    std::string id;
    std::string email;
    std::string pw;

    // 비교 연산자 오버로딩 (unordered_map에서 key로 사용하기 위해 필요)
    bool operator==(const LoginInfo& other) const {
        return id == other.id && email == other.email && pw == other.pw;
    }
};

namespace std {
    template <>
    struct hash<LoginInfo> {
        std::size_t operator()(const LoginInfo& k) const {
            return hash<std::string>()(k.id) ^
                (hash<std::string>()(k.email) << 1) ^
                (hash<std::string>()(k.pw) << 2);
        }
    };
}