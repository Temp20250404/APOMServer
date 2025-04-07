#pragma once

enum class ItemType {
    HealPack,
    InfiniteAmmo,
    MoveSpeed,
    JumpPower,
    DoubleJump,

    END
};

class Item {
public:
    Item(ItemType t, float px, float py, float pz);

private:
    static int m_gItemId;   // ���� ������ id

public:
    int id;                 // ������ id
    ItemType type;          // ������ Ÿ��
    float x, y, z;          // ��ġ
};