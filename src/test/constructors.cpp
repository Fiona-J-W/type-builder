#include "../include/basic_number.hpp"

#include <functional>
#include <vector>
#include <iostream>
#include <atomic>

struct test_type_1_t{};
using test_type_1 = type_builder::basic_number<int, test_type_1_t, type_builder::DISABLE_CONSTRUCTION>;

struct test_type_2_t{};
using test_type_2 = type_builder::basic_number<int, test_type_2_t, 0>;

struct test_type_3_t{};
using test_type_3 = type_builder::basic_number<int, test_type_3_t, type_builder::ENABLE_LATE_ASSIGNEMENT>;

int main(){
	// test_type_1 t1; // fails correctly
	
	// test_type_2 t2_1; // fails correctly
	test_type_2 t2_2{1}; // works correctly
	//t2_2 = 1; //fails correctly
	t2_2 = test_type_2{2}; // works ?
	
	//test_type_3 t3_1; // fails correctly
	test_type_3 t3_2{1}; // works correctly
	t3_2 = 1; // works correctly
	t3_2 = []()->int&{static int i = 0; return i;}(); // works correctly
	t3_2 = []()->const int&{static int i = 0; return i;}(); // works correctly
	t3_2 = test_type_3{3}; // works ?
}
