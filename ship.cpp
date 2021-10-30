#include "abcg.hpp"
#include "openglwindow.hpp"
#include "gamedata.hpp"
#include "ship.hpp"

#include <fmt/core.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

void Ship::initializeGL(GLuint program){
    terminateGL();

    m_program = program;
    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
    m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");

    std::vector<glm::vec2> positions {
        // main body
        glm::vec2( 0.00f,  0.69f), //0
        glm::vec2(-0.40f,  0.00f), //1
        glm::vec2( 0.00f,  0.23f), //2
        glm::vec2( 0.40f,  0.00f), //3
        glm::vec2( 0.00f, -0.23f), //4

        //left wing
        glm::vec2(-0.40f,  0.46f), //5
        glm::vec2(-0.20f,  0.35f), //6
        glm::vec2(-0.50f,  0.00f), //7
        glm::vec2(-0.20f, -0.12f), //8
        glm::vec2(-0.40f, -0.46f), //9

        //right wing
        glm::vec2( 0.40f,  0.46f), //10
        glm::vec2( 0.20f,  0.35f), //11
        glm::vec2( 0.50f,  0.00f), //12
        glm::vec2( 0.20f, -0.12f), //13
        glm::vec2( 0.40f, -0.46f)  //14
    };

    std::vector<glm::vec4> colors {
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),

        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),

        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
    };

    // Get the min e max values of x and y
    float xMax {-10.0f};
    float xMin { 10.0f};
    float yMax {-10.0f};
    float yMin { 10.0f};

    for (glm::vec2 member : positions) {
        if (member.x < xMin) {
            xMin = member.x;
        }
        if (member.x > xMax) {
            xMax = member.x;
        }
        if (member.y < yMin) {
            yMin = member.y;
        }
        if (member.y > yMax) {
            yMax = member.y;
        }
    }
    m_minXYpos = glm::vec2(xMin, yMin);
    m_maxXYpos = glm::vec2(xMax, yMax);

    fmt::print(
        "m_minXYpos = ({}, {}), m_maxXYpos = ({}, {})\n",
        m_minXYpos.x, m_minXYpos.y, m_maxXYpos.x, m_maxXYpos.y);

    std::vector<unsigned int> indices{
        0, 1, 2,
        0, 2, 3,
        2, 1, 3,
        1, 4, 3,

        5, 6, 1,
        1, 8, 9,
        5, 7, 9,

        10, 11, 3,
        3, 13, 14,
        10, 14, 12
    };

    //Gen position VBO
    abcg::glGenBuffers(1, &m_vboPositions);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    abcg::glBufferData(GL_ARRAY_BUFFER,
                        positions.size() * sizeof(glm::vec2),
                        positions.data(),
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

    // Generate EBO
    abcg::glGenBuffers(1, &m_ebo);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        indices.size() * sizeof(unsigned int),
                        indices.data(),
                       GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
    abcg::glVertexAttribPointer(
        positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glEnableVertexAttribArray(colorAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
    abcg::glVertexAttribPointer(
        colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    abcg::glBindVertexArray(0);
}

void Ship::paintGL(const GameData &gameData, float deltaTime){
    abcg::glUseProgram(m_program);

    //Movement
    if (gameData.m_input[static_cast<size_t>(Input::Up)]
        && (m_maxXYpos.y * m_scale + m_translation.y) <= 1.0f) {
            m_translation += glm::vec2(0, deltaTime);
            abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    }
    if (gameData.m_input[static_cast<size_t>(Input::Down)]
        && (m_minXYpos.y * m_scale + m_translation.y) >= -1.0f) {
            m_translation += glm::vec2(0, -deltaTime);
            abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    }
    if (gameData.m_input[static_cast<size_t>(Input::Left)]
        && (m_minXYpos.x * m_scale + m_translation.x) >= -1.0f) {
            m_translation += glm::vec2(-deltaTime, 0);
            abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    }
    if (gameData.m_input[static_cast<size_t>(Input::Right)]
        && (m_maxXYpos.x * m_scale + m_translation.x) <= 1.0f) {
            m_translation += glm::vec2(deltaTime, 0);
            abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    }

    // Scale
    abcg::glUniform1f(m_scaleLoc, m_scale);

    // Render
    abcg::glBindVertexArray(m_vao);
    abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);
    // abcg::glDrawArrays(GL_TRIANGLES, 0, 3);
    abcg::glBindVertexArray(0);

    abcg::glUseProgram(0);
}

void Ship::terminateGL(){
    abcg::glDeleteProgram(m_program);
    abcg::glDeleteBuffers(1, &m_vboPositions);
    abcg::glDeleteBuffers(1, &m_vboColors);
    abcg::glDeleteBuffers(1, &m_ebo);
    abcg::glDeleteVertexArrays(1, &m_vao);
}
