#pragma once
#include "texture.hpp"

namespace aogl {
	class vertex_array;

	class program;
	class texture;
	class framebuffer;

	/**
	 * \namespace gl
	 * \brief wrap around all opengl functions
	 */
	namespace gl {

		namespace clear_flag {
			enum flag_t {
				stencil = GL_STENCIL_BUFFER_BIT,
				depth = GL_DEPTH_BUFFER_BIT,
				color = GL_COLOR_BUFFER_BIT,
				accum = GL_ACCUM_BUFFER_BIT
			};

			flag_t operator|(flag_t lhs, flag_t rhs);
		}

		namespace primitive {
			enum primitive_t {
                points = GL_POINTS,
                line_strip = GL_LINE_STRIP,
                line_loop = GL_LINE_LOOP,
                lines = GL_LINES,
                line_strip_adjacency = GL_LINE_STRIP_ADJACENCY,
                lines_adjacency = GL_LINES_ADJACENCY,
                triangle_strip = GL_TRIANGLE_STRIP,
                triangle_fan = GL_TRIANGLE_FAN,
                triangles = GL_TRIANGLES,
                triangle_strip_adjacency = GL_TRIANGLE_STRIP_ADJACENCY,
                triangles_adjacency = GL_TRIANGLES_ADJACENCY,
                patches = GL_PATCHES
			};
		}

        namespace indices_type {
            enum type_t {
                unsigned_byte = GL_UNSIGNED_BYTE,
                unsigned_short = GL_UNSIGNED_SHORT,
                unsigned_int = GL_UNSIGNED_INT
            };
        }

		namespace capability {
			enum capability_t {
				depth_test = GL_DEPTH_TEST,
				cull_face = GL_CULL_FACE
			};
		}

		namespace face {
			enum face_t {
				front = GL_FRONT,
				back = GL_BACK,
				front_and_back = GL_FRONT_AND_BACK
			};
		}

		namespace winding_order {
			enum order_t {
				clockwise = GL_CW,
				counter_clockwise = GL_CCW
			};
		}
		
		void use_program(const program& p);
		void use_program();

		void clear_color(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);

		void clear(clear_flag::flag_t flags);

		void bind_framebuffer(const framebuffer& fb);
		void bind_framebuffer();

		void active_texture(uint i = 0);

		void bind_texture(texture_target::target_t target, const texture& text);

		void bind_texture_2d(const texture& text, int i = 0);

		void bind_vertex_array(const vertex_array& vao);
        void bind_vertex_array();

        void draw_arrays(primitive::primitive_t primitive, int first, int count);

        void draw_elements(primitive::primitive_t primitive, int count, indices_type::type_t type, void* indices = nullptr);

		void enable(capability::capability_t capability);

		void viewport(int x, int y, int width, int height);

		void cull_face(face::face_t face);

		void front_face(winding_order::order_t order);
	}
}
