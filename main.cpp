
#include <iostream>
#include <map>
#include "custom_allocator.h"
#include "custom_container.h"

int main([[maybe_unused]]int argc, [[maybe_unused]] char const* argv[])
{     
    try
    {        
        std::map<int, int, std::less<int>, custom_allocator<std::pair<int, size_t>, 10>> map_with_custom_allocator;
        
        auto value{ 1 };
        for (auto key = 0; key < 10; ++key)
        {
            value = value * ((key > 0) ? key : 1);
            map_with_custom_allocator[key] = value;
        }        

        for (const auto & element : map_with_custom_allocator)
            std::cout << "key: " << element.first << " " << "value: " << element.second << std::endl;

        custom_forward_list<int> list;        

        for (auto i = 0; i < 10; ++i)            
            list.push_back(i);
        
        for (const auto & element : list)
            std::cout << "custom_forward_list with std::allocator: " << element << std::endl;

        for (auto it = list.begin(); it != list.end(); ++it)
            std::cout << "custom_forward_list with std::allocator, iterator: " << *it << std::endl;        

        custom_forward_list<int, custom_allocator<int, 10>> list_with_custom_allocator;

        for (int i = 0; i < 10; ++i)            
            list_with_custom_allocator.push_back(i);
        
        for (const auto & element : list_with_custom_allocator)
            std::cout << "custom_forward_list with custom_allocator: " << element << std::endl;

        for (auto it = list_with_custom_allocator.begin(); it != list_with_custom_allocator.end(); ++it)
            std::cout << "custom_forward_list with custom_allocator, iterator: " << *it << std::endl;            
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

	return EXIT_SUCCESS;
}
