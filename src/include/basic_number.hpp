#include <cstdint>
#include <ostream>
#include <istream>
#include <string>
#include <type_traits>

namespace type_builder{

enum: uint64_t{
	ENABLE_GENERAL_CONSTRUCTION = 1,
	ENABLE_DEFAULT_CONSTRUCTION = 1 << 1,
	ENABLE_LATE_ASSIGNEMENT = 1 << 2,
	ENABLE_SPECIFIC_EQUALITY_CHECK = 1 << 3,
	ENABLE_SPECIFIC_ORDERING = ENABLE_SPECIFIC_EQUALITY_CHECK | 1 << 4,
	ENABLE_EQUALITY_CHECK = ENABLE_SPECIFIC_EQUALITY_CHECK | 1 << 5,
	ENABLE_ORDERING = ENABLE_SPECIFIC_ORDERING | ENABLE_EQUALITY_CHECK | 1 << 6,
	
	ENABLE_INC_DEC = 1 << 7,
	
	ENABLE_SPECIFIC_PLUS_MINUS = 1 << 8,
	ENABLE_SPECIFIC_MULTIPLICATION = 1 << 9,
	ENABLE_SPECIFIC_DIVISION = 1 << 9,
	
	ENABLE_INTEGER_MULTIPLICATION = 1 << 10,
	ENABLE_INTEGER_DIVISION = 1 << 11,
	
	ENABLE_FLOAT_MULTIPLICATION = ENABLE_INTEGER_MULTIPLICATION | 1 << 12,
	ENABLE_FLOAT_DIVISION = ENABLE_INTEGER_DIVISION | 1 << 13,
	
	ENABLE_INTEGER_MULT_DIV = ENABLE_INTEGER_MULTIPLICATION | ENABLE_INTEGER_DIVISION,
	ENABLE_FLOAT_MULT_DIV = ENABLE_FLOAT_MULTIPLICATION | ENABLE_FLOAT_DIVISION,
	
	ENABLE_GENERAL_PLUS_MINUS = 1 << 20,
	ENABLE_GENERAL_MULTIPLICATION = 1 << 21,
	ENABLE_GENERAL_DIVISION = 1 << 22,
	
	ENABLE_SPECIFIC_MODULO = 1 << 26,
	ENABLE_MODULO = ENABLE_SPECIFIC_MODULO | 1 << 27,
	
	DISABLE_CONSTRUCTION = 1 << 30,
	DISABLE_MUTABILITY = 1L << 31,
	
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
		| ENABLE_ALL_MATH
};

template<typename T, int ID, uint64_t flags = DEFAULT_SETTINGS>
class basic_number{
	public:
		
		basic_number() : value(0) {
			static_assert( !(flags & DISABLE_CONSTRUCTION), 
					"construction of this type is disabled");
			static_assert( flags & ENABLE_DEFAULT_CONSTRUCTION,
					"default constructor not enabled for this type");
		}
		
		explicit basic_number(T value): value(value){
			static_assert( !(flags & DISABLE_CONSTRUCTION), 
					"construction of this type is disabled");
		}
		
		basic_number(const basic_number& other): value(other.value){
			static_assert( !(flags & DISABLE_CONSTRUCTION), 
					"construction of this type is disabled");
		}
		
		template<typename Tother>
		explicit basic_number(Tother value ): value(value){ 
			static_assert( !(flags & DISABLE_CONSTRUCTION), 
					"construction of this type is disabled");
			static_assert(flags & ENABLE_GENERAL_CONSTRUCTION, 
					"invalid construction of number type"); 
		}
		
		inline basic_number& operator=(const basic_number& other) {
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			//this shouldn't be a problem in case of self-assignement:
			value = other.value;
			return *this;
		}
		
		inline basic_number& operator=(T value){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_LATE_ASSIGNEMENT,
					"invalid assignement to numer type");
			this->value = static_cast<T>(value);
			return *this;
		}
		
