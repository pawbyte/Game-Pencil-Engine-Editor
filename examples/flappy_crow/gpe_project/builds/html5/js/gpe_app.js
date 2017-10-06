/*     
#    --------------------------------------------------  #
#       
#       
#  Game Pencil Engine Game File 
#  Created automatically via the Game Pencil Engine Editor Version 1
#  Warning: Manually editing this file may cause unexpected bugs and errors. 
#  If you have any problems reading this file please report it to debug@pawbyte.com . 
#     
#      
#    --------------------------------------------------  # 
*/     

'use strict';
var tex_sky =  GPE.rsm.add_texture(0,'resources/textures/bac_springskyd.png'); 
var spr_crow =  GPE.rsm.add_sprite(0,4,'resources/sprites/spr_crow.png',0,0,24,24,0,0,0,24,24);
var spr_ground =  GPE.rsm.add_sprite(1,4,'resources/sprites/spr_season_grass.png',0,0,64,64,0,0,0,64,64);
var spr_block =  GPE.rsm.add_sprite(2,1,'resources/sprites/spr_block.png',0,0,48,96,0,0,0,48,96);
var spr_score_block =  GPE.rsm.add_sprite(3,1,'resources/sprites/spr_score_block.png',0,0,48,96,0,0,0,48,96);
var spr_enemy =  GPE.rsm.add_sprite(4,4,'resources/sprites/shooter_enemies.png',0,0,32,32,0,0,0,32,32);
var obj_crow =  0; 
var obj_ground =  1; 
var obj_main_menu =  2; 
var obj_score_block =  3; 
var obj_block =  4; 
var obj_enemy_basic =  5; 
var obj_player =  6; 
var newScene1 =  0; 
var  _scn_newScene1 =  GPE.add_gamescene(); 
var scn_title =  1; 
var  _scn_scn_title =  GPE.add_gamescene(); 
var scn_game =  2; 
var  _scn_scn_game =  GPE.add_gamescene(); 

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
                case 4:
                    this.collision_with_obj_block(otherObj);
                break;
                default:
                break;
            }
        };

            
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

GPE._obj_obj_enemy_basic =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_enemy_basic (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =5; 
        this.init_sprite(4);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
        this.imageSpeed = 0;
        this.healthMax = 5;
        this.health = 5;
        this.start_countdown(0,15);
        this.perform_object_logic = function()
        {
            if( this.health <=0)
            {
                this.self_destruct();
            }
            this.canBounce = true;
            
            if( PLAYER_OBJECT!=IS_NULL )
            {
                var distanceToPlayer = this.get_my_distance( PLAYER_OBJECT.getx(), PLAYER_OBJECT.gety() );
                if( distanceToPlayer <32 || distanceToPlayer > 512 )
                {
                    this.speed = 0;
                }
            }
            else
            {
                this.speed = 0;
            }
        }
        this.timedFunction0 = function()
        {
            if( PLAYER_OBJECT!=IS_NULL )
            {
                var distanceToPlayer = this.get_my_distance( PLAYER_OBJECT.getx(), PLAYER_OBJECT.gety() );
                if( distanceToPlayer >32 && distanceToPlayer < 256 )
                {
                    this.move_towards(PLAYER_OBJECT.getx(), PLAYER_OBJECT.gety(),1 );
                }
            }
            this.start_countdown(0,gpe.get_random(10,45) );
        }
        this.gpeHasTimedFunctions = true;
        this.gpeHasLogicFunction = true;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_enemy_basic.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_enemy_basic; 
}());

