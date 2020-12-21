#pragma once

#include <type_traits>

namespace Core
{

	template<typename>
	class DelegateBase;

	template<typename TReturnType, typename ... TArgs>
	class DelegateBase<TReturnType(TArgs...)> {
	public:

		inline void operator()(const TArgs & ... arguments) const
		{
			return this->invoke(std::forward<decltype(arguments)>(arguments)...);
		}

		inline bool operator!=(std::nullptr_t) const
		{
			return !(*this == nullptr);
		}

		inline bool equals(const DelegateBase & other) const
		{
			return this->typeEquals(other) && this->dataEquals(other);
		}

		inline virtual bool operator==(std::nullptr_t) const = 0;
		inline virtual TReturnType invoke(const TArgs & ... arguments) const = 0;

	protected:

		virtual bool typeEquals(const DelegateBase<TReturnType(TArgs...)> & other) const = 0;
		virtual bool dataEquals(const DelegateBase<TReturnType(TArgs...)> & other) const = 0;

	};

}