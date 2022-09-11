#pragma once

#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <algorithm>

//#define USE_PRETTY 1

template <typename _T, std::size_t MEMORY_BLOCK_SIZE>
class custom_allocator 
{
public:    
    using size_type         = size_t;
    using difference_type   = int;
    using pointer           = _T*;
    using const_pointer     = const _T*;
    using reference         = _T&;
    using const_reference   = const _T&;
    using value_type        = _T;

    custom_allocator() :_allocated_block(nullptr), _counter_slot(0)
    {           
//#ifndef USE_PRETTY
//        
//        std::cout << "allocator constructor" << std::endl;
//#else
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
//#endif
        _allocated_block = std::malloc(MEMORY_BLOCK_SIZE * sizeof(value_type));
        
        if (nullptr == _allocated_block)
            throw std::bad_alloc();

//        std::cout << "allocated: " << _allocated_block << std::endl;
    };

    ~custom_allocator() 
    {        
//#ifndef USE_PRETTY
//        std::cout << "allocator destructor" << std::endl;
//#else
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
//#endif       
        free(_allocated_block);
    }

    pointer allocate([[maybe_unused]]std::size_t n)
    {
//#ifndef USE_PRETTY
//       std::cout << "allocate: [n = " << n << "]" << std::endl;
//#else
//        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
//#endif        
        pointer result_value(nullptr);            
        result_value = reinterpret_cast <pointer> (_allocated_block);
        result_value += _counter_slot;
        ++_counter_slot;

        result_value = result_value;
//        std::cout << "return value: " << result_value << std::endl;
        return result_value;
    };

    template<class U, class... Args>
    void construct(U* p, Args&&... args) 
    {        
//#ifndef USE_PRETTY
//        std::cout << "construct " << p << std::endl;
//#else
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
//#endif        
        new(static_cast<void*>(p)) U(std::forward<Args>(args)...);  // Placement 'new'
    }

    template<class U>
    void destroy(U* p) 
    {        
//#ifndef USE_PRETTY
//        std::cout << "destroy " << p << std::endl;
//#else
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
//#endif
        p->~U();
    }

    template<class U> struct rebind { using other = custom_allocator<U, MEMORY_BLOCK_SIZE>; };

    void deallocate([[maybe_unused]] pointer p, [[maybe_unused]] std::size_t n)
    {           
//#ifndef USE_PRETTY
//        std::cout << "deallocate: [n  = " << n << "] " << std::endl;
//#else
//        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
//#endif
        --_counter_slot;
//        std::cout << "counter: " << _counter_slot << std::endl;
    };
private:
    void* _allocated_block;
    size_t _counter_slot;
};
