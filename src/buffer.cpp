#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/buffer.hpp"
#include "../include/aogl/error.hpp"

namespace aogl {

	garbage_collector<uint> buffer::gc = garbage_collector<uint>([](uint handle) {
		glDeleteBuffers(1, &handle);
	});

	void buffer_stream::push_back(uchar* bytes, size_t size) {
		for (auto i = 0; i < static_cast<int>(size); i++) {
			data.push_back(bytes[i]);
		}
	}

	buffer::buffer() {
		glGenBuffers(1, &handle);
		gc.add(handle);
		type = buffer_type::undefined;
	}

	buffer::buffer(const buffer& other) {
		type = other.type;
		gc.copy(other.handle, handle);
	}

	buffer::buffer(buffer_type::type_t _type) {
		glGenBuffers(1, &handle);
		gc.add(handle);
		type = _type;
	}

	buffer::buffer(buffer_type::type_t _type, const void* data, size_t size, buffer_usage::usage_t usage) {
		glGenBuffers(1, &handle);
		gc.add(handle);
		type = _type;
		this->data(data, size, usage);
	}

	buffer::buffer(buffer_type::type_t _type, const mesh& mesh, buffer_usage::usage_t usage, std::function<void(const vertex&, buffer_stream&)> converter) {
		glGenBuffers(1, &handle);
		gc.add(handle);
		type = _type;
		
		buffer_stream stream;

		for (auto& v : mesh.vertices) {
			converter(v, stream);
		}

		this->data(stream.data.data(), stream.data.size() * sizeof(uchar), usage);
	}

	buffer::~buffer() {
		gc.destroy(handle);
	}

	const buffer& buffer::operator=(const buffer& other) {
		gc.destroy(handle);
		gc.copy(other.handle, handle);
		this->type = other.type;
		return *this;
	}

	void buffer::data(const void* data, size_t size, buffer_usage::usage_t usage) const {
		if (type == buffer_type::undefined) throw buffer_exception("tried to call data() on buffer with undefined type!");
		glBindBuffer(type, handle);
		glBufferData(type, size, data, usage);
	}

	void buffer::sub_data(void* data, size_t offset, size_t length) const {
		if (type == buffer_type::undefined) throw buffer_exception("tried to call sub_data() on buffer with undefined type!");	
		glBindBuffer(type, handle);
		glBufferSubData(type, offset, length, data);
	}

	void buffer::get_data(void* data, size_t offset, size_t length) const {
		if (type == buffer_type::undefined) throw buffer_exception("tried to call get_data() on buffer with undefined type!");
		glBindBuffer(type, handle);
		glGetBufferSubData(type, offset, length, data);
	}
}
