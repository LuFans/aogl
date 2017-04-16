/**
 * A simple example which renders a ground plane and multiple cubes throwing shadows onto the plane
 */

#include <iostream>
#include <aogl/aogl.hpp>

/* Const Fields */
float light_radius = 3.0f;
float light_z = -1.5f;
constexpr glm::vec3 view_position = glm::vec3(0.0f, 2.0f, 7.0f);

constexpr uint shadow_resolution = 1024;
constexpr float shadow_field = 5.0f;
constexpr float shadow_near_plane = 1.0f;
constexpr float shadow_far_plane = 10.0f;

constexpr auto window_width = 1600;
constexpr auto window_height = 1200;

constexpr auto plane_size = 10.0f;
constexpr auto plane_height = 0.0f;
constexpr std::array<float, 24> plane_vertices {
	-plane_size, 0.0f, -plane_size, 0.0f, 1.0f, 0.0f,
	-plane_size, 0.0f,  plane_size, 0.0f, 1.0f, 0.0f,
	 plane_size, 0.0f, -plane_size, 0.0f, 1.0f, 0.0f,
	 plane_size, 0.0f, plane_size, 0.0f, 1.0f, 0.0f,
};
constexpr std::array<uint, 6> plane_indices {
	0, 1, 2, 1, 3, 2
};


struct transform {
	glm::vec3 position;
	glm::vec3 scale;
};

void render_scene(aogl::program& program, std::vector<transform>& transforms, aogl::vertex_array& cube_vertex_array, aogl::vertex_array& plane_vertex_array, aogl::vertex_array& teapot_vertex_array, int teapot_vertices);

