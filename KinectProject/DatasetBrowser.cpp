#include "DatasetBrowser.h"
#include "ui_DatasetBrowser.h"
#include <QDebug>
#include "dataset/MSRDailyActivity3D.h"
#include "dataset/MSRDailyActivity3DInstance.h"
#include "InstanceWidgetItem.h"
#include "viewer/basicviewer.h"

using namespace dai;


DatasetBrowser::DatasetBrowser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DatasetBrowser)
{
    ui->setupUi(this);

    // MSR Daily Activity 3D
    if (ui->comboDataset->currentIndex() == 0) {
        m_dataset = new MSRDailyActivity3D();
    } else {
        m_dataset = NULL;
    }

    // Load widgets with DataSet Info
    const DatasetMetadata& info = m_dataset->getMetadata();

    // Load Activities
    for (int i=1; i<=info.getNumberOfActivities(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(info.getActivityName(i), ui->listActivities);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Checked);
    }

    // Load Actors
    for (int i=1; i<=info.getNumberOfActors(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(info.getActorName(i), ui->listActors);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Checked); // AND initialize check state
    }

    // Load Samples
    for (int i=1; i<=info.getNumberOfSampleTypes(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(info.getSampleName(i), ui->listSamples);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Checked); // AND initialize check state
    }

    loadInstances();

    // Connect Signals
    connect(ui->listActivities, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listItemChange(QListWidgetItem*)));
    connect(ui->listActors, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listItemChange(QListWidgetItem*)));
    connect(ui->listSamples, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listItemChange(QListWidgetItem*)));
    connect(ui->listInstances, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(instanceItemActivated(QListWidgetItem*)));
    connect(ui->comboType, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChange(int)));
}

DatasetBrowser::~DatasetBrowser()
{
    ui->listActivities->clear();
    ui->listActors->clear();
    ui->listSamples->clear();
    ui->listInstances->clear();
    delete ui;
}

void DatasetBrowser::closeEvent(QCloseEvent * event)
{
    Q_UNUSED(event);
    QMainWindow* parent =  dynamic_cast<QMainWindow*>(this->parent());
    parent->show();
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
    MSRDailyActivity3DInstance* instance = new MSRDailyActivity3DInstance(info);

    //DataInstance& instance = m_dataset->getDepthInstance(info.getActivity(), info.getActor(), info.getSample());

    BasicViewer* basicViewer = new BasicViewer(this);
    basicViewer->play(instance);
    basicViewer->show();
}

void DatasetBrowser::loadInstances()
{
    const DatasetMetadata& dsMetadata = m_dataset->getMetadata();

    ui->listInstances->clear();

    InstanceType showType;

    if (ui->comboType->currentIndex() == 0) {
        showType = Depth;
    } else if (ui->comboType->currentIndex() == 1) {
        showType = Color;
    } else if (ui->comboType->currentIndex() == 2) {
        showType = Skeleton;
    }

    // Prepare Filter
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
            actors.append(i+1);
    }

    QList<int> samples;

    for (int i=0; i<ui->listSamples->count(); ++i) {
        QListWidgetItem* item = ui->listSamples->item(i);
        if (item != 0 && item->checkState() == Qt::Checked)
            samples.append(i+1);
    }

    // Get and Load Instances
    const InstanceInfoList* instances = dsMetadata.instances(showType, &activities, &actors, &samples);

    for (int i=0; i<instances->count(); ++i) {
        InstanceInfo* instanceInfo = instances->at(i);
        InstanceWidgetItem* item = new InstanceWidgetItem(instanceInfo->getFileName(), ui->listInstances);
        item->setInfo(*instanceInfo);


        /*InstanceInfo* instance = instances->at(i);
        ui->listInstances->addItem(instance->getFile());*/
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
