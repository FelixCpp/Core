#pragma once

#include <memory>

#include <Core/System/Delegate.hpp>
#include <Core/System/StaticObject.hpp>

namespace Core
{

	struct Eventbinder : StaticObject {

		template<typename TReturnType, typename ... TArgs>
		static std::shared_ptr<Delegate<TReturnType(TArgs ...)>> Bind(TReturnType(*function)(TArgs ...))
		{
			return std::make_shared<Delegate<TReturnType(TArgs...)>>(function);
		}

		template<typename TReturnType, class TInvocator, typename ... TArgs>
		static std::shared_ptr<Delegate<TReturnType(TArgs ...), TInvocator>> Bind(TReturnType(TInvocator:: * method)(TArgs ...), TInvocator * invocator)
		{
			return std::make_shared<Delegate<TReturnType(TArgs...), TInvocator>>(method, invocator);
		}

		template<typename TReturnType, class TInvocator, typename ... TArgs>
		static std::shared_ptr<Delegate<TReturnType(TArgs ...), TInvocator>> Bind(TReturnType(TInvocator:: * method)(TArgs ...) const, const TInvocator * invocator)
		{
			const auto passMethod = std::remove_cv_t<TReturnType(TInvocator:: *)(TArgs ...)>(method);
			auto passInvocator = std::remove_cv_t<TInvocator *>(invocator);

			return std::make_shared<Delegate<TReturnType(TArgs...), TInvocator>>(passMethod, passInvocator);
		}

		template<typename TReturnType, class TInvocator, typename ... TArgs>
		static std::shared_ptr<Delegate<TReturnType(TArgs ...), TInvocator>> Bind(TReturnType(TInvocator:: * method)(TArgs ...) volatile, volatile TInvocator * invocator)
		{
			const auto passMethod = std::remove_cv_t<TReturnType(TInvocator:: *)(TArgs ...)>(method);
			auto passInvocator = std::remove_cv_t<TInvocator *>(invocator);

			return std::make_shared<Delegate<TReturnType(TArgs...), TInvocator>>(passMethod, passInvocator);
		}

		template<typename TReturnType, class TInvocator, typename ... TArgs>
		static std::shared_ptr<Delegate<TReturnType(TArgs ...), TInvocator>> Bind(TReturnType(TInvocator:: * method)(TArgs ...) const volatile, const volatile TInvocator * invocator)
		{
			const auto passMethod = std::remove_cv_t<TReturnType(TInvocator:: *)(TArgs ...)>(method);
			auto passInvocator = std::remove_cv_t<TInvocator *>(invocator);

			return std::make_shared<Delegate<TReturnType(TArgs...), TInvocator>>(passMethod, passInvocator);
		}

	};

}