#pragma once
#include "texture.hpp"

namespace aogl {

	namespace image_format {
		enum format_t {
			greyscale = 1,
			greyscale_alpha = 2,
			rgb = 3,
			rgba = 4,
			automatic = 0
		};
	}

	class image {

		friend class texture;

		/**
		 * Garbage Collector
		 */
		static garbage_collector<unsigned char*> gc;

		/**
		 * \var unsigned char* handle
		 * \brief pointer to the image information loaded by SOIL
		 */
		unsigned char* handle;

		/**
		 * \var int width, height, channels
		 * \brief width and height of get_texture
		 */
		int width, height, channels;

		/**
		 * \fn texture_format::format_t get_format()
		 * \brief get the format for the get_texture
		 */
		texture_format::format_t get_format() const;

	public:
		/**
		 * \fn image image(const std::string& path, image_format::format_t format)
		 * \brief load an image from the give path using the specified format
		 */
		image(const std::string& path, image_format::format_t format);

		/**
		* \fn image image(const buffer& other)
		* \brief copy constructor
		*/
		image(const image& other);

		/**
		* \fn ~image
		* \brief destructor
		*/
		~image();

		/**
		* \fn const image& operator=(const image& other)
		* \brief assignment operator overload
		*/
		const image& operator=(const image& other);


	};

}
