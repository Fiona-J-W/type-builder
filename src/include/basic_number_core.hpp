#ifndef BASIC_NUMBER_CORE_HPP
#define BASIC_NUMBER_CORE_HPP

#include <utility>
#include <cstdint>
#include <type_traits>

#include "basic_number_flags.hpp"
#include "policy_types.hpp"

namespace type_builder{

/**
 * @brief A template for a number-class.
 */
// note that the operators are not documented with doxygen, 
// because they behave pretty much as you would expect anyway.
template<
	typename T,
	class Tid,
	flag_t Tflags = DEFAULT_SETTINGS,
	template<typename, class> class Tbase = empty_base>
class basic_number: protected Tbase<T, Tid> {
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
	template<typename Targ> 
	static constexpr bool is_this(){ 
		return ( std::is_same<const basic_number&, const Targ&>::value 
			|| std::is_same<basic_number&, Targ&>::value);
	}
	
	// the template-parameters in the following function
	
	/**
	 * @brief Checks whether a flag is set.
	 * @param flag the flag to be checked
	 * @return true if Tflag is set, false otherwise
	 */
	template<typename = void>
	static constexpr bool flag_set(flag_t flag){
		return (Tflags & flag) != 0 ? true : false;
	}
	
	/**
	 * @brief Checks whether a flag is unset.
	 * @param flag the flag to be checked
	 * @return true if Tflag is unset, false otherwise
	 */
	template<typename = void>
	static constexpr bool flag_unset(flag_t flag){
		return !flag_set(flag);
	}
	
	/**
	 * @brief Provides the correct returntype for some functions.
	 * @param T1 the return type if native typing is enabled
	 * @param T2 the return type if native typing is disabled
	 */
	template<typename T1, typename T2 = basic_number>
	struct return_type{
		using type = typename std::conditional<flag_set<void>(ENABLE_NATIVE_TYPING), T1, T2>::type;
	};
	
	template<typename Targ>
	struct _is_equivalent_basic_number : std::false_type {};
	
	template<typename Targ>
	struct _is_equivalent_basic_number<basic_number<Targ, Tid, Tflags, Tbase>&> :
		std::integral_constant<bool, flag_set(ENABLE_NATIVE_TYPING) ?
			true : is_this<basic_number<Targ, Tid, Tflags, Tbase>>() > {};
	
	/**
	 * @brief Checks whether a type is semanticly equivalent to *this.
	 * @param Targ the type that will be checked.
	 * @return true if it is equivalent, false otherwise
	 */
	template<typename Targ>
	static constexpr bool is_equivalent_basic_number(){
		return _is_equivalent_basic_number<typename std::remove_const<
				typename std::remove_reference<Targ>::type
			>::type&>{};
	}
	
	public:
		
		// constructors and methods that will be created anyway:
		//
		// we have to provide them allways, in order to prevent the creation of defaults 
		// if we do not want them:
		
		basic_number() : value(Tbase<T, Tid>::default_value()) {
			static_assert(flag_set<void>(ENABLE_DEFAULT_CONSTRUCTION),
					"default constructor not enabled for this type");
		}
		
		basic_number(const basic_number& rhs) = default;
		
		basic_number(basic_number&& rhs) = default;
		
		basic_number& operator=(const basic_number& rhs){
			static_assert(flag_unset<void>(DISABLE_MUTABILITY),
					"invalid assignment to immutable type.");
			value = rhs.value;
			return *this;
		}
		
		basic_number& operator=(basic_number&& rhs){
			static_assert(flag_unset<void>(DISABLE_MUTABILITY),
					"invalid assignment to immutable type.");
			value = std::move(rhs.value);
			return *this;
		}
		
		~basic_number() = default;
		
		// custom constructors:
		
		// basic_number(basic_number)
		template<
			typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<std::is_same<Tother,basic_number>::value>::type
		>
		basic_number(const Tother& other): value(other.get_value()){}
		
		// basic_number(T)
		template<
			typename Tother,
			typename = typename std::enable_if<std::is_same<Tother&, T&>::value>::type
		>
		explicit basic_number(const Tother& value): value(value){}
		
		// basic_number(Tother)
		template<
			typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<!std::is_same<Tother&, T&>::value>::type,
			typename = typename std::enable_if<flag_unset<Tother>(ENABLE_GENERAL_CONSTRUCTION)>::type
		>
		explicit basic_number(const Tother& value): value(value){}
		
