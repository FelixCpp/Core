// 
// PollEventSystem.hpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <queue>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Event system that uses polling
	/// 
	////////////////////////////////////////////////////////////
	template<typename TEvent>
	class PollEventSystem
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		virtual ~PollEventSystem() = default;

		////////////////////////////////////////////////////////////
		/// \brief Adds the event by copying the data
		/// 
		////////////////////////////////////////////////////////////
		void PushEvent(const TEvent& event)
		{
			events.push(event);
		}

		////////////////////////////////////////////////////////////
		/// \brief Adds the event by moving the data
		/// 
		////////////////////////////////////////////////////////////
		void PushEvent(TEvent&& event)
		{
			events.emplace(event);
		}
		
		////////////////////////////////////////////////////////////
		/// \brief Iterate through the queue and fill the \a event
		///		   parameter if there is an event to process
		/// 
		////////////////////////////////////////////////////////////
		bool PollEvents(TEvent& event, bool queueEvents = true)
		{
			// queue up events if there are no events left
			if (!HasEvents())
			{
				QueueEvents();
			}

			if(HasEvents())
			{
				// copy event into new buffer
				const TEvent currentEvent = events.front();
				events.pop();

				// apply filtering
				if(FilterEvent(currentEvent))
				{
					// copy the event
					event = currentEvent;
					return true;
				}
			}

			// no event to process
			return false;
		}

		////////////////////////////////////////////////////////////
		/// \brief Check whether there are events stored in the queue or not
		///
		///	\return True if there are events in the queue, false otherwise
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool HasEvents() const
		{
			return !events.empty();
		}

	protected:

		////////////////////////////////////////////////////////////
		/// \brief Queues the events based on external sources
		///
		///	This method can be overwritten to queue up the events
		///	catched by another system.
		/// 
		////////////////////////////////////////////////////////////
		virtual void QueueEvents()
		{
			// nothing to do
		}

		////////////////////////////////////////////////////////////
		/// \brief Filter the event based on its data
		///
		///	\return True if the event should be published, false otherwise
		/// 
		////////////////////////////////////////////////////////////
		virtual bool FilterEvent(const TEvent& event)
		{
			// forward the event
			return true;
		}

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		std::queue<TEvent> events; //!< Events to handle when dispatching

	};

}