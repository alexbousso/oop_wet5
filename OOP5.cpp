#include "OOP5.h"
#include <iostream>
#include <typeinfo>
using namespace std;

/**
 * Evaluates b and continues if b is true.
 * If b is false, ends the test by returning false and prints a detailed
 * message about the failure.
 */
#define ASSERT(b) do { \
        if (!(b)) { \
                printf("\nAssertion failed at %s:%d %s ",__FILE__,__LINE__,#b); \
                return false; \
        } \
} while (0)

/**
 * Macro used for running a test from the main function
 */
#define RUN_TEST(test) do { \
        printf("Running "#test"... "); \
        if (test) { \
            printf("[OK]\n");\
        } else { \
        	printf("[Failed]\n"); \
        	return 1; \
        } \
} while(0)

class SuperCow {};

class God : public SuperCow {};

class Dog : public God {};




// Dynamically inheriting classes
class Satan : public OOPPolymorphic<Satan> {
public:
	Satan() {
		RegisterInheritance(NULL);
	}

	const Type* MyType() {
		return NULL; //OOPPolymorphic<Satan>::GetType();
	}
};

class Cat : public Satan, public OOPPolymorphic<Cat> {
public:
	Cat() {
		OOPPolymorphic<Cat>::RegisterInheritance(Satan::GetType());
	}

	const Type* MyType() {
		return NULL;//OOPPolymorphic<Cat>::GetType();
	}
};

static bool testStaticAssert() {
		int i = 5;
		double d = OOP5::my_static_cast<double, int>(i);
	
	// Compilation error: could not convert ‘src’ ‘int’ to ‘SuperCow’
	// SuperCow sc = OOP5::my_static_cast<SuperCow, int>(i);

	// Compilation error: could not convert ‘src’ from ‘SuperCow’ to ‘God’
	// God g = OOP5::my_static_cast<God, SuperCow>(SuperCow());

	SuperCow sc = OOP5::my_static_cast<SuperCow, God>(God());

	SuperCow superSuperCow;
	SuperCow& refCow = superSuperCow;

	God *g1 =  OOP5::my_static_cast<God *, SuperCow *>(new SuperCow());
	God &g2 =  OOP5::my_static_cast<God&, SuperCow&>(refCow);

	ASSERT(typeid(g1) != typeid(SuperCow *));
	ASSERT(typeid(g1) == typeid(God *));

	ASSERT(typeid(g2) != typeid(SuperCow&));
	ASSERT(typeid(g2) == typeid(God&));

	// Compilation error
	// Dog snoop = OOP5::my_static_cast<Dog, SuperCow>(SuperCow());

	Dog& snoop_ref = OOP5::my_static_cast<Dog&, SuperCow&>(refCow);
	Dog* snoop_ptr = OOP5::my_static_cast<Dog*, SuperCow*>(new SuperCow());

	ASSERT(typeid(snoop_ref) != typeid(SuperCow&));
	ASSERT(typeid(snoop_ref) == typeid(Dog&));

	ASSERT(typeid(snoop_ptr) != typeid(SuperCow*));
	ASSERT(typeid(snoop_ptr) == typeid(Dog*));

	SuperCow mooo = OOP5::my_static_cast<SuperCow, Dog>(Dog());

	ASSERT(typeid(mooo) != typeid(Dog));
	ASSERT(typeid(mooo) == typeid(SuperCow));

	return true;
}

static bool testInheritsFrom() {


	return true;
}

int main() {
	RUN_TEST(testStaticAssert());
}