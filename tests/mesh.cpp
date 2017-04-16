//
// Created by Luca on 12.04.2017.
//

/**
 * Shows simple Mesh loading
 */
#include <aogl/aogl.hpp>
#include <iostream>
#include <typeinfo>

void execute() {

    aogl::window window(800, 600, "Playground", aogl::window::flag::EXIT_ON_CLOSE);
	
	aogl::gl::enable(aogl::gl::capability::depth_test);

    aogl::shader vert(aogl::shader_type::vertex, aogl::utils::read_from_file("assets/mesh.vert"));
    aogl::shader frag(aogl::shader_type::fragment, aogl::utils::read_from_file("assets/mesh.frag"));

    aogl::program program(vert, frag);

    aogl::mesh cube("assets/cube.obj", "assets/");
    aogl::buffer cube_buffer(aogl::buffer_type::array, cube, aogl::buffer_usage::static_draw, [](const aogl::vertex& vertex, aogl::buffer_stream& stream) {
        stream << vertex.position;
        stream << vertex.normal;
    });

    aogl::vertex_array vao;
    vao.bind(cube_buffer, 0, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 0);
    vao.bind(cube_buffer, 1, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 3 * sizeof(float));

    auto projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 10.0f);
    auto view_position = glm::vec3(2.0, 2.0, 3.0);
    auto view = glm::lookAt(view_position, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

    auto light_position = glm::vec3(3.0f, 3.0f, 3.0f);

    while(window.is_open()) {

        using namespace aogl;

        auto greyscale = 51.0f / 255.0f;

        gl::clear_color(greyscale, greyscale, greyscale, 1.0);
        gl::clear(gl::clear_flag::color | gl::clear_flag::depth);

        auto rotation_axis = glm::vec3(0.0, 1.0, 0.0);
        glm::normalize(rotation_axis);
        auto model = glm::rotate(glm::mat4(), static_cast<float>(glfwGetTime() / 10.0), rotation_axis);

        gl::use_program(program);

        program("projection") = projection;
        program("view") = view;
        program("model") = model;

        program("light_position") = light_position;
        program("view_position") = view_position;

        gl::bind_vertex_array(vao);
        gl::draw_arrays(gl::primitive::triangles, 0, 36);

        window.present();
    }

}

int main() {

    try {
        execute();
    } catch (const std::exception& e) {
        // Catch all exceptions (as many are thrown by the library)
        std::cout << "[mesh] exception throw: \n" << typeid(e).name() << "\n\t" << e.what() << std::endl;
#ifdef _MSC_VER
		system("pause");
#endif
    }

}


