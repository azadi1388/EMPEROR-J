
/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *EndCopyright:
 */

//Student Soccer 2D Simulation Base , STDAGENT2D
//Simplified the Agent2D Base for HighSchool Students.
//Technical Committee of Soccer 2D Simulation League, IranOpen
//Nader Zare
//Mostafa Sayahi
//Pooria Kaviani
/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <rcsc/action/body_kick_one_step.h>
#include "bhv_basic_offensive_kick.h"
#include <rcsc/action/body_hold_ball.h>
#include <rcsc/action/body_turn_to_ball.h>
#include <rcsc/action/body_smart_kick.h>
#include <rcsc/action/body_stop_ball.h>
#include <rcsc/player/player_agent.h>
#include <rcsc/player/debug_client.h>
#include <rcsc/player/intercept_table.h>
#include <random>
#include <rcsc/common/logger.h>
#include <rcsc/common/server_param.h>
#include <math.h>
#include <rcsc/geom/sector_2d.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include "sample_player.h"

#include "bhv_basic_offensive_kick.h"
#include "bhv_basic_move.h"
#include "bhv_goalie_basic_move.h"

#include "sample_communication.h"

#include "bhv_penalty_kick.h"
#include "bhv_set_play.h"

#include "bhv_custom_before_kick_off.h"
#include "bhv_basic_offensive_kick.h"

#include "intention_receive.h"

#include <rcsc/action/basic_actions.h>
#include <rcsc/action/bhv_emergency.h>
#include <rcsc/action/body_go_to_point.h>
#include <rcsc/action/body_intercept.h>
#include <rcsc/action/body_kick_one_step.h>
#include <rcsc/action/view_synch.h>

#include <rcsc/action/kick_table.h>
#include <rcsc/player/intercept_table.h>
#include <rcsc/player/say_message_builder.h>
#include <rcsc/player/audio_sensor.h>
#include <rcsc/player/freeform_parser.h>

#include <rcsc/common/basic_client.h>
#include <rcsc/common/logger.h>
#include <rcsc/common/server_param.h>
#include <rcsc/common/player_param.h>
#include <rcsc/common/audio_memory.h>
#include <rcsc/common/say_message_parser.h>
#include <rcsc/common/free_message_parser.h>

#include <rcsc/param/param_map.h>
#include <rcsc/param/cmd_line_parser.h>

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "intention_receive.h"
#include "bhv_basic_move.h"
#include <string>
#include "bhv_basic_tackle.h"

#include <rcsc/action/basic_actions.h>
#include <rcsc/action/body_go_to_point.h>
#include <rcsc/action/body_intercept.h>

#include <rcsc/player/player_agent.h>
#include <rcsc/player/debug_client.h>
#include <rcsc/player/intercept_table.h>

#include <rcsc/common/logger.h>
#include <rcsc/common/server_param.h>
#include <rcsc/action/body_kick_one_step.h>
#include "bhv_basic_offensive_kick.h"
#include <rcsc/action/body_hold_ball.h>
#include <rcsc/action/body_turn_to_ball.h>
#include <rcsc/action/body_smart_kick.h>
#include <rcsc/action/body_stop_ball.h>
#include <rcsc/player/player_agent.h>
#include <rcsc/player/debug_client.h>
#include <rcsc/player/intercept_table.h>
#include <random>
#include <rcsc/common/logger.h>
#include <rcsc/common/server_param.h>
#include <math.h>
#include <rcsc/geom/sector_2d.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <vector>
#include <cstdio>
#include <fstream>

#include <rcsc/rcg/types.h>
#include <rcsc/rcg/util.h>
using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

 */


bool
Bhv_BasicOffensiveKick::execute( PlayerAgent * agent )
{
    const WorldModel &wm = agent->world();
    const rcsc::ServerParam & param = rcsc::ServerParam::i();
    const rcsc::Vector2D target = param.theirTeamGoalPos();
    /*if(MakeDecision(agent)){
        return true;
    }*/
    if((agent->world().gameMode().type() == rcsc::GameMode::KickIn_ || agent->world().gameMode().type() == rcsc::GameMode::FreeKick_ ||agent->world().gameMode().type() == rcsc::GameMode::CornerKick_ || agent->world().gameMode().type() == rcsc::GameMode::KickOff_)&& wm.gameMode().side() == wm.ourSide())
    {
        if(pass_to_forward(agent))
        {
            return true ;
        }
        if(pass(agent))
        {
            return true ;
        }
    }
    /*if(agent->world().ball().pos().x>50 && agent->world().ball().pos().dist(target) < 6)
    {
        if ( rcsc::Body_SmartKick( target,
                                   param.ballSpeedMax(),
                                   param.ballSpeedMax() * 0.96,
                                   2 ).execute( agent ) )
        {
            return true;
        }

        if ( rcsc::Body_KickOneStep( target, param.ballSpeedMax() )
             .execute( agent ) )
        {
            return true;
        }
    }*/
        Vector2D ball_pos = ( wm.self().isKickable() ? wm.ball().pos()  : wm.ball().pos() + wm.ball().vel() );
        const PlayerPtrCont &opps = wm.opponentsFromSelf();
        const PlayerObject * nearest_opp= ( opps.empty() ? static_cast< PlayerObject * >( 0 ): opps.front() );
        const double nearest_opp_dist = ( nearest_opp? nearest_opp->pos().dist(ball_pos) : 1000.0 );
        const Vector2D nearest_opp_pos = ( nearest_opp  ? nearest_opp->pos(): Vector2D( -1000.0, 0.0 ) );
        if(shoot(agent) && wm.gameMode().type() != GameMode::FreeKick_){
         	return true;
        }
        /*if(main_shoot(agent))
            return true;*/
        if(clear_sector(agent) && wm.ball().pos().dist(Vector2D(52, 0)) > 15)
        {
            Body_SmartKick(Vector2D(52.5,0),0.9,0.9,2).execute(agent);
            return true;
        }
         if( pass_fazayy(agent))
                return true;

        if(wm.self().goalie())
            if(pass(agent))
                return true;


        if(wm.getDistOpponentNearestToBall(4, true) < 10){
         if(pass_to_forward(agent))
            return true;}

         if(wm.getDistOpponentNearestToBall(4, true) < 5.2){
            if(pass(agent))
                return true;}
        Vector2D center(52.5,0);
        AngleDeg polar = (center-wm.self().pos()).th().degree();
        Sector2D check = Sector2D(wm.self().pos(),0,6,polar + 20,polar - 20);
        if(!wm.existOpponentIn(check,5,false))
        {
            if(dribble(agent))
            {
                return true ;
            }
        }
        if(!clear_sector(agent)){
        if(GoalPass(agent))
            return true;
        }
        if(dribble(agent) && wm.gameMode().type() == GameMode::PlayOn)
            return true;

        if(GoalPass(agent))
            return true;
        Body_HoldBall().execute(agent);
        clearball(agent);
        return true;


}


bool Bhv_BasicOffensiveKick::shoot( rcsc::PlayerAgent * agent )
{
    const WorldModel &wm = agent->world();
    Vector2D center(52.5,0);
    Vector2D right_goal(52.5,-5.5);
    Vector2D left_goal(52.5,5.5);
    Vector2D self_pos = wm.self().pos();
    if(self_pos.dist(center) > 18)
        return false;
    std::vector <Vector2D> Shoot;
    if(self_pos.dist(left_goal) < self_pos.dist(right_goal))
    {
        for(double i = -5.8 ; i <= 5.8 ; i++)
        {
            Vector2D check(52.5,i);
            Sector2D check_sector(wm.ball().pos(),3,wm.ball().pos().dist(check),(check - wm.ball().pos()).th() - 15,(check - wm.ball().pos()).th() + 15);
            if(!wm.existOpponentIn(check_sector,5,true))
            {
                Vector2D AccuracyCheck = Vector2D(52.5, check.y + 1);
                Sector2D Accuracy = Sector2D(AccuracyCheck, 3, wm.ball().pos().dist(AccuracyCheck), (AccuracyCheck - wm.ball().pos()).th() - 15, (AccuracyCheck - wm.ball().pos()).th() + 15);
                if(!wm.existOpponentIn(check_sector, 5, true))
                {
                    Shoot.push_back(AccuracyCheck);
                    continue;
                }
                Shoot.push_back(check);
            }
        }
    }
    else
    {
        for(double i = 5.8 ; i >= -5.8 ; i--)
        {
            Vector2D check(52.5,i);
            Sector2D check_sector(wm.ball().pos(),3,wm.ball().pos().dist(check),(check - wm.ball().pos()).th() - 15,(check - wm.ball().pos()).th() + 15);
            if(!wm.existOpponentIn(check_sector,5,true))
            {
                Vector2D AccuracyCheck = Vector2D(52.5, check.y - 1);
                Sector2D Accuracy = Sector2D(AccuracyCheck, 3, wm.ball().pos().dist(AccuracyCheck), (AccuracyCheck - wm.ball().pos()).th() - 15, (AccuracyCheck - wm.ball().pos()).th() + 15);
                if(!wm.existOpponentIn(check_sector, 5, true))
                {
                    Shoot.push_back(AccuracyCheck);
                    continue;
                }
                Shoot.push_back(check);
            }
        }
    }
    if(Shoot.empty())
        return false;
    /*Vector2D FinallShoot = Shoot[0];
    double Shd = wm.ball().pos().dist(FinallShoot);
    for(int i = 1 ; i < Shoot.size() ; i++)
    {
        if(Shoot[i].dist(wm.ball().pos()) < Shd)
        {
            FinallShoot = Shoot[i];
            Shd = wm.ball().pos().dist(FinallShoot);
        }
    }
    Body_SmartKick(FinallShoot, 2.5, 2.5, 2).execute(agent);
    return true;*/
    //Saver().do_SaveSmartKick(agent, Shoot[0], 0, wm.time().cycle());
    if(wm.getDistOpponentNearestToSelf(5) <2.5 ){
        if(Body_KickOneStep(Shoot[0],2.7).execute(agent))
        {
            agent->doPointto(Shoot[0].x,Shoot[0].y);
            return true;
        }
    }
    if( Body_SmartKick( Shoot[0],
       2.7, 2.7,2
                           ).execute( agent ) )
    {
          agent->doPointto(Shoot[0].x,Shoot[0].y);
          return true;
    }

    /*int SectorAng = -15;
    Vector2D FinallShoot = Shoot[0];
    for(int i = 0 ; i < Shoot.size() ; i++)
    {
        for(double j = -15 ; j >= -25 ; j--)
        {
            Sector2D check_sector(self_pos, 0, self_pos.dist(Shoot[i]), (Shoot[i] - self_pos).th() + abs(j), (Shoot[i] - self_pos).th() + j);
            if(!wm.existOpponentIn(check_sector, 5, true))
            {
                if(j < SectorAng)
                {
                    SectorAng = j;
                    FinallShoot = Shoot[i];
                }
            }
        }
    }
    //Saver().do_SaveSmartKick(agent, FinallShoot, 0, wm.time().cycle());
    agent->debugClient().addLine(self_pos, FinallShoot);
    agent->debugClient().addMessage("Multi Pos. SectorSize: %d ", abs(SectorAng));
    Body_SmartKick(FinallShoot, 2.7, 2.7, 2).execute(agent);
    return true;
    return true;*/
    return false;
}

