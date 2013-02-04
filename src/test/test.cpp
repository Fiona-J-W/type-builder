#include "../include/basic_number.hpp"

#include <vector>
#include <iostream>
#include <atomic>


using test_type_1 = type_builder::basic_number<int, 1, type_builder::ENABLE_GENERAL_CONSTRUCTION>;
using test_type_2 = type_builder::basic_number<int, 2, type_builder::ENABLE_GENERAL_CONSTRUCTION 
	| type_builder::ENABLE_LATE_ASSIGNEMENT | type_builder::ENABLE_DEFAULT_CONSTRUCTION>;
using test_type_3 = type_builder::basic_number<test_type_2, 3, type_builder::ENABLE_DEFAULT_CONSTRUCTION>;

using mynum = type_builder::basic_number<long double, 4, type_builder::ENABLE_DEFAULT_CONSTRUCTION 
	| type_builder::ENABLE_SPECIFIC_PLUS_MINUS | type_builder::ENABLE_FLOAT_MULT_DIV 
	| type_builder::ENABLE_LATE_ASSIGNEMENT>;

using mighty = type_builder::basic_number<long double, 5, type_builder::ENABLE_ALL>;

using immutable_int = type_builder::basic_number<int, 6, type_builder::ENABLE_GENERAL_CONSTRUCTION
	| type_builder::DISABLE_MUTABILITY>;

enum{
	pm_test_flags = type_builder::DEFAULT_SETTINGS | type_builder::ENABLE_NATIVE_TYPING | type_builder::ENABLE_GENERAL_PLUS_MINUS
};
using pm_test = type_builder::basic_number<int, 7, pm_test_flags>;

int main(){
	mynum a(3.0l),
	      b(2.0l),
	      c(3.0l);
	a = b + c;
	std::cin >> a;
	std::cout << a;
	b + c;
	std::cout << ">>> ";
	std::cin >> a;
	a *= 3;
	std::cout << a << std::endl;
	test_type_3 foo(test_type_2(23));
	std::cout << foo << "\n>>> " << std::flush;
	std::cin >> foo;
	std::cout << foo << std::endl;
	
	immutable_int x{3};
	//foo = 3;
	
	pm_test pm_test_1{4};
	auto pm_test_2 = pm_test_1 + 1.0;
	static_assert( std::is_same<decltype(pm_test_2), type_builder::basic_number<double, 7, pm_test_flags>>::value, 
			"this types should be different");
}
