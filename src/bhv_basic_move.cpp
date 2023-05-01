// -*-c++-*-

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA
//
 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
//g
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
#include <sstream>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
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

#include <rcsc/action/body_kick_one_step.h>
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
#include <boost/range/empty.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/hana/empty.hpp>
#include <boost/metaparse/empty.hpp>
#include <boost/mpl/empty.hpp>
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

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

 */
bool
Bhv_BasicMove::execute( PlayerAgent * agent )
{
    const WorldModel & wm = agent->world();
    agent->debugClient().addLine(wm.self().pos(), wm.ball().pos() + wm.ball().vel());
    agent->debugClient().addLine(wm.self().pos(), Vector2D(wm.offsideLineX(), 0));
    int self_unum = wm.self().unum();
    const int self_min = wm.interceptTable()->selfReachCycle();
    const int mate_min = wm.interceptTable()->teammateReachCycle();
    const int opp_min = wm.interceptTable()->opponentReachCycle();
    if (self_min<=mate_min)
    {
            Body_Intercept().execute( agent );
            return true;
    }
    if(wm.lastKickerSide() != wm.ourSide()){
    agent->doPointto(wm.self().inertiaPoint(1).x,wm.self().inertiaPoint(1).y);}
    if(Scape(agent))
        return true;

    Vector2D target_point;

    /*if(wm.lastKickerSide() != wm.ourSide())
    {
        target_point = DeffenseFormation(agent, wm.self().unum());
    }
    else
    {
        target_point = getPosition(wm, wm.self().unum());
    }*/
    if((wm.self().unum() == 8 || wm.self().unum() == 7 || wm.self().unum() == 6 ) && wm.gameMode().side() == wm.ourSide()){
        target_point = OffensiveFormation(agent, wm.self().unum());

    }
    target_point = Strategy().Get_Formation(agent, wm.self().unum(), Strategy().Get_RecommendedStragetyType(agent));
    
    if((wm.self().unum() == 2 || wm.self().unum() == 3 || wm.self().unum() == 4 || wm.self().unum() == 5) && wm.lastKickerSide() == wm.ourSide())
    {
        target_point = OffensiveFormation(agent, wm.self().unum());
    }
    double dash_power;
    dash_power = dash(agent, target_point);
     if(wm.lastKickerSide() == wm.ourSide() && wm.self().unum() > 6){
        dash_power = 100;
    }
    if(wm.self().unum() > 8 && wm.lastKickerSide() != wm.ourSide())
    {
        if(wm.self().stamina() < ServerParam::i().staminaMax() * 7 / 10)
            dash_power = wm.self().stamina() / 300;
        else
            dash_power = 48;
    }
    if((wm.self().unum() == 7 || wm.self().unum() == 8 || wm.self().unum() == 6) && wm.lastKickerSide() == wm.ourSide())
    {
        if(wm.self().stamina() < ServerParam::i().staminaMax() * 7 / 10)
            dash_power = wm.self().stamina() / 300;
        else
            dash_power = 48;
    }
    if((wm.self().unum() == 4 || wm.self().unum() == 5 || wm.self().unum() == 2 || wm.self().unum() == 3) && wm.lastKickerSide() == wm.ourSide())
    {
        if(wm.self().stamina() < ServerParam::i().staminaMax() * 8 / 10)
            dash_power = wm.self().stamina() / 300;
        else
            dash_power = 48;
    }
    if(wm.self().unum() < 9 && wm.lastKickerSide() != wm.ourSide() && wm.ball().pos().x < -29
    )
    {dash_power = 100;}
     if(wm.self().unum() > 5 && wm.lastKickerSide() == wm.ourSide() && wm.ball().pos().x > 10 && wm.self().stamina() > 7000
    )
    {dash_power = 100;}
    if ( ! Body_GoToPoint( target_point,
        0, dash_power
                           ).execute( agent ) )
    {
        Body_TurnToBall().execute( agent );
    }

    return true;
}

rcsc::Vector2D Bhv_BasicMove::getPosition(const rcsc::WorldModel & wm, int self_unum){

    int ball_step = 0;
    if( wm.gameMode().type() == GameMode::PlayOn|| wm.gameMode().type() == GameMode::GoalKick_ )
    {
        ball_step = std::min( 1000, wm.interceptTable()->teammateReachCycle() );
        ball_step = std::min( ball_step, wm.interceptTable()->opponentReachCycle() );
        ball_step = std::min( ball_step, wm.interceptTable()->selfReachCycle() );
    }

    Vector2D ball_pos = wm.ball().inertiaPoint( ball_step );

    dlog.addText( Logger::TEAM,
                  __FILE__": HOME POSITION: ball pos=(%.1f %.1f) step=%d",
                  ball_pos.x, ball_pos.y,
                  ball_step );


    std::vector<Vector2D> positions(12);

    double min_x_rectengle[12]={0,-52,-52,-52,-52,-52,-50,-50,-40,-32,-32,-32}; //-50
    double max_x_rectengle[12]={0,-48,10,10,10,10,35,35,40,52,52,52}; //35
    double min_y_rectengle[12]={0,-2,-17,-11,-15,0,-10,-22, 5,-15,-33, 0}; //-10
    double max_y_rectengle[12]={0,2, 11, 17,0,15, 22, 10,7, 15, 0, 33};  // 22

    if(wm.self().unum() == 9 && wm.gameMode().type() == GameMode::KickOff_){
        return Vector2D(-3,-3);
    }
    if(wm.self().unum() == 11 && wm.gameMode().type() == GameMode::KickOff_){
        return Vector2D(-3,3);
    }

    if(wm.ball().pos().x >= 0 && wm.lastKickerSide()==wm.ourSide()){
        for(int a = 0 ; a<=11 ; a++)
        {
            if(a==2 && a==3 && a==6)
            {
                max_x_rectengle[a]=max_x_rectengle[a];
            }
            else {
                max_x_rectengle[a]=max_x_rectengle[a]+25;
            }
        }
    }

    for(int i=1; i<=11; i++){
          double xx_rectengle = max_x_rectengle[i] - min_x_rectengle[i];
          double yy_rectengle = max_y_rectengle[i] - min_y_rectengle[i];
          double x_ball = ball_pos.x + 52.5;
          x_ball /= 105.5;
          double y_ball = ball_pos.y + 34;
          y_ball /= 68.0;
          double x_pos = xx_rectengle * x_ball + min_x_rectengle[i];
          if(wm.gameMode().side()==wm.ourSide()){
              x_pos = xx_rectengle * x_ball + min_x_rectengle[i]+3;
        }
          double y_pos = yy_rectengle * y_ball + min_y_rectengle[i];
          positions[i] = Vector2D(x_pos,y_pos);
    }

    if((wm.self().unum() ==2 || wm.self().unum() ==3 || wm.self().unum() ==4 || wm.self().unum() ==5) && positions[wm.self().unum()].x>0){
        positions[wm.self().unum()]=wm.self().pos();
    }
    /*if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().type() != GameMode::FoulCharge_ && wm.gameMode().type() != GameMode::OffSide_ ){
    if(wm.gameMode().side() != wm.ourSide() && wm.ball().pos().x < 0 && wm.ball().pos().x > -22)
    {
        return full_press_1(wm);
    }
    }*/

    Vector2D c(4,5);

    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() == wm.ourSide() && wm.ball().pos().x > 32 && wm.ball().pos().y > -13 && wm.ball().pos().y < 13)
    {
        std::vector <Vector2D> defens_poss(11);
        defens_poss[0] = Vector2D(-50,0);//
        defens_poss[1] = Vector2D(9,-21);//
        defens_poss[2] = Vector2D(9,0);//
        defens_poss[3] = Vector2D(0,-10);//
        defens_poss[4] = Vector2D(0,10);//
        defens_poss[5] = Vector2D(30,0);//
        defens_poss[6] = Vector2D(35,-7);//
        defens_poss[7] = Vector2D(35,7);//
        defens_poss[8] = Vector2D(40,0);//
        defens_poss[9] = Vector2D(44,-8);//
        defens_poss[10] = Vector2D(44,8);//
        int xx;

            xx = wm.self().unum();
            return defens_poss[xx-1];
    }



    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() != wm.ourSide() && wm.ball().pos().x < -35 && wm.ball().pos().y > -20 && wm.ball().pos().y < 20)
    {
        std::vector <Vector2D> defens_pos(11);
        defens_pos[0] = Vector2D(-52,0);//
        defens_pos[1] = Vector2D(-52,-2);//2
        defens_pos[2] = Vector2D(-52,2);//3
        defens_pos[3] = Vector2D(-52,-4);//4
        defens_pos[4] = Vector2D(-52,4);//5
        defens_pos[5] = Vector2D(-51,0);//6
        defens_pos[6] = Vector2D(-51,-5);//7
        defens_pos[7] = Vector2D(-51,5);//8
        defens_pos[8] = Vector2D(-23,-0);//9
        defens_pos[9] = Vector2D(-51,-3);//10
        defens_pos[10] = Vector2D(-51,3);//11
        int x;

            x = wm.self().unum();
            return defens_pos[x-1];
    }
    if ( ServerParam::i().useOffside() )
    {
        double max_x = wm.offsideLineX();
        if ( ServerParam::i().kickoffOffside()
             && ( wm.gameMode().type() == GameMode::BeforeKickOff
                  || wm.gameMode().type() == GameMode::AfterGoal_ ) )
        {
            max_x = 0.0;
        }
        else
        {
            int mate_step = wm.interceptTable()->teammateReachCycle();
            if ( mate_step < 50 )
            {
                Vector2D trap_pos = wm.ball().inertiaPoint( mate_step );
                if ( trap_pos.x > max_x ) max_x = trap_pos.x;
            }

            max_x -= 1.0;
        }

        for ( int unum = 1; unum <= 11; ++unum )
        {
            if ( positions[unum].x > max_x )
            {
                dlog.addText( Logger::TEAM,
                              "____ %d offside. home_pos_x %.2f -> %.2f",
                              unum,
                              positions[unum].x, max_x );
                positions[unum].x = max_x;
            }
        }
    }
    if(self_unum > 5 && self_unum < 9)
    {
        if(positions.at(self_unum).x > 40)
            positions.at(self_unum).x = 40;
    }
    return positions.at(self_unum);
}

double Bhv_BasicMove::get_normal_dash_power( const WorldModel & wm )
{
    static bool s_recover_mode = false;

    if ( wm.self().staminaModel().capacityIsEmpty() )
    {
        return std::min( ServerParam::i().maxDashPower(),
                         wm.self().stamina() + wm.self().playerType().extraStamina() );
    }

    // check recover
    if ( wm.self().staminaModel().capacityIsEmpty() )
    {
        s_recover_mode = false;
    }
    else if ( wm.self().stamina() < ServerParam::i().staminaMax() * 0.5 )
    {
        s_recover_mode = true;
    }
    else if ( wm.self().stamina() > ServerParam::i().staminaMax() * 0.7 )
    {
        s_recover_mode = false;
    }

    /*--------------------------------------------------------*/
    double dash_power = ServerParam::i().maxDashPower();
    const double my_inc
        = wm.self().playerType().staminaIncMax()
        * wm.self().recovery();

    if ( wm.ourDefenseLineX() > wm.self().pos().x
         && wm.ball().pos().x < wm.ourDefenseLineX() + 20.0 )
    {
    }
    else if ( s_recover_mode )
    {
    }
    else if ( wm.existKickableTeammate()
              && wm.ball().distFromSelf() < 20.0 )
    {
    }
    else if ( wm.self().pos().x > wm.offsideLineX() )
    {
    }
    else
    {
        dash_power = std::min( my_inc * 1.7,
                               ServerParam::i().maxDashPower() );
    }

    return dash_power;
}

bool Bhv_BasicMove::press(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    auto tm = wm.ourPlayers();
    Vector2D T1;
    Vector2D T2;
    Vector2D T3;
    Vector2D T4;
    for(int i=0;i<=10;i++)
    {
        if(tm[i]->unum() == 4 )
        {
            if(tm[i]->unum() != wm.self().unum()){
            T1=tm[i]->pos();

        }
            else{
                T1=Vector2D(0,0);
            }
        }
        if(tm[i]->unum() == 5 )
        {
            if(tm[i]->unum() != wm.self().unum()){
            T2=tm[i]->pos();
        }
            else{
                T2=Vector2D(0,0);
            }
        }
        if(tm[i]->unum() == 7 )
        {
            if(tm[i]->unum() != wm.self().unum()){
            T3=tm[i]->pos();
        }
            else{
                T3=Vector2D(0,0);
            }
        }
        if(tm[i]->unum() == 8 )
        {
            if(tm[i]->unum() != wm.self().unum()){
            T4=tm[i]->pos();
        }
        else{
                T1=Vector2D(0,0);
            }

        }
    }
    std::vector <Vector2D> tms;
    if(T1 != Vector2D(0,0))
    {
        tms.push_back(T1);
    }
    if(T2 != Vector2D(0,0))
    {
        tms.push_back(T2);
    }
    if(T3 != Vector2D(0,0))
    {
        tms.push_back(T3);
    }
    if(T4 != Vector2D(0,0))
    {
        tms.push_back(T4);
    }
    const PlayerPtrCont opp = wm.opponentsFromSelf();
    Vector2D center_goal(-52.5,0);
    PlayerPtrCont Nearest_opp;
    for(int i=0;i<10;i++)
        {
            if(opp[i]->pos().y > -20 && opp[i]->pos().y < 20){
            Nearest_opp.push_back(opp[i]);}
        }
    for(int i=0; i<=8;i++)
        {
            for(int j=i+1;j<=9;j++)
            {
                if(Nearest_opp[i]->pos().x > Nearest_opp[j]->pos().x)
                    {

                        std::swap(Nearest_opp[i],Nearest_opp[j]);
                    }
            }
        }
        Vector2D target_pos;
        Vector2D self_pos = wm.self().pos();
        int counter = 0;
    int unum = wm.self().unum();
 if(unum == 4 || unum == 5 || unum == 7 || unum == 8)
 {
    if( self_pos.dist(Nearest_opp[0]->pos()) < tms[0].dist(Nearest_opp[0]->pos()) && self_pos.dist(Nearest_opp[0]->pos()) < tms[1].dist(Nearest_opp[0]->pos()) && self_pos.dist(Nearest_opp[0]->pos()) < tms[2].dist(Nearest_opp[0]->pos())  )
    {
        target_pos = Vector2D(Nearest_opp[0]->pos().x ,Nearest_opp[0]->pos().y);
        counter++;
    }
    else if( self_pos.dist(Nearest_opp[1]->pos()) < tms[0].dist(Nearest_opp[1]->pos()) && self_pos.dist(Nearest_opp[1]->pos()) < tms[1].dist(Nearest_opp[1]->pos()) && self_pos.dist(Nearest_opp[1]->pos()) < tms[2].dist(Nearest_opp[1]->pos())   )
    {
        target_pos = Vector2D(Nearest_opp[1]->pos().x ,Nearest_opp[1]->pos().y);
        counter++;
    }
    else if( self_pos.dist(Nearest_opp[2]->pos()) < tms[0].dist(Nearest_opp[2]->pos()) && self_pos.dist(Nearest_opp[2]->pos()) < tms[1].dist(Nearest_opp[2]->pos()) && self_pos.dist(Nearest_opp[2]->pos()) < tms[2].dist(Nearest_opp[2]->pos())  )
    {
        target_pos = Vector2D(Nearest_opp[2]->pos().x ,Nearest_opp[2]->pos().y);
        counter++;
    }
    else if( self_pos.dist(Nearest_opp[3]->pos()) < tms[0].dist(Nearest_opp[3]->pos()) && self_pos.dist(Nearest_opp[3]->pos()) < tms[1].dist(Nearest_opp[3]->pos()) && self_pos.dist(Nearest_opp[3]->pos()) < tms[2].dist(Nearest_opp[3]->pos())  )
    {
        target_pos = Vector2D(Nearest_opp[3]->pos().x ,Nearest_opp[3]->pos().y);
        counter++;
    }
  }
  if(counter!=0)
    {
        Body_GoToPoint(target_pos,0,100).execute(agent);
        return true;
    }
        return false;
}
rcsc::Vector2D Bhv_BasicMove::last_difense(const rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    const PlayerPtrCont opp = wm.opponentsFromBall();
    PlayerPtrCont nearest_opg;
    double ball_pos_x = wm.ball().pos().x;
    if(wm.ball().pos().x > 4 || wm.ball().pos().y < -26 || wm.ball().pos().y > 26)
    Vector2D target;
    int number = 100;
    int number1= 100;
    for(int i=0;i<10;i++)
    {
        nearest_opg.push_back(opp[i]);
    }
    for(int i=0; i<=8;i++)
    {
        for(int j=i+1;j<=9;j++)
        {
            if(nearest_opg[i]->pos().dist(Vector2D(-52.5,0)) > nearest_opg[j]->pos().dist(Vector2D(-52.5,0)))
            {
                std::swap(nearest_opg[i],nearest_opg[j]);
            }
        }
    }
    Vector2D opp_pos = nearest_opg[0]->pos();
    Vector2D opp_pos1 = nearest_opg[1]->pos();
    double No = wm.getDistTeammateNearestTo(opp_pos,5);
    double No1 = wm.getDistTeammateNearestTo(opp_pos1,5);
    if(wm.self().pos().dist(nearest_opg[0]->pos()) <= No)
    {
        number = wm.self().unum();
    }
    if(wm.self().pos().dist(nearest_opg[1]->pos()) <= No1 && wm.self().unum() != number)
    {
        number1 = wm.self().unum();
    }
/*    if(wm.self().unum() < 7 && wm.self().unum() > 1 && ball_pos_x < 5 && wm.ball().pos().y > -27 && wm.ball().pos().y < 27 && wm.lastKickerSide() != wm.ourSide() && self_interceptor(agent)==false && opp_pos.y > -20 && opp_pos.y < 20)
    {
        if (wm.self().unum()==number)
        {
            return Vector2D(nearest_opg[0]->pos().x-1,nearest_opg[0]->pos().y);
        }
        if (wm.self().unum()==number1)
        {
            return Vector2D(nearest_opg[1]->pos().x-1,nearest_opg[1]->pos().y);
        }
    }*/
    return Vector2D(100,100);
}

rcsc::Vector2D Bhv_BasicMove::full_press(const rcsc::WorldModel &wm)
{
    const PlayerPtrCont opp = wm.opponentsFromSelf();
    Vector2D go_pos;
    PlayerPtrCont Nearest_opp;
    for(int i=0;i<10;i++)
    {
        Nearest_opp.push_back(opp[i]);
    }
    for(int i=0; i<=8;i++)
    {
        for(int j=i+1;j<=9;j++)
        {
            if(Nearest_opp[i]->pos().x > Nearest_opp[j]->pos().x)
            {
                std::swap(Nearest_opp[i],Nearest_opp[j]);
            }
        }
    }
    for(int i=0;i<11;i++)
    {
        if(wm.self().unum() == 1)
        {
            go_pos = Vector2D(-52.5,0);
        }
        if(wm.self().unum() == 2 && opp[i]->unum() == 10 )
        {
            if(wm.ball().pos().x > -25)
                go_pos = Vector2D(Nearest_opp[0]->pos().x - 2,-10);
            else{
            go_pos = opp[i]->pos();}
        }
        if(wm.self().unum() == 3 && opp[i]->unum() == 11)
        {
            if(wm.ball().pos().x > -25)
                go_pos = Vector2D(Nearest_opp[0]->pos().x - 2,10);
            else{
            go_pos = opp[i]->pos();}
        }
        if(wm.self().unum() == 4 && opp[i]->unum() == 8)
        {
            if(wm.ball().pos().x > -25)
                go_pos = Vector2D(Nearest_opp[0]->pos().x - 2,-15);
            else{
            go_pos = opp[i]->pos();}
        }
        if(wm.self().unum() == 5 && opp[i]->unum() == 9)
        {
            if(wm.ball().pos().x > -25)
                go_pos = Vector2D(Nearest_opp[0]->pos().x - 2,15);
            else{
            go_pos = opp[i]->pos();}
        }
        if(wm.self().unum() == 6 && opp[i]->unum() == 6)
        {
                if(wm.ball().pos().x > -25)
                go_pos = Vector2D(Nearest_opp[0]->pos().x - 2,0);
                else{
                go_pos = opp[i]->pos();}
        }
        if(wm.self().unum() == 7 && opp[i]->unum() == 5)
        {
            go_pos = opp[i]->pos();
        }
        if(wm.self().unum() == 8 && opp[i]->unum() == 4)
        {
            go_pos = opp[i]->pos();
        }
        if(wm.self().unum() == 9)
        {
            go_pos = wm.ball().pos();
        }
        if(wm.self().unum() == 10 && opp[i]->unum() == 3)
        {
            go_pos = opp[i]->pos();
        }
        if(wm.self().unum() == 11 && opp[i]->unum() == 2)
        {
            go_pos = opp[i]->pos();
        }
    }
    return go_pos;
}

bool Bhv_BasicMove::s_e_intersept(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    int unum = 7;
    if(wm.self().unum() == 7)
    {
        unum = unum + 1;
    }
    const AbstractPlayerObject *tm = wm.ourPlayer(unum);
    if(tm != NULL && wm.self().pos().dist(wm.ball().pos()) < tm->pos().dist(wm.ball().pos()))
    {
        Body_Intercept().execute(agent);
        return true;
    }
    return false;
}

bool Bhv_BasicMove::cut_OPPs_pass_path(rcsc::PlayerAgent *agent)
{
    const WorldModel &wm = agent->world();
    if(wm.self().unum() != 7 && wm.self().unum() != 8)
        return false;
    if(wm.lastKickerSide() == wm.ourSide())
        return false;
    Vector2D center(-52,0);
    const PlayerPtrCont opp = wm.opponentsFromBall();
    PlayerPtrCont Nearest_opp;
    for(int i=0;i<10;i++)
    {
        Nearest_opp.push_back(opp[i]);
    }
    for(int i=0; i<=8;i++)
    {
        for(int j=i+1;j<=9;j++)
        {
            if(Nearest_opp[i]->pos().x > Nearest_opp[j]->pos().x)
            {
                std::swap(Nearest_opp[i],Nearest_opp[j]);
            }
        }
    }
    const PlayerPtrCont opp_from_ball = wm.opponentsFromBall();
    if(opp_from_ball[0]->pos() == Nearest_opp[0]->pos() || opp_from_ball[0]->pos() == Nearest_opp[1]->pos())
        return false;
    PlayerPtrCont Nearest_opp_to_opp;
    for(int i=0;i<10;i++)
    {
        Nearest_opp_to_opp.push_back(opp[i]);
    }
    for(int i=0; i<=8;i++)
    {
        for(int j=i+1;j<=9;j++)
        {
                if(Nearest_opp_to_opp[i]->pos().dist(opp_from_ball[0]->pos()) > Nearest_opp_to_opp[j]->pos().dist(opp_from_ball[0]->pos()))
                {
                    std::swap(Nearest_opp_to_opp[i],Nearest_opp_to_opp[j]);
                }
        }
    }
    double dist_opp_1 = opp_from_ball[0]->pos().dist(Nearest_opp_to_opp[0]->pos());
    double dist_opp_2 = opp_from_ball[0]->pos().dist(Nearest_opp_to_opp[1]->pos());
    double angle_1 = (Nearest_opp_to_opp[0]->pos() - opp_from_ball[0]->pos()).th().degree();
    double angle_2 = (Nearest_opp_to_opp[1]->pos() - opp_from_ball[0]->pos()).th().degree();
    Vector2D opp_1_cut = Vector2D::polar2vector(dist_opp_1/3,angle_1) + Nearest_opp_to_opp[0]->pos();
    Vector2D opp_2_cut = Vector2D::polar2vector(dist_opp_2/3,angle_2) + Nearest_opp_to_opp[1]->pos();
    Vector2D self_pos = wm.self().pos();
    if(self_pos.dist(opp_1_cut) < self_pos.dist(opp_2_cut))
    {
        Body_GoToPoint(opp_1_cut,0,get_normal_dash_power(wm)).execute(agent);
        return true;
    }
    else
    {
        Body_GoToPoint(opp_2_cut,0,get_normal_dash_power(wm)).execute(agent);
        return true;
    }
    return false;
}

