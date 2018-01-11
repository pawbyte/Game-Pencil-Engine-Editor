/*     
#    --------------------------------------------------  #
#       
#       
#  Game Pencil Engine Game File 
#  Created automatically via the Game Pencil Engine Editor Version 2
#  Warning: Manually editing this file may cause unexpected bugs and errors. 
#  If you have any problems reading this file please report it to help@pawbyte.com . 
#     
#      
#    --------------------------------------------------  # 
*/     

'use strict';
var bg_death =  GPE.rsm.add_texture(0,'resources/textures/gg_Screen_plat.png'); 
var bg_sky =  GPE.rsm.add_texture(1,'resources/textures/bg.png'); 
var ts_map =  GPE.rsm.add_tilesheet(0,'resources/tilesheets/grassmap.png',70,70,0,0,0,0); 
var spr_slime =  GPE.rsm.add_sprite(0,2,'resources/sprites/enemy_walk.png',0,0,51,27,0,0,0,51,27);
var spr_block =  GPE.rsm.add_sprite(1,1,'resources/sprites/spr_70x70_block.png',0,0,70,70,0,0,0,70,70);
var spr_player =  GPE.rsm.add_sprite(2,11,'resources/sprites/player_walk.png',0,0,72,97,0,16,0,48,97);
var spr_heartpiece =  GPE.rsm.add_sprite(3,1,'resources/sprites/spr_platformer_heart_piece.png',0,0,32,32,0,6,6,20,20);
var spr_coin =  GPE.rsm.add_sprite(4,1,'resources/sprites/spr_coin.png',0,0,32,32,0,12,4,12,24);
var newFont1 =  GPE.rsm.add_font(0,'Arial','','','','','',12,32,32,0);
var obj_slime =  0; 
var obj_block =  1; 
var obj_heart_piece =  2; 
var obj_player =  3; 
var obj_coin =  4; 
var obj_player2 =  5; 
var newScene1 =  0; 
var  _scn_newScene1 =  GPE.add_gamescene(0 , 'newScene1'); 
var game_win =  1; 
var  _scn_game_win =  GPE.add_gamescene(1 , 'game_win'); 
var game_over =  2; 
var  _scn_game_over =  GPE.add_gamescene(2 , 'game_over'); 


GPE._obj_obj_slime =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_slime (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =0; 
        this.init_sprite(0);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
        this.horiSpeed = 2;
        this.perform_object_logic = function()
        {
            //Scans for a block in the way of slime's movement
            if( gpe.position_collides(this,this.getx()+this.horiSpeed, this.gety()-4,obj_block)!=IS_NULL)
            {
                //if found one it flips direction and updates its x/y coords to before it collided
                this.update_cords(this.xPast, this.yPast);
                this.horiSpeed*=-1;
            }
            else
            {
                //does the same check for other slimes nearby
                var nearFriend = gpe.position_collides(this,this.getx()+this.horiSpeed, this.gety(),obj_slime)
                if( nearFriend!=IS_NULL)
                {
                    if( nearFriend.getx() < this.getx() )
                    {
                        nearFriend.horiSpeed = gpe.abs(nearFriend.horiSpeed)*-1;
                        this.horiSpeed = gpe.abs(nearFriend.horiSpeed);
                    }
                    else
                    {
                        nearFriend.horiSpeed = gpe.abs(nearFriend.horiSpeed);
                        this.horiSpeed = gpe.abs(nearFriend.horiSpeed)*-1;
                    }
                    /*if( this.canFlipValues && nearFriend.canFlipValues )
                    {
                        this.horiSpeed *= -1;
                        nearFriend.horiSpeed = -gpe.sign(this.horiSpeed)*nearFriend.horiSpeed;
                        this.canFlipValues = false;
                        nearFriend.canFlipValues = false;
                    }*/
                }
            }
            //moves by its horizontal speed
            this.move_right( this.horiSpeed);
            //calculates gravity
            if (gpe.position_collides(this,this.getx(), this.gety() + 1, obj_block)==IS_NULL && gpe.position_collides(this,this.getx(), this.gety() + 1, obj_slime)==IS_NULL )
            {
                this.yspeed += 1;
            }
            if( this.yspeed!=0)
            {
                var _abs = gpe.abs(this.yspeed);
                var _sign = gpe.sign(this.yspeed);
                for (var i = 0; i < _abs; i++)
                {
                    if (gpe.position_collides(this,this.getx(), this.gety() + _sign, obj_block)!=IS_NULL)
                    {
                        // hit ground/ceiling
                        this.yspeed = 0;
                        this.canJump = true
                        break;
                    }
                    else if (gpe.position_collides(this,this.getx(), this.gety() + _sign, obj_slime)!=IS_NULL)
                    {
                        // hit ground/ceiling
                        this.yspeed = 0;
                        this.canJump = true
                        break;
                        this.move_up( _sign);
                    }
                    else this.move_down( _sign);
                }
            }
        }
        this.render_self = function()
        {
            gpe.render_sprite( this.get_sprite(), this.spriteFrame,this.getx(), this.gety(),gpe.sign(this.horiSpeed) );
        }
        this.hasRenderFunction = true;
        this.gpeHasLogicFunction = true;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_slime.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_slime; 
}());