bool Bhv_BasicOffensiveKick::ShootPower(rcsc::PlayerAgent *agent, rcsc::Vector2D Target)
{
    const WorldModel &wm = agent->world();
    for(double i = 2.7 ; i >= 1 ; i = i - 0.3)
    {
        if(Body_SmartKick(Target, i, i, 3).execute(agent))
        {
            return true;
        }
    }
    Body_TurnToBall().execute(agent);
    return true;
}
bool Bhv_BasicOffensiveKick::pass(PlayerAgent * agent){
	const WorldModel & wm = agent->world();
	std::vector<Vector2D> targets;
    std::vector<int>unums;
	Vector2D ball_pos = wm.ball().pos();
	for(int u = 1;u<=11;u++){
		const AbstractPlayerObject * tm = wm.ourPlayer(u);
		if(tm==NULL || tm->unum() < 1 || tm->unum() == wm.self().unum() )
			continue;
		Vector2D tm_pos = tm->pos() + tm->vel();
		if(tm->pos().dist(ball_pos) > 30)
			continue;
		/*Sector2D pass = Sector2D(ball_pos,1,tm_pos.dist(ball_pos)+3,(tm_pos - ball_pos).th() - 20,(tm_pos - ball_pos).th() + 20);*/
		if(is_Passable(agent, ball_pos, tm_pos) && tm->pos().x <=wm.offsideLineX()){
			targets.push_back(tm_pos);
            unums.push_back(tm->unum());
		}
	}
	if(targets.size() == 0)
		return false;
	Vector2D best_target = targets[0];
    int best_unum = unums[0];

    for(unsigned int i=1;i<targets.size();i++){
		if(targets[i].x > best_target.x)
			best_target = targets[i];
        best_unum = unums[i];
	}
	if(wm.ball().pos().x > 35){
        for(unsigned int i=1;i<targets.size();i++){
		if(targets[i].dist(Vector2D(52.5,0)) < best_target.dist(Vector2D(52.5,0)))
			best_target = targets[i];
        best_unum = unums[i];
	}
    }
	double power= pass_power(agent,best_target);
	if(wm.gameMode().type() == GameMode::FreeKick_){
        power = 2;
    }
    if(wm.gameMode().type() == GameMode::KickIn_){
        power = 2.1;
    }
	if(wm.gameMode().type()!= GameMode::PlayOn){
        agent->addSayMessage(new PassMessage(best_unum,
                                             best_target,
                                             agent->effector().queuedNextBallPos(),
                                             wm.ball().vel()));
        Body_SmartKick(best_target,power,power,2).execute(agent);}
	else
        agent->addSayMessage(new PassMessage(best_unum,
                                             best_target,
                                             agent->effector().queuedNextBallPos(),
                                             wm.ball().vel()));
        if(wm.getDistOpponentNearestToSelf(5,false) < 2){
            Body_KickOneStep(best_target,power).execute(agent);
        }
        Body_SmartKick(best_target,power,power,2).execute(agent);
	return true;
}

bool Bhv_BasicOffensiveKick::is_Passable(rcsc::PlayerAgent *agent, Vector2D Sender, Vector2D Receiver)
{
    const WorldModel &wm = agent->world();
    double Power = 2.7;
    double Angle = (Receiver - Sender).th().degree();
    Vector2D Velocity = Vector2D::polar2vector(Power, Angle);
    Vector2D Next = inertia_n_step_point(Sender, Velocity, 1, ServerParam::i().ballDecay());
    int i = 2;
    while(i <= 40 && Next.dist(Receiver) > 1)
    {
        Next = inertia_n_step_point(Sender, Velocity, i, ServerParam::i().ballDecay());
        if(wm.getDistOpponentNearestTo(Next, 5) <= i)
            return false;
        i++;
    }
    if(!wm.existOpponentIn(Circle2D(Sender, 4), 5, true))
        return true;
    return false;
}