bool Bhv_BasicMove::BodyGoToPassablePos(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    if(wm.self().unum() < 7 || wm.ball().pos().x < 11 || wm.lastKickerSide() != wm.ourSide())
        return false;
    Vector2D ball_pos = wm.ball().pos();
    Vector2D self_pos = wm.self().pos();
    double x = ball_pos.x;
    double y = ball_pos.y;
    std::vector <Vector2D> GoodPoses;
    for(double xx = x - 30 ; xx <= x - 30 ; xx++)
    {
        for(double yy = y - 30 ; y <= yy + 30 ; yy++)
        {
            Vector2D check(xx,yy);
            if(!check.isValid())
                continue;
            double ang = (check - ball_pos).th().degree();
            Sector2D check_sector = Sector2D(ball_pos,1,ball_pos.dist(check),ang + 15,ang - 15);
            if(!wm.existOpponentIn(check_sector,5,true))
            {
                GoodPoses.push_back(check);
            }
        }
    }
    int existing_tms = 0;
    for(int i = 7 ; i <= 11 ; i++)
    {
        const AbstractPlayerObject * tmc = wm.ourPlayer(i);
        if(tmc != NULL || tmc->unum() != wm.self().unum())
            existing_tms ++;
    }
    std::vector <Vector2D> GoPoses;
    int counter;
    for(int i = 0 ; i < GoodPoses.size() ; i++)
    {
        counter = 0;
        for(int j = 7 ; j <= 11 ; j++)
        {
            const AbstractPlayerObject * tm = wm.ourPlayer(j);
            if(tm == NULL || tm->unum() == wm.self().unum())
                return false;
            if(tm->pos().dist(GoodPoses[i]) > self_pos.dist(GoodPoses[i]))
                counter ++;
            if(j == 11 && counter > existing_tms)
                GoPoses.push_back(GoodPoses[i]);
        }
    }
    if(GoPoses.empty())
        return false;
    if(GoPoses.size() == 1)
    {
        Body_GoToPoint(GoPoses[0],0,90).execute(agent);
        return true;
    }
    Vector2D NearestGoPos = GoPoses[0];
    for(int i = 1 ; i < GoPoses.size() ; i++)
    {
        if(NearestGoPos.dist(self_pos) > GoPoses[i].dist(self_pos))
            NearestGoPos = GoodPoses[i];
    }
    Body_GoToPoint(NearestGoPos,0,90).execute(agent);
    return true;
}

bool Bhv_BasicMove::self_tacklatoration(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    const int self_min = wm.interceptTable()->selfReachCycle();
    const int mate_min = wm.interceptTable()->teammateReachCycle();
    const int opp_min = wm.interceptTable()->opponentReachCycle();
    /*if(opp_min < mate_min && opp_min < self_min)
        return false;*/
    /*if(wm.ball().pos().x < -20)
    {
        Sector2D check = Sector2D(wm.ball().pos(),0,wm.ball().pos().dist(Vector2D(-52,0)),(Vector2D(-52,0) - wm.ball().pos()).th() + 30,(Vector2D(-52,0) - wm.ball().pos()).th() - 30);
        if(!wm.existTeammateIn(check,5,false))
        {
            Body_Intercept().execute(agent);
            return true;
        }
    }*/
    static bool BodyTurnCheck = false;
    if(wm.self().distFromBall() < 1.5 && wm.ball().pos().x < -20)
    {
        if(!BodyTurnCheck)
        {
            BodyTurnCheck = true;
            Body_TurnToBall().execute(agent);
        }

    }
    /*if(wm.self().distFromBall() < 1 && wm.ball().pos().x >= -40)
    {
        agent->doTackle(20,false);
        return true;
    }*/
   /* if(self_min < mate_min && self_min < 7 && wm.self().pos().x < wm.ball().pos().x + 6)
    {
        Body_GoToPoint(Vector2D( wm.ball().inertiaPoint(4).x-1,wm.ball().inertiaPoint(4).y),0,100).execute(agent);
        return true;
    }*/
    return false;
}

int Bhv_BasicMove::return_pos(const rcsc::PlayerAgent *agent)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    Vector2D poses[117];
    //From up to down
    poses[0].assign(-51,-34);//1
    poses[1].assign(-51,-25.5);//2
    poses[2].assign(-51,-17);//3
    poses[3].assign(-51,-8.5);//4
    poses[4].assign(-51,0);//5
    poses[5].assign(-51,8.5);//6
    poses[6].assign(-51,17);//7
    poses[7].assign(-51,25.5);//8
    poses[8].assign(-51,34);//9
    poses[9].assign(-42.5,-34);//10
    poses[10].assign(-42.5,-25.5);//11
    poses[11].assign(-42.5,-17);//12
    poses[12].assign(-42.5,-8.5);//13
    poses[13].assign(-42.5,0);//14
    poses[14].assign(-42.5,8.5);//15
    poses[15].assign(-42.5,17);//16
    poses[16].assign(-42.5,25.5);//17
    poses[17].assign(-42.5,34);//18
    poses[18].assign(-34,-34);//19
    poses[19].assign(-34,-25.5);//20
    poses[20].assign(-34,-17);//21
    poses[21].assign(-34,-8.5);//22
    poses[22].assign(-34,0);//23
    poses[23].assign(-34,8.5);//24
    poses[24].assign(-34,17);//25
    poses[25].assign(-34,25.5);//26
    poses[26].assign(-34,34);//27
    poses[27].assign(-25.5,-34);//28
    poses[28].assign(-25.5,-25.5);//29
    poses[29].assign(-25.5,-17);//30
    poses[30].assign(-25.5,-8.5);//31
    poses[31].assign(-25.5,0);//32
    poses[32].assign(-25.5,8.5);//33
    poses[33].assign(-25.5,17);//34
    poses[34].assign(-25.5,25.5);//35
    poses[35].assign(-25.5,34);//36
    poses[36].assign(-17,-34);//37
    poses[37].assign(-17,-25.5);//38
    poses[38].assign(-17,-17);//39
    poses[39].assign(-17,-8.5);//40
    poses[40].assign(-17,0);//41
    poses[41].assign(-17,8.5);//42
    poses[42].assign(-17,17);//43
    poses[43].assign(-17,25.5);//44
    poses[44].assign(-17,34);//45
    poses[45].assign(-8.5,-34);//46
    poses[46].assign(-8.5,-25.5);//47
    poses[47].assign(-8.5,-17);//48
    poses[48].assign(-8.5,-8.5);//49
    poses[49].assign(-8.5,0);//50
    poses[50].assign(-8.5,8.5);//51
    poses[51].assign(-8.5,17);//52
    poses[52].assign(-8.5,25.5);//53
    poses[53].assign(-8.5,34);//54
    poses[54].assign(0,-34);//55
    poses[55].assign(0,-25.5);//56
    poses[56].assign(0,-17);//57
    poses[57].assign(0,-8.5);//58
    poses[58].assign(0,0);//59
    poses[59].assign(0,8.5);//60
    poses[60].assign(0,17);//61
    poses[61].assign(0,25.5);//62
    poses[62].assign(0,34);//63
    poses[63].assign(8.5,-34);//64
    poses[64].assign(8.5,-25.5);//65
    poses[65].assign(8.5,-17);//66
    poses[66].assign(8.5,-8.5);//67
    poses[67].assign(8.5,0);//68
    poses[68].assign(8.5,8.5);//69
    poses[69].assign(8.5,17);//70
    poses[70].assign(8.5,25.5);//71
    poses[71].assign(8.5,34);//72
    poses[72].assign(17,-34);//73
    poses[73].assign(17,-25.5);//74
    poses[74].assign(17,-17);//75
    poses[75].assign(17,-8.5);//76
    poses[76].assign(17,0);//77
    poses[77].assign(17,8.5);//78
    poses[78].assign(17,17);//79
    poses[79].assign(17,25.5);//80
    poses[80].assign(17,34);//81
    poses[81].assign(25.5,-34);//82
    poses[82].assign(25.5,-25.5);//83
    poses[83].assign(25.5,-17);//84
    poses[84].assign(25.5,-8.5);//85
    poses[85].assign(25.5,0);//86
    poses[86].assign(25.5,8.5);//87
    poses[87].assign(25.5,17);//88
    poses[88].assign(25.5,25.5);//89
    poses[89].assign(25.5,34);//90
    poses[90].assign(34,-34);//91
    poses[91].assign(34,-25.5);//92
    poses[92].assign(34,-17);//93
    poses[93].assign(34,-8.5);//94
    poses[94].assign(34,0);//95
    poses[95].assign(34,8.5);//96
    poses[96].assign(34,17);//97
    poses[97].assign(34,25.5);//98
    poses[98].assign(34,34);//99
    poses[99].assign(42.5,-34);//100
    poses[100].assign(42.5,-25.5);//101
    poses[101].assign(42.5,-17);//102
    poses[102].assign(42.5,-8.5);//103
    poses[103].assign(42.5,0);//104
    poses[104].assign(42.5,8.5);//105
    poses[105].assign(42.5,17);//106
    poses[106].assign(42.5,25.5);//107
    poses[107].assign(42.5,34);//108
    poses[108].assign(51,-34);//109
    poses[109].assign(51,-25.5);//110
    poses[110].assign(51,-17);//111
    poses[111].assign(51,-8.5);//112
    poses[112].assign(51,0);//113
    poses[113].assign(51,8.5);//114
    poses[114].assign(51,17);//115
    poses[115].assign(51,25.5);//116
    poses[116].assign(51,34);//117
    Vector2D nearest = poses[0];
    int count = 0;
    for(int i=1;i<=116;i++)
    {
        if(ball_pos.dist(poses[i]) < ball_pos.dist(nearest))
        {
            nearest = poses[i];
            count = i;
        }
    }
    Circle2D cir = Circle2D(nearest,4);
    std::cout<<"Cicle: "<<wm.time().cycle()<<"return pos is (" << nearest.x<<", "<<nearest.y<<") & return pos number is:    "<<count<<std::endl;
    return count;
}

