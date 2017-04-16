#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/program.hpp"

#include "../include/aogl/shader.hpp"
#include "../include/aogl/error.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace aogl {

	garbage_collector<uint> program::gc = garbage_collector<uint>([](uint handle) {
		glDeleteProgram(handle);
	});

	void as_uniform<glm::tvec3<int, glm::precision::defaultp>>::to_uniform(int location, glm::ivec3&& data) {
		glUniform3i(location, data.x, data.y, data.z);
	}

	void as_uniform<glm::tvec3<float, glm::precision::defaultp>>::to_uniform(int location, glm::vec3&& data) {
		glUniform3f(location, data.x, data.y, data.z);
	}

	void as_uniform<const glm::tvec3<float, glm::precision::defaultp>>::to_uniform(int location, const glm::vec3&& data) {
		glUniform3f(location, data.x, data.y, data.z);
	}

	void as_uniform<glm::tmat4x4<float, glm::precision::defaultp>>::to_uniform(int location, glm::mat4&& data) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
	}

	void as_uniform<int>::to_uniform(int location, int&& data) {
		glUniform1i(location, data);
	}

	uniform::uniform(int handle) : location(handle) {}

	program::program() {
		handle = glCreateProgram();
		gc.add(handle);
	}

	program::program(const program& other) {
		gc.copy(other.handle, handle);
	}

	program::program(const shader& vert, const shader& frag) {
		handle = glCreateProgram();
		gc.add(handle);
		attach(vert);
		attach(frag);
		link();
	}

	program::program(const shader& vert, const shader& frag, const shader& geometry) {
		handle = glCreateProgram();
		gc.add(handle);
		attach(vert);
		attach(frag);
		attach(geometry);
		link();
	}

	program::~program() {
		gc.destroy(handle);
	}

	const program& program::operator=(const program& other) {
		gc.destroy(handle);
		gc.copy(other.handle, handle);
		return *this;
	}

	uniform program::operator()(std::string&& name) {
		if (uniform_locations.find(name) != uniform_locations.end()) {
			return uniform(uniform_locations.at(name));
		}
		auto location = glGetUniformLocation(handle, name.c_str());
		if (location == -1) throw uniform_exception("[uniform] tried to get location of non existing uniform");
		uniform_locations.insert(std::make_pair(std::move(name), location));
		return uniform(location);
	}

	void program::attach(const shader& shader) const {
		glAttachShader(handle, shader);
	}

	void program::link() const {
		glLinkProgram(handle);

		{
			int success;
			glGetProgramiv(handle, GL_LINK_STATUS, &success);
			if (!success) {
				int log_length;
				glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
				std::string info_log;
				info_log.reserve(log_length);
				glGetProgramInfoLog(handle, log_length, nullptr, &info_log[0]);
				std::stringstream ss;
				ss << "shader failed to link: \n" << info_log;
				throw program_link_exception(ss.str());
			}
		}
	}

}
