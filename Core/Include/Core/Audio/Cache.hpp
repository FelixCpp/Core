#pragma once

#include <string>
#include <unordered_map>

#include <Core/System/Datatypes.hpp>

namespace Core
{

	template<typename TKey, typename TValue>
	class Cache {
	public:

		/* default constructor */
		Cache() :
			storage()
		{ }

		/*
		* returns true if the key is already inserted
		*/
		bool Contains(const TKey & key) const
		{
			return storage.find(key) != this->storage.end();
		}

		/*
		* inserts a new value into the dictionary	
		*/
		void Set(const TKey & key, const TValue & value)
		{
			this->storage[key] = value;
		}

		/*
		* simply returns the value behind the key
		*/
		TValue & Get(const TKey & key)
		{
			return this->storage.at(key);
		}

	private:

		/* unordered_map as datastructure for caching */
		std::unordered_map<TKey, TValue> storage;

	};

}