GPE._obj_obj_block =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_block (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =1; 
        this.init_sprite(1);
        this.isVisible = 0;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_block.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_block; 
}());

GPE._obj_obj_heart_piece =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_heart_piece (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =2; 
        this.init_sprite(3);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
        this.start_countdown(0,15);
        this.movementDirection = 0.25;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_heart_piece.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_heart_piece; 
}());

GPE._obj_obj_player =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_player (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =3; 
        this.init_sprite(2);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        this.imageSpeed = 0.25;
        this.onGround = false;
        this.gravityEffect = 0.5;
        this.yspeed = 0;
        this.isJumping = false;
        this.isRunning = false;
        this.canRun = true;
        this.canJump = true;
        this.isWalking = true;
        this.myDirection = 1;
        this.can_hurt = true;
        this.start_countdown( 1, 5);
        this.coins = 0;
        this.lives = 3;
        this.score = 0;
        this.hearts = 3;
        this.heatsMax = 3;
        this.renderSelf = true;
        this.perform_object_logic = function()
        {
            if( gpe.get_object_count( obj_slime ) ==0 )
            {
                gpe.scene_enter(game_win);
            }
            if( this.canRun==true && gpe.check_keyboard(gp_x) )
            {
                this.isRunning = true;
                this.isWalking = false;
            }
            else
            {
                this.isRunning = false;
                this.isWalking = true;
            }
            if( gpe.check_keyboard( gp_r ) )
            {
                gpe.scene_reset();
            }
            if( gpe.check_keyboard( gp_left) )
            {
                if( this.isRunning && this.getx() >=6 )
                {
                    if( gpe.position_collides(this,this.getx()-6, this.gety(), obj_block)==IS_NULL && this.getx()-4 >=0 )
                    {
                        this.move_left(6);
                    }
                }
                else if( gpe.position_collides(this,this.getx()-3, this.gety(), obj_block)==IS_NULL && this.getx()-3 >=0 )
                {
                    this.move_left(3);
                }
                this.myDirection = -1;
            }
            else if( gpe.check_keyboard( gp_right) )
            {
                if( this.isRunning && this.getx() >=6 )
                {
                    if( gpe.position_collides(this,this.getx()+6, this.gety(), obj_block)==IS_NULL  )
                    {
                        this.move_right(6);
                    }
                }
                else if( gpe.position_collides(this,this.getx()+3, this.gety(), obj_block)==IS_NULL  )
                {
                    this.move_right(3);
                }
                this.myDirection = 1;
            }
            else
            {
                this.isRunning = false;
                this.isWalking = false;
            }
            if (gpe.position_collides(this,this.getx(), this.gety() + 1, obj_block)!=IS_NULL )
            {
                if (gpe.check_keyboard_pressed(gp_up) && this.canJump)
                {
                    if (gpe.position_collides(this,this.getx(), this.gety() -4, obj_block)==IS_NULL )
                    {
                        if( this.isRunning)
                        {
                            this.yspeed = -16;
                        }
                        else
                        {
                            this.yspeed = -14;
                        }
                        this.canJump = false;
                        this.move_up(4);
                    }
                }
            }
            else this.yspeed += 0.5;
            var _abs = gpe.abs(this.yspeed);
            var _sign = gpe.sign(this.yspeed);
            var foundEnemy = IS_NULL;
            for (var i = 0; i < _abs; i++)
            {
                if (gpe.position_collides(this,this.getx(), this.gety() + _sign, obj_block)!=IS_NULL)
                {
                    // hit ground/ceiling
                        this.yspeed = 0;
                        this.canJump = true
                        break;
                }
                else
                {
                    this.move_down(_sign);
                }
            }
            if( this.isRunning)
            {
                //this.set_sprite( );
            }
            else if(this.isWalking)
            {
                //this.set_sprite( );
            }
            else
            {
                //this.set_sprite();
            }
            gpe.center_camera( 0,this.getx(), this.gety() );
            if( this.hearts <=0 )
            {
                gpe.scene_enter( game_over);
            }
        }
        this.render_self = function()
        {
            if(this.renderSelf)
            {
                gpe.render_sprite( this.get_sprite(), this.spriteFrame,this.getx(), this.gety(),gpe.sign(this.myDirection) );
            }
        }
        this.render_hud = function()
        {
            gpe.render_text(-1,gpe.get_camera_screen_width(0)-64,32,"FPS:"+gpe.get_fps(),"white",fa_right,fa_top);
            if( gpe.get_current_scene_id() == game_win )
            {
                gpe.render_text(-1,0,32,"You win!","white",fa_left,fa_top);
            }
            else
            {
                gpe.render_text(-1,0,32,"Destroy all enemies to win!","white",fa_left,fa_top);
                gpe.render_text(-1,0,96,"Coins:"+this.coins,"white",fa_left,fa_top);
                gpe.render_text(-1,0,128,"GamePad Button 0 down:"+gpe.check_gamepad_down(0, 0 ),"white",fa_left,fa_top);
                for( var i = 0; i < this.hearts; i++)
                {
                    gpe.render_sprite( spr_heartpiece, 0,i*32,64);
                }
            }
        }
        this.hasRenderFunction = true;
        this.hasHudRenderFunction = true;
        this.timedFunction0 = function()
        {
            this.can_hurt = true;
            this.renderSelf = true;
            console.log("Can hurt now...");
        }
        this.timedFunction1 = function()
        {
            if( this.can_hurt == false )
            {
                this.renderSelf = !this.renderSelf;
            }
            this.start_countdown( 1, 5);
        }
        this.gpeHasTimedFunctions = true;
        this.gpeHasLogicFunction = true;
        this.scene_start = function()
        {
            console.log( "Value="+this.get_component("player_Variable234") );
        }
        this.collision_with_obj_heart_piece = function(other)
        {
            other.self_destruct();
            if( this.hearts < this.heatsMax)
            {
                this.hearts++;
            }
        }
        this.collision_with_obj_slime = function(other)
        {
            if( this.gety2() <= other.gety()+other.get_collision_height()/2 )
            {
                if( this.can_hurt )
                {
                    other.self_destruct();
                    this.yspeed = -14;
                }
            }
            else if( this.can_hurt && this.hearts > 0)
            {
                this.hearts -=1;
                this.can_hurt = false;
                this.start_timer( 0, 2);
            }
        }
        this.collision_with_obj_coin = function(other)
        {
            other.self_destruct();
            this.coins+=1;
        }
        this.collide_with = function(otherObj)
        {
            switch(otherObj.gpeObjectType)
            {
                case 2:
                    this.collision_with_obj_heart_piece(otherObj);
                break;
                case 0:
                    this.collision_with_obj_slime(otherObj);
                break;
                case 4:
                    this.collision_with_obj_coin(otherObj);
                break;
                default:
                break;
            }
        };

            
            this.OBJECT_COLLISION_LIST.push(2);
            this.OBJECT_COLLISION_LIST.push(0);
            this.OBJECT_COLLISION_LIST.push(4);
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_player.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_player; 
}());

