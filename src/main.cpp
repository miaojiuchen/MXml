#include<mstring.h>

#include<iostream>

int main()
{
	MString str1("str1");
	MString str2("str2");
	const char *c_str3 = "c_str3";

	MString str4 = c_str3 + str2;

	getchar();
}