// -*-c++-*-

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

/////////////////////////////////////////////////////////////////////


#pragma once

#include <rcsc/player/soccer_action.h>
#include <rcsc/geom/vector_2d.h>
#include <rcsc/geom/vector_2d.h>
#include <rcsc/player/soccer_action.h>
#include <rcsc/player/player_agent.h>
#include <rcsc/common/server_param.h>
#include <rcsc/rcg/types.h>
#include <rcsc/rcg/handler.h>
using namespace rcsc;
class Bhv_BasicOffensiveKick
    : public rcsc::SoccerBehavior {
private:

public:
    static Vector2D Dribble;
    bool execute( rcsc::PlayerAgent * agent );
    bool shoot( rcsc::PlayerAgent * agent );
    bool pass_to_forward( rcsc::PlayerAgent * agent );
    bool pass( rcsc::PlayerAgent * agent );
    bool dribble( rcsc::PlayerAgent * agent );
    rcsc::Vector2D dribble_pos ( rcsc::PlayerAgent * agent );
    bool clearball( rcsc::PlayerAgent * agent );
    bool PassAttack(rcsc::PlayerAgent *agent);
    bool is_PassAttackAble(rcsc::PlayerAgent *agent, rcsc::Vector2D Sender, rcsc::Vector2D Reciever, rcsc::Vector2D PassPos);
    double pass_attack_power(rcsc::PlayerAgent *agent,rcsc::Vector2D player_pos,rcsc::Vector2D target_pos);
    bool GO(const rcsc::PlayerAgent *agent);
    float pass_power(rcsc::PlayerAgent *agent,rcsc::Vector2D tm_vector);
    double dribble_power (rcsc::PlayerAgent *agent,rcsc::Vector2D vector_kick);
    bool clear_sector(rcsc::PlayerAgent *agent);
    rcsc::Vector2D Density(rcsc::PlayerAgent *agent,bool less);
    bool pass_fazayy( rcsc::PlayerAgent *agent);
    rcsc::Vector2D pass_faza(rcsc::PlayerAgent *agent);
    int faza_unum (rcsc::PlayerAgent *agent);
    bool GoalPass(rcsc::PlayerAgent *agent);
    bool CheckGoalPath(rcsc::PlayerAgent *agent, rcsc::Vector2D Destinaion);
    rcsc::Vector2D  Density(rcsc::PlayerAgent *agent );
    bool MakeDecision(rcsc::PlayerAgent *agent);
    int AnalyzeGame(rcsc::PlayerAgent * agent );
     bool ShootPower(rcsc::PlayerAgent *agent, rcsc::Vector2D Target);
    int MatchResults(rcsc::PlayerAgent *agent);
    bool is_Passable(rcsc::PlayerAgent *agent, rcsc::Vector2D, rcsc::Vector2D);   
    rcsc::Vector2D recursive_fu_shoot(rcsc::Vector2D ball_pos,rcsc::Vector2D left, rcsc::Vector2D right,int counter, const rcsc::WorldModel &wm , bool sideUP);
    bool main_shoot (rcsc::PlayerAgent *agent);
};

class Saver 
{
    public:
        bool do_SaveSmartKick(rcsc::PlayerAgent *agent, Vector2D Target, double power, int Cycle);
};

class ChainAction
{
private:
public:
    struct TreeStack
    {
        public:
        int Tree [11][11][11];
    };
    /// @brief 
    /// @param agent 
    /// @return
    TreeStack Get_Tree(rcsc::PlayerAgent *agent);
};
