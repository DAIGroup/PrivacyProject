#ifndef DATASETINFO_H
#define DATASETINFO_H

#include <QString>

class DataSetInfo
{
public:
    DataSetInfo();
    /*getNumberOfSamples();
    getNumberOfActors();
    getNumberOfActivities();*/

private:
    QString m_path; // Path where the dataset is located

};

#endif // DATASETINFO_H
