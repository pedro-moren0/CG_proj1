#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>
#include <bitset> //other file

#include "abcg.hpp"

// -----------This needs to go to another file--------------
enum class Input { Right, Left, Down, Up};

struct GameData {
  // State m_state{State::Playing};
  std::bitset<5> m_input;  // [fire, up, down, left, right]
};
// -----------This needs to go to another file--------------

class OpenGLWindow : public abcg::OpenGLWindow {
    protected:
        void handleEvent(SDL_Event& event) override;
        void initializeGL() override;
        void paintGL() override;
        void paintUI() override;
        void resizeGL(int width, int height) override;
        void terminateGL() override;

    private:
        GLuint m_program{};
        GLuint m_vboColors{};
        GLuint m_vboPositions{};
        GLuint m_vao{};

        int m_viewportWidth{};
        int m_viewportHeight{};

        GLint m_translationLoc{};
        glm::vec2 m_translation{glm::vec2(0)};
        GLint m_scaleLoc{};
        float m_scale{0.125f};

        GameData m_gameData;

        void setupModel();

};

#endif
