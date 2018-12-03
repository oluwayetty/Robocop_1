// SanniRepresentation
#include "Representations/HandsIn/Assignment/SanniRepresentation.h"
#include "Representations/Perception/BallPercepts/BallPercept.h"
#include "Representations/Modeling/RobotPose.h"
#include "../Src/Representations/Infrastructure/JointAngles.h"
#include "Tools/Module/Module.h"
#include <iostream>

MODULE (SanniRepresentationProvider,
{,
  REQUIRES(BallPercept),
  REQUIRES(RobotPose),
  REQUIRES(JointAngles),
  PROVIDES(SanniRepresentation),
});

class SanniRepresentationProvider : public SanniRepresentationProviderBase {
  public :
  SanniRepresentationProvider();
  void update(SanniRepresentation &sanniRepresentation);
};
