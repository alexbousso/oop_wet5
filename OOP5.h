#ifndef OOP_H_
#define OOP_H_

#include <type_traits>
#include <functional> // for std::hash<>()
#include <utility>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>
#include <typeinfo> // TODO REMOVE
#include <iostream> // TODO REMOVE

using namespace std;

// Forward declarations
class OOP5;
struct Type;

// Global type UUID counter, increased whenever a new OOPPolymorphic type is created
static int uuidCounter = 0;

// Implementation of the static data structure used to hold the inheritance relationships.
// For each element in the graph, the first item in the vector is the type itself, and
// the second element and forth are the parents
static vector<vector<const Type*>> inheritance_graph = vector<vector<const Type*>>();

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
	const int uuid;

	Type() : uuid(uuidCounter++) {}

	inline bool operator==(const Type& other) const {
		return this->uuid == other.uuid;
	}
};

template <class T>
class OOPPolymorphic {
	friend T; //only the actual type can register this class at the OOP5's data structure.
private:
	static Type* my_type;

	// Looks for the type in the inheritance graph and adds an edge connecting the type and its parent.
	// If the type is not in the inheritance graph yet, adds it first
	void RegisterInheritence(const Type* base) {
		bool isInGraph = false;
		for(auto it : inheritance_graph) {
			if(*(it)[0] == *my_type) {
				isInGraph = true;
				break;
			}
		}

		if(!isInGraph) {
			inheritance_graph.push_back(vector<const Type*>(1, my_type));
		}

		if(base == NULL) {
			return;
		}

		for(int i = 0 ; i < inheritance_graph.size() ; i++) {
			if(*inheritance_graph[i][0] == *my_type) {
				for(auto it : inheritance_graph[i]) {
					if(*it == *base) {
						return;
					}
				}

				inheritance_graph[i].push_back(base);
			}
		}

//		for(auto it : inheritance_graph) {
//			if(*(it)[0] == *my_type) {
//				it->push_back(base);
//			}
//		}
	}

	OOPPolymorphic() {
		if(my_type == NULL) {
			my_type = new Type();
		}
	}

public:
	static const Type* GetType() {
		return my_type;
	}
	virtual const Type* MyType() = 0; //You can assume this method will always be overriden by the inheriting class, and it will be simply implemented by: return OOPPolymorphic<NewClassName>::GetType()
};

template <typename T>
Type* OOPPolymorphic<T>::my_type = NULL;// new Type();

class OOP5 {
public:
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
		//typename conditional<sizeof(implicitlyConvertible) == 1, yes, no>::type res;
		if (sizeof(implicitlyConvertible) == 1) {
			return (Dst)src;
		}

		typedef typename remove_pointer<Src>::type SrcNoPtr;
		typedef typename remove_pointer<Dst>::type DstNoPtr;

		typedef typename remove_reference<SrcNoPtr>::type SrcClean;
		typedef typename remove_reference<DstNoPtr>::type DstClean;

		typedef char isOOPPolymorphic[((IsBaseOf<OOPPolymorphic<SrcClean>, SrcClean>::value) &&
			(IsBaseOf<OOPPolymorphic<DstClean>, DstClean>::value)) ? 1 : 0];

		// Checking if Src is implicitly convertible to Dst, or if both Src and Dst inherit from
		// the class OOPPolymorphic
		static_assert(((sizeof(implicitlyConvertible)) | (sizeof(isOOPPolymorphic))) == 1, "");
		cout << "Src is: " << typeid(src).name() << endl;
		cout << "Size of isOOPPolymorphic: " << sizeof(isOOPPolymorphic) << endl;
		cout << "Size of implicitlyConvertible: " << sizeof(implicitlyConvertible) << endl;

		// Find the dynamic type of src
		const Type* srcStaticType = OOPPolymorphic<SrcClean>::GetType();
		const Type* dstStaticType = OOPPolymorphic<DstClean>::GetType();
		const Type* srcDynamicType = returnSource(src)->MyType();
		

		if((*srcStaticType == *srcDynamicType) || InheritsFrom(srcDynamicType, srcStaticType) == 1) {
			if(InheritsFrom(dstStaticType, srcStaticType) == 1) {
				if((*srcDynamicType == *dstStaticType) || InheritsFrom(srcDynamicType, dstStaticType)) {
					return (Dst)src;
				}
			}
		}

		if(is_pointer<Src>::value) {
			return NULL;
		} else {
			assert(is_reference<Src>::value);
			throw std::bad_cast();
		}
	}

	static int InheritsFrom(const Type* derived,const Type* base) {
		if(base == NULL || derived == NULL)
			return 0;

		vector<const Type*> baseLocation = findGraphNode(base);
		vector<const Type*> derivedLocation = findGraphNode(derived);

        return countPaths(derivedLocation, baseLocation); 
	}

private:

	template<typename Src>
	static Src* returnSource(Src* src) {
		return src;
	}

	template<typename Src>
	static Src* returnSource(Src& src) {
		return &src;
	}

	static vector<const Type*> findGraphNode(const Type* type) {
		for(auto it = inheritance_graph.begin(); it != inheritance_graph.end(); it++) {
			if(*((*it)[0]) == *type) {
				return *it;
			}
		}
	}

	// static int countPaths(int current, int destination) {
	// 	if(current == destination) {
	// 		return 1;
	// 	}

	// 	int count = 0;
	// 	for(auto it : inheritance_graph[current]) {
	// 		count += countPaths(it);
	// 	}

	// 	return count;
	// }

	static int countPaths(vector<const Type*> &current, vector<const Type*> &destination) {
		if(current == destination) {
			return 1;
		}

		if(current.size() <= 1) {
			return 0;
		}

		assert(current.size() > 1);

		int count = 0;
		for(auto it = ++current.begin(); it != current.end(); it++) {
			vector<const Type*> nextVector;
			for(int i = 0 ; i < inheritance_graph.size() ; i++) {
				if(inheritance_graph[i][0] == *it) {
					nextVector = inheritance_graph[i];
					break;
				}
			}

			count += countPaths(nextVector, destination);
		}

		return count;
	}

};

#endif /* OOP_H_ */
