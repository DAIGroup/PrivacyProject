#include "MSRActionDepthInstance.h"
#include "types/DepthFrame.h"
#include <cstddef>
#include <math.h>
#include <stdint.h>
#include <QDebug>

using namespace std;

namespace dai {

MSRActionDepthInstance::MSRActionDepthInstance(const InstanceInfo &info)
    : DataInstance(info), m_currentFrame(320, 240)
{
    m_nFrames = 0;
    m_width = 0;
    m_height = 0;
    m_frameIndex = 0;
}

MSRActionDepthInstance::~MSRActionDepthInstance()
{
    close();
}

void MSRActionDepthInstance::open()
{
    QString instancePath = m_info.getDatasetPath() + "/" + m_info.getFileName();
    m_file.open(instancePath.toStdString().c_str(), ios::in|ios::binary);

    if (!m_file.is_open()) {
        cerr << "Error opening file" << endl;
        return;
    }

    m_file.read((char *) &m_nFrames, 4);
    m_file.read((char *) &m_width, 4);
    m_file.read((char *) &m_height, 4);

    if (m_width != 320 || m_height != 240)
        exit(1);
}

void MSRActionDepthInstance::close()
{
    if (m_file.is_open()) {
        m_file.close();
    }

    m_nFrames = 0;
    m_width = 0;
    m_height = 0;
    m_frameIndex = 0;
}

int MSRActionDepthInstance::getTotalFrames()
{
    return m_nFrames;
}

bool MSRActionDepthInstance::hasNext()
{
    if (m_frameIndex < m_nFrames || m_playLoop)
        return true;

    return false;
}

const DepthFrame &MSRActionDepthInstance::nextFrame()
{
    if (m_playLoop) {
        if (m_frameIndex == m_nFrames) {
            m_frameIndex = 0;
            m_file.seekg(12, ios_base::beg);
        }
    }

    if (m_frameIndex < m_nFrames)
    {
        m_currentFrame.setIndex(m_frameIndex);

        // Read Data from File
        BinaryDepthFrame tempFrame[240]; // I know MSR Action 3D depth is 320 x 240
        m_file.read( (char *) tempFrame, sizeof(tempFrame) );
        int *data = m_currentFrame.getDataPtr();

        for (int r=0; r<m_height; r++)
        {
            memcpy(data, tempFrame[r].depthRow, m_width * sizeof(int));
            data += m_width;
        }

        m_frameIndex++;
    }
    else {
        close();
    }

    return m_currentFrame;
}

} // End Namespace
