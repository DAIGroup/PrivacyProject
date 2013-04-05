#ifndef MSRDAILYACT3D_H
#define MSRDAILYACT3D_H

#include "Dataset.h"
#include <iostream>

using namespace std;

namespace dai {

class MSRDailyActivity3D : public Dataset
{
public:
    explicit MSRDailyActivity3D();
    DataInstance& getDepthInstance(int activity, int actor, int sample);
    DataInstance& getColorInstance(int activity, int actor, int sample);
};

}

#endif // MSRDAILYACT3D_H
