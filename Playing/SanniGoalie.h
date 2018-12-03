option(SanniGoalie)
{
    initial_state(start)
    {
        transition
        {
            if(theBallModel.estimate.position.norm()<1200)
                goto Dive;
        }
        action
        {
            WalkToTargetPathPlanner(Pose2f(1.f, 1.f, 1.f),
                                    Pose2f(theLibCodeRelease.rel2Glob(SPQR::GOALIE_BASE_POSITION_X,
                                                                      SPQR::GOALIE_BASE_POSITION_Y)));
            Stand();
            lookAtBall();

        }
    }

    state(Dive)
    {
        transition
        {
            if(state_time > 3000 )
                goto start;
        }
        action
        {
            lookAtBall();
            if(theBallModel.estimate.velocity.x()<-0.1){
                if(theBallModel.estimate.velocity.y()>0.1)
                    goalieFastDiveLeft();
                else
                    goalieFastDiveRight();
            }
        }

    }

}
