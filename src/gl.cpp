#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/gl.hpp"

#include "../include/aogl/aogl.hpp"
#include "../include/aogl/error.hpp"

namespace aogl {
	namespace gl {

		clear_flag::flag_t clear_flag::operator|(flag_t lhs, flag_t rhs) {
			return static_cast<flag_t>(static_cast<int>(lhs) | static_cast<int>(rhs));
		}

		void use_program(const program& p) {
			glUseProgram(p);
		}

		void use_program() {
			glUseProgram(0);
		}

		void clear_color(float r, float g, float b, float a) {
			glClearColor(r, g, b, a);
		}

		void clear(clear_flag::flag_t flags) {
			glClear(flags);
		}

		void bind_framebuffer(const framebuffer& fb) {
			glBindFramebuffer(GL_FRAMEBUFFER, fb);
		}

		void bind_framebuffer() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void active_texture(uint i) {
			if (i < 0 || i > 31) throw context_exception("[context] active texture index should be in between 0 and 31");
			glActiveTexture(GL_TEXTURE0 + i);
		}

		void bind_texture(texture_target::target_t target, const texture& text) {
			glBindTexture(target, text);
		}

		void bind_texture_2d(const texture& text, int i) {
			active_texture(i);
			bind_texture(texture_target::texture_2d, text);
		}

		void bind_vertex_array(const vertex_array& vao) {
            glBindVertexArray(vao);
        }

        void bind_vertex_array() {
            glBindVertexArray(0);
        }

        void draw_arrays(primitive::primitive_t primitive, int first, int count) {
            glDrawArrays(primitive, first, count);
        }

        void draw_elements(primitive::primitive_t primitive, int count, indices_type::type_t type, void* indices) {
            glDrawElements(primitive, count, type, indices);
        }

		void enable(capability::capability_t capability) {
			glEnable(capability);
		}

		void viewport(int x, int y, int width, int height) {
			glViewport(x, y, width, height);
		}

		void cull_face(face::face_t face) {
			glCullFace(face);
		}

		void front_face(winding_order::order_t order) {
			glFrontFace(order);
		}
	}
}
