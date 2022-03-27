// 
// EventPublisher.hpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/IEventListener.hpp>
#include <Core/System/PollEventSystem.hpp>

#include <vector>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Generic event publisher
	/// 
	////////////////////////////////////////////////////////////
	template<typename TEvent>
	class EventPublisher : PollEventSystem<TEvent>
	{
	public:
		////////////////////////////////////////////////////////////
		/// \brief Default destructor
		/// 
		////////////////////////////////////////////////////////////
		virtual ~EventPublisher() override = default;

		////////////////////////////////////////////////////////////
		/// \brief Add an event listener to the subscribers
		///
		///	\param listener The event listener to add
		/// 
		////////////////////////////////////////////////////////////
		void AddEventListener(IEventListener<TEvent>& listener)
		{
			subscribers.push_back(&listener);
		}

		////////////////////////////////////////////////////////////
		/// \brief Remove an event listener from the subscribers
		///
		///	\param listener The event listener to remove
		/// 
		////////////////////////////////////////////////////////////
		void RemoveEventListener(const IEventListener<TEvent>& listener)
		{
			// search the listener and remove him from the list
			if(const auto itr = std::ranges::find(subscribers, &listener); itr != subscribers.end())
			{
				subscribers.erase(itr);
			}
		}

		////////////////////////////////////////////////////////////
		/// \brief Remove all event listeners from the list
		/// 
		////////////////////////////////////////////////////////////
		void RemoveAllEventListeners()
		{
			subscribers.clear();
		}

		////////////////////////////////////////////////////////////
		/// \brief Adopt the PushEvent method
		/// 
		////////////////////////////////////////////////////////////
		using PollEventSystem<TEvent>::PushEvent;

		////////////////////////////////////////////////////////////
		/// \brief Notify all subscribers
		/// 
		////////////////////////////////////////////////////////////
		void DispatchEvents()
		{
			// declare event buffer
			TEvent event = {};

			// dispatch every queued event
			while (this->PollEvents(event))
			{
				// notify each subscriber
				for(IEventListener<TEvent>* subscriber : subscribers)
				{
					subscriber->OnEvent(event);
				}
			}
		}

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		std::vector<IEventListener<TEvent>*> subscribers; //!< The subscriber to notify

	};

}