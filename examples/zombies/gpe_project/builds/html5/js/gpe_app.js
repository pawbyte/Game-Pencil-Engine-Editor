/*     
#    --------------------------------------------------  #
#       
#       
#  Game Pencil Engine Game File 
#  Created automatically via the Game Pencil Engine Editor Version 1.0.2
#  Warning: Manually editing this file may cause unexpected bugs and errors. 
#  If you have any problems reading this file please report it to debug@pawyte.com . 
#     
#      
#    --------------------------------------------------  # 
*/     

'use strict';






var newObject1 =  0; 
var newScene1 =  0; 

GPE._obj_newObject1 =  (function (xPosIn, yPosIn) 
{ 
    function _obj_newObject1 (xPosIn, yPosIn) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn); 
        this.gpeObjectType =0; 
        this.init_sprite(-1);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 0;
        
        
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_newObject1.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_newObject1; 
}());


GPE.activate_object_families= function()
{
  GPR.GAME_OBJECTS_FAMILIES = [];
  GPR.GAME_OBJECTS_FAMILIES[0] = [];

}


var _scn_temp_layer = IS_NULL; 

var _scn_newScene1 =  GPE.add_gamescene(); 
_scn_newScene1.sceneIdNumber = 0;
_scn_newScene1.sceneName = "newScene1";
_scn_newScene1.sceneCaption = "";
_scn_newScene1.sceneWidth = 4096;
_scn_newScene1.sceneHeight = 4096;
_scn_newScene1.tileWidth  = 32;
_scn_newScene1.tileHeight = 32;
_scn_newScene1.tileAmountX = 128;
_scn_newScene1.tileAmountY = 128;
_scn_newScene1.bgColor = '#000000';
_scn_newScene1.sceneIsContinuous = 0;
_scn_newScene1.scnStartBackgrounds[0] = -1; 
_scn_newScene1.scnStartBackgrounds[1] = -1; 
_scn_newScene1.scnStartBackgrounds[2] = -1; 
_scn_newScene1.scnStartBackgrounds[3] = -1; 
_scn_newScene1.scnStartBackgrounds[4] = -1; 
_scn_newScene1.scnStartBackgrounds[5] = -1; 
_scn_newScene1.scnStartBackgrounds[6] = -1; 
GPE.add_new_object = function(newObjType, newX, newY)
{
     var newObjOut = IS_NULL;
     switch( newObjType )
     {
         case 0:
             newObjOut = new GPE._obj_newObject1(newX, newY);
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
//Start of Project Macros 
//End of Project Macros 
