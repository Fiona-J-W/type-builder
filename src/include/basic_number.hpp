#ifndef BASIC_NUMBER_HPP
#define BASIC_NUMBER_HPP

#include <utility>
#include <cstdint>
#include <ostream>
#include <istream>
#include <string>
#include <type_traits>

#include "policy_types.hpp"

namespace type_builder{

using flag_t = uint64_t;

/**
 * @brief A collection of the available flags for basic_number.
 */
enum: flag_t{
	ENABLE_GENERAL_CONSTRUCTION = flag_t{1},
	ENABLE_DEFAULT_CONSTRUCTION = flag_t{1} << 1,
	ENABLE_LATE_ASSIGNEMENT = flag_t{1} << 2,
	ENABLE_SPECIFIC_EQUALITY_CHECK = flag_t{1} << 3,
	ENABLE_SPECIFIC_ORDERING = ENABLE_SPECIFIC_EQUALITY_CHECK | flag_t{1} << 4,
	ENABLE_EQUALITY_CHECK = ENABLE_SPECIFIC_EQUALITY_CHECK | flag_t{1} << 5,
	ENABLE_ORDERING = ENABLE_SPECIFIC_ORDERING | ENABLE_EQUALITY_CHECK | flag_t{1} << 6,
	
	ENABLE_INC_DEC = flag_t{1} << 7,
	
	ENABLE_SPECIFIC_PLUS_MINUS = flag_t{1} << 8,
	ENABLE_SPECIFIC_MULTIPLICATION = flag_t{1} << 9,
	ENABLE_SPECIFIC_DIVISION = flag_t{1} << 9,
	
	ENABLE_INTEGER_MULTIPLICATION = flag_t{1} << 10,
	ENABLE_INTEGER_DIVISION = flag_t{1} << 11,
	
	ENABLE_FLOAT_MULTIPLICATION = ENABLE_INTEGER_MULTIPLICATION | flag_t{1} << 12,
	ENABLE_FLOAT_DIVISION = ENABLE_INTEGER_DIVISION | flag_t{1} << 13,
	
	ENABLE_INTEGER_MULT_DIV = ENABLE_INTEGER_MULTIPLICATION | ENABLE_INTEGER_DIVISION,
	ENABLE_FLOAT_MULT_DIV = ENABLE_FLOAT_MULTIPLICATION | ENABLE_FLOAT_DIVISION,
	
	ENABLE_GENERAL_PLUS_MINUS = flag_t{1} << 20,
	ENABLE_GENERAL_MULTIPLICATION = flag_t{1} << 21,
	ENABLE_GENERAL_DIVISION = flag_t{1} << 22,
	ENABLE_GENERAL_MULT_DIV = ENABLE_GENERAL_MULTIPLICATION | ENABLE_GENERAL_DIVISION,
	
	ENABLE_SPECIFIC_MODULO = flag_t{1} << 26,
	ENABLE_MODULO = ENABLE_SPECIFIC_MODULO | flag_t{1} << 27,
	
	ENABLE_NATIVE_TYPING = flag_t{1} << 30,
	
	DISABLE_CONSTRUCTION = flag_t{1} << 60,
	DISABLE_MUTABILITY = flag_t{1} << 61,
	
	DEFAULT_SETTINGS = ENABLE_SPECIFIC_ORDERING | ENABLE_INC_DEC
		| ENABLE_SPECIFIC_PLUS_MINUS
		| ENABLE_INTEGER_MULTIPLICATION | ENABLE_INTEGER_DIVISION,
	
	ENABLE_ALL_SPECIFIC_MATH = ENABLE_INC_DEC | ENABLE_SPECIFIC_PLUS_MINUS
		| ENABLE_SPECIFIC_MULTIPLICATION | ENABLE_SPECIFIC_DIVISION
		| ENABLE_SPECIFIC_MODULO,
	
	ENABLE_ALL_MATH = ENABLE_ALL_SPECIFIC_MATH | ENABLE_GENERAL_PLUS_MINUS 
		| ENABLE_GENERAL_MULTIPLICATION | ENABLE_GENERAL_DIVISION
		| ENABLE_FLOAT_MULT_DIV | ENABLE_MODULO,
	