bool Bhv_BasicOffensiveKick::dribble(PlayerAgent * agent)
{

   const WorldModel & wm = agent->world();
    std::cout<<"Cycle "<<wm.time().cycle()<<">>>> Dribble is starting";
	std::vector <Vector2D> find_poses;
    Vector2D self_pos(wm.self().pos());
    double my_X = self_pos.x;
    double my_Y = self_pos.y;
    for(double xx = my_X - 5;xx <= my_X + 20;xx = xx + 2)
    {
        for(double yy = my_Y - 20;yy <= my_Y + 20;yy = yy + 2)
        {
            Vector2D check_pos = Vector2D(xx,yy);
            if(check_pos.x > 52.5 || check_pos.x < -52.5 || check_pos.y > 34 || check_pos.y < -34)
                continue;
            if(check_pos.dist(self_pos + wm.self().vel()) < Vector2D(wm.getOpponentNearestTo(check_pos,5,nullptr)->pos() + wm.getOpponentNearestTo(check_pos,5,nullptr)->vel()).dist(check_pos) - 1
             )
            if(check_pos.isValid())
            {
                if(wm.ball().pos().dist(check_pos) > 3)
                    find_poses.push_back(check_pos);
            }
        }
    }
    if(find_poses.empty())
    {
        std::cout<<"Couldn't dribble!";
        return false;
    }
    double power = 0.8;
    Vector2D finall_pos = find_poses[0];
    if(find_poses.size() == 1)
    {
        if(finall_pos.dist(self_pos) > 7)
            power = wm.ball().pos().dist(finall_pos)/12;
        if(finall_pos.dist(self_pos) < 8)
            power = wm.ball().pos().dist(finall_pos)/8;
        if(power < 0.5)
            power = 0.5;
        if(self_pos.x > finall_pos.x)
            power = power + 0.3;
        if(wm.self().stamina() < 4000)
            power = 0.5;
        agent->debugClient().addLine(self_pos,finall_pos);
        if(wm.getDistOpponentNearestToSelf(5) <3.5 &&
        finall_pos.dist(wm.self().pos())
            < 4

    ){
        if(Body_KickOneStep(finall_pos,power).execute(agent))
        {
            agent->doPointto(finall_pos.x,finall_pos.y);
            return true;
        }
    }
       if ( Body_SmartKick( finall_pos,
       power, power,1
                           ).execute( agent ) )
    {
        agent->doPointto(finall_pos.x,finall_pos.y);
        return true;
    }
    else{
        if ( Body_SmartKick( finall_pos,
       power - 0.1, power-0.1,2
                           ).execute( agent ) )
    {
        agent->doPointto(finall_pos.x,finall_pos.y);
        return true;
    }
    }
    }
    Vector2D nearest;
    nearest = Vector2D(52,0);
    for(int i=1;i<find_poses.size();i++)
    {
        if(finall_pos.dist(nearest) > find_poses[i].dist(nearest))
        {
            finall_pos = find_poses[i];
        }
    }
    if(finall_pos.dist(self_pos) > 7)
            power = wm.ball().pos().dist(finall_pos)/12;
        if(finall_pos.dist(self_pos) < 8)
            power = wm.ball().pos().dist(finall_pos)/8;
    if(power < 0.5)
            power = 0.5;
    if(power > 2)
    {
        power = 2;
    }
    if(self_pos.x > finall_pos.x)
        power = power + 0.1;
    if(wm.self().stamina() < 4000)
            power = 0.5;
    agent->debugClient().addLine(self_pos,finall_pos);
    std::cout<<"Dribble finished!"<<std::endl;
    if(wm.getDistOpponentNearestToSelf(5) <3.5 &&
        finall_pos.dist(wm.self().pos())
            < 4

    ){
        if(Body_KickOneStep(finall_pos,power).execute(agent))
        {
            agent->doPointto(finall_pos.x,finall_pos.y);
            return true;
        }
    }
    if ( Body_SmartKick( finall_pos,
       power, power-0.2,1
                           ).execute( agent ) )
    {
        agent->doPointto(finall_pos.x,finall_pos.y);
        return true;
    }
    else{
        if ( Body_SmartKick( finall_pos,
       power, power-0.2,2
                           ).execute( agent ) )
    {
        agent->doPointto(finall_pos.x,finall_pos.y);
        return true;
    }
    }



    return false;
    ///////////////////////////////////end///////////////////////////////////   

}
rcsc::Vector2D Bhv_BasicOffensiveKick::dribble_pos(rcsc::PlayerAgent* agent)
{
    const WorldModel & wm = agent->world();
    std::cout<<"Cycle "<<wm.time().cycle()<<">>>> Dribble is starting";
	std::vector <Vector2D> find_poses;
    Vector2D self_pos(wm.self().pos());

    double my_X = self_pos.x;
    double my_Y = self_pos.y;
    for(double xx = my_X - 20;xx <= my_X + 20;xx ++)
    {
        for(double yy = my_Y - 20;yy <= my_Y + 20;yy ++)
        {
            Vector2D check_pos = Vector2D(xx,yy);
            if(check_pos.x > 52.5 || check_pos.x < -52.5 || check_pos.y > 34 || check_pos.y < -34)
                continue;
            const AbstractPlayerObject *opp_nearest = wm.getOpponentNearestTo(check_pos,5,nullptr);
            double dist_opp_pos = opp_nearest->pos().dist(check_pos) ;
            if(wm.self().playerType().cyclesToReachDistance(wm.self().pos().dist(check_pos)) < opp_nearest->playerTypePtr()->cyclesToReachDistance(dist_opp_pos))
            {
                if(wm.ball().pos().dist(check_pos) > 1.8)
                    find_poses.push_back(check_pos);
            }
        }
    }
    if(find_poses.empty())
    {
        std::cout<<"Couldn't dribble!";
        return Vector2D(100,100);
    }
    double power = 0.8;
    Vector2D finall_pos = find_poses[0];
    if(find_poses.size() == 1)
    {
        if(finall_pos.dist(self_pos) > 7)
            power = wm.ball().pos().dist(finall_pos)/12;
        if(finall_pos.dist(self_pos) < 8)
            power = wm.ball().pos().dist(finall_pos)/8;
        if(power < 0.5)
            power = 0.5;
        if(wm.self().stamina() < 4000)
            power = 0.5;
        agent->debugClient().addLine(self_pos,finall_pos);
        Body_SmartKick(finall_pos,power,power,2).execute(agent);
        return finall_pos;
    }
    Vector2D nearest(52,0);
    /*if(Density(agent,true) == 1)
        nearest = Vector2D(35,-33);
    if(Density(agent,true) == 2 || self_pos.x > 20)
        nearest = Vector2D(52,0);
    else
        nearest = Vector2D(35,33);*/
    for(int i=1;i<find_poses.size();i++)
    {
        if(finall_pos.dist(nearest) > find_poses[i].dist(nearest))
        {
            finall_pos = find_poses[i];
        }
    }
    if(finall_pos.dist(self_pos) > 7)
            power = wm.ball().pos().dist(finall_pos)/12;
        if(finall_pos.dist(self_pos) < 8)
            power = wm.ball().pos().dist(finall_pos)/8;
    if(power < 0.5)
            power = 0.5;
    if(power > 2)
    {
        power = 2;
    }
    if(wm.self().stamina() < 4000)
            power = 0.5;
    agent->debugClient().addLine(self_pos,finall_pos);
    std::cout<<"Dribble finished!"<<std::endl;
    Body_SmartKick(finall_pos,power,power,2).execute(agent);
    return finall_pos;
    return Vector2D(100,100);
}


bool Bhv_BasicOffensiveKick::clearball(PlayerAgent * agent){
    const WorldModel & wm = agent->world();
    if(!wm.self().isKickable())
        return false;
    Vector2D ball_pos = wm.ball().pos();
    Vector2D target = Vector2D(52.5,0);
    if(ball_pos.x < 0){
        if(ball_pos.x > -20){
            if(ball_pos.dist(Vector2D(0,-34)) < ball_pos.dist(Vector2D(0,+34))){
                target = Vector2D(0,-34);
            }else{
                target = Vector2D(0,+34);
            }
        }else{
            if(ball_pos.absY() < 10 && ball_pos.x < -10){
                if(ball_pos.y > 0){
                    target = Vector2D(-52,20);
                }else{
                    target = Vector2D(-52,-20);
                }
            }else{
                if(ball_pos.y > 0){
                    target = Vector2D(ball_pos.x,34);
                }else{
                    target = Vector2D(ball_pos.x,-34);
                }
            }
        }
    }
    if(Body_SmartKick(target,2.7,2.7,2).execute(agent)){
        return true;
    }
    Body_StopBall().execute(agent);
    return true;
}











/*bool Bhv_BasicOffensiveKick::PassAttack(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    Vector2D center_goal(52.5,0);
    const AbstractPlayerObject *tms[3] = { wm.ourPlayer(9), wm.ourPlayer(10), wm.ourPlayer(11) };
    std::vector <Vector2D> AttackPoses;
    std::vector <int> AttackUnum;
    for(int i = 0 ; i < 3 ; i++)
    {
        if(tms[i]->unum() == wm.self().unum())  
            continue;
        for(int x = tms[i]->pos().x + 3 ; x <= 45 ; x = x + 3)
        {
            for(int y = tms[i]->pos().y - 6 ; y <= tms[i]->pos().y + 6 ; y++)
            {
                Vector2D check(x, y);
                if(!check.isValid())
                    continue;
                if(wm.getDistOpponentNearestTo(check, 5) + 1 > tms[i]->pos().dist(check))
                {
                    double Ang = (check - ball_pos).th().degree();
                    Vector2D tmsp = tms[i]->pos();
                    Sector2D ClearPath = Sector2D(tmsp, 0, tmsp.dist(check) + 2, Ang + 15, Ang - 15);
                    if(is_PassAttackAble(agent, ball_pos, tms[i]->pos(), check)&& !wm.existOpponentIn(ClearPath, 0, true))
                    {
                        AttackPoses.push_back(check);
                        AttackUnum.push_back(i + 1);
                    }
                }
            }
        }
    }
    if(AttackPoses.empty())
        return false;
    Vector2D BestTarget = AttackPoses[0];
    int BestUnum = AttackUnum[0];
    for(int i = 1 ; i < AttackPoses.size() ; i++)
    {
        if(AttackPoses[i].dist(center_goal) < BestTarget.dist(center_goal))
        {
            BestTarget = AttackPoses[i];
            BestUnum = AttackUnum[i];
        }
    }
    agent->addSayMessage(new PassMessage(BestUnum,
                                                BestTarget,
                                                agent->effector().queuedNextBallPos(),
                                                wm.ball().vel()));
    agent->debugClient().addMessage("Found Trg");
    agent->debugClient().addLine(ball_pos, BestTarget);
    agent->debugClient().addLine(ball_pos, Vector2D(0, 0 ));
    if(!Body_SmartKick(BestTarget, pass_attack_power(agent, ball_pos, tms[BestUnum]->pos(), BestTarget), pass_attack_power(agent, ball_pos, tms[BestUnum]->pos(), BestTarget), 2).execute(agent))
    {
        Body_TurnToBall().execute(agent);
        return true;
    }
    else
        return true;

    return false;
}
*/
/*bool Bhv_BasicOffensiveKick::is_PassAttackAble(rcsc::PlayerAgent *agent, Vector2D Sender, Vector2D Reciever, Vector2D PassPos)
{
    const WorldModel &wm = agent->world();
    double Power = pass_attack_power(agent, Sender, Reciever, PassPos);
    double Angle = (PassPos - Sender).th().degree();
    Vector2D Velocity = Vector2D::polar2vector(Power, Angle);
    Vector2D Next = inertia_n_step_point(Sender, Velocity, 1, ServerParam::i().ballDecay());
    int i = 2;
    while(i <= 40 && Next.dist(PassPos) > 1)
    {
        Next = inertia_n_step_point(Sender, Velocity, i, ServerParam::i().ballDecay());
        if(wm.getDistOpponentNearestTo(Next, 5) <= i)
            return false;
        i++;
    }
    if(!wm.existOpponentIn(Circle2D(Sender, 4), 5, true))
        return true;
    return false;
}*/

float Bhv_BasicOffensiveKick::pass_power(rcsc::PlayerAgent* agent, rcsc::Vector2D tm_vector)
{

    const WorldModel &wm = agent->world();
    double dist_to_tm = wm.self().pos().dist(tm_vector);
    float power;
    if(dist_to_tm < 3.1)
    {
        power = 0.6;
    }
    power = dist_to_tm/7.5 + 0.2;
    if(power>3)
        power = 3;
    
    
    return power;
        
}



