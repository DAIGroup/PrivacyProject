#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include <QtGui/QOpenGLShaderProgram>
#include "openglwindow.h"
#include <QElapsedTimer>

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize();
    void render();

private:
    GLuint loadShader(GLenum type, const char *source);

    QElapsedTimer timer;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_loopDuration;
    GLuint m_fragLoopDuration;
    GLuint m_time;
    QOpenGLShaderProgram *m_program;
};

#endif // TRIANGLEWINDOW_H