rcsc::Vector2D Bhv_BasicMove::deffense_poses(const rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    int count = return_pos(agent);
    std::vector <Vector2D> poses(12);
    if(count == 0)
    {
        poses.at(1) = Vector2D( -51,-1       );
        poses.at(2) = Vector2D( -48.5,-7.7   );
        poses.at(3) = Vector2D( -45,-2       );
        poses.at(4) = Vector2D( -49,-27      );
        poses.at(5) = Vector2D( -48,5        );
        poses.at(6) = Vector2D( -43,-13      );
        poses.at(7) = Vector2D( -42,2        );
        poses.at(8) = Vector2D( -39,-6       );
        poses.at(9) = Vector2D( -28,2        );
        poses.at(10)= Vector2D( -27,-23      );
        poses.at(11)= Vector2D( -16,16       );
    }
    if(count == 1)
    {
        poses.at(1) = Vector2D(-51,-1   );
        poses.at(2) = Vector2D(-49,-5   );
        poses.at(3) = Vector2D(-45,-2   );
        poses.at(4) = Vector2D(-49,-20  );
        poses.at(5) = Vector2D(-49,4    );
        poses.at(6) = Vector2D(-43,-10  );
        poses.at(7) = Vector2D(-42,2    );
        poses.at(8) = Vector2D(-39,-5   );
        poses.at(9) = Vector2D(-28,3    );
        poses.at(10)= Vector2D(-30,-13  );
        poses.at(11)= Vector2D(-16,16   );
    }
    if(count == 2)
    {
        poses.at(1) = Vector2D(-51,-2   );
        poses.at(2) = Vector2D(-51,-6   );
        poses.at(3) = Vector2D(-45,-2   );
        poses.at(4) = Vector2D(-49,-13  );
        poses.at(5) = Vector2D(-49,4    );
        poses.at(6) = Vector2D(-45,7    );
        poses.at(7) = Vector2D(-41,2    );
        poses.at(8) = Vector2D(-39,-5   );
        poses.at(9) = Vector2D(-28,2    );
        poses.at(10)= Vector2D(-30,-13  );
        poses.at(11)= Vector2D(-16,16   );
    }
    if(count == 3)
    {
        poses.at(1) = Vector2D(-51,-1   );
        poses.at(2) = Vector2D(-52,-6   );   
        poses.at(3) = Vector2D(-47,-3   );
        poses.at(4) = Vector2D(-50,-7   );
        poses.at(5) = Vector2D(-50,4    );
        poses.at(6) = Vector2D(-41,0    );
        poses.at(7) = Vector2D(-47,1    );
        poses.at(8) = Vector2D(-41,-7   );
        poses.at(9) = Vector2D(-28,2    );
        poses.at(10)= Vector2D(-27,-12  );
        poses.at(11)= Vector2D(-16,16   );
    }
    if(count == 4)
    {
        poses.at(1) = Vector2D(-50,0    );
        poses.at(2) = Vector2D(-51,-3   );
        poses.at(3) = Vector2D(-51,3    );
        poses.at(4) = Vector2D(-51,-6   );
        poses.at(5) = Vector2D(-51,6    );
        poses.at(6) = Vector2D(-51,-1   );
        poses.at(7) = Vector2D(-51,1    );
        poses.at(8) = Vector2D(-41,-7   );
        poses.at(9) = Vector2D(-28,3    );
        poses.at(10)= Vector2D(-27,-12  );
        poses.at(11)= Vector2D(-16,16   );
    }
    if(count == 5)
    {
        poses.at(1) = Vector2D(-51,4    );
        poses.at(2) = Vector2D(-49,-3   );
        poses.at(3) = Vector2D(-44,5    );
        poses.at(4) = Vector2D(-44,-6   );
        poses.at(5) = Vector2D(-50,7    );
        poses.at(6) = Vector2D(-47,2    );
        poses.at(7) = Vector2D(-43,1    );
        poses.at(8) = Vector2D(-41,-4   );
        poses.at(9) = Vector2D(-28,0    );
        poses.at(10)= Vector2D(-16,-22  );
        poses.at(11)= Vector2D(-28,16   );
    }
    if(count == 6)
    {
        poses.at(1) = Vector2D(-51,5    );
        poses.at(2) = Vector2D(-50,-2   );
        poses.at(3) = Vector2D(-46,7    );
        poses.at(4) = Vector2D(-44,-7   );
        poses.at(5) = Vector2D(-49,14   );
        poses.at(6) = Vector2D(-48,3    );
        poses.at(7) = Vector2D(-43,1    );
        poses.at(8) = Vector2D(-40,-3   );
        poses.at(9) = Vector2D(-28,0    );
        poses.at(10)= Vector2D(-16,-22  );
        poses.at(11)= Vector2D(-28,14   );
    }
    if(count == 7)
    {
        poses.at(1) = Vector2D(-51,5    );
        poses.at(2) = Vector2D(-49,-1   );
        poses.at(3) = Vector2D(-45,10   );
        poses.at(4) = Vector2D(-45,-4   );
        poses.at(5) = Vector2D(-49,18   );
        poses.at(6) = Vector2D(-46,4    );
        poses.at(7) = Vector2D(-43,1    );
        poses.at(8) = Vector2D(-40,-3   );
        poses.at(9) = Vector2D(-28,0    );
        poses.at(10)= Vector2D(-16,-22  );
        poses.at(11)= Vector2D(-28,16   );
    }
    if(count == 8)
    {
        poses.at(1) = Vector2D(-51,5    );
        poses.at(2) = Vector2D(-46,2    );
        poses.at(3) = Vector2D(-42,12   );
        poses.at(4) = Vector2D(-51,-4   );
        poses.at(5) = Vector2D(-49,26   );
        poses.at(6) = Vector2D(-41,3    );
        poses.at(7) = Vector2D(-47,11   );
        poses.at(8) = Vector2D(-49,-3   );
        poses.at(9) = Vector2D(-28,0    );
        poses.at(10)= Vector2D(-16,-22  );
        poses.at(11)= Vector2D(-28,16   );
    }
    if(count == 9)
    {
        poses.at(1) = Vector2D(-48,-3   );
        poses.at(2) = Vector2D(-42,-17  );
        poses.at(3) = Vector2D(-39,2    );
        poses.at(4) = Vector2D(-43,-26  );
        poses.at(5) = Vector2D(-35,7    );
        poses.at(6) = Vector2D(-41,-11  );
        poses.at(7) = Vector2D(-40,-4   );
        poses.at(8) = Vector2D(-31,-15  );
        poses.at(9) = Vector2D(-21,0    );
        poses.at(10)= Vector2D(-16,-23  );
        poses.at(11)= Vector2D(-21,15   );
    }
    if(count == 10)
    {
        poses.at(1) = Vector2D(-48,-4   );
        poses.at(2) = Vector2D(-43,-9   );
        poses.at(3) = Vector2D(-41,4    );
        poses.at(4) = Vector2D(-43,-17  );
        poses.at(5) = Vector2D(-35,7    );
        poses.at(6) = Vector2D(-42,-5   );
        poses.at(7) = Vector2D(-40,-2   );
        poses.at(8) = Vector2D(-33,-14  );
        poses.at(9) = Vector2D(-21,0    );
        poses.at(10)= Vector2D(-22,-22  );
        poses.at(11)= Vector2D(-21,16   );
    }
    if(count == 11)
    {
        poses.at(1) = Vector2D(-49,-3   );
        poses.at(2) = Vector2D(-43,-9   );
        poses.at(3) = Vector2D(-40,0    );
        poses.at(4) = Vector2D(-44,-12  );
        poses.at(5) = Vector2D(-39,5    );
        poses.at(6) = Vector2D(-42,-6   );
        poses.at(7) = Vector2D(-40,-2   );
        poses.at(8) = Vector2D(-35,-11  );
        poses.at(9) = Vector2D(-21,0    );
        poses.at(10)= Vector2D(-22,-22  );
        poses.at(11)= Vector2D(-21,15   );
    }
    if(count == 12)
    {
        poses.at(1) = Vector2D(-51,-4   );
        poses.at(2) = Vector2D(-48,0    );
        poses.at(3) = Vector2D(-42,2    );
        poses.at(4) = Vector2D(-44,-8   );
        poses.at(5) = Vector2D(-46,5    );
        poses.at(6) = Vector2D(-44,-5   );
        poses.at(7) = Vector2D(-43,-2   );
        poses.at(8) = Vector2D(-35,-11  );
        poses.at(9) = Vector2D(-22,-1   );
        poses.at(10)= Vector2D(-22,-22  );
        poses.at(11)= Vector2D(-22,16   );
    }
    if(count == 13)
    {
        poses.at(1) = Vector2D(-49,0    );
        poses.at(2) = Vector2D(-43,-5   );
        poses.at(3) = Vector2D(-43,4    );
        poses.at(4) = Vector2D(-44,-9   );
        poses.at(5) = Vector2D(-44,7    );
        poses.at(6) = Vector2D(-43,-2   );
        poses.at(7) = Vector2D(-43,1    );
        poses.at(8) = Vector2D(-33,-7   );
        poses.at(9) = Vector2D(-22,-1   );
        poses.at(10)= Vector2D(-22,-22  );
        poses.at(11)= Vector2D(-22,22   );
    }
    if(count == 14)
    {
        poses.at(1) = Vector2D(-49,0    );
        poses.at(2) = Vector2D(-43,-3   );
        poses.at(3) = Vector2D(-45,7    );
        poses.at(4) = Vector2D(-42,-8   );
        poses.at(5) = Vector2D(-45,11   );
        poses.at(6) = Vector2D(-43,1    );
        poses.at(7) = Vector2D(-43,4    );
        poses.at(8) = Vector2D(-35,-4   );
        poses.at(9) = Vector2D(-22,0    );
        poses.at(10)= Vector2D(-22,-22  );
        poses.at(11)= Vector2D(-22,22   );
    }
    if(count == 15)
    {
        poses.at(1) = Vector2D(-50,3    );
        poses.at(2) = Vector2D(-44,0    );
        poses.at(3) = Vector2D(-45,9    );
        poses.at(4) = Vector2D(-42,-5   );
        poses.at(5) = Vector2D(-43,13   );
        poses.at(6) = Vector2D(-44,5    );
        poses.at(7) = Vector2D(-41,4    );
        poses.at(8) = Vector2D(-35,1    );
        poses.at(9) = Vector2D(-22,0    );
        poses.at(10)= Vector2D(-22,-22  );
        poses.at(11)= Vector2D(-22,22   );
    }
    if(count == 16)
    {
        poses.at(1) = Vector2D(-50,4    );
        poses.at(2) = Vector2D(-43,0    );
        poses.at(3) = Vector2D(-46,12   );
        poses.at(4) = Vector2D(-49,-3   );
        poses.at(5) = Vector2D(-44,20   );
        poses.at(6) = Vector2D(-45,8    );
        poses.at(7) = Vector2D(-43,6    );
        poses.at(8) = Vector2D(-40,4    );
        poses.at(9) = Vector2D(-22,0    );
        poses.at(10)= Vector2D(-22,-22  );
        poses.at(11)= Vector2D(-22,22   );
    }
    if(count == 17)
    {
        poses.at(1) = Vector2D(-49,5    );
        poses.at(2) = Vector2D(-43,3    );
        poses.at(3) = Vector2D(-44,15   );
        poses.at(4) = Vector2D(-42,0    );
        poses.at(5) = Vector2D(-42,25   );
        poses.at(6) = Vector2D(-43,6    );
        poses.at(7) = Vector2D(-44,10   );
        poses.at(8) = Vector2D(-40,-2   );
        poses.at(9) = Vector2D(-22,-1   );
        poses.at(10)= Vector2D(-22,-22  );
        poses.at(11)= Vector2D(-28,22   );
    }
    if(count == 18)
    {
        poses.at(1) = Vector2D(-48,-1   );
        poses.at(2) = Vector2D(-37,-18  );
        poses.at(3) = Vector2D(-34,-2   );
        poses.at(4) = Vector2D(-35,-27  );
        poses.at(5) = Vector2D(-33,2    );
        poses.at(6) = Vector2D(-36,-12  );
        poses.at(7) = Vector2D(-34,-7   );
        poses.at(8) = Vector2D(-18,-21  );
        poses.at(9) = Vector2D(-24,5    );
        poses.at(10)= Vector2D(-10,-29  );
        poses.at(11)= Vector2D(-15,18   );
    }
    if(count == 19)
    {
        poses.at(1) = Vector2D(-47,-2   );
        poses.at(2) = Vector2D(-36,-16  );
        poses.at(3) = Vector2D(-34,-2   );
        poses.at(4) = Vector2D(-37,-21  );
        poses.at(5) = Vector2D(-33,2    );
        poses.at(6) = Vector2D(-35,-11  );
        poses.at(7) = Vector2D(-35,-7   );
        poses.at(8) = Vector2D(-18,-21  );
        poses.at(9) = Vector2D(-24,5    );
        poses.at(10)= Vector2D(-10,-29  );
        poses.at(11)= Vector2D(-15,18   );
    }
    if(count == 20)
    {
        poses.at(1) = Vector2D(-49,-2   );
        poses.at(2) = Vector2D(-37,-16  );
        poses.at(3) = Vector2D(-36,-2   );
        poses.at(4) = Vector2D(-38,-20  );
        poses.at(5) = Vector2D(-37,3    );
        poses.at(6) = Vector2D(-37,-11  );
        poses.at(7) = Vector2D(-36,-7   );
        poses.at(8) = Vector2D(-21,-19  );
        poses.at(9) = Vector2D(-24,5    );
        poses.at(10)= Vector2D(-10,-29  );
        poses.at(11)= Vector2D(-15,18   );
    }
    if(count == 21)
    {
        poses.at(1) = Vector2D(-49,-1   );
        poses.at(2) = Vector2D(-36,-9   );
        poses.at(3) = Vector2D(-38,0    );
        poses.at(4) = Vector2D(-39,-14  );
        poses.at(5) = Vector2D(-37,5    );
        poses.at(6) = Vector2D(-38,-6   );
        poses.at(7) = Vector2D(-38,-3   );
        poses.at(8) = Vector2D(-21,-19  );
        poses.at(9) = Vector2D(-24,5    );
        poses.at(10)= Vector2D(-10,-29  );
        poses.at(11)= Vector2D(-15,17   );
    }
    if(count == 22)
    {
        poses.at(1) = Vector2D(-49,0    );
        poses.at(2) = Vector2D(-38,-5   );
        poses.at(3) = Vector2D(-37,6    );
        poses.at(4) = Vector2D(-38,-11  );
        poses.at(5) = Vector2D(-37,12   );
        poses.at(6) = Vector2D(-38,-1   );
        poses.at(7) = Vector2D(-38,1    );
        poses.at(8) = Vector2D(-27,-18  );
        poses.at(9) = Vector2D(-27,18   );
        poses.at(10)= Vector2D(-13,-22  );
        poses.at(11)= Vector2D(-13,22   );
    }
    if(count == 23)
    {
        poses.at(1) = Vector2D(-48,2    );
        poses.at(2) = Vector2D(-36,-4   );
        poses.at(3) = Vector2D(-38,7    );
        poses.at(4) = Vector2D(-36,-10  );
        poses.at(5) = Vector2D(-38,13   );
        poses.at(6) = Vector2D(-36,0    );
        poses.at(7) = Vector2D(-37,4    );
        poses.at(8) = Vector2D(-30,-18  );
        poses.at(9) = Vector2D(-27,18   );
        poses.at(10)= Vector2D(-13,-23  );
        poses.at(11)= Vector2D(-11,23   );
    }
    if(count == 24)
    {
        poses.at(1) = Vector2D(-48,3    );
        poses.at(2) = Vector2D(-35,-3   );
        poses.at(3) = Vector2D(-38,12   );
        poses.at(4) = Vector2D(-34,-9   );
        poses.at(5) = Vector2D(-38,17   );
        poses.at(6) = Vector2D(-36,2    );
        poses.at(7) = Vector2D(-37,7    );
        poses.at(8) = Vector2D(-30,-17  );
        poses.at(9) = Vector2D(-29,19   );
        poses.at(10)= Vector2D(-13,-22  );
        poses.at(11)= Vector2D(-12,23   );
    }
    if(count == 25)
    {
        poses.at(1) = Vector2D(-48,6    );
        poses.at(2) = Vector2D(-36,-2   );
        poses.at(3) = Vector2D(-38,15   );
        poses.at(4) = Vector2D(-35,-7   );
        poses.at(5) = Vector2D(-38,21   );
        poses.at(6) = Vector2D(-37,4    );
        poses.at(7) = Vector2D(-38,9    );
        poses.at(8) = Vector2D(-24,-12  );
        poses.at(9) = Vector2D(-24,13   );
        poses.at(10)= Vector2D(-13,-22  );
        poses.at(11)= Vector2D(-11,23   );
    }
    if(count == 26)
    {
        poses.at(1) = Vector2D(-47,9    );
        poses.at(2) = Vector2D(-38,3    );
        poses.at(3) = Vector2D(-38,20   );
        poses.at(4) = Vector2D(-37,-4   );
        poses.at(5) = Vector2D(-36,28   );
        poses.at(6) = Vector2D(-37,8    );
        poses.at(7) = Vector2D(-38,14   );
        poses.at(8) = Vector2D(-21,-8   );
        poses.at(9) = Vector2D(-27,17   );
        poses.at(10)= Vector2D(-13,-22  );
        poses.at(11)= Vector2D(-14,25   );
    }
    if(count == 27)
    {
        poses.at(1) = Vector2D(-46,-6   );
        poses.at(2) = Vector2D(-34,-16  );
        poses.at(3) = Vector2D(-35,0    );
        poses.at(4) = Vector2D(-30,-27  );
        poses.at(5) = Vector2D(-35,7    );
        poses.at(6) = Vector2D(-35,11   );
        poses.at(7) = Vector2D(-35,-5   );
        poses.at(8) = Vector2D(-27,-14  );
        poses.at(9) = Vector2D(-25,10   );
        poses.at(10)= Vector2D(-17,-25  );
        poses.at(11)= Vector2D(-8,12    );
    }
    if(count == 28)
    {
        poses.at(1) = Vector2D(-50,0    );
        poses.at(2) = Vector2D(-37,-16  );
        poses.at(3) = Vector2D(-36,-2   );
        poses.at(4) = Vector2D(-32,-26  );
        poses.at(5) = Vector2D(-37,2    );
        poses.at(6) = Vector2D(-36,-11  );
        poses.at(7) = Vector2D(-36,-7   );
        poses.at(8) = Vector2D(-16,-20  );
        poses.at(9) = Vector2D(-23,-9   );
        poses.at(10)= Vector2D(-6,-27   );
        poses.at(11)= Vector2D(-8.0,0   );
    }
    if(count == 29)
    {
        poses.at(1) = Vector2D(-48,0    );
        poses.at(2) = Vector2D(-37,-16  );
        poses.at(3) = Vector2D(-36,-1   );
        poses.at(4) = Vector2D(-33,-24  );
        poses.at(5) = Vector2D(-37,2    );
        poses.at(6) = Vector2D(-36,-11  );
        poses.at(7) = Vector2D(-36,-6   );
        poses.at(8) = Vector2D(-18,-17  );
        poses.at(9) = Vector2D(-27,-7   );
        poses.at(10)= Vector2D(-13,-29  );
        poses.at(11)= Vector2D(-8,5.0   );
    }
    if(count == 30)
    {
        poses.at(1) = Vector2D(-45,-1   );
        poses.at(2) = Vector2D(-33,-9   );
        poses.at(3) = Vector2D(-32,4    );
        poses.at(4) = Vector2D(-33,-15  );
        poses.at(5) = Vector2D(-31,10   );
        poses.at(6) = Vector2D(-32,-4   );
        poses.at(7) = Vector2D(-32,0    );
        poses.at(8) = Vector2D(-17,-10  );
        poses.at(9) = Vector2D(-16,3    );
        poses.at(10)= Vector2D(-3,-24   );
        poses.at(11)= Vector2D(-8,20    );
    }
    if(count == 31)
    {
        poses.at(1) = Vector2D(-46,0    );
        poses.at(2) = Vector2D(-33,-6   );
        poses.at(3) = Vector2D(-32,6    );
        poses.at(4) = Vector2D(-33,-14  );
        poses.at(5) = Vector2D(-32,13   );
        poses.at(6) = Vector2D(-32,-2   );
        poses.at(7) = Vector2D(-33,2    );
        poses.at(8) = Vector2D(-19,-10  );
        poses.at(9) = Vector2D(-19,6    );
        poses.at(10)= Vector2D(-3,-25   );
        poses.at(11)= Vector2D(-7,20    );
    }
    if(count == 32)
    {
        poses.at(1) = Vector2D(-46,1    );
        poses.at(2) = Vector2D(-33,-4   );
        poses.at(3) = Vector2D(-33,12   );
        poses.at(4) = Vector2D(-33,-11  );
        poses.at(5) = Vector2D(-33,18   );
        poses.at(6) = Vector2D(-32,1    );
        poses.at(7) = Vector2D(-33,6    );
        poses.at(8) = Vector2D(-19,-5   );
        poses.at(9) = Vector2D(-19,9    );
        poses.at(10)= Vector2D(-3,-24   );
        poses.at(11)= Vector2D(-8,20    );
    }
    if(count == 33)
    {
        poses.at(1) = Vector2D(-46,4    );
        poses.at(2) = Vector2D(-34,0    );
        poses.at(3) = Vector2D(-32,17   );
        poses.at(4) = Vector2D(-34,-8   );
        poses.at(5) = Vector2D(-32,22   );
        poses.at(6) = Vector2D(-33,5    );
        poses.at(7) = Vector2D(-33,11   );
        poses.at(8) = Vector2D(-22,-5   );
        poses.at(9) = Vector2D(-19,9    );
        poses.at(10)= Vector2D(-5,22    );
        poses.at(11)= Vector2D(-7,26    );
    }
    if(count == 34)
    {
        poses.at(1) = Vector2D(-45,7    );
        poses.at(2) = Vector2D(-32,3    );
        poses.at(3) = Vector2D(-32,22   );
        poses.at(4) = Vector2D(-32,-2   );
        poses.at(5) = Vector2D(-32,27   );
        poses.at(6) = Vector2D(-32,10   );
        poses.at(7) = Vector2D(-32,16   );
        poses.at(8) = Vector2D(-22,-5   );
        poses.at(9) = Vector2D(-21,14   );
        poses.at(10)= Vector2D(-5,-22   );
        poses.at(11)= Vector2D(-14,25   );
    }
    if(count == 35)
    {
        poses.at(1) = Vector2D(-44,9    );
        poses.at(2) = Vector2D(-32,6    );
        poses.at(3) = Vector2D(-32,24   );
        poses.at(4) = Vector2D(-32,2    );
        poses.at(5) = Vector2D(-31,29   );
        poses.at(6) = Vector2D(-31,11   );
        poses.at(7) = Vector2D(-32,17   );
        poses.at(8) = Vector2D(-22,-5   );
        poses.at(9) = Vector2D(-24,17   );
        poses.at(10)= Vector2D(-5,-22   );
        poses.at(11)= Vector2D(-14,25   );
    }
    if(count == 36)
    {
        poses.at(1) = Vector2D(-41,-9   );
        poses.at(2) = Vector2D(-27,-19  );
        poses.at(3) = Vector2D(-26,-1   );
        poses.at(4) = Vector2D(-24,-27  );
        poses.at(5) = Vector2D(-25,11   );
        poses.at(6) = Vector2D(-24,-13  );
        poses.at(7) = Vector2D(-24,-7   );
        poses.at(8) = Vector2D(-17,-19  );
        poses.at(9) = Vector2D(-19,4    );
        poses.at(10)= Vector2D(-10,-27  );
        poses.at(11)= Vector2D(-14,13   );
    }
    if(count == 37)
    {
        poses.at(1) = Vector2D(-42,-7   );
        poses.at(2) = Vector2D(-25,-21  );
        poses.at(3) = Vector2D(-25,-1   );
        poses.at(4) = Vector2D(-24,-27  );
        poses.at(5) = Vector2D(-25,9    );
        poses.at(6) = Vector2D(-24,-15  );
        poses.at(7) = Vector2D(-24,-9   );
        poses.at(8) = Vector2D(-16,-18  );
        poses.at(9) = Vector2D(-19,3    );
        poses.at(10)= Vector2D(-10,-27  );
        poses.at(11)= Vector2D(-14,13   );
    }
    if(count == 38)
    {
        poses.at(1) = Vector2D(-43,-5   );
        poses.at(2) = Vector2D(-25,-19  );
        poses.at(3) = Vector2D(-25,1    );
        poses.at(4) = Vector2D(-25,-25  );
        poses.at(5) = Vector2D(-25,12   );
        poses.at(6) = Vector2D(-22,-13  );
        poses.at(7) = Vector2D(-25,-6   );
        poses.at(8) = Vector2D(-18,-21  );
        poses.at(9) = Vector2D(-18,6    );
        poses.at(10)= Vector2D(-11,-30  );
        poses.at(11)= Vector2D(-14,16   );
    }
    if(count == 39)
    {
        poses.at(1) = Vector2D(-42,-4   );
        poses.at(2) = Vector2D(-25,-17  );
        poses.at(3) = Vector2D(-24,2    );
        poses.at(4) = Vector2D(-25,-26  );
        poses.at(5) = Vector2D(-24,11   );
        poses.at(6) = Vector2D(-25,-9   );
        poses.at(7) = Vector2D(-24,-4   );
        poses.at(8) = Vector2D(-18,-21  );
        poses.at(9) = Vector2D(-18,6    );
        poses.at(10)= Vector2D(-14,-31  );
        poses.at(11)= Vector2D(-14,16   );
    }
    if(count == 40)
    {
        poses.at(1) = Vector2D(-40,0    );
        poses.at(2) = Vector2D(-24,-9   );
        poses.at(3) = Vector2D(-25,7    );
        poses.at(4) = Vector2D(-23,-18  );
        poses.at(5) = Vector2D(-23,18   );
        poses.at(6) = Vector2D(-24,-2   );
        poses.at(7) = Vector2D(-24,2    );
        poses.at(8) = Vector2D(-16,-14  );
        poses.at(9) = Vector2D(-16,14   );
        poses.at(10)= Vector2D(-14,-26  );
        poses.at(11)= Vector2D(-14,26   );
    }
    if(count == 41)
    {
        poses.at(1) = Vector2D(-42,3    );
        poses.at(2) = Vector2D(-24,-4   );
        poses.at(3) = Vector2D(-24,13   );
        poses.at(4) = Vector2D(-24,-11  );
        poses.at(5) = Vector2D(-23,20   );
        poses.at(6) = Vector2D(-24,1    );
        poses.at(7) = Vector2D(-24,6    );
        poses.at(8) = Vector2D(-14,-9   );
        poses.at(9) = Vector2D(-16,15   );
        poses.at(10)= Vector2D(-12,-23  );
        poses.at(11)= Vector2D(-9,27    );
    }
    if(count == 42)
    {
        poses.at(1) = Vector2D(-41,6    );
        poses.at(2) = Vector2D(-23,0    );
        poses.at(3) = Vector2D(-23,17   );
        poses.at(4) = Vector2D(-23,-8   );
        poses.at(5) = Vector2D(-23,23   );
        poses.at(6) = Vector2D(-24,5    );
        poses.at(7) = Vector2D(-24,10   );
        poses.at(8) = Vector2D(-14,-5   );
        poses.at(9) = Vector2D(-18,25   );
        poses.at(10)= Vector2D(-15,-17  );
        poses.at(11)= Vector2D(-12,30   );
    }
    if(count == 43)
    {
        poses.at(1) = Vector2D(-39,10   );
        poses.at(2) = Vector2D(-23,2    );
        poses.at(3) = Vector2D(-23,24   );
        poses.at(4) = Vector2D(-23,-6   );
        poses.at(5) = Vector2D(-23,30   );
        poses.at(6) = Vector2D(-23,10   );
        poses.at(7) = Vector2D(-23,17   );
        poses.at(8) = Vector2D(-14,0    );
        poses.at(9) = Vector2D(-14,20   );
        poses.at(10)= Vector2D(-15,-17  );
        poses.at(11)= Vector2D(-12,30   );
    }
    if(count == 44)
    {
        poses.at(1) = Vector2D(-38,11   );
        poses.at(2) = Vector2D(-24,4    );
        poses.at(3) = Vector2D(-23,26   );
        poses.at(4) = Vector2D(-23,-4   );
        poses.at(5) = Vector2D(-23,32   );
        poses.at(6) = Vector2D(-24,12   );
        poses.at(7) = Vector2D(-24,19   );
        poses.at(8) = Vector2D(-14,0    );
        poses.at(9) = Vector2D(-16,22   );
        poses.at(10)= Vector2D(-15,-17  );
        poses.at(11)= Vector2D(-10,28   );
    }
    
    if(count == 45)
    {
        poses.at(1) = Vector2D(-34,-15  );
        poses.at(2) = Vector2D(-16,-26  );
        poses.at(3) = Vector2D(-15,-6   );
        poses.at(4) = Vector2D(-16,-32  );
        poses.at(5) = Vector2D(-15,1    );
        poses.at(6) = Vector2D(-16,-20  );
        poses.at(7) = Vector2D(-15,-13  );
        poses.at(8) = Vector2D(-7,-22   );
        poses.at(9) = Vector2D(-7,-1    );
        poses.at(10)= Vector2D(-1,-27   );
        poses.at(11)= Vector2D(-6,7     );
    }
    
    if(count == 46)
    {
        poses.at(1) = Vector2D(-35,-13  );
        poses.at(2) = Vector2D(-15,-23  );
        poses.at(3) = Vector2D(-15,-1   );
        poses.at(4) = Vector2D(-15,-29  );
        poses.at(5) = Vector2D(-15,6    );
        poses.at(6) = Vector2D(-15,-15  );
        poses.at(7) = Vector2D(-15,-8   );
        poses.at(8) = Vector2D(-6,-17   );
        poses.at(9) = Vector2D(-7,0     );
        poses.at(10)= Vector2D(-1,-25   );
        poses.at(11)= Vector2D(-4,12    );
    }
    
    if(count == 47)
    {
        poses.at(1) = Vector2D(-34,-8   );
        poses.at(2) = Vector2D(-15,-18  );
        poses.at(3) = Vector2D(-14,1    );
        poses.at(4) = Vector2D(-15,-25  );
        poses.at(5) = Vector2D(-14,8    );
        poses.at(6) = Vector2D(-14,-11  );
        poses.at(7) = Vector2D(-14,-5   );
        poses.at(8) = Vector2D(-1,-14   );
        poses.at(9) = Vector2D(-7,1     );
        poses.at(10)= Vector2D(-2,-31   );
        poses.at(11)= Vector2D(-2,12    );
    }
    
    if(count == 48)
    {
        poses.at(1) = Vector2D(-33,-4   );
        poses.at(2) = Vector2D(-14,-13  );
        poses.at(3) = Vector2D(-13,7    );
        poses.at(4) = Vector2D(-14,-20  );
        poses.at(5) = Vector2D(-13,15   );
        poses.at(6) = Vector2D(-14,-6   );
        poses.at(7) = Vector2D(-14,0    );
        poses.at(8) = Vector2D(-8,-17   );
        poses.at(9) = Vector2D(-6,11    );
        poses.at(10)= Vector2D(-4,-25   );
        poses.at(11)= Vector2D(-2,19    );
    }
    
    if(count == 49)
    {
        poses.at(1) = Vector2D(-33,0    );
        poses.at(2) = Vector2D(-13,-9   );
        poses.at(3) = Vector2D(-13,10   );
        poses.at(4) = Vector2D(-13,-19  );
        poses.at(5) = Vector2D(-13,19   );
        poses.at(6) = Vector2D(-13,-2   );
        poses.at(7) = Vector2D(-13,2    );
        poses.at(8) = Vector2D(-5,-13   );
        poses.at(9) = Vector2D(-5,13    );
        poses.at(10)= Vector2D(-3,-25   );
        poses.at(11)= Vector2D(-3,25    );
    }
    if(count == 50)
    {
        poses.at(1) = Vector2D(-30,4    );
        poses.at(2) = Vector2D(-14,-2   );
        poses.at(3) = Vector2D(-13,17   );
        poses.at(4) = Vector2D(-13,-9   );
        poses.at(5) = Vector2D(-8,22    );
        poses.at(6) = Vector2D(-14,5    );
        poses.at(7) = Vector2D(-13,11   );
        poses.at(8) = Vector2D(-3,-11.5 );
        poses.at(9) = Vector2D(1,16     );
        poses.at(10)= Vector2D(0,-21    );
        poses.at(11)= Vector2D(4,26     );
    }
    if(count == 51)
    {
        poses.at(1) = Vector2D(-30,3.5  );
        poses.at(2) = Vector2D(-13,2    );
        poses.at(3) = Vector2D(-12,19   );
        poses.at(4) = Vector2D(-12,-6   );
        poses.at(5) = Vector2D(-10.5,23 );
        poses.at(6) = Vector2D(-13.5,7  );
        poses.at(7) = Vector2D(-13,14   );
        poses.at(8) = Vector2D(-1,-11   );
        poses.at(9) = Vector2D(-4,18    );
        poses.at(10)= Vector2D(-1,-21   );
        poses.at(11)= Vector2D(6,28     );
    }
    if(count == 52)
    {
        poses.at(1) = Vector2D(-28,7    );
        poses.at(2) = Vector2D(-13,1    );
        poses.at(3) = Vector2D(-12,22   );
        poses.at(4) = Vector2D(-12,-8.5 );
        poses.at(5) = Vector2D(-12,28   );
        poses.at(6) = Vector2D(-12,8    );
        poses.at(7) = Vector2D(-12,15   );
        poses.at(8) = Vector2D(-2,-11   );
        poses.at(9) = Vector2D(0,20     );
        poses.at(10)= Vector2D(0,-21    );
        poses.at(11)= Vector2D(6,28     );
    }
    if(count == 53)
    {
        poses.at(1) = Vector2D(-29,11   );
        poses.at(2) = Vector2D(-14,0    );
        poses.at(3) = Vector2D(-14,24   );
        poses.at(4) = Vector2D(-13,-8   );
        poses.at(5) = Vector2D(-14,30   );
        poses.at(6) = Vector2D(-14,10   );
        poses.at(7) = Vector2D(-13,17   );
        poses.at(8) = Vector2D(-1,-11   );
        poses.at(9) = Vector2D(3,21     );
        poses.at(10)= Vector2D(0,-21    );
        poses.at(11)= Vector2D(14,27    );
    }
    if(count == 54)
    {
        poses.at(1) = Vector2D(-25,-17  );
        poses.at(2) = Vector2D(-5,-26   );
        poses.at(3) = Vector2D(-5,-5    );
        poses.at(4) = Vector2D(-5,-31   );
        poses.at(5) = Vector2D(-5,4     );
        poses.at(6) = Vector2D(-5,-20   );
        poses.at(7) = Vector2D(-5,-13   );
        poses.at(8) = Vector2D(4,-24    );
        poses.at(9) = Vector2D(6,5      );
        poses.at(10)= Vector2D(9,-29    );
        poses.at(11)= Vector2D(10,17    );
    }
    if(count == 55)
    {
        poses.at(1) = Vector2D(-34,-11  );
        poses.at(2) = Vector2D(-5,-23   );
        poses.at(3) = Vector2D(-5,-2    );
        poses.at(4) = Vector2D(-5,-29   );
        poses.at(5) = Vector2D(-5,6     );
        poses.at(6) = Vector2D(-5,-16   );
        poses.at(7) = Vector2D(-5,-10   );
        poses.at(8) = Vector2D(4,-19    );
        poses.at(9) = Vector2D(5,0      );
        poses.at(10)= Vector2D(10,-30   );
        poses.at(11)= Vector2D(8,12     );
    }
    if(count == 56)
    {
        poses.at(1) = Vector2D(-33,-8   );
        poses.at(2) = Vector2D(-5,-17   );
        poses.at(3) = Vector2D(-5,3     );
        poses.at(4) = Vector2D(-5,-23   );
        poses.at(5) = Vector2D(-5,10    );
        poses.at(6) = Vector2D(-5,-11   );
        poses.at(7) = Vector2D(-5,-4    );
        poses.at(8) = Vector2D(4,-20    );
        poses.at(9) = Vector2D(5,0      );
        poses.at(10)= Vector2D(10,-30   );
        poses.at(11)= Vector2D(8,12     );
    }
    if(count == 57)
    {
        poses.at(1) = Vector2D(-34,-5   );
        poses.at(2) = Vector2D(-5,-12   );
        poses.at(3) = Vector2D(-5,7     );
        poses.at(4) = Vector2D(-5,-21   );
        poses.at(5) = Vector2D(-5,16    );
        poses.at(6) = Vector2D(-5,-6    );
        poses.at(7) = Vector2D(-5,0     );
        poses.at(8) = Vector2D(1,-17    );
        poses.at(9) = Vector2D(5,0      );
        poses.at(10)= Vector2D(10,-30   );
        poses.at(11)= Vector2D(8,12     );
    }
    if(count == 58)
    {
        poses.at(1) = Vector2D(-31,0    );
        poses.at(2) = Vector2D(-5,-10   );
        poses.at(3) = Vector2D(-5,10    );
        poses.at(4) = Vector2D(-5,-19   );
        poses.at(5) = Vector2D(-5,19    );
        poses.at(6) = Vector2D(-5,-2    );
        poses.at(7) = Vector2D(-5,2     );
        poses.at(8) = Vector2D(4,-15    );
        poses.at(9) = Vector2D(4,15     );
        poses.at(10)= Vector2D(8,-24    );
        poses.at(11)= Vector2D(8,24     );
    }
    if(count == 59)
    {
        poses.at(1) = Vector2D(-31,3    );
        poses.at(2) = Vector2D(-5,0     );
        poses.at(3) = Vector2D(-5, 17   );
        poses.at(4) = Vector2D(-5,-9    );
        poses.at(5) = Vector2D(-5,25    );
        poses.at(6) = Vector2D(-5,6     );
        poses.at(7) = Vector2D(-5,12    );
        poses.at(8) = Vector2D(5,-10    );
        poses.at(9) = Vector2D(5,13     );
        poses.at(10)= Vector2D(8,-24    );
        poses.at(11)= Vector2D(8,25     );
    }
    if(count == 60)
    {
        poses.at(1) = Vector2D(-31,6    );
        poses.at(2) = Vector2D(-5,2     );
        poses.at(3) = Vector2D(-5,21    );
        poses.at(4) = Vector2D(-5,-4    );
        poses.at(5) = Vector2D(-5,28    );
        poses.at(6) = Vector2D(-5,-8    ); 
        poses.at(7) = Vector2D(-5,14    );
        poses.at(8) = Vector2D(5,-10    );
        poses.at(9) = Vector2D(5,12     );
        poses.at(10)= Vector2D(8,-25    );
        poses.at(11)= Vector2D(5,24     );
    }
    if(count == 61)
    {
        poses.at(1) = Vector2D(-32,9    );
        poses.at(2) = Vector2D(-5,3     );
        poses.at(3) = Vector2D(-5,24    );
        poses.at(4) = Vector2D(-5,-4    );
        poses.at(5) = Vector2D(-5,29    );
        poses.at(6) = Vector2D(-5,10    );
        poses.at(7) = Vector2D(-5,17    );
        poses.at(8) = Vector2D(5,-10    );
        poses.at(9) = Vector2D(4,14     );
        poses.at(10)= Vector2D(8,-24    );
        poses.at(11)= Vector2D(7,26     );
    }
    if(count == 62)
    {
        poses.at(1) = Vector2D(-29,10   );
        poses.at(2) = Vector2D(-5,5     );
        poses.at(3) = Vector2D(-5,27    );
        poses.at(4) = Vector2D(-5,-3    );
        poses.at(5) = Vector2D(-5,32    );
        poses.at(6) = Vector2D(-5,13    );
        poses.at(7) = Vector2D(-5,20    );
        poses.at(8) = Vector2D(5,-10    );
        poses.at(9) = Vector2D(1,19     );
        poses.at(10)= Vector2D(8,-24    );
        poses.at(11)= Vector2D(7,28     );
    }
   /* if(count == 63)
    {
        poses.at(1) = Vector2D(-25,-16  );
        poses.at(2) = Vector2D(4,-26    );
        poses.at(3) = Vector2D(4,-6     );
        poses.at(4) = Vector2D(4,-31    );
        poses.at(5) = Vector2D(4,1      );
        poses.at(6) = Vector2D(4,-21    );
        poses.at(7) = Vector2D(4,-13    );
        poses.at(8) = Vector2D(10,-21   );
        poses.at(9) = Vector2D(8,7      );
        poses.at(10)= Vector2D(12,-28   );
        poses.at(11)= Vector2D(19,11    );
    }
    if(count == 64)
    {
        poses.at(1) = Vector2D(-20,-10  );
        poses.at(2) = Vector2D(4,-25    );
        poses.at(3) = Vector2D(4,-5     );
        poses.at(4) = Vector2D(4,-30    );
        poses.at(5) = Vector2D(4,1      );
        poses.at(6) = Vector2D(4,-19    );
        poses.at(7) = Vector2D(4,-13    );
        poses.at(8) = Vector2D(11,-18   );
        poses.at(9) = Vector2D(9,9      );
        poses.at(10)= Vector2D(12,-28   );
        poses.at(11)= Vector2D(17,13    );
    }
    if(count == 65)
    {
        poses.at(1) = Vector2D(-20,-10  );
        poses.at(2) = Vector2D(4,-15    );
        poses.at(3) = Vector2D(4,4      );
        poses.at(4) = Vector2D(4,-23    );
        poses.at(5) = Vector2D(4,12     );
        poses.at(6) = Vector2D(4,-9     );
        poses.at(7) = Vector2D(4,-4     );
        poses.at(8) = Vector2D(10,-17   );
        poses.at(9) = Vector2D(10,7     );
        poses.at(10)= Vector2D(12,-30   );
        poses.at(11)= Vector2D(16,11    );
    }
    if(count == 66)
    {
        poses.at(1) = Vector2D(-20,0    );
        poses.at(2) = Vector2D(4,-15    );
        poses.at(3) = Vector2D(4,7      );
        poses.at(4) = Vector2D(5,-23    );
        poses.at(5) = Vector2D(5,15     );
        poses.at(6) = Vector2D(4,-6     );
        poses.at(7) = Vector2D(4,-2     );
        poses.at(8) = Vector2D(11,-17   );
        poses.at(9) = Vector2D(11,13    );
        poses.at(10)= Vector2D(12,-27   );
        poses.at(11)= Vector2D(12,23    );
    }
    if(count == 67)
    {
        poses.at(1) = Vector2D(-20,0    );
        poses.at(2) = Vector2D(4,-11    );
        poses.at(3) = Vector2D(4,11     );
        poses.at(4) = Vector2D(5,-19    );
        poses.at(5) = Vector2D(5,19     );
        poses.at(6) = Vector2D(4,-2     );
        poses.at(7) = Vector2D(4,2      );
        poses.at(8) = Vector2D(11,-15   );
        poses.at(9) = Vector2D(11,15    );
        poses.at(10)= Vector2D(12,-25   );
        poses.at(11)= Vector2D(12,25    );
    }
    if(count == 68)
    {
        poses.at(1) = Vector2D(-19,5    );
        poses.at(2) = Vector2D(4,1      );
        poses.at(3) = Vector2D(4,18     );
        poses.at(4) = Vector2D(4,-7     );
        poses.at(5) = Vector2D(4,24     );
        poses.at(6) = Vector2D(4,7      );
        poses.at(7) = Vector2D(4,13     );
        poses.at(8) = Vector2D(14,-7    );
        poses.at(9) = Vector2D(14,8     );
        poses.at(10)= Vector2D(17,-22   );
        poses.at(11)= Vector2D(13,17    );
    }
    if(count == 69)
    {
        poses.at(1) = Vector2D(-19,9    );
        poses.at(2) = Vector2D(5,7      );
        poses.at(3) = Vector2D(4,26     );
        poses.at(4) = Vector2D(5,-2     );
        poses.at(5) = Vector2D(4,31     );
        poses.at(6) = Vector2D(4,15     );
        poses.at(7) = Vector2D(5,22     );
        poses.at(8) = Vector2D(10,-3    );
        poses.at(9) = Vector2D(11,19    );
        poses.at(10)= Vector2D(17,-18   );
        poses.at(11)= Vector2D(15,26    );
    }
    if(count == 70)
    {
        poses.at(1) = Vector2D(-18,9    );
        poses.at(2) = Vector2D(4,9      );
        poses.at(3) = Vector2D(4,29     );
        poses.at(4) = Vector2D(5,0      );
        poses.at(5) = Vector2D(4,33     );
        poses.at(6) = Vector2D(4,18     );
        poses.at(7) = Vector2D(4,24     );
        poses.at(8) = Vector2D(14,-3    );
        poses.at(9) = Vector2D(10,24    );
        poses.at(10)= Vector2D(17,-18   );
        poses.at(11)= Vector2D(14,28    );
    }
    if(count == 71)
    {
        poses.at(1) = Vector2D(-18,9    );
        poses.at(2) = Vector2D(4,9      );
        poses.at(3) = Vector2D(4,29     );
        poses.at(4) = Vector2D(5,0      );
        poses.at(5) = Vector2D(4,33     );
        poses.at(6) = Vector2D(4,18     );
        poses.at(7) = Vector2D(4,24     );
        poses.at(8) = Vector2D(14,-3    );
        poses.at(9) = Vector2D(10,24    );
        poses.at(10)= Vector2D(17,-18   );
        poses.at(11)= Vector2D(14,28    );
    }*/
    if(count > 62)
    {
        float plus_y;
        plus_y = wm.ball().pos().y / 5;
        poses.at(1) = Vector2D(-24,0 + plus_y );
        poses.at(2) = Vector2D(0,-11 + plus_y);
        poses.at(3) = Vector2D(0,11  + plus_y);
        poses.at(4) = Vector2D(0,-19 + plus_y);
        poses.at(5) = Vector2D(0,19  + plus_y);
        poses.at(6) = Vector2D(0,-2  + plus_y);
        poses.at(7) = Vector2D(0,2   + plus_y);
        poses.at(8) = Vector2D(7,-15 + plus_y );
        poses.at(9) = Vector2D(7,15  + plus_y );
        poses.at(10)= Vector2D(8,-25 + plus_y);
        poses.at(11)= Vector2D(8,25  + plus_y);
    }
    return poses.at(wm.self().unum());
}

