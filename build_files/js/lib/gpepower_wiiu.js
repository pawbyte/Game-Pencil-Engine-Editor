var game = game || {};
var nwft = nwft || {};
var GPE = GPE || {};

(function ()
{
	"use strict";
	
	game.WiiU_System_Controller = (function () 
	{
		var buttonList;
		game.pluginRuns = 60;
		// Instantiate an instance of the enemy (constructor).
		function WiiU_System_Controller()
		{
			WiiU_System_Controller.isEnabled = false;
		}
		WiiU_System_Controller.pluginName = "GPE WiiU Power";
		WiiU_System_Controller.prototype.start_wiiu_mode =  function()
		{
			buttonList = nwf.input.ControllerButton;
			// Get the coordinates when the mouse moves on the GamePad canvas if running on a PC.
			// Initialize the touch coordinates and start the main loop.
			// Initialize the mouse cursor position if running on a PC.
			var touchX = nwft.GP_WIDTH / 2;
			var touchY = nwft.GP_HEIGHT / 2;
			nwft.gpCanvas.addEventListener("mousemove", function (e)
			{
				touchX = e.clientX;
				touchY = e.clientY;
			});
			GPE.canvas = nwft.tvCanvas;
			GPE.context = nwft.tvContext;
			GPE.ngpCanvas = nwft.gpCanvas;
			GPE.ngpContext = nwft.gpContext;
			GPE.context.rect(0,0,nwft.GP_WIDTH, nwft.GP_HEIGHT);
			GPE.ngpContext.rect(0,0,nwft.GP_WIDTH, nwft.GP_HEIGHT);
			GPE.context.save();
			GPE.ngpContext.save();
			GPE.SCREEN_WIDTH = nwft.TV_WIDTH;
			GPE.SCREEN_HEIGHT = nwft.TV_HEIGHT;
			
			GPE.SCREEN2_WIDTH = nwft.GP_WIDTH;
			GPE.SCREEN2_HEIGHT = nwft.GP_HEIGHT
			window.requestAnimationFrame( GPE.gameloop );
			WiiU_System_Controller.isEnabled = true;
			
			GPE.load_music = function(soundFile, startLoop)
			{
				// Initialize the background music, and wait for loading and decoding of the audio.
				var newLoopingSound = new Audio();
				newLoopingSound.tvVolume = 1;
				newLoopingSound.loop = startLoop;
				newLoopingSound.addEventListener("canplaythrough", function () 
				{
					newLoopingSound.play();
					newLoopingSound.playbackRate = 1;
				});
				newLoopingSound.src = soundFile;
				return newLoopingSound;
			}
		}
		//used at the opening of the app
		WiiU_System_Controller.prototype.init_self =  function()
		{
			nwft.initialize(GPE_SETTINGS_SCREEN_WIDTH, GPE_SETTINGS_SCREEN_HEIGHT,this.start_wiiu_mode );
		}

		WiiU_System_Controller.prototype.process_plugin=  function()
		{
			this.find_controllers_and_recheck();
		}
		
		WiiU_System_Controller.prototype.find_wiiu_inputs =function()
		{
			buttonList = nwf.input.ControllerButton;
			var cGameController = IS_NULL;
			var max_controllers = 4;
			var tempController = IS_NULL;	
			var gp;			
			for( var ii=0; ii< 5; ii++)
			{
				cGameController = GPE.inputController.game_controllers[ii];
				if(cGameController.isConnected)
				{
					if( cGameController.controllerType="WiiUGamePad" )
					{
						gp = cGameController.systemController;
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_LEFT) 
						{
							cGameController.butIsDown[GPE.cbi.LEFT_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_RIGHT)
						{
							cGameController.butIsDown[GPE.cbi.RIGHT_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_UP) 
						{
							cGameController.butIsDown[GPE.cbi.UP_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_DOWN)
						{
							cGameController.butIsDown[GPE.cbi.DOWN_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_L)
						{
							cGameController.butIsDown[GPE.cbi.L_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_R)
						{
							cGameController.butIsDown[GPE.cbi.R_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_ZL)
						{
							cGameController.butIsDown[GPE.cbi.ZL_BUT] = true;
							cGameController.butIsDown[GPE.cbi.L2_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_ZR)
						{
							cGameController.butIsDown[GPE.cbi.ZR_BUT] = true;
							cGameController.butIsDown[GPE.cbi.R2_BUT] = true;
						}
						 
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_L_STICK)
						{
							cGameController.butIsDown[GPE.cbi.L3_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_R_STICK)
						{
							cGameController.butIsDown[GPE.cbi.R3_BUT] = true;
						}
						if(gp.leftStick.movementX>0.05)
						{
						
						}
						
						if( gp.rightStick.movementX>0.05)
						{
							//Right direction was pressed
						}
					
						if( gp.leftStick.movementX<-0.05 )
						{
						
						}
						
						if( gp.rightStick.movementX<-0.05)
						{ 
						 
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_A)
						{
							cGameController.butIsDown[GPE.cbi.A_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_B)
						{
							cGameController.butIsDown[GPE.cbi.B_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_X)
						{
							cGameController.butIsDown[GPE.cbi.X_BUT] = true;
						}
						
						if(gp.buttons.buttonValue & buttonList.GAMEPAD_Y)
						{
							cGameController.butIsDown[GPE.cbi.Y_BUT] = true;
						}
					}
					else if (cGameController.controllerType=="WiiUProController")
					{
						tempController = cGameController;
						if(tempController.buttons.buttonValue & buttonList.PRO_LEFT) 
						{
							cGameController.butIsDown[GPE.cbi.LEFT_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.PRO_RIGHT)
						{
							cGameController.butIsDown[GPE.cbi.RIGHT_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.PRO_UP) 
						{
							cGameController.butIsDown[GPE.cbi.UP_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.PRO_DOWN)
						{
							cGameController.butIsDown[GPE.cbi.DOWN_BUT] = true;
						}
						if(tempController.buttons.buttonValue & buttonList.PRO_A)
						{
							cGameController.butIsDown[GPE.cbi.A_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.PRO_B)
						{
							cGameController.butIsDown[GPE.cbi.B_BUT] = true;
						}
						if(tempController.buttons.buttonValue & buttonList.PRO_X)
						{
							cGameController.butIsDown[GPE.cbi.X_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.PRO_Y)
						{
							cGameController.butIsDown[GPE.cbi.Y_BUT] = true;
						}
						if(tempController.buttons.buttonValue & buttonList.PRO_L)
						{
							cGameController.butIsDown[GPE.cbi.L_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.PRO_R)
						{
							cGameController.butIsDown[GPE.cbi.R_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.PRO_PLUS)
						{
							cGameController.butIsDown[GPE.cbi.START_BUT] = true;
						}
						
						if(tempController.leftStick.movementX>0.05)
						{
						
						}
						
						if( tempController.rightStick.movementX>0.05)
						{
							//Right direction was pressed
						}
					
						if( tempController.leftStick.movementX<-0.05 )
						{
						
						}
						
						if( tempController.rightStick.movementX<-0.05)
						{ 
						 
						}
					}
					else if (cGameController.controllerType=="WiiRemote")
					{
						tempController = cGameController;
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_LEFT) 
						{
							cGameController.butIsDown[GPE.cbi.LEFT_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_RIGHT)
						{
							cGameController.butIsDown[GPE.cbi.RIGHT_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_UP) 
						{
							cGameController.butIsDown[GPE.cbi.UP_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_DOWN)
						{
							cGameController.butIsDown[GPE.cbi.DOWN_BUT] = true;
						}
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_A)
						{
							cGameController.butIsDown[GPE.cbi.A_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_B)
						{
							cGameController.butIsDown[GPE.cbi.B_BUT] = true;
						}
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_1)
						{
							cGameController.butIsDown[GPE.cbi.X_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_2)
						{
							cGameController.butIsDown[GPE.cbi.Y_BUT] = true;
						}
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_MINUS)
						{
							cGameController.butIsDown[GPE.cbi.SELECT_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_PLUS)
						{
							cGameController.butIsDown[GPE.cbi.START_BUT] = true;
						}
						var tempExtensionController = tempController.extensionController;
						if( tempExtensionController!=null)
						{
							cGameController.controllerExtension=tempExtensionController.name;
							//checks if we are using the nunchuk extension or classic controller
							if( tempExtensionController.name.indexOf("NUNCHUK") >=0)
							{
								cGameController.controllerExtension="Wii_Nunchuk";
								if(tempExtensionController.buttons.buttonValue & buttonList.NUNCHUK_C )
								{
									cGameController.butIsDown[GPE.cbi.R_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.NUNCHUK_Z )
								{
									cGameController.butIsDown[GPE.cbi.R2_BUT] = true;
								}
							}
							else if( tempExtensionController.name.indexOf("CLASSIC") >=0)
							{
								cGameController.controllerExtension="Wii_ClassicController";
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_A )
								{
									cGameController.butIsDown[GPE.cbi.A_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_B )
								{
									cGameController.butIsDown[GPE.cbi.B_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_X )
								{
									cGameController.butIsDown[GPE.cbi.X_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_Y )
								{
									cGameController.butIsDown[GPE.cbi.Y_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_UP )
								{
									cGameController.butIsDown[GPE.cbi.UP_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_DOWN)
								{
									cGameController.butIsDown[GPE.cbi.DOWN_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_LEFT )
								{
									cGameController.butIsDown[GPE.cbi.LEFT_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_RIGHT )
								{
									cGameController.butIsDown[GPE.cbi.RIGHT_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_L )
								{
									cGameController.butIsDown[GPE.cbi.L_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_R )
								{
									cGameController.butIsDown[GPE.cbi.R_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_ZL )
								{
									cGameController.butIsDown[GPE.cbi.ZL_BUT] = true;
									cGameController.butIsDown[GPE.cbi.L2_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_ZR )
								{
									cGameController.butIsDown[GPE.cbi.ZR_BUT] = true;
									cGameController.butIsDown[GPE.cbi.R2_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_PLUS )
								{
									cGameController.butIsDown[GPE.cbi.START_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_MINUS )
								{
									cGameController.butIsDown[GPE.cbi.SELECT_BUT] = true;
								}
							}
						}
					}
				}
			}
		}
	
		WiiU_System_Controller.prototype.find_controllers_and_recheck=  function()
		{
			buttonList = nwf.input.ControllerButton;
			GPE.canvas = nwft.tvCanvas;
			GPE.context = nwft.tvContext;
			GPE.ngpCanvas = nwft.tvCanvas;
			GPE.ngpContext = nwft.tvContext;
			var gp = nwf.input.WiiUGamePad.getController();
			var cGameController = IS_NULL;
			var max_controllers = 4;
			var tempController = IS_NULL;
			
			if(gp.connected)
			{
				cGameController = GPE.inputController.game_controllers[0];
				cGameController.controllerType="WiiUGamePad";
				cGameController.deviceType = "WiiU";
				cGameController.systemController = gp;
				cGameController.isConnected = true;
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_LEFT) 
				{
					cGameController.butIsDown[GPE.cbi.LEFT_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_RIGHT)
				{
					cGameController.butIsDown[GPE.cbi.RIGHT_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_UP) 
				{
					cGameController.butIsDown[GPE.cbi.UP_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_DOWN)
				{
					cGameController.butIsDown[GPE.cbi.DOWN_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_L)
				{
					cGameController.butIsDown[GPE.cbi.L_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_R)
				{
					cGameController.butIsDown[GPE.cbi.R_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_ZL)
				{
					cGameController.butIsDown[GPE.cbi.ZL_BUT] = true;
					cGameController.butIsDown[GPE.cbi.L2_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_ZR)
				{
					cGameController.butIsDown[GPE.cbi.ZR_BUT] = true;
					cGameController.butIsDown[GPE.cbi.R2_BUT] = true;
				}
				 
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_L_STICK)
				{
					cGameController.butIsDown[GPE.cbi.L3_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_R_STICK)
				{
					cGameController.butIsDown[GPE.cbi.R3_BUT] = true;
				}
				if(gp.leftStick.movementX>0.05)
				{
				
				}
				
				if( gp.rightStick.movementX>0.05)
				{
					//Right direction was pressed
				}
			
				if( gp.leftStick.movementX<-0.05 )
				{
				
				}
				
				if( gp.rightStick.movementX<-0.05)
				{ 
				 
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_A)
				{
					cGameController.butIsDown[GPE.cbi.A_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_B)
				{
					cGameController.butIsDown[GPE.cbi.B_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_X)
				{
					cGameController.butIsDown[GPE.cbi.X_BUT] = true;
				}
				
				if(gp.buttons.buttonValue & buttonList.GAMEPAD_Y)
				{
					cGameController.butIsDown[GPE.cbi.Y_BUT] = true;
				}
			}
			else
			{
				cGameController = GPE.inputController.game_controllers[0];
				cGameController.isConnected = false;
				cGameController.controllerType="N/A";
				cGameController.deviceType = "N/A";
				cGameController.systemController = IS_NULL;
				
			}
			
			for( var ii=0; ii< 4; ii++)
			{
				tempController = nwf.input.WiiUProController.getController(ii); // Defaults to channel 0, aka WII_U_PRO_CONTROLLER_1
				cGameController = GPE.inputController.game_controllers[ii+1];
				cGameController.deviceType = "WiiU";
				cGameController.isConnected = false;
				// Keep in mind that any given controller may or may not be connected, so always check
				if (tempController.connected)
				{
					cGameController.controllerType="WiiUProController";
					cGameController.systemController = tempController;
					cGameController.isConnected = true;
					if(tempController.buttons.buttonValue & buttonList.PRO_LEFT) 
					{
						cGameController.butIsDown[GPE.cbi.LEFT_BUT] = true;
					}
					
					if(tempController.buttons.buttonValue & buttonList.PRO_RIGHT)
					{
						cGameController.butIsDown[GPE.cbi.RIGHT_BUT] = true;
					}
					
					if(tempController.buttons.buttonValue & buttonList.PRO_UP) 
					{
						cGameController.butIsDown[GPE.cbi.UP_BUT] = true;
					}
					
					if(tempController.buttons.buttonValue & buttonList.PRO_DOWN)
					{
						cGameController.butIsDown[GPE.cbi.DOWN_BUT] = true;
					}
					if(tempController.buttons.buttonValue & buttonList.PRO_A)
					{
						cGameController.butIsDown[GPE.cbi.A_BUT] = true;
					}
					
					if(tempController.buttons.buttonValue & buttonList.PRO_B)
					{
						cGameController.butIsDown[GPE.cbi.B_BUT] = true;
					}
					if(tempController.buttons.buttonValue & buttonList.PRO_X)
					{
						cGameController.butIsDown[GPE.cbi.X_BUT] = true;
					}
					
					if(tempController.buttons.buttonValue & buttonList.PRO_Y)
					{
						cGameController.butIsDown[GPE.cbi.Y_BUT] = true;
					}
					if(tempController.buttons.buttonValue & buttonList.PRO_L)
					{
						cGameController.butIsDown[GPE.cbi.L_BUT] = true;
					}
					
					if(tempController.buttons.buttonValue & buttonList.PRO_R)
					{
						cGameController.butIsDown[GPE.cbi.R_BUT] = true;
					}
					
					if(tempController.buttons.buttonValue & buttonList.PRO_PLUS)
					{
						cGameController.butIsDown[GPE.cbi.START_BUT] = true;
					}
					
					if(tempController.leftStick.movementX>0.05)
					{
					
					}
					
					if( tempController.rightStick.movementX>0.05)
					{
						//Right direction was pressed
					}
				
					if( tempController.leftStick.movementX<-0.05 )
					{
					
					}
					
					if( tempController.rightStick.movementX<-0.05)
					{ 
					 
					}
				}
				else
				{
					tempController = nwf.input.WiiRemote.getController(ii); // Defaults to channel 0, aka WII_U_PRO_CONTROLLER_1
					cGameController = GPE.inputController.game_controllers[ii+1];
					// Keep in mind that any given controller may or may not be connected, so always check
					if (tempController.connected)
					{
						cGameController.controllerType="WiiRemote";
						cGameController.systemController = tempController;
						cGameController.isConnected = true;
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_LEFT) 
						{
							cGameController.butIsDown[GPE.cbi.LEFT_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_RIGHT)
						{
							cGameController.butIsDown[GPE.cbi.RIGHT_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_UP) 
						{
							cGameController.butIsDown[GPE.cbi.UP_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_DOWN)
						{
							cGameController.butIsDown[GPE.cbi.DOWN_BUT] = true;
						}
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_A)
						{
							cGameController.butIsDown[GPE.cbi.A_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_B)
						{
							cGameController.butIsDown[GPE.cbi.B_BUT] = true;
						}
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_1)
						{
							cGameController.butIsDown[GPE.cbi.X_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_2)
						{
							cGameController.butIsDown[GPE.cbi.Y_BUT] = true;
						}
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_MINUS)
						{
							cGameController.butIsDown[GPE.cbi.SELECT_BUT] = true;
						}
						
						if(tempController.buttons.buttonValue & buttonList.WII_REMOTE_PLUS)
						{
							cGameController.butIsDown[GPE.cbi.START_BUT] = true;
						}
						var tempExtensionController = tempController.extensionController;
						if( tempExtensionController!=null)
						{
							cGameController.controllerExtension=tempExtensionController.name;
							//checks if we are using the nunchuk extension or classic controller
							if( tempExtensionController.name.indexOf("NUNCHUK") >=0)
							{
								cGameController.controllerExtension="Wii_Nunchuk";
								if(tempExtensionController.buttons.buttonValue & buttonList.NUNCHUK_C )
								{
									cGameController.butIsDown[GPE.cbi.R_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.NUNCHUK_Z )
								{
									cGameController.butIsDown[GPE.cbi.R2_BUT] = true;
								}
							}
							else if( tempExtensionController.name.indexOf("CLASSIC") >=0)
							{
								cGameController.controllerExtension="Wii_ClassicController";
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_A )
								{
									cGameController.butIsDown[GPE.cbi.A_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_B )
								{
									cGameController.butIsDown[GPE.cbi.B_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_X )
								{
									cGameController.butIsDown[GPE.cbi.X_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_Y )
								{
									cGameController.butIsDown[GPE.cbi.Y_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_UP )
								{
									cGameController.butIsDown[GPE.cbi.UP_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_DOWN)
								{
									cGameController.butIsDown[GPE.cbi.DOWN_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_LEFT )
								{
									cGameController.butIsDown[GPE.cbi.LEFT_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_RIGHT )
								{
									cGameController.butIsDown[GPE.cbi.RIGHT_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_L )
								{
									cGameController.butIsDown[GPE.cbi.L_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_R )
								{
									cGameController.butIsDown[GPE.cbi.R_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_ZL )
								{
									cGameController.butIsDown[GPE.cbi.ZL_BUT] = true;
									cGameController.butIsDown[GPE.cbi.L2_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_ZR )
								{
									cGameController.butIsDown[GPE.cbi.ZR_BUT] = true;
									cGameController.butIsDown[GPE.cbi.R2_BUT] = true;
								}
								
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_PLUS )
								{
									cGameController.butIsDown[GPE.cbi.START_BUT] = true;
								}
								if(tempExtensionController.buttons.buttonValue & buttonList.CLASSIC_MINUS )
								{
									cGameController.butIsDown[GPE.cbi.SELECT_BUT] = true;
								}
							}
						}
					}
					else
					{
						cGameController.isConnected = false;
					}
				}
			}
		};

		function vibrate_all_controllers(vibOn, vibOff, vibOveride)
		{
			for (var cController=0; cController<=4; cController++)
			{
				if( GPE.inputController.game_controllers[cController].isConnected)
				{
					GPE.inputController.game_controllers[cController].systemController.startVibrate(vibOn, vibOff, vibOveride);
				}
			}
		}

		function vibrate_controller(cId, vibOn, vibOff, vibOveride)
		{
			if(cId>=0 && cId<=4)
			{
				if( GPE.inputController.game_controllers[cId].isConnected)
				{
					GPE.inputController.game_controllers[cId].systemController.startVibrate(vibOn, vibOff, vibOveride);
				}
			}		
		}


		function end_controller_vibration_all()
		{
			for (var cController=0; cController<=4; cController++)
			{
				if( GPE.inputController.game_controllers[cController].isConnected)
				{
					GPE.inputController.game_controllers[cController].systemController.stopVibrate();
				}
			}
		}

		function end_controller_vibration_all()
		{
			if(cId>=0 && cId<=4)
			{
				if( GPE.inputController.game_controllers[cId].isConnected)
				{
					GPE.inputController.game_controllers[cId].systemController.stopVibrate();
				}
			}		
		}

		function random_input_function()
		{
			for (var cController=0; cController<this.max_controllers; cController++)
			{
				if( GPE.inputController.game_controllers[cController].isConnected)
				{

					if(GPE.inputController.game_controllers[cController].controllerType=="WiiUGamePad")
					{
					
					}
					
					if(GPE.inputController.game_controllers[cController].controllerType=="WiiUProController")
					{
					
					}
					
					if(GPE.inputController.game_controllers[cController].controllerType=="WiiRemote")
					{
					
					}
					
					if(GPE.inputController.game_controllers[cController].controllerType=="WiiClassicController")
					{
					
					}

				}
			};
		}
		return WiiU_System_Controller;
	}());
})();
