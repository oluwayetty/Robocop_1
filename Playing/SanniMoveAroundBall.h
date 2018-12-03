#include <cmath>
#include <ctime>

option(SanniMoveAroundBall)
{
  initial_state(start)
  {
    transition
    {
      if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
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
      if (theLibCodeRelease.timeSinceBallWasSeen < 700)
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
      if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
        goto lookAround;

      if (theBallModel.estimate.position.norm() < 300.0) {
        c_start = std::clock(); // start the clock so that it'll turn left or right after some seconds
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
      duration += 5;
      if (duration >= 40) {
        temp1 = theBallModel.estimate.position.x() + 100.0;
        temp2 = theBallModel.estimate.position.y() - 50.0;
        goto turnRight;
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

  state(turnRight)
  {
    transition
    {
      duration = ( std::clock() - c_start ) / (double) CLOCKS_PER_SEC;
      duration += 20;
      if (duration >= 80) {
        duration = 0;
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
      if (sqrt(pow(theBallModel.estimate.position.x(),2) + pow(theBallModel.estimate.position.y(), 2) ) < 200.0)
        goto Skip;
    }
    action
    {
      lookAtBall();
      WalkToTargetPathPlanner(Pose2f(1.f, 1.f, 1.f),
                              Pose2f(theLibCodeRelease.rel2Glob(theBallModel.estimate.position.x(),
                                                             theBallModel.estimate.position.y())));
    }
  }

  state(Skip)
  {
    transition
    {
      if (sqrt(pow(theBallModel.estimate.position.x(),2) + pow(theBallModel.estimate.position.y(), 2) ) >= 200.0)
        goto walkToBall2;

    }
    action
    {
      Stand();
    }
  }
}