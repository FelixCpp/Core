#pragma once

namespace Core
{

	struct StaticObject {
		StaticObject() = delete;
		StaticObject(const StaticObject &) = delete;
		StaticObject(StaticObject &&) = delete;
		StaticObject & operator=(const StaticObject &) = delete;
		StaticObject & operator=(StaticObject &&) = delete;
	};

}