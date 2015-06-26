#include "OOP5.h"
#include <iostream>
#include <typeinfo> // TODO REMOVE
#include <type_traits> // TODO REMOVE
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
		RegisterInheritence(NULL);
	}

	const Type* MyType() {
		return NULL; //OOPPolymorphic<Satan>::GetType();
	}
};

class Cat : public Satan, public OOPPolymorphic<Cat> {
public:
	Cat() {
		OOPPolymorphic<Cat>::RegisterInheritence(Satan::GetType());
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

// Test - if you believe then you're not afraid.
// Inheritance paths: 
// 		- Believer -> NotAfraid -> Person -> Entity
// 		- NonBeliever -> Afraid -> Person -> Entity
// 		- Believer -> NotAfraid -> Monkey -> Entity
class Entity : public OOPPolymorphic<Entity> {
public:

	Entity() { OOPPolymorphic<Entity>::RegisterInheritence(NULL); }
	
	const Type* MyType() {
		return OOPPolymorphic<Entity>::GetType();
	}

	char f() {
		return 'f';
	}
};


class Person : public Entity, public OOPPolymorphic<Person> {
public:
	Person() { OOPPolymorphic<Person>::RegisterInheritence(OOPPolymorphic<Entity>::GetType()); }
	
	const Type* MyType() {
		return OOPPolymorphic<Person>::GetType();
	}

	char p() {
		return 'p';
	}
};

class Monkey : public Entity, public OOPPolymorphic<Monkey> {
public:
	Monkey() { OOPPolymorphic<Monkey>::RegisterInheritence(OOPPolymorphic<Entity>::GetType()); }

	const Type* MyType() {
		return OOPPolymorphic<Monkey>::GetType();
	}

	char m() {
		return 'm';
	}
};

class Afraid : public Person, public OOPPolymorphic<Afraid> {
public:
	Afraid() { OOPPolymorphic<Afraid>::RegisterInheritence(OOPPolymorphic<Person>::GetType()); }

	const Type* MyType() {
		return OOPPolymorphic<Afraid>::GetType();
	}
};

class NotAfraid : public Person, public Monkey, public OOPPolymorphic<NotAfraid> {
public:
	NotAfraid() { 
		OOPPolymorphic<NotAfraid>::RegisterInheritence(OOPPolymorphic<Person>::GetType());
		OOPPolymorphic<NotAfraid>::RegisterInheritence(OOPPolymorphic<Monkey>::GetType());
	}

	const Type* MyType() {
		return OOPPolymorphic<NotAfraid>::GetType();
	}

	char na() {
		return 'n';
	}
};

class Believer : public NotAfraid, public OOPPolymorphic<Believer> {
public:
	Believer() { OOPPolymorphic<Believer>::RegisterInheritence(OOPPolymorphic<NotAfraid>::GetType()); }

	const Type* MyType() {
		return OOPPolymorphic<Believer>::GetType();
	}
};

class NonBeliever : public Afraid, public OOPPolymorphic<NonBeliever> {
public:
	NonBeliever() { OOPPolymorphic<NonBeliever>::RegisterInheritence(OOPPolymorphic<Afraid>::GetType()); }

	const Type* MyType() {
		return OOPPolymorphic<NonBeliever>::GetType();
	}

	char b() {
		return 'b';
	}
};

static bool testInheritsFrom() {
	return true;
}

static bool testGraphConstruction() {
	const Type* f = OOPPolymorphic<Person>::GetType();
	ASSERT(f == NULL);

	Person* person = new Person();
	const Type* f2 = OOPPolymorphic<Person>::GetType();
	ASSERT(f2 != NULL);
	Afraid* afraid = new Afraid();
	ASSERT(OOP5::InheritsFrom(afraid->MyType(), person->MyType()) == 1);

	Believer* believer = new Believer();
	Entity* entity = new Entity();
	ASSERT(OOP5::InheritsFrom(believer->MyType(), entity->MyType()) == 2);

	NonBeliever* nonBeliever = new NonBeliever();
	Monkey* monkey = new Monkey();
	ASSERT(OOP5::InheritsFrom(nonBeliever->MyType(), monkey->MyType()) == 0);
	ASSERT(OOP5::InheritsFrom(entity->MyType(), afraid->MyType()) == 0);

	Person* person2 = new Person();
	Afraid* afraid2 = new Afraid();
	ASSERT(OOP5::InheritsFrom(afraid->MyType(), person->MyType()) == 1);
	ASSERT(OOP5::InheritsFrom(afraid2->MyType(), person->MyType()) == 1);
	ASSERT(OOP5::InheritsFrom(afraid->MyType(), person2->MyType()) == 1);
	ASSERT(OOP5::InheritsFrom(afraid2->MyType(), person2->MyType()) == 1);

	ASSERT(OOP5::InheritsFrom(NULL, NULL) == 0);

	return true;
}

static bool testDynamicCast() {
	Afraid afraid;
	Person person;
	NonBeliever nonBeliever;
	Believer believer;
	NotAfraid notAfraid;
	Monkey monkey;
	Entity entity;

	// Valid dynamic cast - Afraid* is implicitly convertible to Person*
	Afraid* afraid_ptr = new Afraid();
	Person* person_ptr = OOP5::my_dynamic_cast<Person*,Afraid*>(afraid_ptr);
	ASSERT(person_ptr != NULL);

	// Valid dynamic cast - Afraid& is implicitly convertible to Person&
	Afraid& afraid_ref = afraid;
	Person& person_ref = OOP5::my_dynamic_cast<Person&,Afraid&>(afraid_ref);

	// Invalid dynamic cast - Person* is not convertible to Afraid*, should return NULL
	Person* p = new Person();
	Afraid* a = OOP5::my_dynamic_cast<Afraid*, Person*>(p);
	ASSERT(a == NULL);

	// Invalid dynamic cast - Person& is not convertible to Afraid&, should throw bad_cast
	Person& pr = person;
	try {
		Afraid& ar = OOP5::my_dynamic_cast<Afraid&, Person&>(pr);
		ASSERT(false);
	} catch(std::bad_cast&) {}

	// Valid cast - Person* is not convertible to Afraid*, but its runtime type, which is NonBeliever*,
	// is convertible to Afraid*
	Person* pnb = new NonBeliever();
	Afraid* ap = OOP5::my_dynamic_cast<Afraid*, Person*>(pnb);
	ASSERT(ap != NULL);

	// Valid cast - Person& is not convertible to Afraid&, but its runtime type, which is NonBeliever&,
	// is convertible to Afraid&
	Person& pnbr = nonBeliever;
	Afraid& apr = OOP5::my_dynamic_cast<Afraid&, Person&>(pnbr);

	// Invalid cast - while Afraid* is convertible to Person*,
	// its dynamic type, Believer*, is not convertible to Afraid*
	Person* pbelieve = new Believer();
	Afraid* afd = OOP5::my_dynamic_cast<Afraid*, Person*>(pbelieve);
	ASSERT(afd == NULL);

	// Invalid cast - while Afraid& is convertible to Person&,
	// its dynamic type, Believer&, is not convertible to Afraid&
	Person& pbelref = believer;
	try {
		Afraid& afred = OOP5::my_dynamic_cast<Afraid&, Person&>(pbelref);
		ASSERT(false);
	} catch(std::bad_cast&) {}

	Entity* e = OOP5::my_dynamic_cast<Entity*, Monkey*>(new Believer());
	ASSERT(e != NULL);

	Afraid* naa = OOP5::my_dynamic_cast<Afraid*, Entity*>(new Person());
	ASSERT(naa == NULL);

	Entity& ee = OOP5::my_dynamic_cast<Entity&, Monkey&>(believer);

	try {
		Afraid& aaa = OOP5::my_dynamic_cast<Afraid&, Entity&>(person);
		ASSERT(false);
	} catch(std::bad_cast&) {}

	return true;
}

static bool testCallToDynamicMethods() {
	Monkey* monkey = OOP5::my_dynamic_cast<Monkey*, Entity*>(new Monkey());
	ASSERT(monkey->m() == 'm');

	NonBeliever* nonbeliever = OOP5::my_dynamic_cast<NonBeliever*, Entity*>(new NonBeliever());
	ASSERT(nonbeliever->b() == 'b');

	Person* person = OOP5::my_dynamic_cast<Person*, NonBeliever*>(nonbeliever);
	ASSERT(person->p() == 'p');

	return true;
}

int main() {
	RUN_TEST(testStaticAssert());
	RUN_TEST(testGraphConstruction());
	RUN_TEST(testDynamicCast());
	RUN_TEST(testCallToDynamicMethods());
}
