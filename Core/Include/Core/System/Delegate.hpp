#pragma once

#include <Core/System/DelegateBase.hpp>

namespace Core
{

	template<typename, class = std::nullptr_t>
	class Delegate;

	template<typename TReturnType, typename ... TArgs>
	class Delegate<TReturnType(TArgs...)> : public DelegateBase<TReturnType(TArgs...)> {
	public:

		typedef TReturnType(*FunctionType)(TArgs ...);

	public:

		inline Delegate(FunctionType function) :
			function(function)
		{
		}

		inline virtual bool operator==(std::nullptr_t) const override
		{
			return this->function == nullptr;
		}

		inline virtual TReturnType Invoke(const TArgs & ... arguments) const override
		{
			std::invoke(this->function, std::forward<decltype(arguments)>(arguments)...);
		}

		inline FunctionType GetFunction() const { return this->function; }

	private:

		inline virtual bool TypeEquals(const DelegateBase<TReturnType(TArgs...)> & other) const override
		{
			return dynamic_cast<const Delegate<TReturnType(TArgs...)> *>(&other) != nullptr;
		}

		inline virtual bool DataEquals(const DelegateBase<TReturnType(TArgs...)> & other) const override
		{
			const Delegate<TReturnType(TArgs...)> * delegate = dynamic_cast<const Delegate<TReturnType(TArgs...)> *>(&other);
			if (delegate != nullptr)
			{
				return this->function == delegate->function;
			}

			return false;
		}

	private:

		FunctionType function;

	};

	template<typename TReturnType, class TInvocator, typename ... TArgs>
	class Delegate<TReturnType(TArgs...), TInvocator> : public DelegateBase<TReturnType(TArgs...)> {
	public:

		typedef TReturnType(TInvocator:: * MethodType)(TArgs ...);

	public:

		inline explicit Delegate(MethodType method, TInvocator * invocator) :
			method(method),
			invocator(invocator)
		{
		}

		inline virtual bool operator==(std::nullptr_t) const override
		{
			return this->method == nullptr || this->invocator == nullptr;
		}

		inline virtual TReturnType Invoke(const TArgs & ... arguments) const override
		{
			std::invoke(this->method, *this->invocator, std::forward<decltype(arguments)>(arguments)...);
		}

		inline MethodType GetMethod() const { return this->method; }
		inline TInvocator * GetOwner() const { return this->invocator; }

	private:

		inline virtual bool TypeEquals(const DelegateBase<TReturnType(TArgs...)> & other) const override
		{
			return dynamic_cast<const Delegate<TReturnType(TArgs...), TInvocator> *>(&other) != nullptr;
		}

		inline virtual bool DataEquals(const DelegateBase<TReturnType(TArgs...)> & other) const override
		{
			const Delegate<TReturnType(TArgs...), TInvocator> * delegate = dynamic_cast<const Delegate<TReturnType(TArgs...), TInvocator> *>(&other);

			if (delegate != nullptr)
			{
				return
				{
					this->invocator == delegate->invocator &&
					this->method == delegate->method
				};
			}

			return false;
		}

	private:

		MethodType method;
		TInvocator * invocator;

	};

}