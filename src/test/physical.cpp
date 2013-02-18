#include <iostream>
#include <cassert>
#include <sstream>

#include "../include/physical.hpp"

using type_builder::meter;
using type_builder::seconds;

using meter_per_second = decltype(meter{} / seconds{});
using square_meter = decltype(meter{} * meter{});

int main(){
	meter l{10.0};
	seconds t{3.0};
	meter_per_second s1{10.0}, s2{l/t};
	square_meter area{100};
	std::cout << "speed: " << s1 << std::endl;
	std::cout << "distance: " << s2*t << std::endl;
	std::cout << "area: " << l * l << std::endl;
	assert(2*l*2*l*2 == 8.0*area);
	std::cout << "volume: " << l * l * l << std::endl;
	
	std::stringstream teststream;
	teststream << area;
	square_meter area_2{0};
	teststream >> area_2;
	assert(area == area_2);
	
	meter m{0};
	seconds s{0};
	std::cout << "meter> ";
	std::cin >> m;
	std::cout << m << "\nseconds> ";
	std::cin >> s;
	std::cout << s << std::endl;
}
