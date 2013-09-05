#ifndef SILHOUETTEITEM_H
#define SILHOUETTEITEM_H

#include "viewer/SceneItem.h"
#include "types/UserFrame.h"
#include "types/ColorFrame.h"
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#define USER_COLORS 6

namespace dai {

class SilhouetteItem : public SceneItem
{
public:
    SilhouetteItem();
    void setUser(shared_ptr<UserFrame> user);

protected:
    void initialise() override;
    void render(int pass) override;

private:
    void renderFirstPass();
    void renderSecondPass();
    void prepareShaderProgram();
    void prepareVertexBuffer();
    void loadVideoTexture(void *texture, GLsizei width, GLsizei height, GLuint glTextureId);
    void loadMaskTexture(GLuint glTextureId, GLsizei width, GLsizei height, void* texture);

    QOpenGLShaderProgram*    m_shaderProgram;
    shared_ptr<UserFrame>    m_user;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer            m_positionsBuffer;
    QOpenGLBuffer            m_texCoordBuffer;

    // OpenGL identifiers
    GLuint                   m_posAttr; // Pos attr in the shader
    GLuint                   m_texCoord; // Texture coord in the shader
    GLuint                   m_stageUniform;
    GLuint                   m_currentFilterUniform;
    GLuint                   m_maskTextureId;
    GLuint                   m_texFGSampler;
    GLuint                   m_texMaskSampler; // Texture Sampler in the shader
};

} // End Namespace

#endif // SILHOUETTEITEM_H
