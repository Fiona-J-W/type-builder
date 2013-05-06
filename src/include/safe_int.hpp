#ifndef TYPE_BUILDER_SAFE_INT_HPP
#define TYPE_BUILDER_SAFE_INT_HPP

#include <type_traits>
#include <cstdint>
#include <limits>
#include <climits>
#include <stdexcept>

namespace type_builder {

namespace impl{
	template<int Tsize, bool Tsigned> struct integer_type;
	template<typename Tlhs, typename Trhs> 
		struct shared_type; 
	template<typename Ttarget_type, typename Tsource_type>
		constexpr Ttarget_type checked_cast(const Tsource_type& src);
	template<typename Tsigned, typename Tunsigned>
		constexpr bool in_range(const Tunsigned& value);
	
	template<typename T, bool T_is_signed> struct sign_dependent_checks{
		constexpr static bool is_negative(const T& arg){
			return arg < 0;
		}
	};
	template<typename T> struct sign_dependent_checks<T, false>{
		constexpr static bool is_negative(const T&){
			return false;
		}
	};
}

template<typename T>
struct safe_int_default_properties{
	typedef T base_type;
	constexpr static bool is_signed = std::is_signed<T>::value;
	constexpr static base_type max = std::numeric_limits<T>::max();
	constexpr static base_type min = std::numeric_limits<T>::min();
	constexpr static int bits = sizeof(T) * CHAR_BIT;
};

template<typename T, typename Properties = safe_int_default_properties<T>>
class safe_int{
	static_assert(std::is_integral<T>::value, "safe_int holds only integral types");
	T val;
	
	public:
		typedef T base_type;
		constexpr static auto is_signed = Properties::is_signed;
		constexpr static auto max = Properties::max;
		constexpr static auto min = Properties::min;
		constexpr static auto bits = Properties::bits;
	
	private:
		
		constexpr static bool is_negative(const base_type& arg){
			return impl::sign_dependent_checks<base_type, is_signed>::is_negative(arg);
		}
		constexpr static bool is_positive(const base_type& arg){
			return !is_negative(arg);
		}
		
		template<typename Trhs>
		constexpr static bool shift_check(base_type lhs, Trhs rhs){
			return (is_negative(rhs) || rhs>= bits || is_negative(lhs))
				? throw std::domain_error{""}
				: true;
		}
		
	public:
		
		constexpr safe_int(): val{0}{}
		
		//non-explicit constructor because we want to have implicit conversions
		constexpr safe_int(T val): val{val}{}
		
		safe_int(const safe_int& other) = default;
		
		safe_int& operator=(const safe_int& other) = default;
		
		template<typename Tother,
			typename = typename std::enable_if<!std::is_same<Tother,T>::value>::type,
			typename = typename std::enable_if<std::is_integral<Tother>::value>::type,
			typename = typename std::enable_if<
				std::is_signed<Tother>::value && std::is_signed<T>::value
			>::type
		>
		safe_int(const Tother& val): val(val) {
			if(val > max || val < min){
				throw std::overflow_error{""};
			}
		}
		
		template<typename Tother,
			typename = typename std::enable_if<!std::is_same<Tother,T>::value>::type,
			typename = typename std::enable_if<std::is_integral<Tother>::value>::type,
			typename = typename std::enable_if<
				!std::is_signed<Tother>::value && !std::is_signed<T>::value>::type,
			typename = void>
		safe_int(const Tother& val): val(val) {
			if(val > max){
				throw std::overflow_error{""};
			}
		}
		
		template<typename Tother,
			typename = typename std::enable_if<!std::is_same<Tother,T>::value>::type,
			typename = typename std::enable_if<std::is_integral<Tother>::value>::type,
			typename = typename std::enable_if<
				!std::is_signed<Tother>::value && std::is_signed<T>::value>::type,
			typename = void, typename = void>
		safe_int(const Tother& val): val(val) {
			if(val > max){
				throw std::overflow_error{""};
			}
		}
		
