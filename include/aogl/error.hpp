#pragma once

#include <exception>

namespace aogl {

	/**
	 * \brief base exception for all of aogl
	 */
	class exception : public std::exception {
		std::string error;
	public:
		explicit exception(const std::string& _error) : error(_error) {}
		~exception() throw() override = default;
		virtual char const* what() const noexcept override { return error.c_str(); }
	};

	using window_creation_exception = exception;
	using shader_load_exception = exception;
	using program_link_exception = exception;
	using uniform_exception = exception;
	using buffer_exception = exception;
	using vertex_array_exception = exception;
	using texture_exception = exception;
	using mesh_parse_exception = exception;
	using framebuffer_exception = exception;
	using context_exception = exception;

}