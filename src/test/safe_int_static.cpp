#include "../include/safe_int.hpp"

#include <cstdlib>
#include <iostream>
#include <type_traits>

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

////////////////////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<decltype(std::declval<I08>() + std::declval<I08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() + std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() + std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() + std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() + std::declval<U08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() + std::declval<U16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() + std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() + std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U08>() + std::declval<I08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() + std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() + std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() + std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() + std::declval<U08>()),U08>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() + std::declval<U16>()),U16>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() + std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() + std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I16>() + std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() + std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() + std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() + std::declval<U16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() + std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() + std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U16>() + std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() + std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() + std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() + std::declval<U16>()),U16>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() + std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() + std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I32>() + std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() + std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() + std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() + std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U32>() + std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() + std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() + std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() + std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I64>() + std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I64>() + std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U64>() + std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U64>() + std::declval<U64>()),U64>::value, "");


////////////////////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<decltype(std::declval<I08>() - std::declval<I08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() - std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() - std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() - std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() - std::declval<U08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() - std::declval<U16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() - std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() - std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U08>() - std::declval<I08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() - std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() - std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() - std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() - std::declval<U08>()),U08>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() - std::declval<U16>()),U16>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() - std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() - std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I16>() - std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() - std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() - std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() - std::declval<U16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() - std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() - std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U16>() - std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() - std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() - std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() - std::declval<U16>()),U16>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() - std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() - std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I32>() - std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() - std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() - std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() - std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U32>() - std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() - std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() - std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() - std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I64>() - std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I64>() - std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U64>() - std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U64>() - std::declval<U64>()),U64>::value, "");


////////////////////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<decltype(std::declval<I08>() * std::declval<I08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() * std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() * std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() * std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() * std::declval<U08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() * std::declval<U16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() * std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() * std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U08>() * std::declval<I08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() * std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() * std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() * std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() * std::declval<U08>()),U08>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() * std::declval<U16>()),U16>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() * std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() * std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I16>() * std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() * std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() * std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() * std::declval<U16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() * std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() * std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U16>() * std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() * std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() * std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() * std::declval<U16>()),U16>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() * std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() * std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I32>() * std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() * std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() * std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() * std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U32>() * std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() * std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() * std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() * std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I64>() * std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I64>() * std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U64>() * std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U64>() * std::declval<U64>()),U64>::value, "");


////////////////////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<decltype(std::declval<I08>() / std::declval<I08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() / std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() / std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() / std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() / std::declval<U08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() / std::declval<U16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() / std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I08>() / std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U08>() / std::declval<I08>()),I08>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() / std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() / std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() / std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() / std::declval<U08>()),U08>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() / std::declval<U16>()),U16>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() / std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U08>() / std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I16>() / std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() / std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() / std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() / std::declval<U16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() / std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I16>() / std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U16>() / std::declval<I16>()),I16>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() / std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() / std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() / std::declval<U16>()),U16>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() / std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U16>() / std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I32>() / std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() / std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() / std::declval<U32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<I32>() / std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U32>() / std::declval<I32>()),I32>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() / std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() / std::declval<U32>()),U32>::value, "");
static_assert(std::is_same<decltype(std::declval<U32>() / std::declval<U64>()),U64>::value, "");


static_assert(std::is_same<decltype(std::declval<I64>() / std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<I64>() / std::declval<U64>()),I64>::value, "");

static_assert(std::is_same<decltype(std::declval<U64>() / std::declval<I64>()),I64>::value, "");
static_assert(std::is_same<decltype(std::declval<U64>() / std::declval<U64>()),U64>::value, "");

static_assert(std::is_same<decltype(-std::declval<I08>()), I08>::value,"");
static_assert(std::is_same<decltype(-std::declval<I16>()), I16>::value,"");
static_assert(std::is_same<decltype(-std::declval<I32>()), I32>::value,"");
static_assert(std::is_same<decltype(-std::declval<I64>()), I64>::value,"");

static_assert(std::is_same<decltype(-std::declval<U08>()), I08>::value,"");
static_assert(std::is_same<decltype(-std::declval<U16>()), I16>::value,"");
static_assert(std::is_same<decltype(-std::declval<U32>()), I32>::value,"");
static_assert(std::is_same<decltype(-std::declval<U64>()), I64>::value,"");

static_assert(!U08{},"");

int main(){}