double Bhv_BasicOffensiveKick::dribble_power(PlayerAgent* agent, Vector2D vector_kick)
{
   const WorldModel &wm = agent->world();
   double self_dist = wm.self().pos().dist(vector_kick);
   Vector2D self_pos = wm.self().pos();
   for(float i=0.6;i<=self_dist;i=i+0.5)
   {
       double angle = (vector_kick - self_pos).th().degree();
       Vector2D kick_pos = Vector2D::polar2vector(i,angle) + self_pos;
       double opp_dist = wm.getDistOpponentNearestTo(kick_pos,5);
       if(self_pos.dist(kick_pos) > opp_dist)
       {
           double power;
           power = i / 10;
           power = floor(power *100.)/100;
           if(power > 1)
                power = 1;
           return power;
       }
   }
}


bool Bhv_BasicOffensiveKick::pass_to_forward(rcsc::PlayerAgent *agent)
{
    const WorldModel & wm = agent->world();
	std::vector<Vector2D> targets;
    std::vector<int> unums;
	Vector2D ball_pos = wm.ball().pos();
	for(int u = 1;u<=11;u++){
		const AbstractPlayerObject * tm = wm.ourPlayer(u);
		if(tm==NULL || tm->unum() < 1 || tm->unum() == wm.self().unum() || tm->pos().x < wm.self().pos().x )
			continue;
		Vector2D tm_pos = tm->pos();
		if(tm->pos().dist(ball_pos) > 30)
			continue;
		Sector2D pass = Sector2D(ball_pos,1,tm_pos.dist(ball_pos)+3,(tm_pos - ball_pos).th() - 15,(tm_pos - ball_pos).th() + 15);
		if(!wm.existOpponentIn(pass,5,true) && tm->pos().x <=wm.offsideLineX()){
			targets.push_back(tm_pos);
            unums.push_back(tm->unum());
		}
	}
	if(targets.size() == 0)
		return false;
	Vector2D best_target = targets[0];
    int best_unum = unums[0];
    for(unsigned int i=1;i<targets.size();i++){
		if(targets[i].x > best_target.x)
			best_target = targets[i];
        best_unum = unums[i];
	}
	if(wm.gameMode().type()!= GameMode::PlayOn){
 agent->addSayMessage(new PassMessage(best_unum,
                                             best_target,
                                             agent->effector().queuedNextBallPos(),
                                             wm.ball().vel()));

        Body_SmartKick(best_target,3,2.5,1).execute(agent);}
	else
 agent->addSayMessage(new PassMessage(best_unum,
                                             best_target,
                                             agent->effector().queuedNextBallPos(),
                                             wm.ball().vel()));
        Body_SmartKick(best_target,3,2.5,2).execute(agent);
	return true;
}







bool Bhv_BasicOffensiveKick::clear_sector(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    Vector2D center(52, 0);
    Sector2D check = Sector2D(wm.ball().pos(),0,wm.self().pos().dist(center),(center - wm.self().pos()).th() + 20,(center - wm.self().pos()).th() - 20);
    if(!wm.existOpponentIn(check,5,false))
        return true;
    
    return false;
}


double Bhv_BasicOffensiveKick::pass_attack_power(rcsc::PlayerAgent* agent, rcsc::Vector2D player_pos, rcsc::Vector2D target_pos)
{
    /*return 2.5;
    const WorldModel &wm = agent->world();
    const double TmDist = wm.getDistTeammateNearestTo(target_pos,5);
    double power = wm.ball().pos().dist(target_pos) / TmDist;
    const double dist = wm.ball().pos().dist(target_pos);
    const double adition = 1 + dist / 30;
    power = power * adition;
    if(wm.self().pos().dist(pass_faza(agent)) > 10){
    if(power > 3)
        power = 3;
    if(power < 1.2)
        power = 1.2;}
        else{

            if(power > 3)
        power = 3;
    if(power < 0.8)
        power = 0.8;
        }
    return power;
    double Ang = (target_pos - player_pos).th().degree();
    double TmReachCycle = PassPos.dist(target_pos);
    for(double i = 0.5 ; i <= 2.7 ; i = i + 0.2)
    {
        Vector2D Velocity = Vector2D::polar2vector(i, Ang);
        Vector2D Inertia = inertia_n_step_point(player_pos, Velocity, TmReachCycle, ServerParam::i().ballDecay());
        if(Inertia.dist(PassPos) < 5)
        {
            agent->debugClient().addMessage("Found Power!!!! %d", i);
            return i;
        }
    }
    return 0;*/
    /*const WorldModel &wm = agent->world();
    double dist_tm = player_pos.dist(target_pos);
    Vector2D ball_pos = wm.ball().pos();
    double dist_ball = ball_pos.dist(target_pos);
    float ratio;
    ratio = dist_tm/dist_ball;
    float ball_speed;
    ball_speed = ratio/2;
    //double param[2][11] = {
        //{{3},{2.8},{2.6},{2.4},{2.2},{2},{1.8},{1.6},{1.4},{1.2},{1}},
        //{{10/4},{10/5},{10/6},{10/7},{10/7},{10/8},{10/8},{10/9},{10/12},{10/14},{10/15}}

    double power[11];
    power[0] = 3;
    power[1] = 2.8;
    power[2] = 2.6;
    power[3] = 2.4;
    power[4] = 2.2;
    power[5] = 2;
    power[6] = 1.8;
    power[7] = 1.6;
    power[8] = 1.4;
    power[9] = 1.2;
    power[10] = 1;

    double param[11];
    param[0] = 10/4;
    param[1] = 10/5;
    param[2] = 10/6;
    param[3] = 10/7;
    param[4] = 10/7;
    param[5] = 10/8;
    param[6] = 10/8;
    param[7] = 10/9;
    param[8] = 10/12;
    param[9] = 10/14;
    param[10] = 10/15;

    double nearest_speed;
    nearest_speed = 10000;
    int remember = 0;
    for(int i=0;i<11;i++)
    {
        if(abs(param[i] - ball_speed) < abs(ball_speed - nearest_speed))
        {
            nearest_speed = power[i] + 0.6;
            remember = i;
        }
    }
    return power[remember ];*/
    const WorldModel &wm = agent->world();
    const double TmDist = wm.getDistTeammateNearestTo(target_pos,5);
    double power = wm.ball().pos().dist(target_pos) / TmDist;
    const double dist = wm.ball().pos().dist(target_pos);
    const double adition = 1 + dist / 30;
    power = power * adition;
    if(wm.self().pos().dist(pass_faza(agent)) > 10){
    if(power > 3)
        power = 3;
    if(power < 1.2)
        power = 1.2;}
        else{

            if(power > 3)
        power = 3;
    if(power < 0.8)
        power = 0.8;
        }
    return power;

}



rcsc::Vector2D Bhv_BasicOffensiveKick::Density(rcsc::PlayerAgent *agent,bool less)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    AngleDeg ang = (Vector2D(52,0) - ball_pos).th();
    Sector2D up = Sector2D(ball_pos,0,20,ang - 90 , ang - 30);
    Sector2D middle = Sector2D(ball_pos,0,20,ang - 30 , ang + 30);
    Sector2D down = Sector2D(ball_pos,0,20,ang + 90 , ang + 30);
    if(wm.countOpponentsIn(up,5,false) < wm.countOpponentsIn(middle,5,false) && wm.countOpponentsIn(up,5,false) <= wm.countOpponentsIn(down,5,false))
        return Vector2D::polar2vector(20,-60) + ball_pos;
    if(wm.countOpponentsIn(middle,5,false) <= wm.countOpponentsIn(up,5,false) && wm.countOpponentsIn(middle,5,false) < wm.countOpponentsIn(down,5,false))
        return Vector2D::polar2vector(20,0) + ball_pos;
    
    return Vector2D::polar2vector(20,60) + ball_pos;

    
}


