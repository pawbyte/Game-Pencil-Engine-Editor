/*     
#    --------------------------------------------------  #
#       
#       
#  Game Pencil Engine Game File 
#  Created automatically via the Game Pencil Engine Editor Version 1.0.4
#  Warning: Manually editing this file may cause unexpected bugs and errors. 
#  If you have any problems reading this file please report it to debug@pawyte.com . 
#     
#      
#    --------------------------------------------------  # 
*/     

'use strict';
var tex_grass =  GPE.rsm.add_texture(0,'resources/textures/grass.png'); 

var newTilesheet1 = -1;

var spr_block =  GPE.rsm.add_sprite(0,'resources/sprites/spr_block_brown.png',0,0,32,32,0,0,0,32,32);
var spr_enemy =  GPE.rsm.add_sprite(1,'resources/sprites/shooter_enemies.png',0,0,32,32,0,0,0,32,32);
var spr_bullet =  GPE.rsm.add_sprite(2,'resources/sprites/spr_coin.png',0,0,16,16,0,0,0,16,16);
var spr_xcoord =  GPE.rsm.add_sprite(3,'resources/sprites/xcoord.png',0,0,32,32,0,0,0,32,32);
var spr_player_ship =  GPE.rsm.add_sprite(4,'resources/sprites/limehead_bro.png',0,0,32,32,0,0,0,32,32);

var newAudio1 =  GPE.rsm.add_audio(0,'newAudio1','','','','',0,'',1,100); 

var newVideo1 =  GPE.rsm.add_video(0,'newVideo1','','','','',100); 

var newFont1 =  GPE.rsm.add_font(0,'Arial','','','','','',12,0);

var obj_block =  0; 
var obj_enemy_basic =  1; 
var obj_bullet =  2; 
var obj_player =  3; 
var newScene1 =  0; 

GPE._obj_obj_block =  (function (xPosIn, yPosIn) 
{ 
    function _obj_obj_block (xPosIn, yPosIn) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn); 
        this.gpeObjectType =0; 
        this.init_sprite(0);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
        
        this.render_under = function()
        {
            gpe.render_sprite(this.get_sprite(),0,this.getx(),this.gety(),1,1);
        }
        this.render_self = function()
        {
            //
        }
        this.hasPreRenderFunction = true;
        this.hasRenderFunction = true;
        
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_block.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_block; 
}());

GPE._obj_obj_enemy_basic =  (function (xPosIn, yPosIn) 
{ 
    function _obj_obj_enemy_basic (xPosIn, yPosIn) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn); 
        this.gpeObjectType =1; 
        this.init_sprite(1);
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
            this.start_countdown(0,15);
        }
        this.gpeHasTimedFunctions = true;
        this.gpeHasLogicFunction = true;
        this.collision_with_obj_bullet = function(other)
        {
            this.health-=1;
            other.self_destruct();
        }
        this.collision_with_obj_block = function(other)
        {
            this.update_cords(this.xPast, this.yPast);
        }
        this.collide_with = function(otherObj)
        {
            switch(otherObj.gpeObjectType)
            {
                case 2:
                    this.collision_with_obj_bullet(otherObj);
                break;
                case 0:
                    this.collision_with_obj_block(otherObj);
                break;
                default:
                break;
            }
        };

        
        this.OBJECT_COLLISION_LIST.push(2);
        this.OBJECT_COLLISION_LIST.push(0);
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_enemy_basic.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_enemy_basic; 
}());

GPE._obj_obj_bullet =  (function (xPosIn, yPosIn) 
{ 
    function _obj_obj_bullet (xPosIn, yPosIn) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn); 
        this.gpeObjectType =2; 
        this.init_sprite(2);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        this.speed = 8;
        this.direction = 0;
        this.start_timer(0, 5);
        this.canBounce = true;
        
        this.start_frame_logic = function()
        {
            this.canBounce = true;
        }
        this.timedFunction0 = function()
        {
            this.self_destruct();
        }
        this.gpeHasTimedFunctions = true;
        this.gpeHasStartLogicFunction = true;
        this.collision_with_obj_block = function(other)
        {
            if( this.canBounce)
            {
                //this.update_cords(this.xPast, this.yPast );
                this.set_velocity(this.direction+pi,this.speed);
                this.canBounce = false;
            }
        }
        this.collide_with = function(otherObj)
        {
            switch(otherObj.gpeObjectType)
            {
                case 0:
                    this.collision_with_obj_block(otherObj);
                break;
                default:
                break;
            }
        };

        
        this.OBJECT_COLLISION_LIST.push(0);
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_bullet.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_bullet; 
}());