bool Bhv_BasicMove::GoForeHelp(rcsc::PlayerAgent *agent)
{
    const WorldModel &wm = agent->world();
    if(wm.lastKickerSide() != wm.ourSide())
        return false;
    Vector2D ball_pos = wm.ball().pos();
    Vector2D self_pos = wm.self().pos();
    const PlayerPtrCont tm = wm.teammatesFromBall();
    if(self_pos.dist(ball_pos) > tm[1]->pos().dist(ball_pos))
        return false;
    int x = ball_pos.x;
    int y = ball_pos.y;
    std::vector<Vector2D> POSES;
    for(int xx = x - 20 ; xx <= x + 20 ; xx++)
    {
        for(int yy = y - 20 ; yy <= y + 20 ; yy++)
        {
            Vector2D check = Vector2D(xx,yy);
            if(!check.isValid() || xx > wm.offsideLineX() || check.dist(ball_pos) < 5)
                continue;
            double ang = (check - ball_pos).th().degree();
            Sector2D check_sector = Sector2D(ball_pos,1,check.dist(ball_pos) + 3,ang + 20,ang - 20);
            if(!wm.existOpponentIn(check_sector,5,true))
                POSES.push_back(check);
        }
    }
    if(POSES.empty())
        return false;
    if(POSES.size() == 1)
    {
        agent->debugClient().addLine(self_pos,POSES[0]);
        Body_GoToPoint(POSES[0],0,get_normal_dash_power(wm)).execute(agent);
        return true;
    }
    Vector2D Best = POSES[0];
    for(int i = 1 ; i < POSES.size() ; i++)
    {
        if(Best.dist(Vector2D(52,0)) > POSES[i].dist(Vector2D(52,0)))
            Best = POSES[i];
    }
    agent->debugClient().addLine(self_pos,Best);
    Body_GoToPoint(Best,0,get_normal_dash_power(wm)).execute(agent);
    return true;
}
bool Bhv_BasicMove::Scape(rcsc::PlayerAgent *agent)
{const WorldModel & wm = agent->world();
    if(wm.lastKickerSide() != wm.ourSide())
        return false;
    if(getPosition(wm, wm.self().unum()).x < ServerParam::i().pitchHalfLength()  / 2)
        return false;
    if(wm.self().unum() < 6)
        return false;
    if(wm.getDistOpponentNearestToSelf(5, true) > 5)
        return false;
    std::vector <Vector2D> Safe;
    Vector2D ballPos = wm.ball().pos();
    Vector2D selfPos = wm.self().pos();
    double CurrentDist = wm.getDistOpponentNearestToSelf(5, true);
    for(double x = selfPos.x - 5 ; x <= selfPos.x + 5 ; x++)
    {
        for(double y = selfPos.y - 5 ; y <= selfPos.y + 5 ; y++)
        {
            Vector2D Check = Vector2D(x, y);
            if(wm.getDistOpponentNearestTo(Check, 5) > CurrentDist)
                Safe.push_back(Check);
        }
    }
    if(Safe.empty())
        return false;
    Vector2D Home = getPosition(wm, wm.self().unum());
    Vector2D Scape = Safe[0];
    for(int i = 1 ; i < Safe.size() ; i++)
    {
        if(Home.dist(Safe[i]) < Home.dist(Scape))
            Scape = Safe[i];
    }
    if(Body_GoToPoint(Scape, 0, 80).execute(agent))
        return true;
    return false;
}
Vector2D Bhv_BasicMove::DeffenseFormation(rcsc::PlayerAgent *agent, int self_unum)
{
    /*

    Our deffense strategy:
    4 , 3 , 3



    *     *
       *
    *

     * *

    *
       *
    *     *



    //group one:   4 deffense TMs
    //group two:   3 middle TM
    //group three: 1 middle TM
    //group four:  2forward Tms


    our strategy with numbers:

    *4     *10
       *7
    *2

     *6 *9

    *3
       *8
    *5     *11





    The only thing that is left is Coding...
    Soroush: processing...
    When you start Coding, you will realize you are stuck in the mud, like a Dunkey... "The word from Soroush in the end of his life"
    Soroush Left His Life...
    process Complete, Press Enter to Exit...

    */

    const WorldModel &wm = agent->world();
    /*std::vector <Vector2D> KickOffPoses;
    if(wm.gameMode().type() == GameMode::KickOff_ && wm.gameMode().side() == wm.ourSide())
    {
        KickOffPoses.at(1) = Vector2D(-28,0);
        KickOffPoses.at(2) = Vector2D(-17,0);
        KickOffPoses.at(3) = Vector2D(-17,9);
        KickOffPoses.at(4) = Vector2D(-17,-13);
        KickOffPoses.at(5) = Vector2D(-17,14);
        KickOffPoses.at(6) = Vector2D(0,0);
        KickOffPoses.at(7) = Vector2D(-1,-5);
        KickOffPoses.at(8) = Vector2D(-1,24);
        KickOffPoses.at(9) = Vector2D(-1,8);
        KickOffPoses.at(10) = Vector2D(-1,-10);
        KickOffPoses.at(11) = Vector2D(-1,4);
        return KickOffPoses.at(wm.self().unum());
    }
    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() == wm.ourSide() && wm.ball().pos().x > 32 && wm.ball().pos().y > -13 && wm.ball().pos().y < 13)
    {
        std::vector <Vector2D> defens_poss(11);
        defens_poss[0] = Vector2D(-50,0);//
        defens_poss[1] = Vector2D(9,-21);//
        defens_poss[2] = Vector2D(9,0);//
        defens_poss[3] = Vector2D(0,-10);//
        defens_poss[4] = Vector2D(0,10);//
        defens_poss[5] = Vector2D(30,0);//
        defens_poss[6] = Vector2D(35,-7);//
        defens_poss[7] = Vector2D(35,7);//
        defens_poss[8] = Vector2D(40,0);//
        defens_poss[9] = Vector2D(44,-8);//
        defens_poss[10] = Vector2D(44,8);//
        int xx;

            xx = wm.self().unum();
            return defens_poss[xx-1];
    }*/

    Vector2D ball_pos = wm.ball().pos();
    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() != wm.ourSide() && wm.ball().pos().x < -35 && wm.ball().pos().y > -20 && wm.ball().pos().y < 20)
    {
        std::vector <Vector2D> defens_pos(11);
        defens_pos[0] = Vector2D(-52,0);//
        defens_pos[1] = Vector2D(-52,-2);//
        defens_pos[2] = Vector2D(-52,2);//
        defens_pos[3] = Vector2D(-52,-6);//
        defens_pos[4] = Vector2D(-52,6);//
        defens_pos[5] = Vector2D(-44,0);//
        defens_pos[6] = Vector2D(-52,-4);//
        defens_pos[7] = Vector2D(-52,4);//
        defens_pos[8] = Vector2D(-23,-0);//
        defens_pos[9] = Vector2D(-26,-9);//
        defens_pos[10] = Vector2D(-26,9);
        int x;

            x = wm.self().unum();
            return defens_pos[x-1];
    }
    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() == wm.ourSide() && wm.ball().pos().x > 32 && wm.ball().pos().y > -13 && wm.ball().pos().y < 13)
    {
        std::vector <Vector2D> defens_poss(11);
        defens_poss[0] = Vector2D(-50,0);//
        defens_poss[1] = Vector2D(9,-21);//
        defens_poss[2] = Vector2D(9,0);//
        defens_poss[3] = Vector2D(0,-10);//
        defens_poss[4] = Vector2D(0,10);//
        defens_poss[5] = Vector2D(30,0);//
        defens_poss[6] = Vector2D(35,-7);//
        defens_poss[7] = Vector2D(35,7);//
        defens_poss[8] = Vector2D(40,0);//
        defens_poss[9] = Vector2D(44,-8);//
        defens_poss[10] = Vector2D(44,8);//
        int xx;

            xx = wm.self().unum();
     
           return defens_poss[xx-1];
    }
    if(wm.ball().pos().x < -30)
        return GetHybridDeffenseFormation(agent);
    int ConstanChange = ball_pos.y / 2.1;
    double DistToTheirGoal = ball_pos.dist(Vector2D(52,ball_pos.y));
    int ConstantY_MinimizationChange = DistToTheirGoal / 15;
    int ConstantX_MaximizationChange = DistToTheirGoal / 10;
    //group one:
    if(self_unum > 1 && self_unum < 6)
    {
        int xx = ball_pos.x - 18 + ConstantX_MaximizationChange;
        if(xx > 0) {xx = 0;}
        int yy;
        if(self_unum == 2) {yy =  -9 + ConstanChange;}
        if(self_unum == 3) {yy =   9 + ConstanChange;}
        if(self_unum == 4) {yy =  -20 + ConstanChange;}
        if(self_unum == 5) {yy =   20 + ConstanChange;}
        if(self_unum == 2) {yy = yy + ConstantY_MinimizationChange;}
        if(self_unum == 3) {yy = yy - ConstantY_MinimizationChange;}
        if(self_unum == 4) {yy = yy + ConstantY_MinimizationChange;}
        if(self_unum == 5) {yy = yy - ConstantY_MinimizationChange;}
        if(self_unum == 2) {if(yy > 7){yy = 7;}}
        if(self_unum == 3) {if(yy < -7){yy = -7;}}
        if(self_unum == 4) {if(yy > -7){yy = -7;}}
        if(self_unum == 5) {if(yy < 7){yy = 7;}}
        return Vector2D(xx,yy);
    }
    //group(NotRealy) two:
    if(self_unum == 6)
    {
        int xx = ball_pos.x - 15 + ConstantX_MaximizationChange;
        if(xx > 8) xx = 8;
        int yy = 0 + ConstanChange;
        return Vector2D(xx,yy);
    }
    //group three:
    if(self_unum == 7 || self_unum == 8)
    {
        int xx = ball_pos.x - 15 + ConstantX_MaximizationChange;
        if(xx > 8) xx = 8;
        int yy;
        if(self_unum == 7) {yy = -14 + ConstanChange;}
        if(self_unum == 8) {yy =  14 + ConstanChange;}
        if(self_unum == 7) {yy = yy + ConstantY_MinimizationChange;}
        if(self_unum == 8) {yy = yy - ConstantY_MinimizationChange;}
        return Vector2D(xx,yy);
    }
    //group four
    if(self_unum == 9 || self_unum == 10 || self_unum == 11)
    {
        int xx = ball_pos.x - 10 + ConstantX_MaximizationChange;
        if(xx > 13) xx = 13;
        int yy;
        if(self_unum == 9 ) {yy =   0 + ConstanChange;}
        if(self_unum == 10) {yy = -15 + ConstanChange;}
        if(self_unum == 11) {yy =  15 + ConstanChange;}
        if(self_unum == 10) {yy =   yy - ConstantY_MinimizationChange;}
        if(self_unum == 11) {yy =   yy + ConstantY_MinimizationChange;}
        return Vector2D(xx,yy);
    }
}


