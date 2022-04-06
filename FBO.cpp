#include "FBO.hpp"

FBO::FBO() {
    windowHeight = 800;
    windowWidth = 800;

	// Init FBO
	glGenFramebuffers(1, &FBOID);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOID);

	// Init texture to bind to FBO
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

    // Init render buffer
    glGenRenderbuffers(1, &RBOID);
    glBindRenderbuffer(GL_RENDERBUFFER, RBOID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOID);
    // Error checking RBO
    auto FBOError = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (FBOError != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer error: " << FBOError << std::endl;
    }
}

void FBO::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
}

void FBO::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::BindTex() {
    glBindTexture(GL_TEXTURE_2D, texID);
}

void FBO::Delete() {
    glDeleteFramebuffers(1, &FBOID);
}



