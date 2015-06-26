// This test tries to dynamically cast a non-OOPPolymorphic type to a OOPPolymorphic type.
// This should fail

#include "OOP5.h"

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

class Afraid : public Person{
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
};



int main() {
    NotAfraid* na = OOP5::my_dynamic_cast<NotAfraid*, Afraid*>(new Afraid());
	return 0;
}
