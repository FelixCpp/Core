// 
// Error.cpp
// Core
// 
// Created by Felix Busch on 15.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
//

#include <Core/System/Error.hpp>

namespace Core
{
	namespace Internal
	{
		////////////////////////////////////////////////////////////
		/// \brief This class will be used as the default streambuf
		///		   of Core::Error(). It outputs to stderr by default
		///		   (to keep the default behavior)
		/// 
		////////////////////////////////////////////////////////////
		class DefaultErrorStreamBuffer final : public std::basic_streambuf<char>
		{
		public:
			
			////////////////////////////////////////////////////////////
			/// \brief Default constructor for allocation
			/// 
			////////////////////////////////////////////////////////////
			DefaultErrorStreamBuffer()
			{
				// allocate the write buffer
				static constexpr int Size = 64;
				auto* buffer = new char[Size];
				this->setp(buffer, buffer + Size);
			}

			////////////////////////////////////////////////////////////
			/// \brief Default destructor for cleanup
			/// 
			////////////////////////////////////////////////////////////
			virtual ~DefaultErrorStreamBuffer() override
			{
				// Synchronize
				sync();

				// delete the write buffer
				delete[] this->pbase();
			}

		protected:
			
			virtual int_type overflow(int_type character) override
			{
				if(character != EOF)
				{
					if (this->pptr() != this->epptr())
					{
						// valid character
						return this->sputc((char)character);
					}

					// not enough space in the buffer: synchronize output and try again
					sync();
					return overflow(character);
				}

				// invalid character: synchronize output
				return sync();
			}
			
			virtual int sync() override
			{
				// check if there is something into the write buffer
				if(this->pbase() != this->pptr())
				{
					// print the contents of the write buffer into the standard error output
					const auto size = (size_t)(this->pptr() - this->pbase());
					fwrite(this->pbase(), sizeof(char), size, stderr);

					// reset the pointer position to the beginning of the write buffer
					this->setp(this->pbase(), this->epptr());
				}

				return 0;
			}
		};
	}

	////////////////////////////////////////////////////////////
	std::ostream& Err()
	{
		static Internal::DefaultErrorStreamBuffer buffer;
		static std::ostream stream(&buffer);
		return stream;
	}
}