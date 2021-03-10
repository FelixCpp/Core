#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/StaticObject.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <list>
#include <memory>

namespace Core
{

	/// <summary>
	/// How to use this class:
	/// 
	/// <example>
	///		use Event<EventArgs> instead of Event<>.
	///		To raise an event passing no data through do the following:
	/// 
	///		<code>
	///			Event<EventArgs> myEvent;
	///			myEvent(this, EventArgs::Empty);
	///		</code>
	/// </example>
	/// </summary>
	class EventArgs {
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		EventArgs() = default;

		/// <summary>
		/// Virtual default destructor
		/// </summary>
		virtual ~EventArgs() = default;

		/// <summary>
		/// No data
		/// </summary>
		static const EventArgs Empty;

	};

	/// <summary>
	/// Initialize Empty EventArgs
	/// </summary>
	const EventArgs EventArgs::Empty;

	/// <summary>
	/// This class is the base for all delegate types
	/// like FunctionDelegate & MethodDelegate
	/// </summary>
	template<typename ReturnType, typename ... Arguments>
	class Delegate {
	public:

		/// <summary>
		/// Invokes the delegate using the arguments passed
		/// </summary>
		/// <param name="arguments">the arguments to use when raising the callable to execute</param>
		/// <returns>the invocation result from the callable function/method</returns>
		virtual ReturnType Invoke(Arguments ... arguments) const = 0;

		/// <summary>
		/// Checks wether the given delegate equals its comperator
		/// </summary>
		/// <param name="delegate">the delegate to compare against</param>
		bool Equals(const Delegate & delegate) const
		{
			return
			{
				this->TypeEquals(delegate) &&
				this->DataEquals(delegate)
			};
		}

	protected:

		/// <summary>
		/// Checks wether the types are equal or not.
		/// The method will return false if a FunctionDelegate
		/// gets compared with a MethodDelegate.
		/// </summary>
		/// <param name="delegate">the delegate to compare against</param>
		virtual bool TypeEquals(const Delegate & delegate) const = 0;

		/// <summary>
		/// Checks wether the data between the objects are the same or not.
		/// Example: both delgates holds the same function in their internal
		/// storage.
		/// </summary>
		/// <param name="delegate">the delegate to compare against</param>
		virtual bool DataEquals(const Delegate & delegate) const = 0;

	};

	/// <summary>
	/// Forward declaration of the Delegate types
	/// </summary>
	template<typename> class FunctionDelegate;	//< Delegate that stores a function
	template<typename> class MethodDelegate;	//< Delegate that stored a member-function / method

	template<typename ReturnType, typename ... Arguments>
	class FunctionDelegate<ReturnType(*)(Arguments ...)> : public Delegate<ReturnType, Arguments ...> {
	public:

		/// <summary>
		/// The function-signature
		/// </summary>
		using Signature = ReturnType(*)(Arguments ...);

		/// <summary>
		/// Constructor which creates the delegate using the passed-in
		/// function
		/// </summary>
		/// <param name="function">the function to store and invoke later on</param>
		FunctionDelegate(Signature function = nullptr) :
			function(function)
		{
		}

		/// <summary>
		/// Invokes the delegate using the arguments passed
		/// </summary>
		/// <param name="arguments">the arguments to use when raising the callable to execute</param>
		/// <returns>the invocation result from the callable function/method</returns>
		virtual ReturnType Invoke(Arguments ... arguments) const override
		{
			return this->function(std::forward<Arguments>(arguments)...);
		}

	protected:

		/// <summary>
		/// Checks wether the types are equal or not.
		/// The method will return false if a FunctionDelegate
		/// gets compared with a MethodDelegate.
		/// </summary>
		/// <param name="delegate">the delegate to compare against</param>
		virtual bool TypeEquals(const Delegate<ReturnType, Arguments ...> & delegate) const override
		{
			return dynamic_cast<const FunctionDelegate<ReturnType(*)(Arguments ...)> *>(&delegate) != nullptr;
		}

		/// <summary>
		/// Checks wether the data between the objects are the same or not.
		/// Example: both delgates holds the same function in their internal
		/// storage.
		/// </summary>
		/// <param name="delegate">the delegate to compare against</param>
		virtual bool DataEquals(const Delegate<ReturnType, Arguments ...> & delegate) const override
		{
			// we can do a static cast here, because this function won't be executed if the types are not equal.
			// we do this because static_cast is much faster than dynamic_cast
			const auto other = static_cast<const FunctionDelegate<ReturnType(*)(Arguments ...)> *>(&delegate);

			// we don't need to check for nullptr (look above)
			return this->function == other->function;
		}

	private:

		/// <summary>
		/// The internally stored function to execute through Invoke(...)
		/// </summary>
		Signature function;

	};

