#include "openglwindow.hpp"
#include "abcg.hpp"

#include <vector>
#include <imgui.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
    }
}

void OpenGLWindow::initializeGL() {
    const auto *vertexShader{R"gl(
        #version 410

        layout(location = 0) in vec2 inPosition;
        layout(location = 1) in vec4 inColor;

        uniform vec2 translation;
        uniform float scale;

        out vec4 fragColor;

        void main() {
            vec2 newPosition = inPosition * scale + translation;
            gl_Position = vec4(newPosition, 0, 1);
            fragColor = inColor;
        }
    )gl"};

    const auto *fragmentShader{R"gl(
        #version 410

        in vec4 fragColor;

        out vec4 outColor;

        void main() {
            outColor = fragColor;
        }
    )gl"};

    m_program = createProgramFromString(vertexShader, fragmentShader);

    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
    m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");

    abcg::glClearColor(0, 0, 0, 1);
    abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::paintGL() {
    abcg::glClear(GL_COLOR_BUFFER_BIT);
    const float deltaTime{static_cast<float>(getDeltaTime())};
    setupModel();

    abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    abcg::glUseProgram(m_program);

    //Movement
    if (m_gameData.m_input[static_cast<size_t>(Input::Up)]) {
        m_translation += glm::vec2(0, deltaTime);
        abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    }
    if (m_gameData.m_input[static_cast<size_t>(Input::Down)]) {
        m_translation += glm::vec2(0, -deltaTime);
        abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    }
    if (m_gameData.m_input[static_cast<size_t>(Input::Left)]) {
        m_translation += glm::vec2(-deltaTime, 0);
        abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    }
    if (m_gameData.m_input[static_cast<size_t>(Input::Right)]) {
        m_translation += glm::vec2(deltaTime, 0);
        abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    }

    // Scale
    abcg::glUniform1f(m_scaleLoc, m_scale);

    // Render
    abcg::glBindVertexArray(m_vao);
    abcg::glDrawArrays(GL_TRIANGLES, 0, 3);
    abcg::glBindVertexArray(0);

    abcg::glUseProgram(0);

}

void OpenGLWindow::paintUI() {
    abcg::OpenGLWindow::paintUI();
}

void OpenGLWindow::resizeGL(int width, int height) {
    m_viewportWidth = width;
    m_viewportHeight = height;

    abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
    abcg::glDeleteProgram(m_program);
    abcg::glDeleteBuffers(1, &m_vboPositions);
    abcg::glDeleteBuffers(1, &m_vboColors);
    abcg::glDeleteVertexArrays(1, &m_vao);

}

void OpenGLWindow::setupModel() {
    abcg::glDeleteBuffers(1, &m_vboColors);
    abcg::glDeleteBuffers(1, &m_vboPositions);
    abcg::glDeleteVertexArrays(1, &m_vao);


    std::vector<glm::vec2> points {
        glm::vec2(-0.5f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.5f, 0.0f)
    };

    std::vector<glm::vec4> colors {
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
    };


    //Gen position VBO
    abcg::glGenBuffers(1, &m_vboPositions);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    abcg::glBufferData(GL_ARRAY_BUFFER,
                        points.size() * sizeof(glm::vec2),
                        points.data(),
                        GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Gen color VBO
    abcg::glGenBuffers(1, &m_vboColors);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
    abcg::glBufferData(GL_ARRAY_BUFFER,
                        colors.size() * sizeof(glm::vec4),
                        colors.data(),
                        GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLint positionAttribute{
        abcg::glGetAttribLocation(m_program, "inPosition")
    };
    GLint colorAttribute{
        abcg::glGetAttribLocation(m_program, "inColor")
    };

    // Gen VAO
    abcg::glGenVertexArrays(1, &m_vao);

    abcg::glBindVertexArray(m_vao);

    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glEnableVertexAttribArray(colorAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
    abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glBindVertexArray(0);
}
