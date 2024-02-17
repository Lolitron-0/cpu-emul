#pragma once
#include <iostream>

namespace stack
{
template <class T>
using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
class Stack
{
public:
    using size_type = size_t;

    Stack()
        : Stack(s_DefaultCapacity)
    {
    }

    Stack(size_type capacity)
        : m_Capacity{ capacity }
    {
        m_Data = new T[m_Capacity];
    }

    Stack(const Stack& other)
        : m_Capacity{ other.m_Capacity },
          m_Size{ other.m_Size }
    {
        this->m_Data = new T[m_Capacity];
        std::copy(other.m_Data, other.m_Data + other.m_Size, this->m_Data);
    }

    Stack(Stack&& other) noexcept
        : m_Capacity{}
    {
        swap(*this, other);
    }

    Stack& operator=(Stack other)
    {
        using std::swap;
        swap(*this, other);
        return *this;
    }

    ~Stack()
    {
        delete[] m_Data;
        m_Data = nullptr;
        m_Size = 0;
        m_Capacity = 0;
    }

public:
    size_type size() const noexcept
    {
        return m_Size;
    };

    size_type capacity() const noexcept
    {
        return m_Capacity;
    };
    bool empty() const noexcept
    {
        return m_Size == 0;
    }

    T& top()
    {
        return m_Data[m_Size - 1];
    }
    const T& top() const
    {
        return m_Data[m_Size - 1];
    }

    template <typename U>
    void push(U&& val)
    {
        static_assert(std::is_same_v<remove_cv_ref_t<T>, remove_cv_ref_t<U>> ||
                          std::is_convertible_v<U, T>,
                      "Wrong push type");

        _reallocateIfNeeded();
        this->m_Data[m_Size] = std::forward<U>(val);
        m_Size++;
    }

    template <typename... Args>
    void emplace(Args&&... args)
    {
        _reallocateIfNeeded();
        this->m_Data[m_Size] = T(std::forward<Args>(args)...);
        m_Size++;
    }

    void pop()
    {
        this->m_Size--;
    }

private:
    friend void swap(Stack& left, Stack& right) noexcept
    {
        std::swap(left.m_Data, right.m_Data);
        std::swap(left.m_Size, right.m_Size);
        std::swap(left.m_Capacity, right.m_Capacity);
    }

    static constexpr size_type s_DefaultCapacity{ 10 };

private:
    void _reallocateIfNeeded()
    {
        if (m_Size == m_Capacity)
        {
            m_Capacity *= 2;
            T* temp = new T[m_Capacity];
            std::copy(this->m_Data, this->m_Data + this->m_Size, temp);
            delete[] this->m_Data;
            this->m_Data = temp;
        }
    }

private:
    T* m_Data;
    size_type m_Size{ 0 };
    size_type m_Capacity{ s_DefaultCapacity };
};
} // namespace stack
