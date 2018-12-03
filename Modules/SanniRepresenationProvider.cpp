#include "SanniRepresentationProvider.h"
#include <cmath>

MAKE_MODULE(SanniRepresentationProvider , behaviorControl)

SanniRepresentationProvider::SanniRepresentationProvider (){}

void SanniRepresentationProvider::update(SanniRepresentation &sanniRepresentation){


  sanniRepresentation.ballPoseRelative = theBallPercept.positionOnField;
  sanniRepresentation.ballPoseGlobal = theBallPercept.positionInImage;

  //  declaring as a multi-dimensional array
  sanniRepresentation.robotPose = { 
    theRobotPose.translation.x(), 
    theRobotPose.translation.y(),
    theRobotPose.rotation
  };

  //  fetching our joint angle values
  sanniRepresentation.jointValues = theJointAngles;

  //  calculating the position of our robot to the ball in 2meters
  // using the 

  float x = sanniRepresentation.ballPoseGlobal[0]  - sanniRepresentation.robotPose[0];
  float y = sanniRepresentation.ballPoseGlobal[1]  - sanniRepresentation.robotPose[1];
  float sum = pow(x, 2.0) + pow(y, 2.0);
  float length_of_vector = sqrt(sum);  // length of the vector(distance of the ball to the robot)

  // std::cout << "Length = " << length_of_vector << std::endl;

  if(length_of_vector > 2000) {
    // sanniRepresentation.
    sanniRepresentation.ballPercept.status = BallPercept::notSeen;
  }

}