GPE._obj_obj_player =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_player (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =6; 
        this.init_sprite(-1);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
        
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_player.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_player; 
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

  GPR.GAME_OBJECTS_FAMILIES[1] = [];

  GPR.GAME_OBJECTS_FAMILIES[2] = [];

  GPR.GAME_OBJECTS_FAMILIES[3] = [];

  GPR.GAME_OBJECTS_FAMILIES[4] = [];
  GPR.GAME_OBJECTS_FAMILIES[4].push(3);

  GPR.GAME_OBJECTS_FAMILIES[5] = [];

  GPR.GAME_OBJECTS_FAMILIES[6] = [];

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
         case 5:
             newObjOut = new GPE._obj_obj_enemy_basic(newX, newY, objectLayerId);
         break;
         case 6:
             newObjOut = new GPE._obj_obj_player(newX, newY, objectLayerId);
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

_scn_newScene1.sceneIdNumber = 0;
_scn_newScene1.sceneName = "newScene1";
_scn_newScene1.sceneCaption = "";
_scn_newScene1.sceneWidth = 1024;
_scn_newScene1.sceneHeight = 2048;
_scn_newScene1.tileWidth  = 32;
_scn_newScene1.tileHeight = 32;
_scn_newScene1.tileAmountX = 32;
_scn_newScene1.tileAmountY = 64;
_scn_newScene1.bgColor = '#40714D';
_scn_newScene1.sceneIsContinuous = 0;
_scn_temp_layer = _scn_newScene1.add_layer( 1,0,0);
_scn_temp_layer.scnStartBackground = -1; 
_scn_temp_layer = _scn_newScene1.add_layer( 2,1,0);
_scn_temp_layer.scnStartObjects.push( {objId:6,xPos: 160,yPos: 128,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:5,xPos: 224,yPos: 224,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:5,xPos: 416,yPos: 160,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:5,xPos: 672,yPos: 320,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:5,xPos: 512,yPos: 448,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:5,xPos: 736,yPos: 896,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:5,xPos: 480,yPos: 1024,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:5,xPos: 160,yPos: 1184,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:5,xPos: 672,yPos: 1216,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:5,xPos: 736,yPos: 96,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 64,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 32,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 160,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 160,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 64,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 96,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 128,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 320,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 256,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 224,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 192,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 128,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 96,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 64,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 32,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 192,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 288,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 288,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 256,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 288,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 288,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 256,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 224,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 384,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 320,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 352,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 416,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 448,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 480,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 512,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 544,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 480,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 480,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 448,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 384,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 352,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 416,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 512,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 576,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 640,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 768,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 768,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 736,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 608,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 640,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 672,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 736,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 704,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 768,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 800,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 832,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 864,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 896,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 928,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 960,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 992,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1024,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1056,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1120,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1088,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1184,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1152,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1216,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1248,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1344,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1312,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1408,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1376,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1408,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1440,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1472,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1536,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1568,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1600,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1632,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1664,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1696,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1728,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1792,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1856,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1888,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1920,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1952,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 1984,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 64,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 32,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 64,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 192,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 224,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 256,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 416,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 448,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 480,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 512,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 576,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 640,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 736,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 768,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 800,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 928,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 896,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 864,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 832,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 800,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 768,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 480,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 448,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 416,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 384,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 128,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 96,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 64,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 32,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 320,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 352,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 800,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 832,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 864,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 896,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 928,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 960,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 32,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 128,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 96,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 64,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 128,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 160,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 192,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 224,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 256,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 288,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 320,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 352,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 384,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 416,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 480,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 448,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 480,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 512,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 544,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 608,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 640,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 704,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 672,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 704,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 736,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 768,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 800,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 832,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 864,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 896,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 928,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 960,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1024,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1056,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 992,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1024,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1088,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1120,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1152,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1184,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1216,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1248,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1312,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1344,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1376,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1472,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1440,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1408,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1536,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1568,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1600,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1632,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1664,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1696,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1728,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1792,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1856,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1888,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1920,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1952,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1984,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 960,yPos: 2016,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 1120,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 576,yPos: 1120,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 1120,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 1152,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 1152,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 1184,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 1216,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 1248,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 32,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 128,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 96,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 64,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 96,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 128,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 192,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 224,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 256,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 320,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 320,yPos: 1792,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 320,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 320,yPos: 1856,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 320,yPos: 1888,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 992,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 960,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 928,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 896,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 864,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 832,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 800,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 768,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 736,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 1792,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 1728,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 1696,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 1664,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 1632,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 640,yPos: 1632,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 1632,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 576,yPos: 1632,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 1632,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 512,yPos: 1664,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 512,yPos: 1696,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 512,yPos: 1728,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 512,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 480,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 448,yPos: 1760,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 448,yPos: 1792,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 448,yPos: 1824,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 448,yPos: 1856,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 448,yPos: 1888,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 512,yPos: 1632,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 0,yPos: 544,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 96,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 192,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 224,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 256,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 256,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 224,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 192,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 128,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 96,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 64,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 32,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 96,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 192,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 256,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 576,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 960,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 672,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 640,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 608,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 640,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 672,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 704,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 736,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 768,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 800,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 608,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 640,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 672,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 704,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 736,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 768,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 800,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 864,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 896,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 928,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 960,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 832,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 832,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 864,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 896,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 928,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 960,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 992,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 1024,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 1056,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 544,yPos: 1088,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 960,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 992,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1024,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1056,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1088,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1120,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1152,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1184,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1216,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1248,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 224,yPos: 1312,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 192,yPos: 1312,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 1312,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 256,yPos: 1312,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1312,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 1344,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 1376,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 1408,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 1440,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 1472,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 160,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 192,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 224,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 256,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 288,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 320,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 352,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 384,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 416,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 640,yPos: 1280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 1280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1312,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1344,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1376,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1408,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1440,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1472,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1440,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1408,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1376,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1344,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1312,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 1280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 608,yPos: 1280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 800,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 768,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 736,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 832,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 864,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 896,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 928,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 960,yPos: 1504,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 704,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 928,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 896,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 864,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 832,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 672,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:4,xPos: 640,yPos: 576,customComponents: {}}); 
_scn_scn_title.sceneIdNumber = 1;
_scn_scn_title.sceneName = "scn_title";
_scn_scn_title.sceneCaption = "";
_scn_scn_title.sceneWidth = 832;
_scn_scn_title.sceneHeight = 480;
_scn_scn_title.tileWidth  = 32;
_scn_scn_title.tileHeight = 32;
_scn_scn_title.tileAmountX = 26;
_scn_scn_title.tileAmountY = 15;
_scn_scn_title.bgColor = '#40ACB1';
_scn_scn_title.sceneIsContinuous = 0;
_scn_temp_layer = _scn_scn_title.add_layer( 2,1,0);
_scn_temp_layer.scnStartObjects.push( {objId:2,xPos: 320,yPos: 160,customComponents: {}}); 
_scn_scn_game.sceneIdNumber = 2;
_scn_scn_game.sceneName = "scn_game";
_scn_scn_game.sceneCaption = "";
_scn_scn_game.sceneWidth = 832;
_scn_scn_game.sceneHeight = 640;
_scn_scn_game.tileWidth  = 32;
_scn_scn_game.tileHeight = 32;
_scn_scn_game.tileAmountX = 26;
_scn_scn_game.tileAmountY = 20;
_scn_scn_game.bgColor = '#000000';
_scn_scn_game.sceneIsContinuous = 0;
_scn_temp_layer = _scn_scn_game.add_layer( 1,0,0);
_scn_temp_layer.scnStartBackgrounds.push( {bgTexId:0,bgXPos: 0,bgYPos: 0,bgXSpeed: -4,bgYSpeed: 0,bgTileHori: 1,bgTileVert: 1,bgStartStretch: 0} ); 
_scn_temp_layer = _scn_scn_game.add_layer( 2,1,0);
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 320,yPos: 384,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 768,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 704,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 640,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 320,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 256,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 192,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 128,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 64,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 384,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 448,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 576,yPos: 576,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 512,yPos: 576,customComponents: {}}); 
//Start of Project Macros 
var GAME_OVER = false;
var CURRENT_SCORE = 0;
var HIGH_SCORE = 0;//End of Project Macros 
