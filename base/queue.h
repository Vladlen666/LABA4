#pragma once
#include <string>

template <class TType>
class TQueue
{
private:
    struct Node
    {
        TType data;
        Node* pNext;
        Node(const TType _data = {}, Node* _pNext = nullptr) : data(_data), pNext(_pNext) {}
    };

    Node* pFirst;
    int size;

    TQueue(const TQueue&) = delete; // Запрет на копирование очереди

    void operator=(const TQueue&) = delete; // Запрет на присваивание очереди

public:
    TQueue() : pFirst(nullptr), size(0) {} // Конструктор

    ~TQueue() // Деструктор
    {
        while (size > 0)
            pop_front();
    }

    bool empty() const { return size == 0; } // Проверка на пустоту

    int get_size() const { return size; } // Получение значения размера очереди

    TType get_front() const // Получение значения верхнего элемента без удаления из очереди
    {
        if (size == 0)
            throw std::string("Queue is empty");
        return pFirst->data;
    }

    TType pop_front() // Получение значения верхнего элемента с удалением из очереди
    {
        if (size == 0)
            throw std::string("Queue is empty");
        Node* tmp = pFirst;
        TType tmpdata = pFirst->data;
        pFirst = pFirst->pNext;
        delete tmp;
        size--;
        return tmpdata;
    }

    void push_back(const TType obj) // Вставка элемента в конец очереди
    {
        if (size == 0)
        {
            pFirst = new Node(obj, nullptr);
        }
        else
        {
            Node* tmp = pFirst;
            while (tmp->pNext != nullptr)
                tmp = tmp->pNext;
            tmp->pNext = new Node(obj, nullptr);
        }
        size++;
    }
};
