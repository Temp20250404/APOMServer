#pragma once

#include "pch.h"

// �α��� ���� ����ü
struct LoginInfo {
    std::string id;
    std::string email;
    std::string pw;

    // �� ������ �����ε� (unordered_map���� key�� ����ϱ� ���� �ʿ�)
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