bool Bhv_BasicOffensiveKick::pass_fazayy( rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    Vector2D center(52.5,0);
    AngleDeg polar = (center-wm.self().pos()).th().degree();
    Sector2D check = Sector2D(wm.self().pos(),0,6,polar + 20,polar - 20);
    if(!wm.existOpponentIn(check,5,false)){
        return false;}

    const AbstractPlayerObject *mojahed1 = wm.getTeammateNearestTo(Vector2D(52.5,0),5,nullptr);
    Vector2D ball = Vector2D(wm.ball().pos());
    int x=0;
    for(int i=49 ; i>=mojahed1->pos().x+5 ; --i)
    {
        AngleDeg jahatniat = (ball-Vector2D(i,mojahed1->pos().y)).th().degree();;;;;
        Sector2D avalniat = Sector2D(wm.ball().pos(),0,3,jahatniat-4,jahatniat+4) ;;;;
        if(!wm.existOpponentIn(avalniat,2,true))
        {
        AngleDeg gharemoonyadetnare = (ball-Vector2D(i,mojahed1->pos().y)).th().degree();;;;;;
        Vector2D polarkhare = Vector2D::polar2vector(i,gharemoonyadetnare)+Vector2D(ball.x,ball.y);;;;;;;;;;;
        const AbstractPlayerObject *oppkhare = wm.getOpponentNearestTo(polarkhare,5,nullptr);;;;
        if(oppkhare->pos().dist(polarkhare)*3 < mojahed1->pos().dist(polarkhare)/0.852)
        {
            continue;
        }
        }
        x=i;
    }
    Vector2D ball_pos = wm.ball().pos();
    Vector2D center_goal(52.5,0);
    const PlayerPtrCont opp = wm.opponentsFromSelf();
    const PlayerPtrCont tm = wm.teammatesFromSelf();
    PlayerPtrCont tms_n_h;
    int counter=0;
    for(int i=0;i<10;i++)
        {
            tms_n_h.push_back(tm[i]);
        }
    for(int i=0; i<=7;i++)
        {
            for(int j=i+1;j<=8;j++)
            {
                if(tms_n_h[i]->pos().dist(center_goal)>tms_n_h[j]->pos().dist(center_goal) && tm[j]->unum() > 6)
                    {

                        std::swap(tms_n_h[i],tms_n_h[j]);
                        counter++;
                    }
            }
        }

    bool logic = true;
    for(int i=0 ; i<tms_n_h.size() ; i++)
    {
            for(int j=49 ; j>= tms_n_h[i]->pos().x ; j= j-3)
            {
                /*if(wm.audioMemory().stamina().front().sender_ < 5000)
                {
                    continue;
                }*/
                AngleDeg ang_polar_th = (Vector2D(j,tms_n_h[i]->pos().y) - ball_pos).th();
                double dist_n_opp = wm.getDistOpponentNearestTo(Vector2D(j,tms_n_h[i]->pos().y),5);
                if((dist_n_opp < tms_n_h[i]->pos().dist(Vector2D(j,tms_n_h[i]->pos().y))) || j < wm.self().pos().x+10)
                {
                    if(wm.getOpponentGoalie()->pos().dist(Vector2D(j,tms_n_h[i]->pos().y)) < Vector2D(j,tms_n_h[i]->pos().y).dist(tms_n_h[i]->pos()
                    )){
                    continue;}
                    continue;
                }
                if(tms_n_h[i]->playerTypePtr()->cyclesToReachDistance(tms_n_h[i]->pos().dist(Vector2D(j,tms_n_h[i]->pos().y))) >=
                    wm.getOpponentNearestTo(Vector2D(j,tms_n_h[i]->pos().y),5,nullptr)->playerTypePtr()->cyclesToReachDistance(wm.getOpponentNearestTo(Vector2D(j,tms_n_h[i]->pos().y),5,nullptr)->pos().dist(Vector2D(j,tms_n_h[i]->pos().y)))){
                    continue;
                }
                         int counter=0;
                double dist_s_to_gopos = ball_pos.dist(Vector2D(j,tms_n_h[i]->pos().y));
                for(int p=0 ; p<=dist_s_to_gopos ; p= p+2)
                {
                    Vector2D next_pos = Vector2D::polar2vector(p,ang_polar_th)+wm.ball().pos();
                    double dist_opp = wm.getDistOpponentNearestTo(next_pos,5);
                    if(dist_opp < p/2)
                    {
                        logic = false;
                        break;
                    }
                    else
                    {
                        counter=counter+2;
                    }
                }
                Vector2D final_pos = Vector2D(j,tms_n_h[i]->pos().y);
                if(logic == true)
                {

                     float power = (pass_attack_power(agent,wm.self().pos(),final_pos))-0.5;
                     Vector2D self_pos = wm.self().pos();
                     agent->addSayMessage(new PassMessage(tms_n_h[i]->unum(),
                                                          final_pos,
                                                          agent->effector().queuedNextBallPos(),
                                                          wm.ball().vel()));
                     agent->debugClient().addLine(self_pos , final_pos);
                   if ( Body_SmartKick( final_pos,
       power, power,1
                           ).execute( agent ) )
    {

        return true;
    }
    else{
        if ( Body_SmartKick( final_pos,
       power, power,2
                           ).execute( agent ) )
    {

        return true;
    }
    }
                }
            }
    }
    return false;

}








rcsc::Vector2D Bhv_BasicOffensiveKick::pass_faza(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    Vector2D center_goal(52.5,0);
    const PlayerPtrCont opp = wm.opponentsFromSelf();
    const PlayerPtrCont tm = wm.teammatesFromSelf();
    PlayerPtrCont tms_n_h;
    int counter=0;
    for(int i=0;i<10;i++)
        {
            tms_n_h.push_back(tm[i]);
        }
    for(int i=0; i<=7;i++)
        {
            for(int j=i+1;j<=8;j++)
            {
                if(tms_n_h[i]->pos().dist(center_goal)>tms_n_h[j]->pos().dist(center_goal) && tm[j]->unum() > 6)
                    {

                        std::swap(tms_n_h[i],tms_n_h[j]);
                        counter++;
                    }
            }
        }

    bool logic = true;
    for(int i=0 ; i<tms_n_h.size() ; i++)
    {
            for(int j=52 ; j>= tms_n_h[i]->pos().x ; j= j-3)
            {
                AngleDeg ang_polar_th = (Vector2D(j,tms_n_h[i]->pos().y) - ball_pos).th();
                double dist_n_opp = wm.getDistOpponentNearestTo(Vector2D(j,tms_n_h[i]->pos().y),5);
                if((dist_n_opp < tms_n_h[i]->pos().dist(Vector2D(j,tms_n_h[i]->pos().y))) || j < wm.self().pos().x+10)
                {
                    continue;
                }
                         int counter=0;
                double dist_s_to_gopos = ball_pos.dist(Vector2D(j,tms_n_h[i]->pos().y));
                for(int p=0 ; p<=dist_s_to_gopos ; p= p+2)
                {
                    Vector2D next_pos = Vector2D::polar2vector(p,ang_polar_th)+wm.ball().pos();
                    double dist_opp = wm.getDistOpponentNearestTo(next_pos,5);
                    if(dist_opp < p/2)
                    {
                        logic = false;
                        break;
                    }
                    else
                    {
                        counter=counter+2;
                    }
                }
                Vector2D final_pos = Vector2D(j,tms_n_h[i]->pos().y);
                if(logic == true)
                {
                     float power = (pass_power(agent,final_pos)*25)/40;
                     return final_pos;
                }
            }
    }
    return Vector2D(100,100);
}
int Bhv_BasicOffensiveKick::faza_unum(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    Vector2D center_goal(52.5,0);
    const PlayerPtrCont opp = wm.opponentsFromSelf();
    const PlayerPtrCont tm = wm.teammatesFromSelf();
    PlayerPtrCont tms_n_h;
    int counter=0;
    for(int i=0;i<10;i++)
        {
            tms_n_h.push_back(tm[i]);
        }
    for(int i=0; i<=7;i++)
        {
            for(int j=i+1;j<=8;j++)
            {
                if(tms_n_h[i]->pos().dist(center_goal)>tms_n_h[j]->pos().dist(center_goal) && tm[j]->unum() > 6)
                    {

                        std::swap(tms_n_h[i],tms_n_h[j]);
                        counter++;
                    }
            }
        }

    bool logic = true;
    for(int i=0 ; i<tms_n_h.size() ; i++)
    {
            for(int j=52 ; j>= tms_n_h[i]->pos().x ; j= j-3)
            {
                AngleDeg ang_polar_th = (Vector2D(j,tms_n_h[i]->pos().y) - ball_pos).th();
                double dist_n_opp = wm.getDistOpponentNearestTo(Vector2D(j,tms_n_h[i]->pos().y),5);
                if((dist_n_opp < tms_n_h[i]->pos().dist(Vector2D(j,tms_n_h[i]->pos().y))) || j < wm.self().pos().x+10)
                {
                    continue;
                }
                         int counter=0;
                double dist_s_to_gopos = ball_pos.dist(Vector2D(j,tms_n_h[i]->pos().y));
                for(int p=0 ; p<=dist_s_to_gopos ; p= p+2)
                {
                    Vector2D next_pos = Vector2D::polar2vector(p,ang_polar_th)+wm.ball().pos();
                    double dist_opp = wm.getDistOpponentNearestTo(next_pos,5);
                    if(dist_opp < p/2)
                    {
                        logic = false;
                        break;
                    }
                    else
                    {
                        counter=counter+2;
                    }
                }
                Vector2D final_pos = Vector2D(j,tms_n_h[i]->pos().y);
                if(logic == true)
                {
                     float power = (pass_power(agent,final_pos)*25)/40;
                     return tms_n_h[i]->unum();
                }
            }
    }
    return 12;
}