Vector2D Bhv_BasicMove::GetHybridDeffenseFormation(rcsc::PlayerAgent *agent)
{
    int count = area(agent);
    std::vector<Vector2D> poses(12);

    if (count == 0)
    {

        poses.at(1) = Vector2D(-51.40, -6.42);
        poses.at(2) = Vector2D(-50.00, -11.64);
        poses.at(3) = Vector2D(-45.80, -4.90);
        poses.at(4) = Vector2D(-48.91, -23.42);
        poses.at(5) = Vector2D(-49.11, 4.77);
        poses.at(6) = Vector2D(-40.71, -6.93);
        poses.at(7) = Vector2D(-43.38, -13.68);
        poses.at(8) = Vector2D(-42.49, 1.84);
        poses.at(9) = Vector2D(-30.24, -4.50);
        poses.at(10) = Vector2D(-41.35, -21.18);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 1)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-51.48, -14.29);
        poses.at(3) = Vector2D(-47.75, -10.68);
        poses.at(4) = Vector2D(-48.26, -19.69);
        poses.at(5) = Vector2D(-49.29, 3.47);
        poses.at(6) = Vector2D(-41.70, -10.30);
        poses.at(7) = Vector2D(-44.14, -15.19);
        poses.at(8) = Vector2D(-43.76, -2.70);
        poses.at(9) = Vector2D(-34.23, -3.60);
        poses.at(10) = Vector2D(-36.94, -15.83);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 2)
    {

        poses.at(1) = Vector2D(-50.89, -5.66);
        poses.at(2) = Vector2D(-50.13, -10.75);
        poses.at(3) = Vector2D(-47.07, -3.24);
        poses.at(4) = Vector2D(-48.26, -19.69);
        poses.at(5) = Vector2D(-49.29, 3.47);
        poses.at(6) = Vector2D(-45.04, -7.95);
        poses.at(7) = Vector2D(-45.42, -11.90);
        poses.at(8) = Vector2D(-43.77, 0.32);
        poses.at(9) = Vector2D(-34.23, -3.60);
        poses.at(10) = Vector2D(-39.95, -10.11);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 3)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-49.94, -10.94);
        poses.at(3) = Vector2D(-50.45, -3.22);
        poses.at(4) = Vector2D(-48.26, -14.54);
        poses.at(5) = Vector2D(-49.81, 2.83);
        poses.at(6) = Vector2D(-44.53, -5.53);
        poses.at(7) = Vector2D(-46.33, -10.68);
        poses.at(8) = Vector2D(-43.76, 2.32);
        poses.at(9) = Vector2D(-39.64, -5.02);
        poses.at(10) = Vector2D(-44.14, -14.16);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 4)
    {

        poses.at(1) = Vector2D(-51.09, -6.56);
        poses.at(2) = Vector2D(-48.09, -7.44);
        poses.at(3) = Vector2D(-50.45, -3.22);
        poses.at(4) = Vector2D(-50.84, -10.04);
        poses.at(5) = Vector2D(-50.45, 1.29);
        poses.at(6) = Vector2D(-46.31, -1.84);
        poses.at(7) = Vector2D(-47.62, -11.45);
        poses.at(8) = Vector2D(-45.04, 3.24);
        poses.at(9) = Vector2D(-41.48, -3.75);
        poses.at(10) = Vector2D(-43.76, -9.65);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 5)
    {

        poses.at(1) = Vector2D(-51.09, -6.56);
        poses.at(2) = Vector2D(-48.78, -5.79);
        poses.at(3) = Vector2D(-50.45, -3.22);
        poses.at(4) = Vector2D(-51.22, -7.72);
        poses.at(5) = Vector2D(-50.45, 1.29);
        poses.at(6) = Vector2D(-46.46, -6.56);
        poses.at(7) = Vector2D(-49.42, -7.98);
        poses.at(8) = Vector2D(-46.85, 4.63);
        poses.at(9) = Vector2D(-43.13, -2.48);
        poses.at(10) = Vector2D(-46.59, -9.52);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 6)
    {

        poses.at(1) = Vector2D(-51.09, -6.56);
        poses.at(2) = Vector2D(-48.78, -5.79);
        poses.at(3) = Vector2D(-51.35, -3.60);
        poses.at(4) = Vector2D(-51.22, -7.72);
        poses.at(5) = Vector2D(-50.71, 3.22);
        poses.at(6) = Vector2D(-48.01, -5.79);
        poses.at(7) = Vector2D(-49.81, -5.66);
        poses.at(8) = Vector2D(-48.73, -0.32);
        poses.at(9) = Vector2D(-44.66, -4.39);
        poses.at(10) = Vector2D(-46.59, -9.52);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 7)
    {

        poses.at(1) = Vector2D(-51.48, -3.35);
        poses.at(2) = Vector2D(-51.48, -3.35);
        poses.at(3) = Vector2D(-51.35, -4.50);
        poses.at(4) = Vector2D(-51.48, -6.05);
        poses.at(5) = Vector2D(-50.84, 4.76);
        poses.at(6) = Vector2D(-51.35, -2.96);
        poses.at(7) = Vector2D(-51.02, -1.59);
        poses.at(8) = Vector2D(-49.24, 2.35);
        poses.at(9) = Vector2D(-50.13, 0.32);
        poses.at(10) = Vector2D(-45.69, -6.18);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 8)
    {

        poses.at(1) = Vector2D(-51.74, -3.09);
        poses.at(2) = Vector2D(-51.48, -3.35);
        poses.at(3) = Vector2D(-52.25, -4.63);
        poses.at(4) = Vector2D(-52.38, -4.50);
        poses.at(5) = Vector2D(-51.61, 3.09);
        poses.at(6) = Vector2D(-51.35, -2.96);
        poses.at(7) = Vector2D(-52.25, -1.54);
        poses.at(8) = Vector2D(-50.84, 2.32);
        poses.at(9) = Vector2D(-51.35, 0.13);
        poses.at(10) = Vector2D(-46.07, -1.54);
        poses.at(11) = Vector2D(-28.96, 15.19);
    }

    if (count == 9)
    {

        poses.at(1) = Vector2D(-52.25, 1.29);
        poses.at(2) = Vector2D(-52.38, -1.29);
        poses.at(3) = Vector2D(-51.74, -2.45);
        poses.at(4) = Vector2D(-52.38, -4.50);
        poses.at(5) = Vector2D(-51.61, 5.02);
        poses.at(6) = Vector2D(-51.99, 1.80);
        poses.at(7) = Vector2D(-51.48, -2.45);
        poses.at(8) = Vector2D(-51.99, 3.47);
        poses.at(9) = Vector2D(-51.87, -0.64);
        poses.at(10) = Vector2D(-46.07, -1.54);
        poses.at(11) = Vector2D(-38.61, 10.30);
    }

    if (count == 10)
    {

        poses.at(1) = Vector2D(-52.12, 4.25);
        poses.at(2) = Vector2D(-50.45, -1.16);
        poses.at(3) = Vector2D(-51.35, 2.57);
        poses.at(4) = Vector2D(-50.84, -6.31);
        poses.at(5) = Vector2D(-51.74, 5.79);
        poses.at(6) = Vector2D(-51.09, 0.64);
        poses.at(7) = Vector2D(-51.22, -3.47);
        poses.at(8) = Vector2D(-50.71, 1.67);
        poses.at(9) = Vector2D(-50.97, 2.06);
        poses.at(10) = Vector2D(-42.60, -4.38);
        poses.at(11) = Vector2D(-46.85, 7.72);
    }

    if (count == 11)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-50.84, -1.80);
        poses.at(3) = Vector2D(-50.32, 5.15);
        poses.at(4) = Vector2D(-50.84, -6.31);
        poses.at(5) = Vector2D(-50.06, 8.62);
        poses.at(6) = Vector2D(-51.35, 1.80);
        poses.at(7) = Vector2D(-48.52, 1.54);
        poses.at(8) = Vector2D(-41.96, 5.02);
        poses.at(9) = Vector2D(-46.85, 5.53);
        poses.at(10) = Vector2D(-36.68, -7.85);
        poses.at(11) = Vector2D(-44.79, 11.84);
    }

    if (count == 12)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-48.52, 0.77);
        poses.at(3) = Vector2D(-48.98, 8.59);
        poses.at(4) = Vector2D(-48.78, -4.38);
        poses.at(5) = Vector2D(-50.51, 11.13);
        poses.at(6) = Vector2D(-45.56, 0.00);
        poses.at(7) = Vector2D(-47.88, 4.63);
        poses.at(8) = Vector2D(-44.78, 5.53);
        poses.at(9) = Vector2D(-40.03, 0.39);
        poses.at(10) = Vector2D(-32.95, -12.10);
        poses.at(11) = Vector2D(-44.79, 11.84);
    }

    if (count == 13)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-49.94, -0.26);
        poses.at(3) = Vector2D(-49.68, 7.85);
        poses.at(4) = Vector2D(-48.78, -4.38);
        poses.at(5) = Vector2D(-49.42, 14.03);
        poses.at(6) = Vector2D(-47.10, 3.60);
        poses.at(7) = Vector2D(-45.17, -0.77);
        poses.at(8) = Vector2D(-44.53, 7.98);
        poses.at(9) = Vector2D(-40.28, 5.28);
        poses.at(10) = Vector2D(-32.95, -12.10);
        poses.at(11) = Vector2D(-38.22, 18.79);
    }

    if (count == 14)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-49.55, 4.76);
        poses.at(3) = Vector2D(-49.16, 11.33);
        poses.at(4) = Vector2D(-48.78, -4.38);
        poses.at(5) = Vector2D(-49.03, 17.50);
        poses.at(6) = Vector2D(-45.43, 7.34);
        poses.at(7) = Vector2D(-43.24, 0.26);
        poses.at(8) = Vector2D(-44.79, 11.45);
        poses.at(9) = Vector2D(-40.15, 9.14);
        poses.at(10) = Vector2D(-32.95, -12.10);
        poses.at(11) = Vector2D(-39.77, 17.89);
    }

    if (count == 15)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-48.52, 6.95);
        poses.at(3) = Vector2D(-49.42, 15.06);
        poses.at(4) = Vector2D(-48.09, -3.37);
        poses.at(5) = Vector2D(-48.91, 21.75);
        poses.at(6) = Vector2D(-44.27, 8.08);
        poses.at(7) = Vector2D(-43.77, 1.59);
        poses.at(8) = Vector2D(-44.53, 12.91);
        poses.at(9) = Vector2D(-39.00, 11.58);
        poses.at(10) = Vector2D(-24.97, -17.63);
        poses.at(11) = Vector2D(-41.98, 22.58);
    }

    if (count == 16)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-47.84, 4.77);
        poses.at(3) = Vector2D(-48.22, 14.69);
        poses.at(4) = Vector2D(-48.26, -1.54);
        poses.at(5) = Vector2D(-48.78, 24.97);
        poses.at(6) = Vector2D(-43.26, 8.59);
        poses.at(7) = Vector2D(-39.82, 0.57);
        poses.at(8) = Vector2D(-44.78, 13.04);
        poses.at(9) = Vector2D(-39.82, 14.19);
        poses.at(10) = Vector2D(-24.97, -17.63);
        poses.at(11) = Vector2D(-42.62, 25.89);
    }

    if (count == 17)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.82, -21.62);
        poses.at(3) = Vector2D(-44.79, -12.36);
        poses.at(4) = Vector2D(-46.33, -27.67);
        poses.at(5) = Vector2D(-44.66, -1.03);
        poses.at(6) = Vector2D(-43.76, -16.73);
        poses.at(7) = Vector2D(-40.03, -22.27);
        poses.at(8) = Vector2D(-41.18, -7.72);
        poses.at(9) = Vector2D(-38.35, -14.03);
        poses.at(10) = Vector2D(-32.05, -20.85);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 18)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.17, -19.05);
        poses.at(3) = Vector2D(-45.05, -11.20);
        poses.at(4) = Vector2D(-46.33, -23.94);
        poses.at(5) = Vector2D(-44.02, -0.77);
        poses.at(6) = Vector2D(-43.24, -16.09);
        poses.at(7) = Vector2D(-42.21, -19.95);
        poses.at(8) = Vector2D(-41.70, -7.08);
        poses.at(9) = Vector2D(-39.77, -13.00);
        poses.at(10) = Vector2D(-32.95, -19.31);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 19)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.69, -14.80);
        poses.at(3) = Vector2D(-45.43, -7.85);
        poses.at(4) = Vector2D(-46.85, -17.50);
        poses.at(5) = Vector2D(-44.02, -0.77);
        poses.at(6) = Vector2D(-43.63, -13.38);
        poses.at(7) = Vector2D(-41.06, -12.36);
        poses.at(8) = Vector2D(-42.86, -4.63);
        poses.at(9) = Vector2D(-40.54, -8.75);
        poses.at(10) = Vector2D(-32.95, -19.31);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 20)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.17, -13.51);
        poses.at(3) = Vector2D(-43.50, -5.79);
        poses.at(4) = Vector2D(-46.07, -15.19);
        poses.at(5) = Vector2D(-43.37, 3.60);
        poses.at(6) = Vector2D(-43.63, -9.52);
        poses.at(7) = Vector2D(-43.37, -11.58);
        poses.at(8) = Vector2D(-43.24, -1.42);
        poses.at(9) = Vector2D(-39.51, -9.27);
        poses.at(10) = Vector2D(-35.52, -16.34);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 21)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.56, -7.21);
        poses.at(3) = Vector2D(-43.50, -5.79);
        poses.at(4) = Vector2D(-45.69, -11.20);
        poses.at(5) = Vector2D(-44.14, 2.83);
        poses.at(6) = Vector2D(-45.56, -4.76);
        poses.at(7) = Vector2D(-44.92, -9.14);
        poses.at(8) = Vector2D(-44.79, -2.32);
        poses.at(9) = Vector2D(-41.06, -6.56);
        poses.at(10) = Vector2D(-37.07, -13.90);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 22)
    {

        poses.at(1) = Vector2D(-50.71, -3.60);
        poses.at(2) = Vector2D(-45.95, -6.95);
        poses.at(3) = Vector2D(-45.05, -2.32);
        poses.at(4) = Vector2D(-46.33, -7.85);
        poses.at(5) = Vector2D(-46.46, 4.63);
        poses.at(6) = Vector2D(-45.05, -3.73);
        poses.at(7) = Vector2D(-45.56, -5.41);
        poses.at(8) = Vector2D(-45.43, 0.77);
        poses.at(9) = Vector2D(-42.34, 0.00);
        poses.at(10) = Vector2D(-37.07, -13.90);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 23)
    {

        poses.at(1) = Vector2D(-50.71, -1.03);
        poses.at(2) = Vector2D(-45.30, -3.99);
        poses.at(3) = Vector2D(-45.43, -0.51);
        poses.at(4) = Vector2D(-45.17, -7.34);
        poses.at(5) = Vector2D(-45.56, 3.47);
        poses.at(6) = Vector2D(-45.30, -2.32);
        poses.at(7) = Vector2D(-45.30, -5.92);
        poses.at(8) = Vector2D(-45.30, 1.54);
        poses.at(9) = Vector2D(-44.02, -2.45);
        poses.at(10) = Vector2D(-37.07, -13.90);
        poses.at(11) = Vector2D(-29.60, 5.28);
    }

    if (count == 24)
    {

        poses.at(1) = Vector2D(-50.71, -1.03);
        poses.at(2) = Vector2D(-45.17, -0.39);
        poses.at(3) = Vector2D(-46.07, 2.70);
        poses.at(4) = Vector2D(-44.66, -5.02);
        poses.at(5) = Vector2D(-46.07, 6.69);
        poses.at(6) = Vector2D(-45.43, 0.90);
        poses.at(7) = Vector2D(-45.05, -2.19);
        poses.at(8) = Vector2D(-46.33, 4.12);
        poses.at(9) = Vector2D(-42.99, -1.54);
        poses.at(10) = Vector2D(-30.24, -14.16);
        poses.at(11) = Vector2D(-36.94, 7.21);
    }

    if (count == 25)
    {

        poses.at(1) = Vector2D(-50.71, -1.03);
        poses.at(2) = Vector2D(-43.76, 1.67);
        poses.at(3) = Vector2D(-45.30, 3.73);
        poses.at(4) = Vector2D(-41.70, -7.34);
        poses.at(5) = Vector2D(-45.56, 7.08);
        poses.at(6) = Vector2D(-44.40, 3.73);
        poses.at(7) = Vector2D(-43.11, -2.32);
        poses.at(8) = Vector2D(-45.56, 5.53);
        poses.at(9) = Vector2D(-42.34, 1.42);
        poses.at(10) = Vector2D(-30.24, -14.16);
        poses.at(11) = Vector2D(-38.87, 6.95);
    }

    if (count == 26)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-42.73, 5.79);
        poses.at(3) = Vector2D(-44.27, 7.21);
        poses.at(4) = Vector2D(-42.60, -4.12);
        poses.at(5) = Vector2D(-46.20, 10.30);
        poses.at(6) = Vector2D(-43.24, 6.82);
        poses.at(7) = Vector2D(-43.63, 2.32);
        poses.at(8) = Vector2D(-45.30, 8.49);
        poses.at(9) = Vector2D(-37.84, 1.03);
        poses.at(10) = Vector2D(-30.24, -14.16);
        poses.at(11) = Vector2D(-40.03, 6.44);
    }

    if (count == 27)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-43.24, 5.02);
        poses.at(3) = Vector2D(-43.63, 10.04);
        poses.at(4) = Vector2D(-42.60, -4.12);
        poses.at(5) = Vector2D(-44.79, 13.90);
        poses.at(6) = Vector2D(-42.47, 8.62);
        poses.at(7) = Vector2D(-39.64, 1.54);
        poses.at(8) = Vector2D(-44.40, 11.71);
        poses.at(9) = Vector2D(-35.52, 4.25);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-39.00, 8.88);
    }

    if (count == 28)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-43.24, 5.02);
        poses.at(3) = Vector2D(-42.60, 11.71);
        poses.at(4) = Vector2D(-42.60, -4.12);
        poses.at(5) = Vector2D(-44.27, 16.73);
        poses.at(6) = Vector2D(-40.15, 7.59);
        poses.at(7) = Vector2D(-39.64, 1.54);
        poses.at(8) = Vector2D(-42.60, 15.19);
        poses.at(9) = Vector2D(-35.52, 4.25);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-36.42, 14.16);
    }

    if (count == 29)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-44.02, 7.21);
        poses.at(3) = Vector2D(-43.37, 14.80);
        poses.at(4) = Vector2D(-42.60, -4.12);
        poses.at(5) = Vector2D(-45.56, 24.07);
        poses.at(6) = Vector2D(-40.93, 11.20);
        poses.at(7) = Vector2D(-39.90, 3.35);
        poses.at(8) = Vector2D(-43.37, 20.33);
        poses.at(9) = Vector2D(-35.78, 12.48);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-32.69, 18.66);
    }

    if (count == 30)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-44.27, 9.27);
        poses.at(3) = Vector2D(-44.14, 18.28);
        poses.at(4) = Vector2D(-44.14, -2.32);
        poses.at(5) = Vector2D(-45.30, 27.03);
        poses.at(6) = Vector2D(-40.28, 14.29);
        poses.at(7) = Vector2D(-39.90, 3.35);
        poses.at(8) = Vector2D(-42.60, 23.29);
        poses.at(9) = Vector2D(-32.43, 14.67);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-32.30, 23.94);
    }

    if (count == 31)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-44.02, 11.33);
        poses.at(3) = Vector2D(-44.53, 20.33);
        poses.at(4) = Vector2D(-44.14, -2.32);
        poses.at(5) = Vector2D(-45.05, 29.34);
        poses.at(6) = Vector2D(-40.93, 17.63);
        poses.at(7) = Vector2D(-38.61, 7.34);
        poses.at(8) = Vector2D(-42.08, 25.48);
        poses.at(9) = Vector2D(-35.14, 16.09);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-33.20, 26.25);
    }

    if (count == 32)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-39.25, -20.08);
        poses.at(3) = Vector2D(-39.25, -10.68);
        poses.at(4) = Vector2D(-39.12, -30.24);
        poses.at(5) = Vector2D(-39.38, 2.57);
        poses.at(6) = Vector2D(-35.78, -15.32);
        poses.at(7) = Vector2D(-36.55, -25.87);
        poses.at(8) = Vector2D(-35.14, -5.02);
        poses.at(9) = Vector2D(-29.86, -12.10);
        poses.at(10) = Vector2D(-31.53, -24.32);
        poses.at(11) = Vector2D(-23.68, 6.31);
    }

    if (count == 33)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-39.00, -19.18);
        poses.at(3) = Vector2D(-39.25, -10.68);
        poses.at(4) = Vector2D(-38.61, -27.41);
        poses.at(5) = Vector2D(-39.38, 2.57);
        poses.at(6) = Vector2D(-35.26, -16.60);
        poses.at(7) = Vector2D(-36.68, -22.52);
        poses.at(8) = Vector2D(-36.42, -5.53);
        poses.at(9) = Vector2D(-29.86, -12.10);
        poses.at(10) = Vector2D(-31.79, -22.39);
        poses.at(11) = Vector2D(-23.68, 6.31);
    }

    if (count == 34)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-39.25, -15.32);
        poses.at(3) = Vector2D(-39.64, -8.62);
        poses.at(4) = Vector2D(-39.38, -23.29);
        poses.at(5) = Vector2D(-38.87, 3.47);
        poses.at(6) = Vector2D(-36.81, -12.10);
        poses.at(7) = Vector2D(-37.58, -19.69);
        poses.at(8) = Vector2D(-36.04, -3.47);
        poses.at(9) = Vector2D(-31.66, -11.84);
        poses.at(10) = Vector2D(-30.12, -17.12);
        poses.at(11) = Vector2D(-23.68, 6.31);
    }

    if (count == 35)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-39.77, -11.97);
        poses.at(3) = Vector2D(-39.25, -4.76);
        poses.at(4) = Vector2D(-39.90, -18.79);
        poses.at(5) = Vector2D(-38.87, 3.47);
        poses.at(6) = Vector2D(-37.84, -8.62);
        poses.at(7) = Vector2D(-36.55, -14.80);
        poses.at(8) = Vector2D(-36.04, -3.47);
        poses.at(9) = Vector2D(-32.30, -9.65);
        poses.at(10) = Vector2D(-29.99, -14.80);
        poses.at(11) = Vector2D(-23.68, 6.31);
    }

    if (count == 36)
    {

        poses.at(1) = Vector2D(-49.03, -5.66);
        poses.at(2) = Vector2D(-39.77, -10.42);
        poses.at(3) = Vector2D(-39.64, -5.02);
        poses.at(4) = Vector2D(-39.77, -16.60);
        poses.at(5) = Vector2D(-38.87, 3.47);
        poses.at(6) = Vector2D(-37.71, -7.46);
        poses.at(7) = Vector2D(-37.19, -11.20);
        poses.at(8) = Vector2D(-37.45, -1.67);
        poses.at(9) = Vector2D(-35.01, -6.95);
        poses.at(10) = Vector2D(-26.00, -18.53);
        poses.at(11) = Vector2D(-26.51, 4.25);
    }

    if (count == 37)
    {

        poses.at(1) = Vector2D(-49.03, -5.66);
        poses.at(2) = Vector2D(-40.03, -8.11);
        poses.at(3) = Vector2D(-39.51, -2.96);
        poses.at(4) = Vector2D(-39.64, -13.38);
        poses.at(5) = Vector2D(-39.51, 3.99);
        poses.at(6) = Vector2D(-38.22, -5.41);
        poses.at(7) = Vector2D(-38.22, -9.65);
        poses.at(8) = Vector2D(-37.32, -0.51);
        poses.at(9) = Vector2D(-36.68, -7.08);
        poses.at(10) = Vector2D(-26.00, -18.53);
        poses.at(11) = Vector2D(-28.70, 3.22);
    }

    if (count == 38)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.90, -3.73);
        poses.at(3) = Vector2D(-39.90, 0.00);
        poses.at(4) = Vector2D(-40.54, -9.78);
        poses.at(5) = Vector2D(-39.90, 6.05);
        poses.at(6) = Vector2D(-37.32, -1.29);
        poses.at(7) = Vector2D(-38.48, -6.56);
        poses.at(8) = Vector2D(-38.87, 2.96);
        poses.at(9) = Vector2D(-36.94, -2.06);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-32.56, 1.54);
    }

    if (count == 39)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.90, -2.06);
        poses.at(3) = Vector2D(-40.54, 2.19);
        poses.at(4) = Vector2D(-40.54, -7.72);
        poses.at(5) = Vector2D(-40.80, 6.56);
        poses.at(6) = Vector2D(-37.58, 1.80);
        poses.at(7) = Vector2D(-37.84, -4.12);
        poses.at(8) = Vector2D(-37.84, 4.25);
        poses.at(9) = Vector2D(-37.19, -1.03);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-32.69, 5.53);
    }

    if (count == 40)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.64, 0.26);
        poses.at(3) = Vector2D(-39.51, 5.41);
        poses.at(4) = Vector2D(-39.25, -5.79);
        poses.at(5) = Vector2D(-39.64, 11.58);
        poses.at(6) = Vector2D(-36.81, 4.63);
        poses.at(7) = Vector2D(-37.19, -1.42);
        poses.at(8) = Vector2D(-38.74, 7.59);
        poses.at(9) = Vector2D(-36.42, 1.54);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-31.79, 10.94);
    }

    if (count == 41)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.77, 2.96);
        poses.at(3) = Vector2D(-39.38, 8.88);
        poses.at(4) = Vector2D(-39.25, -5.79);
        poses.at(5) = Vector2D(-40.03, 14.93);
        poses.at(6) = Vector2D(-38.10, 7.21);
        poses.at(7) = Vector2D(-36.55, 1.93);
        poses.at(8) = Vector2D(-37.71, 11.71);
        poses.at(9) = Vector2D(-35.52, 6.44);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-31.79, 10.94);
    }

    if (count == 42)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.64, 4.25);
        poses.at(3) = Vector2D(-39.90, 11.58);
        poses.at(4) = Vector2D(-38.61, -4.12);
        poses.at(5) = Vector2D(-39.77, 15.83);
        poses.at(6) = Vector2D(-38.10, 7.98);
        poses.at(7) = Vector2D(-37.45, 0.90);
        poses.at(8) = Vector2D(-37.19, 12.87);
        poses.at(9) = Vector2D(-35.26, 9.27);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-32.05, 10.42);
    }

    if (count == 43)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-38.87, 7.85);
        poses.at(3) = Vector2D(-38.87, 13.26);
        poses.at(4) = Vector2D(-38.74, -1.80);
        poses.at(5) = Vector2D(-39.25, 22.27);
        poses.at(6) = Vector2D(-35.14, 12.10);
        poses.at(7) = Vector2D(-34.75, 4.25);
        poses.at(8) = Vector2D(-36.81, 16.60);
        poses.at(9) = Vector2D(-32.82, 9.01);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-30.76, 15.06);
    }

    if (count == 44)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-38.87, 7.85);
        poses.at(3) = Vector2D(-39.25, 17.63);
        poses.at(4) = Vector2D(-38.74, -4.38);
        poses.at(5) = Vector2D(-38.87, 25.35);
        poses.at(6) = Vector2D(-36.42, 14.41);
        poses.at(7) = Vector2D(-34.75, 4.25);
        poses.at(8) = Vector2D(-37.32, 20.59);
        poses.at(9) = Vector2D(-32.43, 15.44);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-27.80, 22.27);
    }

    if (count == 45)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-40.15, 10.30);
        poses.at(3) = Vector2D(-39.51, 20.98);
        poses.at(4) = Vector2D(-39.25, -4.50);
        poses.at(5) = Vector2D(-39.90, 29.21);
        poses.at(6) = Vector2D(-36.68, 16.09);
        poses.at(7) = Vector2D(-36.42, 5.53);
        poses.at(8) = Vector2D(-37.45, 24.71);
        poses.at(9) = Vector2D(-32.43, 15.44);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-27.54, 23.94);
    }

    if (count == 46)
    {

        poses.at(1) = Vector2D(-46.69, -7.44);
        poses.at(2) = Vector2D(-32.56, -19.18);
        poses.at(3) = Vector2D(-32.18, -9.14);
        poses.at(4) = Vector2D(-32.69, -29.73);
        poses.at(5) = Vector2D(-32.43, 3.99);
        poses.at(6) = Vector2D(-30.24, -15.19);
        poses.at(7) = Vector2D(-31.02, -25.10);
        poses.at(8) = Vector2D(-27.03, -5.41);
        poses.at(9) = Vector2D(-27.54, -23.04);
        poses.at(10) = Vector2D(-22.78, -25.23);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 47)
    {

        poses.at(1) = Vector2D(-46.82, -7.70);
        poses.at(2) = Vector2D(-32.30, -19.56);
        poses.at(3) = Vector2D(-31.79, -7.59);
        poses.at(4) = Vector2D(-32.56, -27.03);
        poses.at(5) = Vector2D(-32.43, 3.99);
        poses.at(6) = Vector2D(-29.73, -13.26);
        poses.at(7) = Vector2D(-30.50, -23.29);
        poses.at(8) = Vector2D(-27.67, -3.73);
        poses.at(9) = Vector2D(-26.64, -19.18);
        poses.at(10) = Vector2D(-22.78, -25.23);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 48)
    {

        poses.at(1) = Vector2D(-47.20, -6.17);
        poses.at(2) = Vector2D(-31.79, -14.29);
        poses.at(3) = Vector2D(-31.92, -5.15);
        poses.at(4) = Vector2D(-31.79, -22.78);
        poses.at(5) = Vector2D(-32.18, 5.15);
        poses.at(6) = Vector2D(-29.47, -10.42);
        poses.at(7) = Vector2D(-30.50, -18.66);
        poses.at(8) = Vector2D(-27.80, -1.93);
        poses.at(9) = Vector2D(-24.58, -11.58);
        poses.at(10) = Vector2D(-22.14, -22.39);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 49)
    {

        poses.at(1) = Vector2D(-47.84, -5.79);
        poses.at(2) = Vector2D(-31.79, -11.58);
        poses.at(3) = Vector2D(-32.30, -1.67);
        poses.at(4) = Vector2D(-32.18, -19.56);
        poses.at(5) = Vector2D(-32.05, 9.27);
        poses.at(6) = Vector2D(-29.47, -7.21);
        poses.at(7) = Vector2D(-29.99, -15.70);
        poses.at(8) = Vector2D(-28.31, 1.80);
        poses.at(9) = Vector2D(-26.90, -12.10);
        poses.at(10) = Vector2D(-20.98, -18.15);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 50)
    {

        poses.at(1) = Vector2D(-50.00, -4.52);
        poses.at(2) = Vector2D(-32.95, -8.37);
        poses.at(3) = Vector2D(-32.56, -1.29);
        poses.at(4) = Vector2D(-32.95, -17.12);
        poses.at(5) = Vector2D(-32.82, 7.98);
        poses.at(6) = Vector2D(-31.02, -6.44);
        poses.at(7) = Vector2D(-30.50, -11.45);
        poses.at(8) = Vector2D(-28.31, 1.80);
        poses.at(9) = Vector2D(-27.54, -5.02);
        poses.at(10) = Vector2D(-23.29, -17.89);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 51)
    {

        poses.at(1) = Vector2D(-48.22, -0.45);
        poses.at(2) = Vector2D(-34.08, -6.61);
        poses.at(3) = Vector2D(-34.01, 1.68);
        poses.at(4) = Vector2D(-34.57, -16.74);
        poses.at(5) = Vector2D(-34.24, 10.45);
        poses.at(6) = Vector2D(-32.32, -2.62);
        poses.at(7) = Vector2D(-31.19, -11.32);
        poses.at(8) = Vector2D(-30.94, 4.82);
        poses.at(9) = Vector2D(-29.92, -3.60);
        poses.at(10) = Vector2D(-25.29, -16.39);
        poses.at(11) = Vector2D(-23.71, 6.80);
    }

    if (count == 52)
    {

        poses.at(1) = Vector2D(-48.35, -0.19);
        poses.at(2) = Vector2D(-33.72, -6.18);
        poses.at(3) = Vector2D(-33.72, 2.32);
        poses.at(4) = Vector2D(-34.36, -16.99);
        poses.at(5) = Vector2D(-33.98, 11.45);
        poses.at(6) = Vector2D(-31.92, -1.93);
        poses.at(7) = Vector2D(-30.76, -11.33);
        poses.at(8) = Vector2D(-30.63, 5.66);
        poses.at(9) = Vector2D(-29.47, -3.09);
        poses.at(10) = Vector2D(-25.23, -16.22);
        poses.at(11) = Vector2D(-23.68, 7.08);
    }

    if (count == 53)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-34.22, -2.86);
        poses.at(3) = Vector2D(-34.10, 5.92);
        poses.at(4) = Vector2D(-34.61, -11.90);
        poses.at(5) = Vector2D(-33.97, 12.66);
        poses.at(6) = Vector2D(-33.21, 1.97);
        poses.at(7) = Vector2D(-29.52, -5.79);
        poses.at(8) = Vector2D(-31.04, 8.21);
        poses.at(9) = Vector2D(-30.03, 3.12);
        poses.at(10) = Vector2D(-25.23, -16.22);
        poses.at(11) = Vector2D(-24.58, 9.40);
    }

    if (count == 54)
    {

        poses.at(1) = Vector2D(-47.46, 3.50);
        poses.at(2) = Vector2D(-33.33, -3.50);
        poses.at(3) = Vector2D(-32.44, 7.70);
        poses.at(4) = Vector2D(-33.33, -15.46);
        poses.at(5) = Vector2D(-32.32, 16.35);
        poses.at(6) = Vector2D(-31.40, 1.42);
        poses.at(7) = Vector2D(-28.63, -6.42);
        poses.at(8) = Vector2D(-29.64, 12.02);
        poses.at(9) = Vector2D(-28.75, 4.90);
        poses.at(10) = Vector2D(-25.23, -16.22);
        poses.at(11) = Vector2D(-24.58, 9.40);
    }

    if (count == 55)
    {

        poses.at(1) = Vector2D(-47.96, 5.79);
        poses.at(2) = Vector2D(-33.59, -0.19);
        poses.at(3) = Vector2D(-33.08, 10.11);
        poses.at(4) = Vector2D(-34.48, -14.19);
        poses.at(5) = Vector2D(-33.21, 20.67);
        poses.at(6) = Vector2D(-30.28, 4.39);
        poses.at(7) = Vector2D(-29.90, -5.66);
        poses.at(8) = Vector2D(-29.77, 15.84);
        poses.at(9) = Vector2D(-28.75, 10.11);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-21.88, 9.91);
    }

    if (count == 56)
    {

        poses.at(1) = Vector2D(-48.85, 6.42);
        poses.at(2) = Vector2D(-32.19, 1.08);
        poses.at(3) = Vector2D(-31.81, 11.51);
        poses.at(4) = Vector2D(-32.70, -12.66);
        poses.at(5) = Vector2D(-32.06, 20.42);
        poses.at(6) = Vector2D(-29.39, 5.92);
        poses.at(7) = Vector2D(-27.86, -4.39);
        poses.at(8) = Vector2D(-29.52, 15.20);
        poses.at(9) = Vector2D(-25.57, 7.44);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-21.76, 19.15);
    }

    if (count == 57)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-33.20, 3.60);
        poses.at(3) = Vector2D(-33.08, 14.16);
        poses.at(4) = Vector2D(-33.72, -9.78);
        poses.at(5) = Vector2D(-32.82, 24.45);
        poses.at(6) = Vector2D(-29.21, 8.37);
        poses.at(7) = Vector2D(-28.31, -2.45);
        poses.at(8) = Vector2D(-30.89, 19.43);
        poses.at(9) = Vector2D(-27.28, 15.06);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-21.75, 20.46);
    }

    if (count == 58)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-33.33, 7.46);
        poses.at(3) = Vector2D(-33.46, 18.79);
        poses.at(4) = Vector2D(-32.95, -6.69);
        poses.at(5) = Vector2D(-33.20, 28.70);
        poses.at(6) = Vector2D(-30.63, 13.26);
        poses.at(7) = Vector2D(-28.19, 1.54);
        poses.at(8) = Vector2D(-29.86, 23.04);
        poses.at(9) = Vector2D(-26.64, 17.76);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-22.14, 24.84);
    }

    if (count == 59)
    {

        poses.at(1) = Vector2D(-47.49, 9.27);
        poses.at(2) = Vector2D(-34.11, 9.78);
        poses.at(3) = Vector2D(-33.33, 21.49);
        poses.at(4) = Vector2D(-32.95, -6.69);
        poses.at(5) = Vector2D(-33.46, 31.15);
        poses.at(6) = Vector2D(-29.73, 16.09);
        poses.at(7) = Vector2D(-27.67, 3.73);
        poses.at(8) = Vector2D(-29.47, 25.23);
        poses.at(9) = Vector2D(-26.13, 20.08);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-20.85, 29.47);
    }

    if (count == 60)
    {

        poses.at(1) = Vector2D(-47.49, 9.27);
        poses.at(2) = Vector2D(-33.46, 12.10);
        poses.at(3) = Vector2D(-33.08, 23.81);
        poses.at(4) = Vector2D(-33.59, -4.50);
        poses.at(5) = Vector2D(-33.20, 32.69);
        poses.at(6) = Vector2D(-29.60, 18.02);
        poses.at(7) = Vector2D(-28.57, 6.95);
        poses.at(8) = Vector2D(-29.86, 27.93);
        poses.at(9) = Vector2D(-26.77, 22.39);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-20.85, 29.47);
    }
    return poses.at(agent->world().self().unum());
}

