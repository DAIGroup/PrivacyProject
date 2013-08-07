#ifndef DAIUSERINSTANCE_H
#define DAIUSERINSTANCE_H

#include <fstream>
#include "types/DataInstance.h"
#include "types/UserFrame.h"

using namespace std;

namespace dai {

class DAIUserInstance : public DataInstance<UserFrame>
{
public:
    explicit DAIUserInstance(const InstanceInfo& info);
    virtual ~DAIUserInstance();
    bool is_open() const override;

protected:
    void openInstance() override;
    void closeInstance() override;
    void restartInstance() override;
    void nextFrame(UserFrame& frame) override;

private:
    ifstream    m_file;
    int         m_width;
    int         m_height;
    shared_ptr<UserFrame> m_frameBuffer[2];
};

} // End Namespace

#endif // DAIUSERINSTANCE_H