GPE._obj_obj_player =  (function (xPosIn, yPosIn) 
{ 
    function _obj_obj_player (xPosIn, yPosIn) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn); 
        this.gpeObjectType =3; 
        this.init_sprite(4);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 1;
        this.isViewIndependent = 1;
        this.speed = 0;
        this.direction = 0;
        
        this.targetX = this.getx();
        this.targetY = this.gety();
        
        this.canFire = true;
        
        PLAYER_OBJECT = this;
        this.mouseDirection = 0;
        new Date
        Math
        this.perform_object_logic = function()
        {
            
            this.mouseDirection = this.get_my_direction( gpe.get_camera_mouse_x(0), gpe.get_camera_mouse_y(0) );
            if( gpe.check_mouse_released( 0 ) )
            {
                this.targetX = gpe.get_camera_mouse_x(0);
                this.targetY = gpe.get_camera_mouse_y(0);
                this.move_towards(this.targetX, this.targetY, 4 );
            }
            else if( gpe.check_mouse_released( 2 ) && this.canFire  )
            {
                var newBullet = gpe.add_new_object( obj_bullet, this.getx(), this.gety() );
                newBullet.direction = this.mouseDirection;
                newBullet.speed = 7;
                this.canFire = false;
                this.start_countdown( 0, 30 );
            }
            
            if( this.speed !=0)
            {
                if( this.get_my_distance( this.targetX, this.targetY ) < this.speed )
                {
                    this.speed = 0;
                }
            }
            
            gpe.center_camera( 0, this.getx(), this.gety() );
            
        }
        this.render_under = function()
        {
            if( this.speed !=0 && this.targetX!=IS_NULL && this.targetY!=IS_NULL)
            {
                gpe.render_sprite(spr_xcoord, 0, this.targetX,this.targetY,1,1);
            }
        }
        this.render_hud = function()
        {
            gpe.render_text(-1,20,20,"Mouse direction "+gpe.round(gpe.degree(this.mouseDirection) )+"["+(gpe.get_camera_mouse_x(0)|0)+" , "+(gpe.get_camera_mouse_y(0)|0)+"]","white",fa_left,fa_top);
            gpe.render_text(-1,gpe.get_camera_screen_width(0)-64,32,"FPS:"+gpe.get_fps(),"white",fa_right,fa_top);
        }
        this.hasPreRenderFunction = true;
        this.hasHudRenderFunction = true;
        this.timedFunction0 = function()
        {
            this.canFire = true;
        }
        this.gpeHasTimedFunctions = true;
        this.gpeHasLogicFunction = true;
        this.collision_with_obj_block = function(other)
        {
            this.update_cords(this.xPast, this.yPast);
            this.speed = 0;
        }
        this.collide_with = function(otherObj)
        {
            switch(otherObj.gpeObjectType)
            {
                case 0:
                    this.collision_with_obj_block(otherObj);
                break;
                default:
                break;
            }
        };

        
        this.OBJECT_COLLISION_LIST.push(0);
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_player.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_player; 
}());


GPE.activate_object_families= function()
{
  GPR.GAME_OBJECTS_FAMILIES = [];
  GPR.GAME_OBJECTS_FAMILIES[0] = [];

  GPR.GAME_OBJECTS_FAMILIES[1] = [];

  GPR.GAME_OBJECTS_FAMILIES[2] = [];

  GPR.GAME_OBJECTS_FAMILIES[3] = [];

}


GPE.add_new_object = function(newObjType, newX, newY)
{
     var newObjOut = IS_NULL;
     switch( newObjType )
     {
         case 0:
             newObjOut = new GPE._obj_obj_block(newX, newY);
         break;
         case 1:
             newObjOut = new GPE._obj_obj_enemy_basic(newX, newY);
         break;
         case 2:
             newObjOut = new GPE._obj_obj_bullet(newX, newY);
         break;
         case 3:
             newObjOut = new GPE._obj_obj_player(newX, newY);
         break;
         default:
         break;
     }
     if(newObjOut!=IS_NULL)
     {
         newObjOut.xPos = newX;
         newObjOut.yPos = newY;
         GPR.add_object(newObjOut);
         return newObjOut;
     }
     return IS_NULL;
};
GPE.rsm.audioIsReadyForLoading = true; 
var _scn_temp_layer = IS_NULL; 

