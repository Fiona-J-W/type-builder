#include "../include/basic_number.hpp"
#include "utility"

template<int Tm, int Tkg, int Ts, int TA, int TK, int Tmol, int Tcd>
class _physical_t{
public:
	enum: int{
		m = Tm,     // meter
		kg = Tkg,   // kilogramm
		s = Ts,     // seconds
		A = TA,     // ampere
		K = TK,     // kelvin
		mol = Tmol, // mol
		cd = Tcd    // candela
	};
};

template<typename T, class Tid>
struct _physical_base : public type_builder::empty_base<T, Tid>{
	template<typename Tchar>
	static std::basic_string<Tchar> format(T value){
		//this is just a test so let's asume usage only with utf8:
		static_assert(std::is_same<Tchar, char>::value, 
				"cannot print physical with another format than utf8");
		using std::to_string;
		
		std::string returnstring{to_string(value)};
		for(const auto& unit: {
				std::pair<const int, const std::string>{Tid::m,"m"},
				std::pair<const int, const std::string>{Tid::kg,"kg"},
				std::pair<const int, const std::string>{Tid::s,"s"},
				std::pair<const int, const std::string>{Tid::A,"A"},
				std::pair<const int, const std::string>{Tid::K,"K"},
				std::pair<const int, const std::string>{Tid::mol,"mol"},
				std::pair<const int, const std::string>{Tid::cd,"cd"}
		}){	
			if(unit.first == 1){
				returnstring += unit.second;
			}
			else if(unit.first == 2){
				returnstring += unit.second + u8"²";
			}
			else if(unit.first == 3){
				returnstring += unit.second + u8"³";
			}
			else if(unit.first != 0){
				returnstring += unit.second + to_string(unit.first);
			}
		}
		
		return returnstring;
	}
	
	enum{
		USE_DEFAULT_STREAM_IN = true,
		USE_DEFAULT_STREAM_OUT = false
	};
};

template<int Tm, int Tkg, int Ts, int TA = 0, int TK = 0, int Tmol = 0, int Tcd = 0>
using physical = type_builder::basic_number<double, _physical_t<Tm, Tkg, Ts, TA, TK, Tmol, Tcd>, 
	type_builder::ENABLE_SPECIFIC_PLUS_MINUS | type_builder::ENABLE_FLOAT_MULT_DIV
	| type_builder::ENABLE_SPECIFIC_ORDERING, _physical_base>;

template<int Tm1, int Tkg1, int Ts1, int TA1, int TK1, int Tmol1, int Tcd1, 
	int Tm2, int Tkg2, int Ts2, int TA2, int TK2, int Tmol2, int Tcd2>
auto operator*(
	const physical<Tm1,Tkg1, Ts1, TA1, TK1, Tmol1, Tcd1>& a, 
	const physical<Tm2,Tkg2, Ts2, TA2, TK2, Tmol2, Tcd2>& b)->
		physical<Tm1+Tm2, Tkg1+Tkg2, Ts1+Ts2, TA1+TA2, TK1+TK2, Tmol1+Tmol2, Tcd1+Tcd2>{
	return physical<Tm1+Tm2, Tkg1+Tkg2, Ts1+Ts2, TA1+TA2, TK1+TK2, Tmol1+Tmol2, Tcd1+Tcd2>{
		a.get_value() * b.get_value()};
}

template<int Tm1, int Tkg1, int Ts1, int TA1, int TK1, int Tmol1, int Tcd1, 
	int Tm2, int Tkg2, int Ts2, int TA2, int TK2, int Tmol2, int Tcd2>
auto operator/(
	const physical<Tm1,Tkg1, Ts1, TA1, TK1, Tmol1, Tcd1>& a, 
	const physical<Tm2,Tkg2, Ts2, TA2, TK2, Tmol2, Tcd2>& b)->
		physical<Tm1-Tm2, Tkg1-Tkg2, Ts1-Ts2, TA1-TA2, TK1-TK2, Tmol1-Tmol2, Tcd1-Tcd2>{
	return physical<Tm1-Tm2, Tkg1-Tkg2, Ts1-Ts2, TA1-TA2, TK1-TK2, Tmol1-Tmol2, Tcd1-Tcd2>{
		a.get_value() / b.get_value()};
}

// base-units:

using meter = physical<1,0,0>;
using kilogramm = physical<0,1,0>;
using seconds = physical<0,0,1>;
using ampere = physical<0,0,0,1>;
using kelvin = physical<0,0,0,0,1>;
using mol = physical<0,0,0,0,0,1>;
using candela = physical<0,0,0,0,0,0,1>;
