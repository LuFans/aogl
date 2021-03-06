//
// Created by Luca on 12.04.2017.
//

/**
 * Simple Quad Window to show basic functionality of AOGL
 */
#include <aogl/aogl.hpp>
#include <iostream>
#include <typeinfo>

void execute() {
    aogl::window window(800, 600, "Playground", aogl::window::flag::EXIT_ON_CLOSE);

    aogl::shader vert(aogl::shader_type::vertex, aogl::utils::read_from_file("assets/quad.vert"));
    aogl::shader frag(aogl::shader_type::fragment, aogl::utils::read_from_file("assets/quad.frag"));

    aogl::program program(vert, frag);

    std::array<float, 8> vertices {
            -0.5f, -0.5f,
            -0.5f,  0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f
    };
    auto vertex_buffer = aogl::create_buffer(aogl::buffer_type::array, vertices, aogl::buffer_usage::static_draw);

    std::array<uint, 6> indices {
            0, 1, 2,
            1, 3, 2
    };
    auto index_buffer = aogl::create_buffer(aogl::buffer_type::element_array, indices, aogl::buffer_usage::static_draw);

    aogl::vertex_array vao;
    vao.bind(vertex_buffer, 0, 2, aogl::attribute_type::float_type, false, 2 * sizeof(float), 0);
    vao.bind_element(index_buffer);

    while(window.is_open()) {

        using namespace aogl;

        auto greyscale = 51.0f / 255.0f;

        gl::clear_color(greyscale, greyscale, greyscale, 1.0);
        gl::clear(gl::clear_flag::color);

        gl::use_program(program);

        gl::bind_vertex_array(vao);
        gl::draw_elements(gl::primitive::triangles, 6, gl::indices_type::unsigned_int);

        window.present();
    }

}

int main() {

    try {
        execute();
    } catch (const std::exception& e) {
        // Catch all exceptions (as many are thrown by the library)
        std::cout << "[quad test] exception throw: \n" << typeid(e).name() << "\n\t" << e.what() << std::endl;
    }

}

