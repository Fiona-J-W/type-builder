#include <iostream>

#include "../include/basic_number.hpp"

enum{
	length_id,
	duration_id, // 'time' would sadly produce name-clashes
	speed_id
};

enum: type_builder::flag_t{
	type_settings = 
		type_builder::ENABLE_SPECIFIC_PLUS_MINUS
		| type_builder::ENABLE_FLOAT_MULT_DIV
		| type_builder::ENABLE_SPECIFIC_ORDERING
};

using length = type_builder::basic_number<double, length_id, type_settings>;
using duration = type_builder::basic_number<double, duration_id, type_settings>;
using speed = type_builder::basic_number<double, speed_id, type_settings>;

speed operator/(length l, duration t){
	return speed{l.get_value() / t.get_value()};
}

length operator*(speed s, duration t){
	return length{s.get_value() * t.get_value()};
}

int main(){
	length l{10.0};
	duration t{3.0};
	speed s{10.0};
	std::cout << "speed: " << l/t << std::endl;
	std::cout << "duration: " << s*t << std::endl;
}