	template<typename ReturnType, class MethodOwner, typename ... Arguments>
	class MethodDelegate<ReturnType(MethodOwner:: *)(Arguments ...)> : public Delegate<ReturnType, Arguments ...> {
	public:

		/// <summary>
		/// The method-signature
		/// </summary>
		using Signature = ReturnType(MethodOwner:: *)(Arguments ...);

		/// <summary>
		/// Constructor which creates the delegate using the passed-in
		/// method / method owner
		/// </summary>
		/// <param name="signature">the method to invoke later on</param>
		/// <param name="owner">the owner that raises the method</param>
		MethodDelegate(Signature method, MethodOwner & owner) :
			method(method), owner(owner)
		{
		}

		/// <summary>
		/// Invokes the delegate using the arguments passed
		/// </summary>
		/// <param name="arguments">the arguments to use when raising the callable to execute</param>
		/// <returns>the invocation result from the callable function/method</returns>
		virtual ReturnType Invoke(Arguments ... arguments) const override
		{
			return (this->owner.*this->method)(std::forward<Arguments>(arguments)...);
		}

	protected:

		/// <summary>
		/// Checks wether the types are equal or not.
		/// The method will return false if a FunctionDelegate
		/// gets compared with a MethodDelegate.
		/// </summary>
		/// <param name="delegate">the delegate to compare against</param>
		virtual bool TypeEquals(const Delegate<ReturnType, Arguments ...> & delegate) const override
		{
			return dynamic_cast<const MethodDelegate<ReturnType(MethodOwner:: *)(Arguments ...)> *>(&delegate) != nullptr;
		}

		/// <summary>
		/// Checks wether the data between the objects are the same or not.
		/// Example: both delgates holds the same function in their internal
		/// storage.
		/// </summary>
		/// <param name="delegate">the delegate to compare against</param>
		virtual bool DataEquals(const Delegate<ReturnType, Arguments ...> & delegate) const override
		{
			// we can do a static cast here, because this function won't be executed if the types are not equal.
			// we do this because static_cast is much faster than dynamic_cast
			const auto other = static_cast<const MethodDelegate<ReturnType(MethodOwner:: *)(Arguments ...)> *>(&delegate);

			// we don't need to check for nullptr (look above)
			return this->method == other->method && &this->owner == &other->owner;
		}

	private:

		Signature method;		//< The method to call
		MethodOwner & owner;	//< The owner who calls the method

	};

	/// <summary>
	/// Use this class to create an EventHandler which
	/// gets added to an existing Event
	/// 
	///		<example>
	///			myEvent += EventHandler::Bind(&MyFunction);
	///			myEvent += EventHandler::Bind(&MyClass::MyMethod, &myClassInstnace);
	///		</example>
	/// </summary>
	struct EventBinder : StaticObject {

		/// <summary>
		/// Methods to bind a function / method
		/// </summary>

		/// <summary>
		/// Used to create an EventHandler holding a function
		/// </summary>
		/// <param name="function">the function to hold</param>
		/// <returns>a new EventHandler</returns>
		template<typename ReturnType, typename ... Arguments>
		static Delegate<ReturnType, Arguments ...> * Bind(ReturnType(*function)(Arguments ...))
		{
			return new FunctionDelegate<ReturnType(*)(Arguments ...)>(function);
		}

		/// <summary>
		/// Used to create an EventHandler holding a member function (method) +
		/// instance of the class containing the method to execute it when raising
		/// the event.
		/// </summary>
		/// <param name="method">the method to hold</param>
		/// <param name="owner">the owner of this method</param>
		/// <returns>a new EventHandler</returns>
		template<typename ReturnType, class MethodOwner, typename ... Arguments>
		static Delegate<ReturnType, Arguments ...> * Bind(ReturnType(MethodOwner:: * method)(Arguments ...), MethodOwner & owner)
		{
			return new MethodDelegate<ReturnType(MethodOwner:: *)(Arguments ...)>(method, owner);
		}

		/// <summary>
		/// Used to create an EventHandler holding a member function (method) +
		/// instance of the class containing the method to execute it when raising
		/// the event.
		/// </summary>
		/// <param name="method">the method to hold</param>
		/// <param name="owner">the owner of this method</param>
		/// <returns>a new EventHandler</returns>
		template<typename ReturnType, class MethodOwner, typename ... Arguments>
		static Delegate<ReturnType, Arguments ...> * Bind(ReturnType(MethodOwner:: * method)(Arguments ...) const, const MethodOwner & owner)
		{
			return new MethodDelegate<ReturnType(MethodOwner:: *)(Arguments ...)>(
				std::remove_const_t<ReturnType(MethodOwner:: *)(Arguments ...)>(method),
				std::remove_const_t<MethodOwner &>(owner)
			);
		}

