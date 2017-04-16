#pragma once

namespace aogl {

	class image;
	class framebuffer;

	namespace texture_target {
		enum target_t {
			undefined = 0,
			texture_1d = GL_TEXTURE_1D,
			texture_2d =  GL_TEXTURE_2D,
			texture_3d = GL_TEXTURE_3D,
			texture_1d_array = GL_TEXTURE_1D_ARRAY,
			texture_2d_array = GL_TEXTURE_2D_ARRAY, 
			texture_rectangle = GL_TEXTURE_RECTANGLE,
			cube_map = GL_TEXTURE_CUBE_MAP,
			cube_map_array = GL_TEXTURE_CUBE_MAP_ARRAY,
			texture_buffer = GL_TEXTURE_BUFFER,
			texture_2d_multisample = GL_TEXTURE_2D_MULTISAMPLE,
			texture_2d_multisample_array = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		};
	}

	namespace texture_filter {
		enum filter_t {
			nearest = GL_NEAREST,
			linear = GL_LINEAR,
			nearest_mipmap_nearest = GL_NEAREST_MIPMAP_NEAREST,
			nearest_mipmap_linear = GL_NEAREST_MIPMAP_LINEAR,
			linear_mipmap_nearest = GL_LINEAR_MIPMAP_NEAREST,
			linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR
		};
	}

	namespace texture_format {
		enum format_t {
			r = GL_R,
			rg = GL_RG,
			rgb = GL_RGB,
			rgba = GL_RGBA,
			depth = GL_DEPTH_COMPONENT,
			depth16 = GL_DEPTH_COMPONENT16,
			depth24 = GL_DEPTH_COMPONENT24,
			depth32 = GL_DEPTH_COMPONENT32,
			depth32f = GL_DEPTH_COMPONENT32F,
			stencil1 = GL_STENCIL_INDEX1,
			stencil4 = GL_STENCIL_INDEX4,
			stencil8 = GL_STENCIL_INDEX8,
			stencil16 = GL_STENCIL_INDEX16
		};
	}

	namespace wrap_option {
		enum option_t {
			clamp = GL_CLAMP,
			clamp_to_edge = GL_CLAMP_TO_EDGE,
			clamp_to_border = GL_CLAMP_TO_BORDER,
			mirrored_repeat = GL_MIRRORED_REPEAT,
			repeat = GL_REPEAT
		};
	}

	namespace wrap_axis {
		enum axis_t {
			s = GL_TEXTURE_WRAP_S,
			t = GL_TEXTURE_WRAP_T,
			r = GL_TEXTURE_WRAP_R
		};
	}

	class texture {

		friend class framebuffer;

		/**
		* Garbage Collector
		*/
		static garbage_collector<uint> gc;
		/**
		* \var uint handle
		* \brief internal opengl handle
		*/
		uint handle;
		/**
		 * \var texture_target::target_t target
		 * \brief the target of the texture
		 */
		texture_target::target_t target;

	public:
		
		/**
		 * \fn texture texture()
		 * \brief default constructor
		 */
		texture();

		/**
		 * \fn texture texture(const image& image);
		 * \brief create a texture from an image
		 */
		explicit texture(const image& image);

		/**
		 * \fn texture texture(texture_target::target_t target, texture_format::format_t format, int width, int height)
		 * \brief create a texture and create a texture buffer of size width and height
		 * 
		 *  used for framebuffers...
		 */
		texture(texture_target::target_t target, texture_format::format_t format, int width, int height);

		/**
		 * \fn texture texture(const texture& other)
		 * \brief copy constructor
		 */
		texture(const texture& other);

		/**
		* \fn ~texture
		* \brief destructor
		*/
		~texture();

		/**
		* \fn const texture& operator=(const texture& other)
		* \brief assignment operator overload
		*/
		const texture& operator=(const texture& other);

		/**
		 * \fn void image2d(const image& image) const
		 * \brief upload an image to the texture
		 */
		void image2d(const image& image) const;

		/**
		* \fn void min_filter(texture_filter::filter_t filter)
		* \brief set the minifying function of the texture
		*/
		void min_filter(texture_filter::filter_t filter) const;

		/**
		 * \fn void map_filter(texture_filter::filter_t filter)
		 * \brief set the magnification function of the texture
		 */
		void mag_filter(texture_filter::filter_t filter) const;

		/**
		 * \fn void texture_wrap(wrap_axis::axis_t axis, wrap_option::option_t option)
		 * \brief specifiy a texture wrap option for this texture
		 */
		void texture_wrap(wrap_axis::axis_t axis, wrap_option::option_t option) const;

		/**
		 * \fn void border_color(float r, float g, float b, float a)
		 * \brief set the border color
		 */
		void border_color(float r, float g, float b, float a) const;

		/**
		 * \fn operator uint()
		 * \brief implicit conversion operator
		 */
		operator uint() const;
	};

}
