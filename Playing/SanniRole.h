// Test everything simulating two robots (striker and goalie).

option(SanniRole)
{
  initial_state(start)
  {
    transition
    {
      if (theLibCodeRelease.timeSinceBallWasSeen > 3000) // confirms it's been long the ball has been seen.
        goto lookAround; // then look for the ball
      else
        goto walkToBall; // if shorter, then go to the ball
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
        goto walkToBall; // go to the ball when last seen is a short time
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
        goto lookAround ;
      if (theBallModel.estimate.position.norm() < 500) // what is norm?
        goto moveAroundBall;
    }
    action
    {
      lookAtBall();
      WalkToTargetPathPlanner(Pose2f(1.f, 1.f, 1.f),
                              Pose2f(theLibCodeRelease.rel2Glob(theBallModel.estimate.position.x(),
                                                             theBallModel.estimate.position.y())));
    }
  }

  state(moveAroundBall)
  {
    transition
    {
      if (theLibCodeRelease.timeSinceBallWasSeen > 3000)
          goto lookAround ;

      if(theBallModel.estimate.position.norm() > 600) {
          goto walkToBall;
      }
      float timeVar = (state_time / 1000);

      if(std::abs(theLibCodeRelease.angleToGoal) <= Angle::fromDegrees(3.f + timeVar)) {
          if (theLibCodeRelease.between(theBallModel.estimate.position.x(), 100, 300)) {
              if (theLibCodeRelease.between(std::abs(theBallModel.estimate.position.y()),
                                            -50, 50 )) {
                  goto kick;
              }
          }
      }
    }
    action
      {
        lookAtBall();
        WalkToTarget(Pose2f(50.f, 50.f, 50.f),
                            Pose2f(theLibCodeRelease.angleToTarget(theFieldDimensions.xPosOpponentGoal,
                                                                  theFieldDimensions.yPosLeftGoal),
                                  theBallModel.estimate.position.x()-200,
                                  theBallModel.estimate.position.y()));
      }
  }

  state(kick)
  {
    transition
      {
          if(state_time > 3000 || (state_time > 10 && action_done))
              goto lookAround;
      }
  action
    {
        lookAtBall();
        Kicks("fastForwardKick");
    }
  }

};