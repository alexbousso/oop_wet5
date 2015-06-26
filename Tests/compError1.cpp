#include "../OOP5.h"

class SuperCow {};

class God : public SuperCow {};

// Compilation error: could not convert ‘src’ from ‘int’ to ‘SuperCow’
SuperCow sc = OOP5::my_static_cast<SuperCow, int>(5);

int main() {
	return 0;
}
