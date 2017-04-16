#pragma once

namespace aogl {

	class buffer;

	namespace attribute_type {
		enum type_t {
			float_type = GL_FLOAT,
			double_type = GL_DOUBLE,
			byte_type = GL_BYTE,
			unsigned_byte_type = GL_UNSIGNED_BYTE,
			short_type = GL_SHORT,
			unsigned_short_type = GL_UNSIGNED_SHORT,
			int_type = GL_INT,
			unsigned_int_type = GL_UNSIGNED_INT,
			half_float = GL_HALF_FLOAT,
			fixed_type = GL_FIXED
		};
	}

	class vertex_array {
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
		 * \fn vertex_array vertex_array()
		 * \brief crate a empty vertex array
		 */
		vertex_array();

		/**
		* \fn vertex_array vertex_array(const buffer& other)
		* \brief copy constructor
		*/
		vertex_array(const vertex_array& other);

		/**
		* \fn ~vertex_array
		* \brief destructor
		*/
		~vertex_array();

		/**
		* \fn const vertex_array& operator=(const vertex_array& other)
		* \brief assignment operator overload
		*/
		const vertex_array& operator=(const vertex_array& other);

		/**
		 * \fn void bind(buffer b, uint attribute, int attribute_size, uint type, uchar normalized, int stride, int offset) const;
		 * \brief bind a buffer to the vertex array
		 * 
		 * \warning may throw an error if buffer is not of type array_buffer 
		 */
		void bind(buffer& b, uint attribute, int attribute_size, attribute_type::type_t type, bool normalized, int stride, int offset) const;

		/**
		 *	void bind_element(buffer& b) const;
		 *	\brief bind an element buffer to the vertex_array
		 *	
		 *	\warning may throw an error if buffer is not of type element_buffer
		 */
		void bind_element(buffer& b) const;

		/**
		 * \fn operator uint() const
		 * \brief implicit conversion operator
		 */
		operator uint() const;
	};

}