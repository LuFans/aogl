#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/image.hpp"

#include <SOIL/SOIL.h>

namespace aogl {
	
	garbage_collector<unsigned char*> image::gc = garbage_collector<unsigned char*>([](unsigned char* handle) {
		SOIL_free_image_data(handle);
	});

	texture_format::format_t image::get_format() const { 
		switch (channels) {
		case 1:
			return texture_format::r;
		case 2:
			return texture_format::rg;
		case 3:
			return texture_format::rgb;
		case 4:
			return texture_format::rgba;
		default: 
			/* Should never happen */
			return texture_format::rgba;
		}
	}

	image::image(const std::string& path, image_format::format_t format) {
		this->handle = SOIL_load_image(path.c_str(), &this->width, &this->height, &this->channels, format);
		gc.add(handle);
		if (format != image_format::automatic) { 
			switch (format) {
			case image_format::greyscale:
				channels = 3;
			case image_format::greyscale_alpha:
				channels = 4;
			case image_format::rgb:
				channels = 3;
			case image_format::rgba:
				channels = 4;
			default:
				return;
			}
		}
	}

	image::image(const image& other) {
		gc.copy(other.handle, handle);
		width = other.width;
		height = other.height;
		channels = other.channels;
	}

	image::~image() {
		gc.destroy(handle);
	}

	const image& image::operator=(const image& other) {
		gc.destroy(handle);
		gc.copy(other.handle, handle);
		return *this;
	}
}
