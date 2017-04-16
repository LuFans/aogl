#pragma once

namespace aogl {

	class shader;

	template <typename T>
	class as_uniform {
	public:
		static void to_uniform(int location, T&& data) = delete;
	};

	template <>
	class as_uniform<glm::ivec3> {
	public:
		static void to_uniform(int location, glm::ivec3&& data);
	};

	template <>
	class as_uniform<glm::vec3> {
	public:
		static void to_uniform(int location, glm::vec3&& data);
	};

	template <>
	class as_uniform<const glm::vec3> {
	public:
		static void to_uniform(int location, const glm::vec3&& data);
	};

	template <>
	class as_uniform<glm::mat4> {
	public:
		static void to_uniform(int location, glm::mat4&& data);
	};

	template <>
	class as_uniform<int> {
	public:
		static void to_uniform(int location, int&& data);
	};

	class uniform {

		friend class program;
		/**
		 * \var uint location
		 * \brief the location of the uniform to upload to
		 */
		int location;

		/**
		 * \fn uniform uniform(uint handle)
		 * \brief private constructor
		 */
		explicit uniform(int handle);
		
	public:
		/**
		 * \fn void operator=(T&& value) const;
		 * \brief assignment operator used to transfer data to opengl uniforms
		 * 
		 *  for this to work type T must be implemented as a as_uniform class
		 *  
		 *  to do that you need to implement the as_uniform class for your type since otherwise this function will point to a deleted function
		 *  
		 *  example implementation:
		 *  
		 *  1. create your type
		 *  
		 *  struct my_vec3 {
		 *		float x, y, z;
		 *  } 
		 *  
		 *  2. implement the template for your type (c++ traits)
		 *  
		 *  template<>
		 *  class as_uniform<my_vec3> {
		 *  public:
		 *		static void to_uniform(int location, my_vec3&& value) {
		 *			glUniform3f(location, value.x, value.y, value.z);
		 *		}
		 *  }
		 * 
		 * \warning compiler error: as_unifrom<T>::to_uniform(...) points to a deleted function, if as_uniform is not implemented for T -> as_uniform<your_type>::to_uniform(int location, your_type&& value)
		 */
		template <typename T>
		void operator=(T&& value) const {
			as_uniform<T>::to_uniform(location, std::move(value));
		};

		/**
		 * \fn void operator=(T& value) const
		 * \brief Same function as the one above just that it takes in a refrence instead of a r-value refrence just so you don't need to type std::move() all the time
		 */
		template <typename T>
		void operator=(T& value) const {
			as_uniform<T>::to_uniform(location, std::move(value));
		}
	};

	class program {

		/**
		 * \brief Garbage Collector
		 */
		static garbage_collector<uint> gc;

		/**
		 * \brief Object Specific handle
		 */
		uint handle;

		/**
		 * \var std::map<std::string, int>
		 * \brief stores uniform locations
		 */
		std::map<std::string, int> uniform_locations;

	public:
		/**
		 * \fn program program()
		 * \brief default constructor
		 * 
		 * creates an empty program object without any shaders attached
		 */
		program();

		/**
		 * \fn program program(const program& other)
		 * \brief copy constructor
		 */
		program(const program& other);

		/**
		 * \fn program program(const shader& vert, const shader& frag)
		 * \brief creates a program attaches a vertex and a fragment shader and links the program
		 * 
		 * \param vert a compiled shader of type: shader_type::vertex
		 * \param frag a compiled shader of type: shader_type::fragment 
		 */
		program(const shader& vert, const shader& frag);

		/**
		* \fn program program(const shader& vert, const shader& frag, const shader& geometry)
		* \brief creates a program attaches a vertex, a fragment and a geometry shader and links the program
		*
		* \param vert a compiled shader of type: shader_type::vertex
		* \param frag a compiled shader of type: shader_type::fragment
		* \param geometry a compiled shader of type: shader_type::geometry
		*/
		program(const shader& vert, const shader& frag, const shader& geometry);

		/**
		* \fn ~program()
		* \brief destructor
		*
		* decrement the refrence count
		*/
		~program();

		/**
		* \fn const program& operator=(const program& other);
		* \brief assignment operator
		*/
		const program& operator=(const program& other);

		/**
		 * \fn uniform operator[](const std::string& name) const;
		 * \brief get a uniform object from the program
		 */
		uniform operator()(std::string&& name);

		/**
		 * \fn void attach(const shader& shader) const;
		 * \brief attaches a shader object to this program
		 * 
		 * \param shader a compiled shader object
		 */
		void attach(const shader& shader) const;
		
		/**
		 * \fn void link() const;
		 * \brief links the program
		 * 
		 * \warning may throw an error if linking failes
		 */
		void link() const;

		/**
		 * \fn operator uint() const
		 * \brief implicit conversion operator
		 */
		operator uint() const { return handle; }
	};

}
