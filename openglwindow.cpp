#include "openglwindow.hpp"
#include "abcg.hpp"

#include <vector>
#include <imgui.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


void OpenGLWindow::initializeGL() {
    const auto *vertexShader{R"gl(
        #version 410

        layout(location = 0) in vec2 inPosition;
        layout(location = 1) in vec4 inColor;

        out vec4 fragColor;

        void main() {
            gl_Position = vec4(inPosition, 0, 1);
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

    abcg::glClearColor(0, 0, 0, 1);
    abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::paintGL() {
    setupModel();

    abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    abcg::glUseProgram(m_program);

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
