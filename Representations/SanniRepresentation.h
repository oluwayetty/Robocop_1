#pragma once
#include "Tools/Streams/Enum.h"
#include "Tools/Math/Eigen.h"
#include "Tools/Streams/AutoStreamable.h"
#include "../Src/Representations/Infrastructure/JointAngles.h"
#include "../Src/Representations/Perception/BallPercepts/BallPercept.h"

class SanniRepresentation : public Streamable
{
  private :
    void serialize(In *in, Out *out)
    {
      STREAM_REGISTER_BEGIN;
      STREAM(robotPose);
      STREAM(ballPoseRelative);
      STREAM(ballPoseGlobal);
      STREAM(jointValues);
      STREAM(ballPercept)
      STREAM_REGISTER_FINISH;
    }
  public:
    Vector3f robotPose;
    Vector2f ballPoseRelative;
    Vector2f ballPoseGlobal;
    JointAngles jointValues;
    BallPercept ballPercept;

    SanniRepresentation(){;}
};