GPE._obj_obj_player2 =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_player2 (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =5; 
        this.init_sprite(2);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        this.imageSpeed = 0.25;
        this.onGround = false;
        this.gravityEffect = 0.5;
        this.yspeed = 0;
        this.isJumping = false;
        this.isRunning = false;
        this.canRun = true;
        this.canJump = true;
        this.isWalking = true;
        this.myDirection = 1;
        this.can_hurt = true;
        this.start_countdown( 1, 5);
        this.coins = 0;
        this.lives = 3;
        this.score = 0;
        this.hearts = 3;
        this.heatsMax = 3;
        this.renderSelf = true;
        gpe.enable_camera(1);
        gpe.setup_camera();
        this.perform_object_logic = function()
        {
            if( gpe.get_object_count( obj_slime ) ==0 )
            {
                gpe.scene_enter(game_win);
            }
            if( this.canRun==true && gpe.check_keyboard(gp_x) )
            {
                this.isRunning = true;
                this.isWalking = false;
            }
            else
            {
                this.isRunning = false;
                this.isWalking = true;
            }
            if( gpe.check_keyboard( gp_r ) )
            {
                gpe.scene_reset();
            }
            if( gpe.check_keyboard( gp_left) )
            {
                if( this.isRunning && this.getx() >=6 )
                {
                    if( gpe.position_collides(this,this.getx()-6, this.gety(), obj_block)==IS_NULL && this.getx()-4 >=0 )
                    {
                        this.move_left(6);
                    }
                }
                else if( gpe.position_collides(this,this.getx()-3, this.gety(), obj_block)==IS_NULL && this.getx()-3 >=0 )
                {
                    this.move_left(3);
                }
                this.myDirection = -1;
            }
            else if( gpe.check_keyboard( gp_right) )
            {
                if( this.isRunning && this.getx() >=6 )
                {
                    if( gpe.position_collides(this,this.getx()+6, this.gety(), obj_block)==IS_NULL  )
                    {
                        this.move_right(6);
                    }
                }
                else if( gpe.position_collides(this,this.getx()+3, this.gety(), obj_block)==IS_NULL  )
                {
                    this.move_right(3);
                }
                this.myDirection = 1;
            }
            else
            {
                this.isRunning = false;
                this.isWalking = false;
            }
            if (gpe.position_collides(this,this.getx(), this.gety() + 1, obj_block)!=IS_NULL )
            {
                if (gpe.check_keyboard_pressed(gp_up) && this.canJump)
                {
                    if (gpe.position_collides(this,this.getx(), this.gety() -4, obj_block)==IS_NULL )
                    {
                        if( this.isRunning)
                        {
                            this.yspeed = -16;
                        }
                        else
                        {
                            this.yspeed = -14;
                        }
                        this.canJump = false;
                        this.move_up(4);
                    }
                }
            }
            else this.yspeed += 0.5;
            var _abs = gpe.abs(this.yspeed);
            var _sign = gpe.sign(this.yspeed);
            var foundEnemy = IS_NULL;
            for (var i = 0; i < _abs; i++)
            {
                if (gpe.position_collides(this,this.getx(), this.gety() + _sign, obj_block)!=IS_NULL)
                {
                    // hit ground/ceiling
                        this.yspeed = 0;
                        this.canJump = true
                        break;
                }
                else
                {
                    this.move_down(_sign);
                }
            }
            if( this.isRunning)
            {
                //this.set_sprite( );
            }
            else if(this.isWalking)
            {
                //this.set_sprite( );
            }
            else
            {
                //this.set_sprite();
            }
            gpe.center_camera( 1,this.getx(), this.gety() );
            if( this.hearts <=0 )
            {
                gpe.scene_enter( game_over);
            }
        }
        this.render_self = function()
        {
            if(this.renderSelf)
            {
                gpe.render_sprite( this.get_sprite(), this.spriteFrame,this.getx(), this.gety(),gpe.sign(this.myDirection) );
            }
        }
        this.render_hud = function()
        {
            gpe.render_text(-1,gpe.get_camera_screen_width(0)-64,32,"FPS:"+gpe.get_fps(),"white",fa_right,fa_top);
            if( gpe.get_current_scene_id() == game_win )
            {
                gpe.render_text(-1,0,32,"You win!","white",fa_left,fa_top);
            }
            else
            {
                gpe.render_text(-1,0,32,"Destroy all enemies to win!","white",fa_left,fa_top);
                gpe.render_text(-1,0,96,"Coins:"+this.coins,"white",fa_left,fa_top);
                gpe.render_text(-1,0,128,"GamePad Button 0 down:"+gpe.check_gamepad_down(0, 0 ),"white",fa_left,fa_top);
                for( var i = 0; i < this.hearts; i++)
                {
                    gpe.render_sprite( spr_heartpiece, 0,i*32,64);
                }
            }
        }
        this.hasRenderFunction = true;
        this.hasHudRenderFunction = true;
        this.timedFunction0 = function()
        {
            this.can_hurt = true;
            this.renderSelf = true;
            console.log("Can hurt now...");
        }
        this.timedFunction1 = function()
        {
            if( this.can_hurt == false )
            {
                this.renderSelf = !this.renderSelf;
            }
            this.start_countdown( 1, 5);
        }
        this.gpeHasTimedFunctions = true;
        this.gpeHasLogicFunction = true;
        this.scene_start = function()
        {
            console.log( "Value="+this.get_component("player_Variable234") );
        }
        this.collision_with_obj_heart_piece = function(other)
        {
            other.self_destruct();
            if( this.hearts < this.heatsMax)
            {
                this.hearts++;
            }
        }
        this.collision_with_obj_slime = function(other)
        {
            if( this.gety2() <= other.gety()+other.get_collision_height()/2 )
            {
                if( this.can_hurt )
                {
                    other.self_destruct();
                    this.yspeed = -14;
                }
            }
            else if( this.can_hurt && this.hearts > 0)
            {
                this.hearts -=1;
                this.can_hurt = false;
                this.start_timer( 0, 2);
            }
        }
        this.collision_with_obj_coin = function(other)
        {
            other.self_destruct();
            this.coins+=1;
        }
        this.collide_with = function(otherObj)
        {
            switch(otherObj.gpeObjectType)
            {
                case 2:
                    this.collision_with_obj_heart_piece(otherObj);
                break;
                case 0:
                    this.collision_with_obj_slime(otherObj);
                break;
                case 4:
                    this.collision_with_obj_coin(otherObj);
                break;
                default:
                break;
            }
        };

            
            this.OBJECT_COLLISION_LIST.push(2);
            this.OBJECT_COLLISION_LIST.push(0);
            this.OBJECT_COLLISION_LIST.push(4);
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_player2.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_player2; 
}());

