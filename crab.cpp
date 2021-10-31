#include "abcg.hpp"
#include "openglwindow.hpp"
// #include "gamedata.hpp"
#include "crab.hpp"

#include <fmt/core.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

void Crab::initializeGL(GLuint program){
    terminateGL();

    m_program = program;
    m_startPositionLoc = abcg::glGetUniformLocation(m_program, "startPosition");
    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
    m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");

    std::vector<glm::vec2> positions {
        // main body
        glm::vec2( 0.00f,  0.00f), //0
        glm::vec2(-0.20f,  0.30f), //1
        glm::vec2( 0.20f,  0.30f), //2
        glm::vec2( 0.40f,  0.20f), //3
        glm::vec2( 0.60f,  0.00f), //4
        glm::vec2( 0.40f, -0.20f), //5
        glm::vec2( 0.00f, -0.30f), //6
        glm::vec2(-0.40f, -0.20f), //7
        glm::vec2(-0.60f,  0.00f), //8
        glm::vec2(-0.40f,  0.20f), //9

        //left eye
        glm::vec2(-0.27f,  0.26f), //10
        glm::vec2(-0.27f,  0.34f), //11

        //right eye
        glm::vec2( 0.27f,  0.26f), //12
        glm::vec2( 0.27f,  0.34f), //13

        //right pawn
        glm::vec2( 0.55f,  0.05f),  //14
        glm::vec2( 0.66f,  0.16f),  //15
        glm::vec2( 0.53f,  0.30f),  //16
        glm::vec2( 0.71f,  0.21f),  //17
        glm::vec2( 0.90f,  0.30f),  //18

        // right leg
        glm::vec2( 0.45f, -0.15f),  //19
        glm::vec2( 0.66f, -0.36f),  //20
        glm::vec2( 0.40f, -0.56f),  //21
        glm::vec2( 0.54f, -0.34f),  //22

        //left leg
        glm::vec2(-0.45f, -0.15f),  //23
        glm::vec2(-0.66f, -0.36f),  //24
        glm::vec2(-0.40f, -0.56f),  //25
        glm::vec2(-0.54f, -0.34f),  //26

        //left pawn
        glm::vec2(-0.55f,  0.05f),  //27
        glm::vec2(-0.66f,  0.16f),  //28
        glm::vec2(-0.53f,  0.30f),  //29
        glm::vec2(-0.71f,  0.21f),  //30
        glm::vec2(-0.90f,  0.30f)   //31
    };

    int count{0};
    std::vector<glm::vec4> colors {};

    std::vector<glm::vec4> lineColors{};

    // Populate colors with the desired vec
    for (glm::vec2 i : positions) {
        if (count >= 10 && count <= 13) {
            colors.emplace_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lineColors.emplace_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            count++;
            continue;
        }
        colors.emplace_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        lineColors.emplace_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        count++;
    }

    std::vector<unsigned int> indices{
        // body
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7,
        0, 7, 8,
        0, 8, 9,
        0, 9, 1,

        //eyes
        10, 11, 1,
        2, 13, 12,

        //right pawn
        4, 14, 17,
        15, 16, 17,
        4, 17, 18,

        //right leg
        5, 19, 22,
        19, 22, 20,
        20, 22, 21,

        //left leg
        7, 23, 26,
        23, 24, 26,
        24, 26, 25,

        //left pawn
        8, 27, 30,
        28, 30, 29,
        8, 30, 31
    };

    std::vector<unsigned int> lineIndices{
        1, 10, 11, 1, 2, 13,
        12, 2, 3, 4, 18, 17,
        16, 15, 17, 14, 4, 19,
        20, 21, 22, 20, 22, 5,
        19, 5, 6, 7, 26, 25,
        24, 26, 24, 23, 7, 8,
        31, 30, 29, 28, 30, 27,
        8, 9, 1
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

void Crab::paintGL(){
    abcg::glUseProgram(m_program);

    // Start Position
    abcg::glUniform2fv(m_startPositionLoc, 1, &m_startPosition.x);

    // // Traslation
    // if (gameData.m_input[static_cast<size_t>(Input::Left)]
    //     && (m_minXpos * m_scale + m_translation.x) >= -1.0f) {
    //         m_translation += glm::vec2(-deltaTime, 0);
    //         abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    // }
    // if (gameData.m_input[static_cast<size_t>(Input::Right)]
    //     && (m_maxXpos * m_scale + m_translation.x) <= 1.0f) {
    //         m_translation += glm::vec2(deltaTime, 0);
    //         abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    // }

    // Scale
    abcg::glUniform1f(m_scaleLoc, m_scale);

    // Render
    abcg::glBindVertexArray(m_vaoBody);
    abcg::glDrawElements(GL_TRIANGLES, 23 * 3, GL_UNSIGNED_INT, nullptr);
    // abcg::glDrawArrays(GL_TRIANGLES, 0, 3);
    abcg::glBindVertexArray(0);

    abcg::glBindVertexArray(m_vaoLines);
    abcg::glDrawElements(GL_LINE_STRIP, (6 * 7) + 3, GL_UNSIGNED_INT, nullptr);
    abcg::glBindVertexArray(0);

    abcg::glUseProgram(0);
}

void Crab::terminateGL(){
    abcg::glDeleteProgram(m_program);
    abcg::glDeleteBuffers(1, &m_vboPositions);
    abcg::glDeleteBuffers(1, &m_vboBodyColors);
    // abcg::glDeleteBuffers(1, &m_vboLinesColors);
    abcg::glDeleteBuffers(1, &m_eboBody);
    // abcg::glDeleteBuffers(1, &m_eboLines);
    abcg::glDeleteVertexArrays(1, &m_vaoBody);
    // abcg::glDeleteVertexArrays(1, &m_vaoLines);
}
