#pragma once

#include <memory>

class type_erasure
{
    public:
        type_erasure() = default;

        type_erasure(type_erasure&& old) 
        {
            this->_ptr = old._ptr;
            this->_cur_size = old._cur_size;
            this->_capacity = old._capacity;

            old._ptr = nullptr;
            old._cur_size = 0;
            old._capacity = 0;
        }

        type_erasure& operator=(type_erasure&&) = default;
        type_erasure(type_erasure const&) = delete;
        type_erasure& operator=(type_erasure const&) = delete;

        template <typename T>
        type_erasure& operator=(T&& assign)
        {
            this->set_to(std::forward<T>(assign));
            return *this;
        }

        template <typename T>
        type_erasure& operator=(T const& assign)
        {
            this->set_to(std::forward<T>(assign));
            return *this;
        }

        template <typename T, typename... Tys>
        void emplace(Tys&&... args)
        {
            if (sizeof(T) > _cur_size)
            {
                delete[] _ptr;
                _ptr = reinterpret_cast<std::int8_t*>(new std::int8_t[sizeof(T)]);
                _capacity = sizeof(T);
            }

            new (_ptr) T{ std::forward<Tys...>(args...) };
            _cur_size = sizeof(T);
        }

        void shrink_to_fit()
        {
            if (_capacity <= _cur_size)
                return;

            auto temp_ptr = new std::int8_t[_cur_size];
            *temp_ptr = *_ptr;
            delete[] _ptr;
            _ptr = temp_ptr;
            _capacity = _cur_size;
        }

        ~type_erasure()
        {
            delete[] _ptr;
        }

        template <typename T>
        T& get() { return *reinterpret_cast<T*>(_ptr); }

        size_t capacity() { return _capacity; }

        size_t size() { return _cur_size; }

        std::int8_t* raw() { return _ptr; }

    private:
        template <typename T>
        void set_to(T&& content)
        {
            if (sizeof(T) > _cur_size)
            {
                delete[] _ptr;
                _ptr = reinterpret_cast<std::int8_t*>(new std::int8_t[sizeof(T)]);
                _capacity = sizeof(T);
            }

            *reinterpret_cast<T*>(_ptr) = std::forward<T>(content);
            _cur_size = sizeof(T);
        }

        size_t _capacity;
        size_t _cur_size;
        std::int8_t* _ptr;
};