GPE._obj_obj_coin =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_coin (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE._obj_obj_heart_piece.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =4; 
        this.init_sprite(4);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_coin.prototype = new GPE._obj_obj_heart_piece(xPosIn, yPosIn); 
    return _obj_obj_coin; 
}());


GPE.activate_object_families= function()
{
  GPR.GAME_OBJECTS_FAMILIES = [];
  GPR.GAME_OBJECTS_FAMILIES[0] = [];

  GPR.GAME_OBJECTS_FAMILIES[1] = [];

  GPR.GAME_OBJECTS_FAMILIES[2] = [];
  GPR.GAME_OBJECTS_FAMILIES[2].push(4);

  GPR.GAME_OBJECTS_FAMILIES[3] = [];

  GPR.GAME_OBJECTS_FAMILIES[4] = [];

  GPR.GAME_OBJECTS_FAMILIES[5] = [];

}


GPE.add_new_object = function(newObjType, newX, newY, objectLayerId )
{
if( typeof objectLayerId!="number"){objectLayerId = 1;}
     var newObjOut = IS_NULL;
     switch( newObjType )
     {
         case 0:
             newObjOut = new GPE._obj_obj_slime(newX, newY, objectLayerId);
         break;
         case 1:
             newObjOut = new GPE._obj_obj_block(newX, newY, objectLayerId);
         break;
         case 2:
             newObjOut = new GPE._obj_obj_heart_piece(newX, newY, objectLayerId);
         break;
         case 3:
             newObjOut = new GPE._obj_obj_player(newX, newY, objectLayerId);
         break;
         case 5:
             newObjOut = new GPE._obj_obj_player2(newX, newY, objectLayerId);
         break;
         case 4:
             newObjOut = new GPE._obj_obj_coin( newX, newY,objectLayerId);
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
_scn_newScene1.sceneWidth = 3500;
_scn_newScene1.sceneHeight = 1080;
_scn_newScene1.tileWidth  = 70;
_scn_newScene1.tileHeight = 70;
_scn_newScene1.tileAmountX = 50;
_scn_newScene1.tileAmountY = 16;
_scn_newScene1.bgColor = '#3F91B1';
_scn_newScene1.sceneIsContinuous = 0;
_scn_temp_layer = _scn_newScene1.add_layer( 1,0,0);
_scn_temp_layer.scnStartBackgrounds.push( {bgTexId:1,bgXPos: 0,bgYPos: 0,bgXSpeed: 0,bgYSpeed: 0,bgTileHori: 1,bgTileVert: 1,bgStartStretch: 0} ); 
_scn_temp_layer = _scn_newScene1.add_layer( 2,1,0);
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 490,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 560,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 630,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 700,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 770,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 630,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 490,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 420,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 280,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 210,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 140,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 70,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 280,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 350,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 420,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 210,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 420,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 490,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 560,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 630,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 70,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 140,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 560,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 420,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 210,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 280,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 350,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 420,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 490,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 630,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 700,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 770,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 840,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1400,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1330,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1260,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1190,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1120,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1050,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 910,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1960,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1680,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1610,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1540,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1470,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1540,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1890,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1820,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1750,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2030,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2100,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2170,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2240,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2310,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2380,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2520,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2590,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2660,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2730,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2800,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2870,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2940,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3010,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3080,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3150,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3360,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3290,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3220,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3290,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3360,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3220,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3150,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3080,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3010,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2870,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2940,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3010,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3080,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3150,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3220,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3290,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3360,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3290,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3150,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2940,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2800,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2380,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2520,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2590,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2660,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2730,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2800,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2170,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2100,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2030,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2100,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2170,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2240,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2310,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2240,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2030,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1960,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1890,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1820,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1750,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1680,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1330,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1400,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1470,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1540,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1610,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1540,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1400,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1190,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1260,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1330,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1260,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1120,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 840,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 910,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1050,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1120,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 140,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 210,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 350,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 420,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 490,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 560,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 630,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 70,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1050,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1050,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1120,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1260,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1330,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1330,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1260,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1190,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1120,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1190,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1750,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1750,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1750,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1750,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1750,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1750,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1680,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1680,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1610,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1820,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1890,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1960,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2030,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2100,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2590,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2660,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2590,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2520,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3220,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3150,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3080,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3010,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 490,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3360,yPos: 490,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2940,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3010,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3080,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3150,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2870,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2800,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2730,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2660,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2590,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2520,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2170,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2310,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2030,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2100,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1960,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1890,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1820,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1750,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1610,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1680,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1540,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1190,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1260,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1190,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1120,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1050,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 910,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 840,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 770,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 700,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 630,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 560,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 490,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 420,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 350,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 280,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 210,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 140,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 70,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 910,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 840,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 770,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 700,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 630,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 630,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 700,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 770,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 840,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 910,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 490,yPos: 910,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 700,yPos: 840,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 560,yPos: 840,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 1470,yPos: 910,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 1400,yPos: 910,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 2310,yPos: 840,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 2030,yPos: 910,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 1890,yPos: 910,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 140,yPos: 280,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 420,yPos: 280,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 560,yPos: 280,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2310,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2450,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2520,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2450,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2380,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2310,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2240,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1330,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1400,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 1470,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 2030,yPos: 210,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 2240,yPos: 280,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 2380,yPos: 280,customComponents: {test123: '',testing: "testing"}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2450,yPos: 980,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2450,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 210,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 420,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:3,xPos: 140,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 280,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 140,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 2380,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 1960,yPos: 700,customComponents: {testing: "testing",test123: ''}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 2100,yPos: 700,customComponents: {testing: "testing",test123: ''}}); 
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 1820,yPos: 630,customComponents: {testing: "testing",test123: ''}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 560,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 630,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 3430,yPos: 840,customComponents: {}}); 
_scn_temp_layer = _scn_newScene1.add_layer( 0,3,0);
_scn_temp_layer.scnStartTiles.push( {tileNumber: 0,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 1,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 2,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 3,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 4,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 5,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 6,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 7,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 8,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 9,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 10,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 11,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 12,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 13,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 14,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 15,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 16,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 17,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 18,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 19,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 20,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 21,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 22,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 23,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 24,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 25,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 26,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 27,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 28,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 29,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 30,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 31,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 32,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 33,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 34,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 35,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 36,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 37,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 38,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 39,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 40,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 41,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 42,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 43,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 44,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 45,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 46,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 47,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 48,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 49,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 50,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 59,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 60,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 61,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 62,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 63,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 64,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 99,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 100,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 109,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 110,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 111,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 112,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 113,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 114,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 149,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 150,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 199,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 200,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 236,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 237,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 238,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 239,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 240,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 241,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 242,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 243,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 244,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 245,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 249,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 250,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 251,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 252,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 253,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 254,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 255,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 256,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 257,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 258,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 259,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 260,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 261,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 262,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 263,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 264,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 265,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 266,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 267,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 268,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 269,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 270,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 271,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 272,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 273,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 274,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 275,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 276,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 277,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 278,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 279,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 280,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 281,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 282,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 283,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 284,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 285,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 286,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 299,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 300,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 349,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 350,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 398,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 399,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 400,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 449,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 450,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 499,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 500,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 501,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 502,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 503,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 504,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 505,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 506,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 507,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 508,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 509,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 533,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 534,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 536,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 537,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 538,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 543,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 544,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 545,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 546,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 549,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 550,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 575,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 576,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 577,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 578,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 579,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 580,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 599,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 600,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 615,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 616,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 617,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 618,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 619,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 624,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 625,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 649,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 650,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 665,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 666,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 667,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 668,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 669,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 673,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 674,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 675,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 699,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 700,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 701,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 702,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 703,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 704,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 705,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 706,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 707,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 708,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 709,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 710,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 711,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 712,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 713,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 714,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 715,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 716,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 717,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 718,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 719,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 720,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 721,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 722,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 723,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 724,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 725,tileSheetId: 0,tileIndexId: 1}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 726,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 727,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 728,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 729,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 730,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 731,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 732,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 733,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 734,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 735,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 736,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 737,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 738,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 739,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 740,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 741,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 742,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 743,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 744,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 745,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 746,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 747,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 748,tileSheetId: 0,tileIndexId: 16}); 
_scn_temp_layer.scnStartTiles.push( {tileNumber: 749,tileSheetId: 0,tileIndexId: 16}); 
_scn_game_win.sceneId = 1;
_scn_game_win.sceneName = "game_win";
_scn_game_win.sceneCaption = "";
_scn_game_win.sceneWidth = 1024;
_scn_game_win.sceneHeight = 1024;
_scn_game_win.tileWidth  = 72;
_scn_game_win.tileHeight = 72;
_scn_game_win.tileAmountX = 15;
_scn_game_win.tileAmountY = 15;
_scn_game_win.bgColor = '#6EB7B7';
_scn_game_win.sceneIsContinuous = 0;
_scn_temp_layer = _scn_game_win.add_layer( 1,0,0);
_scn_temp_layer.scnStartBackgrounds.push( {bgTexId:1,bgXPos: 0,bgYPos: 0,bgXSpeed: 0,bgYSpeed: 0,bgTileHori: 1,bgTileVert: 1,bgStartStretch: 0} ); 
_scn_temp_layer = _scn_game_win.add_layer( 2,1,0);
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 210,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 420,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 490,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 560,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 630,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 0,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 210,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 280,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 350,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 420,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 490,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 560,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 630,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 700,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 770,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 700,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 560,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 490,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 420,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 350,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 280,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 210,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 140,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 70,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 840,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 910,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 910,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 840,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 770,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 630,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 630,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 560,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 420,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 140,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 490,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 350,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 280,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 210,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 70,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 980,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 910,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 840,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 770,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 700,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 630,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 560,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 490,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 420,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 350,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 280,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 210,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 140,yPos: 0,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:3,xPos: 210,yPos: 700,customComponents: {}}); 
_scn_temp_layer.scnStartObjects.push( {objId:1,xPos: 70,yPos: 0,customComponents: {}}); 
_scn_temp_layer = _scn_game_win.add_layer( 0,2,0);
_scn_game_over.sceneId = 2;
_scn_game_over.sceneName = "game_over";
_scn_game_over.sceneCaption = "";
_scn_game_over.sceneWidth = 720;
_scn_game_over.sceneHeight = 720;
_scn_game_over.tileWidth  = 32;
_scn_game_over.tileHeight = 32;
_scn_game_over.tileAmountX = 23;
_scn_game_over.tileAmountY = 23;
_scn_game_over.bgColor = '#000000';
_scn_game_over.sceneIsContinuous = 0;
_scn_temp_layer = _scn_game_over.add_layer( 1,0,0);
_scn_temp_layer.scnStartBackgrounds.push( {bgTexId:0,bgXPos: 0,bgYPos: 0,bgXSpeed: 0,bgYSpeed: 0,bgTileHori: 1,bgTileVert: 1,bgStartStretch: 0} ); 
_scn_temp_layer = _scn_game_over.add_layer( 2,1,0);
//Start of Project Macros 
var lol = ok;
//End of Project Macros 
