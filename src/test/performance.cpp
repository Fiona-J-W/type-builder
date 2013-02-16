#include "../include/basic_number.hpp"
#include <cstdlib>
#include <cstdio>

enum: type_builder::flag_t{
	int_type_settings = type_builder::DEFAULT_SETTINGS
		| type_builder::ENABLE_FLOAT_MULT_DIV
		| type_builder::ENABLE_MODULO
};

enum{
	int_type
};

using Int = type_builder::basic_number<int, int_type, int_type_settings>;
using Int2 = type_builder::basic_number<int, int_type, int_type_settings | type_builder::ENABLE_NATIVE_TYPING>;

int main(int argc, char** argv){
	if(argc!=3){
		return 1;
	}
	int n = atoi(argv[2]);
	if(argv[1][0] == 'i'){
		int a(1), b(2), c(3);
		for(int i=0; i<n; ++i){
			int tmp = b*3.5;
			a += tmp + (c+c);
			c = c+c;
			b *=2;
			a %= 10000;
			b %= 20000;
			c %= 12345;
		}
		printf("int: %d\n", a);
		//return (a == int(0));
	}
	else if(argv[1][0] == 'I'){
		Int a(1), b(2), c(3);
		for(int i=0; i<n; ++i){
			a += b*3.5 + (c+c);
			c = c+c;
			b *=2;
			a %= 10000;
			b %= 20000;
			c %= 12345;
		}
		printf("Int: %d\n", a.get_value());
		//return (a == Int(0));
	}
	else if(argv[1][0] == 'f'){
		int a(1), b(2), c(3);
		for(int i=0; i<n; ++i){
			a += b*3.5 + (c+c);
			c = c+c;
			b *=2;
			a %= 10000;
			b %= 20000;
			c %= 12345;
		}
		printf("int (with float): %d\n", a);
		//return (a == Int(0));
	}
	else if(argv[1][0] == 'F'){
		Int2 a(1), b(2), c(3);
		for(int i=0; i<n; ++i){
			a += b*3.5 + (c+c);
			c = c+c;
			b *=2;
			a %= 10000;
			b %= 20000;
			c %= 12345;
		}
		printf("Int (with float): %d\n", a.get_value());
		//return (a == Int(0));
	}
	else return 2;
}
