// -*-c++-*-

/*

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

#ifndef BHV_BASIC_MOVE_H
#define BHV_BASIC_MOVE_H

#include <rcsc/geom/vector_2d.h>
#include <rcsc/player/soccer_action.h>
#include <rcsc/player/player_agent.h>

class Bhv_BasicMove
    : public rcsc::SoccerBehavior {
public:
    Bhv_BasicMove()
      { }

    bool execute( rcsc::PlayerAgent * agent );
    rcsc::Vector2D getPosition(const rcsc::WorldModel & wm, int unum);
    double get_normal_dash_power( const rcsc::WorldModel & wm );
    bool self_interceptor (const rcsc::PlayerAgent *agent);
    bool press(rcsc::PlayerAgent *agent);
    rcsc::Vector2D last_difense(const rcsc::PlayerAgent *agent);
    rcsc::Vector2D full_press(const rcsc::WorldModel &wm);
    bool s_e_intersept(rcsc::PlayerAgent *agent);
    bool cut_OPPs_pass_path(rcsc::PlayerAgent *agent);
    bool BodyGoToPassablePos(rcsc::PlayerAgent *agent);
    bool self_tacklatoration(rcsc::PlayerAgent *agent);
    int return_pos(const rcsc::PlayerAgent *agent);
    rcsc::Vector2D deffense_poses(const rcsc::PlayerAgent *agent);
    bool GoForeHelp(rcsc::PlayerAgent *agent);
    bool Scape(rcsc::PlayerAgent *agnet);
    rcsc::Vector2D DeffenseFormation(rcsc::PlayerAgent *agent, int self_unum);
    rcsc::Vector2D GetHybridDeffenseFormation(rcsc::PlayerAgent *agent);
    rcsc::Vector2D OffensiveFormation(rcsc::PlayerAgent *agent, int unum);
    rcsc::Vector2D OffensiveSTRG2(rcsc::PlayerAgent *agent, int unum);
    int dash(rcsc::PlayerAgent *agent, rcsc::Vector2D point);
    int area(rcsc::PlayerAgent *agent);
    rcsc::Vector2D CornerForamtion(rcsc::PlayerAgent *);
private:
    double getDashPower( const rcsc::PlayerAgent * agent );
};

class Strategy
{
private:
    int area(rcsc::PlayerAgent *agent);
    rcsc::Vector2D GetHybridDeffenseFormation(rcsc::PlayerAgent *agent);
    rcsc::Vector2D DefenseFormation(rcsc::PlayerAgent *agent, int self_unum);
    rcsc::Vector2D OffenseStrg1(rcsc::PlayerAgent *agent, int self_unum);
    rcsc::Vector2D OffenseStrg2(rcsc::PlayerAgent *agent, int unum);
public:
    enum StrategyType{STRATEGY1, STRATEGY2};
    rcsc::Vector2D Get_Formation(rcsc::PlayerAgent *agent, int Unum, Strategy::StrategyType);
    Strategy::StrategyType Get_RecommendedStragetyType(rcsc::PlayerAgent *agent);
};

#endif
