// -*-c++-*-
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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
#include "bhv_goalie_free_kick.h"
#include <rcsc/action/body_hold_ball.h>
#include <rcsc/action/body_turn_to_ball.h>
#include <rcsc/rcg/types.h>
#include <rcsc/rcg/util.h>
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
SamplePlayer::SamplePlayer()
: PlayerAgent(),
M_communication()
{
    boost::shared_ptr< AudioMemory > audio_memory( new AudioMemory );

    M_worldmodel.setAudioMemory( audio_memory );

    //
    // set communication message parser
    //
    addSayMessageParser( SayMessageParser::Ptr( new BallMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new PassMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new InterceptMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new GoalieMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new GoalieAndPlayerMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new OffsideLineMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new DefenseLineMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new WaitRequestMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new PassRequestMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new DribbleMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new BallGoalieMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new OnePlayerMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new TwoPlayerMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new ThreePlayerMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new SelfMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new TeammateMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new OpponentMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new BallPlayerMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new StaminaMessageParser( audio_memory ) ) );
    addSayMessageParser( SayMessageParser::Ptr( new RecoveryMessageParser( audio_memory ) ) );

    // addSayMessageParser( SayMessageParser::Ptr( new FreeMessageParser< 9 >( audio_memory ) ) );
    // addSayMessageParser( SayMessageParser::Ptr( new FreeMessageParser< 8 >( audio_memory ) ) );
    // addSayMessageParser( SayMessageParser::Ptr( new FreeMessageParser< 7 >( audio_memory ) ) );
    // addSayMessageParser( SayMessageParser::Ptr( new FreeMessageParser< 6 >( audio_memory ) ) );
    // addSayMessageParser( SayMessageParser::Ptr( new FreeMessageParser< 5 >( audio_memory ) ) );
    // addSayMessageParser( SayMessageParser::Ptr( new FreeMessageParser< 4 >( audio_memory ) ) );
    // addSayMessageParser( SayMessageParser::Ptr( new FreeMessageParser< 3 >( audio_memory ) ) );
    // addSayMessageParser( SayMessageParser::Ptr( new FreeMessageParser< 2 >( audio_memory ) ) );
    // addSayMessageParser( SayMessageParser::Ptr( new FreeMessageParser< 1 >( audio_memory ) ) );

    //
    // set freeform message parser
    //
    setFreeformParser( FreeformParser::Ptr( new FreeformParser( M_worldmodel ) ) );

    //
    // set action generators
    //
    // M_action_generators.push_back( ActionGenerator::Ptr( new PassGenerator() ) );

    //
    // set communication planner
    //
    M_communication = Communication::Ptr( new SampleCommunication() );
}

/*-------------------------------------------------------------------*/
/*!

 */
SamplePlayer::~SamplePlayer()
{

}

/*-------------------------------------------------------------------*/
/*!

 */
