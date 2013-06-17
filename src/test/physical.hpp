#ifndef TYPE_BUILDER_PHYSICAL_HPP
#define TYPE_BUILDER_PHYSICAL_HPP

#include "../include/basic_number.hpp"
#include <utility>
#include <string>
#include <stdexcept>

template<int Tm, int Tkg, int Ts, int TA, int TK, int Tmol, int Tcd>
class physical_t{
public:
	enum: int{
		m = Tm,     // meter
		kg = Tkg,   // kilogramm
		s = Ts,     // second
		A = TA,     // ampere
		K = TK,     // kelvin
		mol = Tmol, // mol
		cd = Tcd    // candela
	};
};

/**
 * @brief Signals that the data in an istream cannot be converted to the requested type.
 */
struct format_error: std::runtime_error{
	using runtime_error::runtime_error;
};

template<typename T, class Tid>
class physical_base : public type_builder::empty_base<T, Tid>{	
public:
	
	template<typename Tchar>
	static std::basic_string<Tchar> get_unit_str(){
		static_assert(std::is_same<Tchar, char>::value, 
				"priting is done with utf8");
		using std::to_string;
		
		std::basic_string<Tchar> returnstring;
		using pair = std::pair<const int, const std::string>;
		for(const auto& unit: {
				pair{Tid::m,"m"},
				pair{Tid::kg,"kg"},
				pair{Tid::s,"s"},
				pair{Tid::A,"A"},
				pair{Tid::K,"K"},
				pair{Tid::mol,"mol"},
				pair{Tid::cd,"cd"}
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
				returnstring += unit.second + "^" + to_string(unit.first);
			}
		}
		return returnstring;
		
	}

	template<typename Tchar>
	static std::basic_string<Tchar> format(const T& value){
		return (std::to_string(value) + get_unit_str<Tchar>());
	}
	
	template<typename Tchar>
	static T read_istream(std::basic_istream<Tchar>& stream) {
		const static std::basic_string<Tchar> unit_str = get_unit_str<Tchar>();
		const static typename std::basic_string<Tchar>::size_type unit_str_len = unit_str.size();
		std::basic_string<Tchar> tmp;
		tmp.resize(unit_str_len);
		
		T returnval;;
		stream >> returnval;
		stream.read(&tmp[0], unit_str_len);
		if(tmp != unit_str){
			throw format_error("invalid unit in istream");
		}
		return returnval;
	}
	
	enum{
		USE_DEFAULT_STREAM_IN = false,
		USE_DEFAULT_STREAM_OUT = false
	};
};


template<int Tm, int Tkg, int Ts, int TA, int TK, int Tmol, int Tcd, typename T>
using physical = type_builder::basic_number<T, physical_t<Tm, Tkg, Ts, TA, TK, Tmol, Tcd>,
	type_builder::ENABLE_SPECIFIC_PLUS_MINUS | type_builder::ENABLE_BASE_MULT_DIV
	| type_builder::ENABLE_SPECIFIC_ORDERING,
	physical_base>;

template<int Tm1, int Tkg1, int Ts1, int TA1, int TK1, int Tmol1, int Tcd1, typename T1,
	int Tm2, int Tkg2, int Ts2, int TA2, int TK2, int Tmol2, int Tcd2, typename T2>
physical<Tm1+Tm2, Tkg1+Tkg2, Ts1+Ts2, TA1+TA2, TK1+TK2, Tmol1+Tmol2, Tcd1+Tcd2,
	decltype(std::declval<T1>()*std::declval<T2>())>
operator*(const physical<Tm1,Tkg1, Ts1, TA1, TK1, Tmol1, Tcd1, T1>& a,
		const physical<Tm2,Tkg2, Ts2, TA2, TK2, Tmol2, Tcd2, T2>& b){
	return decltype(a*b){a.get_value() * b.get_value()};
}

template<int Tm1, int Tkg1, int Ts1, int TA1, int TK1, int Tmol1, int Tcd1, typename T1,
	int Tm2, int Tkg2, int Ts2, int TA2, int TK2, int Tmol2, int Tcd2, typename T2>
physical<Tm1-Tm2, Tkg1-Tkg2, Ts1-Ts2, TA1-TA2, TK1-TK2, Tmol1-Tmol2, Tcd1-Tcd2,
	decltype(std::declval<T1>()/std::declval<T2>())> 
operator/(const physical<Tm1,Tkg1, Ts1, TA1, TK1, Tmol1, Tcd1, T1>& a,
		const physical<Tm2,Tkg2, Ts2, TA2, TK2, Tmol2, Tcd2, T2>& b){
	return decltype(a/b){a.get_value() / b.get_value()};
}


using default_physical_t = double;

// base-units:
template<typename T>
using meter_t = physical<1,0,0,0,0,0,0,T>;
using meter = meter_t<default_physical_t>;

template<typename T>
using kilogramm_t = physical<0,1,0,0,0,0,0,T>;
using kilogramm = kilogramm_t<default_physical_t>;

template<typename T>
using second_t = physical<0,0,1,0,0,0,0,T>;
using second = second_t<default_physical_t>;

template<typename T>
using ampere_t = physical<0,0,0,1,0,0,0,T>;
using ampere = ampere_t<default_physical_t>;

template<typename T>
using kelvin_t = physical<0,0,0,0,1,0,0,T>;
using kelvin = kelvin_t<default_physical_t>;

template<typename T>
using mol_t = physical<0,0,0,0,0,1,0,T>;
using mol = mol_t<default_physical_t>;

template<typename T>
using candela_t = physical<0,0,0,0,0,0,1,T>;
using candela =  candela_t<default_physical_t>;

#endif
