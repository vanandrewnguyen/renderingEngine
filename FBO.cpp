#include "FBO.hpp"

// If MSAA is enabled you can't use post-processing, so you need to make a new fresh FBO
FBO::FBO(bool toggleMSAA) {
    // Frame buffer vert + indicies (rect with no transforms)
    float frameVert[] = {
        // Coords    // texCoords
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        1.0f,  1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

    windowHeight = 800;
    windowWidth = 800;

    // Frame buffer mapped to VAO, VBO
    glGenVertexArrays(1, &rectVAOID);
    glGenBuffers(1, &rectVBOID);
    glBindVertexArray(rectVAOID);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frameVert), &frameVert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Init FBO
	glGenFramebuffers(1, &FBOID);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOID);

    if (toggleMSAA) {
        // Init texture to bind to FBO
        unsigned int samples = 8; // how much we sample for MSAA anti aliasing
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texID);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, windowWidth, windowHeight, GL_TRUE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texID, 0);

        // Init render buffer
        glGenRenderbuffers(1, &RBOID);
        glBindRenderbuffer(GL_RENDERBUFFER, RBOID);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOID);
    } else {
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
    }
	
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



