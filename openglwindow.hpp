#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include "abcg.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
    protected:
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

        void setupModel();

};

#endif
