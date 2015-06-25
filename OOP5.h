#ifndef OOP_H_
#define OOP_H_

#include <type_traits>
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

template <bool g, typename THEN, typename ELSE>
struct IF {
	typedef THEN RET;
};

template <typename THEN, typename ELSE>
struct IF<false, THEN, ELSE> {
	typedef ELSE RET;
};

struct Type {

	//add your implementation here

};

template <class T>
class OOPPolymorphic {
	friend T; //only the actual type can register this class at the OOP5's data structure.
private:
	void RegisterInheritence(const Type* base);
	OOPPolymorphic() {}
public:
	static const Type* GetType();
	virtual const Type* MyType() =0; //You can assume this method will always be overriden by the inheriting class, and it will be simply implemented by: return OOPPolymorphic<NewClassName>::GetType()
};

class OOP5 {
public:
	template<typename Dst, typename Src>
	static Dst my_static_cast(Src src) {

		typedef char implicitlyConvertible[(is_convertible<Src, Dst>::value) ? 1 : 0];
		typedef char bothReferences[(is_reference<Src>::value && is_reference<Dst>::value) ? 1 : 0];
		typedef char bothPointers[(is_pointer<Src>::value && is_pointer<Dst>::value) ? 1 : 0];

		static_assert((((sizeof(implicitlyConvertible)) | (sizeof(bothReferences))) |
			(sizeof(bothPointers))) == 1, "");

		typedef typename remove_pointer<Src>::type SrcNoPtr;
		typedef typename remove_pointer<Dst>::type DstNoPtr;

		typedef typename remove_reference<SrcNoPtr>::type SrcClean;
		typedef typename remove_reference<DstNoPtr>::type DstClean;

		typedef char checkInheritance[IsBaseOf<SrcClean, DstClean>::value ? 1 : 0];

		static_assert(((((sizeof(bothReferences)) | (sizeof(bothPointers))) & (sizeof(checkInheritance))) |
			(sizeof(implicitlyConvertible))) == 1, "");

		return (Dst)src;
	}

	template<typename Dst, typename Src>
	static Dst my_dynamic_cast(Src src) {
		// typedef char bothReferences[(is_reference<Src>::value && is_reference<Dst>::value) ? 1 : 0];
		// typedef char bothPointers[(is_pointer<Src>::value && is_pointer<Dst>::value) ? 1 : 0];

		// static_assert((sizeof(bothPointers) | (sizeof(bothReferences))) == 1, "");

		// IF<>::RET
	}

	static int InheritsFrom(const Type* derived,const Type* base) {

	}
};

#endif /* OOP_H_ */