		// methods:
		
		// *this = basic_number
		template<
			typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<std::is_same<Tother,basic_number>::value>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type
		>
		basic_number& operator=(const Tother& other){
			this->value = other.value;
			return *this;
		}
		
		// *this = Tother
		template<
			typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_LATE_ASSIGNEMENT)>::type,
			typename = typename std::enable_if<
					std::is_same<T&, Tother&>::value
					|| flag_set<Tother>(ENABLE_GENERAL_CONSTRUCTION)
				>::type,
			typename = void
		>
		basic_number& operator=(const Tother& value){
			this->value = value;
			return *this;
		}
		
		// comparision-methods:
		
		// *this == basic_number
		template <typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_EQUALITY_CHECK)>::type
		>
		bool operator==(const Tother& other) const{
			return value == other.get_value();
		}
		
		// *this == Tother
		template <typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_EQUALITY_CHECK_O_)>::type,
			typename = void
		>
		bool operator==(const Tother& other) const{
			return value == other;
		}
		
		// *this != basic_number
		template <typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_EQUALITY_CHECK)>::type
		>
		bool operator!=(const Tother& other) const{
			return value != other.get_value();
		}
		
		// *this != Tother
		template <typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_EQUALITY_CHECK_O_)>::type,
			typename = void
		>
		bool operator!=(const Tother& other) const{
			return value != other;
		}
		
		// *this < basic_number
		template <typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_ORDERING_O_)>::type
		>
		bool operator<(const Tother& other) const{
			return value < other.get_value();
		}
		
		// *this < Tother
		template <typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_ORDERING_O_)>::type,
			typename = void
		>
		bool operator<(const Tother& other) const{
			return value < other;
		}
		
		// *this <= basic_number
		template <typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_ORDERING_O_)>::type
		>
		bool operator<=(const Tother& other) const{
			return value <= other.get_value();
		}
		
		// *this <= Tother
		template <typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_ORDERING_O_)>::type,
			typename = void
		>
		bool operator<=(const Tother& other) const{
			return value <= other;
		}
		
		// *this > basic_number
		template <typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_ORDERING_O_)>::type
		>
		bool operator>(const Tother& other) const{
			return value > other.get_value();
		}
		
		// *this > Tother
		template <typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_ORDERING_O_)>::type,
			typename = void
		>
		bool operator>(const Tother& other) const{
			return value > other;
		}
		
		
		// *this >= basic_number
		template <typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_ORDERING_O_)>::type
		>
		bool operator>=(const Tother& other) const{
			return value >= other.get_value();
		}
		
		// *this >= Tother
		template <typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_ORDERING_O_)>::type,
			typename = void
		>
		bool operator>=(const Tother& other) const{
			return value >= other;
		}
		
		basic_number& operator++(){
			static_assert(flag_unset<void>(DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flag_set<void>(ENABLE_INC_DEC),
					"increment not enabled for this number-type");
			++value;
			return *this;
		}
		
		basic_number operator++(int){
			static_assert(flag_unset<void>(DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flag_set<void>(ENABLE_INC_DEC),
					"increment not enabled for this number-type");
			return basic_number(value++);
		}
		
		basic_number& operator--(){
			static_assert(flag_unset<void>(DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flag_set<void>(ENABLE_INC_DEC),
					"increment not enabled for this number-type");
			--value;
			return *this;
		}
		
		basic_number operator--(int){
			static_assert(flag_unset<void>(DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flag_set<void>(ENABLE_INC_DEC),
					"increment not enabled for this number-type");
			return basic_number(value--);
		}
		
		// unary -
		basic_number operator-(){
			static_assert(flag_set<void>(ENABLE_SPECIFIC_PLUS_MINUS), 
					"unary minus must be enabled by ENABLE_SPECIFIC_PLUS_MINUS");
			return basic_number{ -value };
		}
		
		
		// *this + basic_number
		template<
			typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_PLUS_MINUS)>::type
		>
		auto operator+(const Tother& other) const ->
		typename return_type<basic_number<decltype(value + other.get_value()), Tid, Tflags,Tbase>>::type
		{
			using return_type_base = typename return_type<decltype(value + other.get_value()), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value + other.get_value())
				};
		}
		
		// *this + Tother
		template<
			typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_GENERAL_PLUS_MINUS)>::type,
			typename = void
		>
		auto operator+(const Tother& other) const ->
			typename return_type<basic_number<decltype(value + other), Tid, Tflags, Tbase>>::type
		{
			using return_type_base = typename return_type<decltype(value + other), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value + other)
			};
		}
		
		// *this += basic_number
		template<
			typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_PLUS_MINUS)>::type
		>
		basic_number& operator+=(const Tother& other){
			value += other.get_value();
			return *this;
		}
		
		// *this += Tother
		template<
			typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_GENERAL_PLUS_MINUS)>::type,
			typename = void
		>
		basic_number& operator+=(const Tother& other){
			value += other;
			return *this;
		}
		
		// *this - basic_number
		template<
			typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_PLUS_MINUS)>::type
		>
		auto operator-(const Tother& other) const ->
		typename return_type<basic_number<decltype(value - other.get_value()), Tid, Tflags,Tbase>>::type
		{
			using return_type_base = typename return_type<decltype(value - other.get_value()), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value - other.get_value())
				};
		}
		
		// *this - Tother
		template<
			typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_GENERAL_PLUS_MINUS)>::type,
			typename = void
		>
		auto operator-(const Tother& other) const ->
			typename return_type<basic_number<decltype(value - other), Tid, Tflags, Tbase>>::type
		{
			using return_type_base = typename return_type<decltype(value - other), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value - other)
				};
		}
		
		// *this -= basic_number
		template<
			typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_PLUS_MINUS)>::type
		>
		basic_number& operator-=(const Tother& other){
			value -= other.get_value();
			return *this;
		}
		
		// *this -= Tother
		template<
			typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_GENERAL_PLUS_MINUS)>::type,
			typename = void
		>
		basic_number& operator-=(const Tother& other){
			value -= other;
			return *this;
		}
		
		// *this * basic_number
		template<typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_MULTIPLICATION)>::type
		>
		auto operator*(const Tother& other) const ->
		typename return_type<basic_number<decltype(value * other.get_value()), Tid, Tflags, Tbase>>::type
		{
			using return_type_base = typename return_type<decltype(value * other.get_value()), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value * other.get_value())
				};
		}
		
		// *this * Tother
		template<typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<
				flag_set<Tother>(ENABLE_GENERAL_MULTIPLICATION)
				|| (std::is_floating_point<typename std::remove_reference<Tother>::type>::value 
					&& flag_set<Tother>(ENABLE_FLOAT_MULTIPLICATION_O_))
				|| (std::is_integral<typename std::remove_reference<Tother>::type>::value 
					&& flag_set<Tother>(ENABLE_INTEGER_MULTIPLICATION))
				|| (flag_set<Tother>(ENABLE_BASE_MULTIPLICATION_O_)
					&& std::is_floating_point<typename std::remove_reference<Tother>::type>::value
					&& std::is_floating_point<T>::value)
			>::type,
			typename = void
		>
		auto operator*(const Tother& other) const ->
		typename return_type<basic_number<decltype(value * other), Tid, Tflags, Tbase>>::type
		{
			using return_type_base = typename return_type<decltype(value * other), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value * other)
				};
		}
		
		// *this *= basic_number
		template<
			typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_MULTIPLICATION)>::type
		>
		basic_number& operator*=(const Tother& other){
			value *= other.get_value();
			return *this;
		}
		
		// *this *= Tother
		template<
			typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<
				flag_set<Tother>(ENABLE_GENERAL_MULTIPLICATION)
				|| (std::is_floating_point<typename std::remove_reference<Tother>::type>::value 
					&& flag_set<Tother>(ENABLE_FLOAT_MULTIPLICATION_O_))
				|| (std::is_integral<typename std::remove_reference<Tother>::type>::value 
					&& flag_set<Tother>(ENABLE_INTEGER_MULTIPLICATION))
				|| (flag_set<Tother>(ENABLE_BASE_MULTIPLICATION_O_)
					&& std::is_floating_point<typename std::remove_reference<Tother>::type>::value
					&& std::is_floating_point<T>::value)
			>::type,
			typename = void
		>
		basic_number& operator*=(const Tother& other){
			value *= other;
			return *this;
		}
		
		
		// *this / basic_number
		template<typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_DIVISION)>::type
		>
		auto operator/(const Tother& other) const ->
		typename return_type<basic_number<decltype(value / other.get_value()), Tid, Tflags, Tbase>>::type
		{
			using return_type_base = typename return_type<decltype(value / other.get_value()), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value / other.get_value())
				};
		}
		
		// *this / Tother
		template<typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<
				flag_set<Tother>(ENABLE_GENERAL_DIVISION)
				|| (std::is_floating_point<typename std::remove_reference<Tother>::type>::value 
					&& flag_set<Tother>(ENABLE_FLOAT_DIVISION_O_))
				|| (std::is_integral<typename std::remove_reference<Tother>::type>::value 
					&& flag_set<Tother>(ENABLE_INTEGER_DIVISION))
				|| (flag_set<Tother>(ENABLE_BASE_DIVISION_O_)
					&& std::is_floating_point<typename std::remove_reference<Tother>::type>::value
					&& std::is_floating_point<T>::value)
			>::type,
			typename = void
		>
		auto operator/(const Tother& other) const ->
		typename return_type<basic_number<decltype(value / other), Tid, Tflags, Tbase>>::type
		{
			using return_type_base = typename return_type<decltype(value / other), T>::type;
			return basic_number<return_type_base, Tid, Tflags, Tbase>{
					static_cast<return_type_base>(value / other)
				};
		}
		
		// *this /= basic_number
		template<typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_DIVISION)>::type
		>
		basic_number& operator/=(const Tother& other){
			value /= other.get_value();
			return *this;
		}
		
		// *this /= Tother
		template<typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<
				flag_set<Tother>(ENABLE_GENERAL_DIVISION)
				|| (std::is_floating_point<typename std::remove_reference<Tother>::type>::value 
					&& flag_set<Tother>(ENABLE_FLOAT_DIVISION_O_))
				|| (std::is_integral<typename std::remove_reference<Tother>::type>::value 
					&& flag_set<Tother>(ENABLE_INTEGER_DIVISION))
				|| (flag_set<Tother>(ENABLE_BASE_DIVISION_O_)
					&& std::is_floating_point<typename std::remove_reference<Tother>::type>::value
					&& std::is_floating_point<T>::value)
			>::type,
			typename = void
		>
		basic_number& operator/=(const Tother& other){
			value /= other;
			return *this;
		}
		
		// *this % basic_number
		template<typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_MODULO)>::type
		>
		basic_number operator%(const Tother& other) const{
			return basic_number(value % other.get_value());
		}
		
		// *this % Tother
		template<typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_MODULO_O_)>::type,
			typename = void
		>
		basic_number operator%(const Tother& other) const{
			return basic_number(value % other);
		}
		
		// *this %= basic_number
		template<typename Tother,
			typename = typename std::enable_if<is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_SPECIFIC_MODULO)>::type
		>
		basic_number& operator%=(const Tother& other){
			value %= other.get_value();
			return *this;
		}
		
		// *this %= Tother
		template<typename Tother,
			typename = typename std::enable_if<!is_equivalent_basic_number<Tother>()>::type,
			typename = typename std::enable_if<flag_unset<Tother>(DISABLE_MUTABILITY)>::type,
			typename = typename std::enable_if<flag_set<Tother>(ENABLE_MODULO_O_)>::type,
			typename = void
		>
		basic_number& operator%=(const Tother& other){
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
		 * @param value the new value
		 */
		void set_value(T value){
			this->value = value;
		}
};

namespace impl{

template <typename T>
struct is_basic_number : std::false_type {};

template <typename T, class Tid, flag_t Tflags, template<typename, class> class Tbase>
struct is_basic_number<basic_number<T, Tid, Tflags, Tbase>> : std::true_type {};

}

template<typename T>
constexpr bool is_basic_number(){
	return impl::is_basic_number<T>::value;
}

// we asume * and + to be commutative, so:
template<
	typename Tlhs,
	typename Trhs,
	typename = typename std::enable_if<!is_basic_number<Tlhs>()>::type,
	typename = typename std::enable_if<is_basic_number<Trhs>()>::type
>
auto operator+(const Tlhs& lhs, const Trhs& rhs ) -> decltype(rhs+lhs) {
	return rhs+lhs;
}

template<
	typename Tlhs,
	typename Trhs,
	typename = typename std::enable_if<!is_basic_number<Tlhs>()>::type,
	typename = typename std::enable_if<is_basic_number<Trhs>()>::type
>
auto operator*(const Tlhs& lhs, const Trhs& rhs ) -> decltype(rhs*lhs) {
	return rhs*lhs;
}

} //namespace type_builder

#endif
