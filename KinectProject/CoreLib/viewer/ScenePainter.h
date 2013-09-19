#ifndef SCENEPAINTER_H
#define SCENEPAINTER_H

#include "types/DataFrame.h"
#include "viewer/SceneItem.h"
#include <memory>
#include <atomic>
#include <QList>
#include <QMatrix4x4>
#include <QOpenGLFunctions>

using namespace std;

namespace dai {

class ScenePainter : public QOpenGLFunctions
{
public:
    static void loadVideoTexture(GLuint glTextureId, GLsizei width, GLsizei height, void *texture);
    static void loadMaskTexture(GLuint glTextureId, GLsizei width, GLsizei height, void *texture);

    ScenePainter();
    virtual ~ScenePainter();
    void clearItems();
    void addItem(shared_ptr<SceneItem> item);
    shared_ptr<SceneItem> getFirstItem(ItemType type) const;
    void setBackground(shared_ptr<DataFrame> background);
    void renderScene();
    void setMatrix(const QMatrix4x4& matrix);
    QMatrix4x4& getMatrix();
    void updateItemsMatrix();
    void resetPerspective();
    void setSize(qreal width, qreal height);

protected:
    void renderItems();
    virtual void initialise() = 0;
    virtual void render() = 0;

    qreal                            m_width;
    qreal                            m_height;
    shared_ptr<DataFrame>            m_bg;
    atomic<int>                      m_needLoading;
    QMatrix4x4                       m_matrix;
    QList<shared_ptr<SceneItem>>     m_items;

private: 
    bool                             m_initialised;
};

} // End Namespace

#endif // SCENEPAINTER_H