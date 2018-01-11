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
var obj_controller =  0; 
var newScene1 =  0; 
var  _scn_newScene1 =  GPE.add_gamescene(0 , 'newScene1'); 

GPE._obj_obj_controller =  (function (xPosIn, yPosIn,objectLayerId) 
{ 
    function _obj_obj_controller (xPosIn, yPosIn,objectLayerId) 
    { 
        GPE.GPE_GameObject.call(this,xPosIn, yPosIn,objectLayerId); 
        this.gpeObjectType =0; 
        this.init_sprite(-1);
        this.isVisible = 1;
        this.gpeIsContinuousObject = 0;
        this.isViewIndependent = 1;
        //Toy around with it
        this.phrase = "Hello World!";
        this.self_obliterate = function()
        {
            //For its final action it writes to the console its sad note
            console.log(" Goodbye world!");
        }
        this.perform_object_logic = function()
        {
            //If you press X check the console window in your browser.
            if( gpe.check_keyboard(gp_x) )
            {
                this.self_destruct();
            }
            //To make this example slightly more interesting use WASD to change the phrase
            if( gpe.check_keyboard(gp_a) )
            {
                this.phrase = "Hello world";
            }
            else if( gpe.check_keyboard(gp_d) )
            {
                this.phrase = "Sup?";
            }
            else if( gpe.check_keyboard(gp_w) )
            {
                this.phrase = "Duuuuuubya";
            }
            else if( gpe.check_keyboard(gp_s) )
            {
                this.phrase = "Ello Guvnah!";
            }
        }
        this.render_self = function()
        {
            /*
            Renders the phrase ("Hello World") to the screen at this x/y coordinates aligned top/left.
            With the 1st parameter being the -1 it will use the "Browser's" default font to render it.
            */
            gpe.render_text(-1,this.getx(), this.gety(),this.phrase,"white",fa_left, fa_top );
            gpe.render_text(-1,this.getx(), this.gety()+32,"Use the WASD keys to change text","yellow",fa_left, fa_top );
        }
        this.hasRenderFunction = true;
        this.gpeHasLogicFunction = true;
            
        this.OBJECT_COLLISION_LIST = GPE.make_array_unique(this.OBJECT_COLLISION_LIST);
    } 

    _obj_obj_controller.prototype = new GPE.GPE_GameObject(xPosIn, yPosIn); 
    return _obj_obj_controller; 
}());


GPE.activate_object_families= function()
{
  GPR.GAME_OBJECTS_FAMILIES = [];
  GPR.GAME_OBJECTS_FAMILIES[0] = [];

}


GPE.add_new_object = function(newObjType, newX, newY, objectLayerId )
{
if( typeof objectLayerId!="number"){objectLayerId = 1;}
     var newObjOut = IS_NULL;
     switch( newObjType )
     {
         case 0:
             newObjOut = new GPE._obj_obj_controller(newX, newY, objectLayerId);
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
_scn_newScene1.sceneWidth = 4096;
_scn_newScene1.sceneHeight = 4096;
_scn_newScene1.tileWidth  = 32;
_scn_newScene1.tileHeight = 32;
_scn_newScene1.tileAmountX = 128;
_scn_newScene1.tileAmountY = 128;
_scn_newScene1.bgColor = '#000000';
_scn_newScene1.sceneIsContinuous = 0;
_scn_temp_layer = _scn_newScene1.add_layer( 2,8,0);
_scn_temp_layer.scnStartObjects.push( {objId:0,xPos: 64,yPos: 64,customComponents: {}}); 
//Start of Project Macros 
//End of Project Macros 
