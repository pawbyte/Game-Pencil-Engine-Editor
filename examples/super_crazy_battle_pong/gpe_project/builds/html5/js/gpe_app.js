/*     
#    --------------------------------------------------  #
#       
#       
#  Game Pencil Engine Game File 
#  Created automatically via the Game Pencil Engine Editor Version 1
#  Warning: Manually editing this file may cause unexpected bugs and errors. 
#  If you have any problems reading this file please report it to help@pawbyte.com . 
#     
#      
#    --------------------------------------------------  # 
*/     

'use strict';
var spr_paddle =  GPE.rsm.add_sprite(0,1,'resources/sprites/pong_paddle.png',0,0,32,64,0,0,0,32,64);
var spr_ball =  GPE.rsm.add_sprite(1,1,'resources/sprites/spr_pong_ball.png',0,0,12,12,0,0,0,12,12);
var spr_paddle2 =  GPE.rsm.add_sprite(2,1,'resources/sprites/pong_paddle2.png',0,0,32,64,0,0,0,32,64);
var aud_bounce =  GPE.rsm.add_audio(0,'aud_bounce','','','','resources/audio/pong_bounce_sound.wav',0,'',1,100); 
var guiFont =  GPE.rsm.add_font(0,'Arial','','','','','',72,32,32,0);
var smallGuiFont =  GPE.rsm.add_font(1,'Arial','','','','','',24,32,32,0);
var obj_ball =  0; 
var obj_paddle_p1 =  1; 
var obj_paddle_p2 =  2; 
var newScene1 =  0; 
var  _scn_newScene1 =  GPE.add_gamescene(0 , 'newScene1'); 

GPE._obj_obj_ball =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_ball (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =0; 
        this.init_sprite(1);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        //Randomly goes left or right for 1st throw
        this.player1Score = 0;
        this.player2Score = 0;
        this.speed = 0;
        this.futureSpeed = 4;
        if( gpe.get_random(0, 10) > 5)
        {
            this.direction = gpe.radian(0);
        }
        else
        {
            this.direction = gpe.radian(180);
        }
        this.canBounce = true;
        this.perform_object_logic = function()
        {
            //Handles points and bouncing from top/down "invisible walls"
            this.canBounce = true;
            if( this.getx() < 0)
            {
                this.update_cords( this.xBegin, this.yBegin);
                this.direction = gpe.radian(180);
                this.futureSpeed = 4;
                this.speed = 0;
                //player 1 starts
                this.player2Score++;
            }
            if( this.getx2() > gpe.get_scene_width() )
            {
                this.update_cords( this.xBegin, this.yBegin);
                this.direction = gpe.radian(0);
                this.futureSpeed = 4;
                this.speed = 0;
                 //player 2 starts
                this.player1Score++;
            }
            if( this.gety() < 0)
            {
                if(!gpe.audio_playing(aud_bounce) )
                {
                    gpe.audio_play(aud_bounce);
                    this.update_cords( this.xPast, this.yPast);
                    this.set_velocity( -this.direction, this.speed);
                }
            }
            if( this.gety2() >= gpe.get_scene_height() )
            {
                if(!gpe.audio_playing(aud_bounce) )
                {
                    gpe.audio_play(aud_bounce);
                    this.update_cords( this.xPast, gpe.get_scene_height()-32);
                    this.set_velocity( -this.direction, this.speed);
                }
            }
            if( this.speed ==0 && gpe.check_keyboard(gp_space) || gpe.check_keyboard(gp_y) )
            {
                if( this.futureSpeed!=0)
                {
                    this.speed = this.futureSpeed;
                }
                else
                {
                    this.speed = 4;
                }
            }
            if( this.speed > 20)
            {
                this.speed = 20;
            }
        }
        this.render_hud = function()
        {
            if( this.speed ==0)
            {
                gpe.render_text(smallGuiFont,gpe.get_camera_screen_width()/2,32,"Super Crazy Battle Pong","white",fa_center,fa_top);
                gpe.render_text(smallGuiFont,gpe.get_camera_screen_width()/2,gpe.get_camera_screen_height()-64,"Press Y or Space to Start","white",fa_center,fa_bottom);
            }
            gpe.render_text(guiFont,gpe.get_scene_width()/2,64,this.player1Score+":"+this.player2Score,"white",fa_center,fa_top);
        }
        this.hasHudRenderFunction = true;
        this.gpeHasLogicFunction = true;
        this.collision_with_obj_paddle_p1 = function(other)
        {
            if( this.canBounce==true)
            {
                gpe.audio_play(aud_bounce);
                this.update_cords( other.getx2()+2, this.yPast );
                var hh = other.get_collision_height() / 2;
                var bounceAngle = ((this.gety() + this.get_collision_height() / 2) - (other.gety() + hh)) / hh * 45;
                //please note the angel automatically sets it to 45 degrees
                //In a real game there will be a bit more math for the "perfect bounce"
                //For this tutorial we will not go that advanced.
                this.set_velocity( gpe.radian(bounceAngle), this.speed*-1.1);
                this.canBounce = false;
            }
        }
        this.collision_with_obj_paddle_p2 = function(other)
        {
            if( this.canBounce==true)
            {
                gpe.audio_play(aud_bounce);
                this.update_cords( other.getx()-this.get_collision_width()-2, this.yPast );
                //The only change in code is the angle 235[degrees]
                var bounceAngle = 180;
                var hh = other.get_collision_height() / 2;
                bounceAngle =( (this.gety() + this.get_collision_height() / 2) - (other.gety() + hh) ) / hh *(-45);
                this.set_velocity( gpe.radian(bounceAngle+180), this.speed*-1.1);
                this.canBounce = false;
                //please note the angel automatically sets it to 45 degrees
                //In a real game there will be a bit more math for the "perfect bounce"
                //For this tutorial we will not go that advanced.
            }
        }
        this.collide_with = function(otherObj)
        {
            switch(otherObj.gpeObjectType)
            {
                case 1:
                    this.collision_with_obj_paddle_p1(otherObj);
                break;
                case 2:
                    this.collision_with_obj_paddle_p2(otherObj);
                break;
                default:
                break;
            }
        };

            
            this.OBJECT_COLLISION_LIST.push(1);
            this.OBJECT_COLLISION_LIST.push(2);
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_ball.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_ball; 
}());

