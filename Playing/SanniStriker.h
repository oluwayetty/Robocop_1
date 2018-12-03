// Write a striker behavior that makes the robot kicking 
// the ball towards its own goal

#include <cmath>
#include <ctime>

float temp1 = 0.0;
float temp2 = 0.0;
std::clock_t c_start;
double duration = 0;

option(SanniStriker)
{
  initial_state(start)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > 3000)
        goto lookAround;
      else
        goto walkToBall;
    }
    action
    {
      lookAtBall();
      Stand();
    }
  }

  state(lookAround)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < 700)
        goto walkToBall;
    }
    action
    {
      lookAtBall();
      WalkAtRelativeSpeed(Pose2f(0.6f, 0.f, 0.f));
    }
  }

  state(walkToBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > 3000)
        goto lookAround;

      if(sqrt(pow(theBallModel.estimate.position.x(),2) + pow(theBallModel.estimate.position.y(), 2) ) < 300.0) {
        c_start = std::clock();
        temp1 = theBallModel.estimate.position.x() + 200.0;
        temp2 = theBallModel.estimate.position.y() - 150.0;
        goto turnLeft;
      }
    }
    action
    {
      lookAtBall();
      WalkToTargetPathPlanner(Pose2f(1.f, 1.f, 1.f),
                              Pose2f(theLibCodeRelease.glob2Rel(theBallModel.estimate.position.x()+300.0,
                                                             theBallModel.estimate.position.y())));
    }
  }

  state(turnLeft)
  {
    transition
    {
      duration = ( std::clock() - c_start ) / (double) CLOCKS_PER_SEC;
      if(duration >= 40) {
        duration = 0;
        temp1 = theBallModel.estimate.position.x() + 100.0;
        temp2 = theBallModel.estimate.position.y() - 50.0;
        goto turnRight;
      }
    }
    action
    {
      if(temp1 == theRobotPose.translation.x() && temp2 == theRobotPose.translation.y()) {
        WalkToTargetPathPlanner(Pose2f(1.f, 1.f, 1.f),
                              Pose2f(theLibCodeRelease.glob2Rel(temp1,
                                                             (temp2))));

      }
    }
  }

  state(turnRight)
  {
    transition
    {
      duration = ( std::clock() - c_start ) / (double) CLOCKS_PER_SEC;
      if(duration >= 80) {
        goto walkToBall2;
      }
    }
    action
    {
      if (temp1 == theRobotPose.translation.x() && temp2 == theRobotPose.translation.y()) {
        WalkToTargetPathPlanner(Pose2f(1.f, 1.f, 1.f),
                              Pose2f(theLibCodeRelease.glob2Rel(temp1,
                                                             (temp2))));

      }
    }
  }

  state(walkToBall2)
  {
    transition
    {
      if(sqrt(pow(theBallModel.estimate.position.x(),2) + pow(theBallModel.estimate.position.y(), 2) ) < 200.0)
        goto kickToOwnGoal;
    }
    action
    {
      lookAtBall(); 
      WalkToTargetPathPlanner(Pose2f(1.f, 1.f, 1.f),
                              Pose2f(theLibCodeRelease.rel2Glob(theBallModel.estimate.position.x(),
                                                             theBallModel.estimate.position.y())));
    }
  }
  state(kickToOwnGoal)
  {
    transition
    {
      //Check if robot is not close to the ball
      if(sqrt(pow(theBallModel.estimate.position.x(),2) + pow(theBallModel.estimate.position.y(), 2) ) >= 200.0) 
        goto walkToBall2;
    }
    action
    {
      Kicks("forwardKick");
    }
  }
}