		template<typename Tother,
			typename = typename std::enable_if<!std::is_same<Tother,T>::value>::type,
			typename = typename std::enable_if<std::is_integral<Tother>::value>::type,
			typename = typename std::enable_if<
				std::is_signed<Tother>::value && !std::is_signed<T>::value>::type,
			typename = void, typename = void, typename = void>
		safe_int(const Tother& val): val(val) {
			if(val > max || val < 0){
				throw std::overflow_error{""};
			}
		}
		
		friend constexpr bool operator==(const safe_int& lhs, const safe_int& rhs){
			return lhs.val == rhs.val;
		}
		friend constexpr bool operator!=(const safe_int& lhs, const safe_int& rhs){
			return lhs.val != rhs.val;
		}
		friend constexpr bool operator<(const safe_int& lhs, const safe_int& rhs){
			return lhs.val < rhs.val;
		}
		friend constexpr bool operator<=(const safe_int& lhs, const safe_int& rhs){
			return lhs.val <= rhs.val;
		}
		friend constexpr bool operator>(const safe_int& lhs, const safe_int& rhs){
			return lhs.val > rhs.val;
		}
		friend constexpr bool operator>=(const safe_int& lhs, const safe_int& rhs){
			return lhs.val >= rhs.val;
		}
		
		friend constexpr safe_int operator+(const safe_int& lhs, const safe_int& rhs){
			return (is_positive(rhs.val)&&(lhs.val > max - rhs.val)) ?
				throw std::overflow_error{""} :
				(lhs.val < min - rhs.val) ?
					throw std::underflow_error{""} :
					safe_int{ lhs.val + rhs.val };
		}
			
		friend constexpr safe_int operator-(const safe_int& lhs, const safe_int& rhs){
			return (is_negative(rhs.val)&&(lhs.val > max + rhs.val)) ?
				throw std::overflow_error{""} :
				(lhs.val < min + rhs.val) ?
					throw std::underflow_error{""} :
					safe_int{ lhs.val - rhs.val };
		}
			
		friend constexpr safe_int operator*(const safe_int& lhs, const safe_int& rhs){
			return (is_negative(rhs.val) ?
					(is_negative(lhs.val) ?
						!(lhs.val < max/rhs.val) :
						!(min/rhs.val < lhs.val)) :
					(is_negative(lhs.val) ?
						!(min/lhs.val < rhs.val) :
						!(max/rhs.val < lhs.val))
				) ? safe_int{lhs.val * rhs.val} :
				throw std::overflow_error{"overflow in multiplication"};
		}
			
		friend constexpr safe_int operator/(const safe_int& lhs, const safe_int& rhs){
			return (rhs.val == 0)?
				throw std::domain_error{"integer-division by 0"}
				: safe_int{lhs.val / rhs.val};
		}
			
		friend constexpr safe_int operator%(const safe_int& lhs, const safe_int& rhs){
			return (rhs.val == 0)?
				throw std::domain_error{"modulo of 0"}
				: safe_int{lhs.val % rhs.val};
		}
		
		safe_int& operator+=(const safe_int& other){
			return (*this = *this + other);
		}
		
		safe_int& operator-=(const safe_int& other){
			return (*this = *this - other);
		}
		
		safe_int& operator*=(const safe_int& other){
			return (*this = *this * other);
		}
		
		safe_int& operator/=(const safe_int& other){
			return (*this = *this / other);
		}
		
		safe_int& operator%=(const safe_int& other){
			return (*this = *this & other);
		}
		
		safe_int& operator++(){
			if(val == max){
				throw std::overflow_error{""};
			}
			++val;
			return *this;
		}
		
		safe_int operator++(int){
			if(val == max){
				throw std::overflow_error{""};
			}
			return safe_int{val++};
		}
		
		safe_int& operator--(){
			if(val == min){
				throw std::underflow_error{""};
			}
			--val;
			return *this;
		}
		
		safe_int operator--(int){
			if(val == min){
				throw std::underflow_error{""};
			}
			return safe_int{val--};
		}
		
		constexpr safe_int& operator+(){
			return *this;
		}
		
