#ifndef vlinder_rtimdb_details_variant_hpp
#define vlinder_rtimdb_details_variant_hpp

namespace Vlinder {	namespace RTIMDB { namespace Details {
	template < typename T >
	class Variant
	{
	public :
		Variant()
			: empty_(true)
			, p_(nullptr)
		{ /* no-op */ }

		Variant(T const &v)
			: empty_(true)
			, p_(nullptr)
		{
			p_ = new (value_.buffer_) T(v);
			empty_ = false;
		}

		T const &get() const { return *p_; }
		T const &operator*() const { return get(); }
		T const *operator->() const { return p_; }
		bool empty() const { return empty_; }

	private :
		bool empty_;
		union
		{
			double alignment_;
			unsigned char buffer_[sizeof(T)];
		} value_;
		T *p_;
	};
}}}


#endif