bool Bhv_BasicOffensiveKick::GoalPass(rcsc::PlayerAgent *agent)
{
     const WorldModel &wm = agent->world();
    std::vector<Vector2D> targets;
    std::vector<int>unums;
    double OffsideX = wm.offsideLineX();
	Vector2D ball_pos = wm.ball().pos();
	for(int u = 6;u<=11;u++){
		const AbstractPlayerObject * tm = wm.ourPlayer(u);
		if(tm->unum() == wm.self().unum() || tm->pos().dist(Vector2D(52,0)) > 18 || tm->pos().x > OffsideX)
			continue;
		Vector2D tm_pos = tm->pos();
		if(tm->pos().dist(ball_pos) > 15)
			continue;
		Sector2D pass = Sector2D(ball_pos,1,tm_pos.dist(ball_pos)+3,(tm_pos - ball_pos).th() - 5,(tm_pos - ball_pos).th() + 5);
		if(!wm.existOpponentIn(pass,5,true)){
			targets.push_back(tm_pos);
            unums.push_back(tm->unum());
		}
	}
	if(targets.size() == 0)
		return false;
	Vector2D best_target = targets[0];
    int best_unum = unums[0];
    for(unsigned int i=1;i<targets.size();i++){
		if(targets[i].dist(Vector2D(52,0)) > best_target.dist(Vector2D(52,0)))
			best_target = targets[i];
        best_unum = unums[i];
	}
    double power = 2.7;
	if(wm.gameMode().type()!= GameMode::PlayOn){
        agent->addSayMessage(new PassMessage(best_unum,
                                             best_target,
                                             agent->effector().queuedNextBallPos(),
                                             wm.ball().vel()));
        Body_KickOneStep(best_target, 2.7).execute(agent);}
	else
    {
        agent->addSayMessage(new PassMessage(best_unum,
                                             best_target,
                                             agent->effector().queuedNextBallPos(),
                                             wm.ball().vel()));

        if(wm.getDistOpponentNearestToBall(5, true) < 2)
        {
            Body_KickOneStep(best_target, 2.7).execute(agent);
            return true;
        }
        Body_SmartKick(best_target,power,power,2).execute(agent);
	    return true;
    }
    return false;
}
rcsc::Vector2D Bhv_BasicOffensiveKick::Density(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    Rect2D Down = Rect2D(Vector2D(13.01,11.89),Size2D(10,10));
    Rect2D Mid = Rect2D(Vector2D(10.01,-7.89),Size2D(10,10));
    Rect2D Up = Rect2D(Vector2D(8.01,-31.89),Size2D(10,10));
    if(wm.countOpponentsIn(Down,5,true) < wm.countOpponentsIn(Up,5,true) && wm.countOpponentsIn(Down,5,true) < wm.countOpponentsIn(Mid,5,true)){
        return Vector2D(23.01,21.89);
    }
    else if(wm.countOpponentsIn(Up,5,true) < wm.countOpponentsIn(Down,5,true) && wm.countOpponentsIn(Up,5,true) < wm.countOpponentsIn(Mid,5,true)){
        return Vector2D(23,-27);
    }
    else{
        return Vector2D(23,0.01);
    }
    return Vector2D(100,100);
}
int Bhv_BasicOffensiveKick::AnalyzeGame(rcsc::PlayerAgent * agent )
{
    const WorldModel &wm = agent->world();
    Rect2D up(Vector2D(0,-34), Size2D(52.5, 34));
    Rect2D down(Vector2D(0,0), Size2D(52.5, 34));
    static int CountUp = 0;
    static int CountDown = 0;
    static int GoalsU = 0;
    static int GoalsO = 0;
    if(wm.ourSide() == LEFT  && wm.gameMode().scoreLeft()){
        GoalsU++;
    }
    if(wm.ourSide() == RIGHT  && wm.gameMode().scoreRight()){
        GoalsU++;
    }
    if(wm.theirSide() == LEFT  && wm.gameMode().scoreLeft()){
        GoalsO++;
    }
    if(wm.theirSide() == RIGHT  && wm.gameMode().scoreRight()){
        GoalsO++;
    }
    return GoalsU-GoalsO;
    /*if(wm.time().cycle() < 2950){
        if(wm.ball().pos().y > 0 )
        {
                CountDown = CountDown+ wm.countTeammatesIn(down,5,true);
        }
        if(wm.ball().pos().y < 0 )
        {
                CountUp = CountUp+ wm.countTeammatesIn(up,5,true);
        }
    }
        if(GoalsU > GoalsO+3){
            return 0;
        }
        else if(GoalsU <= GoalsO){
               if(CountUp > CountDown  && wm.time().cycle() > 2950){
                  return 1 ;
                }
                else if(CountUp <= CountDown && wm.time().cycle() > 2950){
                   return 2 ;
                }
        }
*/
}

int Bhv_BasicOffensiveKick::MatchResults(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    if(AnalyzeGame(agent) > 5){
        return 1;
    }
    else if(AnalyzeGame(agent) <5 &&AnalyzeGame(agent) >-2){
        return 2;

    }
    else if (AnalyzeGame(agent) <= -2 ){
        return 3 ;
    }


}
bool Bhv_BasicOffensiveKick::MakeDecision(rcsc::PlayerAgent* agent)
{
    dlog.addText( Logger::TEAM,
                  __FILE__": Bhv_BasicOffensiveKick_MakeDecision" );
    Body_TurnToBall().execute(agent);
    Body_HoldBall().execute(agent);
    const WorldModel & wm = agent->world();
    if(MatchResults(agent) == 1)
    {
        if(wm.getOpponentNearestToBall(5,true)->isTackling() && wm.getOpponentNearestToBall(5,true)->pos().dist(wm.ball().pos()) <= ServerParam::i().tackleWidth()){
        Body_StopBall().execute(agent);
    }
    const PlayerPtrCont &opps = wm.opponentsFromSelf();
    const PlayerObject * nearest_opp= ( opps.empty() ? static_cast< PlayerObject * >( 0 ): opps.front() );
    const double nearest_opp_dist = ( nearest_opp? nearest_opp->distFromSelf() : 1000.0 );
    const Vector2D nearest_opp_pos = ( nearest_opp  ? nearest_opp->pos(): Vector2D( -1000.0, 0.0 ) );
    int Dng = 7;
    if(wm.time().cycle() > 5000 && AnalyzeGame(agent) == 0)
        Dng = 3;
    if(nearest_opp_dist < Dng)
        if(pass(agent))
            return true;
    if(shoot(agent)){
    	return true;
    }
    if(clear_sector(agent) && wm.ball().pos().dist(Vector2D(52.5,0)) > 15)
    {
        Body_SmartKick(Vector2D(52.5,0),0.9,0.9,2).execute(agent);
        return true;
    }
    Vector2D center(52.5,0);
    AngleDeg polar = (center-wm.self().pos()).th().degree();
    Sector2D check = Sector2D(wm.self().pos(),0,6,polar + 20,polar - 20);
    if(wm.getDistOpponentNearestToBall(5, true) < 8)
        if(pass(agent))
            return true;
    if(!wm.existOpponentIn(check,5,false))
    {
        if(dribble(agent) == true)
        {
            return true ;
        }
    }
    if( pass_fazayy(agent))
            return true;

    if(nearest_opp_dist < 5){
    	if(pass(agent))
    		return true;
    }
    if(pass_to_forward(agent))
        return true;
    if(GoalPass(agent))
        return true;
    if(dribble(agent))
        return true;
    clearball(agent);
    return true;
    }
    else if(MatchResults(agent) == 2)
    {

        const PlayerPtrCont &opps = wm.opponentsFromSelf();
        const PlayerObject * nearest_opp= ( opps.empty() ? static_cast< PlayerObject * >( 0 ): opps.front() );
        const double nearest_opp_dist = ( nearest_opp? nearest_opp->distFromSelf() : 1000.0 );
        const Vector2D nearest_opp_pos = ( nearest_opp  ? nearest_opp->pos(): Vector2D( -1000.0, 0.0 ) );
        if(shoot(agent)){
         	return true;
        }
        if(clear_sector(agent) && wm.ball().pos().dist(Vector2D(52.5,0)) > 15)
        {
            Body_SmartKick(Vector2D(52.5,0),0.9,0.9,2).execute(agent);
            return true;
        }
         Vector2D center(52.5,0);
        AngleDeg polar = (center-wm.self().pos()).th().degree();
        Sector2D check = Sector2D(wm.self().pos(),0,6,polar + 20,polar - 20);
         if(!wm.existOpponentIn(check,5,false))
        {
            if(dribble(agent))
            {
                return true ;
            }
        }
        if(!clear_sector(agent)){
        if(GoalPass(agent))
            return true;
        }
        if(pass_fazayy(agent))
            return true;
        if(dribble(agent))
            return true;
        if(wm.getDistOpponentNearestToBall(5, true) < 6)
            if(pass(agent))
                return true;


        /*if( pass_fazayy(agent))
                return true;*/
        if(pass_to_forward(agent))
            return true;


        if(nearest_opp_dist < 3){
    	    if(pass(agent))
    	    	return true;
        }
        if(GoalPass(agent))
            return true;

        clearball(agent);
        return true;
    }
    else if(MatchResults(agent) == 3){
        if(wm.getOpponentNearestToBall(5,true)->isTackling() && wm.getOpponentNearestToBall(5,true)->pos().dist(wm.ball().pos()) <= ServerParam::i().tackleWidth()){
        Body_StopBall().execute(agent);
    }
    const PlayerPtrCont &opps = wm.opponentsFromSelf();
    const PlayerObject * nearest_opp= ( opps.empty() ? static_cast< PlayerObject * >( 0 ): opps.front() );
    const double nearest_opp_dist = ( nearest_opp? nearest_opp->distFromSelf() : 1000.0 );
    const Vector2D nearest_opp_pos = ( nearest_opp  ? nearest_opp->pos(): Vector2D( -1000.0, 0.0 ) );
    if(shoot(agent)){
    	return true;
    }
    if(clear_sector(agent) && wm.ball().pos().dist(Vector2D(52.5,0)) > 15)
    {
        Body_SmartKick(Vector2D(52.5,0),0.9,0.9,2).execute(agent);
        return true;
    }
    Vector2D center(52.5,0);
    AngleDeg polar = (center-wm.self().pos()).th().degree();
    Sector2D check = Sector2D(wm.self().pos(),0,6,polar + 20,polar - 20);
    if(!wm.existOpponentIn(check,5,false))
    {
        if(dribble(agent) == true)
        {
            return true ;
        }
    }
    /*if( pass_fazayy(agent))
            return true;*/
    if(nearest_opp_dist < 5){
    	if(pass(agent))
    		return true;
    }
    if(pass_to_forward(agent))
        return true;
    /*if(GoalPass(agent))
        return true;*/
    if(dribble(agent))
        return true;
    clearball(agent);
    return true;
    }
}