		template<typename = typename std::enable_if<is_signed>::type>
		safe_int operator-() const{
			return *this * safe_int{-1};
		}
		template<typename = typename std::enable_if<!is_signed>::type, typename = void>
		safe_int<typename impl::integer_type<bits, true>::type> operator-() const{
			return *this * safe_int<typename impl::integer_type<bits, true>::type>{-1};
		}
		
		constexpr base_type get_value() const {
			return val;
		}
		
		constexpr operator base_type() const {
			return val;
		}
		
		constexpr explicit operator bool() const{
			return val != 0;
		}
		
		constexpr bool operator!() const{
			return val == 0;
		}
		
		// the bit-stuff:
		constexpr friend safe_int operator|(const safe_int& lhs, const safe_int& rhs){
			return safe_int{lhs.val|rhs.val};
		}
		safe_int& operator|=(const safe_int& other){
			val|=other;
			return *this;
		}
		
		constexpr friend safe_int operator&(const safe_int& lhs, const safe_int& rhs){
			return safe_int{lhs.val&rhs.val};
		}
		safe_int& operator&=(const safe_int& other){
			val&=other;
			return *this;
		}
		
		constexpr friend safe_int operator^(const safe_int& lhs, const safe_int& rhs){
			return safe_int{lhs.val^rhs.val};
		}
		safe_int& operator^=(const safe_int& other){
			val^=other;
			return *this;
		}
		
		constexpr safe_int operator~(){
			return safe_int{~val};
		}
		
		template<typename Targ>
		constexpr friend safe_int operator<<(const safe_int& lhs, const safe_int<Targ>& rhs){
			return shift_check(lhs.val, rhs.val), safe_int{lhs.val<<rhs.val};
		}
		template<typename Targ>
		safe_int& operator <<=(const safe_int<Targ>& other){
			shift_check(val, other.val);
			val<<=other.val;
			return *this;
		}
		template<typename Targ>
		constexpr friend safe_int operator<<(const safe_int& lhs, const Targ& rhs){
			return shift_check(lhs.val, rhs),  safe_int{lhs.val<<rhs};
		}
		template<typename Targ>
		safe_int& operator <<=(const Targ& other){
			shift_check(val, other);
			val<<=other;
			return *this;
		}
		
