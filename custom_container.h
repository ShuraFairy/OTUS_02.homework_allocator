#pragma once

#include <memory>
#include <stdexcept>
#include <iterator>

template <typename T>
struct forward_list_node 
{
	forward_list_node* _next;
	T	_value;

	forward_list_node() { _next = nullptr; _value = T(); };
};

template<typename _T>
struct custom_forward_list_iterator
{
	using _iterator			= custom_forward_list_iterator<_T>;
	using iterator_category = std::forward_iterator_tag;
	using value_type		= _T;
	using pointer			= const _T*;
	using reference			= const _T&;
	
	forward_list_node<_T>* _data;	//	Значение соответствующего узла в списке
	
	custom_forward_list_iterator() :_data{nullptr} { };
	
	explicit custom_forward_list_iterator(forward_list_node<_T>* pointer_node) :_data(pointer_node) {};

public:
	
	reference operator*() { return _data->_value; }	
	pointer operator->()  { return &(_data->_value); }
		
	_iterator& operator++()
	{ 
		if (nullptr == _data)	return *this;
		else _data = _data->_next;
		return *this;
	}
	
	bool operator==(const _iterator& rhs) const { return _data == rhs._data; }

	bool operator!=(const _iterator& rhs) const	{ return _data != rhs._data; }
};

template<typename _T, class _Allocator = std::allocator<_T>> 
class custom_forward_list 
{	
	using allocator_type	= _Allocator;
	using value_type		= typename _Allocator::value_type;
	using size_type			= typename _Allocator::size_type;	
	using reference			= typename _Allocator::reference;
	using const_reference	= typename _Allocator::const_reference;
	using pointer			= typename _Allocator::pointer;
	using const_pointer		= typename _Allocator::const_pointer;
	using pointer_node		= forward_list_node<_T>*;
	using list_type			= custom_forward_list<_T, _Allocator>;
	using iterator			= custom_forward_list_iterator<value_type>;
	using const_iterator	= custom_forward_list_iterator<value_type>;	
	using node_allocator	= typename _Allocator::template rebind<forward_list_node<_T>>::other;
	
	pointer_node _head;	
	pointer_node _tail;
	forward_list_node<_T> node_end;	
	node_allocator _allocator;
	size_type _size;
	
	pointer_node _create_node(const_reference new_value) 
	{		
		auto ptr = _allocator.allocate(1);
		//std::cout << "allocate in: " << ptr << std::endl;
		
		_allocator.construct(ptr);
		
		ptr->_value = new_value;
		ptr->_next = nullptr;
		//std::cout << "next: " << ptr->_next << std::endl;
		return ptr;
	};
	
	public:
		custom_forward_list() :_head(nullptr), _tail(nullptr), _allocator(), _size(0)
		{
			//std::cout << "sizeof list_node: " << sizeof(list_node<T>) << std::endl;
			//std::cout << "sizeof T: " << sizeof(T) << std::endl;
		};
		
		~custom_forward_list() 
		{
			auto curr = _head;
			while (curr != nullptr) 
			{
				//std::cout << "curr: " << curr << " next: " << curr->_next << std::endl;
				auto ptr_remove(curr);
				curr = curr->_next;	

				//std::cout << "deallocate: " << ptr_remove << std::endl;
				//std::cout << "next: " << ptr_remove->_next << std::endl;
				_allocator.destroy(ptr_remove);
				_allocator.deallocate(ptr_remove, 1);
			}
		};
		
		void push_back(const_reference value)
		{			
			auto _new_element = _create_node(value);
						
			if (_tail == nullptr) {
				_head = _new_element;
				_tail = _new_element;
			}
			else 
			{			
				_tail->_next = _new_element;
				//std::cout << "prev: " << _last << " next: " << _last->_next << " _last: " << new_element << std::endl;
				_tail = _new_element;
				_tail->_next = nullptr;
			}
			++_size;
		};

		iterator begin() noexcept {	return iterator(_head);	}
		iterator end() noexcept { return iterator(); }

		const_iterator cbegin() noexcept { return const_iterator(_head); }
		const_iterator cend() noexcept { return const_iterator(); }		

		bool empty() { return _head == nullptr; }
		size_type size() const { return _size; }
};
