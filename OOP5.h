#ifndef OOP_H_
#define OOP_H_

#include <type_traits>
#include <functional> // for std::hash<>()
#include <utility>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

typedef char (&yes)[1];
typedef char (&no)[2];

template <typename B, typename D>
struct Host
{
	operator B*() const;
	operator D*();
};

template <typename B, typename D>
struct IsBaseOf
{
	template <typename T> 
	static yes check(D*, T);
	static no check(B*, int);

	static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(yes);
};

struct Type {
	const string name;
	size_t hash_code;

	template <typename T>
	Type(T t) {
		t = t;
	}

	Type(const string name, const int type_size) : name(name),
			hash_code(hash<string>(name) + PRIME_NUMBER * type_size) {}

	inline bool operator ==(const Type& other) const {
		return this->hash_code == other.hash_code;
	}

private:
	const int PRIME_NUMBER = 97;
	
};

template <class T>
class OOPPolymorphic {
	friend T; //only the actual type can register this class at the OOP5's data structure.
private:
	static Type* my_type = nullptr;

	void RegisterInheritence(const Type* base) {
		bool isInGraph = false;
		for(auto it : inheritance_graph) {
			if(it == base) {
				isInGraph = true;
			}
		}

		if(!isInGraph) {
			inheritance_graph.push_back(vector<Type*>(1, base));
		}

		if(base == nullptr) {
			return;
		}

		for(auto it : inheritance_graph) {
			if(it == base) {
				it->push_back(base);
			}
		}
	}

	OOPPolymorphic() {
		my_type = new Type()
	}
public:
	static const Type* GetType() {
		if(my_type == nullptr) {
			my_type = Type()
		}
	}
	virtual const Type* MyType() =0; //You can assume this method will always be overriden by the inheriting class, and it will be simply implemented by: return OOPPolymorphic<NewClassName>::GetType()
};

class OOP5 {
public:
	// Implementation of the static data structure used to hold the inheritance relationships.
	// For each element in the graph, the first item in the vector is the type itself, whereas
	// the second element and forth are the parents
	static vector<vector<Type*>> inheritance_graph;

	template<typename Dst, typename Src>
	static Dst my_static_cast(Src src) {
		typedef char implicitlyConvertible[(is_convertible<Src, Dst>::value) ? 1 : 0];
		typedef char bothReferences[(is_reference<Src>::value && is_reference<Dst>::value) ? 1 : 0];
		typedef char bothPointers[(is_pointer<Src>::value && is_pointer<Dst>::value) ? 1 : 0];

		// Checking if one of the following is true:
		//     - Src is implicitly convertible to Dst
		//     - both Src and Dst are references
		//     - both Src and Dst are pointers
		// If none of them apply, then the compilation will fail.
		static_assert((((sizeof(implicitlyConvertible)) | (sizeof(bothReferences))) |
			(sizeof(bothPointers))) == 1, "");

		typedef typename remove_pointer<Src>::type SrcNoPtr;
		typedef typename remove_pointer<Dst>::type DstNoPtr;

		typedef typename remove_reference<SrcNoPtr>::type SrcClean;
		typedef typename remove_reference<DstNoPtr>::type DstClean;

		typedef char checkInheritance[IsBaseOf<SrcClean, DstClean>::value ? 1 : 0];

		// Checking if one of the following apply:
		//     - BOTH apply:
		//         1. Dst and Src are either both references, or both pointers
		//         2. Dst inherits from Src
		//     - Src is implicitly convertible to Dst
		// If none of them apply, then the compilation will fail.
		static_assert(((((sizeof(bothReferences)) | (sizeof(bothPointers))) & (sizeof(checkInheritance))) |
			(sizeof(implicitlyConvertible))) == 1, "");

		return (Dst)src;
	}

	template<typename Dst, typename Src>
	static Dst my_dynamic_cast(Src src) {
		typedef char bothReferences[(is_reference<Src>::value && is_reference<Dst>::value) ? 1 : 0];
		typedef char bothPointers[(is_pointer<Src>::value && is_pointer<Dst>::value) ? 1 : 0];

		// Checking if both Dst and Src are pointers, or if both Dst and Src are references.
		// In the case of failure, then the compilation will fail.
		static_assert((sizeof(bothPointers) | (sizeof(bothReferences))) == 1, "");

		typedef char implicitlyConvertible[(is_convertible<Src, Dst>::value) ? 1 : 0];

		// TODO: check this code, according to bullet number 2 in part 2.1 <----------------------------------------- remember me!
		conditional<sizeof(implicitlyConvertible) == 1, yes, no>::type res;
		if (sizeof(res) == sizeof(yes)) {
			return (Dst)src;
		}

		typedef char isOOPPolymorphic[((IsBaseOf<OOPPolymorphic, Src>::value) && 
			(IsBaseOf<OOPPolymorphic, Dst>::value)) ? 1 : 0];

		// Checking if Src is implicitly convertible to Dst, or if both Src and Dst inherit from
		// the class OOPPolymorphic<>
		static_assert(((implicitlyConvertible) | (isOOPPolymorphic<Src>)) == 0, "");
	}

	static int InheritsFrom(const Type* derived,const Type* base) {
		vector<Type*> baseLocation = find(inheritance_graph.iterator_begin(), inheritance_graph.iterator_end(), base);
		vector<Type*> derivedLocation = find(inheritance_graph.iterator_begin(), inheritance_graph.iterator_end(), derived);

        return countPaths(derivedLocation, baseLocation); 
	}

private:

	int countPaths(vector<Type>& current, vector<Type>& destination) {
		if(current == destination) {
			return 1;
		}

		int count = 0;
		for(auto it : inheritance_graph[current]) {
			count += countPaths(it);
		}

		return count;
	}

};

#endif /* OOP_H_ */
