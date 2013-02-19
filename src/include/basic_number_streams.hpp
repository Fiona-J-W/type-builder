#ifndef BASIC_NUMBER_STREAM_HPP
#define BASIC_NUMBER_STREAM_HPP

#include <ostream>
#include <istream>

#include "basic_number_core.hpp"

namespace type_builder{


template<typename Tchar, typename T, class Tid, flag_t Tflags, template<typename, class> class Tbase>
auto operator << (::std::basic_ostream<Tchar>& stream, 
		const type_builder::basic_number<T, Tid, Tflags, Tbase> number)
	-> typename std::enable_if<!Tbase<T, Tid>::USE_DEFAULT_STREAM_OUT, ::std::basic_ostream<Tchar>&>::type 
{
	stream << Tbase<T, Tid>::template format<Tchar>(number.get_value());
	return stream;
}

template<typename Tchar, typename T, class Tid, flag_t Tflags, template<typename, class> class Tbase>
auto operator << (::std::basic_ostream<Tchar>& stream, 
		const type_builder::basic_number<T, Tid, Tflags, Tbase> number)
	-> typename std::enable_if<Tbase<T, Tid>::USE_DEFAULT_STREAM_OUT, ::std::basic_ostream<Tchar>&>::type 
{
	stream << number.get_value();
	return stream;
}


template<typename Tchar, typename T, class Tid, flag_t Tflags, template<typename, class> class Tbase>
auto operator >> (::std::basic_istream<Tchar>& stream, type_builder::basic_number<T, Tid, Tflags, Tbase>& number)
	-> typename std::enable_if<!Tbase<T, Tid>::USE_DEFAULT_STREAM_IN, ::std::basic_istream<Tchar>&>::type 
{
	number = type_builder::basic_number<T,Tid, Tflags, Tbase>(
			Tbase<T, Tid>::read_istream(stream));
	return stream;
}


template<typename Tchar, typename T, class Tid, flag_t Tflags, template<typename, class> class Tbase>
auto operator >> (::std::basic_istream<Tchar>& stream, type_builder::basic_number<T, Tid, Tflags, Tbase> number)
	-> typename std::enable_if<Tbase<T, Tid>::USE_DEFAULT_STREAM_IN, ::std::basic_istream<Tchar>&>::type 
{
	T tmp;
	stream >> tmp;
	number = type_builder::basic_number<T,Tid, Tflags, Tbase>{tmp};
	return stream;
}

} //namespace type_builder

#endif
