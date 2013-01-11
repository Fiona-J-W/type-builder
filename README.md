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
	int x = 1.0;
	int y = 2.0;
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

