#include "OOP5.h"

class SuperCow {};

class God : public SuperCow {};

// Compilation error: could not convert ‘src’ from ‘SuperCow’ to ‘God’
God g = OOP5::my_static_cast<God, SuperCow>(SuperCow());

int main() {
	return 0;
}
