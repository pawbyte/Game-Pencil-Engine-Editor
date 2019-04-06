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
var tex_sky =  GPE.rsm.add_texture(0,'resources/textures/bac_springskyd.png'); 
var spr_crow =  GPE.rsm.add_sprite(0,4,'resources/animations/spr_crow.png',0,0,24,24,0,0,0,24,24);
var spr_ground =  GPE.rsm.add_sprite(1,4,'resources/animations/spr_season_grass.png',0,0,64,64,0,0,0,64,64);
var spr_block =  GPE.rsm.add_sprite(2,1,'resources/animations/spr_block.png',0,0,48,96,0,0,0,48,96);
var spr_score_block =  GPE.rsm.add_sprite(3,1,'resources/animations/spr_score_block.png',0,0,48,96,0,0,0,48,96);
var newAudio1 =  GPE.rsm.add_audio(0,'newAudio1','','','','',0,'',1,100); 
var newVideo1 =  GPE.rsm.add_video(0,'newVideo1','','','','',100); 
var obj_crow =  0; 
var obj_ground =  1; 
var obj_main_menu =  2; 
var obj_score_block =  3; 
var obj_block =  4; 
var scn_title =  0; 
var  _scn_scn_title =  GPE.add_gamescene(0 , 'scn_title'); 
var scn_game =  1; 
var  _scn_scn_game =  GPE.add_gamescene(1 , 'scn_game'); 

function newFunction1(  ) 
{
}


GPE._obj_obj_crow =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_crow (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =0; 
        this.init_sprite(0);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        this.imageSpeed = 0.125;
        this.canJump = true;
        this.start_countdown( 1, 5);
        GAME_OVER = false;
        this.perform_object_logic = function()
        {
            if( GAME_OVER )
            {
                if( gpe.check_keyboard_released(gp_enter) )
                {
                    gpe.scene_enter( scn_title );
                    GAME_OVER = false;
                    CURRENT_SCORE = 0;
                }
            }
            else
            {
                if (gpe.position_collides(this,this.getx(), this.gety() + 1, obj_ground)==IS_NULL )
                {
                    if ( gpe.check_keyboard_released(gp_space) && this.canJump && !this.gameOver )
                    {
                        if (gpe.position_collides(this,this.getx(), this.gety() -4, obj_ground)==IS_NULL )
                        {
                            this.yspeed = -5;
                        }
                        this.canJump = false;
                        this.start_countdown( 0, 5);
                        this.move_up(2);
                    }
                }
                this.yspeed += 0.5;
                if( this.yspeed < -14 )
                {
                    this.yspeed = -14;
                }
                else if( this.yspeed > 14 )
                {
                    this.yspeed = 14;
                }
                var _abs = gpe.abs(this.yspeed);
                var _sign = gpe.sign(this.yspeed);
                for (var i = 0; i < _abs; i++)
                {
                    if (gpe.position_collides(this,this.getx(), this.gety() + _sign, obj_ground )!=IS_NULL)
                    {
                        // hit ground/ceiling
                        this.bird_die();
                        break;
                    }
                    else
                    {
                        this.move_down(_sign);
                    }
                }
            }
            gpe.center_camera(0,this.getx(), this.gety() );
        }
        this.render_hud = function()
        {
            if( GAME_OVER )
            {
                gpe.render_text(-1,32,64,"Game Over!","red");
                gpe.render_text(-1,32,32,"Score: "+CURRENT_SCORE+" | High Score: "+HIGH_SCORE,"red");
                gpe.render_text(-1,gpe.get_camera_screen_width(0)/2,gpe.get_camera_screen_height(0)-64,"Press [ENTER] to Restart!","white",fa_center,fa_middle );
            }
            else
            {
                gpe.render_text(-1,32,32,"Score: "+CURRENT_SCORE,"white");
            }
        }
        this.hasHudRenderFunction = true;
        this.timedFunction0 = function()
        {
            this.canJump = true;
        }
        this.timedFunction1 = function()
        {
            if( !GAME_OVER )
            {
                var sceneWidth = gpe.get_scene_width();
                var sceneHeight = gpe.get_scene_height();
                var ii = 0;
                var scoreNewYPos = Math.round( gpe.get_random( 2*96,sceneHeight-(96*3) )/96 )*96;
                for( ii = scoreNewYPos-96; ii >=0; ii-=96 )
                {
                    this.add_object(obj_block, sceneWidth,ii );
                }
                var newBlock = this.add_object(obj_score_block, sceneWidth, scoreNewYPos );
                for( ii = scoreNewYPos+96; ii <= sceneHeight+96; ii+=96 )
                {
                    this.add_object(obj_block, sceneWidth,ii );
                }
            }
            this.start_countdown( 1, 90 );
        }
        this.gpeHasTimedFunctions = true;
        this.gpeHasLogicFunction = true;
        this.collision_with_obj_score_block = function(other)
        {
            CURRENT_SCORE++;
            other.self_destruct();
        }
        this.collision_with_obj_block = function(other)
        {
            if( !this.gameOver)
            {
                this.bird_die();
            }
        }
        this.collide_with = function(otherObj)
        {
            switch(otherObj.gpeObjectType)
            {
                case 3:
                    this.collision_with_obj_score_block(otherObj);
                break;
                case 4:
                    this.collision_with_obj_block(otherObj);
                break;
                default:
                break;
            }
        };

            
            this.OBJECT_COLLISION_LIST.push(3);
            this.OBJECT_COLLISION_LIST.push(4);
        this.bird_die = function()
        {
            this.canJump = false;
            GAME_OVER = true;
            this.yspeed = 0;
            if( CURRENT_SCORE > HIGH_SCORE)
            {
                HIGH_SCORE = CURRENT_SCORE;
            }
            this.imageSpeed = 0;
        };

        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_crow.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_crow; 
}());

