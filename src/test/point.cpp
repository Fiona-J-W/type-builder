#include "../include/basic_number.hpp"

#include <cassert>

enum: uint64_t{
	COORD_SETTINGS = type_builder::DEFAULT_SETTINGS
		| type_builder::ENABLE_FLOAT_MULT_DIV
		| type_builder::ENABLE_LATE_ASSIGNEMENT
};

template<typename T>
using x_coord = type_builder::basic_number<T, 0, COORD_SETTINGS>;

template<typename T>
using y_coord = type_builder::basic_number<T, 1, COORD_SETTINGS>;

template<typename T>
class point{
	public:
		point(x_coord<T> x, y_coord<T> y): x(x), y(y) {}
		x_coord<T> x;
		y_coord<T> y;
};

int main(){
	x_coord<int> X(1);
	y_coord<int> Y(2);
	point<int> p1(X, Y);
	assert(X != X*2);
	// This fails (as expected):
	//point<int> pn(X, X);
}
