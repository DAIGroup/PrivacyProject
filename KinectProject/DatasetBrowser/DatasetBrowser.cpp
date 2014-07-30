#include "DatasetBrowser.h"
#include "ui_DatasetBrowser.h"
#include "dataset/MSRDaily/MSRDailyActivity3D.h"
#include "dataset/MSRAction3D/MSR3Action3D.h"
#include "dataset/HuDaAct/HuDaAct.h"
#include "playback/PlaybackControl.h"
#include "viewer/InstanceViewerWindow.h"
#include "InstanceWidgetItem.h"
#include "DatasetSelector.h"
#include <QTimer>
#include <QMessageBox>

using namespace dai;

DatasetBrowser::DatasetBrowser(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::DatasetBrowser), m_settings(this)
{
    ui->setupUi(this);

    m_playback.enablePlayLoop(true);
    m_playback.setFPS(10);
    m_dataset = nullptr;

    // First Setup
    if (m_settings.getMSRAction3D().isEmpty() || m_settings.getMSRDailyActivity3D().isEmpty()
            || m_settings.getHuDaAct().isEmpty()) {
        QTimer::singleShot(1, &m_settings, SLOT(show()));
    }

    // Connect Signals
    connect(ui->actionClose_dataset, SIGNAL(triggered()), this, SLOT(closeDataset()));
    connect(ui->actionOpen_dataset, SIGNAL(triggered()), this, SLOT(openDatasetSelector()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(openSettings()));
    connect(ui->listActivities, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listItemChange(QListWidgetItem*)));
    connect(ui->listActors, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listItemChange(QListWidgetItem*)));
    connect(ui->listSamples, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listItemChange(QListWidgetItem*)));
    connect(ui->listInstances, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(instanceItemActivated(QListWidgetItem*)));
    connect(ui->comboType, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChange(int)));
}

DatasetBrowser::~DatasetBrowser()
{
    ui->listActivities->disconnect();
    ui->listActors->disconnect();
    ui->listSamples->disconnect();
    ui->listInstances->disconnect();
    ui->comboType->disconnect();

    closeDataset();
    delete ui;
}

void DatasetBrowser::openSettings()
{
    if (m_settings.exec() == QDialog::Accepted) {
        closeDataset();
    }
}

void DatasetBrowser::openDatasetSelector()
{
    DatasetSelector* window = new DatasetSelector(this);

    if (window->exec() != QDialog::Rejected) {
        loadDataset( (Dataset::DatasetType) (window->result()-1) );
    }
}

void DatasetBrowser::closeDataset()
{
    ui->comboType->blockSignals(true);

    if (m_dataset != nullptr) {
        delete m_dataset;
        m_dataset = nullptr;
    }

    ui->listActivities->clear();
    ui->listActors->clear();
    ui->listSamples->clear();
    ui->listInstances->clear();
    ui->comboType->clear();

    ui->comboType->blockSignals(false);
}

void DatasetBrowser::listItemChange(QListWidgetItem * item)
{
    Q_UNUSED(item);
    loadInstances();
}

void DatasetBrowser::comboBoxChange(int index)
{
    Q_UNUSED(index);
    loadInstances();
}

void DatasetBrowser::instanceItemActivated(QListWidgetItem * item)
{
    InstanceWidgetItem* instanceItem = dynamic_cast<InstanceWidgetItem*>(item);
    InstanceInfo& info = instanceItem->getInfo();
    shared_ptr<StreamInstance> instance = m_dataset->getInstance(info, m_showType);

    if (instance)
    {
        InstanceViewerWindow* windowViewer = new InstanceViewerWindow;

        m_playback.stop();
        m_playback.clearInstances();
        m_playback.addInstance(instance);
        m_playback.addListener(&m_depthFilter);
        m_depthFilter.addListener(windowViewer);

        try {
            m_playback.play();
            windowViewer->setTitle("Instance Viewer");
            windowViewer->show();
        }
        catch (CannotOpenInstanceException)
        {
            QMessageBox::warning(this, tr("Error reading the instance"),
                                           tr("The selected instance cannot be opened maybe\n"
                                              "due to file permissions or wrong dataset path"));
        }
    }
}

