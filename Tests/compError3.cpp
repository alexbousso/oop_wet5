#include "../OOP5.h"

class SuperCow {};

class God : public SuperCow {};

class Dog : public God {};

Dog snoop = OOP5::my_static_cast<Dog, SuperCow>(SuperCow());

int main() {
	return 0;
}
