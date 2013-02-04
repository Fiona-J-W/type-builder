Type Builder
============

C++ offers a strong, static and reflexive typesystem. Such typesystems are able to increase the correctness of code by  detecting errors at compiletime. As a language that supports OOP (with some flaws) many projects take advantage of this ability; however one kind of types is often used without real semantics: Build-in number-types.

Let's take a look at a pretty realistic example: Implementing a point-class. Many of us would do something like this:

```c++
template<typename T>
struct point{
	point(T x, T y): x(x), y(y) {}
	//public attributes to keep this short (this is an example):
	T x;
	T y;
};

int main(){
	point<double> p(1.0, 2.0);
	// use p
}

```

This looks nice on the first glance. However: point.x and point.y are both of the same type, though they represent completly different things. Let's look at this code:

```c++
int main(){
	int x = 1;
	int y = 2;
	// simple to spot here; this won't be allways so:
	point<int> p(y, x);
	// use p
}
```

The error may be obvious but no compiler in the world will complain about this. The solution to this is giving point.x and point.y different types:

```c++
class x_coord{
	// MUCH stuff
};

class y_coord{
	// basicly exactly the same trivial stuff as in x_coord
};

struct point{
	point(x_coord x, y_coord y): x(x), y(y) {}
	//public attributes to keep this short (this is an example):
	x_coord x;
	y_coord y;
};

int main(){
	x_coord x(1.0);
	y_coord y(2.0);
	// ERROR:
	// point p(y, x);
	
	// this works like a charm:
	point p(x, y);

```

So this solves the problem, but you have to write lots of trivial code for every single type when all you want is basicly integer-semantics.

This is the part where Type Builder will help you a lot. The code can be reduced to the following:

```c++

// completly distinct types (enforced by the second template-parameter 
// (I admit that this is not the most beautiful thing but definitly much 
// more beautifull than lots of copy-paste-code):
using x_coord = type_builder::basic_number<int, 1, type_builder::ENABLE_ALL_SPECIFIC_MATH>;
using y_coord = type_builder::basic_number<int, 2, type_builder::ENABLE_ALL_SPECIFIC_MATH>;

struct point{
	point(x_coord x, y_coord y): x(x), y(y) {}
	//public attributes to keep this short (this is an example):
	x_coord x;
	y_coord y;
};

int main(){
	x_coord x(1.0);
	y_coord y(2.0);
	// ERROR:
	// point p(y, x);
	
	// this works like a charm:
	point p(x, y);
	
	// use p
}
```

Current state
=============

The library is currently in alpha-state without strong guarantees that the API won't change. Especially the 
interactions of the basic types with other types should be considered as something that might change in the
future.

Nevertheless the performance is already identical to using the build-in types if the code is compiled with 
enabled optimization.

Library description
===================

All types and constants are part of the namespace type_builder. You shouldn't do much about this because the 
usage-recommendation is anyway to use a “using”, like in the code above.

basic\_number
-------------

This template-class is used to create distinct number type that serves a specific purpose. You can enable and disable
several operators in different contexts (e.g. enable addition with other instances and multiplication with integral
numbers but not with floating point). All arithmetic (that excludes bit-operators) operators are available but most
are disabled by default. The implementations are just small wrappers around the implementation of the basetype with
std::enable\_if and static asserts to check whether the called function is enabled for the type in question.

###Template-arguments

The function takes three template-arguments:

1. T = the underlying type. This may be any type that overloads the arithmetic-operators like the built-in 
	number-types do, but the usage of the built-in ones is usually a good choice.
2. Tid = The integer that identifies the type. This has to be unique for every semantic type, even if they have 
	otherwise different template-arguments. The easiest way to use this correctly is propably to create an enum
	that contains a constant for every type.
3. Tflags = This argument is a bitmask that will controll which operations are permitted for your type. A detailed
	description can be found in the description of the settings-enum below.
4. Tbase = The base type of the basic\_number. This defaults to a do-nothing base but might be changed to enable some 
	more advanced usages.

the settings-enum
-----------------

This enum contains several constants that controll the operations that are possible to use on a 
class-template-instance; they are designed to be used as bit-masks and there are already several combined flags.

The values of the constants are undefined behaviour and may change between versions. *Don't rely on them*!

The existing flags are:

* ENABLE\_GENERAL\_CONSTRUCTION = Enables the construction from any type.
* ENABLE\_DEFAULT\_CONSTRUCTION = Enables the default-construction.
* ENABLE\_LATE\_ASSIGNEMENT = Enables late assignement from other types.

* ENABLE\_SPECIFIC\_EQUALITY\_CHECK = Enables an equality-check with other instances.
* ENABLE\_SPECIFIC\_ORDERING = Provides the ordering with any other instances.

* ENABLE\_EQUALITY\_CHECK = Enables an equality-check with most other types.
* ENABLE\_ORDERING = Provides an ordering with most other types.

* ENABLE\_INC\_DEC = Enables the usage of the pre/post-increment/decrement-operators.
* ENABLE\_SPECIFIC\_PLUS\_MINUS = Enables addition and subtraction with other instances.

* ENABLE\_SPECIFIC\_MULTIPLICATION = Enables multiplication with other instances.
* ENABLE\_ENABLE\_SPECIFIC\_DIVISION = Enables dividing by other instances.

* ENABLE\_INTEGER\_MULTIPLICATION = Enables multiplication with integers.
* ENABLE\_INTEGER\_DIVISION = Enables dividing by integers.
* ENABLE\_INTEGER\_MULT\_DIV = This combines the two flags above.

* ENABLE\_FLOAT\_MULTIPLICATION = Enables multiplication by integers and floating-point-types.
* ENABLE\_FLOAT\_DIVISION = Enables dividing by integers and floating-point-types.
* ENABLE\_FLOAT\_MULT\_DIV = This combines the two flags above.

* ENABLE\_GENERAL\_PLUS\_MINUS = Enables adding most other types.
* ENABLE\_GENERAL\_MULTIPLICATION = Enables multiplication with other types.
* ENABLE\_GENERAL\_DIVISION = Enables dividing by other types.
* ENABLE\_GENERAL\_MULT\_DIV = This combines the two flags above.

* ENABLE\_SPECIFIC\_MODULO = Enables the calculation of the modulo with other instances.
* ENABLE\_MODULO = Enable the calculation of the modulo with any supporting type.

* ENABLE\_NATIVE\_TYPING = Enables native type-behaviour. (eg. `my\_type<int> * double = my\_type<double>`)

* DISABLE\_CONSTRUCTION = Disables the creation of instances.
* DISABLE\_MUTABILITY = Disables all later changes to the value of the variable.

* DEFAULT\_SETTINGS = The default settings: this enables inc/dec, specific plus/minus and integer
	multiplication and division

* ENABLE\_ALL\_SPECIFIC\_MATH = Enables all specific math.
* ENABLE\_ALL\_MATH = Enables all math.
* ENABLE\_ALL =  enables everything.
