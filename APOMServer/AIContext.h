#pragma once

// 언리얼에서 blackboard라 불리는 AI 정보를 컨트롤하는 구조체
struct AIContext {
    // 체력 관련
    float currentHP = 100.0f;    // 현재 체력
    float maxHP = 100.0f;        // 최대 체력

    bool bDeath = false;         // 체력이 0 이하가 되면 true

    // 플레이어와의 거리
    float distanceToPlayer = 30.0f; // 초기값은 30m로 가정

    // 이동/공격 관련
    float moveSpeed = 1.0f;      // 보스 이동 속도
    float baseDamage = 10.0f;    // 기본 공격력
    float attackRange = 5.0f;    // 공격 거리

    // 페이즈 전환 기준
    float phase2Distance = 20.0f;  // 페이즈2로 전환하는 거리(20m 이하)
    float phase3HPPercent = 0.8f;  // 체력이 80% 이하가 되면 페이즈3
    float phase4HPPercent = 0.5f;  // 체력이 50% 이하가 되면 페이즈4

    float detectionRange = 15.0f; // 플레이어 감지 범위
};