void run() {
	aogl::window window(window_width, window_height, "Shadows", aogl::window::flag::EXIT_ON_CLOSE);
	/* opengl settings */
	aogl::gl::enable(aogl::gl::capability::depth_test);
	aogl::gl::enable(aogl::gl::capability::cull_face);
	aogl::gl::front_face(aogl::gl::winding_order::counter_clockwise);
	aogl::gl::clear_color(0.2, 0.2, 0.2, 1.0);
	/* Load the Cube Mesh */
	aogl::mesh cube("assets/cube.obj", "assets/");
	aogl::buffer cube_buffer(aogl::buffer_type::array, cube, aogl::buffer_usage::static_draw, [](const aogl::vertex& v, aogl::buffer_stream& stream) {
		stream << v.position;
		stream << v.normal;
	});
	aogl::vertex_array cube_vertex_array;
	cube_vertex_array.bind(cube_buffer, 0, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 0);
	cube_vertex_array.bind(cube_buffer, 1, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 3 * sizeof(float));
	/* Load the Teapot */
	aogl::mesh teapot("assets/wt_teapot.obj");
	auto teapot_vertices = teapot.vertices_count();
	aogl::buffer teapot_buffer(aogl::buffer_type::array, teapot, aogl::buffer_usage::static_draw, [](const aogl::vertex& v, aogl::buffer_stream& stream) {
		stream << v.position;
		stream << v.normal;
	});
	aogl::vertex_array teapot_vertex_array;
	teapot_vertex_array.bind(teapot_buffer, 0, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 0);
	teapot_vertex_array.bind(teapot_buffer, 1, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 3 * sizeof(float));
	/* Create the Transforms for the Cubes */
	std::vector<transform> transforms;
	transforms.push_back(transform{ glm::vec3(0.5f, 1.5f, 0.5f) , glm::vec3(0.2f) });
	transforms.push_back(transform{ glm::vec3(2.0f, 0.5f, 1.0f) , glm::vec3(0.2f) });
	transforms.push_back(transform{ glm::vec3(-0.5f, 1.0f, -1.0f) , glm::vec3(0.2f) });
	transforms.push_back(transform{ glm::vec3(1.0f, 0.4f, 1.5f), glm::vec3(0.2f) });
	transforms.push_back(transform{ glm::vec3(-0.75f, 0.75f, 1.5f), glm::vec3(0.4f) });
	/* Create the ground Plane */
	auto plane_buffer = aogl::create_buffer(aogl::buffer_type::array, plane_vertices, aogl::buffer_usage::static_draw);
	auto plane_element_buffer = aogl::create_buffer(aogl::buffer_type::element_array, plane_indices, aogl::buffer_usage::static_draw);
	aogl::vertex_array plane_vertex_array;
	plane_vertex_array.bind(plane_buffer, 0, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 0);
	plane_vertex_array.bind(plane_buffer, 1, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 3 * sizeof(float));
	plane_vertex_array.bind_element(plane_element_buffer);
	/* Load the shaders */
	aogl::shader vert(aogl::shader_type::vertex, aogl::utils::read_from_file("assets/shadow.vert"));
	aogl::shader frag(aogl::shader_type::fragment, aogl::utils::read_from_file("assets/shadow.frag"));
	aogl::program program(vert, frag);
	/* "Camera" Setup */
	auto projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 10.0f);
	auto view = glm::lookAt(view_position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	/* Shadow Projection Setup */
	auto light_projection = glm::ortho(-shadow_field, shadow_field, -shadow_field, shadow_field, shadow_near_plane, shadow_far_plane);
	/* Shadow Framebuffer Setup */
	aogl::framebuffer shadow_framebuffer;
	{
		aogl::texture depth_texture(aogl::texture_target::texture_2d, aogl::texture_format::depth, shadow_resolution, shadow_resolution);
		depth_texture.texture_wrap(aogl::wrap_axis::s, aogl::wrap_option::clamp_to_border);
		depth_texture.texture_wrap(aogl::wrap_axis::t, aogl::wrap_option::clamp_to_border);
		depth_texture.border_color(1.0f, 1.0f, 1.0f, 1.0f);
		shadow_framebuffer.attach_texture(aogl::framebuffer_attachment::depth, std::move(depth_texture));
	}
	aogl::gl::bind_framebuffer(shadow_framebuffer);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	aogl::gl::bind_framebuffer();
	/* Shadow Shader */
	aogl::shader shadow_vert(aogl::shader_type::vertex, aogl::utils::read_from_file("assets/shadow_depth.vert"));
	aogl::shader shadow_frag(aogl::shader_type::fragment, aogl::utils::read_from_file("assets/shadow_depth.frag"));

	aogl::program shadow_program(shadow_vert, shadow_frag);

	auto time = aogl::utils::get_time();
	auto counter = 0.0;
	auto frames = 0;

	/* RUN */
	while (window.is_open()) {

		using namespace aogl;

		auto delta_time = aogl::utils::get_time() - time;
		time += delta_time;

		counter += delta_time;
		frames++;
		if (counter >= 1.0) {
			counter -= 1.0;
			std::cout << frames << std::endl;
			frames = 0;
		}
		

		/* Calculate Light Position */
		auto angle = static_cast<float>(glm::radians(time) * 4.0);
		auto y = sin(angle) * light_radius;
		auto x = cos(angle) * light_radius;
		auto light_position = glm::vec3(x, y, light_z);
		auto light_view = glm::lookAt(light_position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		auto light_space = light_projection * light_view;

		/* Render to the Shadow Framebuffer */
		gl::bind_framebuffer(shadow_framebuffer);
		gl::viewport(0, 0, shadow_resolution, shadow_resolution);
		gl::clear(gl::clear_flag::depth);

		gl::use_program(shadow_program);

		shadow_program("light_space") = light_space;

		gl::cull_face(gl::face::front);

		render_scene(shadow_program, transforms, cube_vertex_array, plane_vertex_array, teapot_vertex_array, teapot_vertices);

		/* Draw to the main framebuffer */
		gl::bind_framebuffer();
		gl::viewport(0, 0, window_width, window_height);
		gl::clear(gl::clear_flag::color | gl::clear_flag::depth);

		/* Bind Shader Program */
		gl::use_program(program);

		gl::bind_texture_2d(shadow_framebuffer.get_texture(aogl::framebuffer_attachment::depth), 0);

		/* Upload Shader Uniforms */
		program("projection") = projection;
		program("view") = view;
		program("light_position") = light_position;
		program("view_position") = view_position;
		program("light_space") = light_space;

		program("depth_map") = 0;

		/* Render the Scene */
		gl::cull_face(gl::face::back);

		render_scene(program, transforms, cube_vertex_array, plane_vertex_array, teapot_vertex_array, teapot_vertices);
		/* swap the window framebuffers */
		window.present();
	}
}

void render_scene(aogl::program& program, std::vector<transform>& transforms, aogl::vertex_array& cube_vertex_array, aogl::vertex_array& plane_vertex_array, aogl::vertex_array& teapot_vertex_array, int teapot_vertices) {

	using namespace aogl;

	/* Draw the Teapot */

	auto model = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3));
	program("model") = model;

	gl::bind_vertex_array(teapot_vertex_array);
	gl::draw_arrays(gl::primitive::triangles, 0, teapot_vertices);
	/* Draw the Cubes */
	for (auto& transform : transforms) {
		/* Calculate Model Matrix */
		model = glm::mat4();
		model = glm::translate(model, transform.position);
		model = glm::scale(model, transform.scale);
		/* Upload Model Matrix */
		program("model") = model;
		/* Draw the transformed Cube */
		gl::bind_vertex_array(cube_vertex_array);
		gl::draw_arrays(gl::primitive::triangles, 0, 36);
	}
	/* Draw the Plane */
	model = glm::translate(glm::mat4(), glm::vec3(0.0f, plane_height, 0.0f));
	program("model") = model;
	gl::bind_vertex_array(plane_vertex_array);
	gl::draw_elements(gl::primitive::triangles, 6, gl::indices_type::unsigned_int);
}

int main() {
	
	try {
		run();
	} catch (const std::exception& e) {
		// Catch all exceptions (as many are thrown by the library)
		std::cout << "[shadow] exception throw: \n" << typeid(e).name() << "\n\t" << e.what() << std::endl;
#ifdef _MSC_VER
		system("pause");
#endif
	}


}