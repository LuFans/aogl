#include "../include/aogl/stdafx.hpp"
#include "../include/aogl/framebuffer.hpp"
#include "../include/aogl/error.hpp"

namespace aogl {

	garbage_collector<uint> renderbuffer::gc = garbage_collector<uint>([](uint handle) {
		glDeleteRenderbuffers(1, &handle);
	});

	renderbuffer::renderbuffer(renderbuffer_format::format_t format, int width, int height) {
		glGenRenderbuffers(1, &handle);
		gc.add(handle);
		glBindRenderbuffer(GL_RENDERBUFFER, handle);
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	}

	renderbuffer::renderbuffer(const renderbuffer& other) {
		gc.copy(other.handle, handle);
	}

	renderbuffer::~renderbuffer() {
		gc.destroy(handle);
	}

	const renderbuffer& renderbuffer::operator=(const renderbuffer& rhs) {
		gc.destroy(handle);
		gc.copy(rhs.handle, handle);
		return *this;
	}

	renderbuffer::operator uint() const { return handle; }

	garbage_collector<uint> framebuffer::gc = garbage_collector<uint>([](uint handle) {
		glDeleteFramebuffers(1, &handle);
	});

	framebuffer::framebuffer() {
		glGenFramebuffers(1, &handle);
		gc.add(handle);
		width = 0; height = 0;
	}

	framebuffer::framebuffer(int _width, int _height) {
		glGenFramebuffers(1, &handle);
		gc.add(handle);
		width = _width; height = _height;

		texture color(texture_target::texture_2d, texture_format::rgba, width, height);
		attach_texture(framebuffer_attachment::color0, std::move(color));

		renderbuffer rbo(renderbuffer_format::depth_stencil, width, height);
		attach_renderbuffer(framebuffer_attachment::depth_stencil, std::move(rbo));

		if (!is_completed()) throw framebuffer_exception("[framebuffer] standard constructor failed to complete the framebuffer!");
	}

	framebuffer::framebuffer(const framebuffer& other) {
		width = other.width; height = other.height;
		gc.copy(other.handle, handle);
	}

	framebuffer::~framebuffer() {
		gc.destroy(handle);
	}

	const framebuffer& framebuffer::operator=(const framebuffer& rhs) {
		width = rhs.width; height = rhs.height;
		gc.destroy(handle);
		gc.copy(rhs.handle, handle);
		return *this;
	}

	void framebuffer::attach_texture(framebuffer_attachment::attachment_t attachment, texture&& texture) {
		if (texture.target != texture_target::texture_2d) throw framebuffer_exception("[framebuffer] get_texture should have target texture2d");
		if (textures.find(attachment) != textures.end() || renderbuffers.find(attachment) != renderbuffers.end()) throw framebuffer_exception("[framebuffer] tried to attach a get_texture to a already filled attachment");
		textures.insert(std::make_pair(attachment, std::move(texture)));
		glBindFramebuffer(GL_FRAMEBUFFER, handle);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture.target, texture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	} 

	void framebuffer::attach_renderbuffer(framebuffer_attachment::attachment_t attachment, renderbuffer&& renderbuffer) {
		if (renderbuffers.find(attachment) != renderbuffers.end() || textures.find(attachment) != textures.end()) throw framebuffer_exception("[framebuffer] tried to attach a renderbuffer to a already used attachment");
		renderbuffers.insert(std::make_pair(attachment, std::move(renderbuffer)));
		glBindFramebuffer(GL_FRAMEBUFFER, handle);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void framebuffer::detach_attachment(framebuffer_attachment::attachment_t attachment) {
		{
			auto iterator = textures.find(attachment);
			if (iterator != textures.end()) {
				glBindFramebuffer(GL_FRAMEBUFFER, handle);
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				textures.erase(iterator);
				return;
			}
		}
		{
			auto iterator = renderbuffers.find(attachment);
			if (iterator != renderbuffers.end()) {
				glBindFramebuffer(GL_FRAMEBUFFER, handle);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, 0);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				renderbuffers.erase(iterator);
				return;
			}
		}
	}

	texture& framebuffer::get_texture(framebuffer_attachment::attachment_t attachment) {
		auto iterator = textures.find(attachment);
		if (iterator == textures.end()) throw framebuffer_exception("[framebuffer] tried to get non existing get_texture attachment");
		return iterator->second;
	}

	renderbuffer& framebuffer::get_renderbuffer(framebuffer_attachment::attachment_t attachment) {
		auto iterator = renderbuffers.find(attachment);
		if (iterator == renderbuffers.end()) throw framebuffer_exception("[framebuffer] tried to get non existing get_texture attachment");
		return iterator->second;
	}

	bool framebuffer::is_completed() const {
		glBindFramebuffer(GL_FRAMEBUFFER, handle);
		auto return_value = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return return_value;
	}

	framebuffer::operator uint() const { return handle; }

}
