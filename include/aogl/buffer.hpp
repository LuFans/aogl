#pragma once
#include "mesh.hpp"

namespace aogl {
	
	namespace buffer_type {
		enum type_t {
			undefined = 0,
			array = GL_ARRAY_BUFFER,
			element_array = GL_ELEMENT_ARRAY_BUFFER,
			uniform = GL_UNIFORM_BUFFER
		};
	}

	namespace buffer_usage {
		enum usage_t {
			stream_draw = GL_STREAM_DRAW,
			stream_read = GL_STREAM_READ,
			stream_copy = GL_STREAM_COPY,
			static_draw = GL_STATIC_DRAW,
			static_read = GL_STATIC_READ,
			static_copy = GL_STATIC_COPY,
			dynamic_draw = GL_DYNAMIC_DRAW,
			dynamic_read = GL_DYNAMIC_READ,
			dynamic_copy = GL_DYNAMIC_COPY
		};
	}


	class buffer_stream {
		
		std::vector<uchar> data;

		friend class buffer;

		/**
		 * \fn buffer_stream buffer_stream()
		 * \brief make class only constructable from buffer class
		 */
		buffer_stream() = default;

		/**
		 * \fn void push_back(uchar* byte, size_t size)
		 * \brief append data to the data vector
		 * 
		 * use case: float i = 0.0f; push_back(reinterpret_cast<uchar*>(&i), sizeof(i));
		 */
		void push_back(uchar* byte, size_t size);

	public:

		/**
		 * \brief all the operator overload to push back data to the stream
		 */

		buffer_stream& operator<<(float value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }
		buffer_stream& operator<<(char value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }
		buffer_stream& operator<<(short value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }
		buffer_stream& operator<<(int value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }
		buffer_stream& operator<<(uchar value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }
		buffer_stream& operator<<(ushort value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }
		buffer_stream& operator<<(uint value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }

		buffer_stream& operator<<(glm::vec2 value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }
		buffer_stream& operator<<(glm::vec3 value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }
		buffer_stream& operator<<(glm::vec4 value) { push_back(reinterpret_cast<uchar*>(&value), sizeof(value));  return *this; }

	};

	class buffer {
		/**
		 * Garbage Collector
		 */
		static garbage_collector<uint> gc;

		/**
		 * \var uint handle
		 * \brief internal opengl handle
		 */
		uint handle;

	public:

		/**
		* \var buffer_type::type_t type
		* \brief the type of the buffer (see buffer_type::type_t for options)
		*/
		buffer_type::type_t type;

		/**
		 * \fn buffer buffer()
		 * \brief default constructor
		 * 
		 * creates an empty buffer
		 */
		buffer();

		/**
		 * \fn buffer buffer(const buffer& other)
		 * \brief copy constructor
		 */
		buffer(const buffer& other);

		/**
		 * \fn buffer buffer(buffer_type::type_t _type)
		 * \brief Creates an empty buffer with defined buffer type
		 */
		explicit buffer(buffer_type::type_t _type);

		/**
		 * \fn buffer buffer(buffer_type::type_t _type, void* data, size_t size, buffer_usage::usage_t usage)
		 * \brief create a buffer and fill it with data
		 */
		buffer(buffer_type::type_t _type, const void* data, size_t size, buffer_usage::usage_t usage);

		/**
		 * \fn buffer(buffer_type::type_t _type, const mesh& mesh, buffer_usage::usage_t usage, std::function<void(const vertex&, buffer_stream&)> converter)
		 * \brief fill the buffer from data loaded from a mesh
		 */
		buffer(buffer_type::type_t _type, const mesh& mesh, buffer_usage::usage_t usage, std::function<void(const vertex&, buffer_stream&)> converter);

		/**
		 * \fn ~buffer
		 * \brief destructor
		 */
		~buffer();

		/**
		 * \fn const buffer& operator=(const buffer& other)
		 * \brief assignment operator overload
		 */
		const buffer& operator=(const buffer& other);

		/**
		 * \fn void data(void* data, size_t size, buffer_usage::usage_t usage) const
		 * \brief upload data to the buffer
		 * 
		 * \warning may throw buffer_exception if buffer type is undefined
		 */
		void data(const void* data, size_t size, buffer_usage::usage_t usage) const;

		/**
		* \fn void sub_data(void* data, size_t offset, size_t length) const
		* \brief upload sub data to the buffer
		*
		* \warning may throw buffer_exception if buffer type is undefined
		*/
		void sub_data(void* data, size_t offset, size_t length) const;

		/**
		* \fn void get_data(void* data, size_t offset, size_t length) const
		* \brief store part of the buffer to the data pointer
		*
		* \warning may throw buffer_exception if buffer type is undefined
		*/
		void get_data(void* data, size_t offset, size_t length) const;

		/**
		 * \fn operator uint() const
		 * \brief implicit conversion operator
		 */
		operator uint() const { return handle; }
	};

	/**
	 * helper functions to create buffers from STL Containers
	 */

	template <typename T>
	buffer create_buffer(buffer_type::type_t _type, const std::vector<T>& vec, buffer_usage::usage_t usage) {
		return buffer(_type, vec.data(), vec.size() * sizeof(T), usage);
	}

	template <typename T, size_t S>
	buffer create_buffer(buffer_type::type_t _type, const std::array<T, S>& array, buffer_usage::usage_t usage) {
		return buffer(_type, array.data(), array.size() * sizeof(T), usage);
	}

}
