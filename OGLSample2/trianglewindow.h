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
    GLuint m_offset;
    GLuint m_zNear;
    GLuint m_zFar;
    GLuint m_frustumScale;
    QOpenGLShaderProgram *m_program;
};

#endif // TRIANGLEWINDOW_H