GPE._obj_obj_paddle_p1 =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_paddle_p1 (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =1; 
        this.init_sprite(0);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        this.perform_object_logic = function()
        {
            if( gpe.check_keyboard( gp_w ) && this.gety() > 4 )
            {
                this.move_up(8);
            }
            else if( gpe.check_keyboard( gp_s ) && this.gety2() < gpe.get_scene_height() )
            {
                this.move_down(8);
            }
            else if( gpe.check_keyboard( gp_d ) && this.x() < gpe.get_scene_width() )
            {
                this.x+=8;
            }
        }
        this.gpeHasLogicFunction = true;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_paddle_p1.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_paddle_p1; 
}());

GPE._obj_obj_paddle_p2 =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_paddle_p2 (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =2; 
        this.init_sprite(2);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        this.perform_object_logic = function()
        {
            if( gpe.check_keyboard( gp_up ) && this.gety() > 4 )
            {
                this.move_up(8);
            }
            else if( gpe.check_keyboard( gp_down ) && this.gety2() < gpe.get_scene_height() )
            {
                this.move_down(8);
            }
        }
        this.gpeHasLogicFunction = true;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_paddle_p2.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_paddle_p2; 
}());


GPE.activate_object_families= function()
{
  GPR.GAME_OBJECTS_FAMILIES = [];
  GPR.GAME_OBJECTS_FAMILIES[0] = [];

  GPR.GAME_OBJECTS_FAMILIES[1] = [];

  GPR.GAME_OBJECTS_FAMILIES[2] = [];

}


GPE.add_new_object = function(newObjType, newX, newY, objectLayerId )
{
if( typeof objectLayerId!="number"){objectLayerId = 1;}
     var newObjOut = IS_NULL;
     switch( newObjType )
     {
         case 0:
             newObjOut = new GPE._obj_obj_ball(newX, newY, objectLayerId);
         break;
         case 1:
             newObjOut = new GPE._obj_obj_paddle_p1(newX, newY, objectLayerId);
         break;
         case 2:
             newObjOut = new GPE._obj_obj_paddle_p2(newX, newY, objectLayerId);
         break;
         default:
         break;
     }
     if(newObjOut!=IS_NULL)
     {
         newObjOut.xPos = newX;
         newObjOut.yPos = newY;
         newObjOut.sceneLayerPos = objectLayerId;
         GPR.add_object(newObjOut, false, objectLayerId);
         return newObjOut;
     }
     return IS_NULL;
};
GPE.rsm.audioIsReadyForLoading = true; 
var _scn_temp_layer = IS_NULL; 

_scn_newScene1.sceneId = 0;
_scn_newScene1.sceneName = "newScene1";
_scn_newScene1.sceneCaption = "";
_scn_newScene1.sceneWidth = 800;
_scn_newScene1.sceneHeight = 640;
_scn_newScene1.tileWidth  = 32;
_scn_newScene1.tileHeight = 32;
_scn_newScene1.tileAmountX = 25;
_scn_newScene1.tileAmountY = 20;
_scn_newScene1.bgColor = '#131316';
_scn_newScene1.sceneIsContinuous = 0;
_scn_temp_layer = _scn_newScene1.add_layer( 1,0,0);
_scn_temp_layer = _scn_newScene1.add_layer( 2,1,0);
_scn_temp_layer.scnStartObjects.push( {objId:2,xPos: 672,yPos: 288,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 64,yPos: 288,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 352,yPos: 288,customComponents: {}}); 
//Start of Project Macros 
//End of Project Macros 
