#pragma once

#include <vector>
#include <list>
#include <optional>

#include <Core/System/Function.hpp>

namespace Core
{

	template<typename T>
	struct WrapVector {
		typedef std::vector<T> Type;
	};

	template<typename T>
	struct WrapList {
		typedef std::list<T> Type;
	};

	template<typename T>
	struct WrapString {
		typedef std::basic_string<T, std::char_traits<T>, std::allocator<T>> Type;
	};

	template<typename T, template<typename> class TCollection>
	struct Collection : TCollection<T>::Type {

		using RawCollectionType = typename TCollection<T>::Type;
		using TCollection<T>::Type::Type;

		Collection filter(const Predicate<T> & predicate) const
		{
			Collection result;

			for (const auto & element : *this)
			{
				if (predicate(element))
				{
					result.push_back(element);
				}
			}

			return result;
		}

		T reduce(const Function<T(T, T)> & combine) const
		{
			if (this->empty()) return T();

			T result = this->front();
			
			for (auto itr = ++this->begin(); itr != this->end(); itr++)
			{
				result = combine(result, *itr);
			}

			return result;
		}

		template<typename TReturnType, template<typename> class TNewCollection = TCollection>
		Collection<TReturnType, TNewCollection> map(const Function<TReturnType(T)> & function) const
		{
			Collection<TReturnType, TNewCollection> result;

			for (const auto & element : *this)
				result.push_back(function(element));

			return result;
		}

		void for_each(const Action<T> & onIteration) const
		{
			for (const auto & element : *this)
				onIteration(element);
		}

		void for_each(const Action<T, size_t> & onIteration) const
		{
			size_t index = 0;
			for (const auto & element : *this)
				onIteration(element, index++);
		}

		bool every(const Predicate<T> & predicate) const
		{
			for (const auto & element : *this)
				if (!predicate(element))
					return false;

			return true;
		}

		bool any(const Predicate<T> & predicate) const
		{
			for (const auto & element : *this)
				if (predicate(element))
					return true;

			return false;
		}

		typename RawCollectionType::const_iterator find(const Predicate<T> & predicate) const
		{
			typename RawCollectionType::const_iterator itr = this->begin();
			for (; itr != this->end(); itr++)
				if (predicate(*itr))
					return itr;

			return itr;
		}

		typename RawCollectionType::iterator find(const Predicate<T> & predicate)
		{
			typename RawCollectionType::iterator itr = this->begin();
			for (; itr != this->end(); itr++)
				if (predicate(*itr))
					return itr;

			return itr;
		}

		bool includes(const T & searched) const
		{
			for (const auto & element : *this)
				if (element == searched)
					return true;

			return false;
		}

		Collection & concat(const Collection & other)
		{
			this->insert(this->end(), other.begin(), other.end());
			return *this;
		}

		Collection concatenated(const Collection & other) const
		{
			return Collection(*this).concat(other);
		}

	};
	 
	template<typename T> using Vector = Collection<T, WrapVector>;
	template<typename T> using List   = Collection<T, WrapList>;
	
	using CString = Collection<char, WrapString>;
	using WString = Collection<wchar_t, WrapString>;

}