void DatasetBrowser::loadDataset(Dataset::DatasetType type)
{
    closeDataset();

    ui->listActivities->blockSignals(true);
    ui->listActors->blockSignals(true);
    ui->listSamples->blockSignals(true);
    ui->comboType->blockSignals(true);

    if (type == Dataset::Dataset_MSRDailyActivity3D) {
        m_dataset = new MSRDailyActivity3D;
        m_dataset->setPath(m_settings.getMSRDailyActivity3D());
    } else if (type == Dataset::Dataset_MSRAction3D) {
        m_dataset = new MSR3Action3D;
        m_dataset->setPath(m_settings.getMSRAction3D());
    } else if (type == Dataset::Dataset_HuDaAct) {
        m_dataset = new HuDaAct;
        m_dataset->setPath(m_settings.getHuDaAct());
    }

    // Load widgets with DataSet Info
    const DatasetMetadata& dsMetaData = m_dataset->getMetadata();

    // Load Activities
    for (int i=1; i<=dsMetaData.getNumberOfActivities(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(dsMetaData.getActivityName(i), ui->listActivities);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Checked);
    }

    // Load Actors
    foreach (int actor, dsMetaData.actors()) {
        QListWidgetItem* item = new QListWidgetItem(dsMetaData.getActorName(actor), ui->listActors);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Checked); // AND initialize check state
    }

    // Load Samples
    for (int i=1; i<=dsMetaData.getNumberOfSampleTypes(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(dsMetaData.getSampleName(i), ui->listSamples);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Checked); // AND initialize check state
    }

    // Load Instance Types
    const DataFrame::SupportedFrames supportedFrames = dsMetaData.availableInstanceTypes();

    if (supportedFrames.testFlag(DataFrame::Depth)) {
        ui->comboType->addItem("Depth", QVariant(DataFrame::Depth));
    }
    if (supportedFrames.testFlag(DataFrame::Color)) {
        ui->comboType->addItem("Color", QVariant(DataFrame::Color));
    }
    if (supportedFrames.testFlag(DataFrame::Skeleton)) {
        ui->comboType->addItem("Skeleton", QVariant(DataFrame::Skeleton));
    }
    if (supportedFrames.testFlag(DataFrame::Mask)) {
        ui->comboType->addItem("User", QVariant(DataFrame::Mask));
    }

    ui->listActivities->blockSignals(false);
    ui->listActors->blockSignals(false);
    ui->listSamples->blockSignals(false);
    ui->comboType->blockSignals(false);
    loadInstances();
}

void DatasetBrowser::loadInstances()
{
    const DatasetMetadata& dsMetadata = m_dataset->getMetadata();

    ui->listInstances->clear();

    // Prepare Filter
    m_showType = (DataFrame::FrameType) ui->comboType->itemData(ui->comboType->currentIndex()).toInt();
    QList<int> activities;

    for (int i=0; i<ui->listActivities->count(); ++i) {
        QListWidgetItem* item = ui->listActivities->item(i);
        if (item != 0 && item->checkState() == Qt::Checked)
            activities.append(i+1);
    }

    QList<int> actors;

    for (int i=0; i<ui->listActors->count(); ++i) {
        QListWidgetItem* item = ui->listActors->item(i);
        if (item != 0 && item->checkState() == Qt::Checked)
            actors.append(item->text().toInt());
    }

    QList<int> samples;

    for (int i=0; i<ui->listSamples->count(); ++i) {
        QListWidgetItem* item = ui->listSamples->item(i);
        if (item != 0 && item->checkState() == Qt::Checked)
            samples.append(i+1);
    }

    // Get and Load Instances
    const InstanceInfoList instances = dsMetadata.instances(m_showType, &activities, &actors, &samples);

    for (int i=0; i<instances.count(); ++i) {
        InstanceInfo* instanceInfo = instances.at(i);
        InstanceWidgetItem* item = new InstanceWidgetItem(instanceInfo->getFileName(m_showType), ui->listInstances);
        item->setInfo(*instanceInfo);
    }
}

void DatasetBrowser::on_btnSelectAllActivities_clicked()
{
    for (int i=0; i<ui->listActivities->count(); ++i) {
        QListWidgetItem* item = ui->listActivities->item(i);
        item->setCheckState(Qt::Checked);
    }
}

void DatasetBrowser::on_btnUnselectAllActivities_clicked()
{
    for (int i=0; i<ui->listActivities->count(); ++i) {
        QListWidgetItem* item = ui->listActivities->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}

void DatasetBrowser::on_btnSelectAllActors_clicked()
{
    for (int i=0; i<ui->listActors->count(); ++i) {
        QListWidgetItem* item = ui->listActors->item(i);
        item->setCheckState(Qt::Checked);
    }
}

void DatasetBrowser::on_btnUnselectAllActors_clicked()
{
    for (int i=0; i<ui->listActors->count(); ++i) {
        QListWidgetItem* item = ui->listActors->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}

void DatasetBrowser::on_btnSelectAllSamples_clicked()
{
    for (int i=0; i<ui->listSamples->count(); ++i) {
        QListWidgetItem* item = ui->listSamples->item(i);
        item->setCheckState(Qt::Checked);
    }
}

void DatasetBrowser::on_btnUnselectAllSamples_clicked()
{
    for (int i=0; i<ui->listSamples->count(); ++i) {
        QListWidgetItem* item = ui->listSamples->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}
