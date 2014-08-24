#include "OpenNIColorInstance.h"
#include <exception>
#include <iostream>

using namespace std;

namespace dai {

OpenNIColorInstance::OpenNIColorInstance()
    : StreamInstance(DataFrame::Color, 640, 480)
{
    m_device = nullptr;
}

OpenNIColorInstance::OpenNIColorInstance(OpenNIDevice* device)
    : StreamInstance(DataFrame::Color, 640, 480)
{
    m_device = device;
}

OpenNIColorInstance::~OpenNIColorInstance()
{
    closeInstance();
}

bool OpenNIColorInstance::is_open() const
{
    return m_device != nullptr && m_device->is_open();
}

bool OpenNIColorInstance::openInstance()
{
    bool result = false;

    if (!is_open())
    {
        if (m_device == nullptr)
            m_device = OpenNIDevice::create("ANY_DEVICE");
        m_device->open();
        result = true;
    }

    return result;
}

void OpenNIColorInstance::closeInstance()
{
    if (is_open())
    {
        m_device->close();
    }
}

void OpenNIColorInstance::restartInstance()
{
}

void OpenNIColorInstance::nextFrame(QHashDataFrames &output)
{
    Q_ASSERT(output.size() > 0);
    shared_ptr<ColorFrame> colorFrame = static_pointer_cast<ColorFrame>(output.value(DataFrame::Color));
    m_device->readColorFrame(colorFrame);
}

} // End namespace
