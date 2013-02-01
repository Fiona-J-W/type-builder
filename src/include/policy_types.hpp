#ifndef POLICY_TYPES_HPP
#define POLICY_TYPES_HPP

namespace type_builder{


/**
 * @brief This type indicates that some other type has no unit.
 */
class unit_free{};

/**
 * @brief This class is the default base class for number_types that don't need a base.
 *
 * NEVER use this class to refer to a type_builder::basic_number as it has no virtual
 * destructor and will blow away all type-safety you would gain otherwise.
 */
class empty_base{};

/**
 * @brief This class is an alternative to empty_base that provides a virtual destructor to enable further inheritance.
 * 
 * Note that this might still not be enough for many design-patterns; provide your own base, if you need it.
 */
class virtual_empty_base{
	public:
		virtual ~virtual_empty_base() = 0;
};

}

#endif
