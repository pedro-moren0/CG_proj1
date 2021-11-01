#include "abcg.hpp"
#include "openglwindow.hpp"
#include "gamedata.hpp"
#include "bullet.hpp"

#include <fmt/core.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

void Bullet::initializeGL(GLuint program, float shipMaxY){
    terminateGL();

#if !defined(__EMSCRIPTEN__)
    abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

    //store program and unifor locations
    m_program = program;
    m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
    m_shipPositionLoc = abcg::glGetUniformLocation(m_program, "shipPosition");

    m_startPosition = glm::vec2(0.0f, shipMaxY);

    //Gen position VBO
    abcg::glGenBuffers(1, &m_vboPositions);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    abcg::glBufferData(GL_ARRAY_BUFFER,
                        sizeof(m_startPosition),
                        &m_startPosition,
                        GL_STATIC_DRAW);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLint positionAttribute{
        abcg::glGetAttribLocation(m_program, "inPosition")
    };

    // Gen VAO
    abcg::glGenVertexArrays(1, &m_vao);

    abcg::glBindVertexArray(m_vao);

    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    abcg::glVertexAttribPointer(
        positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glBindVertexArray(0);
}

void Bullet::paintGL(
    glm::vec2 shipTranslation, float deltaTime, bool spacebarActive){

    abcg::glUseProgram(m_program);
    abcg::glBindVertexArray(m_vao);

    // Horizontal (x) positioning
    m_shipPosition = shipTranslation;

    // Adding new points at intervals iff spacebar is pressed
    if (spacebarActive && m_timer.elapsed() >= 250 / 1000.0 ){
        m_bulletQueue.emplace_back(shipTranslation);
        m_timer.restart();
    }

    // Render
    for (glm::vec2 bullet: m_bulletQueue) {
        // if (m_translation.y > 1.0f) {
        //     m_bulletQueue.remove(bullet);
        // }
        m_translation += glm::vec2(0.0f, deltaTime);
        abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

        abcg::glUniform2fv(m_shipPositionLoc, 1, &bullet.x);
        abcg::glDrawArrays(GL_POINTS, 0, 1);
        bullet += m_translation;
    }

    abcg::glBindVertexArray(0);
    abcg::glUseProgram(0);
}

void Bullet::terminateGL(){
    abcg::glDeleteProgram(m_program);
    abcg::glDeleteBuffers(1, &m_vboPositions);
    abcg::glDeleteVertexArrays(1, &m_vao);
}