Vector2D Bhv_BasicMove::OffensiveFormation(PlayerAgent *agent, int unum)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    if(unum == 2 || unum == 3)
    {
        int ConstanChange = ball_pos.y / 2.2;
        int xx = ball_pos.x - 10;
        if(xx > 5) xx = 5;
        int yy;
        if(unum == 2) {yy =  -5 + ConstanChange;}
        if(unum == 3) {yy =   5 + ConstanChange;}
        return Vector2D(xx, yy);
    }
    if(unum == 4 || unum == 5)
    {
        int ConstanChange = ball_pos.y / 2.9;
        int xx = ball_pos.x - 3;
        if(xx > 10) xx = 10;
        int yy;
        if(unum == 4) {yy =  -18 + ConstanChange;}
        if(unum == 5) {yy =   18 + ConstanChange;}
        if(yy > 32)
            yy = 32;
        if(yy < -32)
            yy = -32;
        return Vector2D(xx, yy);
    }
    if(unum == 6)
    {
        int ConstanChange = ball_pos.y / 4.25;
        int xx = ball_pos.x - 1;
        if(xx > 30) xx = 30;
        int yy;
        if(unum == 6) {yy = ConstanChange;}
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > -10)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
    if(unum == 7 || unum == 8)
    {
        int ConstanChange = ball_pos.y / 5;
        int xx = ball_pos.x + 4;
        if(xx > 36) xx = 36;
        int yy;
        if(unum == 7) {yy = -15 + ConstanChange;}
        if(unum == 8) {yy =  15 + ConstanChange;}
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > -10)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
    if(unum == 9)
    {
        int ConstanChange = ball_pos.y / 2;
        int xx = ball_pos.x + 10;
        if(xx > 45) xx = 45;
        int yy;
        if(unum == 9) {yy = ConstanChange;}
        if(wm.getTeammateNearestToBall(5, true)->unum() == 9 || wm.getTeammateNearestToBall(5, true)->unum() == 10 || wm.getTeammateNearestToBall(5, true)->unum() == 11  && wm.gameMode().type() == GameMode::PlayOn)
            xx = ball_pos.x;
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > 20)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
    if(unum == 10 || unum == 11)
    {
        int ConstanChange = ball_pos.y / 5.7;
        int xx = ball_pos.x + 10;
        if(xx > 45) xx = 45;
        int yy;
        if(unum == 10) {yy =  -28 + ConstanChange;}
        if(unum == 11) {yy =   28 + ConstanChange;}
        if(ball_pos.x > 20)
        {
            if(unum == 10)
            {
                yy = yy + ball_pos.x / 2;
            }
            if(unum == 11)
            {
                yy = yy - ball_pos.x / 2;
            }
        }
        if(wm.getTeammateNearestToBall(5, true)->unum() == 9 || wm.getTeammateNearestToBall(5, true)->unum() == 10 || wm.getTeammateNearestToBall(5, true)->unum() == 11)
            xx = ball_pos.x;
        if(xx + 2 >= wm.offsideLineX() && wm.gameMode().type() == GameMode::PlayOn)
            xx = wm.offsideLineX() - 2;
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > 20)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
}

Vector2D Bhv_BasicMove::OffensiveSTRG2(rcsc::PlayerAgent *agent, int unum)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    if(unum == 2 || unum == 3 || unum == 4 || unum == 5)
    {
        int ConstanChange = ball_pos.y / 2.2;
        int xx = ball_pos.x - 10;
        if(xx > 5) xx = 5;
        int yy;
        if(unum == 2) {yy =    2;}
        if(unum == 3) {yy =   15;}
        if(unum == 4) {yy =  -14;}
        if(unum == 5) {yy =   26;}
        return Vector2D(xx, yy);
    }
    if(unum == 6 || unum == 7 || unum == 8)
    {
        int xx = ball_pos.x;
        if(xx > 36) xx = 36;
        int yy;
        if(unum == 6) {yy = 18;}
        if(unum == 7) {yy = 6;}
        if(unum == 8) {yy = 30;}
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > 20)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
    if(unum == 9 || unum == 10 || unum == 11)
    {
        int xx = ball_pos.x + 12;
        if(xx > 48) xx = 48;
        int yy;
        if(unum == 9) {yy =  21;}
        if(unum == 10) {yy = 10;}
        if(unum == 11) {yy = 33;}
        if(wm.getTeammateNearestToBall(5, true)->unum() == 9 || wm.getTeammateNearestToBall(5, true)->unum() == 10 || wm.getTeammateNearestToBall(5, true)->unum() == 11  && wm.gameMode().type() == GameMode::PlayOn)
            xx = ball_pos.x;
        if(xx + 2 > wm.offsideLineX())
            xx = wm.offsideLineX() - 2;
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > 20)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
}

int Bhv_BasicMove::dash(rcsc::PlayerAgent *agent, Vector2D point)
{
    const WorldModel &wm = agent->world();
    double dist = wm.self().pos().dist(point);
    double power = dist * 15;
    if (power > 100)
        power = 100;
    return power;
}

int Bhv_BasicMove::area(rcsc::PlayerAgent *agent)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    std::vector<Vector2D> poses;
    poses.push_back(Vector2D(-48.52, -28.96));
    poses.push_back(Vector2D(-47.75, -25.87));
    poses.push_back(Vector2D(-48.26, -21.88));
    poses.push_back(Vector2D(-48.13, -18.40));
    poses.push_back(Vector2D(-48.22, -15.20));
    poses.push_back(Vector2D(-48.52, -11.33));
    poses.push_back(Vector2D(-48.78, -8.75));
    poses.push_back(Vector2D(-49.03, -5.66));
    poses.push_back(Vector2D(-48.78, -2.06));
    poses.push_back(Vector2D(-48.65, 1.80));
    poses.push_back(Vector2D(-48.78, 6.18));
    poses.push_back(Vector2D(-48.78, 10.94));
    poses.push_back(Vector2D(-49.03, 15.06));
    poses.push_back(Vector2D(-48.78, 19.69));
    poses.push_back(Vector2D(-48.39, 23.55));
    poses.push_back(Vector2D(-48.91, 27.54));
    poses.push_back(Vector2D(-49.03, 30.89));
    poses.push_back(Vector2D(-42.21, -29.34));
    poses.push_back(Vector2D(-42.34, -24.07));
    poses.push_back(Vector2D(-41.96, -20.21));
    poses.push_back(Vector2D(-41.83, -15.83));
    poses.push_back(Vector2D(-41.70, -11.20));
    poses.push_back(Vector2D(-41.57, -7.21));
    poses.push_back(Vector2D(-41.57, -2.70));
    poses.push_back(Vector2D(-41.70, 1.54));
    poses.push_back(Vector2D(-41.57, 5.79));
    poses.push_back(Vector2D(-41.57, 10.55));
    poses.push_back(Vector2D(-41.44, 14.80));
    poses.push_back(Vector2D(-41.44, 19.43));
    poses.push_back(Vector2D(-41.44, 24.07));
    poses.push_back(Vector2D(-41.44, 28.19));
    poses.push_back(Vector2D(-41.44, 32.05));
    poses.push_back(Vector2D(-34.23, -30.37));
    poses.push_back(Vector2D(-34.11, -26.51));
    poses.push_back(Vector2D(-33.98, -21.49));
    poses.push_back(Vector2D(-33.85, -16.86));
    poses.push_back(Vector2D(-33.98, -12.36));
    poses.push_back(Vector2D(-33.72, -7.46));
    poses.push_back(Vector2D(-33.85, -2.70));
    poses.push_back(Vector2D(-33.85, 1.80));
    poses.push_back(Vector2D(-33.46, 6.82));
    poses.push_back(Vector2D(-33.98, 11.20));
    poses.push_back(Vector2D(-34.11, 15.44));
    poses.push_back(Vector2D(-34.36, 19.56));
    poses.push_back(Vector2D(-34.62, 23.94));
    poses.push_back(Vector2D(-35.01, 28.19));
    poses.push_back(Vector2D(-27.93, -30.50));
    poses.push_back(Vector2D(-27.67, -26.00));
    poses.push_back(Vector2D(-27.67, -20.72));
    poses.push_back(Vector2D(-27.54, -16.73));
    poses.push_back(Vector2D(-27.03, -11.58));
    poses.push_back(Vector2D(-27.61, -7.00));
    poses.push_back(Vector2D(-27.16, -2.32));
    poses.push_back(Vector2D(-26.51, 2.96));
    poses.push_back(Vector2D(-26.64, 7.59));
    poses.push_back(Vector2D(-26.38, 12.36));
    poses.push_back(Vector2D(-26.13, 16.00));
    poses.push_back(Vector2D(-26.77, 20.72));
    poses.push_back(Vector2D(-26.77, 25.23));
    poses.push_back(Vector2D(-27.16, 28.70));
    poses.push_back(Vector2D(-27.28, 32.82));

    Vector2D nearest = poses[0];
    int count = 0;
    for (int i = 1; i < poses.size(); i++)
    {
        if (ball_pos.dist(poses[i]) < ball_pos.dist(nearest))
        {
            nearest = poses[i];
            count = i;
        }
    }
    return count;
}

Vector2D Bhv_BasicMove::CornerForamtion(rcsc::PlayerAgent *agent)
{
    const WorldModel &wm = agent->world();
    const Vector2D ball_pos = wm.ball().pos();
    std::vector <Vector2D> poses(12);
    if(ball_pos.y > 30)
    {
        poses.at(6) = Vector2D(41,  20);
        poses.at(7) = Vector2D(37, 6);
        poses.at(8) = Vector2D(48, 29);
        poses.at(9) = Vector2D(49,  14);
        poses.at(10) = Vector2D(44, -1);
        poses.at(11) = Vector2D(52, 34);
    }
    else if(ball_pos.y < 30)
    {
        poses.at(6) = Vector2D(41, -20);
        poses.at(7) = Vector2D(48, -29);
        poses.at(8) = Vector2D(37, -6);
        poses.at(9) = Vector2D(49, -14);
        poses.at(10) = Vector2D(52, -34);
        poses.at(11) = Vector2D(44, 1);
    }
    return poses.at(wm.self().unum()).y != 0.0 ? poses.at(wm.self().unum()) : getPosition(wm, wm.self().unum());
}








////////////////////////                 Strategy Class                    ////////////////////




Strategy::StrategyType Strategy::Get_RecommendedStragetyType(PlayerAgent *agent)
{
    static bool is_Changed = false;
    const WorldModel &wm = agent->world();
    int our_score = wm.ourSide() == LEFT ? wm.gameMode().scoreLeft() : wm.gameMode().scoreRight();
    int opp_score = wm.ourSide() == LEFT ? wm.gameMode().scoreRight() : wm.gameMode().scoreLeft();
    if(wm.time().cycle() < 4000)
        return Strategy::STRATEGY1;
    else if(((our_score <= opp_score ) && wm.time().cycle() > 4000) || is_Changed)
    {
        is_Changed = true;
        agent->debugClient().addMessage("STRG222");
        return Strategy::STRATEGY2;
    }
    else
        return Strategy::STRATEGY1;   
}

Vector2D Strategy::Get_Formation(PlayerAgent *agent, int Unum, Strategy::StrategyType Type)
{
    const WorldModel &wm = agent->world();
    if(wm.gameMode().type() == GameMode::PlayOn)
    {
        if(wm.lastKickerSide() != wm.ourSide())
            return DefenseFormation(agent, Unum);
        else
        {
            switch (Type)
            {
            case Strategy::StrategyType::STRATEGY1:
                agent->debugClient().addMessage("Strg1");
                return OffenseStrg1(agent, Unum);
                break;
            case Strategy::StrategyType::STRATEGY2:
                agent->debugClient().addMessage("Strg2");
                return OffenseStrg2(agent, Unum);
                break;
            }
        }
    }
    else
    {
        if(!wm.gameMode().isOurSetPlay(wm.ourSide()))
            return DefenseFormation(agent, Unum);
        else
        {
            return OffenseStrg2(agent, Unum);      
        }
    }
}

Vector2D Strategy::DefenseFormation(PlayerAgent *agent, int self_unum)
{
    /*

    Our deffense strategy:
    4 , 3 , 3



    *     *
       *
    *

     * *

    *
       *
    *     *



    //group one:   4 deffense TMs
    //group two:   3 middle TM
    //group three: 1 middle TM
    //group four:  2forward Tms


    our strategy with numbers:

    *4     *10
       *7
    *2

     *6 *9

    *3
       *8
    *5     *11





    The only thing that is left is Coding...
    Soroush: processing...
    When you start Coding, you will realize you are stuck in the mud, like a Dunkey... "The word from Soroush in the end of his life"
    Soroush Left His Life...
    process Complete, Press Enter to Exit...

    */

    const WorldModel &wm = agent->world();
    /*std::vector <Vector2D> KickOffPoses;
    if(wm.gameMode().type() == GameMode::KickOff_ && wm.gameMode().side() == wm.ourSide())
    {
        KickOffPoses.at(1) = Vector2D(-28,0);
        KickOffPoses.at(2) = Vector2D(-17,0);
        KickOffPoses.at(3) = Vector2D(-17,9);
        KickOffPoses.at(4) = Vector2D(-17,-13);
        KickOffPoses.at(5) = Vector2D(-17,14);
        KickOffPoses.at(6) = Vector2D(0,0);
        KickOffPoses.at(7) = Vector2D(-1,-5);
        KickOffPoses.at(8) = Vector2D(-1,24);
        KickOffPoses.at(9) = Vector2D(-1,8);
        KickOffPoses.at(10) = Vector2D(-1,-10);
        KickOffPoses.at(11) = Vector2D(-1,4);
        return KickOffPoses.at(wm.self().unum());
    }
    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() == wm.ourSide() && wm.ball().pos().x > 32 && wm.ball().pos().y > -13 && wm.ball().pos().y < 13)
    {
        std::vector <Vector2D> defens_poss(11);
        defens_poss[0] = Vector2D(-50,0);//
        defens_poss[1] = Vector2D(9,-21);//
        defens_poss[2] = Vector2D(9,0);//
        defens_poss[3] = Vector2D(0,-10);//
        defens_poss[4] = Vector2D(0,10);//
        defens_poss[5] = Vector2D(30,0);//
        defens_poss[6] = Vector2D(35,-7);//
        defens_poss[7] = Vector2D(35,7);//
        defens_poss[8] = Vector2D(40,0);//
        defens_poss[9] = Vector2D(44,-8);//
        defens_poss[10] = Vector2D(44,8);//
        int xx;

            xx = wm.self().unum();
            return defens_poss[xx-1];
    }*/

    Vector2D ball_pos = wm.ball().pos();
    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() != wm.ourSide() && wm.ball().pos().x < -35 && wm.ball().pos().y > -20 && wm.ball().pos().y < 20)
    {
        std::vector <Vector2D> defens_pos(11);
        defens_pos[0] = Vector2D(-52,0);//
        defens_pos[1] = Vector2D(-52,-2);//
        defens_pos[2] = Vector2D(-52,2);//
        defens_pos[3] = Vector2D(-52,-6);//
        defens_pos[4] = Vector2D(-52,6);//
        defens_pos[5] = Vector2D(-44,0);//
        defens_pos[6] = Vector2D(-52,-4);//
        defens_pos[7] = Vector2D(-52,4);//
        defens_pos[8] = Vector2D(-23,-0);//
        defens_pos[9] = Vector2D(-26,-9);//
        defens_pos[10] = Vector2D(-26,9);
        int x;

            x = wm.self().unum();
            return defens_pos[x-1];
    }
    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() == wm.ourSide() && wm.ball().pos().x > 32 && wm.ball().pos().y > -13 && wm.ball().pos().y < 13)
    {
        std::vector <Vector2D> defens_poss(11);
        defens_poss[0] = Vector2D(-50,0);//
        defens_poss[1] = Vector2D(9,-21);//
        defens_poss[2] = Vector2D(9,0);//
        defens_poss[3] = Vector2D(0,-10);//
        defens_poss[4] = Vector2D(0,10);//
        defens_poss[5] = Vector2D(30,0);//
        defens_poss[6] = Vector2D(35,-7);//
        defens_poss[7] = Vector2D(35,7);//
        defens_poss[8] = Vector2D(40,0);//
        defens_poss[9] = Vector2D(44,-8);//
        defens_poss[10] = Vector2D(44,8);//
        int xx;

            xx = wm.self().unum();
     
           return defens_poss[xx-1];
    }
    if(wm.ball().pos().x < -27)
        return Strategy().GetHybridDeffenseFormation(agent);
    int ConstanChange = ball_pos.y / 2.1;
    double DistToTheirGoal = ball_pos.dist(Vector2D(52,ball_pos.y));
    int ConstantY_MinimizationChange = DistToTheirGoal / 15;
    int ConstantX_MaximizationChange = DistToTheirGoal / 10;
    //group one:
    if(self_unum > 1 && self_unum < 6)
    {
        int xx = ball_pos.x - 18 + ConstantX_MaximizationChange;
        if(xx > 0) {xx = 0;}
        int yy;
        if(self_unum == 2) {yy =  -9 + ConstanChange;}
        if(self_unum == 3) {yy =   9 + ConstanChange;}
        if(self_unum == 4) {yy =  -20 + ConstanChange;}
        if(self_unum == 5) {yy =   20 + ConstanChange;}
        if(self_unum == 2) {yy = yy + ConstantY_MinimizationChange;}
        if(self_unum == 3) {yy = yy - ConstantY_MinimizationChange;}
        if(self_unum == 4) {yy = yy + ConstantY_MinimizationChange;}
        if(self_unum == 5) {yy = yy - ConstantY_MinimizationChange;}
        if(self_unum == 2) {if(yy > 7){yy = 7;}}
        if(self_unum == 3) {if(yy < -7){yy = -7;}}
        if(self_unum == 4) {if(yy > -7){yy = -7;}}
        if(self_unum == 5) {if(yy < 7){yy = 7;}}
        return Vector2D(xx,yy);
    }
    //group(NotRealy) two:
    if(self_unum == 6)
    {
        int xx = ball_pos.x - 15 + ConstantX_MaximizationChange;
        if(xx > 8) xx = 8;
        int yy = 0 + ConstanChange;
        return Vector2D(xx,yy);
    }
    //group three:
    if(self_unum == 7 || self_unum == 8)
    {
        int xx = ball_pos.x - 15 + ConstantX_MaximizationChange;
        if(xx > 8) xx = 8;
        int yy;
        if(self_unum == 7) {yy = -14 + ConstanChange;}
        if(self_unum == 8) {yy =  14 + ConstanChange;}
        if(self_unum == 7) {yy = yy + ConstantY_MinimizationChange;}
        if(self_unum == 8) {yy = yy - ConstantY_MinimizationChange;}
        return Vector2D(xx,yy);
    }
    //group four
    if(self_unum == 9 || self_unum == 10 || self_unum == 11)
    {
        int xx = ball_pos.x - 10 + ConstantX_MaximizationChange;
        if(xx > 13) xx = 13;
        int yy;
        if(self_unum == 9 ) {yy =   0 + ConstanChange;}
        if(self_unum == 10) {yy = -15 + ConstanChange;}
        if(self_unum == 11) {yy =  15 + ConstanChange;}
        if(self_unum == 10) {yy =   yy - ConstantY_MinimizationChange;}
        if(self_unum == 11) {yy =   yy + ConstantY_MinimizationChange;}
        return Vector2D(xx,yy);
    }
}

