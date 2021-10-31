#ifndef CRAB_HPP
#define CRAB_HPP

#include "abcg.hpp"

class OpenGLWindow;

class Crab {
    public:
        void initializeGL(GLuint program);
        void paintGL();
        void terminateGL();
    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLuint m_vboPositions{};
        GLuint m_vboBodyColors{};
        GLuint m_eboBody{};
        GLuint m_vaoBody{};

        GLuint m_vboLinesColors{};
        GLuint m_eboLines{};
        GLuint m_vaoLines{};

        GLint m_startPositionLoc{};
        GLint m_translationLoc{};
        GLint m_scaleLoc{};

        glm::vec2 m_startPosition{glm::vec2(0)};
        glm::vec2 m_translation{glm::vec2(0)};
        float m_scale{1.0f/*0.125f*/};
};

#endif
