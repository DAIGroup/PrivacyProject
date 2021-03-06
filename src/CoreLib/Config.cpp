#include "Config.h"
#include <QThreadPool>

void CoreLib_InitResources()
{
    Q_INIT_RESOURCE(corelib);
}

namespace dai {

Config* Config::_instance = nullptr;
QMutex Config::_mutex;

Config* Config::getInstance()
{
    _mutex.lock();
    if (!_instance) {
        _instance = new Config;
    }
    _mutex.unlock();

    return _instance;
}

Config::Config()
{
    m_filters_enabled = true;
    //QThreadPool::globalInstance()->setMaxThreadCount(20);
}

void Config::enableFilters()
{
    m_filters_enabled = true;
}

void Config::disableFilters()
{
    m_filters_enabled = false;
}

bool Config::isFiltersEnabled() const
{
    return m_filters_enabled;
}

} // End namespace