bool
SamplePlayer::initImpl( CmdLineParser & cmd_parser )
{
    bool result = PlayerAgent::initImpl( cmd_parser );

    rcsc::ParamMap my_params( "Additional options" );
#if 0
    std::string param_file_path = "params";
    param_map.add()
    ( "param-file", "", &param_file_path, "specified parameter file" );
#endif

    cmd_parser.parse( my_params );

    if ( cmd_parser.count( "help" ) > 0 )
    {
        my_params.printHelp( std::cout );
        return false;
    }

    if ( cmd_parser.failed() )
    {
        std::cerr << "player: ***WARNING*** detected unsuppprted options: ";
        cmd_parser.print( std::cerr );
        std::cerr << std::endl;
    }

    if ( ! result )
    {
        return false;
    }

    if ( KickTable::instance().read( config().configDir() + "/kick-table" ) )
    {
        std::cerr << "Loaded the kick table: ["
        << config().configDir() << "/kick-table]"
        << std::endl;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!
  main decision
  virtual method in super class
 */

void
SamplePlayer::actionImpl()
{
    /*if(){
        std::cout<<"  LEFT     =       "<<M_left_team_name<<" ->    SCORE = "<<M_left_score<<std::endl;
        std::cout<< "        RIGHT         =      "<<M_right_team_name<<" ->    SCORE = "<<M_right_score;
    }*/
    static int ccccc = 0;
     const WorldModel & wm = this->world();
    if(this->world().gameMode().type() == GameMode::AfterGoal_ && wm.gameMode().side() == wm.ourSide())
    {
        if(ccccc >= 0 && ccccc < 10)
        {
            if(wm.ourSide() == RIGHT)
            {
                std::vector<Vector2D> KickOffPosition(12);
                KickOffPosition[1] = Vector2D(-15, 0);
                KickOffPosition[2] = Vector2D(-15, -6);
                KickOffPosition[3] = Vector2D(-15, 6);
                KickOffPosition[4] = Vector2D(-15, -12);
                KickOffPosition[5] = Vector2D(-15, 12);
                KickOffPosition[6] = Vector2D(-21, 0);
                KickOffPosition[7] = Vector2D(-21, -12);
                KickOffPosition[8] = Vector2D(-21, 12);
                KickOffPosition[9] = Vector2D(-28, 0);
                KickOffPosition[10] = Vector2D(-28, -12);
                KickOffPosition[11] = Vector2D(-28, 12);
                if (wm.self().side() == RIGHT)
                {
                    this->doMove(KickOffPosition[this->world().self().unum()].x, -(KickOffPosition[this->world().self().unum()].y));
                    ccccc++;
                    return;
                }
            }
            std::vector<Vector2D> KickOffPosition(12);
            KickOffPosition[1] = Vector2D(-28,0);
            KickOffPosition[2] = Vector2D(-28,-6);
            KickOffPosition[3] = Vector2D(-28,6);
            KickOffPosition[4] = Vector2D(-28,-12);
            KickOffPosition[5] = Vector2D(-28,12);
            KickOffPosition[6] = Vector2D(-21,0);
            KickOffPosition[7] = Vector2D(-21,-12);
            KickOffPosition[8] = Vector2D(-21,12);
            KickOffPosition[9] = Vector2D(-15,0);
            KickOffPosition[10] = Vector2D(-15,-12);
            KickOffPosition[11] = Vector2D(-15,12);
            this->doMove(KickOffPosition[this->world().self().unum()].x, KickOffPosition[this->world().self().unum()].y);
            ccccc++;
            return;
            this->doMove(KickOffPosition[this->world().self().unum()].x, KickOffPosition[this->world().self().unum()].y);
            ccccc++;
            return;
        }
        if(ccccc >= 10 && ccccc < 20)
        {
            std::vector<Vector2D> KickOffPosition(12);
            KickOffPosition[1] = Vector2D(-21,-7);
            KickOffPosition[2] = Vector2D(-19,-10);
            KickOffPosition[3] = Vector2D(-23,-10);
            KickOffPosition[4] = Vector2D(-27,-10);
            KickOffPosition[5] = Vector2D(-31,-10);
            KickOffPosition[6] = Vector2D(-15,-10);
            KickOffPosition[7] = Vector2D(-21,-3);
            KickOffPosition[8] = Vector2D(-21,2);
            KickOffPosition[9] = Vector2D(-21,6);
            KickOffPosition[10] = Vector2D(-21,10);
            KickOffPosition[11] = Vector2D(-11,-10);
            if(wm.self().side() == RIGHT)
            {
                this->doMove(KickOffPosition[this->world().self().unum()].x, -(KickOffPosition[this->world().self().unum()].y));
                ccccc++;
                return;
            }
            this->doMove(KickOffPosition[this->world().self().unum()].x, KickOffPosition[this->world().self().unum()].y);
            ccccc++;
            return;
        }
        if(ccccc >= 20 && ccccc < 30)
        {
            std::vector<Vector2D> KickOffPosition(12);
            KickOffPosition[1] = Vector2D(-20,0);
            KickOffPosition[2] = Vector2D(-13,-11);
            KickOffPosition[3] = Vector2D(-21,-14);
            KickOffPosition[4] = Vector2D(-27,-11);
            KickOffPosition[5] = Vector2D(-30,-5);
            KickOffPosition[6] = Vector2D(-27,-1);
            KickOffPosition[7] = Vector2D(-14,1);
            KickOffPosition[8] = Vector2D(-12,6);
            KickOffPosition[9] = Vector2D(-15,12);
            KickOffPosition[10] = Vector2D(-21,15);
            KickOffPosition[11] = Vector2D(-28,12);
            this->doMove(KickOffPosition[this->world().self().unum()].x, KickOffPosition[this->world().self().unum()].y);
            ccccc++;
            return;
        }
        if(ccccc >= 30 && ccccc < 50)
        {
            std::vector<Vector2D> KickOffPosition(12);
            KickOffPosition[1] = Vector2D(-22,0);
            KickOffPosition[2] = Vector2D(-32,0);
            KickOffPosition[3] = Vector2D(-27,0);
            KickOffPosition[4] = Vector2D(-35,-6);
            KickOffPosition[5] = Vector2D(-29,5);
            KickOffPosition[6] = Vector2D(-22,-7);
            KickOffPosition[7] = Vector2D(-17,0);
            KickOffPosition[8] = Vector2D(-22,5);
            KickOffPosition[9] = Vector2D(-12,0);
            KickOffPosition[10] = Vector2D(-8,-6);
            KickOffPosition[11] = Vector2D(-15,5);
            if(wm.self().side() == RIGHT)
            {
                this->doMove(KickOffPosition[this->world().self().unum()].x, -(KickOffPosition[this->world().self().unum()].y));
                ccccc++;
                return;
            }
            this->doMove(KickOffPosition[this->world().self().unum()].x, KickOffPosition[this->world().self().unum()].y);
            ccccc++;
            return;
        }
        if(ccccc == 50)
            ccccc = 0;
    }

    const int self_min = wm.interceptTable()->selfReachCycle();
    const int mate_min = wm.interceptTable()->teammateReachCycle();
    if(!this->world().self().goalie() && this->world().gameMode().type() == rcsc::GameMode::PlayOn &&  !this->world().self().isKickable() ){
        if ( Bhv_BasicTackle( 0.8, 60).execute( this ) )
        {
            return ;
        }
    }
    // auto = Qt(s,a)=Qt-1(s,a)+αTDt(a,s);
    if(self_min<=mate_min ){
            Body_Intercept().execute(this);
    }
    /*else if(!wm.self().isKickable()){
        if(wm.ball().pos().x < -23 ){
        Vector2D point = wm.getOpponentNearestToSelf(5,true)->pos();
        auto teamm = wm.getTeammateNearestTo(point,5,nullptr);
        if(point.dist(wm.self().pos())<11 && point.x < -35 && wm.self().pos().dist(point ) < teamm->pos().dist(point)){
            if(!Body_GoToPoint(point,0,100).execute(this))
            {
                Body_TurnToPoint(point).execute(this);
                return;
            }
        }
    }
    }*/
    else if((this->world().gameMode().type() == rcsc::GameMode::KickIn_ || this->world().gameMode().type() == rcsc::GameMode::FreeKick_ ||this->world().gameMode().type() == rcsc::GameMode::CornerKick_ )&& wm.self().isKickable()){
    Bhv_SetPlay().execute( this );

    }
    else if(wm.self().isKickable()){
        if(Bhv_BasicOffensiveKick().execute(this)){
            return;
        }
    }


    //
    // special situations (tackle, objects accuracy, intention...)
    //
    if(this->world().gameMode().type() == GameMode::GoalKick_ ){
        if(this->world().self().goalie()
        )
            Bhv_GoalieFreeKick().execute(this);
    }

    if ( doPreprocess() )
    {
        dlog.addText( Logger::TEAM,
        __FILE__": preprocess done" );
        return;
    }

    if(this->world().gameMode().type() == GameMode::PlayOn){
        if(this->world().self().goalie()){


            static const Rect2D our_penalty( Vector2D( -ServerParam::i().pitchHalfLength(),
                                                       -ServerParam::i().penaltyAreaHalfWidth() + 1.0 ),
                                             Size2D( ServerParam::i().penaltyAreaLength() - 1.0,
                                                     ServerParam::i().penaltyAreaWidth() - 2.0 ) );

            //////////////////////////////////////////////////////////////
            // catchable
            if ( this->world().time().cycle()
                 > this->world().self().catchTime().cycle() + ServerParam::i().catchBanCycle()
                 && this->world().ball().distFromSelf() < ServerParam::i().catchableArea() - 0.05
                 && our_penalty.contains( this->world().ball().pos() )  && this->world().lastKickerSide() != this->world().ourSide())
            {
                this->doCatch();
            }
            else if ( this->world().self().isKickable() )
            {
                Bhv_BasicOffensiveKick().execute( this );
            }
            else
            {
                Bhv_GoalieBasicMove().execute( this );
            }

        }else{
            bool kickable = this->world().self().isKickable();
            if ( this->world().existKickableTeammate()
            && this->world().teammatesFromBall().front()->distFromBall()
            < this->world().ball().distFromSelf() )
            {
                kickable = false;
            }

            if ( kickable )
            {
                Bhv_BasicOffensiveKick().execute( this );
            }
            else
            {
                Bhv_BasicMove().execute(this);
            }

        }
        return;
    }

    //
    // penalty kick mode
    //
    if ( world().gameMode().isPenaltyKickMode() )
    {
        dlog.addText( Logger::TEAM,
        __FILE__": penalty kick" );
        Bhv_PenaltyKick().execute( this );
        return;
    }

    //
    // other set play mode
    //
    Bhv_SetPlay().execute( this );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
SamplePlayer::handleActionStart()
{

}

/*-------------------------------------------------------------------*/
/*!

 */
void
SamplePlayer::handleActionEnd()
{
    if ( world().self().posValid() )
    {
        // top,lower
        debugClient().addLine( Vector2D( world().ourDefenseLineX(),
        world().self().pos().y - 2.0 ),
        Vector2D( world().ourDefenseLineX(),
        world().self().pos().y + 2.0 ) );

        //
        debugClient().addLine( Vector2D( world().offsideLineX(),
        world().self().pos().y - 15.0 ),
        Vector2D( world().offsideLineX(),
        world().self().pos().y + 15.0 ) );
    }

    //
    // ball position & velocity
    //
    dlog.addText( Logger::WORLD,
    "WM: BALL pos=(%lf, %lf), vel=(%lf, %lf, r=%lf, ang=%lf)",
    world().ball().pos().x,
    world().ball().pos().y,
    world().ball().vel().x,
    world().ball().vel().y,
    world().ball().vel().r(),
    world().ball().vel().th().degree() );


    dlog.addText( Logger::WORLD,
    "WM: SELF move=(%lf, %lf, r=%lf, th=%lf)",
    world().self().lastMove().x,
    world().self().lastMove().y,
    world().self().lastMove().r(),
    world().self().lastMove().th().degree() );

    Vector2D diff = world().ball().rpos() - world().ball().rposPrev();
    dlog.addText( Logger::WORLD,
    "WM: BALL rpos=(%lf %lf) prev_rpos=(%lf %lf) diff=(%lf %lf)",
    world().ball().rpos().x,
    world().ball().rpos().y,
    world().ball().rposPrev().x,
    world().ball().rposPrev().y,
    diff.x,
    diff.y );

    Vector2D ball_move = diff + world().self().lastMove();
    Vector2D diff_vel = ball_move * ServerParam::i().ballDecay();
    dlog.addText( Logger::WORLD,
    "---> ball_move=(%lf %lf) vel=(%lf, %lf, r=%lf, th=%lf)",
    ball_move.x,
    ball_move.y,
    diff_vel.x,
    diff_vel.y,
    diff_vel.r(),
    diff_vel.th().degree() );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
SamplePlayer::handleServerParam()
{
    if ( KickTable::instance().createTables() )
    {
        std::cerr << world().teamName() << ' '
        << world().self().unum() << ": "
        << " KickTable created."
        << std::endl;
    }
    else
    {
        std::cerr << world().teamName() << ' '
        << world().self().unum() << ": "
        << " KickTable failed..."
        << std::endl;
        M_client->setServerAlive( false );
    }


    if ( ServerParam::i().keepawayMode() )
    {
        std::cerr << "set Keepaway mode communication." << std::endl;
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void SamplePlayer::handlePlayerParam()
{

}

/*-------------------------------------------------------------------*/
/*!

 */
void
SamplePlayer::handlePlayerType()
{

}

/*-------------------------------------------------------------------*/
/*!
  communication decision.
  virtual method in super class
 */
void
SamplePlayer::communicationImpl()
{
    if ( M_communication )
    {
        M_communication->execute( this );
    }
}

/*-------------------------------------------------------------------*/
/*!
 */
bool
SamplePlayer::doPreprocess()
{
    // check tackle expires
    // check self position accuracy
    // ball search
    // check queued intention
    // check simultaneous kick

    const WorldModel & wm = this->world();

    dlog.addText( Logger::TEAM,
    __FILE__": (doPreProcess)" );

    //
    // freezed by tackle effect
    //
    if ( wm.self().isFrozen() )
    {
        dlog.addText( Logger::TEAM,
        __FILE__": tackle wait. expires= %d",
        wm.self().tackleExpires() );
        // face neck to ball
        return true;
    }

    //
    // BeforeKickOff or AfterGoal. jump to the initial position
    //


    /*if ( (wm.gameMode().type() == GameMode::AfterGoal_ && wm.gameMode().type() == wm.gameMode().scoreLeft() && wm.ourSide() == LEFT) || (wm.gameMode().side() == wm.ourSide() && wm.gameMode().type() == GameMode::KickOff_))
    {
        dlog.addText( Logger::TEAM,
        __FILE__": before_kick_off" );
        std::vector<Vector2D> KickOffPosition(12);
        KickOffPosition[1] = Vector2D(-28,0);
        KickOffPosition[2] = Vector2D(-10,-7);
        KickOffPosition[3] = Vector2D(-10,7);
        KickOffPosition[4] = Vector2D(-6,-21);
        KickOffPosition[5] = Vector2D(-6,21);
        KickOffPosition[6] = Vector2D(-5,0);
        KickOffPosition[7] = Vector2D(-2,-15);
        KickOffPosition[8] = Vector2D(-2,15);
        KickOffPosition[9] = Vector2D(-0.5,0);
        KickOffPosition[10] = Vector2D(-1,-26);
        KickOffPosition[11] = Vector2D(-1,26);
        Vector2D move_point =  KickOffPosition.at( wm.self().unum() );
        //Bhv_CustomBeforeKickOff( move_point ).execute( this );
        this->doMove(KickOffPosition.at(wm.self().unum()).x, KickOffPosition.at(wm.self().unum()).y);
        return true;
    }

    if ( (wm.gameMode().type() == GameMode::AfterGoal_ && wm.gameMode().type() == wm.gameMode().scoreRight() && wm.ourSide() == RIGHT) || (wm.gameMode().side() == wm.ourSide() && wm.gameMode().type() == GameMode::KickOff_))
    {
        dlog.addText( Logger::TEAM,
        __FILE__": before_kick_off" );
        std::vector<Vector2D> KickOffPosition(12);
        KickOffPosition[1] = Vector2D(-28,0);
        KickOffPosition[2] = Vector2D(-10,-7);
        KickOffPosition[3] = Vector2D(-10,7);
        KickOffPosition[4] = Vector2D(-6,-21);
        KickOffPosition[5] = Vector2D(-6,21);
        KickOffPosition[6] = Vector2D(-5,0);
        KickOffPosition[7] = Vector2D(-2,-15);
        KickOffPosition[8] = Vector2D(-2,15);
        KickOffPosition[9] = Vector2D(-0.5,0);
        KickOffPosition[10] = Vector2D(-1,-26);
        KickOffPosition[11] = Vector2D(-1,26);
        Vector2D move_point =  KickOffPosition.at( wm.self().unum() );
        this->doMove(KickOffPosition.at(wm.self().unum()).x, KickOffPosition.at(wm.self().unum()).y);
        return true;
    }

    if ( (wm.gameMode().type() == GameMode::AfterGoal_ && wm.gameMode().type() == wm.gameMode().scoreLeft() && wm.ourSide() == RIGHT) || (wm.gameMode().side() == wm.ourSide() && wm.gameMode().type() == GameMode::KickOff_))
    {
        dlog.addText( Logger::TEAM,
        __FILE__": before_kick_off" );
        std::vector<Vector2D> KickOffPosition(12);
        KickOffPosition[1] = Vector2D(-28,0);
        KickOffPosition[2] = Vector2D(-18,-5);
        KickOffPosition[3] = Vector2D(-18, 5);
        KickOffPosition[4] = Vector2D(-18,-18);
        KickOffPosition[5] = Vector2D(-18,18);
        KickOffPosition[6] = Vector2D(-15,0);
        KickOffPosition[7] = Vector2D(-15,-11);
        KickOffPosition[8] = Vector2D(-15,11);
        KickOffPosition[9] = Vector2D(-10, 0);
        KickOffPosition[10] = Vector2D(-10,-18);
        KickOffPosition[11] = Vector2D(-10,18);
        Vector2D move_point =  KickOffPosition.at( wm.self().unum() );
        this->doMove(KickOffPosition.at(wm.self().unum()).x, KickOffPosition.at(wm.self().unum()).y);
        return true;
    }

    if ( (wm.gameMode().type() == GameMode::AfterGoal_ && wm.gameMode().type() == wm.gameMode().scoreRight() && wm.ourSide() == LEFT) || (wm.gameMode().side() == wm.ourSide() && wm.gameMode().type() == GameMode::KickOff_))
    {
        dlog.addText( Logger::TEAM,
        __FILE__": before_kick_off" );
        std::vector<Vector2D> KickOffPosition(12);
        KickOffPosition[1] = Vector2D(-28,0);
        KickOffPosition[2] = Vector2D(-10,-7);
        KickOffPosition[3] = Vector2D(-10,7);
        KickOffPosition[4] = Vector2D(-6,-21);
        KickOffPosition[5] = Vector2D(-6,21);
        KickOffPosition[6] = Vector2D(-5,0);
        KickOffPosition[7] = Vector2D(-2,-15);
        KickOffPosition[8] = Vector2D(-2,15);
        KickOffPosition[9] = Vector2D(-0.5,0);
        KickOffPosition[10] = Vector2D(-1,-26);
        KickOffPosition[11] = Vector2D(-1,26);
        Vector2D move_point =  KickOffPosition.at( wm.self().unum() );
        this->doMove(KickOffPosition.at(wm.self().unum()).x, KickOffPosition.at(wm.self().unum()).y);
        return true;
    }*/
    
    if ( wm.gameMode().type() == GameMode::BeforeKickOff || wm.gameMode().type() == GameMode::AfterGoal_ || (wm.gameMode().type() == GameMode::KickOff_ && wm.gameMode().side() == wm.ourSide() && wm.self().unum() != 9))
    {
        dlog.addText( Logger::TEAM,
        __FILE__": before_kick_off" );
        std::vector<Vector2D> KickOffPosition(12);
        KickOffPosition[1] = Vector2D(-28,0);
        KickOffPosition[2] = Vector2D(-10,-7);
        KickOffPosition[3] = Vector2D(-10,7);
        KickOffPosition[4] = Vector2D(-7.1,-18.5);
        KickOffPosition[5] = Vector2D(-7.1,18.5);
        KickOffPosition[6] = Vector2D(-5,0);
        KickOffPosition[7] = Vector2D(-2,-10);
        KickOffPosition[8] = Vector2D(-2,10);
        KickOffPosition[9] = Vector2D(-0.5,0);
        KickOffPosition[10] = Vector2D(-1,-26);
        KickOffPosition[11] = Vector2D(-1,26);
        Vector2D move_point =  KickOffPosition.at( wm.self().unum() );
        Bhv_CustomBeforeKickOff( move_point ).execute( this );
        return true;
    }



    //
    // self localization error
    //
    if ( ! wm.self().posValid() )
    {
        dlog.addText( Logger::TEAM,
        __FILE__": invalid my pos" );
        Bhv_Emergency().execute( this ); // includes change view
        return true;
    }

    //
    // ball localization error
    //
    const int count_thr = ( wm.self().goalie()
    ? 10
    : 5 );
    if ( wm.ball().posCount() > count_thr
    || ( wm.gameMode().type() != GameMode::PlayOn
    && wm.ball().seenPosCount() > count_thr + 10 ) )
    {
        dlog.addText( Logger::TEAM,
        __FILE__": search ball" );
        Bhv_NeckBodyToBall().execute( this );
        return true;
    }


    //
    // check queued action
    //

    if ( this->doIntention() )
    {
        dlog.addText( Logger::TEAM,
        __FILE__": do queued intention" );
        return true;
    }

    //
    // check simultaneous kick
    //
    if ( doForceKick() )
    {
        return true;
    }

    //
    // check pass message
    //
    if ( doHeardPassReceive() )
    {
        return true;
    }

    return false;
}



/*-------------------------------------------------------------------*/
/*!

 */
bool
SamplePlayer::doForceKick()
{
    const WorldModel & wm = this->world();

    if ( wm.gameMode().type() == GameMode::PlayOn
    && ! wm.self().goalie()
    && wm.self().isKickable()
    && wm.existKickableOpponent() )
    {
        dlog.addText( Logger::TEAM,
        __FILE__": simultaneous kick" );
        this->debugClient().addMessage( "SimultaneousKick" );
        Vector2D goal_pos( ServerParam::i().pitchHalfLength(), 0.0 );

        if ( wm.self().pos().x > 36.0
        && wm.self().pos().absY() > 10.0 )
        {
            goal_pos.x = 45.0;
            dlog.addText( Logger::TEAM,
            __FILE__": simultaneous kick cross type" );
        }
        Body_KickOneStep( goal_pos,
        ServerParam::i().ballSpeedMax()
        ).execute( this );
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
SamplePlayer::doHeardPassReceive()
{
    const WorldModel & wm = this->world();

    if ( wm.audioMemory().passTime() != wm.time()
    || wm.audioMemory().pass().empty()
    || wm.audioMemory().pass().front().receiver_ != wm.self().unum() )
    {

        return false;
    }
    int self_min = wm.interceptTable()->selfReachCycle();
    Vector2D intercept_pos = wm.ball().inertiaPoint( self_min );
    Vector2D heard_pos = wm.audioMemory().pass().front().receive_pos_;

    dlog.addText( Logger::TEAM,
    __FILE__":  (doHeardPassReceive) heard_pos(%.2f %.2f) intercept_pos(%.2f %.2f)",
    heard_pos.x, heard_pos.y,
    intercept_pos.x, intercept_pos.y );
    if(wm.self().unum() == wm.audioMemory().pass().front().receiver_)
    {
        Body_GoToPoint(Vector2D(heard_pos),0,100).execute(this);
    }

    if ( ! wm.existKickableTeammate()
    && wm.ball().posCount() <= 1
    && wm.ball().velCount() <= 1
    && self_min < 20
    //&& intercept_pos.dist( heard_pos ) < 3.0 ) //5.0 )
    )
    {
        dlog.addText( Logger::TEAM,
        __FILE__": (doHeardPassReceive) intercept cycle=%d. intercept",
        self_min );
        this->debugClient().addMessage( "Comm:Receive:Intercept" );
        Body_Intercept().execute( this );
    }
    else
    {
        dlog.addText( Logger::TEAM,
        __FILE__": (doHeardPassReceive) intercept cycle=%d. go to receive point",
        self_min );
        this->debugClient().setTarget( heard_pos );
        this->debugClient().addMessage( "Comm:Receive:GoTo" );
        Body_GoToPoint( heard_pos,
        0.5,
        ServerParam::i().maxDashPower()
        ).execute( this );
    }

    this->setIntention( new IntentionReceive( heard_pos,
    ServerParam::i().maxDashPower(),
    0,
    5,
    wm.time() ) );

    return true;
}

bool SamplePlayer::goforfaza(rcsc::PlayerAgent* agent)
{
    const WorldModel &wm = agent->world();
    if(wm.lastKickerSide() == wm.ourSide()){
    if(Bhv_BasicOffensiveKick().pass_fazayy(agent)== true)
    {
        if(Bhv_BasicOffensiveKick().pass_faza(agent).isValid())
        {
            if(Bhv_BasicOffensiveKick().faza_unum(agent) != 12){
                if(wm.self().unum() == Bhv_BasicOffensiveKick().faza_unum(agent)){
            std::cout<<">>>>>>>> DARIM MIRIM FAZA DASHI ;)";
        agent->setIntention( new IntentionReceive( Bhv_BasicOffensiveKick().pass_faza(agent),
        ServerParam::i().maxDashPower(),
        0,
        2,
        wm.time() ) );
        agent->debugClient().addLine(wm.self().pos(),Bhv_BasicOffensiveKick().pass_faza(agent));;;
        agent->debugClient().setTarget(Vector2D(Bhv_BasicOffensiveKick().pass_faza(agent)));;;

        return true;}}}
    }}
    return false;
}











