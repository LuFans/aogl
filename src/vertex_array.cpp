#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/vertex_array.hpp"

#include "../include/aogl/buffer.hpp"
#include "../include/aogl/error.hpp"

namespace aogl {
	
	garbage_collector<uint> vertex_array::gc = garbage_collector<uint>([](uint handle) {
		glDeleteVertexArrays(1, &handle);
	});
	

	vertex_array::vertex_array() {
		glGenVertexArrays(1, &handle);
		gc.add(handle);
	}

	vertex_array::vertex_array(const vertex_array& other) {
		gc.copy(other.handle, handle);
	}

	vertex_array::~vertex_array() {
		gc.destroy(handle);
	}

	const vertex_array& vertex_array::operator=(const vertex_array& other) {
		gc.destroy(handle);
		gc.copy(other.handle, handle);
		return *this;
	}

	void vertex_array::bind(buffer& b,uint attribute, int attribute_size, attribute_type::type_t type, bool normalized, int stride, int offset) const {
		if (b.type != buffer_type::array) throw vertex_array_exception("buffer must have type: array");
		glBindVertexArray(handle);
		glBindBuffer(GL_ARRAY_BUFFER, b);
		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute, attribute_size, type, normalized == true ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<void*>(offset));
	}

	void vertex_array::bind_element(buffer& b) const {
		if (b.type != buffer_type::element_array) throw vertex_array_exception("buffer must have type: element_array");
		glBindVertexArray(handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	vertex_array::operator uint() const { return handle; }
}
