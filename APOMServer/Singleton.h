#pragma once

template <typename T>
class SingletonBase {
public:
    // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;

    // �̱��� �ν��Ͻ��� ��� ���� �޼���
    static T& GetInstance() {
        static T instance;
        return instance;
    }

protected:
    // ����� ���� protected ������
    SingletonBase() {
        // �ʱ�ȭ �ڵ�
    }

    // ����� ���� protected �Ҹ���
    virtual ~SingletonBase() {
        // ���� �ڵ�
    }

    // ��Ÿ protected ����� �޼���
};