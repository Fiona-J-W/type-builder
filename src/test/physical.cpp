#include <iostream>
#include <cassert>

#include "physical.hpp"

using meter_per_second = decltype(meter{} / seconds{});
using square_meter = decltype(meter{} * meter{});

int main(){
	meter l{10.0};
	seconds t{3.0};
	meter_per_second s1{10.0}, s2{l/t};
	square_meter area{100};
	std::cout << "speed: " << s2 << std::endl;
	std::cout << "distance: " << s1*t << std::endl;
	std::cout << "area: " << l * l << std::endl;
	assert(2*l*2*l*2 == 8*area);
	std::cout << "volume: " << l * l * l << std::endl;
}