		template<typename Tother>
		inline basic_number& operator=(Tother value){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_LATE_ASSIGNEMENT &&
					flags & ENABLE_GENERAL_CONSTRUCTION,
					"invalid assignement to numer type");
			this->value = static_cast<T>(value);
			return *this;
		}
		
		inline bool operator==(const basic_number& other) const{
			static_assert(flags & ENABLE_EQUALITY_CHECK,
					"invalud comparission of number type");
			return value == other.value;
		}
		
		inline bool operator!=(const basic_number& other) const{
			static_assert(flags & ENABLE_EQUALITY_CHECK,
					"invalud comparission of number type");
			return value != other.value;
		}
		
		inline bool operator<(const basic_number& other) const{
			static_assert(flags & ENABLE_ORDERING,
					"invalud comparission of number type");
			return value < other.value;
		}
		
		inline bool operator<=(const basic_number& other) const{
			static_assert(flags & ENABLE_ORDERING,
					"invalud comparission of number type");
			return value <= other.value;
		}
		
		inline bool operator>(const basic_number& other) const{
			static_assert(flags & ENABLE_ORDERING,
					"invalud comparission of number type");
			return value > other.value;
		}
		
		inline bool operator>=(const basic_number& other) const{
			static_assert(flags & ENABLE_ORDERING,
					"invalud comparission of number type");
			return value >= other.value;
		}
		
		template<typename Tother>
		inline bool operator==(const Tother& other) const{
			static_assert(flags & ENABLE_EQUALITY_CHECK,
					"invalud comparission of number type");
			return value == other.value;
		}
		
		template<typename Tother>
		inline bool operator!=(const Tother& other) const{
			static_assert(flags & ENABLE_EQUALITY_CHECK,
					"invalud comparission of number type");
			return value != other.value;
		}
		
		template<typename Tother>
		inline bool operator<(const Tother& other) const{
			static_assert(flags & ENABLE_ORDERING,
					"invalud comparission of number type");
			return value < other.value;
		}
		
		template<typename Tother>
		inline bool operator<=(const Tother& other) const{
			static_assert(flags & ENABLE_ORDERING,
					"invalud comparission of number type");
			return value <= other.value;
		}
		
		template<typename Tother>
		inline bool operator>(const Tother& other) const{
			static_assert(flags & ENABLE_ORDERING,
					"invalud comparission of number type");
			return value > other.value;
		}
		
		template<typename Tother>
		inline bool operator>=(const Tother& other) const{
			static_assert(flags & ENABLE_ORDERING,
					"invalud comparission of number type");
			return value >= other.value;
		}
		
		inline basic_number& operator++(){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_INC_DEC,
					"increment not enabled for this number-type");
			++value;
			return *this;
		}
		
		inline basic_number operator++(int){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_INC_DEC,
					"increment not enabled for this number-type");
			return basic_number(value++);
		}
		
		inline basic_number& operator--(){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_INC_DEC,
					"increment not enabled for this number-type");
			--value;
			return *this;
		}
		
		inline basic_number operator--(int){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_INC_DEC,
					"increment not enabled for this number-type");
			return basic_number(value--);
		}
		
		inline basic_number operator+(const basic_number& other) const{
			static_assert(flags & ENABLE_SPECIFIC_PLUS_MINUS,
					"addition not enabled for this type");
			return basic_number(value + other.value);
		}
		
		inline basic_number& operator+=(const basic_number& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_SPECIFIC_PLUS_MINUS,
					"addition not enabled for this type");
			value += other.value;
			return *this;
		}
		
		inline basic_number operator-(const basic_number& other) const{
			static_assert(flags & ENABLE_SPECIFIC_PLUS_MINUS,
					"subtraction not enabled for this type");
			return basic_number(value - other.value);
		}
		
		inline basic_number& operator-=(const basic_number& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_SPECIFIC_PLUS_MINUS,
					"subtraction not enabled for this type");
			value -= other.value;
			return *this;
		}
		
		inline basic_number operator*(const basic_number& other) const{
			static_assert(flags & ENABLE_SPECIFIC_MULTIPLICATION,
					"multiplication not enabled for this type");
			return basic_number(value * other.value);
		}
		
		inline basic_number operator*=(const basic_number& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_SPECIFIC_MULTIPLICATION,
					"multiplication not enabled for this type");
			value *= other.value;
			return *this;
		}
		
		inline basic_number operator/(const basic_number& other) const{
			static_assert(flags & ENABLE_SPECIFIC_DIVISION,
					"division not enabled for this type");
			return basic_number(value / other.value);
		}
		
		inline basic_number operator/=(const basic_number& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_SPECIFIC_DIVISION,
					"division not enabled for this type");
			value /= other.value;
			return *this;
		}
		
		template<typename Tother>
		inline basic_number operator+(const Tother& other) const{
			static_assert(flags & ENABLE_GENERAL_PLUS_MINUS,
					"addition not enabled for this type");
			return basic_number(static_cast<T>(value + other.value));
		}
		
		template<typename Tother>
		inline basic_number& operator+=(const Tother& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_GENERAL_PLUS_MINUS,
					"addition not enabled for this type");
			value += other;
			return *this;
		}
		
		template<typename Tother>
		inline basic_number operator-(const Tother& other) const{
			static_assert(flags & ENABLE_GENERAL_PLUS_MINUS,
					"subtraction not enabled for this type");
			return basic_number(static_cast<T>(value - other.value));
		}
		
		template<typename Tother>
		inline basic_number& operator-=(const Tother& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_GENERAL_PLUS_MINUS,
					"subtraction not enabled for this type");
			value -= other;
			return *this;
		}
		
		template<typename Tother>
		inline basic_number operator*(const Tother& other) const{
			static_assert(flags & ENABLE_GENERAL_MULTIPLICATION
					|| (std::is_floating_point<Tother>:: value &&
						flags & ENABLE_FLOAT_MULTIPLICATION)
					|| (std::is_integral<Tother>::value &&
						flags & ENABLE_INTEGER_MULTIPLICATION),
					"multiplication not enabled for this type");
			return basic_number(static_cast<T>(value * other.value));
		}
		
		template<typename Tother>
		inline basic_number& operator*=(const Tother& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert( (flags & ENABLE_GENERAL_MULTIPLICATION)
					|| (std::is_floating_point<Tother>::value &&
						(flags & ENABLE_FLOAT_MULTIPLICATION))
					|| (std::is_integral<Tother>::value &&
						flags & ENABLE_INTEGER_MULTIPLICATION),
					"multiplication not enabled for this type");
			value *= other;
			return *this;
		}
		
		template<typename Tother>
		inline basic_number operator/(const Tother& other) const{
			static_assert(flags & ENABLE_GENERAL_DIVISION 
					|| (std::is_floating_point<Tother>:: value &&
						flags & ENABLE_FLOAT_DIVISION)
					|| (std::is_integral<Tother>::value &&
						flags & ENABLE_INTEGER_DIVISION),
					"division not enabled for this type");
			return basic_number(static_cast<T>(value / other.value));
		}
		
		template<typename Tother>
		inline basic_number operator/=(const Tother& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(flags & ENABLE_GENERAL_DIVISION 
					|| (std::is_floating_point<Tother>:: value &&
						flags & ENABLE_FLOAT_DIVISION)
					|| (std::is_integral<Tother>::value &&
						flags & ENABLE_INTEGER_DIVISION),
					"division not enabled for this type");
			value /= other;
			return *this;
		}
		
		inline basic_number operator%(const basic_number& other) const{
			static_assert(std::is_integral<T>::value,
					"cannot calculate the modulo of a non-integer type");
			static_assert(flags & ENABLE_SPECIFIC_MODULO,
					"calculating the modulo of this numbertype is not"
					"enabled");
			return basic_number(value % other.value);
		}
		
		inline basic_number& operator%=(const basic_number& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(std::is_integral<T>::value,
					"cannot calculate the modulo of a non-integer type");
			static_assert(flags & ENABLE_SPECIFIC_MODULO,
					"calculating the modulo of this numbertype is not"
					"enabled");
			value %= other.value;
			return *this;
		}
		
		template<typename Tother>
		inline basic_number operator%(const Tother& other) const{
			static_assert(std::is_integral<T>::value,
					"cannot calculate the modulo of a non-integer type");
			static_assert(flags & ENABLE_MODULO,
					"calculating the modulo of this numbertype is not"
					"enabled");
			return basic_number(value % other.value);
		}
		
		template<typename Tother>
		inline basic_number& operator%=(const Tother& other){
			static_assert( !(flags & DISABLE_MUTABILITY),
					"You cannot change the value of an instance "
					"of an immutable type");
			static_assert(std::is_integral<T>::value,
					"cannot calculate the modulo of a non-integer type");
			static_assert(flags & ENABLE_MODULO,
					"calculating the modulo of this numbertype is not"
					"enabled");
			value %= other.value;
			return *this;
		}
		///////////////////////////////////////////////
		
		inline T get_value() const{ 
			return value; 
		}
		
		enum: uint64_t{
			FLAGS = flags
		};
		
		enum: int {
			TYPE_ID = ID
		};
		
	private:
		T value;
};

} //namespace type_builder

template<typename Tchar, typename T, int ID, uint64_t flags>
::std::basic_ostream<Tchar>& operator << (::std::basic_ostream<Tchar>& stream,
		const type_builder::basic_number<T, ID, flags>& number){
	stream << number.get_value();
	return stream;
}

template<typename Tchar, typename T, int ID, uint64_t flags>
::std::basic_istream<Tchar>& operator >> (::std::basic_istream<Tchar>& stream,
		type_builder::basic_number<T, ID, flags>& number){
	T tmp;
	stream >> tmp;
	number = type_builder::basic_number<T,ID, flags>(tmp);
	return stream;
}