Vector2D Strategy::OffenseStrg1(PlayerAgent *agent, int self_unum)
{
    const WorldModel &wm = agent->world();
    int ball_step = 0;
    if( wm.gameMode().type() == GameMode::PlayOn|| wm.gameMode().type() == GameMode::GoalKick_ )
    {
        ball_step = std::min( 1000, wm.interceptTable()->teammateReachCycle() );
        ball_step = std::min( ball_step, wm.interceptTable()->opponentReachCycle() );
        ball_step = std::min( ball_step, wm.interceptTable()->selfReachCycle() );
    }

    Vector2D ball_pos = wm.ball().inertiaPoint( ball_step );

    dlog.addText( Logger::TEAM,
                  __FILE__": HOME POSITION: ball pos=(%.1f %.1f) step=%d",
                  ball_pos.x, ball_pos.y,
                  ball_step );


    std::vector<Vector2D> positions(12);

    double min_x_rectengle[12]={0,-52,-52,-52,-52,-52,-40,-40,-40,-32,-32,-32}; //-50
    double max_x_rectengle[12]={0,-48,10,10,10,10,35,35,40,52,52,52}; //35
    double min_y_rectengle[12]={0,-2,-17,-11,-15,0,-10,-22, 5,-15,-33, 10}; //-10
    double max_y_rectengle[12]={0,2, 11, 17,0,15, 22, 10,7, 15, -10, 33};  // 22

    if(wm.self().unum() == 9 && wm.gameMode().type() == GameMode::KickOff_){
        return Vector2D(-3,-3);
    }
    if(wm.self().unum() == 11 && wm.gameMode().type() == GameMode::KickOff_){
        return Vector2D(-3,3);
    }

    if(wm.ball().pos().x >= 0 && wm.lastKickerSide()==wm.ourSide()){
        for(int a = 0 ; a<=11 ; a++)
        {
            if(a==2 && a==3 && a==6)
            {
                max_x_rectengle[a]=max_x_rectengle[a];
            }
            else {
                max_x_rectengle[a]=max_x_rectengle[a]+25;
            }
        }
    }

    for(int i=1; i<=11; i++){
          double xx_rectengle = max_x_rectengle[i] - min_x_rectengle[i];
          double yy_rectengle = max_y_rectengle[i] - min_y_rectengle[i];
          double x_ball = ball_pos.x + 52.5;
          x_ball /= 105.5;
          double y_ball = ball_pos.y + 34;
          y_ball /= 68.0;
          double x_pos = xx_rectengle * x_ball + min_x_rectengle[i];
          if(wm.gameMode().side()==wm.ourSide()){
              x_pos = xx_rectengle * x_ball + min_x_rectengle[i]+3;
        }
          double y_pos = yy_rectengle * y_ball + min_y_rectengle[i];
          positions[i] = Vector2D(x_pos,y_pos);
    }
    
    if((wm.self().unum() ==2 || wm.self().unum() ==3 || wm.self().unum() ==4 || wm.self().unum() ==5) && positions[wm.self().unum()].x>0){
        positions[wm.self().unum()]=wm.self().pos();
    }
    /*if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().type() != GameMode::FoulCharge_ && wm.gameMode().type() != GameMode::OffSide_ ){
    if(wm.gameMode().side() != wm.ourSide() && wm.ball().pos().x < 0 && wm.ball().pos().x > -22)
    {
        return Bhv_BasicMove().full_press_1(wm);
    }
    }*/

    Vector2D c(4,5);

    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() == wm.ourSide() && wm.ball().pos().x > 32 && wm.ball().pos().y > -13 && wm.ball().pos().y < 13)
    {
        std::vector <Vector2D> defens_poss(11);
        defens_poss[0] = Vector2D(-50,0);//
        defens_poss[1] = Vector2D(9,-21);//
        defens_poss[2] = Vector2D(9,0);//
        defens_poss[3] = Vector2D(0,-10);//
        defens_poss[4] = Vector2D(0,10);//
        defens_poss[5] = Vector2D(30,0);//
        defens_poss[6] = Vector2D(35,-7);//
        defens_poss[7] = Vector2D(35,7);//
        defens_poss[8] = Vector2D(40,0);//
        defens_poss[9] = Vector2D(44,-8);//
        defens_poss[10] = Vector2D(44,8);//
        int xx;

            xx = wm.self().unum();
            return defens_poss[xx-1];
    }



    if(wm.gameMode().type() != GameMode::PlayOn && wm.gameMode().side() != wm.ourSide() && wm.ball().pos().x < -35 && wm.ball().pos().y > -20 && wm.ball().pos().y < 20)
    {
        std::vector <Vector2D> defens_pos(11);
        defens_pos[0] = Vector2D(-52,0);//
        defens_pos[1] = Vector2D(-52,-2);//2
        defens_pos[2] = Vector2D(-52,2);//3
        defens_pos[3] = Vector2D(-52,-4);//4
        defens_pos[4] = Vector2D(-52,4);//5
        defens_pos[5] = Vector2D(-51,0);//6
        defens_pos[6] = Vector2D(-51,-5);//7
        defens_pos[7] = Vector2D(-51,5);//8
        defens_pos[8] = Vector2D(-23,-0);//9
        defens_pos[9] = Vector2D(-51,-3);//10
        defens_pos[10] = Vector2D(-51,3);//11
        int x;

            x = wm.self().unum();
            return defens_pos[x-1];
    }
    if ( ServerParam::i().useOffside() )
    {
        double max_x = wm.offsideLineX();
        if ( ServerParam::i().kickoffOffside()
             && ( wm.gameMode().type() == GameMode::BeforeKickOff
                  || wm.gameMode().type() == GameMode::AfterGoal_ ) )
        {
            max_x = 0.0;
        }
        else
        {
            int mate_step = wm.interceptTable()->teammateReachCycle();
            if ( mate_step < 50 )
            {
                Vector2D trap_pos = wm.ball().inertiaPoint( mate_step );
                if ( trap_pos.x > max_x ) max_x = trap_pos.x;
            }

            max_x -= 1.0;
        }

        for ( int unum = 1; unum <= 11; ++unum )
        {
            if ( positions[unum].x > max_x )
            {
                dlog.addText( Logger::TEAM,
                              "____ %d offside. home_pos_x %.2f -> %.2f",
                              unum,
                              positions[unum].x, max_x );
                positions[unum].x = max_x;
            }
        }
    }
    if(self_unum > 5 && self_unum < 9)
    {
        if(positions.at(self_unum).x > 40)
            positions.at(self_unum).x = 40;
    }
    return positions.at(self_unum);
}

Vector2D Strategy::OffenseStrg2(PlayerAgent *agent, int unum)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    if(unum == 2 || unum == 3)
    {
        int ConstanChange = ball_pos.y / 2.2;
        int xx = ball_pos.x - 10;
        if(xx > 5) xx = 5;
        int yy;
        if(unum == 2) {yy =  -5 + ConstanChange;}
        if(unum == 3) {yy =   5 + ConstanChange;}
        return Vector2D(xx, yy);
    }
    if(unum == 4 || unum == 5)
    {
        int ConstanChange = ball_pos.y / 2.9;
        int xx = ball_pos.x - 3;
        if(xx > 20) xx = 20;
        int yy;
        if(unum == 4) {yy =  -18 + ConstanChange;}
        if(unum == 5) {yy =   18 + ConstanChange;}
        if(yy > 32)
            yy = 32;
        if(yy < -32)
            yy = -32;
        return Vector2D(xx, yy);
    }
    if(unum == 6)
    {
        int ConstanChange = ball_pos.y / 4.25;
        int xx = ball_pos.x - 1;
        if(xx > 30) xx = 30;
        int yy;
        if(unum == 6) {yy = ConstanChange;}
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > 20)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
    if(unum == 7 || unum == 8)
    {
        int ConstanChange = ball_pos.y / 5;
        int xx = ball_pos.x + 4;
        if(xx > 36) xx = 36;
        int yy;
        if(unum == 7) {yy = -15 + ConstanChange;}
        if(unum == 8) {yy =  15 + ConstanChange;}
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > 20)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
    if(unum == 9)
    {
        int ConstanChange = ball_pos.y / 2;
        int xx = ball_pos.x + 10;
        if(xx > 45) xx = 45;
        int yy;
        if(unum == 9) {yy = ConstanChange;}
        if(wm.getTeammateNearestToBall(5, true)->unum() == 9 || wm.getTeammateNearestToBall(5, true)->unum() == 10 || (wm.getTeammateNearestToBall(5, true)->unum() == 11  && wm.gameMode().type() == GameMode::PlayOn))
            xx = ball_pos.x;
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > 20)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
    if(unum == 10 || unum == 11)
    {
        int ConstanChange = ball_pos.y / 5.7;
        int xx = ball_pos.x + 10;
        if(xx > 45) xx = 45;
        int yy;
        if(unum == 10) {yy =  -28 + ConstanChange;}
        if(unum == 11) {yy =   28 + ConstanChange;}
        if(ball_pos.x > 20)
        {
            if(unum == 10)
            {
                yy = yy + ball_pos.x / 2;
            }
            if(unum == 11)
            {
                yy = yy - ball_pos.x / 2;
            }
        }
        if(wm.getTeammateNearestToBall(5, true)->unum() == 9 || wm.getTeammateNearestToBall(5, true)->unum() == 10 || wm.getTeammateNearestToBall(5, true)->unum() == 11)
            xx = ball_pos.x;
        if(xx + 2 >= wm.offsideLineX() && wm.gameMode().type() == GameMode::PlayOn)
            xx = wm.offsideLineX() - 2;
        Vector2D trg = Vector2D(xx, yy);
        if(ball_pos.x > 20)
        {
            double dist = wm.ball().distFromSelf();
            double Ang = (ball_pos - trg).th().degree();
            Vector2D Secondary = Vector2D::polar2vector(dist * 20 / 100, Ang) + trg;
            trg = Secondary;
        }
        return trg;
    }
}

