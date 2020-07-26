#include <iostream>
#include "LineBuilder.h"

int main() {
	LineBuilder lb = LineBuilder();
	Line l = lb.getLine({ 0,0,0 }, {1000,500});
	
	std::cout << "Hello World!\n" << l.getV() << std::endl;
}

