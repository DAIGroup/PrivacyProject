#ifndef SINBAD_H
#define SINBAD_H

#include <Ogre.h>
#include <OgreStringConverter.h>
#include <OgreErrorDialog.h>
#include <memory>
#include "types/Skeleton.h"

#define RUN_SPEED   17         // character running speed in units per second
#define TURN_SPEED  500.0f     // character turning in degrees per second

using namespace std;

class SinbadCharacterController
{
public:
    SinbadCharacterController(Ogre::Camera* cam);
    void addTime(Ogre::Real deltaTime);
    void setSkeleton(shared_ptr<dai::Skeleton> skeleton);
    void newUser(int userId);
    void lostUser(int userId);
    void setVisible(bool visible);

private:
    void setupBody(Ogre::SceneManager* sceneMgr);
    void setupBone(const Ogre::String &name, const Ogre::Radian &angle, const Ogre::Vector3 axis);
    void setupBone(const Ogre::String &name, const Ogre::Degree &yaw, const Ogre::Degree &pitch, const Ogre::Degree &roll);
    void setupBone(const Ogre::String &name, const Ogre::Quaternion &q);
    void setupAnimations();
    void resetBonesToInitialState();
    void transformBone(const Ogre::String& modelBoneName, dai::SkeletonJoint::JointType jointType);
    void PSupdateBody(Ogre::Real deltaTime);
    void updateBody(Ogre::Real deltaTime);

    bool m_enabled;
    Ogre::Vector3 m_origTorsoPos;
    Ogre::AnimationState* mAnimIdle;
    Ogre::SceneNode* mBodyNode;
    Ogre::SceneNode* mCameraNode;
    Ogre::Entity* mBodyEnt;
    Ogre::Vector3 mKeyDirection;      // player's local intended direction based on WASD keys
    Ogre::Vector3 mGoalDirection;     // actual intended direction in world-space
    Ogre::Real mTimer;                // general timer to see how long animations have been playing
    shared_ptr<dai::Skeleton> m_skeleton;
    bool m_userVisible;
};

#endif
