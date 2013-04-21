#include <complex>
#include <iostream>

#include "../include/basic_number.hpp"

using cmplx = std::complex<double>;

struct complex_id_t{};
using my_complex = type_builder::basic_number<cmplx, complex_id_t,
	type_builder::DEFAULT_SETTINGS | type_builder::ENABLE_GENERAL_MULT_DIV>;

int main(){
	my_complex c1{cmplx{1,0}}, c2{cmplx{2,1}};
	std::cout << c1 << "; " << c2 << std::endl;
	std::cout << c1 + c2 << std::endl;
	std::cout << (c1+c2)*cmplx{3} << std::endl;
	std::cout << (c1+c2)*cmplx{3,4} << std::endl;
	std::cout << (c1+c2)*cmplx{0,1} << std::endl;
	std::cout << (c1+c2)*5.0 << std::endl;
}
	