var _scn_newScene1 =  GPE.add_gamescene(); 
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
_scn_newScene1.scnStartBackgrounds[0]= {bgTexId:0,bgXPos: 0,bgYPos: 0,bgXSpeed: 0,bgYSpeed: 0,bgInFont: 0,bgTileHori: 1,bgTileVert: 1,bgStartStretch: 0}; 
_scn_newScene1.scnStartBackgrounds[1] = -1; 
_scn_newScene1.scnStartBackgrounds[2] = -1; 
_scn_newScene1.scnStartBackgrounds[3] = -1; 
_scn_newScene1.scnStartBackgrounds[4] = -1; 
_scn_newScene1.scnStartBackgrounds[5] = -1; 
_scn_newScene1.scnStartBackgrounds[6] = -1; 
_scn_newScene1.scnStartObjects.push( {objId:3,xPos: 160,yPos: 128,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:1,xPos: 224,yPos: 224,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:1,xPos: 416,yPos: 160,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:1,xPos: 672,yPos: 320,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:1,xPos: 512,yPos: 448,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:1,xPos: 736,yPos: 896,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:1,xPos: 480,yPos: 1024,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:1,xPos: 160,yPos: 1184,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:1,xPos: 672,yPos: 1216,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:1,xPos: 736,yPos: 96,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 64,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 32,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 160,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 160,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 64,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 96,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 128,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 320,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 256,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 224,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 192,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 128,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 96,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 64,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 32,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 192,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 288,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 288,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 256,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 288,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 288,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 256,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 224,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 384,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 320,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 352,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 416,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 448,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 480,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 512,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 544,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 480,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 480,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 448,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 384,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 352,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 416,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 512,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 576,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 640,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 768,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 768,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 736,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 608,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 640,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 672,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 736,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 704,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 768,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 800,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 832,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 864,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 896,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 928,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 960,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 992,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1024,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1056,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1120,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1088,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1184,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1152,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1216,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1248,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1280,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1344,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1312,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1408,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1376,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1408,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1440,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1472,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1536,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1568,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1600,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1632,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1664,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1696,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1728,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1792,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1856,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1888,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1920,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1952,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 1984,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 64,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 32,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 64,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 192,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 224,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 256,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 416,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 448,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 480,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 512,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 576,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 640,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 736,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 768,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 800,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 928,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 896,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 864,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 832,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 800,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 768,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 480,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 448,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 416,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 384,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 128,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 96,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 64,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 32,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 320,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 352,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 800,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 832,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 864,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 896,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 928,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 960,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 32,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 0,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 128,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 96,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 64,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 128,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 160,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 192,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 224,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 256,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 288,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 320,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 352,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 384,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 416,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 480,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 448,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 480,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 512,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 544,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 608,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 640,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 704,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 672,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 704,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 736,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 768,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 800,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 832,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 864,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 896,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 928,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 960,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1024,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1056,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 992,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1024,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1088,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1120,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1152,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1184,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1216,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1248,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1280,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1312,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1344,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1376,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1472,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1440,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1408,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1536,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1568,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1600,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1632,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1664,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1696,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1728,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1792,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1856,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1888,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1920,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1952,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1984,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 960,yPos: 2016,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 1120,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 576,yPos: 1120,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 1120,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 1152,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 1152,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 1184,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 1216,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 1248,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 32,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 128,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 96,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 64,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 96,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 128,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 192,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 224,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 256,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 320,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 320,yPos: 1792,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 320,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 320,yPos: 1856,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 320,yPos: 1888,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 992,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 960,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 928,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 896,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 864,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 832,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 800,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 768,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 736,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 1792,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 1728,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 1696,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 1664,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 1632,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 640,yPos: 1632,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 1632,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 576,yPos: 1632,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 1632,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 512,yPos: 1664,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 512,yPos: 1696,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 512,yPos: 1728,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 512,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 480,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 448,yPos: 1760,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 448,yPos: 1792,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 448,yPos: 1824,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 448,yPos: 1856,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 448,yPos: 1888,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 512,yPos: 1632,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 0,yPos: 544,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 96,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 192,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 224,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 256,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 256,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 224,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 192,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 128,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 96,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 64,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 32,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 96,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 192,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 256,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 576,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 960,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 672,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 640,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 608,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 640,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 672,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 704,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 736,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 768,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 800,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 608,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 640,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 672,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 704,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 736,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 768,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 800,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 864,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 896,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 928,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 960,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 832,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 832,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 864,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 896,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 928,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 960,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 992,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 1024,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 1056,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 544,yPos: 1088,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 960,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 992,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1024,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1056,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1088,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1120,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1152,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1184,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1216,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1248,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1280,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 224,yPos: 1312,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 192,yPos: 1312,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 1312,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 256,yPos: 1312,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1312,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 1344,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 1376,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 1408,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 1440,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 1472,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 160,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 192,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 224,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 256,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 288,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 320,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 352,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 384,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 416,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 640,yPos: 1280,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 1280,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1312,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1344,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1376,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1408,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1440,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1472,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1440,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1408,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1376,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1344,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1312,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 1280,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 608,yPos: 1280,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 800,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 768,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 736,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 832,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 864,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 896,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 928,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 960,yPos: 1504,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 704,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 928,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 896,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 864,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 832,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 672,yPos: 576,customComponents: {}}); 
_scn_newScene1.scnStartObjects.push( {objId:0,xPos: 640,yPos: 576,customComponents: {}}); 
//Start of Project Macros 
var PLAYER_OBJECT = IS_NULL;//End of Project Macros 