GPE._obj_obj_ground =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_ground (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =1; 
        this.init_sprite(1);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
        this.imageSpeed = 0;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_ground.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_ground; 
}());

GPE._obj_obj_main_menu =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_main_menu (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =2; 
        this.init_sprite(-1);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        this.imageSpeed = 0.125;
        this.perform_object_logic = function()
        {
            if( gpe.check_keyboard_released(gp_enter) )
            {
                gpe.scene_enter_next();
            }
            gpe.center_camera(0,this.getx(), this.gety() );
        }
        this.render_hud = function()
        {
            gpe.render_text(-1,gpe.get_camera_screen_width(0)/2,gpe.get_camera_screen_height(0)-64,"Flappy Crow - A Horribly made Game","white",fa_center,fa_middle );
            gpe.render_text(-1,gpe.get_camera_screen_width(0)/2,gpe.get_camera_screen_height(0)-32,"Press [ENTER] to Start Game!","white",fa_center,fa_middle );
        }
        this.hasHudRenderFunction = true;
        this.gpeHasLogicFunction = true;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_main_menu.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_main_menu; 
}());

GPE._obj_obj_block =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_block (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =4; 
        this.init_sprite(2);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        this.set_velocity( 180 * GPE_TO_RADIANS,-4 );
        this.perform_object_logic = function()
        {
            if( this.getx2() < 0)
            {
                this.self_destruct();
            }
            if( GAME_OVER )
            {
                this.set_velocity(0,0);
            }
        }
        this.gpeHasLogicFunction = true;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_block.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_block; 
}());

GPE._obj_obj_score_block =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_score_block (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE._obj_obj_block.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =3; 
        this.init_sprite(3);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_score_block.prototype = new GPE._obj_obj_block(xPosIn, yPosIn); 
    return _obj_obj_score_block; 
}());


