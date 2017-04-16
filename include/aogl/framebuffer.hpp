#pragma once
#include "texture.hpp"

namespace aogl {
	
	namespace renderbuffer_format {
		enum format_t {
			depth16 = GL_DEPTH_COMPONENT16,
			depth24 = GL_DEPTH_COMPONENT24,
			depth32f = GL_DEPTH_COMPONENT32F,
			depth_stencil = GL_DEPTH24_STENCIL8,
			stencil = GL_STENCIL_INDEX8
		};
	}

	namespace framebuffer_attachment {
		enum attachment_t {
			color0 = GL_COLOR_ATTACHMENT0,
			color1 = GL_COLOR_ATTACHMENT1,
			color2 = GL_COLOR_ATTACHMENT2,
			color3 = GL_COLOR_ATTACHMENT3,
			color4 = GL_COLOR_ATTACHMENT4,
			color5 = GL_COLOR_ATTACHMENT5,
			color6 = GL_COLOR_ATTACHMENT6,
			color7 = GL_COLOR_ATTACHMENT7,
			color8 = GL_COLOR_ATTACHMENT8,
			color9 = GL_COLOR_ATTACHMENT9,
			color10 = GL_COLOR_ATTACHMENT10,
			color11 = GL_COLOR_ATTACHMENT11,
			color12 = GL_COLOR_ATTACHMENT12,
			color13 = GL_COLOR_ATTACHMENT13,
			color14 = GL_COLOR_ATTACHMENT14,
			color15 = GL_COLOR_ATTACHMENT15,
			depth = GL_DEPTH_ATTACHMENT,
			stencil = GL_STENCIL_ATTACHMENT,
			depth_stencil = GL_DEPTH_STENCIL_ATTACHMENT
		};
	}

	class renderbuffer {
		
		/**
		 * \var static garbage_collector<uint> gc
		 * \brief garbage collector
		 */
		static garbage_collector<uint> gc;

		/**
		 * \var uint handle
		 * \brief the handle to the current opengl object
		 */
		uint handle;

	public:

		/**
		 * \fn renderbuffer renderbuffer(renderbuffer_format::format_t format, int width, int height)
		 * \brief create a renderbuffer and fill it
		 */
		renderbuffer(renderbuffer_format::format_t format, int width, int height);

		/**
		 * \fn renderbuffer renderbuffer(const renderbuffer& other)
		 * \brief copy constructor
		 */
		renderbuffer(const renderbuffer& other);

		/**
		 * \fn ~renderbuffer()
		 * \brief destructor
		 */
		~renderbuffer();

		/**
		 * \fn const renderbuffer& operator=(const renderbuffer& rhs)
		 * \brief assignment operator overload
		 */
		const renderbuffer& operator=(const renderbuffer& rhs);

		/**
		 * \fn operator uint() const
		 * \brief implicit conversion operator
		 */
		operator uint() const;

	};



	class framebuffer {
		
		/**
		* \var static garbage_collector<uint> gc
		* \brief garbage collector
		*/
		static garbage_collector<uint> gc;

		/**
		* \var uint handle
		* \brief the handle to the current opengl object
		*/
		uint handle;

		/**
		 * \var int width, height
		 * \brief default width and height of the attechments
		 */
		int width, height;

		/**
		 * \var std::map<framebuffer_attachment::attachment_t, get_texture> textures
		 * \brief map of attached textures
		 */
		std::map<framebuffer_attachment::attachment_t, texture> textures;

		/**
		 * \var std::map<framebuffer_attachment::attachment_t, renderbuffer> renderbuffers
		 * \brief map of attached framebuffers
		 */
		std::map<framebuffer_attachment::attachment_t, renderbuffer> renderbuffers;

	public:

		/**
		 * \fn framebuffer framebuffer()
		 * \brief default constructor
		 */
		framebuffer();

		/**
		 * \fn framebuffer framebuffer(int _width, int _height, renderbuffer_format::format_t format)
		 * \brief create a frambuffer and attach a color attachment as well as a renderbuffer as a depth stencil buffer
		 * \param _width default width of all attachments
		 * \param _height default height of all attachments
		 */
		framebuffer(int _width, int _height);

		/**
		 * \fn framebuffer framebuffer(const framebuffer& other);
		 * \brief copy constructor
		 */
		framebuffer(const framebuffer& other);

		/**
		 * \fn ~framebuffer()
		 * \brief destructor
		 */
		~framebuffer();

		/**
		* \fn const framebuffer& operator=(const framebuffer& rhs)
		* \brief assignment operator overload
		*/
		const framebuffer& operator=(const framebuffer& rhs);

		/**
		 * \fn void attach_texture(framebuffer_attachment::attachment_t attachment, get_texture& get_texture)
		 * \brief add a color attachment to this framebuffer
		 * \param attachment the target attachment (please note that the get_texture should have a matching internal format color -> GL_RGB/GL_RGBA, depth -> GL_DEPTH_COMPONENT, stencil -> GL_STENCIL_COMPONENT) 
		 * \param texture empty get_texture
		 */
		void attach_texture(framebuffer_attachment::attachment_t attachment, texture&& texture);

		/**
		 * \fn void attach_renderbuffer(framebuffer_attachment::attachment_t attachment, renderbuffer& renderbuffer)
		 * \brief attach a renderbuffer to the object
		 * \param attachment the attachment type should match the renderbuffer format (for example framebuffer_attachment::depth_stencil -> renderbuffer_format::depth_stencil)
		 */
		void attach_renderbuffer(framebuffer_attachment::attachment_t attachment, renderbuffer&& renderbuffer);

		/**
		 * \fn void detach_attachment(framebuffer_attachment::attachment_t attachment)
		 * \brief used to detach / delete a get_texture or renderbuffer from the given fbo
		 */
		void detach_attachment(framebuffer_attachment::attachment_t attachment);

		/**
		 * \fn get_texture& get_texture(framebuffer_attachment::attachment_t attachment)
		 * \brief get a get_texture attached to the framebuffer
		 * 
		 * \warning may throw framebuffer_exception if framebuffer has not that attachment
		 */
		texture& get_texture(framebuffer_attachment::attachment_t attachment);

		/**
		* \fn renderbuffer& renderbuffer(framebuffer_attachment::attachment_t attachment)
		* \brief get a renderbuffer attached to the framebuffer
		*
		* \warning may throw framebuffer_exception if framebuffer has not that attachment
		*/
		renderbuffer& get_renderbuffer(framebuffer_attachment::attachment_t attachment);

		/**
		 * \fn bool is_completed()
		 * \brief check if framebuffer is completed
		 */
		bool is_completed() const;

		/**
		 * \fn operator uint() const
		 * \brief implicit operator overload
		 */
		operator uint() const;
	};

}
