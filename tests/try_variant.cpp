#include "../details/variant.hpp"
#include "../point.hpp"
#include <cassert>

using namespace Vlinder::RTIMDB;

int tryCreateInstance()
{
	Details::Variant< Point > v;
	return v.empty() ? 0 : 1;
}
int tryNonEmptyInstance()
{
	Point p;
	Details::Variant< Point > v(p);

	return v.empty() ? 1 : 0;
}

int tryCopyVariant()
{
	struct T
	{
		T(bool &f)
			: f_(f)
		{}
		T(T const &t)
			: f_(t.f_)
		{
			f_ = true;
		}

		bool &f_;
	};
	bool f(false);
	T t(f);
	Details::Variant< T > v1(t);
	if (!f) return 1;
	f = false;
	Details::Variant< T > v2(t);
	return f ? 0 : 1;
}

int tryAssignVariant()
{
	struct T
	{
		T(bool &f)
			: f_(&f)
		{}
		T(T const &t)
			: f_(t.f_)
		{
			*f_ = true;
		}

		bool *f_;
	};
	bool f(false);
	T t(f);
	Details::Variant< T > v1(t);
	if (!f) return 1;
	f = false;
	Details::Variant< T > v2;
	v2 = v1;
	return f ? 0 : 1;
}

int main()
{
	return 0
		|| tryCreateInstance()
		|| tryNonEmptyInstance()
		|| tryCopyVariant()
		|| tryAssignVariant()
		;
}
