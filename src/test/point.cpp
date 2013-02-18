#include "../include/basic_number.hpp"

#include <cassert>
#include <string>
#include <iostream>

// If we wish to, we can define our own unit by creating a type that provides these methods:
template<typename T, class Tid>
struct meter_t : public type_builder::empty_base<T, Tid>{
	template<typename Tchar>
	static std::basic_string<Tchar> format(T value){
		//this is just a test so let's asume usage only with utf8:
		static_assert(std::is_same<Tchar, char>::value, 
				"cannot print meter with another format than utf8");
		return std::to_string(value) + "m";
	}
	
	template<typename Tchar>
	static T read_istream(std::basic_istream<Tchar>& stream){
		T returnval;
		stream >> returnval;
		std::string tmp;
		stream >> tmp;
		if(tmp != "m"){
			
		}
		return returnval;
	}
	
	enum{
		USE_DEFAULT_STREAM_IN = false,
		USE_DEFAULT_STREAM_OUT = false
	};
};

// defining constants is recommended if you need the same settings more 
// than once:
enum: type_builder::flag_t{
	COORD_SETTINGS = type_builder::DEFAULT_SETTINGS
		| type_builder::ENABLE_FLOAT_MULT_DIV
};


struct x_coord_type{};
struct y_coord_type{};


// thanks to 'using' we can still keep some flexibility:
template<typename T>
using x_coord = type_builder::basic_number<T, x_coord_type, COORD_SETTINGS, meter_t>;

template<typename T>
using y_coord = type_builder::basic_number<T, y_coord_type, COORD_SETTINGS, meter_t>;

// and this is how the types can be used to ensure correctness:
template<typename T>
class point{
	public:
		point(const x_coord<T>& x, const y_coord<T>& y): x(x), y(y) {}
		
		point(const point& other) = default;
		
		point(): x(static_cast<T>(0)), y(static_cast<T>(0)) {}
		
		bool operator==(const point& other) const;
		
		bool operator!=(const point& other) const;
		
		template<typename Tother>
		point operator*(Tother other){
			return {x*other, y*other};
		}
		
		template<typename Tother>
		point& operator*=(Tother other){
			x *= other;
			y *= other;
			return *this;
		}
		
		x_coord<T> get_x() const {return x;}
		y_coord<T> get_y() const {return y;}
		
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

template<typename T>
std::ostream& operator<<(std::ostream& stream, const point<T>& p){
	stream << "[" << p.get_x() << ", " << p.get_y() << "]";
	return stream;
}

int main(){
	x_coord<int> X1{1}, X2{0}, X3{X1 + X2};
	y_coord<int> Y1{2}, Y2{2};
	point<int> p1{X1, Y1}, p2{X2, Y2};
	std::cout << p1 << std::endl;
	assert(X1 != X1*2);
	assert(p1 != p2);
	static_assert(std::is_same<decltype(X2), decltype(X1)>::value, "strange error");
	X2 = X1;
	X1 = X3 - X2;
	assert(((X1 = X1)) == X1);
	
	point<short> ps1{x_coord<short>{short{6}}, y_coord<short>{short{8}}};
	point<short> ps2{x_coord<short>{short{3}} * 2, 2 * y_coord<short>{short{4}}};
	assert(ps1 == ps2);
	ps1 *= 6;
	assert(ps1*0.5 == ps2*3);
	// This fails (as expected):
	//point<int> pn(X, X);
}


