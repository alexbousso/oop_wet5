// This test tries to dynamically cast an OOPPolymorphic type to a non-OOPPolymorphic type.
// This should fail

#include "../OOP5.h"

class Entity : public OOPPolymorphic<Entity> {
public:

	Entity() { OOPPolymorphic<Entity>::RegisterInheritence(NULL); }

	const Type* MyType() {
		return OOPPolymorphic<Entity>::GetType();
	}
};


class Person : public Entity, public OOPPolymorphic<Person> {
public:
	Person() { OOPPolymorphic<Person>::RegisterInheritence(OOPPolymorphic<Entity>::GetType()); }

	const Type* MyType() {
		return OOPPolymorphic<Person>::GetType();
	}
};

class Monkey : public Entity, public OOPPolymorphic<Monkey> {
public:
	Monkey() { OOPPolymorphic<Monkey>::RegisterInheritence(OOPPolymorphic<Entity>::GetType()); }

	const Type* MyType() {
		return OOPPolymorphic<Monkey>::GetType();
	}
};

class Afraid : public Person, public OOPPolymorphic<Afraid> {
public:
	Afraid() { OOPPolymorphic<Afraid>::RegisterInheritence(OOPPolymorphic<Person>::GetType()); }

	const Type* MyType() {
		return OOPPolymorphic<Afraid>::GetType();
	}
};

class NotAfraid : public Person, public Monkey {
};



int main() {
    Afraid a;
    Afraid& ref_a = a;
    
    NotAfraid& na = OOP5::my_dynamic_cast<NotAfraid&, Afraid&>(ref_a);
	return 0;
}