		/// <summary>
		/// Used to create an EventHandler holding a member function (method) +
		/// instance of the class containing the method to execute it when raising
		/// the event.
		/// </summary>
		/// <param name="method">the method to hold</param>
		/// <param name="owner">the owner of this method</param>
		/// <returns>a new EventHandler</returns>
		template<typename ReturnType, class MethodOwner, typename ... Arguments>
		static Delegate<ReturnType, Arguments ...> * Bind(ReturnType(MethodOwner:: * method)(Arguments ...) volatile, volatile MethodOwner & owner)
		{
			return new MethodDelegate<ReturnType(MethodOwner:: *)(Arguments ...)>(
				std::remove_volatile_t<ReturnType(MethodOwner:: *)(Arguments ...)>(method),
				std::remove_volatile_t<MethodOwner &>(owner)
			);
		}

		/// <summary>
		/// Used to create an EventHandler holding a member function (method) +
		/// instance of the class containing the method to execute it when raising
		/// the event.
		/// </summary>
		/// <param name="method">the method to hold</param>
		/// <param name="owner">the owner of this method</param>
		/// <returns>a new EventHandler</returns>
		template<typename ReturnType, class MethodOwner, typename ... Arguments>
		static Delegate<ReturnType, Arguments ...> * Bind(ReturnType(MethodOwner:: * method)(Arguments ...) const volatile, const volatile MethodOwner & owner)
		{
			return new MethodDelegate<ReturnType(MethodOwner:: *)(Arguments ...)>(
				std::remove_cv_t<ReturnType(MethodOwner:: *)(Arguments ...)>(method),
				std::remove_cv_t<MethodOwner &>(owner)
			);
		}

	};

	/// <summary>
	/// Forward declaration for an Event
	/// </summary>
	template<typename> class GenericEvent;

	/// <summary>
	/// This class should be used just like C#-Events.
	/// </summary>
	template<typename ReturnType, typename ... Arguments>
	class GenericEvent<ReturnType(Arguments ...)> {
	public:

		using DelegateRawPtr = Delegate<ReturnType, Arguments ...> *;
		using DelegateSharedPtr = std::shared_ptr<Delegate<ReturnType, Arguments ...>>;

	public:

		/// <summary>
		/// The default constructor
		/// </summary>
		GenericEvent() :
			delegates()
		{
		}

		/// <summary>
		/// Used to add an eventhandler
		/// </summary>
		/// <param name="handler">the new eventhandler to add</param>
		/// <returns>the updated instance of the event the operator was called on</returns>
		GenericEvent & operator += (DelegateRawPtr delegate)
		{
			this->delegates.emplace_back(delegate);
			return *this;
		}

		/// <summary>
		/// Used to remove an eventhandler
		/// </summary>
		/// <param name="handler">the eventhandler to remove</param>
		/// <returns>the updated instance of the event the operator was called on</returns>
		GenericEvent & operator -= (DelegateRawPtr delegate)
		{
			const auto itr = std::find_if(this->delegates.begin(), this->delegates.end(), [&](const DelegateSharedPtr & current) -> bool
			{
				return current->Equals(*delegate);
			});

			if (itr != this->delegates.end())
			{
				this->delegates.erase(itr);
			}

			return *this;
		}

		/// <summary>
		/// To be used to raise the event
		/// </summary>
		ReturnType Invoke(Arguments ... arguments) const
		{
			if (this->delegates.size() >= 1)
			{
				for (const DelegateSharedPtr & pDelegate : this->delegates)
				{
					if (pDelegate.get() != this->delegates.back().get())
					{
						DelegateRawPtr delegate = pDelegate.get();
						delegate->Invoke(std::forward<Arguments>(arguments)...);
					}
				}
			}

			// Return the result of the last invocation
			return this->delegates.back()->Invoke(std::forward<Arguments>(arguments)...);
		}

		/// <summary>
		/// Removes all subscribers from the event
		/// </summary>
		/// <returns>the updated instance of the event the method was called on</returns>
		GenericEvent & Clear()
		{
			this->delegates.clear();
			return *this;
		}

		/// <summary>
		/// Checks if the event has subscribers
		/// </summary>
		operator bool() const
		{
			return (*this != nullptr);
		}

		/// <summary>
		/// Checks if the event has no subscribers
		/// </summary>
		bool operator == (std::nullptr_t) const
		{
			return this->delegates.empty();
		}

		/// <summary>
		/// Checks if the event has subscribers
		/// </summary>
		bool operator != (std::nullptr_t) const
		{
			return !(*this == nullptr);
		}

	private:

		/// <summary>
		/// Internally stored delegates
		/// </summary>
		std::list<DelegateSharedPtr> delegates;

	};

	template<class FirstArgument = EventArgs, typename ... Arguments>
	using Event = GenericEvent<void(void *, FirstArgument, Arguments ...)>;

}