Vector2D Strategy::GetHybridDeffenseFormation(rcsc::PlayerAgent *agent)
{
    int count = area(agent);
    std::vector<Vector2D> poses(12);

    if (count == 0)
    {

        poses.at(1) = Vector2D(-51.40, -6.42);
        poses.at(2) = Vector2D(-50.00, -11.64);
        poses.at(3) = Vector2D(-45.80, -4.90);
        poses.at(4) = Vector2D(-48.91, -23.42);
        poses.at(5) = Vector2D(-49.11, 4.77);
        poses.at(6) = Vector2D(-40.71, -6.93);
        poses.at(7) = Vector2D(-43.38, -13.68);
        poses.at(8) = Vector2D(-42.49, 1.84);
        poses.at(9) = Vector2D(-30.24, -4.50);
        poses.at(10) = Vector2D(-41.35, -21.18);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 1)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-51.48, -14.29);
        poses.at(3) = Vector2D(-47.75, -10.68);
        poses.at(4) = Vector2D(-48.26, -19.69);
        poses.at(5) = Vector2D(-49.29, 3.47);
        poses.at(6) = Vector2D(-41.70, -10.30);
        poses.at(7) = Vector2D(-44.14, -15.19);
        poses.at(8) = Vector2D(-43.76, -2.70);
        poses.at(9) = Vector2D(-34.23, -3.60);
        poses.at(10) = Vector2D(-36.94, -15.83);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 2)
    {

        poses.at(1) = Vector2D(-50.89, -5.66);
        poses.at(2) = Vector2D(-50.13, -10.75);
        poses.at(3) = Vector2D(-47.07, -3.24);
        poses.at(4) = Vector2D(-48.26, -19.69);
        poses.at(5) = Vector2D(-49.29, 3.47);
        poses.at(6) = Vector2D(-45.04, -7.95);
        poses.at(7) = Vector2D(-45.42, -11.90);
        poses.at(8) = Vector2D(-43.77, 0.32);
        poses.at(9) = Vector2D(-34.23, -3.60);
        poses.at(10) = Vector2D(-39.95, -10.11);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 3)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-49.94, -10.94);
        poses.at(3) = Vector2D(-50.45, -3.22);
        poses.at(4) = Vector2D(-48.26, -14.54);
        poses.at(5) = Vector2D(-49.81, 2.83);
        poses.at(6) = Vector2D(-44.53, -5.53);
        poses.at(7) = Vector2D(-46.33, -10.68);
        poses.at(8) = Vector2D(-43.76, 2.32);
        poses.at(9) = Vector2D(-39.64, -5.02);
        poses.at(10) = Vector2D(-44.14, -14.16);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 4)
    {

        poses.at(1) = Vector2D(-51.09, -6.56);
        poses.at(2) = Vector2D(-48.09, -7.44);
        poses.at(3) = Vector2D(-50.45, -3.22);
        poses.at(4) = Vector2D(-50.84, -10.04);
        poses.at(5) = Vector2D(-50.45, 1.29);
        poses.at(6) = Vector2D(-46.31, -1.84);
        poses.at(7) = Vector2D(-47.62, -11.45);
        poses.at(8) = Vector2D(-45.04, 3.24);
        poses.at(9) = Vector2D(-41.48, -3.75);
        poses.at(10) = Vector2D(-43.76, -9.65);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 5)
    {

        poses.at(1) = Vector2D(-51.09, -6.56);
        poses.at(2) = Vector2D(-48.78, -5.79);
        poses.at(3) = Vector2D(-50.45, -3.22);
        poses.at(4) = Vector2D(-51.22, -7.72);
        poses.at(5) = Vector2D(-50.45, 1.29);
        poses.at(6) = Vector2D(-46.46, -6.56);
        poses.at(7) = Vector2D(-49.42, -7.98);
        poses.at(8) = Vector2D(-46.85, 4.63);
        poses.at(9) = Vector2D(-43.13, -2.48);
        poses.at(10) = Vector2D(-46.59, -9.52);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 6)
    {

        poses.at(1) = Vector2D(-51.09, -6.56);
        poses.at(2) = Vector2D(-48.78, -5.79);
        poses.at(3) = Vector2D(-51.35, -3.60);
        poses.at(4) = Vector2D(-51.22, -7.72);
        poses.at(5) = Vector2D(-50.71, 3.22);
        poses.at(6) = Vector2D(-48.01, -5.79);
        poses.at(7) = Vector2D(-49.81, -5.66);
        poses.at(8) = Vector2D(-48.73, -0.32);
        poses.at(9) = Vector2D(-44.66, -4.39);
        poses.at(10) = Vector2D(-46.59, -9.52);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 7)
    {

        poses.at(1) = Vector2D(-51.48, -3.35);
        poses.at(2) = Vector2D(-51.48, -3.35);
        poses.at(3) = Vector2D(-51.35, -4.50);
        poses.at(4) = Vector2D(-51.48, -6.05);
        poses.at(5) = Vector2D(-50.84, 4.76);
        poses.at(6) = Vector2D(-51.35, -2.96);
        poses.at(7) = Vector2D(-51.02, -1.59);
        poses.at(8) = Vector2D(-49.24, 2.35);
        poses.at(9) = Vector2D(-50.13, 0.32);
        poses.at(10) = Vector2D(-45.69, -6.18);
        poses.at(11) = Vector2D(-22.27, 14.54);
    }

    if (count == 8)
    {

        poses.at(1) = Vector2D(-51.74, -3.09);
        poses.at(2) = Vector2D(-51.48, -3.35);
        poses.at(3) = Vector2D(-52.25, -4.63);
        poses.at(4) = Vector2D(-52.38, -4.50);
        poses.at(5) = Vector2D(-51.61, 3.09);
        poses.at(6) = Vector2D(-51.35, -2.96);
        poses.at(7) = Vector2D(-52.25, -1.54);
        poses.at(8) = Vector2D(-50.84, 2.32);
        poses.at(9) = Vector2D(-51.35, 0.13);
        poses.at(10) = Vector2D(-46.07, -1.54);
        poses.at(11) = Vector2D(-28.96, 15.19);
    }

    if (count == 9)
    {

        poses.at(1) = Vector2D(-52.25, 1.29);
        poses.at(2) = Vector2D(-52.38, -1.29);
        poses.at(3) = Vector2D(-51.74, -2.45);
        poses.at(4) = Vector2D(-52.38, -4.50);
        poses.at(5) = Vector2D(-51.61, 5.02);
        poses.at(6) = Vector2D(-51.99, 1.80);
        poses.at(7) = Vector2D(-51.48, -2.45);
        poses.at(8) = Vector2D(-51.99, 3.47);
        poses.at(9) = Vector2D(-51.87, -0.64);
        poses.at(10) = Vector2D(-46.07, -1.54);
        poses.at(11) = Vector2D(-38.61, 10.30);
    }

    if (count == 10)
    {

        poses.at(1) = Vector2D(-52.12, 4.25);
        poses.at(2) = Vector2D(-50.45, -1.16);
        poses.at(3) = Vector2D(-51.35, 2.57);
        poses.at(4) = Vector2D(-50.84, -6.31);
        poses.at(5) = Vector2D(-51.74, 5.79);
        poses.at(6) = Vector2D(-51.09, 0.64);
        poses.at(7) = Vector2D(-51.22, -3.47);
        poses.at(8) = Vector2D(-50.71, 1.67);
        poses.at(9) = Vector2D(-50.97, 2.06);
        poses.at(10) = Vector2D(-42.60, -4.38);
        poses.at(11) = Vector2D(-46.85, 7.72);
    }

    if (count == 11)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-50.84, -1.80);
        poses.at(3) = Vector2D(-50.32, 5.15);
        poses.at(4) = Vector2D(-50.84, -6.31);
        poses.at(5) = Vector2D(-50.06, 8.62);
        poses.at(6) = Vector2D(-51.35, 1.80);
        poses.at(7) = Vector2D(-48.52, 1.54);
        poses.at(8) = Vector2D(-41.96, 5.02);
        poses.at(9) = Vector2D(-46.85, 5.53);
        poses.at(10) = Vector2D(-36.68, -7.85);
        poses.at(11) = Vector2D(-44.79, 11.84);
    }

    if (count == 12)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-48.52, 0.77);
        poses.at(3) = Vector2D(-48.98, 8.59);
        poses.at(4) = Vector2D(-48.78, -4.38);
        poses.at(5) = Vector2D(-50.51, 11.13);
        poses.at(6) = Vector2D(-45.56, 0.00);
        poses.at(7) = Vector2D(-47.88, 4.63);
        poses.at(8) = Vector2D(-44.78, 5.53);
        poses.at(9) = Vector2D(-40.03, 0.39);
        poses.at(10) = Vector2D(-32.95, -12.10);
        poses.at(11) = Vector2D(-44.79, 11.84);
    }

    if (count == 13)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-49.94, -0.26);
        poses.at(3) = Vector2D(-49.68, 7.85);
        poses.at(4) = Vector2D(-48.78, -4.38);
        poses.at(5) = Vector2D(-49.42, 14.03);
        poses.at(6) = Vector2D(-47.10, 3.60);
        poses.at(7) = Vector2D(-45.17, -0.77);
        poses.at(8) = Vector2D(-44.53, 7.98);
        poses.at(9) = Vector2D(-40.28, 5.28);
        poses.at(10) = Vector2D(-32.95, -12.10);
        poses.at(11) = Vector2D(-38.22, 18.79);
    }

    if (count == 14)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-49.55, 4.76);
        poses.at(3) = Vector2D(-49.16, 11.33);
        poses.at(4) = Vector2D(-48.78, -4.38);
        poses.at(5) = Vector2D(-49.03, 17.50);
        poses.at(6) = Vector2D(-45.43, 7.34);
        poses.at(7) = Vector2D(-43.24, 0.26);
        poses.at(8) = Vector2D(-44.79, 11.45);
        poses.at(9) = Vector2D(-40.15, 9.14);
        poses.at(10) = Vector2D(-32.95, -12.10);
        poses.at(11) = Vector2D(-39.77, 17.89);
    }

    if (count == 15)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-48.52, 6.95);
        poses.at(3) = Vector2D(-49.42, 15.06);
        poses.at(4) = Vector2D(-48.09, -3.37);
        poses.at(5) = Vector2D(-48.91, 21.75);
        poses.at(6) = Vector2D(-44.27, 8.08);
        poses.at(7) = Vector2D(-43.77, 1.59);
        poses.at(8) = Vector2D(-44.53, 12.91);
        poses.at(9) = Vector2D(-39.00, 11.58);
        poses.at(10) = Vector2D(-24.97, -17.63);
        poses.at(11) = Vector2D(-41.98, 22.58);
    }

    if (count == 16)
    {

        poses.at(1) = Vector2D(-52.64, 5.53);
        poses.at(2) = Vector2D(-47.84, 4.77);
        poses.at(3) = Vector2D(-48.22, 14.69);
        poses.at(4) = Vector2D(-48.26, -1.54);
        poses.at(5) = Vector2D(-48.78, 24.97);
        poses.at(6) = Vector2D(-43.26, 8.59);
        poses.at(7) = Vector2D(-39.82, 0.57);
        poses.at(8) = Vector2D(-44.78, 13.04);
        poses.at(9) = Vector2D(-39.82, 14.19);
        poses.at(10) = Vector2D(-24.97, -17.63);
        poses.at(11) = Vector2D(-42.62, 25.89);
    }

    if (count == 17)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.82, -21.62);
        poses.at(3) = Vector2D(-44.79, -12.36);
        poses.at(4) = Vector2D(-46.33, -27.67);
        poses.at(5) = Vector2D(-44.66, -1.03);
        poses.at(6) = Vector2D(-43.76, -16.73);
        poses.at(7) = Vector2D(-40.03, -22.27);
        poses.at(8) = Vector2D(-41.18, -7.72);
        poses.at(9) = Vector2D(-38.35, -14.03);
        poses.at(10) = Vector2D(-32.05, -20.85);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 18)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.17, -19.05);
        poses.at(3) = Vector2D(-45.05, -11.20);
        poses.at(4) = Vector2D(-46.33, -23.94);
        poses.at(5) = Vector2D(-44.02, -0.77);
        poses.at(6) = Vector2D(-43.24, -16.09);
        poses.at(7) = Vector2D(-42.21, -19.95);
        poses.at(8) = Vector2D(-41.70, -7.08);
        poses.at(9) = Vector2D(-39.77, -13.00);
        poses.at(10) = Vector2D(-32.95, -19.31);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 19)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.69, -14.80);
        poses.at(3) = Vector2D(-45.43, -7.85);
        poses.at(4) = Vector2D(-46.85, -17.50);
        poses.at(5) = Vector2D(-44.02, -0.77);
        poses.at(6) = Vector2D(-43.63, -13.38);
        poses.at(7) = Vector2D(-41.06, -12.36);
        poses.at(8) = Vector2D(-42.86, -4.63);
        poses.at(9) = Vector2D(-40.54, -8.75);
        poses.at(10) = Vector2D(-32.95, -19.31);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 20)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.17, -13.51);
        poses.at(3) = Vector2D(-43.50, -5.79);
        poses.at(4) = Vector2D(-46.07, -15.19);
        poses.at(5) = Vector2D(-43.37, 3.60);
        poses.at(6) = Vector2D(-43.63, -9.52);
        poses.at(7) = Vector2D(-43.37, -11.58);
        poses.at(8) = Vector2D(-43.24, -1.42);
        poses.at(9) = Vector2D(-39.51, -9.27);
        poses.at(10) = Vector2D(-35.52, -16.34);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 21)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-45.56, -7.21);
        poses.at(3) = Vector2D(-43.50, -5.79);
        poses.at(4) = Vector2D(-45.69, -11.20);
        poses.at(5) = Vector2D(-44.14, 2.83);
        poses.at(6) = Vector2D(-45.56, -4.76);
        poses.at(7) = Vector2D(-44.92, -9.14);
        poses.at(8) = Vector2D(-44.79, -2.32);
        poses.at(9) = Vector2D(-41.06, -6.56);
        poses.at(10) = Vector2D(-37.07, -13.90);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 22)
    {

        poses.at(1) = Vector2D(-50.71, -3.60);
        poses.at(2) = Vector2D(-45.95, -6.95);
        poses.at(3) = Vector2D(-45.05, -2.32);
        poses.at(4) = Vector2D(-46.33, -7.85);
        poses.at(5) = Vector2D(-46.46, 4.63);
        poses.at(6) = Vector2D(-45.05, -3.73);
        poses.at(7) = Vector2D(-45.56, -5.41);
        poses.at(8) = Vector2D(-45.43, 0.77);
        poses.at(9) = Vector2D(-42.34, 0.00);
        poses.at(10) = Vector2D(-37.07, -13.90);
        poses.at(11) = Vector2D(-25.23, 2.32);
    }

    if (count == 23)
    {

        poses.at(1) = Vector2D(-50.71, -1.03);
        poses.at(2) = Vector2D(-45.30, -3.99);
        poses.at(3) = Vector2D(-45.43, -0.51);
        poses.at(4) = Vector2D(-45.17, -7.34);
        poses.at(5) = Vector2D(-45.56, 3.47);
        poses.at(6) = Vector2D(-45.30, -2.32);
        poses.at(7) = Vector2D(-45.30, -5.92);
        poses.at(8) = Vector2D(-45.30, 1.54);
        poses.at(9) = Vector2D(-44.02, -2.45);
        poses.at(10) = Vector2D(-37.07, -13.90);
        poses.at(11) = Vector2D(-29.60, 5.28);
    }

    if (count == 24)
    {

        poses.at(1) = Vector2D(-50.71, -1.03);
        poses.at(2) = Vector2D(-45.17, -0.39);
        poses.at(3) = Vector2D(-46.07, 2.70);
        poses.at(4) = Vector2D(-44.66, -5.02);
        poses.at(5) = Vector2D(-46.07, 6.69);
        poses.at(6) = Vector2D(-45.43, 0.90);
        poses.at(7) = Vector2D(-45.05, -2.19);
        poses.at(8) = Vector2D(-46.33, 4.12);
        poses.at(9) = Vector2D(-42.99, -1.54);
        poses.at(10) = Vector2D(-30.24, -14.16);
        poses.at(11) = Vector2D(-36.94, 7.21);
    }

    if (count == 25)
    {

        poses.at(1) = Vector2D(-50.71, -1.03);
        poses.at(2) = Vector2D(-43.76, 1.67);
        poses.at(3) = Vector2D(-45.30, 3.73);
        poses.at(4) = Vector2D(-41.70, -7.34);
        poses.at(5) = Vector2D(-45.56, 7.08);
        poses.at(6) = Vector2D(-44.40, 3.73);
        poses.at(7) = Vector2D(-43.11, -2.32);
        poses.at(8) = Vector2D(-45.56, 5.53);
        poses.at(9) = Vector2D(-42.34, 1.42);
        poses.at(10) = Vector2D(-30.24, -14.16);
        poses.at(11) = Vector2D(-38.87, 6.95);
    }

    if (count == 26)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-42.73, 5.79);
        poses.at(3) = Vector2D(-44.27, 7.21);
        poses.at(4) = Vector2D(-42.60, -4.12);
        poses.at(5) = Vector2D(-46.20, 10.30);
        poses.at(6) = Vector2D(-43.24, 6.82);
        poses.at(7) = Vector2D(-43.63, 2.32);
        poses.at(8) = Vector2D(-45.30, 8.49);
        poses.at(9) = Vector2D(-37.84, 1.03);
        poses.at(10) = Vector2D(-30.24, -14.16);
        poses.at(11) = Vector2D(-40.03, 6.44);
    }

    if (count == 27)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-43.24, 5.02);
        poses.at(3) = Vector2D(-43.63, 10.04);
        poses.at(4) = Vector2D(-42.60, -4.12);
        poses.at(5) = Vector2D(-44.79, 13.90);
        poses.at(6) = Vector2D(-42.47, 8.62);
        poses.at(7) = Vector2D(-39.64, 1.54);
        poses.at(8) = Vector2D(-44.40, 11.71);
        poses.at(9) = Vector2D(-35.52, 4.25);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-39.00, 8.88);
    }

    if (count == 28)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-43.24, 5.02);
        poses.at(3) = Vector2D(-42.60, 11.71);
        poses.at(4) = Vector2D(-42.60, -4.12);
        poses.at(5) = Vector2D(-44.27, 16.73);
        poses.at(6) = Vector2D(-40.15, 7.59);
        poses.at(7) = Vector2D(-39.64, 1.54);
        poses.at(8) = Vector2D(-42.60, 15.19);
        poses.at(9) = Vector2D(-35.52, 4.25);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-36.42, 14.16);
    }

    if (count == 29)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-44.02, 7.21);
        poses.at(3) = Vector2D(-43.37, 14.80);
        poses.at(4) = Vector2D(-42.60, -4.12);
        poses.at(5) = Vector2D(-45.56, 24.07);
        poses.at(6) = Vector2D(-40.93, 11.20);
        poses.at(7) = Vector2D(-39.90, 3.35);
        poses.at(8) = Vector2D(-43.37, 20.33);
        poses.at(9) = Vector2D(-35.78, 12.48);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-32.69, 18.66);
    }

    if (count == 30)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-44.27, 9.27);
        poses.at(3) = Vector2D(-44.14, 18.28);
        poses.at(4) = Vector2D(-44.14, -2.32);
        poses.at(5) = Vector2D(-45.30, 27.03);
        poses.at(6) = Vector2D(-40.28, 14.29);
        poses.at(7) = Vector2D(-39.90, 3.35);
        poses.at(8) = Vector2D(-42.60, 23.29);
        poses.at(9) = Vector2D(-32.43, 14.67);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-32.30, 23.94);
    }

    if (count == 31)
    {

        poses.at(1) = Vector2D(-50.58, 4.12);
        poses.at(2) = Vector2D(-44.02, 11.33);
        poses.at(3) = Vector2D(-44.53, 20.33);
        poses.at(4) = Vector2D(-44.14, -2.32);
        poses.at(5) = Vector2D(-45.05, 29.34);
        poses.at(6) = Vector2D(-40.93, 17.63);
        poses.at(7) = Vector2D(-38.61, 7.34);
        poses.at(8) = Vector2D(-42.08, 25.48);
        poses.at(9) = Vector2D(-35.14, 16.09);
        poses.at(10) = Vector2D(-25.35, -12.61);
        poses.at(11) = Vector2D(-33.20, 26.25);
    }

    if (count == 32)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-39.25, -20.08);
        poses.at(3) = Vector2D(-39.25, -10.68);
        poses.at(4) = Vector2D(-39.12, -30.24);
        poses.at(5) = Vector2D(-39.38, 2.57);
        poses.at(6) = Vector2D(-35.78, -15.32);
        poses.at(7) = Vector2D(-36.55, -25.87);
        poses.at(8) = Vector2D(-35.14, -5.02);
        poses.at(9) = Vector2D(-29.86, -12.10);
        poses.at(10) = Vector2D(-31.53, -24.32);
        poses.at(11) = Vector2D(-23.68, 6.31);
    }

    if (count == 33)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-39.00, -19.18);
        poses.at(3) = Vector2D(-39.25, -10.68);
        poses.at(4) = Vector2D(-38.61, -27.41);
        poses.at(5) = Vector2D(-39.38, 2.57);
        poses.at(6) = Vector2D(-35.26, -16.60);
        poses.at(7) = Vector2D(-36.68, -22.52);
        poses.at(8) = Vector2D(-36.42, -5.53);
        poses.at(9) = Vector2D(-29.86, -12.10);
        poses.at(10) = Vector2D(-31.79, -22.39);
        poses.at(11) = Vector2D(-23.68, 6.31);
    }

    if (count == 34)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-39.25, -15.32);
        poses.at(3) = Vector2D(-39.64, -8.62);
        poses.at(4) = Vector2D(-39.38, -23.29);
        poses.at(5) = Vector2D(-38.87, 3.47);
        poses.at(6) = Vector2D(-36.81, -12.10);
        poses.at(7) = Vector2D(-37.58, -19.69);
        poses.at(8) = Vector2D(-36.04, -3.47);
        poses.at(9) = Vector2D(-31.66, -11.84);
        poses.at(10) = Vector2D(-30.12, -17.12);
        poses.at(11) = Vector2D(-23.68, 6.31);
    }

    if (count == 35)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-39.77, -11.97);
        poses.at(3) = Vector2D(-39.25, -4.76);
        poses.at(4) = Vector2D(-39.90, -18.79);
        poses.at(5) = Vector2D(-38.87, 3.47);
        poses.at(6) = Vector2D(-37.84, -8.62);
        poses.at(7) = Vector2D(-36.55, -14.80);
        poses.at(8) = Vector2D(-36.04, -3.47);
        poses.at(9) = Vector2D(-32.30, -9.65);
        poses.at(10) = Vector2D(-29.99, -14.80);
        poses.at(11) = Vector2D(-23.68, 6.31);
    }

    if (count == 36)
    {

        poses.at(1) = Vector2D(-49.03, -5.66);
        poses.at(2) = Vector2D(-39.77, -10.42);
        poses.at(3) = Vector2D(-39.64, -5.02);
        poses.at(4) = Vector2D(-39.77, -16.60);
        poses.at(5) = Vector2D(-38.87, 3.47);
        poses.at(6) = Vector2D(-37.71, -7.46);
        poses.at(7) = Vector2D(-37.19, -11.20);
        poses.at(8) = Vector2D(-37.45, -1.67);
        poses.at(9) = Vector2D(-35.01, -6.95);
        poses.at(10) = Vector2D(-26.00, -18.53);
        poses.at(11) = Vector2D(-26.51, 4.25);
    }

    if (count == 37)
    {

        poses.at(1) = Vector2D(-49.03, -5.66);
        poses.at(2) = Vector2D(-40.03, -8.11);
        poses.at(3) = Vector2D(-39.51, -2.96);
        poses.at(4) = Vector2D(-39.64, -13.38);
        poses.at(5) = Vector2D(-39.51, 3.99);
        poses.at(6) = Vector2D(-38.22, -5.41);
        poses.at(7) = Vector2D(-38.22, -9.65);
        poses.at(8) = Vector2D(-37.32, -0.51);
        poses.at(9) = Vector2D(-36.68, -7.08);
        poses.at(10) = Vector2D(-26.00, -18.53);
        poses.at(11) = Vector2D(-28.70, 3.22);
    }

    if (count == 38)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.90, -3.73);
        poses.at(3) = Vector2D(-39.90, 0.00);
        poses.at(4) = Vector2D(-40.54, -9.78);
        poses.at(5) = Vector2D(-39.90, 6.05);
        poses.at(6) = Vector2D(-37.32, -1.29);
        poses.at(7) = Vector2D(-38.48, -6.56);
        poses.at(8) = Vector2D(-38.87, 2.96);
        poses.at(9) = Vector2D(-36.94, -2.06);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-32.56, 1.54);
    }

    if (count == 39)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.90, -2.06);
        poses.at(3) = Vector2D(-40.54, 2.19);
        poses.at(4) = Vector2D(-40.54, -7.72);
        poses.at(5) = Vector2D(-40.80, 6.56);
        poses.at(6) = Vector2D(-37.58, 1.80);
        poses.at(7) = Vector2D(-37.84, -4.12);
        poses.at(8) = Vector2D(-37.84, 4.25);
        poses.at(9) = Vector2D(-37.19, -1.03);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-32.69, 5.53);
    }

    if (count == 40)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.64, 0.26);
        poses.at(3) = Vector2D(-39.51, 5.41);
        poses.at(4) = Vector2D(-39.25, -5.79);
        poses.at(5) = Vector2D(-39.64, 11.58);
        poses.at(6) = Vector2D(-36.81, 4.63);
        poses.at(7) = Vector2D(-37.19, -1.42);
        poses.at(8) = Vector2D(-38.74, 7.59);
        poses.at(9) = Vector2D(-36.42, 1.54);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-31.79, 10.94);
    }

    if (count == 41)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.77, 2.96);
        poses.at(3) = Vector2D(-39.38, 8.88);
        poses.at(4) = Vector2D(-39.25, -5.79);
        poses.at(5) = Vector2D(-40.03, 14.93);
        poses.at(6) = Vector2D(-38.10, 7.21);
        poses.at(7) = Vector2D(-36.55, 1.93);
        poses.at(8) = Vector2D(-37.71, 11.71);
        poses.at(9) = Vector2D(-35.52, 6.44);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-31.79, 10.94);
    }

    if (count == 42)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-39.64, 4.25);
        poses.at(3) = Vector2D(-39.90, 11.58);
        poses.at(4) = Vector2D(-38.61, -4.12);
        poses.at(5) = Vector2D(-39.77, 15.83);
        poses.at(6) = Vector2D(-38.10, 7.98);
        poses.at(7) = Vector2D(-37.45, 0.90);
        poses.at(8) = Vector2D(-37.19, 12.87);
        poses.at(9) = Vector2D(-35.26, 9.27);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-32.05, 10.42);
    }

    if (count == 43)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-38.87, 7.85);
        poses.at(3) = Vector2D(-38.87, 13.26);
        poses.at(4) = Vector2D(-38.74, -1.80);
        poses.at(5) = Vector2D(-39.25, 22.27);
        poses.at(6) = Vector2D(-35.14, 12.10);
        poses.at(7) = Vector2D(-34.75, 4.25);
        poses.at(8) = Vector2D(-36.81, 16.60);
        poses.at(9) = Vector2D(-32.82, 9.01);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-30.76, 15.06);
    }

    if (count == 44)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-38.87, 7.85);
        poses.at(3) = Vector2D(-39.25, 17.63);
        poses.at(4) = Vector2D(-38.74, -4.38);
        poses.at(5) = Vector2D(-38.87, 25.35);
        poses.at(6) = Vector2D(-36.42, 14.41);
        poses.at(7) = Vector2D(-34.75, 4.25);
        poses.at(8) = Vector2D(-37.32, 20.59);
        poses.at(9) = Vector2D(-32.43, 15.44);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-27.80, 22.27);
    }

    if (count == 45)
    {

        poses.at(1) = Vector2D(-48.91, -1.16);
        poses.at(2) = Vector2D(-40.15, 10.30);
        poses.at(3) = Vector2D(-39.51, 20.98);
        poses.at(4) = Vector2D(-39.25, -4.50);
        poses.at(5) = Vector2D(-39.90, 29.21);
        poses.at(6) = Vector2D(-36.68, 16.09);
        poses.at(7) = Vector2D(-36.42, 5.53);
        poses.at(8) = Vector2D(-37.45, 24.71);
        poses.at(9) = Vector2D(-32.43, 15.44);
        poses.at(10) = Vector2D(-26.38, -17.37);
        poses.at(11) = Vector2D(-27.54, 23.94);
    }

    if (count == 46)
    {

        poses.at(1) = Vector2D(-46.69, -7.44);
        poses.at(2) = Vector2D(-32.56, -19.18);
        poses.at(3) = Vector2D(-32.18, -9.14);
        poses.at(4) = Vector2D(-32.69, -29.73);
        poses.at(5) = Vector2D(-32.43, 3.99);
        poses.at(6) = Vector2D(-30.24, -15.19);
        poses.at(7) = Vector2D(-31.02, -25.10);
        poses.at(8) = Vector2D(-27.03, -5.41);
        poses.at(9) = Vector2D(-27.54, -23.04);
        poses.at(10) = Vector2D(-22.78, -25.23);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 47)
    {

        poses.at(1) = Vector2D(-46.82, -7.70);
        poses.at(2) = Vector2D(-32.30, -19.56);
        poses.at(3) = Vector2D(-31.79, -7.59);
        poses.at(4) = Vector2D(-32.56, -27.03);
        poses.at(5) = Vector2D(-32.43, 3.99);
        poses.at(6) = Vector2D(-29.73, -13.26);
        poses.at(7) = Vector2D(-30.50, -23.29);
        poses.at(8) = Vector2D(-27.67, -3.73);
        poses.at(9) = Vector2D(-26.64, -19.18);
        poses.at(10) = Vector2D(-22.78, -25.23);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 48)
    {

        poses.at(1) = Vector2D(-47.20, -6.17);
        poses.at(2) = Vector2D(-31.79, -14.29);
        poses.at(3) = Vector2D(-31.92, -5.15);
        poses.at(4) = Vector2D(-31.79, -22.78);
        poses.at(5) = Vector2D(-32.18, 5.15);
        poses.at(6) = Vector2D(-29.47, -10.42);
        poses.at(7) = Vector2D(-30.50, -18.66);
        poses.at(8) = Vector2D(-27.80, -1.93);
        poses.at(9) = Vector2D(-24.58, -11.58);
        poses.at(10) = Vector2D(-22.14, -22.39);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 49)
    {

        poses.at(1) = Vector2D(-47.84, -5.79);
        poses.at(2) = Vector2D(-31.79, -11.58);
        poses.at(3) = Vector2D(-32.30, -1.67);
        poses.at(4) = Vector2D(-32.18, -19.56);
        poses.at(5) = Vector2D(-32.05, 9.27);
        poses.at(6) = Vector2D(-29.47, -7.21);
        poses.at(7) = Vector2D(-29.99, -15.70);
        poses.at(8) = Vector2D(-28.31, 1.80);
        poses.at(9) = Vector2D(-26.90, -12.10);
        poses.at(10) = Vector2D(-20.98, -18.15);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 50)
    {

        poses.at(1) = Vector2D(-50.00, -4.52);
        poses.at(2) = Vector2D(-32.95, -8.37);
        poses.at(3) = Vector2D(-32.56, -1.29);
        poses.at(4) = Vector2D(-32.95, -17.12);
        poses.at(5) = Vector2D(-32.82, 7.98);
        poses.at(6) = Vector2D(-31.02, -6.44);
        poses.at(7) = Vector2D(-30.50, -11.45);
        poses.at(8) = Vector2D(-28.31, 1.80);
        poses.at(9) = Vector2D(-27.54, -5.02);
        poses.at(10) = Vector2D(-23.29, -17.89);
        poses.at(11) = Vector2D(-17.89, 5.15);
    }

    if (count == 51)
    {

        poses.at(1) = Vector2D(-48.22, -0.45);
        poses.at(2) = Vector2D(-34.08, -6.61);
        poses.at(3) = Vector2D(-34.01, 1.68);
        poses.at(4) = Vector2D(-34.57, -16.74);
        poses.at(5) = Vector2D(-34.24, 10.45);
        poses.at(6) = Vector2D(-32.32, -2.62);
        poses.at(7) = Vector2D(-31.19, -11.32);
        poses.at(8) = Vector2D(-30.94, 4.82);
        poses.at(9) = Vector2D(-29.92, -3.60);
        poses.at(10) = Vector2D(-25.29, -16.39);
        poses.at(11) = Vector2D(-23.71, 6.80);
    }

    if (count == 52)
    {

        poses.at(1) = Vector2D(-48.35, -0.19);
        poses.at(2) = Vector2D(-33.72, -6.18);
        poses.at(3) = Vector2D(-33.72, 2.32);
        poses.at(4) = Vector2D(-34.36, -16.99);
        poses.at(5) = Vector2D(-33.98, 11.45);
        poses.at(6) = Vector2D(-31.92, -1.93);
        poses.at(7) = Vector2D(-30.76, -11.33);
        poses.at(8) = Vector2D(-30.63, 5.66);
        poses.at(9) = Vector2D(-29.47, -3.09);
        poses.at(10) = Vector2D(-25.23, -16.22);
        poses.at(11) = Vector2D(-23.68, 7.08);
    }

    if (count == 53)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-34.22, -2.86);
        poses.at(3) = Vector2D(-34.10, 5.92);
        poses.at(4) = Vector2D(-34.61, -11.90);
        poses.at(5) = Vector2D(-33.97, 12.66);
        poses.at(6) = Vector2D(-33.21, 1.97);
        poses.at(7) = Vector2D(-29.52, -5.79);
        poses.at(8) = Vector2D(-31.04, 8.21);
        poses.at(9) = Vector2D(-30.03, 3.12);
        poses.at(10) = Vector2D(-25.23, -16.22);
        poses.at(11) = Vector2D(-24.58, 9.40);
    }

    if (count == 54)
    {

        poses.at(1) = Vector2D(-47.46, 3.50);
        poses.at(2) = Vector2D(-33.33, -3.50);
        poses.at(3) = Vector2D(-32.44, 7.70);
        poses.at(4) = Vector2D(-33.33, -15.46);
        poses.at(5) = Vector2D(-32.32, 16.35);
        poses.at(6) = Vector2D(-31.40, 1.42);
        poses.at(7) = Vector2D(-28.63, -6.42);
        poses.at(8) = Vector2D(-29.64, 12.02);
        poses.at(9) = Vector2D(-28.75, 4.90);
        poses.at(10) = Vector2D(-25.23, -16.22);
        poses.at(11) = Vector2D(-24.58, 9.40);
    }

    if (count == 55)
    {

        poses.at(1) = Vector2D(-47.96, 5.79);
        poses.at(2) = Vector2D(-33.59, -0.19);
        poses.at(3) = Vector2D(-33.08, 10.11);
        poses.at(4) = Vector2D(-34.48, -14.19);
        poses.at(5) = Vector2D(-33.21, 20.67);
        poses.at(6) = Vector2D(-30.28, 4.39);
        poses.at(7) = Vector2D(-29.90, -5.66);
        poses.at(8) = Vector2D(-29.77, 15.84);
        poses.at(9) = Vector2D(-28.75, 10.11);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-21.88, 9.91);
    }

    if (count == 56)
    {

        poses.at(1) = Vector2D(-48.85, 6.42);
        poses.at(2) = Vector2D(-32.19, 1.08);
        poses.at(3) = Vector2D(-31.81, 11.51);
        poses.at(4) = Vector2D(-32.70, -12.66);
        poses.at(5) = Vector2D(-32.06, 20.42);
        poses.at(6) = Vector2D(-29.39, 5.92);
        poses.at(7) = Vector2D(-27.86, -4.39);
        poses.at(8) = Vector2D(-29.52, 15.20);
        poses.at(9) = Vector2D(-25.57, 7.44);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-21.76, 19.15);
    }

    if (count == 57)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-33.20, 3.60);
        poses.at(3) = Vector2D(-33.08, 14.16);
        poses.at(4) = Vector2D(-33.72, -9.78);
        poses.at(5) = Vector2D(-32.82, 24.45);
        poses.at(6) = Vector2D(-29.21, 8.37);
        poses.at(7) = Vector2D(-28.31, -2.45);
        poses.at(8) = Vector2D(-30.89, 19.43);
        poses.at(9) = Vector2D(-27.28, 15.06);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-21.75, 20.46);
    }

    if (count == 58)
    {

        poses.at(1) = Vector2D(-50.45, -6.56);
        poses.at(2) = Vector2D(-33.33, 7.46);
        poses.at(3) = Vector2D(-33.46, 18.79);
        poses.at(4) = Vector2D(-32.95, -6.69);
        poses.at(5) = Vector2D(-33.20, 28.70);
        poses.at(6) = Vector2D(-30.63, 13.26);
        poses.at(7) = Vector2D(-28.19, 1.54);
        poses.at(8) = Vector2D(-29.86, 23.04);
        poses.at(9) = Vector2D(-26.64, 17.76);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-22.14, 24.84);
    }

    if (count == 59)
    {

        poses.at(1) = Vector2D(-47.49, 9.27);
        poses.at(2) = Vector2D(-34.11, 9.78);
        poses.at(3) = Vector2D(-33.33, 21.49);
        poses.at(4) = Vector2D(-32.95, -6.69);
        poses.at(5) = Vector2D(-33.46, 31.15);
        poses.at(6) = Vector2D(-29.73, 16.09);
        poses.at(7) = Vector2D(-27.67, 3.73);
        poses.at(8) = Vector2D(-29.47, 25.23);
        poses.at(9) = Vector2D(-26.13, 20.08);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-20.85, 29.47);
    }

    if (count == 60)
    {

        poses.at(1) = Vector2D(-47.49, 9.27);
        poses.at(2) = Vector2D(-33.46, 12.10);
        poses.at(3) = Vector2D(-33.08, 23.81);
        poses.at(4) = Vector2D(-33.59, -4.50);
        poses.at(5) = Vector2D(-33.20, 32.69);
        poses.at(6) = Vector2D(-29.60, 18.02);
        poses.at(7) = Vector2D(-28.57, 6.95);
        poses.at(8) = Vector2D(-29.86, 27.93);
        poses.at(9) = Vector2D(-26.77, 22.39);
        poses.at(10) = Vector2D(-20.08, -12.87);
        poses.at(11) = Vector2D(-20.85, 29.47);
    }
    return poses.at(agent->world().self().unum());
}

int Strategy::area(rcsc::PlayerAgent *agent)
{
    const WorldModel &wm = agent->world();
    Vector2D ball_pos = wm.ball().pos();
    std::vector<Vector2D> poses;
    poses.push_back(Vector2D(-48.52, -28.96));
    poses.push_back(Vector2D(-47.75, -25.87));
    poses.push_back(Vector2D(-48.26, -21.88));
    poses.push_back(Vector2D(-48.13, -18.40));
    poses.push_back(Vector2D(-48.22, -15.20));
    poses.push_back(Vector2D(-48.52, -11.33));
    poses.push_back(Vector2D(-48.78, -8.75));
    poses.push_back(Vector2D(-49.03, -5.66));
    poses.push_back(Vector2D(-48.78, -2.06));
    poses.push_back(Vector2D(-48.65, 1.80));
    poses.push_back(Vector2D(-48.78, 6.18));
    poses.push_back(Vector2D(-48.78, 10.94));
    poses.push_back(Vector2D(-49.03, 15.06));
    poses.push_back(Vector2D(-48.78, 19.69));
    poses.push_back(Vector2D(-48.39, 23.55));
    poses.push_back(Vector2D(-48.91, 27.54));
    poses.push_back(Vector2D(-49.03, 30.89));
    poses.push_back(Vector2D(-42.21, -29.34));
    poses.push_back(Vector2D(-42.34, -24.07));
    poses.push_back(Vector2D(-41.96, -20.21));
    poses.push_back(Vector2D(-41.83, -15.83));
    poses.push_back(Vector2D(-41.70, -11.20));
    poses.push_back(Vector2D(-41.57, -7.21));
    poses.push_back(Vector2D(-41.57, -2.70));
    poses.push_back(Vector2D(-41.70, 1.54));
    poses.push_back(Vector2D(-41.57, 5.79));
    poses.push_back(Vector2D(-41.57, 10.55));
    poses.push_back(Vector2D(-41.44, 14.80));
    poses.push_back(Vector2D(-41.44, 19.43));
    poses.push_back(Vector2D(-41.44, 24.07));
    poses.push_back(Vector2D(-41.44, 28.19));
    poses.push_back(Vector2D(-41.44, 32.05));
    poses.push_back(Vector2D(-34.23, -30.37));
    poses.push_back(Vector2D(-34.11, -26.51));
    poses.push_back(Vector2D(-33.98, -21.49));
    poses.push_back(Vector2D(-33.85, -16.86));
    poses.push_back(Vector2D(-33.98, -12.36));
    poses.push_back(Vector2D(-33.72, -7.46));
    poses.push_back(Vector2D(-33.85, -2.70));
    poses.push_back(Vector2D(-33.85, 1.80));
    poses.push_back(Vector2D(-33.46, 6.82));
    poses.push_back(Vector2D(-33.98, 11.20));
    poses.push_back(Vector2D(-34.11, 15.44));
    poses.push_back(Vector2D(-34.36, 19.56));
    poses.push_back(Vector2D(-34.62, 23.94));
    poses.push_back(Vector2D(-35.01, 28.19));
    poses.push_back(Vector2D(-27.93, -30.50));
    poses.push_back(Vector2D(-27.67, -26.00));
    poses.push_back(Vector2D(-27.67, -20.72));
    poses.push_back(Vector2D(-27.54, -16.73));
    poses.push_back(Vector2D(-27.03, -11.58));
    poses.push_back(Vector2D(-27.61, -7.00));
    poses.push_back(Vector2D(-27.16, -2.32));
    poses.push_back(Vector2D(-26.51, 2.96));
    poses.push_back(Vector2D(-26.64, 7.59));
    poses.push_back(Vector2D(-26.38, 12.36));
    poses.push_back(Vector2D(-26.13, 16.00));
    poses.push_back(Vector2D(-26.77, 20.72));
    poses.push_back(Vector2D(-26.77, 25.23));
    poses.push_back(Vector2D(-27.16, 28.70));
    poses.push_back(Vector2D(-27.28, 32.82));

    Vector2D nearest = poses[0];
    int count = 0;
    for (int i = 1; i < poses.size(); i++)
    {
        if (ball_pos.dist(poses[i]) < ball_pos.dist(nearest))
        {
            nearest = poses[i];
            count = i;
        }
    }
    return count;
}









///////////Some Functions Removed
