option(SanniDisregard)
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
        //Find the distance of the ball to the robot
      if(sqrt(pow(theBallModel.estimate.position.x(),2) + pow(theBallModel.estimate.position.y(), 2) ) < 350.0) {
        Stand();
      }
    }
    action
    {
      lookAtBall();
      //Walk to ball if it is lesser than or equal to 2m away, else, disregard ball
      if (sqrt(pow(theBallModel.estimate.position.x(),2) + pow(theBallModel.estimate.position.y(), 2) ) <= 2000.0) {
        WalkToTargetPathPlanner(Pose2f(1.f, 1.f, 1.f), // take two args- speed & target
                              Pose2f(theLibCodeRelease.rel2Glob(theBallModel.estimate.position.x(),
                                                             theBallModel.estimate.position.y())));
      }
      
    }
  }
  
}