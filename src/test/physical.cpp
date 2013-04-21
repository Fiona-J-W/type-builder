#include <iostream>
#include <cassert>
#include <sstream>

#include "physical.hpp"
#include <type_traits>

using meter_per_second = decltype(meter{1.0} / second{1.0});
using square_meter = decltype(meter{1.0} * meter{1.0});

meter_per_second operator"" _mps(long double val){
	return meter_per_second{val};
}

square_meter operator"" _m2(long double val){
	return square_meter{val};
}

meter operator"" _m(long double val){
	return meter{val};
}

second operator"" _s(long double val){
	return second{val};
}


int main(){
	auto l = 10.0_m;
	auto t = 3.0_s;
	//meter_per_second s1{10.0};
	auto s1 = 10.0_mps;
	auto s2 = l/t;
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
	
	auto m = 0.0_m;
	auto s = 0.0_s;
	std::cout << "meter> ";
	std::cin >> m;
	std::cout << m << "\nsecond> ";
	std::cin >> s;
	std::cout << s << std::endl;
}
