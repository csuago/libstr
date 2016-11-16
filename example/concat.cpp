#include <stdio.h>
#include "../header/string.h"

using namespace libstr;

int main(void) {

	string s = "This is a text";
	s += " this is a new text";
	s.replace("text", "string");

	printf("%s\n", s.toString());

	system("PAUSE");
	return 0;
}