	ENABLE_ALL = ENABLE_GENERAL_CONSTRUCTION | ENABLE_DEFAULT_CONSTRUCTION 
		| ENABLE_LATE_ASSIGNEMENT | ENABLE_ORDERING 
		| ENABLE_ALL_MATH | ENABLE_NATIVE_TYPING
};

/**
 * @brief A template for a number-class.
 */
// note that the operators are not documented with doxygen, 
// because they behave pretty much as you would expect anyway.
template<
	typename T,
	int Tid,
	flag_t Tflags = DEFAULT_SETTINGS,
	template<typename> class Tbase = empty_base>
class basic_number: public Tbase<T> {
	//put the private stuff to the begining because we will need it in 
	// signatures:
	
	/**
	 * @brief The value of the number.
	 */
	T value;
	
	/**
	 * @brief Checks if a type is identical to *this.
	 * @return true if it is, false otherwise.
	 */
	template<typename Targ> struct is_this{ 
		enum : bool{
			value = ( std::is_same<const basic_number&, const Targ&>::value 
				|| std::is_same<basic_number&, Targ&>::value)
		};
	};
	
	static_assert(is_this<basic_number>::value, "is_this doesn't work.");
	static_assert(!is_this<basic_number<T,Tid+1>>::value, "is_this doesn't work.");
	static_assert(!is_this<int>::value, "is_this doesn't work.");
	
	/**
	 * @brief Provides the correct returntype for some functions.
	 * @param T1 the return type if native typing is enabled
	 * @param T2 the return type if native typing is disabled
	 */
	template<typename T1, typename T2>
	struct return_type{
		using type = typename std::conditional<(Tflags & ENABLE_NATIVE_TYPING) != 0, T1, T2>::type;
	};
	
	// implementation for is_equivalent_basic_number
	template<typename Targ>
	struct _is_equivalent_basic_number{
		enum : bool{
			value = 0
		};
	};
	// dito:
	template<typename Targ>
	struct _is_equivalent_basic_number<basic_number<Targ, Tid, Tflags, Tbase>&>{
		enum : bool{
			value = (Tflags & ENABLE_NATIVE_TYPING) ? true : is_this<basic_number<Targ, Tid, Tflags, Tbase>>::value
		};
	};
	
	/**
	 * @brief Checks whether a type is semanticly equivalent to *this.
	 * @param Targ the type that will be checked.
	 * @return true if it is equivalent, false otherwise
	 */
	template<typename Targ>
	struct is_equivalent_basic_number{
		enum : bool{
			value = _is_equivalent_basic_number<
				typename std::remove_const<
					typename std::remove_reference<Targ>::type
				>::type&>::value
		};
	};
	
	static_assert(is_equivalent_basic_number<basic_number>::value, 
		"is_equivalent_basic_number doesn't work.");
	static_assert(is_equivalent_basic_number<basic_number&>::value, 
		"is_equivalent_basic_number doesn't work.");
	static_assert(is_equivalent_basic_number<const basic_number>::value, 
		"is_equivalent_basic_number doesn't work.");
	static_assert(is_equivalent_basic_number<const basic_number&>::value, 
		"is_equivalent_basic_number doesn't work.");
	static_assert(is_equivalent_basic_number<basic_number&&>::value, 
		"is_equivalent_basic_number doesn't work.");
	
	public:
		
		// constructors:
		
		// clang says I cannot disable this constructor with std::enable_if :-(
		basic_number() : value(Tbase<T>::default_value()) {
			static_assert( !(Tflags & DISABLE_CONSTRUCTION), 
					"construction of this type is disabled");
			static_assert( Tflags & ENABLE_DEFAULT_CONSTRUCTION,
					"default constructor not enabled for this type");
		}
		
		template<typename Tother>
		explicit basic_number(Tother&& value,
			typename std::enable_if<(std::is_same<Tother&, T&>::value)
				&& (!(Tflags & DISABLE_CONSTRUCTION))>::type* = 0
		): value(std::forward<T>(value)){}
		
