#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/texture.hpp"
#include "../include/aogl/error.hpp"

#include "../include/aogl/image.hpp"

namespace aogl {

	garbage_collector<uint> texture::gc = garbage_collector<uint>([](uint handle) {
		glDeleteTextures(1, &handle);
	});

	texture::texture() {
		glGenTextures(1, &handle);
		target = texture_target::undefined;
		gc.add(handle);
	}

	texture::texture(const image& image) {
		glGenTextures(1, &handle);
		target = texture_target::texture_2d;
		gc.add(handle);
		min_filter(texture_filter::linear);
		mag_filter(texture_filter::nearest);
		image2d(image);
	}

	texture::texture(texture_target::target_t target, texture_format::format_t format, int width, int height) {
		glGenTextures(1, &handle);
		this->target = target;
		gc.add(handle);

		glBindTexture(target, handle);
		glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

		min_filter(texture_filter::linear);
		mag_filter(texture_filter::linear);
	}

	texture::texture(const texture& other) {
		gc.copy(other.handle, handle);
		target = other.target;
	}

	texture::~texture() {
		gc.destroy(handle);
	}

	const texture& texture::operator=(const texture& other) {
		gc.destroy(handle);
		gc.copy(other.handle, handle);
		return *this;
	}

	void texture::image2d(const image& image) const {
		if (target == texture_target::undefined) throw texture_exception("get_texture target is undefined");
		glBindTexture(target, handle);
		glTexImage2D(target, 0, image.get_format(), image.width, image.height, 0, image.get_format(), GL_UNSIGNED_BYTE, image.handle);
		glGenerateMipmap(target);
		glBindTexture(target, 0);
	}

	void texture::min_filter(texture_filter::filter_t filter) const {
		if (target == texture_target::undefined) throw texture_exception("get_texture target is undefined");
		glBindTexture(target, handle);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
	}

	void texture::mag_filter(texture_filter::filter_t filter) const {
		if (target == texture_target::undefined) throw texture_exception("get_texture target is undefined");
		if (filter != texture_filter::linear && filter != texture_filter::nearest) throw texture_exception("only linear and nearest are valid magnification filtering functions!");
		glBindTexture(target, handle);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
	}

	void texture::texture_wrap(wrap_axis::axis_t axis, wrap_option::option_t option) const {
		glBindTexture(target, handle);
		glTexParameteri(target, axis, option);
		glBindTexture(target, 0);
	}

	void texture::border_color(float r, float g, float b, float a) const {
		float border[]{ r, g, b, a };
		glBindTexture(target, handle);
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, border);
		glBindTexture(target, 0);
	}

	texture::operator uint() const { return handle; }
}
