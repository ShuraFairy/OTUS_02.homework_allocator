#pragma once

#ifndef __PRETTY_FUNCTION__
#include "pretty.h"
#endif

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <array>

//#define USE_PRETTY 1

template <typename _T, std::size_t size_block>
class custom_allocator {
public:
	
	using size_type = size_t;
	using difference_type = int;
	using pointer = _T*;
	using const_pointer = const _T*;
	using reference = _T&;
	using const_reference = const _T&;
	using value_type = _T;

	custom_allocator() 
	{
		_pointer_data_block = reinterpret_cast<pointer>(std::malloc(size_block * sizeof(value_type)));
        if (_pointer_data_block == nullptr)
            throw std::bad_alloc();
		_memory_map_flags.fill(false);
	}

	~custom_allocator() 
	{
		std::free(_pointer_data_block);
	}

	template <typename U>
	struct rebind {
		using other = custom_allocator<U, size_block>;
	};

	pointer allocate(std::size_t number_of_elements) 
	{
		/*
		#ifndef USE_PRETTY
				std::cout << "allocate: [number_of_elements = " << number_of_elements << "]" << std::endl;
		#else
				std::cout << __PRETTY_FUNCTION__ << "[number_of_elements = " << number_of_elements << "]" << std::endl;
		#endif
		*/		

		if (size_block - _allocated_size < number_of_elements) 
		{ 
			throw std::bad_alloc(); 
		}

		std::size_t from = 0, to = 0;
		for (std::size_t i = 0; i < size_block; ++i) 
		{
			if (_memory_map_flags[i])
				continue;
			std::size_t j = i + 1;
			for (; j < size_block && !_memory_map_flags[j] && j - i + 1 <= number_of_elements; ++j) {}
			if (j - i == number_of_elements) 
			{
				from = i;
				to = j;
				break;
			}
		}
		if (from == 0 && to == 0) {
			throw std::bad_alloc();
		}
		for (std::size_t i = from; i < to; ++i) {
			_memory_map_flags[i] = true;
		}
		_allocated_size += number_of_elements;
		return _pointer_data_block + from;	// сдвигаем указатель
	}

	void deallocate(pointer p, std::size_t number_of_elements) 
	{
		/*
		#ifndef USE_PRETTY
				std::cout << "deallocate: [number_of_elements  = " << number_of_elements << "] " << std::endl;
		#else
				std::cout << __PRETTY_FUNCTION__ << "[number_of_elements = " << number_of_elements << "]" << std::endl;
		#endif
		*/
		std::size_t from = p - _pointer_data_block;

		for (std::size_t i = 0; i < number_of_elements; ++i, ++p) 
		{
			_memory_map_flags[from + i] = false;
		}
		_allocated_size -= number_of_elements;
	}

	template <typename U, typename... Args>
	void construct(U* p, Args&&... args) 
	{
		/*
		#ifndef USE_PRETTY
				std::cout << "construct" << std::endl;
		#else
				std::cout << __PRETTY_FUNCTION__ << std::endl;
		#endif
		*/
		new (p) U(std::forward<Args>(args)...);
	}

	void destroy(pointer p) 
	{
		/*
		#ifndef USE_PRETTY
				std::cout << "destroy" << std::endl;
		#else
				std::cout << __PRETTY_FUNCTION__ << std::endl;
		#endif
		*/
		p->~_T();
	}

private:
	pointer _pointer_data_block = nullptr;	// указатель на блок памяти размера size_block
	std::size_t _allocated_size = 0;	// 
	std::array<bool, size_block> _memory_map_flags;	// флаги для заполнения количства элементов в блоке
};