		template<typename Tother>
		basic_number(Tother&& other, 
			typename std::enable_if<(is_equivalent_basic_number<Tother>::value) 
				&& (!(Tflags & DISABLE_CONSTRUCTION))>::type* = 0
		): value(std::forward<T>(other.get_value())){}
		
		template<typename Tother>
		explicit basic_number(Tother&& value,
			typename std::enable_if<(!is_equivalent_basic_number<Tother>::value) 
				&& (!std::is_same<Tother&, T&>::value)
				&& (!(Tflags & DISABLE_CONSTRUCTION))
				&& (Tflags & ENABLE_GENERAL_CONSTRUCTION)>::type* = 0
		): value(std::forward<T>(value)){}
		
		// methods:
		
		// *this = basic_number
		template<typename Tother> 
		typename std::enable_if<
			is_this<Tother>::value
			&& (!(Tflags & DISABLE_MUTABILITY)),
		basic_number&>::type
		operator=(Tother&& other){
			// we should check for self-assignement:
			if(this != &other){
				this->value = std::forward<Tother>(other).value;
			}
			return *this;
		}
		
		// *this = Tother
		template<typename Tother>
		typename std::enable_if< 
			!is_this<Tother>::value
			&& !(std::is_same<T&, Tother&>::value)
			&& (Tflags & ENABLE_LATE_ASSIGNEMENT)
			&& (Tflags & ENABLE_GENERAL_CONSTRUCTION),
		basic_number&>::type
		operator=(Tother&& value){
			this->value = std::forward<Tother>(value);
			return *this;
		}
		
		// *this == basic_number
		template <typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_EQUALITY_CHECK), bool>::type
		operator==(Tother&& other) const{
			return value == other.get_value();
		}
		
		// *this == Tother
		template <typename Tother>
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& (Tflags & ENABLE_EQUALITY_CHECK), bool>::type
		operator==(Tother&& other) const{
			return value == other;
		}
		
