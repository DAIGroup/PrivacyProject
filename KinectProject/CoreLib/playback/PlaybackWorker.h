#ifndef PLAYBACKWORKER_H
#define PLAYBACKWORKER_H

#include <QObject>
#include "NodeProducer.h"
#include <QList>
#include <memory>
#include "types/StreamInstance.h"

using namespace std;

namespace dai {

class NodeListener;

class PlaybackWorker : public QObject, public NodeProducer
{
    Q_OBJECT

    friend class PlaybackControl;
    friend class NodeListener;

public:
    PlaybackWorker();
    ~PlaybackWorker();

public slots:
    void run();
    void stop();

protected:
    QHashDataFrames produceFrames() override;

private:
    void enablePlayLoop(bool value);
    void setFPS(float fps);
    bool addInstance(shared_ptr<StreamInstance> instance);
    void removeInstance(shared_ptr<StreamInstance> instance);
    void clearInstances();
    void setupListeners();
    void openAllInstances();
    void closeAllInstances();

    QList<shared_ptr<StreamInstance>>  m_instances;
    bool                               m_playloop_enabled;
    qint64                             m_slotTime;
    bool                               m_running;
    DataFrame::SupportedFrames         m_supportedFrames;
};

} // End Namespace

#endif // PLAYBACKWORKER_H
