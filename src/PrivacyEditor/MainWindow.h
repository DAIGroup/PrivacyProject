#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsPathItem>
#include <QSize>
#include <QPointF>
#include <QPen>
#include "types/MaskFrame.h"
#include "types/ColorFrame.h"
#include "types/SkeletonFrame.h"
#include "filters/PrivacyFilter.h"

namespace Ui {
class MainWindow;
}

struct MapData {
    float x;
    float y;
    dai::SkeletonJoint::JointType joint;
    int parent;
};

class Display {
    QGraphicsScene m_scene;
    QGraphicsPixmapItem* m_bg_item;
    QImage m_image;

public:
    Display();
    QGraphicsScene* scene();
    const QGraphicsPixmapItem* background() const;
    QImage& image();
    int imageWidth() const;
    int imageHeight() const;
    void setImage(const QImage &image);
    void update();
};

class MainWindow : public QMainWindow, dai::FrameListener
{
    Q_OBJECT

    const static QSize MAX_IMAGE_SIZE;

    Ui::MainWindow* m_ui;
    QFileSystemModel m_fs_model;
    Display m_input;
    Display m_mask;
    Display m_background;
    Display m_output;
    QGraphicsPathItem* m_mask_item;    
    QPointF m_last_pixel;
    QPen m_pen;
    QString m_current_image_path;
    dai::PrivacyFilter m_privacy;
    QGraphicsRectItem* m_selected_joint;
    bool m_drawing = false;
    QGraphicsItem* m_skeleton_root;
    QString m_output_path;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void load_selected_image();
    dai::SkeletonPtr load_skeleton(QString fileName) const;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void eventSilhouette(QEvent *event);
    void eventSkeleton(QEvent *event);
    void newFrames(const dai::QHashDataFrames dataFrames);

private slots:
    void first_setup();
    void setOutputImage(QImage image);

private:
    void setup_skeleton(dai::SkeletonPtr skeleton = nullptr);
    dai::SkeletonFramePtr create_skeleton_from_scene();
    bool exceedSize(const QImage& image) const;
    void scaleImage(QImage &image, const QSize size = MAX_IMAGE_SIZE, Qt::AspectRatioMode aspectMode = Qt::KeepAspectRatioByExpanding) const;
    void convertJointToMapData(MapData& data, const dai::SkeletonJoint& joint, int parent, MapData* coords);
    dai::MaskFramePtr create_mask(const QPainterPath& path);
    dai::MaskFramePtr create_mask(const QImage& image);

    QGraphicsItem *addJoint(int x, int y, dai::SkeletonJoint::JointType type, QGraphicsItem *parent = nullptr);
};

#endif // MAINWINDOW_H
