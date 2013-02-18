#include "../include/basic_number.hpp"

#include <vector>
#include <iostream>
#include <atomic>

struct test_type_1_t{};
using test_type_1 = type_builder::basic_number<int, test_type_1_t, type_builder::ENABLE_GENERAL_CONSTRUCTION>;

struct test_type_2_t{};
using test_type_2 = type_builder::basic_number<int, test_type_2_t, type_builder::ENABLE_GENERAL_CONSTRUCTION 
	| type_builder::ENABLE_LATE_ASSIGNEMENT | type_builder::ENABLE_DEFAULT_CONSTRUCTION>;
	
struct test_type_3_t{};
using test_type_3 = type_builder::basic_number<test_type_2, test_type_3_t, type_builder::ENABLE_DEFAULT_CONSTRUCTION>;

struct mynum_t{};
using mynum = type_builder::basic_number<long double, mynum_t, type_builder::ENABLE_DEFAULT_CONSTRUCTION 
	| type_builder::ENABLE_SPECIFIC_PLUS_MINUS | type_builder::ENABLE_FLOAT_MULT_DIV 
	| type_builder::ENABLE_LATE_ASSIGNEMENT>;

struct mighty_t{};
using mighty = type_builder::basic_number<long double, mighty_t, type_builder::ENABLE_ALL>;

struct immutable_int_t{};
using immutable_int = type_builder::basic_number<int, immutable_int_t, type_builder::ENABLE_GENERAL_CONSTRUCTION
	| type_builder::DISABLE_MUTABILITY>;

enum: type_builder::flag_t{
	pm_test_flags = type_builder::DEFAULT_SETTINGS | type_builder::ENABLE_NATIVE_TYPING | type_builder::ENABLE_GENERAL_PLUS_MINUS
};
struct pm_test_t{};
using pm_test = type_builder::basic_number<int, pm_test_t, pm_test_flags>;

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
	static_assert( std::is_same<decltype(pm_test_2), type_builder::basic_number<double, pm_test_t, pm_test_flags>>::value, 
			"this types should be different");
}
