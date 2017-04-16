#include <iostream>

#include <aogl/aogl.hpp>

int main() {

    try {

        aogl::window window(800, 600, "playground", aogl::window::flag::EXIT_ON_CLOSE);

        glEnable(GL_DEPTH_TEST);

        aogl::shader vert(aogl::shader_type::vertex, aogl::utils::read_from_file("assets/shader.vert"));
        aogl::shader frag(aogl::shader_type::fragment, aogl::utils::read_from_file("assets/shader.frag"));

        aogl::program program(vert, frag);

        aogl::shader master_vert(aogl::shader_type::vertex, aogl::utils::read_from_file("assets/master.vert"));
        aogl::shader master_frag(aogl::shader_type::fragment, aogl::utils::read_from_file("assets/master.frag"));

        aogl::program master_program(master_vert, master_frag);

        aogl::mesh my_mesh("assets/cube.obj", "assets/");
        aogl::buffer buffer(aogl::buffer_type::array, my_mesh, aogl::buffer_usage::static_draw, [](const aogl::vertex& v, aogl::buffer_stream& stream) {
            stream << v.position;
            stream << v.normal;
        });

        aogl::vertex_array vao;
        vao.bind(buffer, 0, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 0);
        vao.bind(buffer, 1, 3, aogl::attribute_type::float_type, false, 6 * sizeof(float), 3 * sizeof(float));

        std::array<float, 24> master_vertices {
                //   positions | tex_coords
                -1.0, -1.0, 0.0, 0.0, // -|
                -1.0,  1.0, 0.0, 1.0, //  |-> first triangle
                1.0, -1.0, 1.0, 0.0, // -|

                -1.0,  1.0, 0.0, 1.0, // -|
                1.0,  1.0, 1.0, 1.0, //  |-> second triangle
                1.0, -1.0, 1.0, 0.0  // -|
        };

        auto master_buffer = aogl::create_buffer(aogl::buffer_type::array, master_vertices, aogl::buffer_usage::static_draw);

        aogl::vertex_array master_vao;
        master_vao.bind(master_buffer, 0, 2, aogl::attribute_type::float_type, false, 4 * sizeof(float), 0);
        master_vao.bind(master_buffer, 1, 2, aogl::attribute_type::float_type, false, 4 * sizeof(float), 2 * sizeof(float));

        aogl::image image("assets/Mailand.jpg", aogl::image_format::rgba);
        aogl::texture texture(image);

        aogl::framebuffer framebuffer(800, 600);

        auto& color_attachment = framebuffer.get_texture(aogl::framebuffer_attachment::color0);

        auto projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 10.0f);
        auto view_position = glm::vec3(2.0, 2.0, 3.0);
        auto view = glm::lookAt(view_position, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

        auto light_position = glm::vec3(3.0f, 3.0f, 3.0f);

        while (window.is_open()) {

            using namespace aogl;

            gl::bind_framebuffer(framebuffer);

            gl::clear_color(0.2, 0.2, 0.2);
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

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            gl::bind_framebuffer();

            gl::clear_color(0.0, 0.0, 0.0, 1.0);
            gl::clear(gl::clear_flag::color | gl::clear_flag::depth);

            gl::use_program(master_program);

            gl::bind_texture_2d(color_attachment, 0);

            master_program("text") = 0;

            glBindVertexArray(master_vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            window.present();
        }

    } catch (const std::exception& exception) {
        std::cout << "exception thrown: \n" << exception.what() << std::endl;
    }

}