#include <iostream>

#include "../include/basic_number.hpp"


enum: type_builder::flag_t{
	type_settings = 
		type_builder::ENABLE_SPECIFIC_PLUS_MINUS
		| type_builder::ENABLE_FLOAT_MULT_DIV
		| type_builder::ENABLE_SPECIFIC_ORDERING
};

template<int Tm, int Tk, int Ts>
class physical_t{
public:
	enum: int{
		M = Tm,
		K = Tk,
		S = Ts
	};
};

template<typename T, class Tid>
struct physical_base : public type_builder::empty_base<T, Tid>{
	template<typename Tchar>
	static std::basic_string<Tchar> format(T value){
		//this is just a test so let's asume usage only with utf8:
		static_assert(std::is_same<Tchar, char>::value, 
				"cannot print meter with another format than utf8");
		using std::to_string;
		
		std::string returnstring{to_string(value)};
		
		if(Tid::M == 1){
			returnstring += "m";
		}
		else if(Tid::M < 0 || Tid::M >0){
			returnstring += "m^" + to_string(Tid::M);
		}
		
		if(Tid::K == 1){
			returnstring += "kg";
		}
		else if(Tid::K < 0 || Tid::K >0){
			returnstring += "kg^" + to_string(Tid::K);
		}
		
		if(Tid::S == 1){
			returnstring += "s";
		}
		else if(Tid::S < 0 || Tid::S >0){
			returnstring += "s^" + to_string(Tid::S);
		}
		
		return returnstring;
	}
	
	enum{
		USE_DEFAULT_STREAM_IN = true,
		USE_DEFAULT_STREAM_OUT = false
	};
};

template<int Tm, int Tk, int Ts>
using physical = type_builder::basic_number<double, physical_t<Tm, Tk, Ts>, type_settings, physical_base>;

template<int M1, int K1, int S1, int M2, int K2, int S2>
auto operator*(const physical<M1,K1,S1>& a, const physical<M2,K2,S2>& b)->
		physical<M1+M2, K1+K2, S1+S2>{
	return physical<M1+M2, K1+K2, S1+S2>{a.get_value() * b.get_value()};
}

template<int M1, int K1, int S1, int M2, int K2, int S2>
auto operator/(const physical<M1,K1,S1>& a, const physical<M2,K2,S2>& b)->
		physical<M1-M2, K1-K2, S1-S2>{
	return physical<M1-M2, K1-K2, S1-S2>{a.get_value() / b.get_value()};
}


using length = physical<1,0,0>;
using duration = physical<0,0,1>;
using speed = physical<1,0,-1>;

int main(){
	length l{10.0};
	duration t{3.0};
	speed s{10.0};
	std::cout << "speed: " << l/t << std::endl;
	std::cout << "distance: " << s*t << std::endl;
}
