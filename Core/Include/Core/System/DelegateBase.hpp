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
			return this->Invoke(std::forward<decltype(arguments)>(arguments)...);
		}

		inline bool operator!=(std::nullptr_t) const
		{
			return !(*this == nullptr);
		}

		inline bool Equals(const DelegateBase & other) const
		{
			return this->TypeEquals(other) && this->DataEquals(other);
		}

		inline virtual bool operator==(std::nullptr_t) const = 0;
		inline virtual TReturnType Invoke(const TArgs & ... arguments) const = 0;

	protected:

		virtual bool TypeEquals(const DelegateBase<TReturnType(TArgs...)> & other) const = 0;
		virtual bool DataEquals(const DelegateBase<TReturnType(TArgs...)> & other) const = 0;

	};

}