#ifndef SKELETONJOINT_H
#define SKELETONJOINT_H

#include "Point3f.h"
#include "Quaternion.h"
#include <QObject>

namespace dai {

class SkeletonJoint : public QObject
{
    Q_OBJECT
    Q_ENUMS(JointType)

public:
    enum JointType {
        JOINT_HEAD,

        JOINT_CENTER_SHOULDER, // NECK on OpenNI
        JOINT_LEFT_SHOULDER,
        JOINT_RIGHT_SHOULDER,

        JOINT_LEFT_ELBOW,
        JOINT_RIGHT_ELBOW,

        JOINT_LEFT_WRIST,
        JOINT_RIGHT_WRIST,

        JOINT_LEFT_HAND,
        JOINT_RIGHT_HAND,

        JOINT_SPINE,

        JOINT_CENTER_HIP, // TORSO on OpenNI
        JOINT_LEFT_HIP,
        JOINT_RIGHT_HIP,

        JOINT_LEFT_KNEE,
        JOINT_RIGHT_KNEE,

        JOINT_LEFT_ANKLE,
        JOINT_RIGHT_ANKLE,

        JOINT_LEFT_FOOT,
        JOINT_RIGHT_FOOT
    };

    explicit SkeletonJoint();
    explicit SkeletonJoint(const Point3f point);
    explicit SkeletonJoint(const Point3f point, const Quaternion orientation);
    SkeletonJoint(const SkeletonJoint& other);
    void setPosition(const Point3f point);
    void setScreenPosition(const Point3f point);
    void setType(JointType type);

    JointType getType() const;
    const Point3f& getPosition() const;
    const Point3f& getScreenPosition() const;
    const Quaternion& getOrientation() const;

    SkeletonJoint& operator=(const SkeletonJoint& other);

private:
    JointType   m_type;
    Point3f     m_realworld_point;    // Real World Point
    Point3f     m_screen_point; // Screen Point
    Quaternion  m_orientation;
};



} // End Namespace

#endif // SKELETONJOINT_H
