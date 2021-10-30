#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "ship.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
    protected:
        void handleEvent(SDL_Event& event) override;
        void initializeGL() override;
        void paintGL() override;
        void paintUI() override;
        void resizeGL(int width, int height) override;
        void terminateGL() override;

    private:
        int m_viewportWidth{};
        int m_viewportHeight{};

        GameData m_gameData;
        Ship m_ship;
};

#endif
