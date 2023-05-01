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

#ifndef SAMPLE_PLAYER_H
#define SAMPLE_PLAYER_H

#include "communication.h"

#include <rcsc/player/player_agent.h>
#include <vector>
#include <rcsc/rcg/types.h>
#include <rcsc/rcg/handler.h>




class SamplePlayer
    : public rcsc::PlayerAgent {
private:
     std::string M_left_team_name;
    std::string M_right_team_name;
    int M_left_score;
    int M_right_score;
    Communication::Ptr M_communication;

public:

    SamplePlayer();
    virtual
    ~SamplePlayer();

    //! handle log version info
    virtual
    bool handleLogVersion( const int ver )
      {
          return true;
      }

    //! return log version
    virtual
    int logVersion() const
      {
          return 0;
      }

    //! handle dist info
    virtual
    bool handleDispInfo( const rcsc::rcg::dispinfo_t& info )
      {
          return true;
      }

    //! handle show info
    virtual
    bool handleShowInfo( const rcsc::rcg::showinfo_t& info )
      {
          return true;
      }

    //! handle show info 2
    virtual
    bool handleShowInfo2( const rcsc::rcg::showinfo_t2& info )
      {
          return true;
      }

    //! handle short show info 2
    virtual
    bool handleShortShowInfo2( const rcsc::rcg::short_showinfo_t2& info )
      {
          return true;
      }

    //! handle msg info
    virtual
    bool handleMsgInfo( short board,
                        const std::string& msg )
      {
          return true;
      }

    //! handle playmode info
    virtual
    bool handlePlayMode( char playmode )
      {
          return true;
      }

    //! handle team info
   // virtual
    //bool handleTeamInfo( const rcsc::rcg::team_t& team_left,
                //         const rcsc::rcg::team_t& team_right );

    //! handle player type
    virtual
    bool handlePlayerType( const rcsc::rcg::player_type_t& type )
      {
          return true;
      }

    //! handle server param
    virtual
    bool handleServerParam( const rcsc::rcg::server_params_t& param )
      {
          return true;
      }

    //! handle player param
    virtual
    bool handlePlayerParam( const rcsc::rcg::player_params_t& param )
      {
          return true;
      }

    //! handle the end of file
    //virtual bool handleEOF();

protected:

    /*!
      You can override this method.
      But you must call PlayerAgent::initImpl() in this method.
    */
    virtual
    bool initImpl( rcsc::CmdLineParser & cmd_parser );

    //! main decision
    virtual
    void actionImpl();

    //! communication decision
    virtual
    void communicationImpl();

    virtual
    void handleActionStart();
    virtual
    void handleActionEnd();

    virtual
    void handleServerParam();
    virtual
    void handlePlayerParam();
    virtual
    void handlePlayerType();

private:

    bool doPreprocess();
    bool doheareddribblerecive;
    bool gocatchball();
    bool doShoot();
    bool doForceKick();
    bool doHeardPassReceive();
    bool goforfaza(rcsc::PlayerAgent*agent);
public:
};

#endif