		template<typename Targ>
		constexpr friend safe_int operator>>(const safe_int& lhs, const safe_int<Targ>& rhs){
				return shift_check(lhs.val, rhs.val), safe_int{lhs.val>>rhs.val};
		}
		template<typename Targ>
		safe_int& operator >>=(const safe_int<Targ>& other){
			shift_check(val, other.val);
			val>>=other.val;
			return *this;
		}
		template<typename Targ>
		constexpr friend safe_int operator>>(const safe_int& lhs, const Targ& rhs){
				return shift_check(lhs.val, rhs), safe_int{lhs.val>>rhs};
		}
		template<typename Targ>
		safe_int& operator >>=(const Targ& other){
			shift_check(val, other);
			val>>=other;
			return *this;
		}
};

template<typename Tlhs, typename Trhs>
auto operator+(safe_int<Tlhs> lhs, safe_int<Trhs> rhs)
-> safe_int<typename impl::shared_type<Tlhs, Trhs>::type>
{
	typedef typename impl::shared_type<Tlhs, Trhs>::type return_base;
	typedef safe_int<return_base> return_type;
	return return_type{impl::checked_cast<return_base>(lhs.get_value())} +
		return_type{impl::checked_cast<return_base>(rhs.get_value())};
}


template<typename Tlhs, typename Trhs>
auto operator-(safe_int<Tlhs> lhs, safe_int<Trhs> rhs)
-> safe_int<typename impl::shared_type<Tlhs, Trhs>::type>
{
	typedef typename impl::shared_type<Tlhs, Trhs>::type return_base;
	typedef safe_int<return_base> return_type;
	return return_type{impl::checked_cast<return_base>(lhs.get_value())} -
		return_type{impl::checked_cast<return_base>(rhs.get_value())};
}

template<typename Tlhs, typename Trhs>
auto operator*(safe_int<Tlhs> lhs, safe_int<Trhs> rhs)
-> safe_int<typename impl::shared_type<Tlhs, Trhs>::type>
{
	typedef typename impl::shared_type<Tlhs, Trhs>::type return_base;
	typedef safe_int<return_base> return_type;
	return return_type{impl::checked_cast<return_base>(lhs.get_value())} *
		return_type{impl::checked_cast<return_base>(rhs.get_value())};
}

template<typename Tlhs, typename Trhs>
constexpr auto operator/(safe_int<Tlhs> lhs, safe_int<Trhs> rhs)
-> safe_int<typename impl::shared_type<Tlhs, Trhs>::type>
{
	typedef typename impl::shared_type<Tlhs, Trhs>::type return_base;
	typedef safe_int<return_base> return_type;
	return return_type{impl::checked_cast<return_base>(lhs.get_value())} /
		return_type{impl::checked_cast<return_base>(rhs.get_value())};
}

template<typename Tlhs, typename Trhs>
constexpr auto operator%(safe_int<Tlhs> lhs, safe_int<Trhs> rhs)
-> safe_int<typename impl::shared_type<Tlhs, Trhs>::type>
{
	typedef typename impl::shared_type<Tlhs, Trhs>::type return_base;
	typedef safe_int<return_base> return_type;
	return return_type{impl::checked_cast<return_base>(lhs.get_value())} %
		return_type{impl::checked_cast<return_base>(rhs.get_value())};
}

// comparisions:

template<typename Tlhs, typename Trhs>
bool operator==(const safe_int<Tlhs>& lhs, const safe_int<Trhs>& rhs){
	using common_base = typename impl::shared_type<Tlhs, Trhs>::type;
	if(std::is_signed<common_base>::value){
		if(!std::is_signed<Tlhs>::value){
			if(!impl::in_range<common_base, Tlhs>(lhs.get_value())){
				return false;
			}
		}
		else if(!std::is_signed<Trhs>::value){
			if(!impl::in_range<common_base, Trhs>(rhs.get_value())){
				return false;
			}
		}
	}
	return static_cast<common_base>(lhs.get_value()) == static_cast<common_base>(rhs.get_value());
}

template<typename Tlhs, typename Trhs>
bool operator!=(const safe_int<Tlhs>& lhs, const safe_int<Trhs>& rhs){
	using common_base = typename impl::shared_type<Tlhs, Trhs>::type;
	if(std::is_signed<common_base>::value){
		if(!std::is_signed<Tlhs>::value){
			if(!impl::in_range<common_base, Tlhs>(lhs.get_value())){
				return true;
			}
		}
		else if(!std::is_signed<Trhs>::value){
			if(!impl::in_range<common_base, Trhs>(rhs.get_value())){
				return true;
			}
		}
	}
	return static_cast<common_base>(lhs.get_value()) != static_cast<common_base>(rhs.get_value());
}

template<typename Tlhs, typename Trhs>
bool operator<(const safe_int<Tlhs>& lhs, const safe_int<Trhs>& rhs){
	using common_base = typename impl::shared_type<Tlhs, Trhs>::type;
	if(std::is_signed<common_base>::value){
		if(!std::is_signed<Tlhs>::value){
			if(!impl::in_range<common_base, Tlhs>(lhs.get_value())){
				return false;
			}
		}
		else if(!std::is_signed<Trhs>::value){
			if(!impl::in_range<common_base, Trhs>(rhs.get_value())){
				return true;
			}
		}
	}
	return static_cast<common_base>(lhs.get_value()) < static_cast<common_base>(rhs.get_value());
}

template<typename Tlhs, typename Trhs>
bool operator<=(const safe_int<Tlhs>& lhs, const safe_int<Trhs>& rhs){
	using common_base = typename impl::shared_type<Tlhs, Trhs>::type;
	if(std::is_signed<common_base>::value){
		if(!std::is_signed<Tlhs>::value){
			if(!impl::in_range<common_base, Tlhs>(lhs.get_value())){
				return false;
			}
		}
		else if(!std::is_signed<Trhs>::value){
			if(!impl::in_range<common_base, Trhs>(rhs.get_value())){
				return true;
			}
		}
	}
	return static_cast<common_base>(lhs.get_value()) <= static_cast<common_base>(rhs.get_value());
}

template<typename Tlhs, typename Trhs>
bool operator>(const safe_int<Tlhs>& lhs, const safe_int<Trhs>& rhs){
	using common_base = typename impl::shared_type<Tlhs, Trhs>::type;
	if(std::is_signed<common_base>::value){
		if(!std::is_signed<Tlhs>::value){
			if(!impl::in_range<common_base, Tlhs>(lhs.get_value())){
				return true;
			}
		}
		else if(!std::is_signed<Trhs>::value){
			if(!impl::in_range<common_base, Trhs>(rhs.get_value())){
				return false;
			}
		}
	}
	return static_cast<common_base>(lhs.get_value()) > static_cast<common_base>(rhs.get_value());
}

template<typename Tlhs, typename Trhs>
bool operator>=(const safe_int<Tlhs>& lhs, const safe_int<Trhs>& rhs){
	using common_base = typename impl::shared_type<Tlhs, Trhs>::type;
	if(std::is_signed<common_base>::value){
		if(!std::is_signed<Tlhs>::value){
			if(!impl::in_range<common_base, Tlhs>(lhs.get_value())){
				return true;
			}
		}
		else if(!std::is_signed<Trhs>::value){
			if(!impl::in_range<common_base, Trhs>(rhs.get_value())){
				return false;
			}
		}
	}
	return static_cast<common_base>(lhs.get_value()) >= static_cast<common_base>(rhs.get_value());
}


namespace impl{

template<int Tsize, bool Tsigned>
struct integer_type{};

template<> struct integer_type<8,false>{typedef std::uint8_t type;};
template<> struct integer_type<16,false>{typedef std::uint16_t type;};
template<> struct integer_type<32,false>{typedef std::uint32_t type;};
template<> struct integer_type<64,false>{typedef std::uint64_t type;};

template<> struct integer_type<8,true>{typedef std::int8_t type;};
template<> struct integer_type<16,true>{typedef std::int16_t type;};
template<> struct integer_type<32,true>{typedef std::int32_t type;};
template<> struct integer_type<64,true>{typedef std::int64_t type;};

template<typename Tlhs, typename Trhs>
struct shared_type{
	typedef typename integer_type<
			(sizeof(Tlhs) > sizeof(Trhs)? sizeof(Tlhs) : sizeof(Trhs)) * 8,
			std::is_signed<Tlhs>::value || std::is_signed<Trhs>::value
		>::type type;
};

template<typename Tsource_type, typename Ttarget_type, bool Trequire_check>
struct convertable_check{
	constexpr static bool check(const Tsource_type& value){
	return (value > static_cast<Tsource_type>(std::numeric_limits<Ttarget_type>::max()))
			? throw std::overflow_error("conversion failure") 
			: true;
	}
};
template<typename Tsource_type, typename Ttarget_type>
struct convertable_check<Tsource_type, Ttarget_type, false>{
	constexpr static bool check(const Tsource_type&){
		return true;
	}
};

template<typename Ttarget_type, typename Tsource_type>
constexpr Ttarget_type checked_cast(const Tsource_type& src){
	static_assert(sizeof(Ttarget_type) >= sizeof(Tsource_type), 
			"checked cast is not to be used for narrowing casts");
	static_assert(std::is_signed<Tsource_type>::value ? std::is_signed<Ttarget_type>::value : true,
			"checked cast does not cast from signed to unsigned");
	return convertable_check<
		Tsource_type,
		Ttarget_type,
		std::is_signed<Ttarget_type>::value&&
			(!std::is_signed<Tsource_type>::value)
		>::check(src) ? 
		static_cast<Ttarget_type>(src) 
		: /* this will never be done:*/ throw std::exception{};
}

template<typename Tsigned, typename Tunsigned>
constexpr bool in_range(const Tunsigned& value){
	return value <= static_cast<Tunsigned>(std::numeric_limits<Tsigned>::max());
}

} // namespace impl

} // namespace type_builder

#endif
