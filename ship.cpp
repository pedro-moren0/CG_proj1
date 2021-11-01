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
    m_startPositionLoc = abcg::glGetUniformLocation(m_program, "startPosition");
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
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),

        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),

        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)
    };

    std::vector<glm::vec4> lineColors{};

    float xMax {-10.0f};
    float xMin { 10.0f};
    float yMax {-10.0f};
    float yMin { 10.0f};

    // Get the min e max values of x and save them as a ship attribute
    // At the same time populate lineColors with vec4(black)
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
        lineColors.emplace_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    m_minXYpos = glm::vec2(xMin, yMin);
    m_maxXYpos = glm::vec2(xMax, yMax);

    std::vector<unsigned int> indices{
        0, 1, 2,
        0, 2, 3,
        2, 1, 3,
        1, 4, 3,

        5, 6, 1,
        1, 8, 9,
        5, 7, 9,

        10, 11,  3,
         3, 13, 14,
        10, 14, 12
    };
    std::vector<unsigned int> lineIndices{
         0,  1,  2,  0,
         1,  4,  3,  2,
         0,  3, 10, 14, 13,
        14, 12, 10, 11,
         0,
         6,  5,  7,  9,
         8,  1,  5,  9
    };

    //Gen position VBO
    abcg::glGenBuffers(1, &m_vboPositions);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    abcg::glBufferData(GL_ARRAY_BUFFER,
                        positions.size() * sizeof(glm::vec2),
                        positions.data(),
                        GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Gen body color VBO
    abcg::glGenBuffers(1, &m_vboBodyColors);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboBodyColors);
    abcg::glBufferData(GL_ARRAY_BUFFER,
                        colors.size() * sizeof(glm::vec4),
                        colors.data(),
                        GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Gen line color VBO
    abcg::glGenBuffers(1, &m_vboLinesColors);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboLinesColors);
    abcg::glBufferData(GL_ARRAY_BUFFER,
                        lineColors.size() * sizeof(glm::vec4),
                        lineColors.data(),
                        GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Generate body EBO
    abcg::glGenBuffers(1, &m_eboBody);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboBody);
    abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        indices.size() * sizeof(unsigned int),
                        indices.data(),
                       GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Generate line EBO
    abcg::glGenBuffers(1, &m_eboLines);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboLines);
    abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        lineIndices.size() * sizeof(unsigned int),
                        lineIndices.data(),
                       GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLint positionAttribute{
        abcg::glGetAttribLocation(m_program, "inPosition")
    };
    GLint colorAttribute{
        abcg::glGetAttribLocation(m_program, "inColor")
    };

    // Gen body VAO
    abcg::glGenVertexArrays(1, &m_vaoBody);

    abcg::glBindVertexArray(m_vaoBody);

    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    abcg::glVertexAttribPointer(
        positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glEnableVertexAttribArray(colorAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboBodyColors);
    abcg::glVertexAttribPointer(
        colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboBody);

    abcg::glBindVertexArray(0);

    // Gen lines VAO
    abcg::glGenVertexArrays(1, &m_vaoLines);

    abcg::glBindVertexArray(m_vaoLines);

    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    abcg::glVertexAttribPointer(
        positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glEnableVertexAttribArray(colorAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboLinesColors);
    abcg::glVertexAttribPointer(
        colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboLines);

    abcg::glBindVertexArray(0);
}

void Ship::paintGL(const GameData &gameData, float deltaTime){
    abcg::glUseProgram(m_program);

    // Star Position
    abcg::glUniform2fv(m_startPositionLoc, 1, &m_startPosition.x);

    // Traslation
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
    abcg::glBindVertexArray(m_vaoBody);
    abcg::glDrawElements(GL_TRIANGLES, 10 * 3, GL_UNSIGNED_INT, nullptr);
    abcg::glBindVertexArray(0);

    abcg::glBindVertexArray(m_vaoLines);
    abcg::glDrawElements(GL_LINE_STRIP, (6 * 4) + 2, GL_UNSIGNED_INT, nullptr);
    abcg::glBindVertexArray(0);

    abcg::glUseProgram(0);

    // Shoot
    // if (gameData.m_input[static_cast<size_t>(Input::Fire)]
    //     && !m_bulletState.wasSpacebarPressed)
    // {
    //     m_bulletState.addToQueue(m_bulletState, m_translation);
    // }

    m_spacebarStatus = gameData.m_input[static_cast<size_t>(Input::Fire)];
    m_bullet.paintGL(
        m_translation, //Horizontal only
        deltaTime,
        m_spacebarStatus
    );
    m_spacebarStatus = false;
}

void Ship::terminateGL(){
    abcg::glDeleteProgram(m_program);
    abcg::glDeleteBuffers(1, &m_vboPositions);
    abcg::glDeleteBuffers(1, &m_vboBodyColors);
    abcg::glDeleteBuffers(1, &m_vboLinesColors);
    abcg::glDeleteBuffers(1, &m_eboBody);
    abcg::glDeleteBuffers(1, &m_eboLines);
    abcg::glDeleteVertexArrays(1, &m_vaoBody);
    abcg::glDeleteVertexArrays(1, &m_vaoLines);
}
