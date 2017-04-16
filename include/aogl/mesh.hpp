#pragma once

namespace aogl {
	
	struct vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
		uint material_index;
	};

	struct material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
	};

	class mesh {

		friend class buffer;

		/**
		 * \var std::vector<vertex> vertices
		 * \brief the parsed data
		 */
		std::vector<vertex> vertices;

		/**
		 * \var std::vector<material> materials
		 * \brief the material from the .mtl file
		 */
		std::vector<material> materials;

	public:
		/**
		 * \fn mesh mesh(const std::string& path)
		 * \brief load a .obj mesh from a file
		 */
		explicit mesh(const std::string& path, const std::string& mtl_base = "");

		/**
		 * \fn int vertices_count()
		 * \brief the count of vertices
		 */
		int vertices_count() const;
	};

}