		// *this != basic_number
		template <typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_EQUALITY_CHECK), bool>::type
		operator!=(Tother&& other) const{
			return value != other.get_value();
		}
		
		// *this != Tother
		template <typename Tother>
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& (Tflags & ENABLE_EQUALITY_CHECK), bool>::type
		operator!=(Tother&& other) const{
			return value != other;
		}
		
		// *this < basic_number
		template <typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_ORDERING), bool>::type
		operator<(Tother&& other) const{
			return value < other.get_value();
		}
		
		// *this < Tother
		template <typename Tother>
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& (Tflags & ENABLE_ORDERING), bool>::type
		operator<(Tother&& other) const{
			return value < other;
		}
		
		// *this <= basic_number
		template <typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_ORDERING), bool>::type
		operator<=(Tother&& other) const{
			return value <= other.get_value();
		}
		
		// *this <= Tother
		template <typename Tother>
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& (Tflags & ENABLE_ORDERING), bool>::type
		operator<=(Tother&& other) const{
			return value <= other;
		}
		
		// *this > basic_number
		template <typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_ORDERING), bool>::type
		operator>(Tother&& other) const{
			return value > other.get_value();
		}
		
		// *this > Tother
		template <typename Tother>
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& (Tflags & ENABLE_ORDERING), bool>::type
		operator>(Tother&& other) const{
			return value > other;
		}
		
		
		// *this >= basic_number
		template <typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_ORDERING), bool>::type
		operator>=(Tother&& other) const{
			return value >= other.get_value();
		}
		
		// *this >= Tother
		template <typename Tother>
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& (Tflags & ENABLE_ORDERING), bool>::type
		operator>=(Tother&& other) const{
			return value >= other;
		}
		
		basic_number& operator++(){
			static_assert( !(Tflags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(Tflags & ENABLE_INC_DEC,
					"increment not enabled for this number-type");
			++value;
			return *this;
		}
		
		basic_number operator++(int){
			static_assert( !(Tflags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(Tflags & ENABLE_INC_DEC,
					"increment not enabled for this number-type");
			return basic_number(value++);
		}
		
		basic_number& operator--(){
			static_assert( !(Tflags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(Tflags & ENABLE_INC_DEC,
					"increment not enabled for this number-type");
			--value;
			return *this;
		}
		
		basic_number operator--(int){
			static_assert( !(Tflags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(Tflags & ENABLE_INC_DEC,
					"increment not enabled for this number-type");
			return basic_number(value--);
		}
		
		
		// *this + basic_number
		template<typename Tother>
		auto operator+(Tother&& other) const ->
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_SPECIFIC_PLUS_MINUS), 
			typename return_type<
				basic_number<
					decltype(value + other.get_value()), Tid, Tflags,
					Tbase>, 
				basic_number>::type>::type
		{
			using return_type_base = typename return_type<decltype(value + other.get_value()), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value + other.get_value())
				};
		}
		
		// *this + Tother
		template<typename Tother>
		auto operator+(Tother&& other) const ->
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& (Tflags & ENABLE_GENERAL_PLUS_MINUS), 
			typename return_type< 
				basic_number<decltype(value + other), Tid, Tflags, Tbase>, 
				basic_number>::type >::type
		{
			using return_type_base = typename return_type<decltype(value - other), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value - other)
			};
		}
		
		// *this += basic_number
		template<typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& !(Tflags & DISABLE_MUTABILITY)
			&& (Tflags & ENABLE_SPECIFIC_PLUS_MINUS), basic_number&>::type
		operator+=(Tother&& other){
			value += other.get_value();
			return *this;
		}
		
		// *this += Tother
		template<typename Tother>
		typename std::enable_if< !is_equivalent_basic_number<Tother>::value
			&& !(Tflags & DISABLE_MUTABILITY)
			&& (Tflags & ENABLE_GENERAL_PLUS_MINUS), basic_number&>::type
		operator+=(const Tother& other){
			value += other;
			return *this;
		}
		
		// *this - basic_number
		template<typename Tother>
		auto operator-(Tother&& other) const ->
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_SPECIFIC_PLUS_MINUS), 
			typename return_type<
				basic_number<decltype(value - other.get_value()), Tid, Tflags, Tbase>, 
				basic_number>::type>::type
		{
			using return_type_base = typename return_type<decltype(value - other.get_value()), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value - other.get_value())
				};
		}
		
		// *this - Tother
		template<typename Tother>
		auto operator-(Tother&& other) const ->
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& (Tflags & ENABLE_GENERAL_PLUS_MINUS), 
			typename return_type< 
				basic_number<decltype(value - other), Tid, Tflags, Tbase>, 
				basic_number>::type >::type
		{
			using return_type_base = typename return_type<decltype(value - other), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value - other)
				};
		}
		
		// *this -= basic_number
		template<typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& !(Tflags & DISABLE_MUTABILITY)
			&& (Tflags & ENABLE_SPECIFIC_PLUS_MINUS), basic_number&>::type
		operator-=(Tother&& other){
			value -= other.get_value();
			return *this;
		}
		
		// *this -= Tother
		template<typename Tother>
		typename std::enable_if< !is_equivalent_basic_number<Tother>::value
			&& !(Tflags & DISABLE_MUTABILITY)
			&& (Tflags & ENABLE_GENERAL_PLUS_MINUS), basic_number&>::type
		operator-=(const Tother& other){
			value -= other;
			return *this;
		}
		
		// *this * basic_number
		template<typename Tother>
		auto operator*(Tother&& other) const ->
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_SPECIFIC_MULTIPLICATION),
			typename return_type<
				basic_number<decltype(value - other.get_value()), Tid, Tflags, Tbase>, 
				basic_number>::type>::type
		{
			using return_type_base = typename return_type<decltype(value * other.get_value()), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value * other.get_value())
				};
		}
		
		// *this * Tother
		template<typename Tother>
		auto operator*(Tother&& other) const ->
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& ((Tflags & ENABLE_GENERAL_MULTIPLICATION)
				|| (std::is_floating_point<typename std::remove_reference<Tother>::type>::value 
					&& (Tflags & ENABLE_FLOAT_MULTIPLICATION))
				|| (std::is_integral<typename std::remove_reference<Tother>::type>::value 
					&& (Tflags & ENABLE_INTEGER_MULTIPLICATION))
			), typename return_type< 
				basic_number<decltype(value * other), Tid, Tflags, Tbase>, 
				basic_number>::type >::type
		{
			using return_type_base = typename return_type<decltype(value * other), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value * other)
				};
		}
		
		// *this *= basic_number
		template<typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& !(Tflags & DISABLE_MUTABILITY)
			&& (Tflags & ENABLE_SPECIFIC_MULTIPLICATION), basic_number&>::type
		operator*=(Tother&& other){
			value *= other.get_value();
			return *this;
		}
		
		// *this *= Tother
		template<typename Tother>
		typename std::enable_if< !is_equivalent_basic_number<Tother>::value
			&& !(Tflags & DISABLE_MUTABILITY)
			&& ((Tflags & ENABLE_GENERAL_MULTIPLICATION)
				|| (std::is_floating_point<typename std::remove_reference<Tother>::type>::value 
					&& Tflags & ENABLE_FLOAT_MULTIPLICATION)
				|| (std::is_integral<typename std::remove_reference<Tother>::type>::value 
					&& Tflags & ENABLE_INTEGER_MULTIPLICATION)
			), basic_number&>::type
		operator*=(Tother&& other){
			value *= other;
			return *this;
		}
		
		
		// *this / basic_number
		template<typename Tother>
		auto operator/(Tother&& other) const ->
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& (Tflags & ENABLE_SPECIFIC_DIVISION),
			typename return_type<
				basic_number<decltype(value / other.get_value()), Tid, Tflags, Tbase>, 
				basic_number>::type>::type
		{
			using return_type_base = typename return_type<decltype(value / other.get_value()), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value / other.get_value())
				};
		}
		
		// *this / Tother
		template<typename Tother>
		auto operator/(Tother&& other) const ->
		typename std::enable_if< !(is_equivalent_basic_number<Tother>::value)
			&& ((Tflags & ENABLE_GENERAL_DIVISION)
				|| (std::is_floating_point<typename std::remove_reference<Tother>::type>::value 
					&& (Tflags & ENABLE_FLOAT_DIVISION))
				|| (std::is_integral<typename std::remove_reference<Tother>::type>::value 
					&& (Tflags & ENABLE_INTEGER_DIVISION))
			), typename return_type< 
				basic_number<decltype(value / other), Tid, Tflags, Tbase>, 
				basic_number>::type >::type
		{
			using return_type_base = typename return_type<decltype(value / other), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value / other)
				};
		}
		
		// *this /= basic_number
		template<typename Tother>
		typename std::enable_if< is_equivalent_basic_number<Tother>::value
			&& !(Tflags & DISABLE_MUTABILITY)
			&& (Tflags & ENABLE_SPECIFIC_DIVISION), basic_number&>::type
		operator/=(Tother&& other){
			value /= other.get_value();
			return *this;
		}
		
		// *this /= Tother
		template<typename Tother>
		typename std::enable_if< !is_equivalent_basic_number<Tother>::value
			&& !(Tflags & DISABLE_MUTABILITY)
			&& ((Tflags & ENABLE_GENERAL_DIVISION)
				|| (std::is_floating_point<typename std::remove_reference<Tother>::type>::value 
					&& Tflags & ENABLE_FLOAT_DIVISION)
				|| (std::is_integral<typename std::remove_reference<Tother>::type>::value 
					&& Tflags & ENABLE_INTEGER_DIVISION)
			), basic_number&>::type
		operator/=(Tother&& other){
			value /= other;
			return *this;
		}
		
		// *this % basic_number
		template<typename Tother>
		typename std::enable_if<is_equivalent_basic_number<Tother>::value
			&& std::is_integral<T>::value
			&& (Tflags & ENABLE_SPECIFIC_MODULO), basic_number>::type
		operator%(Tother&& other) const{
			return basic_number(value % other.get_value());
		}
		
		// *this %= basic_number
		template<typename Tother>
		typename std::enable_if<is_equivalent_basic_number<Tother>::value
			&& !(Tflags & DISABLE_MUTABILITY)
			&& std::is_integral<T>::value
			&& (Tflags & ENABLE_SPECIFIC_MODULO), basic_number&>::type
		operator%=(Tother&& other){
			value %= other.get_value();
			return *this;
		}
		
		// *this % Tother
		template<typename Tother>
		typename std::enable_if<!(is_equivalent_basic_number<Tother>::value)
			&& std::is_integral<T>::value
			&& (Tflags & ENABLE_MODULO), basic_number>::type
		operator%(Tother&& other) const{
			return basic_number(value % other);
		}
		
		// *this %= Tother
		template<typename Tother>
		typename std::enable_if<!(is_equivalent_basic_number<Tother>::value)
			&& !(Tflags & DISABLE_MUTABILITY)
			&& std::is_integral<T>::value
			&& (Tflags & ENABLE_MODULO), basic_number>::type
		operator%=(Tother&& other){
			value %= other;
			return *this;
		}
		
		///////////////////////////////////////////////
		
		/**
		 * @brief Provides reading access to the underlying value.
		 * @return the value of the underlying variable.
		 */
		T get_value() const{ 
			return value; 
		}
		
	protected:
		
		/**
		 * @brief Sets the value of 'this'. 
		 * @note This method is mainly provided for use by Tbase.
		 * @param value the new value
		 */
		void set_value(T value){
			this->value = value;
		}
};