GPE.activate_object_families= function()
{
  GPR.GAME_OBJECTS_FAMILIES = [];
  GPR.GAME_OBJECTS_FAMILIES[0] = [];
  GPR.GAME_OBJECTS_FAMILIES[0].push(0);

  GPR.GAME_OBJECTS_FAMILIES[1] = [];
  GPR.GAME_OBJECTS_FAMILIES[1].push(1);

  GPR.GAME_OBJECTS_FAMILIES[2] = [];
  GPR.GAME_OBJECTS_FAMILIES[2].push(2);

  GPR.GAME_OBJECTS_FAMILIES[3] = [];
  GPR.GAME_OBJECTS_FAMILIES[3].push(3);

  GPR.GAME_OBJECTS_FAMILIES[4] = [];
  GPR.GAME_OBJECTS_FAMILIES[4].push(3);

}


GPE.add_new_object = function(newObjType, newX, newY, objectLayerId )
{
if( typeof objectLayerId!="number"){objectLayerId = 1;}
     var newObjOut = IS_NULL;
     switch( newObjType )
     {
         case 0:
             newObjOut = new GPE._obj_obj_crow(newX, newY, objectLayerId);
         break;
         case 1:
             newObjOut = new GPE._obj_obj_ground(newX, newY, objectLayerId);
         break;
         case 2:
             newObjOut = new GPE._obj_obj_main_menu(newX, newY, objectLayerId);
         break;
         case 4:
             newObjOut = new GPE._obj_obj_block(newX, newY, objectLayerId);
         break;
         case 3:
             newObjOut = new GPE._obj_obj_score_block( newX, newY,objectLayerId);
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

newGPEScene_000 = _scn_scn_title;
_scn_scn_title.sceneId = 0;
_scn_scn_title.sceneName = "scn_title";
_scn_scn_title.sceneCaption = "";
_scn_scn_title.sceneWidth = 832;
_scn_scn_title.sceneHeight = 480;
_scn_scn_title.defaultTileWidth  = 32;
_scn_scn_title.defaultTileHeight = 32;
_scn_scn_title.tileAmountX = 26;
_scn_scn_title.tileAmountY = 15;
_scn_scn_title.bgColor = '#40ACB1';
_scn_scn_title.sceneIsContinuous = 0;
var newBranch = {};
    newBranch = _scn_temp_layer = newGPEScene_000.add_layer( 0,0);
    newBranch.init_basic(    newBranch = _scn_temp_layer = newGPEScene_000.add_layer( 1,0);
    newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:2} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId: -1} ); 
            newBranch.init_basic(newGPEScene_000 = _scn_scn_game;
_scn_scn_game.sceneId = 1;
_scn_scn_game.sceneName = "scn_game";
_scn_scn_game.sceneCaption = "";
_scn_scn_game.sceneWidth = 832;
_scn_scn_game.sceneHeight = 640;
_scn_scn_game.defaultTileWidth  = 32;
_scn_scn_game.defaultTileHeight = 32;
_scn_scn_game.tileAmountX = 26;
_scn_scn_game.tileAmountY = 20;
_scn_scn_game.bgColor = '#000000';
_scn_scn_game.sceneIsContinuous = 0;
var newBranch = {};
    newBranch = _scn_temp_layer = newGPEScene_000.add_layer( 0,0);
    newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartBackgrounds.push( {bgTexId:0,bgXPos: 0,bgYPos: 0,bgXSpeed: -4,bgYSpeed: 0,bgTileHori: 1,bgTileVert: 1,bgStartStretch: 0} ); 
            newBranch.init_basic(    newBranch = _scn_temp_layer = newGPEScene_000.add_layer( 1,0);
    newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:0} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(        newBranch = _scn_temp_layer.scnStartObjects.push( {objId:1} ); 
            newBranch.init_basic(//Start of Project Macros 
var GAME_OVER = false;
var CURRENT_SCORE = 0;
var HIGH_SCORE = 0;//End of Project Macros 
