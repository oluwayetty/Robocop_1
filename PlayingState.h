#define SINGLE_ROLE
#define getExtraTime() ( (theGameInfo.kickingTeam != theOwnTeamInfo.teamNumber) ? 10000.f : 0.f)

option(PlayingState)
{
    initial_state(demo)
    {
        action
        {

#ifndef SINGLE_ROLE

            if((int)theGameInfo.setPlay == 0 || (((int)theGameInfo.setPlay == 2)&&((int)theGameInfo.kickingTeam==19)) || (((int)theGameInfo.setPlay == 1)&&((int)theGameInfo.kickingTeam==19))){

                if(theRole.role == Role::RoleType::goalie)
                    Goalie();
                else if(theRole.role== Role::RoleType::defender)
                    Defender();
                else if(theRole.role== Role::RoleType::supporter)
                    Supporter();
                else if(theRole.role == Role::RoleType::jolly) {
                    /*if (state_time < (40000.f + getExtraTime())) StaticJolly();
                    else*/ SimpleJolly();
                } else if(theRole.role == Role::RoleType::striker)
                    StrikerDev();

                //Search
                else if(theRole.role == Role::RoleType::searcher_1)
                    Guard(1);
                else if(theRole.role == Role::RoleType::searcher_2)
                    Guard(2);
                else if(theRole.role == Role::RoleType::searcher_3)
                    Searcher(3);
                else if(theRole.role == Role::RoleType::searcher_4)
                    Searcher(4);
            }
            else if(((int)theGameInfo.setPlay == 2)&&((int)theGameInfo.kickingTeam!=19)){
                if(theRole.role == Role::RoleType::striker || theRole.role == Role::RoleType::searcher_3)
                {
                    if(theBallModel.estimate.position.norm() < 950)
                    {
                        //                          Pose2f globBall = theLibCodeRelease.rel2Glob(theBallModel.estimate.position.x(), theBallModel.estimate.position.y());
                        Pose2f globBall = theTeamBallModel.position;
                        Pose2f result;

                        if(globBall.translation.x() < theRobotPose.translation.x()-100 && std::abs(globBall.translation.y() - theRobotPose.translation.y()) < 150)
                        {
                            if(globBall.translation.y() < theRobotPose.translation.y())
                                result = Pose2f(globBall.translation.x() - 1000, globBall.translation.y()+8000);
                            else
                                result = Pose2f(globBall.translation.x() - 1000, globBall.translation.y()-8000);
                        }
                        else
                            result = Pose2f(globBall.translation.x() - 1000, globBall.translation.y());


                        if(std::abs(result.translation.x() - theLibCodeRelease.defenderPosition.x()) > 500)
                            WalkToTarget(Pose2f( 1.f, 1.f, 1.f), theLibCodeRelease.glob2Rel(result.translation.x(), result.translation.y()));
                        else
                        {
                            if(globBall.translation.x() >= theRobotPose.translation.x()-100 && std::abs(globBall.translation.y() - theRobotPose.translation.y()) < 150){

                                if(globBall.translation.y() < theRobotPose.translation.y())
                                    result = Pose2f(globBall.translation.x() + 2000, globBall.translation.y()+8000);
                                else
                                    result = Pose2f(globBall.translation.x() + 2000, globBall.translation.y()-8000);

                                WalkToTarget(Pose2f( 1.f, 1.f, 1.f), theLibCodeRelease.glob2Rel(result.translation.x(), result.translation.y()));
                            }
                            else
                            {
                                result = Pose2f(result.translation.x() + 2000, globBall.translation.y());
                                WalkToTarget(Pose2f( 1.f, 1.f, 1.f), theLibCodeRelease.glob2Rel(result.translation.x(), result.translation.y()));
                            }
                        }
                    }
                    else
                    {
                        Stand();
                        lookAtBall();
                    }
                }
                else
                {
                    Pose2f readyPose(theLibCodeRelease.getReadyPose(true, theRole.role ));
                    GetIntoReadyPosition(Pose2f( .6f, 70.f, 20.f), readyPose.translation );
                }


            }
            else if ((int)theGameInfo.setPlay == 1 && (int)theGameInfo.kickingTeam != 19){

                if(theRobotInfo.number == 1)
                    Goalie();
                else
                {
                    //if( theGameInfo.kickingTeam != theOwnTeamInfo.teamColor )
                    //{

                    Pose2f readyPose(theLibCodeRelease.getReadyPose(true, theRole.role ));
                    GetIntoReadyPosition(Pose2f( .6f, 70.f, 20.f), readyPose.translation );

                    //                       }
                }
            }


#else
    if (theRobotInfo.number == 1)
        SanniGoalie(); //the goalie behaviour
    else if(theRobotInfo.number == 2)
        //SanniMoveAroundBall(); //behavior to make the robot walk around the ball
        //SanniDisregard(); //behaviour to make robot disregard ball that are more than 2m away from its' location
        // SanniStriker(); //behaviour for the striker robot to kick the ball towards its own goal
        SanniRole(); //free kick behaviour for goalie and striker with two defenders
#endif
        }
    }
}

