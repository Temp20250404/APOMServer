#pragma once

#define CIRCULAR_QUEUE_SIZE 10000


template <typename T>
class CircularQueue {
public:
    // ������
    CircularQueue(int size = CIRCULAR_QUEUE_SIZE) : front(0), rear(-1), capacity(size), count(0) {
        //queue = new T[capacity];  // ���� �迭 �Ҵ�
    }

    // �Ҹ���
    ~CircularQueue() {
        //delete[] queue;  // ���� �迭 ����
    }

    // ť�� ������� Ȯ��
    bool isEmpty() const {
        return count == 0;
    }

    // ť�� �� á���� Ȯ��
    bool isFull() const {
        return count == capacity;
    }

    // ť�� ������ �߰� (enqueue)
    void enqueue(const T& data) {
        /*if (isFull()) {
            dequeue();
        }*/
        rear = (rear + 1) % capacity;  // ���� �迭 ó��
        queue[rear] = data;
        count++;
    }

    // ť���� ������ ���� (dequeue)
    void dequeue() {
        /*if (isEmpty()) {
            return;
        }*/
        front = (front + 1) % capacity;  // ���� �迭 ó��
        count--;
    }

    // ť�� �տ� �ִ� �����͸� ��ȯ (peek)
    T peek() const {
        if (isEmpty()) {
            return T();  // �⺻ ������ T ��ȯ
        }
        return queue[front];
    }

    // ť�� ����� �������� ���� ��ȯ
    int size() const {
        return count;
    }

public:
    T queue[CIRCULAR_QUEUE_SIZE];         // ť �迭
    int front, rear;  // ť�� �հ� �� �ε���
    int capacity;     // ť�� �ִ� ũ��
    int count;        // ���� ť�� ����� ������ ����
};