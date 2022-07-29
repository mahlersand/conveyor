#pragma once

#include <ranges>
#include <utility>

namespace conveyor
{
	template <typename View>
	concept input_view =
	        std::ranges::input_range<View> &&
	        std::ranges::view<View>;

	template<input_view View>
	struct enumerate_view;
}

namespace conveyor
{
	// Enumeration view
	template<input_view View>
	struct enumerate_view : public std::ranges::view_interface<enumerate_view<View>>
	{
		using index_type				= std::size_t;
		using base_iterator_type		= typename std::ranges::iterator_t<View>;

		struct enumerator
		{
			using value_type	= std::pair<index_type, typename std::iterator_traits<base_iterator_type>::reference>;

			std::size_t			idx;
			base_iterator_type	it;

			enumerator() = default;

			enumerator(std::size_t idx, base_iterator_type it) :
			    idx(idx),
			    it(it)
			{}

			enumerator &operator++()
			requires std::weakly_incrementable<base_iterator_type>
			{
				++idx;
				++it;

				return *this;
			}

			enumerator operator++(int)
			requires std::weakly_incrementable<base_iterator_type>
			{
				enumerator copy = *this;

				++(*this);

				return copy;
			}

			enumerator &operator--()
			requires std::bidirectional_iterator<base_iterator_type>
			{
				--idx;
				--it;

				return *this;
			}

			enumerator operator--(int)
			requires std::bidirectional_iterator<base_iterator_type>
			{
				enumerator copy = *this;

				--(*this);

				return copy;
			}

			bool operator!=(enumerator const &other) const
			{
				return it != other.it;
			}

			bool operator==(enumerator const &other) const
			{
				return !operator!=(other);
			}

			value_type operator*() const
			{
				return value_type(idx, *it);
			}

			friend std::make_signed_t<index_type> operator-(enumerator const &lhs, enumerator const &rhs)
			{
				return lhs.idx - rhs.idx;
			}
		};



	private:
		View		base_ {};
		enumerator	begin_ {0, std::begin(base_)};
		enumerator	end_ {0, std::end(base_)};

	public:
		enumerate_view() = default;

		enumerate_view(View base) :
		    base_(base),
		    begin_(0, std::begin(base_)),
		    end_(0, std::end(base_))
		{}

		enumerate_view(View base) requires std::ranges::sized_range<View> :
		    base_(base),
		    begin_(0, std::begin(base_)),
		    end_(base.size(), std::end(base_))
		{}

		constexpr enumerator begin() const
		{
			return begin_;
		}

		constexpr enumerator end() const
		{
			return end_;
		}
	};

	template <typename R>
	enumerate_view(R &&) -> enumerate_view<std::ranges::views::all_t<R>>;

	namespace details
	{
		namespace rg = std::ranges;

		struct enumerate_range_adaptor_closure
		{
			constexpr enumerate_range_adaptor_closure()
			{}

			template <rg::viewable_range R>
			constexpr auto operator()(R && r) const
			{
				return enumerate_view(std::forward<R>(r));
			}
		} ;

		struct enumerate_range_adaptor
		{
			template<rg::viewable_range R>
			constexpr auto operator()(R && r) const
			{
				return enumerate_view(std::forward<R>(r));
			}

			constexpr auto operator()() const
			{
				return enumerate_range_adaptor_closure();
			}
		};

		template <rg::viewable_range R>
		constexpr auto operator|(R&& r, enumerate_range_adaptor_closure const &a)
		{
			return a(std::forward<R>(r));
		}

		template <rg::viewable_range R>
		constexpr auto operator|(R&& r, enumerate_range_adaptor const &a)
		{
			return a(std::forward<R>(r));
		}
	}

	details::enumerate_range_adaptor enumerate;
}
