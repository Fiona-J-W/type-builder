#include "../include/basic_number.hpp"

#include <cassert>

// defining constants is recommended if you need the same settings more 
// than once:
enum: uint64_t{
	COORD_SETTINGS = type_builder::DEFAULT_SETTINGS
		| type_builder::ENABLE_FLOAT_MULT_DIV
};

// it is recommended to use an enum to create different type-tags:
enum basic_number_types{
	x_coord_type,
	y_coord_type
};

// thanks to 'using' we can still keep some flexibility:
template<typename T>
using x_coord = type_builder::basic_number<T, x_coord_type, COORD_SETTINGS>;

template<typename T>
using y_coord = type_builder::basic_number<T, y_coord_type, COORD_SETTINGS>;

// and this is how the types can be used to ensure correctness:
template<typename T>
class point{
	public:
		point(x_coord<T> x, y_coord<T> y): x(x), y(y) {}
		
		point(): x(static_cast<T>(0)), y(static_cast<T>(0)) {}
		
		bool operator==(const point& other) const;
		
		bool operator!=(const point& other) const;
		
	private:
		x_coord<T> x;
		y_coord<T> y;
};

template<typename T>
bool point<T>::operator==(const point<T>& other) const{
	return (x==other.x) && (y==other.y);
}

template<typename T>
bool point<T>::operator!=(const point<T>& other) const{
	return !(*this == other);
}

int main(){
	x_coord<int> X1{1}, X2{0};
	y_coord<int> Y1{2}, Y2{2};
	point<int> p1{X1, Y1}, p2{X2, Y2};
	
	assert(X1 != X1*2);
	assert(p1 != p2);
	// This fails (as expected):
	//point<int> pn(X, X);
}
