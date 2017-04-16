#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/shader.hpp"
#include "../include/aogl/error.hpp"

namespace aogl {

	/**
	 * Shader Implementation
	 */

	garbage_collector<uint> shader::gc = garbage_collector<uint>([](uint handle) {
		glDeleteShader(handle);
	});

	shader::shader(shader_type::type _type, std::string&& content) {
		
		auto c_content = content.c_str();

		handle = glCreateShader(_type);
		gc.add(handle);

		glShaderSource(handle, 1, &c_content, nullptr);
		glCompileShader(handle);
		// Check for Compilation Error
		{
			int success;
			glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
			if (!success) {
				int log_length;
				glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
				std::string info_log;
				info_log.reserve(log_length);
				glGetShaderInfoLog(handle, log_length, nullptr, &info_log[0]);
				std::stringstream ss;
				ss << "shader (" << std::move(_type) << ") to compile: \n" << info_log.c_str();
				throw shader_load_exception(ss.str());
			}
		}
	}

	shader::shader(const shader& other) {
		gc.copy(other.handle, handle);
	}

	shader::~shader() {
		gc.destroy(handle);
	}

	const shader& shader::operator=(const shader& other) {
		gc.destroy(handle);
		gc.copy(other.handle, handle);
		return *this;
	}

	shader::operator uint() const { return handle; }

	/**
	 * Utils Implementation
	 */

	std::string utils::read_from_file(std::string&& path) {

		std::stringstream sstream;
		try {
			std::ifstream fstream(std::move(path), std::ios_base::in);
			if (!fstream.is_open()) {
				std::stringstream ss;
				ss << "[shader] failed to load shader file: (" << std::move(path) << ")";
				throw shader_load_exception(ss.str());
			}
			sstream << fstream.rdbuf();
		}
		catch (const std::exception& e) {
			throw shader_load_exception(e.what());
		}

		return sstream.str();
	}

	float utils::get_time() {
		return static_cast<float>(glfwGetTime());
	}
}
