#pragma once

namespace aogl {

	namespace shader_type {
		enum type {
			vertex = GL_VERTEX_SHADER,
			fragment = GL_FRAGMENT_SHADER,
			geometry = GL_GEOMETRY_SHADER
		};
	}

	class shader {

		/* Garbage Collector:
		 *  -> Keeps track of all active handles (with refrence counter)
		 */
		static garbage_collector<uint> gc;

		/**
		 * \brief Object Handle
		 */
		uint handle;

	public:
		/**
		 * \fn shader shader()
		 * \brief default constructor
		 * 
		 * \warning does not create a actual opengl object !!!!
		 */
		shader() = default;

		/**
		 * \fn shader shader(type _type, std::string&& path)
		 * \brief creates a shader from a string
		 * 
		 * \warning may throw an error if compilation fails
		 */
		shader(shader_type::type _type, std::string&& content);

		/**
		 * \fn shader shader(const shader& other)
		 * \brief copy constructor
		 */
		shader(const shader& other);

		/**
		 * \fn ~shader()
		 * \brief destructor
		 * 
		 * decrement the refrence count
		 */
		~shader();

		/**
		 * \fn const shader& operator=(const shader& other);
		 * \brief assignment operator
		 */
		const shader& operator=(const shader& other);

		/**
		 * \fn operator uint() const;
		 * \brief Conversion Operator
		 */
		operator uint() const;

	};

	namespace utils {
		
		/**
		 * \fn std::string read_from_file(std::string&& path)
		 * \brief load a file to string (used for string initialisation)
		 */
		std::string read_from_file(std::string&& path);

		/**
		 * \fn float get_time()
		 * \brief get execution time
		 */
		float get_time();

	}

}