class Reinforcement_Learning { // No i n h e r i t a n c e r e q u i r e d .

 private :

    int current ; double r ;
    //Α α, Β β, Γ γ, Δ δ, Ε ε, Ζ ζ, Η η, Θ θ, Ι ι, Κ κ, Λ λ, Μ μ, Ν ν, Ξ ξ, Ο ο, Π π, Ρ ρ, Σ σ/ς, Τ τ, Υ υ, Φ φ, Χ χ, Ψ ψ, Ω ω.

    /*
    Αα	Alpha	Νν	Nu
    Ββ	Beta	Ξξ	Xi
    Γγ	Gamma	Οο	Omicron
    Δδ	Delta	Ππ	Pi
    Εε	Epsilon	Ρρ	Rho
    Ζζ	Zeta	Σσς	Sigma
    Ηη	Eta	Ττ	Tau
    Θθ	Theta	Υυ	Upsilon
    Ιι	Iota	Φφ	Phi
    Κκ	Kappa	Χχ	Chi
    Λλ	Lambda	Ψψ	Psi
    Μμ	Mu	Ωω	Omega*/

    //full-form Equation = Qt(s,a)=Qt-1(s,a)+α(R(s,a)+γmaxa′ Q(s′,a′)-Qt-1(s,a))
    //V(s)=maxa(R(s,a)+γ∑s′P(s,a,s′)V(s′))
    //Bellman Equation = ν(s) = max'a'(R(s,a)'+γν(s'))
    //Deep Q-Learning = Q(s,a) = r(s,a) + γmax's'Q(s',a)
    //Loss = R(s,a) + γΣ(s')(P(s,a,s')max(s')Q(s',a')) - Q(s,a)
    //Description ---> γ(Discount_Factor) = 0.75 , α = 0.9

 public :
    typedef std::int32_t Orders ; // BallMovement = 0.0 Dribble = 1.0 PassState = 2.0 HardPressChangePitchSide = 3.0
    typedef float EvaluateValue ; // 0.0 - 0.990
    typedef _Float32 CalculateEntropy ; // collect action posibility then "try" 5 new states for best result
    typedef int phase_type ;
    typedef double reward_type ;
    typedef int observation_type ;
    typedef enum { up , down} action_type ;
    typedef Reinforcement_Learning::phase_type S;
    typedef Reinforcement_Learning::action_type A;
    //typedef rl::Iterator<A, Sim::up,Sim::down>Aenum; // enumerate all actions .
    //rl::SA<S,A>    SA ;
    //rl::sa::Transition<S,A,double>Transition ; // i . e . , ( s , a , r , s ’ , a ’)


    Reinforcement_Learning ( void ) : current ( 0 ) , r ( 0 ) {}
    void setPhase ( const phase_type &s ) { current = s %10;}
    const observation_type& sense ( void ) const {
        return current ;

    }
    reward_type   reward ( void ) const { return r ;}
    void timeStep ( const action_type &a ) {
        if ( a == up )
            current++ ;
        else
            current-- ;
        if ( current < 0 )
            r =-1;
        else if ( current > 9 )
            r = 1 ;
        else
            r = 0 ;
        if ( r != 0 )

              // throw rl::exception::Terminal( ” Out of range ” )
              throw NULL;
            ;


    }
    std::vector< int16_t > AvailableStatesSActions ( PlayerAgent * agent , Vector2D NextStateS)//s' , a'
    {


    };
    // Entropy Values BallMovement = 0.0 Dribble = 1.0 PassState = 2.0 HardPressChangePitchSide = 3.0
    //if Empty -> PushBack NULL in vector
    CalculateEntropy ActionCalc ( int32_t ActionADRESS , std::vector< int16_t > AvailableStatesSActions)
    {


    };
    Orders ΣActionOrders ( int32_t BallMovement , int32_t Dribble , int32_t DeepPassState , int16_t HardPressChangePitchSide )
    {


    };
    float Reward ( PlayerAgent * agent , Vector2D NextStateS , int32_t Action )
    {
        const WorldModel &wm = agent->world();

        const ServerParam &SP = ServerParam::i();
        Vector2D ball_pos = ( wm.self().isKickable() ? wm.ball().inertiaPoint(1)  : wm.ball().inertiaPoint(1) + wm.ball().vel() );


        Vector2D left_goal = Vector2D(SP.pitchHalfLength(),-SP.goalHalfWidth());
        Vector2D right_goal = Vector2D(SP.pitchHalfLength(),+SP.goalHalfWidth());

        float reward ;
        if( Action == 3 ){

            //needs tahas pass system
        }
        else if ( Action == 2 ){

            double distance ;// a value from 0.0 to 110.0(chord of field(max dist))  NEXT STATE DIST (0.1S)
            if(ball_pos.dist( left_goal ) < ball_pos.dist( right_goal )){

                distance = NextStateS.dist(left_goal) ;
            }
            else{

                distance = NextStateS.dist(right_goal) ;
            }
            //just test deep pass
            bool STATE = false ;
            Vector2D center(52.5,0);
            AngleDeg polar = (center-wm.self().pos()).th().degree();
            Sector2D check = Sector2D(wm.self().pos(),0,6,polar + 20,polar - 20);
            if(!wm.existOpponentIn(check,5,false)){
                return false;}

            const AbstractPlayerObject *mojahed1 = wm.getTeammateNearestTo(Vector2D(52.5,0),5,nullptr);
            Vector2D ball = Vector2D(wm.ball().inertiaPoint(1));
            int x=0;
            for(int i=49 ; i>=mojahed1->pos().x+5 ; --i)
            {
                AngleDeg jahatniat = (ball-Vector2D(i,mojahed1->pos().y)).th().degree();;;;;
                Sector2D avalniat = Sector2D(wm.ball().pos(),0,3,jahatniat-4,jahatniat+4) ;;;;
                if(!wm.existOpponentIn(avalniat,2,true))
                {
                AngleDeg gharemoonyadetnare = (ball-Vector2D(i,mojahed1->pos().y)).th().degree();;;;;;
                Vector2D polarkhare = Vector2D::polar2vector(i,gharemoonyadetnare)+Vector2D(ball.x,ball.y);;;;;;;;;;;
                const AbstractPlayerObject *oppkhare = wm.getOpponentNearestTo(polarkhare,5,nullptr);;;;
                if(oppkhare->pos().dist(polarkhare)*3 < mojahed1->pos().dist(polarkhare)/0.852)
                {
                    continue;
                }
                }
                x=i;
            }
            Vector2D ball_pos = wm.ball().inertiaPoint(1);
            Vector2D center_goal(52.5,0);
            const PlayerPtrCont opp = wm.opponentsFromSelf();
            const PlayerPtrCont tm = wm.teammatesFromSelf();
            PlayerPtrCont tms_n_h;
            int counter=0;
            for(int i=0;i<10;i++)
                {
                    tms_n_h.push_back(tm[i]);
                }
            for(int i=0; i<=7;i++)
                {
                    for(int j=i+1;j<=8;j++)
                    {
                        if(tms_n_h[i]->inertiaPoint(1).dist(center_goal)>tms_n_h[j]->inertiaPoint(1).dist(center_goal) && tm[j]->unum() > 6)
                            {

                                std::swap(tms_n_h[i],tms_n_h[j]);
                                counter++;
                            }
                    }
                }

            bool logic = true;
            for(int i=0 ; i<tms_n_h.size() ; i++)
            {
            for(int j=49 ; j>= tms_n_h[i]->inertiaPoint(1).x ; j= j-3)
            {
                /*if(wm.audioMemory().stamina().front().sender_ < 5000)
                {
                    continue;
                }*/
                AngleDeg ang_polar_th = (Vector2D(j,tms_n_h[i]->inertiaPoint(1).y) - ball_pos).th();
                double dist_n_opp = wm.getDistOpponentNearestTo(Vector2D(j,tms_n_h[i]->inertiaPoint(1).y),5);
                if((dist_n_opp < tms_n_h[i]->inertiaPoint(1).dist(Vector2D(j,tms_n_h[i]->inertiaPoint(1).y))) ||
                    j < wm.self().inertiaPoint(1).x+10)
                {
                    if(wm.getOpponentGoalie()->inertiaPoint(1).dist(Vector2D(j,tms_n_h[i]->inertiaPoint(1).y)) <
                        Vector2D(j,tms_n_h[i]->inertiaPoint(1).y).dist(tms_n_h[i]->inertiaPoint(1)
                    )){
                    continue;}
                    continue;
                }
                if(tms_n_h[i]->playerTypePtr()->cyclesToReachDistance(tms_n_h[i]->inertiaPoint(1).dist(Vector2D(j,tms_n_h[i]->inertiaPoint(1).y))) >=
                    wm.getOpponentNearestTo(Vector2D(j,tms_n_h[i]->inertiaPoint(1).y),5,nullptr)->playerTypePtr()->cyclesToReachDistance(
                        wm.getOpponentNearestTo(Vector2D(j,tms_n_h[i]->inertiaPoint(1).y),5,nullptr)->inertiaPoint(1).dist(Vector2D(
                            j,tms_n_h[i]->inertiaPoint(1).y)))){
                    continue;
                }
                         int counter=0;
                double dist_s_to_gopos = ball_pos.dist(Vector2D(j,tms_n_h[i]->inertiaPoint(1).y));
                for(int p=0 ; p<=dist_s_to_gopos ; p= p+2)
                {
                    Vector2D next_pos = Vector2D::polar2vector(p,ang_polar_th)+wm.ball().inertiaPoint(1);
                    double dist_opp = wm.getDistOpponentNearestTo(next_pos,5);
                    if(dist_opp < p/2)
                    {
                        logic = false;
                        break;
                    }
                    else
                    {
                        counter=counter+2;
                    }
                }
                Vector2D final_pos = Vector2D(j,tms_n_h[i]->inertiaPoint(1).y);
                if(logic == true)
                {
                    STATE = true ;

                }
                }
            }

            // deep pass test complete --- now statedist & action
            auto Discount_Factor = 0.440 ;
            auto ValidAction = 0.0 ;
            if(STATE){
                ValidAction = 0.40 ;
            }
            double Distvalue = distance/1000 + Discount_Factor ;
            reward = Distvalue + ValidAction ;
            return reward ;
            }
        if(Action == 1){

            std::vector <Vector2D> find_poses;
            Vector2D self_pos(wm.self().pos());
            double my_X = self_pos.x;
            double my_Y = self_pos.y;
            bool STATE = true ;
                for(double xx = my_X - 5;xx <= my_X + 20;xx = xx + 2)
                {
                for(double yy = my_Y - 20;yy <= my_Y + 20;yy = yy + 2)
                {
                Vector2D check_pos = Vector2D(xx,yy);
                if(check_pos.x > 52.5 || check_pos.x < -52.5 || check_pos.y > 34 || check_pos.y < -34)
                continue;
                if(NextStateS.dist(check_pos) < wm.getOpponentNearestTo(check_pos,5,nullptr)->inertiaPoint(1).dist(check_pos) - 1
                )
                if(check_pos.isValid())
                {
                if(wm.ball().pos().dist(check_pos) > 3)
                    find_poses.push_back(check_pos);
                }
                }
            }
            if(find_poses.empty())
            {
                STATE = false;
            }
            Vector2D finall_pos = find_poses[0];
            auto distance = finall_pos.dist(Vector2D(52.5,0));
            auto Discount_Factor = 0.440 ;
            auto ValidAction = 0.0 ;
            if(STATE){
                ValidAction = 0.40 ;
            }
            double Distvalue = distance/1000 + Discount_Factor ;
            reward = Distvalue + ValidAction ;
            return reward ;
        }
        if( Action == 0){

            std::vector <Vector2D> find_poses;
            Vector2D self_pos(wm.self().pos());
            double my_X = self_pos.x;
            double my_Y = self_pos.y;
            bool STATE = true ;
                for(double xx = my_X - 5;xx <= my_X + 20;xx = xx + 2)
                {
                for(double yy = my_Y - 20;yy <= my_Y + 20;yy = yy + 2)
                {
                Vector2D check_pos = Vector2D(xx,yy);
                if(check_pos.x > 52.5 || check_pos.x < -52.5 || check_pos.y > 34 || check_pos.y < -34)
                continue;
                if(NextStateS.dist(check_pos) < wm.getOpponentNearestTo(check_pos,5,nullptr)->inertiaPoint(1).dist(check_pos) - 1
                )
                if(check_pos.isValid())
                {
                if(wm.ball().pos().dist(check_pos) > 3)
                    find_poses.push_back(check_pos);
                }
                }
            }
            if(find_poses.empty())
            {
                STATE = false;
            }
            if(STATE){
                return 0.99 ;
            }
        }

    };
    EvaluateValue GetEntropyGiveStateRewards ( _Float32 Entropy ,  Vector2D NextStateS , PlayerAgent * agent  , int32_t Action )
    {
        //Bellman Equation = ν(s) = max'a'(R(s,a)'+γν(s'))

        _Float32 EntropyResult = Entropy ;

        Vector2D CheckPos = NextStateS ;

        auto γ = 0.99 ;

        _Float32 _Reward  = Reward( agent , CheckPos , Action );//R(s,a)'

        float maxα = 1 - EntropyResult; //-> * purity *  ! impurity  max'a'

        auto BellManValue = ( γ *_Reward ) * maxα ;

        return BellManValue ;

    };
};