// these templates check, whether a type is an instance of basic_number:
template <typename T>
struct is_basic_number{
	enum {
		value = false
	};
};
template <typename T, int Tid, flag_t Tflags, template<typename> class Tbase>
struct is_basic_number<basic_number<T, Tid, Tflags, Tbase>>{
	enum {
		value = true
	};
};

template<typename Tother, class T, int Tid, flag_t Tflags, template<typename> class Tbase>
auto operator+(const Tother& other, const basic_number<T, Tid, Tflags, Tbase>& value)
	-> typename std::enable_if<!is_basic_number<Tother>::value, decltype(value.operator+(other))>::type {
	return {value+other};
}

template<typename Tother, class T, int Tid, flag_t Tflags, template<typename> class Tbase>
auto operator*(const Tother& factor, const basic_number<T, Tid, Tflags, Tbase>& value)
	-> typename std::enable_if<!is_basic_number<Tother>::value, decltype(value.operator*(factor))>::type {
	return {value*factor};
}


template<typename Tchar, typename T, int Tid, flag_t Tflags, template<typename> class Tbase>
auto operator << (::std::basic_ostream<Tchar>& stream, 
		const type_builder::basic_number<T, Tid, Tflags, Tbase> number)
	-> typename std::enable_if<!Tbase<T>::USE_DEFAULT_STREAM_OUT, ::std::basic_ostream<Tchar>&>::type 
{
	stream << Tbase<T>::template format<Tchar>(number.get_value());
	return stream;
}

