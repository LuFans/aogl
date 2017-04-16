#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../include/tiny_obj_loader.h"
#include "../include/aogl/error.hpp"

namespace aogl {

	mesh::mesh(const std::string& path, const std::string& mtl_base) {

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> obj_materials;

		std::string err;
		auto ret = tinyobj::LoadObj(&attrib, &shapes, &obj_materials, &err, path.c_str(), mtl_base.c_str(), true);

		if (!ret) throw mesh_parse_exception("fatal error!");

		if (!err.empty()) throw mesh_parse_exception(err);

		auto convert_a2v = [](float* data, size_t size) -> glm::vec3 {
			if (size != 12) return glm::vec3(0.0);
			return glm::vec3(data[0], data[1], data[2]);
		};

		for (auto& m : obj_materials) {
			auto ambient = convert_a2v(m.ambient, sizeof(m.ambient));
			auto diffuse = convert_a2v(m.diffuse, sizeof(m.diffuse));
			auto specular = convert_a2v(m.specular, sizeof(m.specular));
			auto shininess = m.shininess;
			material mat{ ambient, diffuse, specular, shininess };
			materials.push_back(mat);
		}

		for (auto& shape : shapes) {
			
			size_t index_offset = 0;

			for (auto f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
				
				auto face_vertices = shape.mesh.num_face_vertices[f];

				for (auto v = 0; v < face_vertices; v++) {
					
					auto index = shape.mesh.indices[index_offset + v];

					glm::vec3 position(
						index.vertex_index != -1 ? attrib.vertices[3 * index.vertex_index + 0] : 0.0f,
						index.vertex_index != -1 ? attrib.vertices[3 * index.vertex_index + 1] : 0.0f,
						index.vertex_index != -1 ? attrib.vertices[3 * index.vertex_index + 2] : 0.0f
					);

					glm::vec3 normal(0.0f, 0.0f, 0.0f);

					if (index.normal_index != -1) {
						normal.x = attrib.normals[3 * index.normal_index + 0];
						normal.y = attrib.normals[3 * index.normal_index + 1];
						normal.z = attrib.normals[3 * index.normal_index + 2];
					}

					glm::vec2 tex_coord(0.0f, 0.0f);

					if (index.texcoord_index != -1) {
						tex_coord.x = attrib.texcoords[2 * index.texcoord_index + 0];
						tex_coord.y = attrib.texcoords[2 * index.texcoord_index + 1];
					}

					auto material_index = shape.mesh.material_ids[f];

					auto _vertex = vertex{ position, normal, tex_coord };

					vertices.push_back(std::move(_vertex));

				}

				index_offset += face_vertices;

			}

		}

	}

	int mesh::vertices_count() const {
		return vertices.size();
	}
}
