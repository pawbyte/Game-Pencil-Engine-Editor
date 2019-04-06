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
var tex_sky =  GPE.rsm.add_texture(0,'resources/textures/sky2.jpg'); 
var newTexture2 =  GPE.rsm.add_texture(1,'resources/textures/gpe_discount_20bucks.png'); 
var spr_crow =  GPE.rsm.add_sprite(0,4,'resources/animations/spr_crow.png',0,0,32,32,0,0,0,32,32);
var spr_ground =  GPE.rsm.add_sprite(1,4,'resources/animations/spr_season_grass.png',0,0,64,256,0,0,0,64,256);
var newSprite3 = GPE.rsm.add_sprite_empty();
var obj_crow =  0; 
var obj_ground =  1; 
var obj_main_menu =  2; 
var scn_title =  0; 
var  _scn_scn_title =  GPE.add_gamescene(0 , 'scn_title'); 
var scn_game =  1; 
var  _scn_scn_game =  GPE.add_gamescene(1 , 'scn_game'); 


GPE._obj_obj_crow =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_crow (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =0; 
        this.init_sprite(0);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
        this.imageSpeed = 0.125;
            
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
        this.isViewIndependent = 0;
        gpe.scene_enter_next();
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_main_menu.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_main_menu; 
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
_scn_scn_title.sceneWidth = 800;
_scn_scn_title.sceneHeight = 480;
_scn_scn_title.defaultTileWidth  = 32;
_scn_scn_title.defaultTileHeight = 32;
_scn_scn_title.tileAmountX = 25;
_scn_scn_title.tileAmountY = 15;
_scn_scn_title.bgColor = '#000000';
_scn_scn_title.sceneIsContinuous = 0;
var newBranch = {};
    newBranch = _scn_temp_layer = newGPEScene_000.add_layer( 0,0);
    newBranch.init_basic(    newBranch = _scn_temp_layer = newGPEScene_000.add_layer( 1,0);
    newBranch.init_basic(newGPEScene_000 = _scn_scn_game;
_scn_scn_game.sceneId = 1;
_scn_scn_game.sceneName = "scn_game";
_scn_scn_game.sceneCaption = "";
_scn_scn_game.sceneWidth = 800;
_scn_scn_game.sceneHeight = 480;
_scn_scn_game.defaultTileWidth  = 32;
_scn_scn_game.defaultTileHeight = 32;
_scn_scn_game.tileAmountX = 25;
_scn_scn_game.tileAmountY = 15;
_scn_scn_game.bgColor = '#000000';
_scn_scn_game.sceneIsContinuous = 0;
var newBranch = {};
    newBranch = _scn_temp_layer = newGPEScene_000.add_layer( 0,0);
    newBranch.init_basic(    newBranch = _scn_temp_layer = newGPEScene_000.add_layer( 1,0);
    newBranch.init_basic(//Start of Project Macros 
//End of Project Macros 
