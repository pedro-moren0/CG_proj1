#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

#include <vector>

class OpenGLWindow;

class Ship {
    public:
        void initializeGL(GLuint program);
        void paintGL(const GameData &gameData, float deltaTime);
        void terminateGL();

    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLuint m_vboColors{};
        GLuint m_vboPositions{};
        GLuint m_ebo{};
        GLuint m_vao{};

        GLint m_translationLoc{};
        GLint m_scaleLoc{};

        glm::vec2 m_translation{glm::vec2(0)};
        float m_scale{0.125f};

        glm::vec2 m_minXYpos {};
        glm::vec2 m_maxXYpos {};

};

#endif
