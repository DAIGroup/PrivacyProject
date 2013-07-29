#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QtWidgets/QDesktopWidget>
#include <fstream>
#include <iostream>
#include "dataset/Dataset.h"
#include "dataset/MSRDaily/MSRDailyActivity3D.h"
#include "dataset/MSRAction3D/MSR3Action3D.h"
#include "dataset/DAI/DAIDataset.h"
#include "openni/OpenNIDepthInstance.h"
#include "openni/OpenNIColorInstance.h"
#include "openni/OpenNIUserInstance.h"
#include "openni/OpenNISkeletonInstance.h"
#include "types/DepthFrame.h"
#include "types/SkeletonFrame.h"
#include "types/DataFrame.h"
#include "playback/PlaybackControl.h"
#include "viewer/InstanceViewerWindow.h"
#include "viewer/InstanceRecorder.h"
#include "viewer/TestListener.h"
#include "KMeans.h"
#include "DepthSeg.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   
    ui->setupUi(this);

    // Show this window centered
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize;

    screenWidth = desktop->availableGeometry(desktop->primaryScreen()).width();
    screenHeight = desktop->availableGeometry(desktop->primaryScreen()).height();
    windowSize = size();
    width = windowSize.width();
    height = windowSize.height();

    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 50;

    move ( x, y );
    setFixedSize(windowSize.width(), windowSize.height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::number(int value)
{
    QString result = QString::number(value);

    if (value < 10)
        result = "0" + QString::number(value);

    return result;
}

void MainWindow::testSegmentation()
{
    /*float data[] = {0.1, 0.3, 0.2, 0.5, 0.6, 0.2, 0.3, 0.1, 0.30, 0.36, 0.45, 0.3, 0.15, 0.17, -0.1, -0.3, -0.2, -0.5, -0.6, -0.2, -0.3, -0.1, -0.30, -0.36, -0.45, -0.3, -0.15, -0.17};
    //float data[] = {1, 3, 2, 5, 6, 2, 3, 1, 30, 36, 45, 3, 15, 17};
    float n = sizeof(data) / sizeof(float);
    const dai::KMeans* kmeans = dai::KMeans::execute(data, n, 4);

    qDebug() << "--------------------------------------";
    qDebug() << "Centroids" << kmeans->getCentroids();
    qDebug() << "Compactness" << kmeans->getCompactness();

    const QList<float>* values = kmeans->getClusterValues();
    for (int i=0; i<kmeans->getK(); ++i) {
        qDebug() << "Cluster" << i << values[i];
    }*/


    int width = 12;
    int height = 12;

    dai::DepthFrame depth_frame(width, height);

    float my_frame1[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
        0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    float my_frame2[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 4, 4, 4, 4, 4, 4, 4, 4, 1, 0,
        0, 1, 4, 8, 8, 8, 8, 8, 8, 4, 1, 0,
        0, 1, 4, 8, 8, 8, 8, 8, 8, 4, 1, 0,
        0, 1, 4, 8, 8, 9, 9, 8, 8, 4, 1, 0,
        0, 1, 4, 8, 8, 9, 9, 8, 8, 4, 1, 0,
        0, 1, 4, 8, 8, 8, 8, 8, 8, 4, 1, 0,
        0, 1, 4, 8, 8, 8, 8, 8, 8, 4, 1, 0,
        0, 1, 4, 4, 4, 4, 4, 4, 4, 4, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    for (int i=0; i<12; ++i)
    {
        for (int j=0; j<12; ++j)
        {
            depth_frame.setItem(i, j, my_frame2[i*12 + j ]);
        }
    }

    dai::DepthSeg dseg(depth_frame);
    dseg.execute();
    //dseg.print_cluster_mask(3, 3);

    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            cout << dseg.getCluster(i, j) + 1 << " ";
        }
        cout << endl;
    }
}

void MainWindow::on_btnOpenDataSets_clicked()
{
    m_browser.show();
}

void MainWindow::on_btnParseDataset_clicked()
{
    dai::MSR3Action3D* dataset = new dai::MSR3Action3D();
    const dai::DatasetMetadata& dsMetadata = dataset->getMetadata();
    const dai::InstanceInfoList* instances = dsMetadata.instances(dai::InstanceInfo::Skeleton);

    QListIterator<dai::InstanceInfo*> it(*instances);

    while (it.hasNext())
    {
        dai::InstanceInfo* info = it.next();
        shared_ptr<dai::DataInstance> dataInstance = dataset->getSkeletonInstance(info->getActivity(), info->getActor(), info->getSample());

        dataInstance->open();

        qDebug() << "Instance" << info->getActivity() << info->getActor() << info->getSample() << "open";
        int framesProcessed = 0;

        QString fileName = "a" + number(info->getActivity()) +
                "_s" + number(info->getActor()) +
                "_e" + number(info->getSample()) + "_quaternion.txt";

        ofstream of;
        of.open( (dsMetadata.getPath() + "/" + fileName).toStdString().c_str(), ios::out | ios::trunc );
        of << dataInstance->getTotalFrames() << endl;

        while (dataInstance->hasNext() && of.is_open())
        {
            dataInstance->readNextFrame();

            shared_ptr<dai::SkeletonFrame> skeletonFrame = static_pointer_cast<dai::SkeletonFrame>(dataInstance->frame());
            of << (skeletonFrame->getIndex() + 1) << endl;

            for (int i=0; i<17; ++i) {
                dai::Quaternion::QuaternionType type = (dai::Quaternion::QuaternionType) i;
                const dai::Quaternion& quaternion = skeletonFrame->getSkeleton(1)->getQuaternion(type);

                float w = quaternion.scalar();
                float x = quaternion.vector().x();
                float y = quaternion.vector().y();
                float z = quaternion.vector().z();
                float theta = quaternion.getAngle();

                of << w << " " << x << " " << y << " " << z << " " << theta << endl;
            }

            framesProcessed++;
            qDebug() << "Frame: " << framesProcessed;
        }

        qDebug() << "Close instance";
        dataInstance->close();
        of.close();
    }
}

void MainWindow::on_btnTest_clicked()
{
    //searchMinAndMaxDepth();

    //dai::DAIDataset dataset;
    dai::MSR3Action3D dataset;

    // Create instances
    //shared_ptr<dai::DataInstance> colorInstance = dataset.getColorInstance(1, 1, 1);
    shared_ptr<dai::DataInstance> depthInstance = dataset.getDepthInstance(17, 1, 2);
    //shared_ptr<dai::DataInstance> userInstance = dataset.getUserInstance(1, 1, 1);
    shared_ptr<dai::DataInstance> skeletonInstance = dataset.getSkeletonInstance(17, 1, 2);

    // Create playback to control instances reading
    dai::PlaybackControl* playback = new dai::PlaybackControl;
    playback->enablePlayLoop(true);
    connect(playback, &dai::PlaybackControl::onPlaybackFinished, playback, &dai::PlaybackControl::deleteLater);

    // Create Viewers
    //dai::InstanceViewerWindow* colorViewer = new dai::InstanceViewerWindow;
    //dai::InstanceViewerWindow* depthViewer = new dai::InstanceViewerWindow;
    //dai::InstanceViewerWindow* userViewer = new dai::InstanceViewerWindow;
    dai::InstanceViewerWindow* skeletonViewer = new dai::InstanceViewerWindow;
    //dai::TestListener* skeletonViewer = new dai::TestListener;

    // Connect all together
    //playback->addInstance(colorInstance);
    //playback->addInstance(depthInstance);
    //playback->addInstance(userInstance);
    playback->addInstance(skeletonInstance);
    playback->addInstance(depthInstance);

    //playback->addListener(colorViewer, colorInstance);
    //playback->addListener(colorViewer, userInstance);
    //playback->addNewFrameListener(depthViewer, depthInstance);
    //playback->addNewFrameListener(userViewer, userInstance);
    //playback->addNewFrameListener(recorder, userInstance);
    playback->addListener(skeletonViewer, skeletonInstance);
    playback->addListener(skeletonViewer, depthInstance);

    playback->play();
    skeletonViewer->show();
    //colorViewer->show();
    //depthViewer->show();
    //userViewer->show();
}

void MainWindow::on_btnStartKinect_clicked()
{
    // Create instance
    shared_ptr<dai::OpenNIColorInstance> colorInstance(new dai::OpenNIColorInstance);
    shared_ptr<dai::OpenNIDepthInstance> depthInstance(new dai::OpenNIDepthInstance);
    shared_ptr<dai::OpenNIUserInstance> userInstance(new dai::OpenNIUserInstance);
    shared_ptr<dai::OpenNISkeletonInstance> skeletonInstance(new dai::OpenNISkeletonInstance);
    /*colorInstance->setOutputFile("/ramfs/capture.rgb");
    depthInstance->setOutputFile("/ramfs/capture.bin");
    skeletonInstance->setOutputFile("/ramfs/capture.bskel");
    userInstance->setOutputFile("/ramfs/capture.user");*/
    //depthInstance->setOutputFile("/ramfs/jose/capture.rgb");

    // Create Playback
    dai::PlaybackControl* playback = new dai::PlaybackControl;
    connect(playback, &dai::PlaybackControl::onPlaybackFinished, playback, &dai::PlaybackControl::deleteLater);

    // Create viewers
    dai::InstanceViewerWindow* colorViewer = new dai::InstanceViewerWindow;
    dai::InstanceViewerWindow* depthViewer = new dai::InstanceViewerWindow;
    dai::InstanceViewerWindow* userViewer = new dai::InstanceViewerWindow;

    // Connect all together
    playback->addInstance(colorInstance);
    playback->addInstance(depthInstance);
    playback->addInstance(userInstance);
    playback->addInstance(skeletonInstance);

    playback->addListener(colorViewer, colorInstance);
    playback->addListener(depthViewer, depthInstance);
    playback->addListener(userViewer, userInstance);
    playback->addListener(depthViewer, skeletonInstance);

    // Run
    playback->play();
    colorViewer->show();
    depthViewer->show();
    userViewer->show();
}

void MainWindow::searchMinAndMaxDepth()
{
    dai::MSR3Action3D* dataset = new dai::MSR3Action3D();
    const dai::DatasetMetadata& dsMetadata = dataset->getMetadata();
    const dai::InstanceInfoList* instances = dsMetadata.instances(dai::InstanceInfo::Depth);

    QListIterator<dai::InstanceInfo*> it(*instances);
    int framesProcessed = 0;

    // Traverse all dataset instances
    while (it.hasNext())
    {
        dai::InstanceInfo* info = it.next();
        shared_ptr<dai::DataInstance> dataInstance = dataset->getDepthInstance(info->getActivity(), info->getActor(), info->getSample());

        dataInstance->open();

        qDebug() << "Instance" << info->getActivity() << info->getActor() << info->getSample() << "open";

        // Traverse each frame of this instance
        while (dataInstance->hasNext())
        {
            dataInstance->readNextFrame();
            //shared_ptr<dai::DepthFrame> depthFrame = static_pointer_cast<dai::DepthFrame>(dataInstance->frame());
            framesProcessed++;
            //qDebug() << "Frame: " << framesProcessed;
        }

        qDebug() << "Close instance";
        dataInstance->close();
    }
}