///////////////////////////////          Class Saver        //////////////////////////////












bool Saver::do_SaveSmartKick(PlayerAgent *agent, Vector2D Target, double power, int Cycle)
{
    std::string Data;
    std::ifstream Reader("SmartKickSaver.txt");
    int Y = 100;
    int cycle = 0;
    //12
    //1
    //10 + 2
    if(Reader.is_open())
    {
        while(getline(Reader, Data))
        {
            if(Data[0] == 'Y')
            {
                Y = Data[1] - 48;
            }
            if(Data[0] == 'C')
            {
                for(int i = 1 ; i <= Data.size() ; i++)
                {
                    cycle = cycle * 10 + (Data[i] - 48);
                }
                
            }
        }
    }
    agent->debugClient().addMessage("cycle %d", cycle);
    if(Y != 100 && agent->world().self().isKickable())
    {
        agent->debugClient().addMessage("Saver");
        Body_SmartKick(Vector2D(52.5, Y), 2.5, 2.5, 2).execute(agent);
        return true;
    }
    std::ofstream SmartKick("SmartKickSaver.txt");
    SmartKick<<"Y"<<Target.y;
    SmartKick<<std::endl;
    SmartKick<<"C"<<Cycle;
    return false;
}

ChainAction::TreeStack ChainAction::Get_Tree(rcsc::PlayerAgent *agent)
{
    
}

rcsc::Vector2D Bhv_BasicOffensiveKick::recursive_fu_shoot(rcsc::Vector2D ball_pos,rcsc::Vector2D left, rcsc::Vector2D right, int counter, const rcsc::WorldModel& wm , bool sideUp)
{
    Vector2D middle = ( left + right ) / 2 ;
    Triangle2D check1 = Triangle2D ( ball_pos , left , middle ) ;
    Triangle2D check2 = Triangle2D ( ball_pos , right , middle ) ;
    if (counter == 5)
    {
        if ( !wm.existOpponentIn ( check1 , 5 , true ) )
        {
            std :: cout << "\n\n Ok found ! \n\n";
            return ( middle + left ) / 2 ;
        }
        else if ( !wm.existOpponentIn ( check2 , 5 , true ) )
        {
            std :: cout << "\n\n Ok found ! \n\n";
            return ( middle + right ) / 2 ;
        }
        else
        {
            return Vector2D(0,0);
        }
    }
    if ( wm.countOpponentsIn ( check1 , 5 , true ) <
                wm.countOpponentsIn ( check2 , 5 , true ) )
        {
            std :: cout << "\n\n yeah continue ! \n\n";
            return recursive_fu_shoot ( ball_pos ,
                                        left ,
                                        middle ,
                                        counter+1 ,
                                        wm ,
                                        true ) ;
        }

    else if ( wm.countOpponentsIn ( check2 , 5 , true ) <
                wm.countOpponentsIn ( check1 , 5 , true ) )
    {
        std :: cout << "\n\n yeah continue ! \n\n";
        return recursive_fu_shoot ( ball_pos ,
                                    middle ,
                                    right ,
                                    counter+1 ,
                                    wm ,
                                    false ) ;
    }
    else if (sideUp)
    {
        return recursive_fu_shoot ( ball_pos ,
                                    left ,
                                    middle ,
                                    counter+1 ,
                                    wm ,
                                    true ) ;
    }
    else if (!sideUp)
    {
        return recursive_fu_shoot ( ball_pos ,
                                    middle ,
                                    right ,
                                    counter+1 ,
                                    wm ,
                                    false ) ;
    }

}

bool Bhv_BasicOffensiveKick::main_shoot(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();

    Vector2D self_pos = wm.self().pos();

    double y = ServerParam::i().goalHalfWidth() - 0.8;
    double x = ServerParam::i().pitchHalfLength();
    double dist_goal = self_pos.dist(Vector2D(x,0));
    dist_goal = std::min(dist_goal,self_pos.dist(Vector2D(x,y )));
    dist_goal = std::min(dist_goal,self_pos.dist(Vector2D(x,-y)));

    if (dist_goal>13)
        return false;

    Vector2D shoot_pos = recursive_fu_shoot(wm.ball().pos(),Vector2D(x,-y),
                                       Vector2D(x,y),0,wm,true);

    std::cout<<"pos ::: "<<shoot_pos.x<<","<<shoot_pos.y<<std::endl;

    if (shoot_pos == Vector2D(0,0))
    {
        return false;
    }

    Body_SmartKick(shoot_pos,2.7,2.7,3).execute(agent);
        return true;

}
