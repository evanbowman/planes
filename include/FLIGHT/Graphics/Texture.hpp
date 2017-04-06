#pragma once

#ifdef FLIGHT_MAC
#include <OpenGL/gl3.h>
#elif FLIGHT_LINUX
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#elif FLIGHT_WINDOWS
#include <GL/glew.h>
#endif
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

namespace FLIGHT {
class Texture {
    GLuint m_id;
    bool m_loaded = false;
    void Cleanup();

public:
    enum class Sampling { Nearest = GL_NEAREST, Linear = GL_LINEAR };
    void LoadFromFile(const std::string & path,
                      Sampling sampling = Sampling::Nearest);
    void LoadFromMemory(const std::vector<uint8_t> & data,
                        const glm::vec2 & size);
    GLuint GetId() const;
    const Texture & operator=(const Texture &) = delete;
    ~Texture();
};
}
