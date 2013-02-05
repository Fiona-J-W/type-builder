#ifndef POLICY_TYPES_HPP
#define POLICY_TYPES_HPP

namespace type_builder{

/**
 * @brief This class is the default base class for number_types that don't need a base.
 *
 * If default-initializiation is enabled, all numbers will be initialized with 0.
 * 
 * NEVER use this class to refer to a type_builder::basic_number as it has no virtual
 * destructor and will blow away all type-safety you would gain otherwise.
 */
template<typename T>
struct empty_base{
	static constexpr T default_value(){
		return 0;
	}
};

/**
 * @brief This class is an alternative to empty_base that provides a virtual destructor to enable further inheritance.
 * 
 * Note that this might still not be enough for many design-patterns; provide your own base, if you need it.
 */
template <typename T>
struct virtual_empty_base{
	static constexpr T default_value(){
		return 0;
	}
	public:
		virtual ~virtual_empty_base() = 0;
};

}

#endif
