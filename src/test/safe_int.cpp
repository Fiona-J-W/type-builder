#include "../include/safe_int.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <type_traits>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

template<typename T>
using ui = type_builder::safe_int<T>;

using U08 = ui<std::uint8_t>;
using U16 = ui<std::uint16_t>;
using U32 = ui<std::uint32_t>;
using U64 = ui<std::uint64_t>;
using I08 = ui<std::int8_t>;
using I16 = ui<std::int16_t>;
using I32 = ui<std::int32_t>;
using I64 = ui<std::int64_t>;

using U = ui<unsigned int>;
using I = ui<int>;

#define ASSERT_THROW(expr) \
	do{ try{ expr; assert(false); } catch (std::exception& e){ assert(true); } }while(false)

#define ASSERT_THROW_EX(expr, exception) \
	do{ try{ expr; assert(false); } catch (exception& e){ assert(true); } }while(false)

#define ASSERT_NOTHROW(expr) \
	do{ try{ expr; assert(true);} catch(...){assert(false);} }while(false)

int main(){
	const U08
		u08_0 = 0,
		u08_1 = 1,
		u08_127 = 127,
		u08_128 = 128,
		u08_255 = 255;
	assert(u08_0 < u08_1);
	assert(u08_1 < u08_127);
	assert(u08_127 < u08_128);
	assert(u08_128 < u08_255);
	ASSERT_THROW(U08 var = 1000; var = var);
	ASSERT_THROW(U08 var = -1; var = var);

	const I08
		i08__128 = -128,
		i08__127 = -127,
		i08__1 = -1,
		i08_0 = 0,
		i08_1 = 1,
		i08_127 = 127;
	assert(i08__127 > i08__128);
	assert(i08__1 > i08__127);
	assert(i08_0 > i08__127);
	assert(i08_1 > i08_0);
	assert(i08_1 < i08_127);

	assert(u08_0 > i08__127);
	assert(u08_127 == i08_127);
	assert(!(u08_128 == i08_127));
	assert(u08_128 > i08_127);
	assert(u08_128 != i08__1);
	assert(i08__1 != u08_128);
	assert(!(u08_128 == i08__1));
	assert(u08_128 > i08__1);
	assert(u08_255 > i08__128);
	assert(!(u08_255 == i08__128));

	ASSERT_THROW(u08_255 + i08_1);
	ASSERT_THROW(u08_255 + i08__1);
	ASSERT_NOTHROW(u08_127 + i08__1);
	ASSERT_NOTHROW(u08_127 * i08__1);
	ASSERT_THROW(u08_127 * i08__128);
	ASSERT_THROW(u08_127 * u08_128);
	ASSERT_THROW(i08_127 * i08_127);
	ASSERT_THROW(i08__127 * i08_127);
	ASSERT_THROW(i08_127 * i08__127);
	ASSERT_THROW(i08__127 * i08__127);
	ASSERT_THROW(i08__128 * i08__1);
	ASSERT_THROW(auto tmp = i08_127; ++tmp;);
	ASSERT_THROW(auto tmp = u08_255; tmp++;);
	ASSERT_NOTHROW(auto tmp = i08_127; --tmp;);
	ASSERT_NOTHROW(auto tmp = u08_255; tmp--;);
	ASSERT_NOTHROW(auto tmp = i08_0; ++tmp;);
	ASSERT_NOTHROW(auto tmp = u08_0; tmp++;);
	ASSERT_THROW(auto tmp = u08_0; tmp--;);
	ASSERT_THROW(auto tmp = u08_0; --tmp;);
	ASSERT_THROW(-i08__128;);
	ASSERT_NOTHROW(-i08__127;);

	const U16
		u16_0 = 0,
		u16_1 = 1,
		u16_127 = 127,
		u16_128 = 128,
		u16_32767 = 32767,
		u16_32768 = 32768,
		u16_65535 = 65535;
	assert(u16_0 < u16_1);
	assert(u16_128 > u16_127);
	assert(u16_32767 < u16_32768);
	assert(u16_32768 < u16_65535);

	const I16
		i16__32768 = -32768,
		i16__32767 = -32767,
		i16__1 = -1,
		i16_0 = 0,
		i16_1 = 1,
		i16_32767 = 32767;
	assert(i16__32768 < i16__32767);
	assert(i16__32767 < i16__1);
	assert(i16_0 > i16__1);
	assert(i16_1 < i16_32767);
	
	ASSERT_THROW(i16_32767 * u16_32767);
	ASSERT_NOTHROW(i16__1 * i16_1);
	ASSERT_NOTHROW(i16__1 / i16_1);
	ASSERT_THROW(i16__1 / i16_0);
	ASSERT_THROW(i16__1 / u16_0);
	ASSERT_NOTHROW(-u16_1);
	ASSERT_NOTHROW(-i16_1);
	
	if(i16_0){
		assert(false);
	}
	
	if(i16_1){
		assert(true);
	} 
	else {
		assert(false);
	}
	
	if(!i16_1){
		assert(false);
	}
	std::stringstream stream;
	stream << i16_0 << std::endl;
	assert(stream.str() == "0\n");
	
	ASSERT_THROW(I(INT_MIN) * I(INT_MIN););
	ASSERT_THROW(I(INT_MIN) * I(INT_MAX););
	ASSERT_THROW(I(INT_MAX) * I(INT_MIN););
	ASSERT_THROW(I(INT_MAX) * I(INT_MAX););
	
	ASSERT_THROW(U(UINT_MAX) * I(INT_MIN););
	ASSERT_THROW(U(UINT_MAX) * I(INT_MAX););
}