template<typename Tchar, typename T, int Tid, flag_t Tflags, template<typename> class Tbase>
auto operator << (::std::basic_ostream<Tchar>& stream, 
		const type_builder::basic_number<T, Tid, Tflags, Tbase> number)
	-> typename std::enable_if<Tbase<T>::USE_DEFAULT_STREAM_OUT, ::std::basic_ostream<Tchar>&>::type 
{
	stream << number.get_value();
	return stream;
}


template<typename Tchar, typename T, int Tid, flag_t Tflags, template<typename> class Tbase>
auto operator >> (::std::basic_istream<Tchar>& stream, type_builder::basic_number<T, Tid, Tflags, Tbase> number)
	-> typename std::enable_if<!Tbase<T>::USE_DEFAULT_STREAM_IN, ::std::basic_istream<Tchar>&>::type 
{
	type_builder::basic_number<T,Tid, Tflags, Tbase>(
			Tbase<T>::read_istream(stream), number);
	return stream;
}


template<typename Tchar, typename T, int Tid, flag_t Tflags, template<typename> class Tbase>
auto operator >> (::std::basic_istream<Tchar>& stream, type_builder::basic_number<T, Tid, Tflags, Tbase> number)
	-> typename std::enable_if<Tbase<T>::USE_DEFAULT_STREAM_IN, ::std::basic_istream<Tchar>&>::type 
{
	T tmp;
	stream >> tmp;
	number = type_builder::basic_number<T,Tid, Tflags, Tbase>{tmp};
	return stream;
}

} //namespace type_builder

#endif
