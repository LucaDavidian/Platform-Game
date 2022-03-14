#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>

#include "double_ended_singly_linked_list.hpp"

using std::size_t;

// with double ended linked list all operations are implemented in O(1) constant time
template <typename T, template <typename> class S = DE_SinglyLinkedList>  
class Queue
{
public:
    Queue() = default;
    ~Queue() = default;

    bool Empty() const { return mContainer.Empty(); }
    size_t Size() const { return mContainer.Size(); }

    void Clear() { mContainer.Clear(); }

    template <typename U>
    void Enqueue(U &&element) { mContainer.InsertLast(std::forward<U>(element)); }
    void Dequeue() { mContainer.RemoveFirst(); }

    T &Front() { return const_cast<T&>(static_cast<const Queue*>(this)->Front()); }
    const T &Front() const { return mContainer.First(); }
    T &Back() { return const_cast<T&>(static_cast<const Queue*>(this)->Back()); }
    const T &Back() const { return mContainer.Last(); }
private:
    S<T> mContainer;
};

#endif  // QUEUE_H