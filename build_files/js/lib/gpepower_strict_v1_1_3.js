/**
The MIT License (MIT)
Copyright (c) 2016-2017 PawByte [ PawByte.com ]

VERSION 1.13 [ October 2017 Paths Update]

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the "Software"), to deal in the 
Software without restriction, including without limitation the rights to use, copy, 
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the 
following conditions:

The above copyright notice and this permission notice shall be included in all copies 
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/	
"use strict"; 

//var eval = function(){};

var GPE_LAYER_TYPE_TILES = 0;
var GPE_LAYER_TYPE_BACKGROUND = 1;
var GPE_LAYER_TYPE_OBJECTS = 2;
var GPE_LAYER_TYPE_MAX = 2;

var GPE_DRAW_OBJECT_BOXES = false;
var PI = Math.PI;
var pi = PI;
var pie = PI;
var degreesMultiple = -180/PI;
var radiansMultiple = -PI/180;

//engine null variable

var fa_left = 0;
var fa_center = 1;
var fa_right = 2;

var fa_top = 0;
var fa_middle = 1;
var fa_bottom = 2;

var GPE_TO_RADIANS = Math.PI/180; 
var IS_NULL = -1; 
var isnull = -1;
var gpe = IS_NULL;
var GPE = IS_NULL;
var GPR = IS_NULL;
var newestFileData = IS_NULL;

var game = game || {};
var nwft = nwft || {};
var preloadedImageSRC = [];
var preloadedImages = [];

var GPE_AAC = 0;
var GPE_MP3 = 1;
var GPE_OGG = 2;
var GPE_WAV = 3;

var GPE_SUPPORTED_AUDIO_FORMAT_COUNT = 4;

var GPE_MP4 = 0;
var GPE_WEBM = 1;
var GPE_VIDEO_OGG = 2;
var GPE_SUPPORTED_VIDEO_FORMAT_COUNT = 3;

var GPE_EOT = 0;
var GPE_TTF = 1;
var GPE_SVG= 2;
var GPE_WOFF = 3;
var GPE_WOFF2 = 4;

var GPE_SUPPORTED_FONT_FORMAT_COUNT = 5;

var GPE_AUDIO_PLAYABLE = [];
GPE_AUDIO_PLAYABLE[ 0 ] = -1;
GPE_AUDIO_PLAYABLE[ 1 ] = -1;
GPE_AUDIO_PLAYABLE[ 2 ] = -1;
GPE_AUDIO_PLAYABLE[ 3 ] = -1;

var GPE_TempVariable = document.createElement('audio');
var GPE_OtherTempVariable = GPE_TempVariable.canPlayType('audio/aac');

if (GPE_OtherTempVariable=="probably")
{
	GPE_AUDIO_PLAYABLE[ GPE_AAC ] = 1;
}
else if( GPE_OtherTempVariable=="maybe")
{
	GPE_AUDIO_PLAYABLE[ GPE_AAC ] = 0;
}

GPE_OtherTempVariable = GPE_TempVariable.canPlayType('audio/mpeg');
if (GPE_OtherTempVariable=="probably")
{
	GPE_AUDIO_PLAYABLE[ GPE_MP3 ] = 1;
}
else if( GPE_OtherTempVariable=="maybe")
{
	GPE_AUDIO_PLAYABLE[ GPE_MP3 ] = 0;
}

GPE_OtherTempVariable = GPE_TempVariable.canPlayType('audio/ogg');
if (GPE_OtherTempVariable=="probably")
{
	GPE_AUDIO_PLAYABLE[ GPE_OGG ] = 1;
}
else if( GPE_OtherTempVariable=="maybe")
{
	GPE_AUDIO_PLAYABLE[ GPE_OGG ] = 0;
}

//
GPE_OtherTempVariable = GPE_TempVariable.canPlayType('audio/wav');
if (GPE_OtherTempVariable=="probably")
{
	GPE_AUDIO_PLAYABLE[ GPE_WAV ] = 1;
}
else if( GPE_OtherTempVariable=="maybe")
{
	GPE_AUDIO_PLAYABLE[ GPE_WAV ] = 0;
}


var GPE_VIDEO_PLAYABLE = [];
GPE_VIDEO_PLAYABLE[ 0 ] = -1;
GPE_VIDEO_PLAYABLE[ 1 ] = -1;
GPE_VIDEO_PLAYABLE[ 2 ] = -1;
GPE_VIDEO_PLAYABLE[ 3 ] = -1;

GPE_TempVariable = document.createElement('video');
GPE_OtherTempVariable = GPE_TempVariable.canPlayType('video/mp4');

if (GPE_OtherTempVariable=="probably")
{
	GPE_VIDEO_PLAYABLE[ GPE_MP4 ] = 1;
}
else if( GPE_OtherTempVariable=="maybe")
{
	GPE_VIDEO_PLAYABLE[ GPE_MP4 ] = 0;
}

GPE_OtherTempVariable = GPE_TempVariable.canPlayType('video/webm');
if (GPE_OtherTempVariable=="probably")
{
	GPE_VIDEO_PLAYABLE[ GPE_WEBM ] = 1;
}
else if( GPE_OtherTempVariable=="maybe")
{
	GPE_VIDEO_PLAYABLE[ GPE_WEBM ] = 0;
}

GPE_OtherTempVariable = GPE_TempVariable.canPlayType('video/ogg');
if (GPE_OtherTempVariable=="probably")
{
	GPE_VIDEO_PLAYABLE[ GPE_VIDEO_OGG ] = 1;
}
else if( GPE_OtherTempVariable=="maybe")
{
	GPE_VIDEO_PLAYABLE[ GPE_VIDEO_OGG ] = 0;
}

var GPE_AUD_TYPE_NAME = [];
GPE_AUD_TYPE_NAME[ 0 ] = 'video/mp4';
GPE_AUD_TYPE_NAME[ 1 ] = 'video/webm'
GPE_AUD_TYPE_NAME[ 2 ] = 'video/ogg';

GPE_TempVariable = IS_NULL;

var GPE_MASTER_COLLISION_LIST = [];
var GAMEPAD_SUPPORTED = false;

// Checks if the web browser supports the Gamepad API
if(!!navigator.getGamepads)
{
	GAMEPAD_SUPPORTED = true;
}

var inheritsFrom = function (child, parent) 
{
    child.prototype = Object.create(parent.prototype);
};


//Credits: http://stackoverflow.com/users/821334/matthew
function addSourceToVideo(element, src, type)
{
    var source = document.createElement('source');
    source.src = src;
    source.type = type;
    element.appendChild(source);
}

	//GameController constants [ BEGIN ]
	var gc_anycontroller = -1;
	var gc_nocontroller = -1;
	var gc_nobutton = -1;
	var gc_anybutton = -2;
	
	var gc_joystick_left_x = 0;
	var gc_joystick_left_y = 1;
	var gc_joystick_right_x = 2;
	var gc_joystick_right_y = 3;
	var gc_joystick_dpad_x = 4;
	var gc_joystick_dpad_y = 5;
	
	var gc_left = 0;
	var gc_right = 1;
	var gc_up = 2;
	var gc_down= 3;
	
	var gc_but0 = 4;
	var gc_but1 = 5;
	var gc_but2 = 6;
	var gc_but3 = 7;
	
	var gc_l1= 8;
	var gc_l2= 9;
	var gc_l3 = 10;
	
	var gc_r1= 11;
	var gc_r2= 12
	var gc_r3= 13;
	
	var gc_start = 14;
	var gc_select = 15;
	var gc_home = 16;
	var gc_share = 17;
	//GameController constants [ END ]
	
	//Mouse constants [ BEGIN ]

	var mb_left = 0;
	var mb_right = 1;
	var mb_middle = 2;
	var mb_center = 2;
	
	//Mouse constants [ END ]
	
	//Keyboard constants [ BEGIN ]
	var kb_nokey = -1;
	var kb_anykey = -2;
	
	var kb_backspace = 8;
	var kb_tab = 9;
	var kb_enter = 13;
	var kb_pause = 19;
	var kb_caps = 20;
	var kb_esc = 27;
	var kb_space = 32;
	var kb_page_up = 33;
	var kb_page_down = 34;
	var kb_end = 35;
	var kb_home = 36;
	var kb_left = 37;
	var kb_up = 38;
	var kb_right = 39;
	var kb_down = 40;
	var kb_insert = 45;
	var kb_delete = 46;
	//
	var kb_0 = 48;
	var kb_1 = 49;
	var kb_2 = 50;
	var kb_3 = 51;
	var kb_4 = 52;
	var kb_5 = 53;
	var kb_6 = 54;
	var kb_7 = 55;
	var kb_8 = 56;
	var kb_9 = 57;
	//
	var kb_a = 65;
	var kb_b = 66;
	var kb_c = 67;
	var kb_d = 68;
	var kb_e = 69;
	var kb_f = 70;
	var kb_g = 71;
	var kb_h = 72;
	var kb_i = 73;
	var kb_j = 74;
	var kb_k = 75;
	var kb_l = 76;
	var kb_m = 77;
	var kb_n = 78;
	var kb_o = 79;
	var kb_p = 80;
	var kb_q = 81;
	var kb_r = 82;
	var kb_s = 83;
	var kb_t = 84;
	var kb_u = 85;
	var kb_v = 86;
	var kb_w = 87;
	var kb_x = 88;
	var kb_y = 89;
	var kb_z = 90;
	
	var kb_np0 = 96;
	var kb_np1 = 97;
	var kb_np2 = 98;
	var kb_np3 = 99;
	var kb_np4 = 100;
	var kb_np5 = 101;
	var kb_np6 = 102;
	var kb_np7 = 103;
	var kb_np8 = 104;
	var kb_np9 = 105;
	
	var kb_multiply = 106;
	var kb_add = 107;
	var kb_substract = 109;
	var kb_decimal = 110;
	var kb_divide = 111;
	
	var kb_f1 = 112;
	var kb_f2 = 113;
	var kb_f3 = 114;
	var kb_f4 = 115;
	var kb_f5 = 116;
	var kb_f6 = 117;
	var kb_f7 = 118;
	var kb_f8 = 119;
	var kb_f9 = 120;
	var kb_f10 = 121;
	var kb_f11 = 122;
	var kb_f12 = 123;
	
	var kb_shift = 16;
	var kb_ctrl = 17;
	var kb_alt = 18;
	var kb_plus = 187;
	var kb_comma = 188;
	var kb_minus = 189;
	var kb_period = 190;
	var kb_fslash = 191;
	var kb_gaccent = 192;
	var kb_obracket = 220;
	var kb_bslash = 220;
	var kb_cbracket = 221;
	var kb_squote = 222;
	//Keyboard constants [ END ]

	//Keyboard (legacy 1.1.1 and earlier ) constants [ BEGIN ]
	var gp_nokey = -1;
	var gp_anykey = -2;
	
	var gp_backspace = 8;
	var gp_tab = 9;
	var gp_enter = 13;
	var gp_pause = 19;
	var gp_caps = 20;
	var gp_esc = 27;
	var gp_space = 32;
	var gp_page_up = 33;
	var gp_page_down = 34;
	var gp_end = 35;
	var gp_home = 36;
	var gp_left = 37;
	var gp_up = 38;
	var gp_right = 39;
	var gp_down = 40;
	var gp_insert = 45;
	var gp_delete = 46;
	//
	var gp_0 = 48;
	var gp_1 = 49;
	var gp_2 = 50;
	var gp_3 = 51;
	var gp_4 = 52;
	var gp_5 = 53;
	var gp_6 = 54;
	var gp_7 = 55;
	var gp_8 = 56;
	var gp_9 = 57;
	//
	var gp_a = 65;
	var gp_b = 66;
	var gp_c = 67;
	var gp_d = 68;
	var gp_e = 69;
	var gp_f = 70;
	var gp_g = 71;
	var gp_h = 72;
	var gp_i = 73;
	var gp_j = 74;
	var gp_k = 75;
	var gp_l = 76;
	var gp_m = 77;
	var gp_n = 78;
	var gp_o = 79;
	var gp_p = 80;
	var gp_q = 81;
	var gp_r = 82;
	var gp_s = 83;
	var gp_t = 84;
	var gp_u = 85;
	var gp_v = 86;
	var gp_w = 87;
	var gp_x = 88;
	var gp_y = 89;
	var gp_z = 90;
	
	var gp_np0 = 96;
	var gp_np1 = 97;
	var gp_np2 = 98;
	var gp_np3 = 99;
	var gp_np4 = 100;
	var gp_np5 = 101;
	var gp_np6 = 102;
	var gp_np7 = 103;
	var gp_np8 = 104;
	var gp_np9 = 105;
	
	var gp_multiply = 106;
	var gp_add = 107;
	var gp_substract = 109;
	var gp_decimal = 110;
	var gp_divide = 111;
	
	var gp_f1 = 112;
	var gp_f2 = 113;
	var gp_f3 = 114;
	var gp_f4 = 115;
	var gp_f5 = 116;
	var gp_f6 = 117;
	var gp_f7 = 118;
	var gp_f8 = 119;
	var gp_f9 = 120;
	var gp_f10 = 121;
	var gp_f11 = 122;
	var gp_f12 = 123;
	
	var gp_shift = 16;
	var gp_ctrl = 17;
	var gp_alt = 18;
	var gp_plus = 187;
	var gp_comma = 188;
	var gp_minus = 189;
	var gp_period = 190;
	var gp_fslash = 191;
	var gp_gaccent = 192;
	var gp_obracket = 220;
	var gp_bslash = 220;
	var gp_cbracket = 221;
	var gp_squote = 222;
	//Keyboard (legacy 1.1.1 and earlier ) constants [ END ]
	
(function () 
{
	//defines the canvas and it's contexts as globals
	//Not 100% sure how this is used, but looks as if it might be better to pass the context into the function that needs it. 
	//Or perhaps have a GPE_GameController.context field.
	
	var SYS_FPS = 0;
	var iLoop = 0;

	//preloadedImageSRC[0] = 'res/game_titlescreen.jpg';
	var imageCount = GPE_PreloadImageResource.length;

	var APP_FULL_PATH = window.location;
	var tempPath = ""+APP_FULL_PATH;
	APP_FULL_PATH = tempPath.substring(0,tempPath.lastIndexOf("/") );
	
	var programStates =
	{
		STATE_NULL: 0,
		STATE_LOAD_RESOURCES: 1,
		STATE_LOGO_DISPLAY: 2,
		STATE_INTRO: 3,
		STATE_TITLE: 4,
		STATE_MAINMENU: 5,
		STATE_GAME_LOADING: 6,
		STATE_MAINGAME: 7,
		STATE_LEVEL_EDITOR: 8,
		STATE_CREDITS: 9,
		STATE_CUTSCENE: 10,
		STATE_FLASHBACK: 11,
		STATE_OPTIONS: 12,
		STATE_EXIT: 13
	};

	var nextProgramState = 0;

	function set_next_state( newState )
	{
		//If the user doesn't want to exit
		if( nextProgramState != programStates.STATE_EXIT )
		{
			//Set the next state
			nextProgramState = newState;
		}
	}

	function change_state()
	{
		//If the state needs to be changed
		if( nextProgramState != programStates.STATE_NULL )
		{
			//Delete the current state
			if( nextProgramState != programStates.STATE_EXIT )
			{
				 gpe.currentState = IS_NULL;
			}
			 //Change the state
			switch( nextProgramState )
			{
				case programStates.STATE_LOGO_DISPLAY:
					gpe.currentState = new GPE_LogoDisplay();
				break;
				case programStates.STATE_LOAD_RESOURCES:
					gpe.currentState = new GPE_ResourceLoader();
				break;
				case programStates.STATE_MAINMENU:
					gpe.currentState = new GPE_MainMenu();
				break;
				case programStates.STATE_MAINGAME:
					gpe.currentState = new GPE_MainGame();
				break;
				default:
			 //       nextProgramState=STATE_INTRO;
			 //       gpe.currentState = new IntroState();
				break;
			}
		
			GPR = gpe.currentState;
			//Change the current state ID
			programStateId = nextProgramState;

			//NULL the next state ID
			nextProgramState = programStates.STATE_NULL;
			gpe.handle_canvas_size();
		}
	}

	var programStateId;
	var nextProgramState = programStates.STATE_NULL;

	window.onload = function()
	{
		init_gpe();
	};

	//used for checking the loading state of an image
	function IsImageOk(img)
	{
		//Credit: (sajithmr) http://blog.sajithmr.me/javascript-check-an-image-is-loaded-or-not 
		// During the onload event, IE correctly identifiesany images that
		// weren’t downloaded as not complete. Others should too. Gecko-based
		// browsers act like NS4 in that they report this incorrectly.
		if (!img.complete) 
		{
			return false;
		}

		// However, they do have two very useful properties:naturalWidth and
		// naturalHeight. These give the true size of the image. If it failed
		// to load, either of these should be zero.
		if (typeof img.naturalWidth	!= "undefined" && img.naturalWidth	== 0)
		{
			return false;
		}
		return true;
	}

	function GPE_JSFile( nFileName)
	{
		this.dataSource = "";
		this.fileName = nFileName;
		this.fileLines = [];
		this.fileLines[0] = ""; //default line, will be overwritten in onload event
		this.fileType = "";
		this.fileLinePos = -1;
		this.fileIsReady = false;
		
		this.is_open = function()
		{
			return this.fileIsReady;
		}
		
		this.is_good = function()
		{
			if( this.fileIsReady)
			{
				if( this.fileLinePos < this.fileLines.length )
				{
					return true;
				}
				return false;
			}
			return IS_NULL;
		}
		
		this.get_next_line = function()
		{
			if( this.fileIsReady)
			{
				if( this.fileLinePos < this.fileLines.length -1 )
				{
					this.fileLinePos+=1;
					return this.fileLines[this.fileLinePos];
				}
				else
				{
					return this.fileLinePos+=1;
				}
			}
			return IS_NULL;
		}
		
		this.get_line_count = function()
		{
			if(this.fileIsReady)
			{
				return this.fileLines.length;
			}
			return -1;
		}
		
		this.get_line_pos = function()
		{
			if( this.fileIsReady )
			{
				return this.fileLinePos;
			}
			return -1;
		}
		
		this.get_remaining_lines = function()
		{
			if( this.fileIsReady )
			{
				return this.fileLines.length-this.fileLinePos;
			}
		}
		
		this.process_file_as_text = function()
		{
			//Credits: juanpastas - http://stackoverflow.com/users/1197775/juanpastas 
			//URL: http://stackoverflow.com/questions/23331546/how-to-use-javascript-to-read-local-text-file-and-read-line-by-line
			// Reads the entire file by lines
			var lines = this.dataSource.split("*nl*");
			for(var line = 0; line < lines.length; line++)
			{
				this.fileLines[line] = lines[line];
			}
			this.fileIsReady = true;
		}
		
		this.close_file = function()
		{
			this.fileIsReady;
		};
	}

	//The FileReaderClass
	function GPE_FileReader()
	{
		this.frLoadedFiles = [];
		this.osFileReader = -1;
		this.fileObbj;
		this.lastFileOpened = "";
		this.frFileCount = 0;
		
		this.recognize_new_file = function(newLoadedFile)
		{
			if( this.lastFileOpened!="")
			{
				this.frLoadedFiles[this.frFileCount] = newLoadedFile;
				newLoadedFile.process_file_as_text();
			}
			this.lastFileOpened = "";
			this.frFileCount+=1;
		}
		
		this.open_file = function(newFileName, fileReadType)
		{
			if( newFileName!="")
			{
				if( newFileName.length > 3)
				{
					newestFileData = new GPE_JSFile(newFileName, fileReadType);
					this.lastFileOpened = newFileName;
					var s = document.createElement("script");
					s.type = "text/javascript";
					s.src = newFileName || null;
					s.innerHTML = s.src || null;
					document.getElementsByTagName("head")[0].appendChild(s);
					return newestFileData;
				}
			}
			return IS_NULL;
		};
		
		this.open_new_file = function(newFileName, fileReadType)
		{
		
		};
		
		this.open_write_file = function(newFileName, fileWriteType)
		{
		
		};
	}

	//the basic controller input state
	function GPE_InputManager_GameController()
	{
		this.controllerType = IS_NULL;
		this.controllerExtension = "";
		this.detectedDeviceType = "";
		this.deviceType = "";
		this.systemController = IS_NULL;
		this.isConnected = false;
		this.mapping = 0;
		this.timestamp = 0;
		
		this.button_count = 25; //way more than enough for a video game controller
		this.axes_count = 16; //also way more than enough for any normal controller
		this.system_axes_count = 0;
		this.touch_mouse_x = 0; //for touch screen enabled controllers
		this.touch_mouse_y = 0; 
		
		this.prevHit = [];
		this.butIsDown = [];
		this.butIsPressed = [];
		this.butIsReleased = [];
		this.butIsLocked = [];
		
		this.axesMap = [];
		this.buttonsMap = [];
		
		for (var but=0; but <this.button_count; but++)
		{
			this.prevHit[but] = false;
			this.butIsDown[but] = false;
			this.butIsPressed[but] = false;
			this.butIsReleased[but] = false;
			this.butIsLocked[but] = false;
			this.buttonsMap[but] = but;
		}
		
		this.axesValues = [];
		for (var tempAxes=0; tempAxes <this.axes_count; tempAxes++)
		{
			this.axesMap[ tempAxes ] = tempAxes;
			this.axesValues[ tempAxes ] = 0;
		}
		
		//GameController Input related functions/objects/variables as of Version 1.12 [ BEGIN ] 

		this.map_to_ds4_chrome = function()
		{
			this.buttonsMap[gc_left] = 14;
			this.buttonsMap[gc_right] = 15;
			this.buttonsMap[gc_up] = 12;
			this.buttonsMap[gc_down] = 13;
		
			this.buttonsMap[gc_but0] = 0;
			this.buttonsMap[gc_but1] = 1;
			this.buttonsMap[gc_but2] = 2;
			this.buttonsMap[gc_but3] = 3;
		
			this.buttonsMap[gc_l1] = 4;
			this.buttonsMap[gc_l2] = 6;
			this.buttonsMap[gc_l3] = 10;
			
			this.buttonsMap[gc_r1] = 5;
			this.buttonsMap[gc_r2] = 7;
			this.buttonsMap[gc_r3] = 11;
			
			
			this.buttonsMap[gc_home] = 16;
			this.buttonsMap[gc_select] = 9;
			this.buttonsMap[gc_share] = 8;
			this.buttonsMap[gc_start] = 17;
			
			this.axesMap[ gc_joystick_left_x ] = 0;
			this.axesMap[ gc_joystick_left_y ] = 1;

			
		};
		
		this.map_to_ds4_firefox = function()
		{
			this.buttonsMap[gc_left] = 14;
			this.buttonsMap[gc_right] = 15;
			this.buttonsMap[gc_up] = 12;
			this.buttonsMap[gc_down] = 13;
		
			this.buttonsMap[gc_but0] = 1;
			this.buttonsMap[gc_but1] = 2;
			this.buttonsMap[gc_but2] = 0;
			this.buttonsMap[gc_but3] = 3;
		
			this.buttonsMap[gc_l1] = 4;
			this.buttonsMap[gc_l2] = 6;
			this.buttonsMap[gc_l3] = 10;
			
			this.buttonsMap[gc_r1] = 5;
			this.buttonsMap[gc_r2] = 7;
			this.buttonsMap[gc_r3] = 11;
			
			
			this.buttonsMap[gc_home] = 16;
			this.buttonsMap[gc_select] = 9;
			this.buttonsMap[gc_share] = 8;
			this.buttonsMap[gc_start] = 17;
			
		}
		
		this.map_to_switchpro_chrome = function()
		{
			this.buttonsMap[gc_left] = 14;
			this.buttonsMap[gc_right] = 15;
			this.buttonsMap[gc_up] = 12;
			this.buttonsMap[gc_down] = 13;
		
			this.buttonsMap[gc_but0] = 1;
			this.buttonsMap[gc_but1] = 0;
			this.buttonsMap[gc_but2] = 3;
			this.buttonsMap[gc_but3] = 2;
		
			this.buttonsMap[gc_l1] = 4;
			this.buttonsMap[gc_l2] = 6;
			this.buttonsMap[gc_l3] = 10;
			
			this.buttonsMap[gc_r1] = 5;
			this.buttonsMap[gc_r2] = 7;
			this.buttonsMap[gc_r3] = 11;
			
			
			this.buttonsMap[gc_home] = 12;
			this.buttonsMap[gc_select] = 8;
			this.buttonsMap[gc_share] = 13;
			this.buttonsMap[gc_start] = 9;
			
		};
		
		this.map_to_switchpro_firefox = function()
		{
			this.buttonsMap[gc_left] = 14;
			this.buttonsMap[gc_right] = 15;
			this.buttonsMap[gc_up] = 12;
			this.buttonsMap[gc_down] = 13;
		
			this.buttonsMap[gc_but0] = 1;
			this.buttonsMap[gc_but1] = 0;
			this.buttonsMap[gc_but2] = 3;
			this.buttonsMap[gc_but3] = 2;
		
			this.buttonsMap[gc_l1] = 4;
			this.buttonsMap[gc_l2] = 6;
			this.buttonsMap[gc_l3] = 10;
			
			this.buttonsMap[gc_r1] = 5;
			this.buttonsMap[gc_r2] = 7;
			this.buttonsMap[gc_r3] = 11;
			
			
			this.buttonsMap[gc_home] = 12;
			this.buttonsMap[gc_select] = 8;
			this.buttonsMap[gc_share] = 13;
			this.buttonsMap[gc_start] = 9;
			
		}
		
		//Default controller mapping...
		this.map_to_ds4_chrome();
		
		this.check_connection = function()
		{
			return this.isConnected;
		};
		
		this.check_button = function(buttonId)
		{
			if( this.isConnected )
			{
				if(  buttonId >=0 && buttonId < this.button_count)
				{
					return this.butIsDown[ this.buttonsMap[buttonId] ] > 0;
				}
				else if( buttonId == gc_anybutton )
				{
					for( var iAnyButton = this.button_count-1; iAnyButton >=0; iAnyButton-- )
					{
						if( this.butIsDown[ this.buttonsMap[iAnyButton] ] > 0 )
						{
							return true;
						}
					}
					return false;
				}
				else if( buttonId == gc_nobutton )
				{
					for( var iNoButton = this.button_count-1; iNoButton >=0; iNoButton-- )
					{
						if( this.butIsDown[ this.buttonsMap[iNoButton] ] > 0 )
						{
							return false;
						}
					}
					return true;
				}
			}
			return false;
		};
		
		this.check_button_pressed = function(buttonId)
		{
			if( this.isConnected )
			{
				if( buttonId >=0 && buttonId < this.button_count)
				{
					return this.butIsPressed[ this.buttonsMap[buttonId] ] > 0;
				}
				else if( buttonId == gc_anybutton )
				{
					for( var iAnyButton = this.button_count-1; iAnyButton >=0; iAnyButton-- )
					{
						if( this.butIsPressed[ this.buttonsMap[iAnyButton] ] > 0 )
						{
							return true;
						}
					}
					return false;
				}
				else if( buttonId == gc_nobutton )
				{
					for( var iNoButton = this.button_count-1; iNoButton >=0; iNoButton-- )
					{
						if( this.butIsPressed[ this.buttonsMap[iNoButton] ] > 0 )
						{
							return false;
						}
					}
					return true;
				}
			}
			return false;
		};
		
		this.check_button_released = function(buttonId)
		{
			if( this.isConnected )
			{
				if( buttonId >=0 && buttonId < this.button_count)
				{
					return this.butIsReleased[ this.buttonsMap[buttonId] ];
				}
				else if( buttonId == gc_anybutton )
				{
					for( var iAnyButton = this.button_count-1; iAnyButton >=0; iAnyButton-- )
					{
						if( this.butIsReleased[ this.buttonsMap[iAnyButton] ] > 0 )
						{
							return true;
						}
					}
					return false;
				}
				else if( buttonId == gc_nobutton )
				{
					for( var iNoButton = this.button_count-1; iNoButton >=0; iNoButton-- )
					{
						if( this.butIsReleased[ this.buttonsMap[iNoButton] ] > 0 )
						{
							return false;
						}
					}
					return true;
				}
			}
			return false;
		};
		
		this.gamepad_get_axes_count = function( )
		{
			return this.system_axes_count;
		};
		
		this.find_gamepad_axes_mapping = function( axesId )
		{
			if(  axesId >=0 && axesId < this.axes_count )
			{
				return this.axesMap[axesId];
			}
			return -1;
		};
		
		this.find_gamepad_button_mapping = function( buttonId)
		{
			if(  buttonId >=0 && buttonId < this.button_count )
			{
				return this.axesMap[buttonId];
			}
			return -1;
		};
		
		this.gamepad_map_axes = function(axesId, newAxesId )
		{
			if(  axesId >=0 && axesId < this.axes_count)
			{
				if(  newAxesId >=0 && newAxesId < this.axes_count )
				{
					this.buttonsMap[axesId] = newAxesId;
					return true;
				}
			}
			return false;
		};
		
		this.gamepad_map_button = function(buttonId, newButtonId)
		{
			if(  buttonId >=0 && buttonId < this.button_count)
			{
				if(  newButtonId >=0 && newButtonId < this.button_count)
				{
					this.buttonsMap[buttonId] = newButtonId;
					return true;
				}
			}
			return false;
		};
		
		//GameController Input related functions/objects/variables as of Version 1.12 [ END ] 

		this.reset_controller = function()
		{
			for(var but=0; but<this.button_count; but++)
			{
				this.butIsPressed[ this.buttonsMap[but] ] = this.butIsDown[ this.buttonsMap[but] ] && !this.prevHit[ this.buttonsMap[but] ];
				this.butIsReleased[ this.buttonsMap[but] ] = !this.butIsDown[ this.buttonsMap[but] ] && this.prevHit[ this.buttonsMap[but] ];
			}
		};
		
		this.pure_reset = function()
		{
			for( var but=0; but<this.button_count; but++)
			{
				this.prevHit[ this.buttonsMap[but] ] = false;
				this.butIsDown[ this.buttonsMap[but] ] = false;
				this.butIsPressed[ this.buttonsMap[but] ] = false;
				this.butIsReleased[ this.buttonsMap[but] ] = false;
				this.butIsLocked[ this.buttonsMap[but] ] = false;
			}
			for (var tempAxes=0; tempAxes <this.axes_count; tempAxes++)
			{
				this.axesValues[ tempAxes ] = 0;
			}
		};
	};

	//the Input Manager
	function GPE_InputManager()
	{
		this.mouse_button_count = 3;
		this.key_count = 225;
		this.joy_key_count = 2;
		
		this.prevHit = [];
		this.keyIsDown = [];
		this.keyIsPressed = [];
		this.keyIsReleased = [];
		this.keyIsLocked = [];
		
		this.mousePrevHit = [];
		this.mbIsDown = [];
		this.mbIsPressed = [];
		this.mbIsReleased = [];
		this.mbIsLocked = [];
		
		this.asyncKeyIsDown = [];
		this.asyncKeyIsPressed = [];
		this.asyncKeyIsReleased = [];
		this.asyncKeyIsLocked = [];
		
		this.asyncMousePrevHit = [];
		this.asyncMbIsDown = [];
		this.asyncMbIsPressed = [];
		this.asyncMbIsReleased = [];
		this.asyncMbIsLocked = [];
		
		this.game_controllers = [];
		this.max_controllers = 5; //max of 4 players. Change this if you want manually.
		
		for (var key=0; key<this.key_count; key++)
		{
			this.prevHit[key] = false;
			this.keyIsDown[key] = false;
			this.keyIsPressed[key] = false;
			this.keyIsReleased[key] = false;
			this.keyIsLocked[key] = false;
			
			this.asyncKeyIsDown[key] = false;
			this.asyncKeyIsPressed[key] = false;
			this.asyncKeyIsReleased[key] = false;
			this.asyncKeyIsLocked[key] = false;
		}
		
		for( var mbi = 0; mbi < this.mouse_button_count;mbi++)
		{
			this.mousePrevHit[mbi] = false;
			this.mbIsDown[mbi] = false;
			this.mbIsPressed[mbi] = false;
			this.mbIsReleased[mbi] = false;
			this.mbIsLocked[mbi] = false;
			
			this.asyncMousePrevHit[mbi] = false;
			this.asyncMbIsDown[mbi] = false;
			this.asyncMbIsPressed[mbi] = false;
			this.asyncMbIsReleased[mbi] = false;
			this.asyncMbIsLocked[mbi] = false;
		}

		for (var cController=0; cController<this.max_controllers; cController++)
		{
			this.game_controllers[cController] = new GPE_InputManager_GameController();
		}
		
		//gamepad functions ADDED Since Version 1.1.0
		this.gamepad_connected = function(controllerPos)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].isConnected;
			}
			return -1;
		};
		
		this.gamepad_detected_name = function(controllerPos)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].detectedDeviceType;
			}
			return "";
		};
		
		
		this.gamepad_name = function(controllerPos)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].deviceType;
			}
			return "";
		};
		
		this.gamepad_object = function(controllerPos)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos];
			}
			return IS_NULL;
		};
		
		this.gamepad_get_axes_count = function( controllerPos )
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].gamepad_get_axes_count();;
			}
			return -1;
		};
		
		this.find_gamepad_mapping = function(controllerPos, buttonId)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].find_gamepad_mapping( controllerPos, buttonId);
			}
			return -1;
		};
		
		this.check_gamepad = function(controllerPos, buttonId)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].check_button(buttonId);
			}
			else if( controllerPos == gc_anycontroller )
			{
				for( var iConPos = this.game_controllers.length -1; iConPos >=0; iConPos-- )
				{
					if( this.game_controllers[iConPos].check_button(buttonId) )
					{
						return true;
					}
				}
			}
			return false;
		}
		
		this.check_gamepad_down = function(controllerPos, buttonId)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].check_button(buttonId);
			}
			else if( controllerPos == gc_anycontroller )
			{
				for( var iConPos = this.game_controllers.length -1; iConPos >=0; iConPos-- )
				{
					if( this.game_controllers[iConPos].check_button(buttonId) )
					{
						return true;
					}
				}
			}
			return false;
		};
		
		this.check_gamepad_pressed = function(controllerPos, buttonId)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].check_button_pressed(buttonId);
			}
			else if( controllerPos == gc_anycontroller )
			{
				for( var iConPos = this.game_controllers.length -1; iConPos >=0; iConPos-- )
				{
					if( this.game_controllers[iConPos].check_button_pressed(buttonId) )
					{
						return true;
					}
				}
			}
			return false;
		};
		
		this.check_gamepad_released = function(controllerPos, buttonId)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].check_button_released(buttonId);
			}
			else if( controllerPos == gc_anycontroller )
			{
				for( var iConPos = this.game_controllers.length -1; iConPos >=0; iConPos-- )
				{
					if( this.game_controllers[iConPos].check_button_released(buttonId) )
					{
						return true;
					}
				}
			}
			return false;
		};
		
		this.find_gamepad_axes_mapping = function( controllerPos, axesId)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].find_gamepad_axes_mapping( axesId);
			}
			return IS_NULL;
		};
		
		
		this.find_gamepad_button_mapping = function( controllerPos, buttonId)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].find_gamepad_mapping( buttonId);
			}
			return IS_NULL;
		};
		
		this.gamepad_map_axes = function(controllerPos, axesId, newaxesId)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].gamepad_map_axes(  axesId, newaxesId);
			}
			else if( controllerPos == gc_anycontroller )
			{
				var successfulMapping = false;
				for( var iConPos = this.game_controllers.length -1; iConPos >=0; iConPos-- )
				{
					if( this.game_controllers[iConPos].gamepad_map_axes(  axesId, newaxesId) )
					{
						successfulMapping = true;;
					}
				}
				return successfulMapping;
			}
			return false;
		};
		
		
		this.gamepad_map_button = function(controllerPos, buttonId, newButtonId)
		{
			if( controllerPos >=0 && controllerPos < this.max_controllers )
			{
				return this.game_controllers[controllerPos].gamepad_map_button(  buttonId, newButtonId);
			}
			else if( controllerPos == gc_anycontroller )
			{
				var successfulMapping = false;
				for( var iConPos = this.game_controllers.length -1; iConPos >=0; iConPos-- )
				{
					if( this.game_controllers[iConPos].gamepad_map_button(  buttonId, newButtonId) )
					{
						successfulMapping = true;;
					}
				}
				return successfulMapping;
			}
			return false;
		};
		
		//keyboard functions
		this.check_keyboard_down = function(keyId)
		{
			if(  keyId >=0 && keyId < this.key_count)
			{
				return this.keyIsDown[keyId];
			}
			else if( keyId == gp_anykey )
			{
				for( var iAnyKey = this.key_count-1; iAnyKey >=0; iAnyKey-- )
				{
					if( this.keyIsDown[ iAnyKey ] > 0 )
					{
						return true;
					}
				}
				return false;
			}
			else if( keyId == gp_nokey )
			{
				for( var iNoKey = this.key_count-1; iNoKey >=0; iNoKey-- )
				{
					if( this.keyIsDown[ iNoKey ] > 0 )
					{
						return false;
					}
				}
				return true;
			}
			return false;
		};
		
		this.check_keyboard_pressed = function(keyId)
		{
			if(  keyId >=0 && keyId < this.key_count)
			{
				return this.keyIsPressed[keyId];
			}
			else if( keyId == gp_anykey )
			{
				for( var iAnyKey = this.key_count-1; iAnyKey >=0; iAnyKey-- )
				{
					if( this.keyIsPressed[ iAnyKey ] > 0 )
					{
						return true;
					}
				}
				return false;
			}
			else if( keyId == gp_nokey )
			{
				for( var iNoKey = this.key_count-1; iNoKey >=0; iNoKey-- )
				{
					if( this.keyIsPressed[ iNoKey ] > 0 )
					{
						return false;
					}
				}
				return true;
			}
			return false;
		};
		
		this.check_keyboard_released = function(keyId)
		{
			if(  keyId >=0 && keyId < this.key_count)
			{
				return this.keyIsReleased[keyId];
			}
			else if( keyId == gp_anykey )
			{
				for( var iAnyKey = this.key_count-1; iAnyKey >=0; iAnyKey-- )
				{
					if( this.keyIsReleased[ iAnyKey ] > 0 )
					{
						return true;
					}
				}
				return false;
			}
			else if( keyId == gp_nokey )
			{
				for( var iNoKey = this.key_count-1; iNoKey >=0; iNoKey-- )
				{
					if( this.keyIsReleased[ iNoKey ] > 0 )
					{
						return false;
					}
				}
				return true;
			}
			return false;
		};
		
		//mouse functions
		this.check_mouse_down = function(mbId)
		{
			if(  mbId >=0 && mbId < this.mouse_button_count)
			{
				return this.mbIsDown[mbId];
			}
			return false;
		};
		
		this.check_mouse_pressed = function(mbId)
		{
			if(  mbId >=0 && mbId < this.mouse_button_count)
			{
				return this.mbIsPressed[mbId];
			}
			return false;
		};
		
		this.check_mouse_released = function(mbId)
		{
			if(  mbId >=0 && mbId < this.mouse_button_count)
			{
				return this.mbIsReleased[mbId];
			}
			return false;
		};
		
		this.get_controller = function(playerNumber)
		{
			var foundOnControllerCount = 0;
			var foundController = IS_NULL;
			for (var cController=0; cController<this.max_controllers; cController++)
			{
				foundController = this.game_controllers[cController];
				if( foundController.isConnected)
				{
					foundOnControllerCount++;
				}
				if( foundOnControllerCount>=playerNumber)
				{
					return foundController;
				}
			}
			return foundController;
		}
		
		this.reset_input = function()
		{
			var key = 0;
			
			for ( key=0; key<this.key_count; key++)
			{
				this.keyIsPressed[key] = this.keyIsDown[key] && !this.prevHit[key];
				this.keyIsReleased[key] = !this.keyIsDown[key] && this.prevHit[key];
			}
			
			var mb = 0;
			
			for ( mb=0; mb<this.mouse_button_count; mb++)
			{
				this.mbIsPressed[mb] = this.mbIsDown[mb] && !this.mousePrevHit[mb];
				this.mbIsReleased[mb] = !this.mbIsDown[mb] && this.mousePrevHit[mb];
			}
			
			var foundController = IS_NULL;
			for (var cController=0; cController<this.max_controllers; cController++)
			{
				foundController = this.game_controllers[cController];
				foundController.reset_controller();
			}
			
			for( key=0; key<this.key_count; key++)
			{
				this.prevHit[key] = this.keyIsDown[key];
			}
			
			for ( mb=0; mb<this.mouse_button_count; mb++)
			{
				this.mousePrevHit[mb] = this.mbIsDown[mb];
			}
		};
		
		this.handle_key_down = function( evt )
		{
			if (typeof evt=="undefined")
			{
				var evt = window.event;
			}
			var foundKey = parseInt( evt.keyCode ? evt.keyCode : evt.which)  ;
			if( foundKey < this.key_count )
			{
				if( foundKey < gp_np0)
				{
					evt.preventDefault();
				}
				this.asyncKeyIsPressed[foundKey] = true;
				this.asyncKeyIsDown[foundKey] = true;
				this.asyncKeyIsReleased[foundKey] = false;
				//this.prevHit[foundKey] = true;
			}
		};
		
		this.handle_key_press = function( evt )
		{
			if (typeof evt=="undefined")
			{
				var evt = window.event;
			}
			var foundKey = parseInt( evt.keyCode ? evt.keyCode : evt.which)  ;
			if( foundKey > 33)
			{
				foundKey -= 32;
			}
			if( foundKey < this.key_count )
			{
				if( foundKey < gp_np0)
				{
					evt.preventDefault();
				}
				this.asyncKeyIsPressed[foundKey] = true;
				this.asyncKeyIsDown[foundKey] = true;
				this.asyncKeyIsReleased[foundKey] = false;
				//this.prevHit[foundKey] = true;
			}
		};
		
		this.handle_key_up = function( evt )
		{
			if (typeof evt=="undefined")
			{
				var evt = window.event;
			}
			var foundKey = parseInt( evt.which ) ;
			if( foundKey < this.key_count )
			{
				if( foundKey < gp_np0)
				{
					evt.preventDefault();
				}
				this.asyncKeyIsPressed[foundKey] = false;
				this.asyncKeyIsDown[foundKey] = false;
				this.asyncKeyIsReleased[foundKey] = true;
			}
		};
		
		this.handle_mouse_press = function( evt )
		{
			if (typeof evt=="undefined")
			{
				var evt = window.event;
			}
			var foundButton = evt.button;
			if( foundButton < this.mouse_button_count )
			{
				this.asyncMbIsPressed[foundButton] = true;
				this.asyncMbIsDown[foundButton] = true;
				this.asyncMbIsReleased[foundButton] = false;
				//this.prevHit[foundKey] = true;
			}
		}
		
		this.handle_mouse_up = function( evt )
		{
			if (typeof evt=="undefined")
			{
				var evt = window.event;
			}
			var foundButton = evt.button;
			if( foundButton < this.mouse_button_count )
			{
				this.asyncMbIsPressed[foundButton] = false;
				this.asyncMbIsDown[foundButton] = false;
				this.asyncMbIsReleased[foundButton] = true;
			}
		}
		
		this.process_input = function()
		{
			var key = 0;
			
			for ( key=this.key_count-1; key >=0; key--)
			{
				this.keyIsPressed[key] = this.asyncKeyIsPressed[key];
				this.keyIsDown[key] = this.asyncKeyIsDown[key];
				this.keyIsReleased[key] = this.asyncKeyIsReleased[key];
				
				this.asyncKeyIsPressed[key] = false;
				//this.asyncKeyIsDown[key] = false;
				this.asyncKeyIsReleased[key] = false;
			}
			
			var mb = 0;
			
			for ( mb=this.mouse_button_count-1; mb >=0; mb--)
			{
				this.mbIsPressed[mb] = this.asyncMbIsPressed[mb];
				//this.mbIsDown[mb] = this.asyncMbIsDown[mb];
				this.mbIsReleased[mb] = this.asyncMbIsReleased[mb];
				
				this.asyncMbIsPressed[mb] = false;
				this.asyncMbIsDown[mb] = false;
				this.asyncMbIsReleased[mb] = false;
			}
			//Added into GPE Version 1.1.1 and later
			var axesInc;
			var buttonsInc;
			var mappedButtonId = 0;
			if( GAMEPAD_SUPPORTED )
			{
				var gamepads = navigator.getGamepads();
				var foundController;
				var foundControllerApiController;
				var foundUseableId = 0;
				for (var cController=0; cController<this.max_controllers && cController < gamepads.length ; cController++)
				{
					foundControllerApiController = gamepads[cController];
					if( foundControllerApiController!=null )
					{
						foundController = this.game_controllers[foundUseableId];
						//foundController.reset_controller();
						if( foundControllerApiController.connected )
						{
							foundController.deviceType = foundControllerApiController.id;
							foundController.system_axes_count = foundControllerApiController.axes.length;
							if( !foundController.isConnected )
							{
								console.log( "{"+foundController.deviceType+"} for controller["+cController+"] connected...");
								console.log("Controller detected with "+foundController.system_axes_count+" axes...");
								foundController.pure_reset();
								if( foundController.deviceType == "Wireless Controller (STANDARD GAMEPAD Vendor: 054c Product: 09cc)" )
								{
									foundController.map_to_ds4_chrome();
									foundController.detectedDeviceType = "DullShock4";
								}
								else if( foundController.deviceType == "054c-09cc-Wireless Controller" )
								{
									foundController.map_to_ds4_firefox();
									foundController.detectedDeviceType = "DullShock4";
								}
								if( foundController.deviceType == "Wireless Gamepad (Vendor: 057e Product: 2009)" )
								{
									foundController.map_to_switchpro_chrome();
									foundController.detectedDeviceType = "SwitchPro";
								}
								else if( foundController.deviceType == " 057e-2009-Wireless Gamepad" )
								{
									foundController.map_to_switchpro_firefox();
									foundController.detectedDeviceType = "SwitchPro";
								}
								if( foundController.deviceType == "Mayflash WiiU Pro Game Controller Adapter (Vendor: 0079 Product: 1800)" )
								{
									foundController.map_to_wiiupro_mayflash_chrome();
									foundController.detectedDeviceType = "WIiUPro_MayFlash";
								}
								else if( foundController.deviceType == " 0079-1800-Mayflash WiiU Pro Game Controller Adapter" )
								{
									foundController.map_to_wiiupro_mayflash_firefox();
									foundController.detectedDeviceType = "WIiUPro_MayFlash";
								}
							}
							foundController.isConnected = true;
							foundController.systemController = foundControllerApiController;
							
							foundController.mapping = foundControllerApiController.mapping;
							foundController.timestamp = foundControllerApiController.timestamp;
							for ( axesInc=0; axesInc <foundController.axes_count && axesInc < foundControllerApiController.axes.length; axesInc++)
							{
								foundController.axesValues[ axesInc ] = foundControllerApiController.axes[ axesInc ];
							}
							for ( buttonsInc=0; buttonsInc < foundController.button_count && buttonsInc < foundControllerApiController.buttons.length; buttonsInc++)
							{
								mappedButtonId = foundController.buttonsMap[buttonsInc];
								if( mappedButtonId >=0 && mappedButtonId < foundController.button_count && mappedButtonId < foundControllerApiController.buttons.length )
								{
									foundController.butIsDown[ mappedButtonId]  = foundController.butIsPressed[ mappedButtonId ] = foundControllerApiController.buttons[ mappedButtonId ].pressed;
									if( foundControllerApiController.buttons[ mappedButtonId ].pressed )
									{
										//console.log("Controller["+cController+"] Button ["+foundController.buttonsMap[buttonsInc]+"] is pressed ");
										foundController.butIsDown[ mappedButtonId] = foundController.butIsPressed[ mappedButtonId ] = true;
									}
								}
							}
							
						}
						else
						{
							if( foundController.isConnected )
							{
								console.log( "{"+foundController.deviceType+"} for controller["+cController+"] disconnected...");
							}
							foundController.pure_reset();
							foundController.foundController.systemController = null;
							foundController.isConnected = false;
							foundController.deviceType = "disconnected";
							foundController.mapping = "disconnected";
							foundController.timestamp = 0;
						}
						foundUseableId++;
					}
				}
			}
		};
	}

	//the logo display class
	function GPE_LogoDisplay()
	{
		this.stateName = "LogoDisplay";
		this.logoImg = new Image();
		
		this.logoImg.src = GPE_SETTINGS_APP_LOGO_LOCATION;
		var that = this;
		//this.introMusic.onload = function ()
		this.audioPlayed = false;
		this.displayedTimes = 0;
		this.maxTilExit = 90;
		this.apply_logic = function()
		{
			if( gpe.check_image_loading_status( this.logoImg) )
			{
				this.displayedTimes+=1;
				//context.font = 'bold 32pt Calibri';
				//context.fillText(this.displayedTimes+" / "+this.maxTilExit,640, 640);
				if( this.displayedTimes>=this.maxTilExit)
				{
					set_next_state(programStates.STATE_LOAD_RESOURCES);
				}
			}
		};
		
		this.render_state = function()
		{
			if( typeof gpe.context!="undefined")
			{
				gpe.context.fillStyle = '#CCCCCC';
				gpe.context.fillRect(0, 0, gpe.canvas.width, gpe.canvas.height );
				if( IsImageOk( this.logoImg) )
				{
					var xRender = 0;
					var rWidth = gpe.canvas.width;
					var rHeight = gpe.canvas.height;
					var yRender = 0;
					if( this.logoImg.width < gpe.canvas.width)
					{
						rWidth = this.logoImg.width;
						xRender = (gpe.canvas.width - rWidth)/2;
					}
					if( this.logoImg.height < gpe.canvas.height)
					{
						rHeight = this.logoImg.height;
						yRender = (gpe.canvas.height - rHeight)/2;
					}
					gpe.context.drawImage(this.logoImg, xRender, yRender, rWidth, rHeight);
					if( this.displayedTimes>=this.maxTilExit)
					{
						set_next_state(programStates.STATE_LOAD_RESOURCES);
					}
				}
			}
		};
	}
	
	
	//the Main Game class
	function GPE_MainGame()
	{
		this.sceneWidth = 0;
		this.sceneHeight = 0;
		this.stateName = "MainGame";
		this.currentScene = gpe.gpeScenes[0];
		this.currentSceneId = 0;
		this.pastSceneId = -1;
		this.pastSceneName = "";
		this.sceneWasReset = false;
		gpe.play_audio(this.currentScene.startAudio, false );
		gpe.loop_audio(this.currentScene.bgMusic);
		this.sceneToEnter = 0;
		this.currentScene.update_tilemap();
		
		//Used for handling mouse inputs
		this.MOUSE_CAM_X= [];
		this.MOUSE_CAM_Y = [];
		this.MOUSE_IN_CAM = [];
		for( var imCam = 0; imCam < this.maxCameras; imCam++)
		{
			this.MOUSE_CAM_X[imCam] = 0;
			this.MOUSE_CAM_Y[imCam] = 0;
			this.MOUSE_IN_CAM[imCam] = true;
		}
		//this.currentScene.load_game_scene(GPE_SETTINGS_START_SCENE,'text');
		
		//Used to keep track of game objects
		this.GAME_OBJECTS = []; //listed version of game objects based on object types
		this.GAME_OBJECTS_TO_BE_DESTROYED = [];
		this.GAME_OBJECTS_TO_DELETE_TYPES = [];
		this.needToDeleteObjects = false;
		this.GAME_OBJECTS_CONTINUOUS = [];
		this.GAME_OBJECTS_FAMILIES = [];
		this.OBJECT_COUNT = 0;
		this.MAIN_GAME_CODE_IS_READ = false;
		//Collision related variables
		this.collisionSpacesInView = [];
		this.spatialGridIsCreated = false;
		this.spatialMapWidthSize = 0;
		this.spatialMapHeightSize = 0;
		this.superDebugCollisions = false;

		this.objectCollisionIsChecked = {};
		this.collisionPairA = [];
		this.collisionPairB = [];
		this.collisionPairsTotal = 0;
		
		this.parsed_objects = 0;
		this.spatialGridWidthAmount = 2;
		this.spatialGridHeightAmount = 2;
		this.objectCollisionHashesChecked = 0;
		this.spatialCollisionChecks = 0;
		//

		this.spatialGridIsCreated = false;
		this.spatialMapWidthSize = -1;
		this.spatialMapHeightSize = -1;

		this.set_default_grid_size = function()
		{
			this.spatialGridBoxSize = 128;
			this.spatialGridWidthSize = 128;
			this.spatialGridHeightSize = 128;
		}
		this.set_default_grid_size();
		
		//All class functions should be declared as prototypes.
		this.add_collision_pair = function( objPairA, objPairB)
		{
			this.collisionPairA[this.collisionPairsTotal] = objPairA;
			this.collisionPairB[this.collisionPairsTotal] = objPairB;
			this.collisionPairsTotal+=1;
		}
		
		this.add_object_to_deletion_list = function( deletingObject)
		{
			if( typeof deletingObject!="undefined" && deletingObject!=-1)
			{
				if( deletingObject.gpeObjectType>=0)
				{
					if( this.GAME_OBJECTS_TO_BE_DESTROYED.length > deletingObject.gpeObjectType)
					{
						var objectHolder = this.GAME_OBJECTS_TO_BE_DESTROYED[deletingObject.gpeObjectType];
						objectHolder.push_back(deletingObject);
					}
					else
					{
						var newObjectHolder;
						for(var ii=this.GAME_OBJECTS_TO_BE_DESTROYED.length; ii<= deletingObject.gpeObjectType; ii++)
						{
								newObjectHolder = [];
								this.GAME_OBJECTS_TO_BE_DESTROYED.push(newObjectHolder);
						}
						var newestObjectHolder = this.GAME_OBJECTS_TO_BE_DESTROYED[this.GAME_OBJECTS_TO_BE_DESTROYED.length-1];
						newestObjectHolder.push_back(deletingObject);
					}
					this.GAME_OBJECTS_TO_DELETE_TYPES.push(deletingObject.gpeObjectType);
					this.needToDeleteObjects = true;
				}
			}
		};
		
		this.add_to_continuous_objects = function( nObject)
		{
			if( nObject!=-1)
			{
				var foundCopy = false;
				for( var i = this.GAME_OBJECTS_CONTINUOUS.length - 1; i >=0; i--)
				{
					if( this.GAME_OBJECTS_CONTINUOUS[i].gpeUniqGameObjectId == nObject.gpeUniqGameObjectId)
					{
						foundCopy = true;
						return false;
					}
				}
				if( foundCopy==false)
				{
					this.GAME_OBJECTS_CONTINUOUS.push( nObject );
					this.add_object_to_deletion_list( nObject );
				}
				return true;
			}
			return false;
		};
		
		this.add_object = function (newObject, objectAlreadyExisted, newObjLayerPos)
		{
			if( typeof newObjLayerPos!="number")
			{
				newObjLayerPos = 1;
			}
			if( objectAlreadyExisted )
			{
				newObjLayerPos = newObject.sceneLayerStartPos;
			}
			if( typeof newObject!="undefined" && newObject!=-1 && newObjLayerPos >=0 && newObjLayerPos < 32 )
			{
				if( newObject.gpeObjectType>=0 && this.currentScene!=-1)
				{
					var currentSceneLayer = this.currentScene.get_layer(newObjLayerPos, GPE_LAYER_TYPE_OBJECTS);
					if( currentSceneLayer!=-1)
					{
						if( this.GAME_OBJECTS.length > newObject.gpeObjectType)
						{
							var cOHolder = this.GAME_OBJECTS[newObject.gpeObjectType];
							cOHolder.push(newObject);
							this.GAME_OBJECTS[newObject.gpeObjectType] = cOHolder;
						}
						else
						{
							var newObjectHolder;
							for(var ii=this.GAME_OBJECTS.length; ii<= newObject.gpeObjectType; ii++)
							{
									newObjectHolder = [];
									this.GAME_OBJECTS.push_back(newObjectHolder);
							}
							var cOHolder = this.GAME_OBJECTS[this.GAME_OBJECTS.length-1];
							cOHolder.push(newObject);
						}
						if( newObject.gpeUniqGameObjectId==-1)
						{
							this.OBJECT_COUNT += 1;
							newObject.gpeUniqGameObjectId = this.OBJECT_COUNT;
						}
						
						newObject.isInit = true;
						newObject.gpeSceneStartCodeUsed = false;
						for(var ii=0; ii <gpe.maxCameras; ii++)
						{
							newObject.isInView[ii] = true;
						}
						
						if( newObject.isViewIndependent == 1 )
						{
							currentSceneLayer.independentObjects.push(newObject);
							newObject.indepedentCameraPos = currentSceneLayer.independentObjects.length - 1;
						}
						currentSceneLayer.layerObjects.push( newObject );
						if( !objectAlreadyExisted )
						{
							newObject.gpeSceneStartId = this.currentSceneId;
						}
						this.update_object_in_map(newObject);
						newObject.GPE_AddedToGridSystem = true;
						
						this.sceneIsSpatiallyPartitioned = false;
						//console.log("Adding an object of type ["+newObject.gpeObjectType+"].");
						//console.log(newObject);
					}
					else
					{
						console.log("Unable to obtain layer from current scene with id: ["+newObjLayerPos+"].");
					}
				}
				else
				{
					console.log("Unable to add object of type ["+newObject.gpeObjectType+"].");
				}
			}
			else
			{
				console.log("Undefined object added...");
			}
		};
	
		this.apply_game_collisions = function()
		{
			var matchA, matchB;
			for( var i = this.collisionPairsTotal - 1; i >=0; i--)
			{
				matchA = this.collisionPairA[i];
				matchB = this.collisionPairB[i];
				matchA.collide_with(matchB);
				matchB.collide_with(matchA);
			}
		}
		
		this.apply_spatial_game_objects_prelogics = function()
		{
			var jOBjectParse = 0;
			var currentSpace = IS_NULL;
			var foundGameObject = IS_NULL;
			var i = 0;
			var iObject;
			
			var tempSceneLayer = IS_NULL;
			var spacesArray = this.collisionSpacesInView;
			var foundCollisionSpaceId = 0;
			for(var iLayer = this.currentScene.sceneLayers.length-1; iLayer >= 0; iLayer--)
			{
				tempSceneLayer = this.currentScene.sceneLayers[iLayer];
				for( i=tempSceneLayer.independentObjects.length-1; i >=0; i--)
				{
					foundGameObject = tempSceneLayer.independentObjects[i];
					if( foundGameObject.gpePrelogicAppliedInFrame==false && foundGameObject.isInit && !foundGameObject.gpeIsBeingDestroyed )
					{
						foundGameObject.perform_object_prelogic();
						if( foundGameObject.gpeHasStartLogicFunction) foundGameObject.start_frame_logic();
						if( foundGameObject.gpeHasTimedFunctions) foundGameObject.process_timed_functions();
						foundGameObject.process_gpe_movement();
						foundGameObject.gpePrelogicAppliedInFrame = true;
					}
				}
				
				for( i = this.collisionSpacesInView.length-1; i >= 0; i--)
				{
					foundCollisionSpaceId = this.collisionSpacesInView[i];
					if( foundCollisionSpaceId >=0 && foundCollisionSpaceId < tempSceneLayer.COLLISION_AREA_SPACES.size() )
					{
						currentSpace = tempSceneLayer.COLLISION_AREA_SPACES.at( foundCollisionSpaceId );
						for( jOBjectParse = currentSpace.internalArray.length-1;jOBjectParse >= 0; jOBjectParse--)
						{
							foundGameObject = currentSpace.internalArray[jOBjectParse];
							if( foundGameObject.isViewIndependent==false && foundGameObject.gpePrelogicAppliedInFrame==false && foundGameObject.isInit  && !foundGameObject.gpeIsBeingDestroyed )
							{
								foundGameObject.perform_object_prelogic();
								if( foundGameObject.gpeHasStartLogicFunction) foundGameObject.start_frame_logic();
								if( foundGameObject.gpeHasTimedFunctions) foundGameObject.process_timed_functions();
								foundGameObject.process_gpe_movement();
								foundGameObject.gpePrelogicAppliedInFrame = true;
							}
						}
					}
				}
				
				for(iObject = tempSceneLayer.COLLISION_AREA_OUTSIDE_SCENE.internalArray.length-1; iObject >= 0; iObject--)
				{
					foundGameObject = tempSceneLayer.COLLISION_AREA_OUTSIDE_SCENE.internalArray[iObject];
					if( foundGameObject.isViewIndependent==false && foundGameObject.gpePrelogicAppliedInFrame==false && foundGameObject.isInit  && !foundGameObject.gpeIsBeingDestroyed )
					{
						foundGameObject.perform_object_prelogic();
						if( foundGameObject.gpeHasStartLogicFunction) foundGameObject.start_frame_logic();
						if( foundGameObject.gpeHasTimedFunctions) foundGameObject.process_timed_functions();
						foundGameObject.process_gpe_movement();
						foundGameObject.gpePrelogicAppliedInFrame = true;
					}
				}
			}
		}
		
		this.apply_spatial_game_objects_logics = function()
		{
			var jOBjectParse = 0;
			var currentSpace = IS_NULL;
			var foundGameObject = IS_NULL;
			
			var iObject = 0;
			
			var tempSceneLayer = IS_NULL;
			var spacesArray = this.collisionSpacesInView;
			for(var iLayer = this.currentScene.sceneLayers.length-1; iLayer >= 0; iLayer--)
			{
				tempSceneLayer = this.currentScene.sceneLayers[iLayer];
				
				for(  iObject=tempSceneLayer.independentObjects.length-1; iObject >=0; iObject--)
				{
					foundGameObject = tempSceneLayer.independentObjects[iObject];
					if( foundGameObject.isInit && foundGameObject.gpeLogicAppliedInFrame==false  && foundGameObject.gpeIsBeingDestroyed == false )
					{
						if( foundGameObject.imageSpeed!=0) foundGameObject.update_animation();
						if(foundGameObject.gpeHasLogicFunction) foundGameObject.perform_object_logic();
						foundGameObject.gpeLogicAppliedInFrame = true;
					}
				}
				
				for(iObject = tempSceneLayer.COLLISION_AREA_OUTSIDE_SCENE.internalArray.length-1; iObject >= 0; iObject--)
				{
					foundGameObject = tempSceneLayer.COLLISION_AREA_OUTSIDE_SCENE.internalArray[iObject];
					
					if( foundGameObject.isViewIndependent==false && foundGameObject.isInit && foundGameObject.gpeLogicAppliedInFrame==false  && foundGameObject.gpeIsBeingDestroyed == false )
					{
						if( foundGameObject.imageSpeed!=0) foundGameObject.update_animation();
						foundGameObject.perform_object_logic();
						foundGameObject.gpeLogicAppliedInFrame = true;
					}
				}
				
				for(var iSpace = this.collisionSpacesInView.length-1; iSpace >= 0; iSpace--)
				{
					currentSpace = tempSceneLayer.COLLISION_AREA_SPACES.at( this.collisionSpacesInView[iSpace] );
					for( jOBjectParse = currentSpace.objectsHeld-1;jOBjectParse >= 0; jOBjectParse--)
					{
						foundGameObject = currentSpace.internalArray[jOBjectParse];
						if( foundGameObject.isViewIndependent==false && foundGameObject.isInit && foundGameObject.gpeLogicAppliedInFrame==false  && foundGameObject.gpeIsBeingDestroyed == false )
						{
							if( foundGameObject.imageSpeed!=0) foundGameObject.update_animation();
							if(foundGameObject.gpeHasLogicFunction) foundGameObject.perform_object_logic();
							foundGameObject.gpeLogicAppliedInFrame = true;
						}
					}
				}
			}
		}
		
		this.apply_logic = function()
		{
			if( this.currentScene!=-1 )
				for( var imCam = 0; imCam < gpe.maxCameras; imCam++)
				{
					var cCam = this.currentScene.sceneCamera[imCam];
					if( cCam.isVisible == true )
					{
						var tempXScale = gpe.masterScaleX;
						var tempYScale = gpe.masterScaleY;
						this.MOUSE_CAM_X[imCam] = (gpe.MOUSE_SCALED_X + cCam.cameraRect.xPos - tempXScale*cCam.renderRect.xPos)|0;
						this.MOUSE_CAM_Y[imCam] = (gpe.MOUSE_SCALED_Y + cCam.cameraRect.yPos - tempYScale*cCam.renderRect.yPos)|0;
						if( this.MOUSE_CAM_X[imCam] > 0 && this.MOUSE_CAM_Y[imCam] >=0 )
						{
							this.MOUSE_IN_CAM[imCam] = true;
						}
						else
						{
							this.MOUSE_CAM_X[imCam] = 0;
							this.MOUSE_CAM_Y[imCam] = 0;
							this.MOUSE_IN_CAM[imCam] = false;
						}
					}
					else
					{
						this.MOUSE_CAM_X[imCam] = 0;
						this.MOUSE_CAM_Y[imCam] = 0;
						this.MOUSE_IN_CAM[imCam] = false;
					}
				}
			
			if( this.GAME_OBJECTS_FAMILIES.length == 0)
				GPE.activate_object_families();
			
			this.process_scene_movement();
			if( this.currentScene!=-1)
			{
				if( this.currentScene.sceneHasBeenActivated!=true)
				{
					this.begin_spatialpartioning(this.currentScene.sceneWidth, this.currentScene.sceneHeight);					
					this.currentScene.scene_init();
					this.sceneWidth = this.currentScene.sceneWidth;
					this.sceneHeight = this.currentScene.sceneHeight;
				}
				this.currentScene.process_scene();
				if( this.currentScene.sceneIsProcessed && this.currentScene.sceneResourcesAreProcessed )
				{
					//deletes objects set for deletion
					this.delete_selected_objects();
					//calculaes the camera viewing spaces for this frame
					this.calculate_camera_spacing();
										
					//Prepares the current game frame for all future logic & collisions
					this.prepare_game_frame();
					
					//apply game object prelogics
					this.apply_spatial_game_objects_prelogics();
					
					var currentSpace;
					var currentOtherSpace;
					var tempMatrixArray;
					var iTempLayerMatrixPos;
					var iSpace;
					var iLayer;
					var jLayer;
					var tempLayer = IS_NULL;
					var tempOtherLayer = IS_NULL;
					var tempSoughtLayerId = IS_NULL;
					var foundCollisionSpaceId = IS_NULL;

					for( iLayer = this.currentScene.sceneLayers.length-1; iLayer >= 0; iLayer--)
					{
						tempLayer = this.currentScene.sceneLayers[iLayer];
						if( Game_SelfCollideLayerList[ tempLayer.layerId]==true )
						{
							for( iSpace = this.collisionSpacesInView.length-1; iSpace >= 0; iSpace--)
							{
								foundCollisionSpaceId = this.collisionSpacesInView[iSpace];
								if( foundCollisionSpaceId >=0 && foundCollisionSpaceId < tempLayer.COLLISION_AREA_SPACES.size() )
								{
									currentSpace = tempLayer.COLLISION_AREA_SPACES.at( foundCollisionSpaceId );
									this.find_collisions_of_spatialpartion(currentSpace);
								}
							}
							this.find_collisions_of_spatialpartion(tempLayer.COLLISION_AREA_OUTSIDE_SCENE);
						}
						tempMatrixArray = Game_LayerMatrix[tempLayer.layerId];
						for( iTempLayerMatrixPos = tempMatrixArray.length-1; iTempLayerMatrixPos>=0; iTempLayerMatrixPos--)
						{
							tempSoughtLayerId = tempMatrixArray[iTempLayerMatrixPos];
							for( jLayer = iLayer-1; jLayer >= 0; jLayer--)
							{
								tempOtherLayer = this.currentScene.sceneLayers[jLayer];
								if( tempOtherLayer.layerId == tempSoughtLayerId )
								{
									for( iSpace = this.collisionSpacesInView.length-1; iSpace >= 0; iSpace--)
									{
										foundCollisionSpaceId = this.collisionSpacesInView[iSpace];
										if( foundCollisionSpaceId >=0 && foundCollisionSpaceId < tempOtherLayer.COLLISION_AREA_SPACES.size() )
										{
											currentSpace = tempLayer.COLLISION_AREA_SPACES.at( foundCollisionSpaceId );
											currentOtherSpace = tempOtherLayer.COLLISION_AREA_SPACES.at( foundCollisionSpaceId );
											this.find_collisions_of_two_spatialpartions(currentSpace,currentOtherSpace);
										}
									}
									this.find_collisions_of_two_spatialpartions(tempLayer.COLLISION_AREA_OUTSIDE_SCENE,tempOtherLayer.COLLISION_AREA_OUTSIDE_SCENE);
								}
							}
						}
					}
					
					this.apply_game_collisions();
					//apply game object logics
					this.apply_spatial_game_objects_logics();
					
					//deletes objects set for deletion
					this.delete_selected_objects();
				}
			}
		};
		
		this.calculate_camera_spacing = function()
		{
			this.collisionSpacesInView = [];
			var cSceneViewFound;
			var iV = 0;
			var icameraX = 0;
			var jcameraY = 0;
			var viewPartitionSpace = 0;
			var currentSpace = IS_NULL;
			if( this.currentScene.sceneIsProcessed && this.currentScene.sceneResourcesAreProcessed )
			{
				var camMaxXPos = 0;
				var camMaxYPos = 0;
				var tempCLayer = IS_NULL;
				var tempDefLayer = IS_NULL;
				for( var iTempLayer = 0; iTempLayer< this.currentScene.sceneLayers.length; iTempLayer++)
				{
					tempCLayer = this.currentScene.sceneLayers[iTempLayer];
					if( tempCLayer.COLLISION_AREA_SPACES.length > 0)
					{
						tempDefLayer = tempCLayer;
						break;
					}
				}
				
				if( tempDefLayer!=IS_NULL)
				{
					for( iV=0; iV < gpe.maxCameras; iV++)
					{
						cSceneViewFound =this.currentScene.sceneCamera[iV];
						if( cSceneViewFound.isVisible ==true)
						{
							camMaxXPos = cSceneViewFound.cameraRect.x2Pos+this.spatialGridWidthSize;
							camMaxYPos = cSceneViewFound.cameraRect.y2Pos+this.spatialGridHeightSize;
							//console.log(cSceneViewFound.cameraRect.xPos );
							//console.log(cSceneViewFound.cameraRect.yPos );
							for( icameraX = cSceneViewFound.cameraRect.xPos; icameraX <= camMaxXPos;icameraX+=this.spatialGridWidthSize)
							{
								for( jcameraY = cSceneViewFound.cameraRect.yPos; jcameraY <= camMaxYPos;jcameraY+=this.spatialGridHeightSize)
								{
									viewPartitionSpace = (( icameraX / this.spatialGridWidthSize)|0)*this.spatialGridHeightAmount;
									viewPartitionSpace+= ( jcameraY / this.spatialGridHeightSize)|0;
									if( viewPartitionSpace >=0 && viewPartitionSpace < tempDefLayer.COLLISION_AREA_SPACES.length )
									{
										this.collisionSpacesInView.push(viewPartitionSpace);
									}
								}
							}
						}
					}
				}
			}	
			this.collisionSpacesInView = gpe.make_array_unique(this.collisionSpacesInView );
		};
		
		this.camera_on = function( cameraId)
		{
			if( this.currentScene!=-1)
			{
				if( cameraId >=0 && cameraId < gpe.maxCameras)
				{
					return this.currentScene.sceneCamera[cameraId].isVisible;
				}
			}
			return IS_NULL;
		}
		
		this.center_camera = function( cameraId, centerX, centerY)
		{
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				this.currentScene.sceneCamera[cameraId].center_camera(centerX, centerY);
			}
		}
		
		this.check_collision_rect = function( rectA, rectB)
		{
			//Calculate the sides of rect A
			var leftA = rectA.xPos;
			var rightA = rectA.xPos + rectA.rectWidth;
			var topA = rectA.yPos;
			var bottomA = rectA.yPos + rectA.rectHeight;

			//Calculate the sides of rect B
			var leftB = rectB.xPos;
			var rightB = rectB.xPos + rectB.rectWidth;
			var topB = rectB.yPos;
			var bottomB = rectB.yPos + rectB.rectHeight;
			return !(  bottomA < topB ||  topA > bottomB  || rightA < leftB || leftA > rightB );
		}

		this.check_collision_with_rect = function( boxX, boxY, boxW, boxH, rectA)
		{
			var newRectB= new GPE_Rect();
			newRectB.update_shape(boxX, boxY, boxW, boxH);
			return this.check_collision_rect(rectA,newRectB);
		};

		this.disable_camera = function( cameraId)
		{
			if( this.currentScene!=-1)
			{
				this.currentScene.disable_camera( cameraId );
			}
		};
		
		this.delete_selected_objects = function()
		{
			if( this.needToDeleteObjects == true)
			{
				var jj = -1;
				var kItr = -1;
				var objTypeInDeletion = -1;
				var foundDelObjTypeHolder = -1;
				var foundRegObj = -1;
				var fcSpace = -1;
				var spaceToDeleteObjectFrom = 0;
				
				
				var matchingLayer = IS_NULL;
				var foundLayerMatch = false;
				var iCLayer = 0;
				 
					
						
				this.GAME_OBJECTS_TO_DELETE_TYPES = gpe.make_array_unique(this.GAME_OBJECTS_TO_DELETE_TYPES);
				for( var iHolderType = this.GAME_OBJECTS_TO_DELETE_TYPES.length-1; iHolderType>=0; iHolderType--)
				{
					
					objTypeInDeletion = this.GAME_OBJECTS_TO_DELETE_TYPES[iHolderType];
					foundDelObjTypeHolder = this.GAME_OBJECTS_TO_BE_DESTROYED[ objTypeInDeletion ];
					for( jj=foundDelObjTypeHolder.length-1; jj>=0; jj--)
					{
						foundRegObj = foundDelObjTypeHolder[jj];
						foundLayerMatch = false;
						for( iCLayers = GPR.currentScene.sceneLayers.length-1; iCLayers >=0; iCLayers--)
						{
							matchingLayer = GPR.currentScene.sceneLayers[iCLayers];
							if( matchingLayer.layerId == foundRegObj.sceneLayerPos)
							{
								foundLayerMatch = true;
								break;
							}
						
						}
						if( foundLayerMatch )
						{
							//matchingLayer.remove_object( foundRegObj );
							for( kItr = foundRegObj.SPATIAL_PARTITION_LIST.length -1; kItr>=0; kItr--)
							{
								fcSpace = foundRegObj.SPATIAL_PARTITION_LIST[kItr];
								if( fcSpace < matchingLayer.COLLISION_AREA_SPACES.length && fcSpace >=0 )
								{
									spaceToDeleteObjectFrom = matchingLayer.COLLISION_AREA_SPACES[fcSpace];
									spaceToDeleteObjectFrom.remove_held_object(foundRegObj);
								}
								else
								{
									matchingLayer.COLLISION_AREA_OUTSIDE_SCENE.remove_held_object(foundRegObj);
								}
							}
						}
					}
				}
				this.GAME_OBJECTS_TO_DELETE_TYPES = [];
				this.GAME_OBJECTS_TO_BE_DESTROYED = [];
				this.needToDeleteObjects = false;
			}
		}
		
		this.enable_camera = function( cameraId)
		{
			if( this.currentScene!=-1)
			{
				this.currentScene.enable_camera(cameraId );
			}
		}
		
		//returns the first object of otherObjectType that objOne collides with if it is at (xIn,yIn)
		this.find_collision_rectangle = function(  objOne,  xIn,  yIn,  otherObjectType, layerIdToCheck)
		{
			//makes sure our object isn't null before checking
			if( typeof layerIdToCheck!="number")
			{
				layerIdToCheck = objOne.sceneLayerPos; //default object layer...
			}
			
			if(objOne!=-1 && layerIdToCheck >=0 && layerIdToCheck < 32 )
			{
				var matchingLayer = IS_NULL;
				var foundLayerMatch = false;
				for( var iCLayers = GPR.currentScene.sceneLayers.length-1; iCLayers >=0; iCLayers--)
				{
					matchingLayer = GPR.currentScene.sceneLayers[iCLayers];
					if( matchingLayer.layerId == layerIdToCheck)
					{
						foundLayerMatch = true;
						break;
					}
				
				}
				if( foundLayerMatch )
				{
					var tempX =objOne.collisionBox.xPos;
					var tempY = objOne.collisionBox.yPos;
					if( objOne.spriteId>=0 && objOne.spriteId < gpe.rsm.rSprites.length )
					{
						var fSprite = gpe.rsm.rSprites[objOne.spriteId];
						objOne.collisionBox.update_position( xIn+fSprite.colX, yIn+fSprite.colY);
					}
					else
					{	
						objOne.collisionBox.update_position( xIn, yIn);
					}
					var tempColSpaceList = [];
					var positionSpaceId = IS_NULL;
					
					positionSpaceId = matchingLayer.find_spatial_at( objOne.collisionBox.xPos, objOne.collisionBox.yPos);
					tempColSpaceList.push( positionSpaceId);

					positionSpaceId = matchingLayer.find_spatial_at( objOne.collisionBox.x2Pos, objOne.collisionBox.yPos);
					tempColSpaceList.push( positionSpaceId);
					
					positionSpaceId = matchingLayer.find_spatial_at( objOne.collisionBox.xPos, objOne.collisionBox.y2Pos);
					tempColSpaceList.push( positionSpaceId);
					
					positionSpaceId = matchingLayer.find_spatial_at( objOne.collisionBox.x2Pos, objOne.collisionBox.y2Pos);
					tempColSpaceList.push( positionSpaceId);
					
					tempColSpaceList = gpe.make_array_unique(tempColSpaceList);
					var positionSpace = IS_NULL;
					var cObjController = IS_NULL;
					var iItr = IS_NULL;
					var fObject = IS_NULL;
					var iColSpace;
					
					if( otherObjectType >= 0 && otherObjectType < GPE_MAX_OBJECT_TYPES )
					{
						if( objOne.gpeObjectType==otherObjectType)
						{
							for( var iColSpace = tempColSpaceList.length-1; iColSpace>=0; iColSpace--)
							{
								positionSpaceId = tempColSpaceList[iColSpace];
								if( positionSpaceId >= 0 || positionSpaceId < matchingLayer.COLLISION_AREA_SPACES.length)
								{
									positionSpace = matchingLayer.COLLISION_AREA_SPACES.at(positionSpaceId);
								}
								else
								{
									positionSpace = matchingLayer.COLLISION_AREA_OUTSIDE_SCENE;
								}
								
								if( positionSpace==-1)
								{
									positionSpace = matchingLayer.COLLISION_AREA_OUTSIDE_SCENE;
								}
								cObjController = positionSpace.heldObjectsArray[otherObjectType];
								for( iItr = cObjController.length-1; iItr>=0;iItr--)
								{
									fObject = cObjController[iItr];
									if( fObject!=-1)
									{
										if( fObject.gpeUniqGameObjectId!=objOne.gpeUniqGameObjectId)
										{
											if( gpe.check_collision_rect( objOne.collisionBox, fObject.collisionBox) )
											{
												objOne.collisionBox.update_position( tempX, tempY);
												return fObject;
											}
										}
									}
								}
							}
						}
						else
						{
							for( iColSpace = tempColSpaceList.length-1; iColSpace>=0; iColSpace--)
							{
								positionSpaceId = tempColSpaceList[iColSpace];
								if( positionSpaceId >= 0 || positionSpaceId < matchingLayer.COLLISION_AREA_SPACES.length)
								{
									positionSpace = matchingLayer.COLLISION_AREA_SPACES.at(positionSpaceId);
								}
								else
								{
									positionSpace = matchingLayer.COLLISION_AREA_OUTSIDE_SCENE;
								}
								if( otherObjectType >= 0 && otherObjectType < GPE_MAX_OBJECT_TYPES )
								{
									if( positionSpace==-1)
									{
										positionSpace = matchingLayer.COLLISION_AREA_OUTSIDE_SCENE;
									}
									cObjController = positionSpace.heldObjectsArray[otherObjectType];
									for( iItr = cObjController.length-1; iItr>=0;iItr--)
									{
										fObject = cObjController[iItr];
										if( fObject!=-1)
										{
											if( gpe.check_collision_rect( objOne.collisionBox, fObject.collisionBox) )
											{
												objOne.collisionBox.update_position( tempX, tempY);
												return fObject;
											}
										}
									}
								}
							}
						}
					}
					else
					{
						for( iColSpace = tempColSpaceList.length-1; iColSpace>=0; iColSpace--)
						{
							positionSpaceId = tempColSpaceList[iColSpace];
							if( positionSpaceId >= 0 || positionSpaceId < matchingLayer.COLLISION_AREA_SPACES.length)
							{
								positionSpace = matchingLayer.COLLISION_AREA_SPACES.at(positionSpaceId);
							}
							else
							{
								positionSpace = matchingLayer.COLLISION_AREA_OUTSIDE_SCENE;
							}
							for( iItr = positionSpace.internalArray.length-1; iItr>=0;iItr--)
							{
								fObject = positionSpace.internalArray[iItr];
								if( fObject.gpeUniqGameObjectId!=objOne.gpeUniqGameObjectId)
								{
									if( gpe.check_collision_rect( objOne.collisionBox, fObject.collisionBox) )
									{
										objOne.collisionBox.update_position( tempX, tempY);
										return fObject;
									}
								}
							}
						}
					}
					//in the event of no collision reset object
					objOne.collisionBox.update_position( tempX, tempY);
				}
			}
			
			return IS_NULL;
		};

		//returns the first MEMBER of the familyToSearch family of objects[top->down] that objOne collides with if it is at (xIn,yIn)
		this.touches_family = function(  objOne,  xIn,  yIn,  familyToSearch, layerIdToCheck)
		{
			//makes sure our object isn't null before checking
			if( typeof layerIdToCheck!="number")
			{
				layerIdToCheck = objOne.sceneLayerPos; //default object layer...
			}
			
			
			//makes sure our object isn't null before checking
			if(objOne!=-1 && layerIdToCheck >=0 && layerIdToCheck < 32 )
			{
				var matchingLayer = IS_NULL;
				var foundLayerMatch = false;
				for( var iCLayers = GPR.currentScene.sceneLayers.length-1; iCLayers >=0; iCLayers--)
				{
					matchingLayer = GPR.currentScene.sceneLayers[iCLayers];
					if( matchingLayer.layerId == layerIdToCheck)
					{
						foundLayerMatch = true;
						break;
					}
				
				}
				if( foundLayerMatch )
				{
					var familyContainer = -1;
					familyContainer = this.GAME_OBJECTS_FAMILIES[familyToSearch];
					//console.log( familyContainer );
					if( familyContainer.length == 0)
					{
						return this.find_collision_rectangle( objOne,  xIn,  yIn,  familyToSearch, layerId );
					}
					if( familyToSearch >= 0 && familyToSearch < GPE_MAX_OBJECT_TYPES )
					{
						var memberToSearchType = -1;
						var tempX =objOne.collisionBox.xPos;
						var tempY = objOne.collisionBox.yPos;
						if( objOne.spriteId>=0 && objOne.spriteId < gpe.rsm.rSprites.length )
						{
							var fSprite = gpe.rsm.rSprites[objOne.spriteId];
							objOne.collisionBox.update_position( xIn+fSprite.colX, yIn+fSprite.colY);
						}
						else
						{	
							objOne.collisionBox.update_position( xIn, yIn);
						}
						var tempColSpaceList = [];
						var positionSpaceId = IS_NULL;
						
						positionSpaceId = matchingLayer.find_spatial_at( objOne.collisionBox.xPos, objOne.collisionBox.yPos);
						tempColSpaceList.push( positionSpaceId);

						positionSpaceId = matchingLayer.find_spatial_at( objOne.collisionBox.x2Pos, objOne.collisionBox.yPos);
						tempColSpaceList.push( positionSpaceId);
							
						positionSpaceId = matchingLayer.find_spatial_at( objOne.collisionBox.xPos, objOne.collisionBox.y2Pos);
						tempColSpaceList.push( positionSpaceId);

						positionSpaceId = matchingLayer.find_spatial_at( objOne.collisionBox.x2Pos, objOne.collisionBox.y2Pos);
						tempColSpaceList.push( positionSpaceId);

						tempColSpaceList = gpe.make_array_unique(tempColSpaceList);
						var positionSpace = IS_NULL;
						var cObjController = IS_NULL;
						var iItr = IS_NULL;
						var fObject = IS_NULL;
						var iColSpace = 0;
						var jItr = 0;
						for( var iColSpace = tempColSpaceList.length-1; iColSpace>=0; iColSpace--)
						{
							positionSpaceId = tempColSpaceList[iColSpace];
							if( positionSpaceId >= 0 || positionSpaceId < matchingLayer.COLLISION_AREA_SPACES.length)
							{
								positionSpace = matchingLayer.COLLISION_AREA_SPACES.at(positionSpaceId);
							}
							else
							{
								positionSpace = matchingLayer.COLLISION_AREA_OUTSIDE_SCENE;
							}
							
							if( positionSpace==-1)
							{
								positionSpace = matchingLayer.COLLISION_AREA_OUTSIDE_SCENE;
							}
							for( iItr = familyContainer.length-1; iItr>=0; iItr--)
							{
								memberToSearchType = familyContainer[iItr];
								cObjController = positionSpace.heldObjectsArray[memberToSearchType];
								if( objOne.gpeObjectType==memberToSearchType)
								{
									for( jItr = cObjController.length-1; jItr>=0;jItr--)
									{
										fObject = cObjController[jItr];
										if( fObject!=-1)
										{
											if( fObject.gpeUniqGameObjectId!=objOne.gpeUniqGameObjectId)
											{
												if( gpe.check_collision_rect( objOne.collisionBox, fObject.collisionBox) )
												{
													objOne.collisionBox.update_position( tempX, tempY);
													return fObject;
												}
											}
										}
									}
								}
								else
								{
									for( jItr = cObjController.length-1; jItr>=0;jItr--)
									{
										fObject = cObjController[jItr];
										if( fObject!=-1)
										{
											if( gpe.check_collision_rect( objOne.collisionBox, fObject.collisionBox) )
											{
												objOne.collisionBox.update_position( tempX, tempY);
												return fObject;
											}
										}
									}
								}
							}
						}
					}
					else
					{
						return this.find_collision_rectangle( objOne,  xIn,  yIn,  -1 , layerId);
					}
				}
			}
			return IS_NULL;
		};

		//will be optimized later to only process object types with collision lists
		this.find_collisions_of_spatialpartion = function(currentSpace)
		{
			var jObjectTypeToCollide = IS_NULL;
			var jObjectHolder = IS_NULL;
			var jSubObjItr = 0;
			var jObjectToCollide = IS_NULL;
			
			var kObjectTypeToCollide = IS_NULL;
			var kObjectHolder = IS_NULL;
			var kSubObjItr = 0;
			var kObjectToCollide = IS_NULL;
			var objUniqColIdA = -1;
			var objUniqColIdB = -1;
			
			//loops through collision object types
			for( jObjectTypeToCollide = GPE_MAX_OBJECT_TYPES-1; jObjectTypeToCollide >=0 ; jObjectTypeToCollide--)
			{
				jObjectHolder = currentSpace.heldObjectsArray[jObjectTypeToCollide];
				//loops through objects within object type
				for(jSubObjItr = jObjectHolder.length-1; jSubObjItr >= 0 ; jSubObjItr-- )
				{
					jObjectToCollide = jObjectHolder[jSubObjItr];
					//console.log("Searching "+jObjectToCollide.gpeUniqGameObjectId+"...");
					for( kObjectTypeToCollide = jObjectToCollide.OBJECT_COLLISION_LIST.length - 1; kObjectTypeToCollide >=0; kObjectTypeToCollide--)
					{
						kObjectHolder = currentSpace.heldObjectsArray[ jObjectToCollide.OBJECT_COLLISION_LIST[kObjectTypeToCollide] ];
						//loops through objects within object type
						for(kSubObjItr = kObjectHolder.length-1; kSubObjItr >= 0 ; kSubObjItr-- )
						{
							kObjectToCollide = kObjectHolder[kSubObjItr];
							if( jObjectToCollide.gpeUniqGameObjectId !=kObjectToCollide.gpeUniqGameObjectId )
							{
								this.objectCollisionHashesChecked+=2;
								objUniqColIdA = jObjectToCollide.gpeUniqGameObjectId + ':' + kObjectToCollide.gpeUniqGameObjectId;
								objUniqColIdB = kObjectToCollide.gpeUniqGameObjectId + ':' + jObjectToCollide.gpeUniqGameObjectId;
								 
								if( !this.objectCollisionIsChecked[objUniqColIdA] && !this.objectCollisionIsChecked[objUniqColIdB] )
								{
									// mark this pair as checked
									this.objectCollisionIsChecked[objUniqColIdA] = this.objectCollisionIsChecked[objUniqColIdB] = true;
									this.spatialCollisionChecks+=1;
									if( this.check_collision_rect(jObjectToCollide.collisionBox,kObjectToCollide.collisionBox) )
									{
										this.add_collision_pair(jObjectToCollide,kObjectToCollide);
									}
								}
							}
						}
					}
				}		
			}
		}
		
		//will be optimized later to only process object types with collision lists
		this.find_collisions_of_two_spatialpartions = function(currentSpace, otherSpace)
		{
			var jObjectTypeToCollide = IS_NULL;
			var jObjectHolder = IS_NULL;
			var jSubObjItr = 0;
			var jObjectToCollide = IS_NULL;
			
			var kObjectTypeToCollide = IS_NULL;
			var kObjectHolder = IS_NULL;
			var kSubObjItr = 0;
			var kObjectToCollide = IS_NULL;
			var objUniqColIdA = -1;
			var objUniqColIdB = -1;
			
			//loops through collision object types
			for( jObjectTypeToCollide = currentSpace.heldObjectsArray.size()-1; jObjectTypeToCollide >=0 ; jObjectTypeToCollide--)
			{
				jObjectHolder = currentSpace.heldObjectsArray[jObjectTypeToCollide];
				//loops through objects within object type
				for(jSubObjItr = jObjectHolder.length-1; jSubObjItr >= 0 ; jSubObjItr-- )
				{
					jObjectToCollide = jObjectHolder[jSubObjItr];
					//console.log("Searching "+jObjectToCollide.gpeUniqGameObjectId+"...");
					for( kObjectTypeToCollide = jObjectToCollide.OBJECT_COLLISION_LIST.length - 1; kObjectTypeToCollide >=0; kObjectTypeToCollide--)
					{
						kObjectHolder = otherSpace.heldObjectsArray[ jObjectToCollide.OBJECT_COLLISION_LIST[kObjectTypeToCollide] ];
						//loops through objects within object type
						for(kSubObjItr = kObjectHolder.length-1; kSubObjItr >= 0 ; kSubObjItr-- )
						{
							kObjectToCollide = kObjectHolder[kSubObjItr];
							if( jObjectToCollide.gpeUniqGameObjectId !=kObjectToCollide.gpeUniqGameObjectId )
							{
								this.objectCollisionHashesChecked+=2;
								objUniqColIdA = jObjectToCollide.gpeUniqGameObjectId + ':' + kObjectToCollide.gpeUniqGameObjectId;
								objUniqColIdB = kObjectToCollide.gpeUniqGameObjectId + ':' + jObjectToCollide.gpeUniqGameObjectId;
								 
								if( !this.objectCollisionIsChecked[objUniqColIdA] && !this.objectCollisionIsChecked[objUniqColIdB] )
								{
									// mark this pair as checked
									this.objectCollisionIsChecked[objUniqColIdA] = this.objectCollisionIsChecked[objUniqColIdB] = true;
									this.spatialCollisionChecks+=1;
									if( this.check_collision_rect(jObjectToCollide.collisionBox,kObjectToCollide.collisionBox) )
									{
										this.add_collision_pair(jObjectToCollide,kObjectToCollide);
									}
								}
							}
						}
					}
				}		
			}
		}
		
		this.find_collisions_in_all_spatialpartions = function()
		{
			var currentSpace = IS_NULL;
			var iSpace = IS_NULL;
			//loops through collision spaces
			for(iSpace = this.COLLISION_AREA_SPACES.size()-1; iSpace >= 0; iSpace--)
			{
				currentSpace = this.COLLISION_AREA_SPACES.at(iSpace);
				this.find_collisions_of_spatialpartion(currentSpace);
			}
			this.find_collisions_of_spatialpartion(this.COLLISION_AREA_OUTSIDE_SCENE);
		}
		
		this.find_furthest_object = function( theObjectType, xIn, yIn, avoidObjId, layerIdToCheck)
		{
			if( typeof avoidObjId=="undefined")
			{
				var avoidObjId = IS_NULL;
			}
			//wraps the position into the spatial grid.
			var foundSpace = this.find_nearest_spatial_at(xIn, yIn);
			if( foundSpace >= this.COLLISION_AREA_SPACES.size() )
			{
				foundSpace = this.COLLISION_AREA_SPACES.size() - 1;
			}
			if( foundSpace < 0)
			{ 
				foundSpace = 0;
			}
			
			var spaceToZero = foundSpace;
			var spaceToEnd = this.COLLISION_AREA_SPACES.size() - foundSpace;
			var startLoopDirection = 0;
			if( foundSpace >= this.COLLISION_AREA_SPACES.size() /2 )
			{
				startLoopDirection = 1;
			}
			var fObjC;
			var iObj;
			var objMinDistance = IS_NULL;
			var foundObj = IS_NULL;
			var mostDisObj = IS_NULL;
			var objAvX, objAvY;
			var cObjDistace = IS_NULL;
			var currentSpaceToSearchId = this.COLLISION_AREA_SPACES.length - 1;
			
			if(theObjectType>=0 && theObjectType < GPE_MAX_OBJECT_TYPES )
			{
				while ( currentSpaceToSearchId >=0)
				{
					fObjC=  this.COLLISION_AREA_SPACES[currentSpaceToSearchId];
					if(fObjC!=-1)
					{
						fObjC = fObjC.heldObjectsArray[theObjectType];
						iObj = fObjC.length - 1;
						while( iObj >=0 )
						{
							foundObj = fObjC[iObj];
							if( foundObj!=-1)
							{
								if( foundObj.gpeIsBeingDestroyed==false && foundObj.gpeUniqGameObjectId !=avoidObjId)
								{
									objAvX = foundObj.collisionBox.xCenter;
									objAvY = foundObj.collisionBox.yMiddle;
									cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
									if( cObjDistace > objMinDistance )
									{
										mostDisObj = foundObj;
										objMinDistance = cObjDistace;
									}
								}
							}
							iObj--;
						}
					}
					currentSpaceToSearchId--;
				}
				
				fObjC = this.COLLISION_AREA_OUTSIDE_SCENE.heldObjectsArray[theObjectType];
				iObj = fObjC.length - 1;
				while( iObj >= 0)
				{
					foundObj = fObjC[iObj];
					if( foundObj!=-1)
					{
						if( foundObj.gpeIsBeingDestroyed==false && foundObj.gpeUniqGameObjectId !=avoidObjId)
						{
							objAvX = foundObj.collisionBox.xCenter;
							objAvY = foundObj.collisionBox.yMiddle;
							cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
							if( cObjDistace > objMinDistance )
							{
								mostDisObj = foundObj;
								objMinDistance = cObjDistace;
							}
						}
					}
					iObj--;
				}
				return  mostDisObj;
			}
			else
			{
				//loops thru every game object of any type( painfully slow, but yolo, right?)
				while ( currentSpaceToSearchId >=0)
				{
					fObjC=  this.COLLISION_AREA_SPACES[currentSpaceToSearchId];
					if(fObjC!=-1)
					{
						fObjC = fObjC.internalArray;
						iObj = fObjC.length - 1;
						while( iObj >= 0)
						{
							foundObj = fObjC[iObj];
							if( foundObj!=-1)
							{
								if( foundObj.gpeIsBeingDestroyed==false && foundObj.gpeUniqGameObjectId !=avoidObjId)
								{
									objAvX = foundObj.collisionBox.xCenter;
									objAvY = foundObj.collisionBox.yMiddle;
									cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
									if( cObjDistace > objMinDistance )
									{
										mostDisObj = foundObj;
										objMinDistance = cObjDistace;
									}
								}
							}
							iObj--;
						}
					}
					currentSpaceToSearchId--;
				}
				
				fObjC =  this.COLLISION_AREA_OUTSIDE_SCENE.internalArray;
				iObj = fObjC.length - 1;
				while( iObj >= 0)
				{
					foundObj = fObjC[iObj];
					if( foundObj!=-1)
					{
						if( foundObj.gpeIsBeingDestroyed==false && foundObj.gpeUniqGameObjectId !=avoidObjId)
						{
							objAvX = foundObj.collisionBox.xCenter;
							objAvY = foundObj.collisionBox.yMiddle;
							cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
							if( cObjDistace > objMinDistance )
							{
								mostDisObj = foundObj;
								objMinDistance = cObjDistace;
							}
						}
					}
					iObj--;
				}
				return  mostDisObj;
			}
			return IS_NULL;
		}
		
		//gets the oldest object of theObjectType
		this.find_last_object = function( theObjectType)
		{
			if(theObjectType>=0 &&theObjectType < GPE_MAX_OBJECT_TYPES)
			{
				
			}
			return IS_NULL;
		};

		this.find_nearest_object = function( theObjectType, xIn, yIn, avoidObjId, layerIdToCheck)
		{
			if( typeof avoidObjId=="undefined")
			{
				var avoidObjId = IS_NULL;
			}
			
			var fObjC;
			var iObj;
			var objMaxDistance = 9007199254740991;
			var foundObj = IS_NULL;
			var minDisObj = IS_NULL;
			var objAvX, objAvY;
			var cObjDistace = IS_NULL;
			var currentSpaceToSearchId = this.COLLISION_AREA_SPACES.length - 1;
			if(theObjectType>=0 && theObjectType < GPE_MAX_OBJECT_TYPES )
			{
				while ( currentSpaceToSearchId >=0)
				{
					fObjC=  this.COLLISION_AREA_SPACES[currentSpaceToSearchId];
					if(fObjC!=-1)
					{
						fObjC = fObjC.heldObjectsArray[theObjectType];
						iObj = fObjC.length - 1;
						while( iObj >=0 )
						{
							foundObj = fObjC[iObj];
							if( foundObj!=-1)
							{
								if( foundObj.gpeIsBeingDestroyed==false && foundObj.gpeUniqGameObjectId !=avoidObjId)
								{
									objAvX = foundObj.collisionBox.xCenter;
									objAvY = foundObj.collisionBox.yMiddle;
									cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
									if( cObjDistace < objMaxDistance )
									{
										minDisObj = foundObj;
										objMaxDistance = cObjDistace;
									}
								}
							}
							iObj--;
						}
					}
					currentSpaceToSearchId--;
				}
				
				fObjC = this.COLLISION_AREA_OUTSIDE_SCENE.heldObjectsArray[theObjectType];
				iObj = fObjC.length - 1;
				while( iObj >= 0)
				{
					foundObj = fObjC[iObj];
					if( foundObj!=-1)
					{
						if( foundObj.gpeIsBeingDestroyed==false && foundObj.gpeUniqGameObjectId !=avoidObjId)
						{
							objAvX = foundObj.collisionBox.xCenter;
							objAvY = foundObj.collisionBox.yMiddle;
							cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
							if( cObjDistace < objMaxDistance )
							{
								minDisObj = foundObj;
								objMaxDistance = cObjDistace;
							}
						}
					}
					iObj--;
				}
				return  minDisObj;
			}
			else
			{
				//loops thru every game object of any type( painfully slow, but yolo, right?)
				while ( currentSpaceToSearchId >=0)
				{
					fObjC=  this.COLLISION_AREA_SPACES[currentSpaceToSearchId];
					if(fObjC!=-1)
					{
						fObjC = fObjC.internalArray;
						iObj = fObjC.length - 1;
						while( iObj >= 0)
						{
							foundObj = fObjC[iObj];
							if( foundObj!=-1)
							{
								if( foundObj.gpeIsBeingDestroyed==false && foundObj.gpeUniqGameObjectId !=avoidObjId)
								{
									objAvX = foundObj.collisionBox.xCenter;
									objAvY = foundObj.collisionBox.yMiddle;
									cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
									if( cObjDistace < objMaxDistance )
									{
										minDisObj = foundObj;
										objMaxDistance = cObjDistace;
									}
								}
							}
							iObj--;
						}
					}
					currentSpaceToSearchId--;
				}
				
				fObjC =  this.COLLISION_AREA_OUTSIDE_SCENE.internalArray;
				iObj = fObjC.length - 1;
				while( iObj >= 0)
				{
					foundObj = fObjC[iObj];
					if( foundObj!=-1)
					{
						if( foundObj.gpeIsBeingDestroyed==false && foundObj.gpeUniqGameObjectId !=avoidObjId)
						{
							objAvX = foundObj.collisionBox.xCenter;
							objAvY = foundObj.collisionBox.yMiddle;
							cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
							if( cObjDistace < objMaxDistance )
							{
								minDisObj = foundObj;
								objMaxDistance = cObjDistace;
							}
						}
					}
					iObj--;
				}
				return  minDisObj;
			}
			return IS_NULL;
		};
		
		//gets the last object of theObjectType
		this.find_newest_object = function( theObjectType)
		{
			if(theObjectType>=0 && theObjectType < GPE_MAX_OBJECT_TYPES )
			{
				
			}
			return IS_NULL;
		};
				
		
		this.find_object_collision = function(  objOne,  xIn,  yIn,  otherObjectType)
		{
			
		}
		
		this.find_tile = function( layerToCheck, xPos, yPos )
		{
			return this.currentScene.find_tile( layerToCheck, xPos, yPos )
		}
		
		this.find_tile_in_box = function( layerToCheck, x1,  y1,  x2, y2 )
		{
			return this.currentScene.find_tile_in_box( layerToCheck, x1,  y1,  x2, y2 );
		}
		
		this.find_matching_tile = function( layerToCheck,  xPos,  yPos,  tileTypeToSearchFor )
		{
			return this.currentScene.find_matching_tile( layerToCheck,  xPos,  yPos,  tileTypeToSearchFor );
		}
		
		this.find_matching_tile_in_box = function( layerToCheck,  x1,  y1,  x2, y2,  tileTypeToSearchFor )
		{
			return this.currentScene.find_tile_in_box( layerToCheck,  x1,  y1,  x2, y2,  tileTypeToSearchFor );
		}
		
		this.game_reset = function()
		{
		
		}
		
		this.get_camera_info = function(cameraId)
		{
			if( typeof cameraId !="number")
			{
				var cameraId = 0;
			}
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return {
				x:  cCam.cameraRect.get_x(),  y: cCam.cameraRect.get_x(), w: cCam.cameraRect.get_width(), h: cCam.cameraRect.get_height(),
				sx: cCam.renderRect.get_x(), sy: cCam.renderRect.get_x(), sw: cCam.renderRect.get_width(), sh: cCam.renderRect.get_height(),
				};
			}
			else
			{
				return IS_NULL;
			}
		}
		
		this.get_camera_height = function(cameraId)
		{
			if( typeof cameraId !="number")
			{
				var cameraId = 0;
			}
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras )
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.cameraRect.get_height();
			}
			return IS_NULL;
		}
		
		this.get_camera_width = function(cameraId)
		{
			if( typeof cameraId !="number")
			{
				var cameraId = 0;
			}
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras )
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.cameraRect.get_width();
			}
			return IS_NULL;
		}
		
		this.get_camera_x = function(cameraId)
		{
			if( typeof cameraId !="number")
			{
				var cameraId = 0;
			}
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras )
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.cameraRect.get_x();
			}
			return IS_NULL;
		}
		
		this.get_camera_y = function(cameraId)
		{
			if( typeof cameraId !="number")
			{
				var cameraId = 0;
			}
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras )
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.cameraRect.get_y();
			}
			return IS_NULL;
		}
		
		this.get_camera_mouse_x = function(cameraId)
		{
			if( cameraId >=0 && cameraId < gpe.maxCameras )
			{
				return this.MOUSE_CAM_X[cameraId];
			}
			else
			{
				return this.MOUSE_CAM_X[0];
			}
			return 0;
		}
		
		this.get_camera_mouse_y = function(cameraId)
		{
			if( cameraId >=0 && cameraId < gpe.maxCameras )
			{
				return this.MOUSE_CAM_Y[cameraId];
			}
			else
			{
				return this.MOUSE_CAM_Y[0];
			}
			return 0;
		}
		
		//
		this.get_camera_screen_height = function(cameraId)
		{
			if( typeof cameraId !="number")
			{
				var cameraId = 0;
			}
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras )
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.renderRect.get_height();
			}
			return IS_NULL;
		}
		
		this.get_camera_screen_width = function(cameraId)
		{
			if( typeof cameraId !="number")
			{
				var cameraId = 0;
			}
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras )
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.renderRect.get_width();
			}
			return IS_NULL;
		}
		
		this.get_camera_screen_x = function(cameraId)
		{
			if( typeof cameraId !="number")
			{
				var cameraId = 0;
			}
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras )
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.renderRect.get_x();
			}
			return IS_NULL;
		};
		
		this.get_camera_screen_y = function(cameraId)
		{
			if( typeof cameraId !="number")
			{
				var cameraId = 0;
			}
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras )
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.renderRect.get_y();
			}
			return IS_NULL;
		};
		
		
		//Font related functions/objects/variables as of Version 1.12 [ BEGIN ] 
		//Font GETTERS/SETTERS
		this.get_font_family = function( fontIn )
		{
			if(typeof fontIdIn!="undefined"&& textToRender.length > 0)
			{
				if( fontIdIn>=0 && fontIdIn < gpe.rsm.rFonts.length )
				{
					var fFont = gpe.rsm.rFonts[fontIdIn];
					if( fFont!=-1)
					{
						return fFont.get_family();
					}
				}
			}
			return "";
		};
		
		this.get_font_width = function( fontIn, words )
		{
			if(typeof fontIdIn!="undefined"&& textToRender.length > 0)
			{
				if( fontIdIn>=0 && fontIdIn < gpe.rsm.rFonts.length )
				{
					var fFont = gpe.rsm.rFonts[fontIdIn];
					if( fFont!=-1)
					{
						return fFont.get_text_width( words );
					}
				}
			}
			return 0;
		};
		
		this.get_font_height = function( fontIn, words )
		{
			if(typeof fontIdIn!="undefined"&& textToRender.length > 0)
			{
				if( fontIdIn>=0 && fontIdIn < gpe.rsm.rFonts.length )
				{
					var fFont = gpe.rsm.rFonts[fontIdIn];
					if( fFont!=-1)
					{
						return fFont.get_text_height( words );
					}
				}
			}
			return 0;
		};
			
		this.get_font_size = function( fontIn )
		{
			if(typeof fontIdIn!="undefined"&& textToRender.length > 0)
			{
				if( fontIdIn>=0 && fontIdIn < gpe.rsm.rFonts.length )
				{
					var fFont = gpe.rsm.rFonts[fontIdIn];
					if( fFont!=-1)
					{
						return fFont.fontSize;
					}
				}
			}
			return -1;
		};
		
		this.font_is_monospaced = function( fontIn )
		{
			if(typeof fontIdIn!="undefined"&& textToRender.length > 0)
			{
				if( fontIdIn>=0 && fontIdIn < gpe.rsm.rFonts.length )
				{
					var fFont = gpe.rsm.rFonts[fontIdIn];
					if( fFont!=-1)
					{
						return fFont.isMonoSpacedFont;
					}
				}
			}
			return false;
		};
		
		//Font functions/objects/variables as of Version 1.12 [ END ] 
		
		//returns the number of objects of the object type
		this.get_object_count = function( theObjectType)
		{
			//finds the object array
			if(theObjectType>=0 && theObjectType < this.GAME_OBJECTS.length )
			{
				return this.GAME_OBJECTS[theObjectType].length;
			}
			return 0;
		};
		
		//returns the number of objects of the object's FAMILY
		this.get_object_count_all = function( theObjectType)
		{
			//finds the object array
			if(theObjectType>=0 && theObjectType < GPE_MAX_OBJECT_TYPES )
			{
				if( GPE.GAME_OBJECTS_FAMILIES[theObjectType].length ==0)
				{
					return this.GAME_OBJECTS[theObjectType].length;
				}
				else
				{
					var familyCountTotal = 0;
					for( var iFamily = this.GAME_OBJECTS[theObjectType].length-1; iFamily >=0; iFamily--)
					{
						familyCountTotal+=gpe.GAME_OBJECTS[ gpe.GAME_OBJECTS_FAMILIES[iFamily] ].length;
					}
					return familyCountTotal;
				}
			}
			return 0;
		};
		
		//returns the number of objects of the object type
		this.get_object_in_view_count = function( theObjectType)
		{
			//finds the object array
			if(theObjectType>=0 && theObjectType < GPE_MAX_OBJECT_TYPES )
			{
				for( var i = this.collisionSpacesInView.length-1; i>=0; i--)
				{
					
				}
			}
			return 0;
		};
		
		//checks if the position is open and free of ANYTHING  or of the otherObjectType
		this.position_open = function(  xIn, yIn, otherObjectType, layerIdToCheck )
		{
			//makes sure our object isn't null before checking
			if( typeof layerIdToCheck!="number")
			{
				layerIdToCheck = 1; //default object layer...
			}
			
			if(objOne!=-1 && layerIdToCheck >=0 && layerIdToCheck < 32 )
			{
				var matchingLayer = IS_NULL;
				var foundLayerMatch = false;
				for( var iCLayers = GPR.currentScene.sceneLayers.length-1; iCLayers >=0; iCLayers--)
				{
					matchingLayer = GPR.currentScene.sceneLayers[iCLayers];
					if( matchingLayer.layerId == layerIdToCheck)
					{
						foundLayerMatch = true;
						break;
					}
				
				}
				if( foundLayerMatch )
				{
					
					var positionSpaceId = matchingLayer.find_spatial_at( xIn, yIn);
					var positionSpace = IS_NULL;
					if( positionSpaceId < 0 || positionSpace >= matchingLayer.COLLISION_AREA_SPACES.length)
					{
						positionSpace = matchingLayer.COLLISION_AREA_OUTSIDE_SCENE;
					}
					else
					{
						positionSpace = matchingLayer.COLLISION_AREA_SPACES.at(positionSpaceId);
					}
					var iItr = IS_NULL;
					var fObject = IS_NULL;
					if(otherObjectType > 0 && otherObjectType < GPE_MAX_OBJECT_TYPES)
					{
						var cObjController = positionSpace.heldObjectsArray[otherObjectType];
						for( iItr = cObjController.length-1; iItr>=0; iItr--)
						{
							fObject = cObjController[iItr];
							if( fObject!=-1)
							{
								if( gpe.point_within(xIn, yIn, fObject.collisionBox.xPos, fObject.collisionBox.yPos, fObject.collisionBox.x2Pos, fObject.collisionBox.y2Pos) )
								{
									return fObject;
								}
							}
						}
					}
					else
					{
						for( iItr = positionSpace.internalArray.length-1; iItr>=0; iItr--)
						{
							fObject = positionSpace.internalArray[iItr];
							if( gpe.point_within(xIn, yIn, fObject.collisionBox.xPos, fObject.collisionBox.yPos, fObject.collisionBox.x2Pos, fObject.collisionBox.y2Pos) )
							{
								return fObject;
							}
						}
					}
				}
			}
			return IS_NULL;
		};
		
		this.find_nearest_spatial_at = function( xIn, yIn)
		{
			var sPar = -1;
			if( xIn  < 0)
			{
				xIn = 0;
			}
			if( yIn < 0)
			{
				yIn = 0;
			}
			if( xIn >= this.currentScene.sceneWidth)
			{
				xIn = this.currentScene.sceneWidth-1;
			}
			if( yIn >= this.currentScene.sceneHeight)
			{
				yIn = this.currentScene.sceneHeight-1;
			}
			sPar =  (( xIn / this.spatialGridWidthSize)|0 )*this.spatialGridHeightAmount;
			sPar+=( yIn / this.spatialGridHeightSize)|0;
			if( sPar < 0)
			{
				sPar = 0;
			}
			if( sPar >= this.COLLISION_AREA_SPACES.length)
			{
				sPar = this.COLLISION_AREA_SPACES.length-1;
			}
			return sPar;
		}
	
		this.find_spatial_at = function( xIn, yIn)
		{
			var sPar = -1;
			if( xIn >=0 && yIn >=0 && xIn < this.currentScene.sceneWidth && yIn < this.currentScene.sceneHeight)
			{
				sPar =  (( xIn / this.spatialGridWidthSize)|0 )*this.spatialGridHeightAmount;
				sPar+=( yIn / this.spatialGridHeightSize)|0;
				if( sPar < 0 || sPar >= this.COLLISION_AREA_SPACES.length )
				{
					sPar = -1;
				}
			}
			return sPar;
		};
		
		this.render_state = function()
		{
			//gpe.context.fillStyle = '#333';
			//gpe.context.fillRect(0, 0, gpe.canvas.width, gpe.canvas.height);
			if( this.currentScene!=-1 && this.sceneWasReset==false  && this.currentScene.sceneHasBeenActivated)
			{
				gpe.context.font = 'bold 16pt Calibri';
				gpe.context.textAlign = 'left';
				gpe.context.fillStyle = '#FFF';
				gpe.GPE_Current_Context = gpe.preGuiContext;
				gpe.preGuiContext.clearRect(0, 0, gpe.preGuiCanvas.width, gpe.preGuiCanvas.height);
				
				if( this.currentScene.scene_is_ready())
				{
					if( this.currentScene.sceneIsProcessed && this.currentScene.sceneResourcesAreProcessed )
					{
						this.currentScene.render_scene(gpe.GPE_Current_Context,this);
						gpe.context.drawImage(gpe.preGuiCanvas,0,0,gpe.preGuiCanvas.width,gpe.preGuiCanvas.height,0,0,gpe.canvas.width,gpe.canvas.height );
					}
				}
				else
				{
					gpe.context.fillStyle = '#FFF';
					gpe.context.fillText("Loading...", 40, gpe.canvas.height-64);
				}
				gpe.GPE_Current_Context = gpe.context;
				if( GPE_SETTINGS_IS_DEBUGGING )
				{
				
					gpe.GPE_Current_Context.font = 'bold 16pt Calibri';
					gpe.GPE_Current_Context.textAlign = 'left';
					gpe.GPE_Current_Context.textAlign = 'left';
					gpe.GPE_Current_Context.fillStyle = 'red';
					var visSpaces = "";
					for( var iiiVS = 0; iiiVS < this.collisionSpacesInView.length;  iiiVS++)
					{
						visSpaces = visSpaces+this.collisionSpacesInView[iiiVS]+", ";
					}
					gpe.GPE_Current_Context.fillText(visSpaces, 32,32);
					var cSceneViewFound;
					for(var iV=0; iV < gpe.maxCameras; iV++)
					{
						cSceneViewFound =this.currentScene.sceneCamera[iV];
						if( cSceneViewFound.isVisible)
						{
							gpe.GPE_Current_Context.fillText("["+iV+"] ("+cSceneViewFound.cameraRect.xPos+","+cSceneViewFound.cameraRect.yPos+") , ("+cSceneViewFound.cameraRect.x2Pos+","+cSceneViewFound.cameraRect.y2Pos+").", 32,64+32*iV);
						}
					}
				}
			}
			else
			{
				gpe.context.fillStyle = this.currentScene.bgColor;
				gpe.context.fillRect(0, 0, gpe.canvas.width, gpe.canvas.height);
				//gpe.context.fillStyle = '#FFF';
				//gpe.context.fillText("Loading Scene...", 40, gpe.canvas.height-64);
			}
		};

		//Used to separate scene into zones for collision checks
		this.begin_spatialpartioning = function( cSceneWidth, cSceneHeight)
		{
			this.sceneWidth = cSceneWidth;
			this.sceneHeight = cSceneHeight;
			this.spatialGridWidthAmount = Math.ceil(cSceneWidth/this.spatialGridWidthSize);
			if( this.spatialGridWidthAmount<2)
			{
				this.spatialGridWidthSize = cSceneWidth/2;
				this.spatialGridWidthAmount = 2; 
			}
			this.spatialGridHeightAmount = Math.ceil(cSceneHeight/this.spatialGridHeightSize);
			if( this.spatialGridHeightAmount<2)
			{
				this.spatialGridHeightSize = cSceneHeight/2;
				this.spatialGridHeightAmount = 2;
			}
			for( var i = this.currentScene.sceneLayers.length-1; i >=0; i--)
			{
				this.currentScene.sceneLayers[i].begin_spatialpartioning( cSceneWidth, cSceneHeight);
			}
		}
		
		//creates all of the collisions
		this.prepare_game_frame = function()
		{
			var iHolderType = 0;
			var gameObjHolder = 0;
			this.parsing_objects_begun = true;
			//rest collision tree
			this.collisionsCheckedCount = 0;
			this.colisionObjectLoops = 0;
			this.objectCollisionHashesChecked = 0;
			this.spatialCollisionChecks = 0;
			this.objectCollisionIsChecked = {};
			this.collisionPairA = [];
			this.collisionPairB = [];
			this.collisionPairsTotal = 0;	
			//if( this.currentScene.sceneIsProcessed && this.currentScene.sceneResourcesAreProcessed )
			{
				//if( this.currentScene.sceneHasBeenActivated==true && this.currentScene.sceneIsSpatiallyPartitioned == false)
				{
					var foundGameObject = IS_NULL;
					var foundOtherGameObject = IS_NULL;
					var foundGameObjectHolder = IS_NULL;
					var foundOtherGameObjectHolder = IS_NULL;
					var iObjectHolder = IS_NULL;
					var jObjectHolder = IS_NULL;
					var OBJECT_LIST = IS_NULL;
					var iObject = IS_NULL;
					var jObject = IS_NULL;
					var objectTypeCollidedBefore = false;
					var otherObjectType;
					var jObjectStart;
					
					var objPartition = [-1, -1, -1, -1];
					var sgX1;
					var sgY1;
					var sgX2;
					var sgY2;
					
					var iCtrPartition = 0;
					var foundPartitionSpace = IS_NULL;
					this.parsed_objects = 0;
					//Implement soonISH
												
					var jLoop;
					var iView  = -1;
					var maxCameras = gpe.maxCameras-1;
					
					var currentSpace;
					var jOBjectParse;
					var tempSceneLayer = IS_NULL;
					var spacesArray = this.collisionSpacesInView;
					for(var iLayer = this.currentScene.sceneLayers.length-1; iLayer >= 0; iLayer--)
					{
						tempSceneLayer = this.currentScene.sceneLayers[iLayer];
						/*
						for(var iSpace = spacesArray.length-1; iSpace >= 0; iSpace--)
						{
							currentSpace = tempSceneLayer.COLLISION_AREA_SPACES.at( spacesArray[iSpace] );
							for( jLoop = currentSpace.internalArray.length-1;jLoop >= 0; jLoop--)
							foundGameObject = currentSpace.internalArray[jLoop];
						}
						*/
						for( jLoop = tempSceneLayer.layerObjects.length-1; jLoop >= 0; jLoop--)
						{
							foundGameObject = tempSceneLayer.layerObjects[jLoop];
							foundGameObject.collisionBeingChecked = true;
							foundGameObject.isOutsideScene = false;
							foundGameObject.gpeLogicAppliedInFrame = false;
							foundGameObject.gpePrelogicAppliedInFrame = false;
							for( iView = maxCameras; iView>=0; iView--)
							{
								foundGameObject.prerenderedInFrame[iView] = false;
								foundGameObject.renderedInFrame[iView] = false;
							}
							foundGameObject.gpeAddedToRenderList = false;
							foundGameObject.gpeAddedToContinuousList = false;
						}
						
						/*for( jLoop = tempSceneLayer.COLLISION_AREA_OUTSIDE_SCENE.internalArray.length-1;jLoop >= 0; jLoop--)
						{
							foundGameObject = tempSceneLayer.COLLISION_AREA_OUTSIDE_SCENE.internalArray[jLoop];
							foundGameObject.collisionBeingChecked = true;
							foundGameObject.isOutsideScene = false;
							foundGameObject.gpeLogicAppliedInFrame = false;
							foundGameObject.gpePrelogicAppliedInFrame = false;
							for( iView = maxCameras; iView>=0; iView--)
							{
								foundGameObject.prerenderedInFrame[iView] = false;
								foundGameObject.renderedInFrame[iView] = false;
							}
							foundGameObject.gpeAddedToRenderList = false;
							foundGameObject.gpeAddedToContinuousList = false;
						}*/
					}
					this.superDebugCollisions = false;
					this.currentScene.sceneIsSpatiallyPartitioned = true;
				}
			}
		}

		
		//GPR Scene Functions [ BEGIN ]
		this.get_scene_width = function( sceneIdIn )
		{
			return this.currentScene.get_scene_width();
		}
		
		this.get_scene_height = function( sceneIdIn )
		{
			return this.currentScene.get_scene_height();
		}
		
		this.get_past_scene = function()
		{
			return pastSceneId;
		}
		
		this.get_past_scene_name = function()
		{
			return pastSceneName;
		}
		
		this.get_scene_id = function()
		{
			return this.currentScene.sceneId;
		};
		
		this.get_scene_name = function()
		{
			return this.currentScene.sceneName;
		};

		this.process_scene_movement = function()
		{
			var nextSceneToGo = this.sceneToEnter;
			if( this.currentSceneId!=nextSceneToGo && nextSceneToGo>=0 && nextSceneToGo < gpe.gpeScenes.length)
			{
				if( GPE_SETTINGS_IS_DEBUGGING )
				console.log("Attempting to visit scene ID #"+nextSceneToGo+".");
				this.pastSceneId = this.currentScene;
				this.pastSceneName = this.currentScene.sceneName;
				var ii, jj;
				var iHolderType;
				var foundObjTypeHolder;
				var foundRegObj;
				if( this.currentScene.sceneIsContinuous ==false)
				{
					this.currentScene.reset_scene_meta();
				}
				this.GAME_OBJECTS_CONTINUOUS = [];
				this.currentScene.persistentObjects = [];
				
				
				for( ii=this.GAME_OBJECTS.length-1; ii>=0; ii--)
				{
					foundObjTypeHolder = this.GAME_OBJECTS[ii];
					for( jj=foundObjTypeHolder.length-1; jj>=0; jj--)
					{
						foundRegObj = foundObjTypeHolder[jj];
						if( foundRegObj.gpeIsBeingDestroyed==false)
						{
							foundRegObj.scene_end();
							if( foundRegObj.gpeIsContinuousObject)
							{
								this.add_to_continuous_objects(foundRegObj);
							}
							else if( this.currentScene.sceneIsContinuous)
							{
								this.currentScene.add_to_persistent_objects(foundRegObj);
							}
						}
					}
				}
				
				this.delete_selected_objects();
				this.scene_restart_object_info();
				
				if( this.currentScene.sceneIsContinuous)
				{
					this.currentScene.sceneWasVisitedBefore = true;
				}
				else
				{
					//Restart Object and scene related things
					this.currentScene.reset_scene_meta();
				}
				this.currentScene.clear_layers_data();
				if( this.currentScene!=-1)
				{
					gpe.stop_audio(this.currentScene.startAudio);
					gpe.stop_audio(this.currentScene.bgMusic);
					gpe.play_audio(this.currentScene.endAudio,false);
				}
				
				this.currentScene.sceneHasBeenActivated = false;
				
				//now goes to "next scene"
				this.currentScene = gpe.gpeScenes[ nextSceneToGo ];
				this.sceneToEnter = nextSceneToGo;
				this.currentSceneId = nextSceneToGo;
				if( GPE_SETTINGS_IS_DEBUGGING )
				console.log( this.currentScene );
				this.currentScene.update_tilemap();
				//if( !this.currentScene.sceneIsContinuous )
				{
					this.currentScene.sceneHasBeenActivated = false;
				}
				gpe.loop_audio(this.currentScene.bgMusic);
				gpe.play_audio(this.currentScene.startAudio,false);
			}
			else if( this.sceneWasReset )
			{
				var ii;
				var jj;
				var iHolderType;
				var foundObjTypeHolder;
				var foundRegObj;
				
				this.GAME_OBJECTS_CONTINUOUS = [];
				this.currentScene.persistentObjects = [];
				
				if( this.currentScene!=-1)
				{
					gpe.stop_audio(this.currentScene.startAudio);
					gpe.stop_audio(this.currentScene.bgMusic);
					gpe.play_audio(this.currentScene.endAudio,false);
				}
				this.currentScene.reset_scene_meta();
				this.currentScene.clear_layers_data();
				
				if( this.sceneWasHardReset )
				{
					for( ii=this.GAME_OBJECTS.length-1; ii>=0; ii--)
					{
						foundObjTypeHolder = this.GAME_OBJECTS[ii];
						for( jj=foundObjTypeHolder.length-1; jj>=0; jj--)
						{
							foundRegObj = foundObjTypeHolder[jj];
							if( foundRegObj.gpeIsBeingDestroyed==false)
							{
								foundRegObj.scene_end();
							}
						}
					}
				}
				else
				{
					for( ii=this.GAME_OBJECTS.length-1; ii>=0; ii--)
					{
						foundObjTypeHolder = this.GAME_OBJECTS[ii];
						for( jj=foundObjTypeHolder.length-1; jj>=0; jj--)
						{
							foundRegObj = foundObjTypeHolder[jj];
							if( foundRegObj.gpeIsBeingDestroyed==false)
							{
								foundRegObj.scene_end();
								if( foundRegObj.gpeIsContinuousObject && foundRegObj.gpeSceneStartId!=this.currentSceneId )
								{
									this.add_to_continuous_objects(foundRegObj);
								}
							}
						}
					}
				}
				
				this.delete_selected_objects();
				this.scene_restart_object_info();
				
				this.currentScene.reset_scene_meta();
				
				this.currentScene.sceneHasBeenActivated = false;
				if( this.currentScene.sceneIsContinuous)
				{
					this.currentScene.sceneWasVisitedBefore = true;
				}
				//now goes to "next scene"
				this.currentScene.update_tilemap();
				gpe.loop_audio(this.currentScene.bgMusic);
				gpe.play_audio(this.currentScene.startAudio,false);
				
				this.sceneWasReset = false;
				this.sceneWasHardReset = false;
			}
		};
		
		this.scene_restart_object_info = function()
		{
			this.GAME_OBJECTS_TO_BE_DESTROYED = [];


			this.collisionSpacesInView = [];
			this.spatialGridIsCreated = false;
			this.spatialMapWidthSize = 0;
			this.spatialMapHeightSize = 0;
			this.objectCollisionIsChecked = {};

			this.GAME_OBJECTS = [];
			this.COLLISION_CHECK_ARRAY = new gpe.SuperVector();
			var newVector;

			for(var i=0; i <GPE_MAX_OBJECT_TYPES; i+=1)
			{
				newVector = new gpe.SuperVector();
				for(var j=0; j <GPE_MAX_OBJECT_TYPES; j+=1)
				{
					newVector.push_back(false);
				}
				this.COLLISION_CHECK_ARRAY.push_back(newVector);
				
				this.GAME_OBJECTS.push_back([]);
			}
			this.COLLISION_AREA_SPACES = []; //Used for spatial partitioning collision checker
			this.COLLISION_AREA_OUTSIDE_SCENE = new gpe.SuperSpatialPartition();
			this.superDebugCollisions = false;
			this.collisionPairA = [];
			this.collisionPairB = [];
			this.collisionPairsTotal = 0;


			this.parsed_objects = 0;

			this.set_default_grid_size();
			
			this.spatialGridWidthAmount = 2;
			this.spaitalGridHeightAmount = 2;

			this.objectCollisionHashesChecked = 0;
			this.spatialCollisionChecks = 0;
			this.spatialGridIsCreated = false;
			this.spatialMapWidthSize = -1;
			this.spatialMapHeightSize = -1;
		};
		
		this.scene_enter = function( nextSceneId )
		{
			if( this.sceneToEnter==this.currentSceneId)
			{
				if( nextSceneId>=0 && nextSceneId < gpe.gpeScenes.length)
				{
					this.sceneToEnter = nextSceneId;
					return true;
				}
			}
			return false;
		};
		
		this.scene_enter_previous = function()
		{
			if( this.sceneToEnter==this.currentSceneId)
			{
				if( this.currentSceneId > 0)
				{
					this.sceneToEnter = this.currentSceneId - 1;
					return true;
				}
			}
		};
		
		this.scene_enter_next = function()
		{
			if( this.sceneToEnter==this.currentSceneId)
			{
				if( this.currentSceneId < gpe.gpeScenes.length-1)
				{
					this.sceneToEnter = this.currentSceneId + 1;
					return true;
				}
			}
			return false;
		};
		
		this.scene_reset = function( hardReset )
		{
			if( this.sceneWasReset==false )
			{
				if( hardReset )
				{
					this.sceneWasHardReset = true;
				}
				else
				{
					this.sceneWasHardReset = false;
				}
				this.sceneWasReset = true;
			}
		};
		
		//GPR Scene Functions [ END ]
		
		this.setup_camera = function(cameraId, cameraX,cameraY, cameraW, cameraH)
		{
			if( this.currentScene!=-1 && cameraId >=0 && cameraId < gpe.maxCameras )
			{
				this.currentScene.sceneCamera[cameraId].setup_camera( cameraX, cameraY, cameraW, cameraH );
			}
		};
		
		this.split_screen = function( screenTotal, verticallySplit, useBlanks )
		 { 
			 if( screenTotal >=1 && screenTotal <=8 )
			 {
				 console.log("Setting up "+screenTotal+" cameras...");
				 var iCam = 0;
				 //enables cameras
				 for( iCam = 0; iCam < screenTotal; iCam++ )
				 {
					 this.enable_camera( iCam );
				 }
				 //disables extra cameras
				for(  iCam = screenTotal; iCam < gpe.maxCameras; iCam++ )
				{
					this.disable_camera( iCam );
				}
			 
				if( typeof verticallySplit=="boolean" )
				{
					verticallySplit = false;
				}
				
				if( typeof useBlanks=="boolean" )
				{
					useBlanks = false;
				}
				
				if( screenTotal == 1 )
				{
					this.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT );
				}
				else if( screenTotal == 2 )
				{
					if( verticallySplit )
					{
						this.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT );
						this.setup_camera(1,GPE_SETTINGS_SCREEN_WIDTH/2,0, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT );
					}
					else
					{
						this.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT/2 );
						this.setup_camera(1,0,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT/2 );
					}
				}
				else if( screenTotal == 3 )
				{
					if( verticallySplit )
					{
						this.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH/3,GPE_SETTINGS_SCREEN_HEIGHT );
						this.setup_camera(1,GPE_SETTINGS_SCREEN_WIDTH/3,0, GPE_SETTINGS_SCREEN_WIDTH/3,GPE_SETTINGS_SCREEN_HEIGHT );
						this.setup_camera(2,GPE_SETTINGS_SCREEN_WIDTH*2/3,0, GPE_SETTINGS_SCREEN_WIDTH/3,GPE_SETTINGS_SCREEN_HEIGHT );
					}
					else
					{
						if( useBlanks )
						{
							this.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );
							this.setup_camera(1,GPE_SETTINGS_SCREEN_WIDTH/2,0, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );
							this.setup_camera(2,0,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );
						}
						else
						{
							this.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT/2 );
							this.setup_camera(1,0,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT/2 );
							this.setup_camera(2,GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT/2 );							
						}
					}
				}
				else if( screenTotal == 4 )
				{
					//top screens
					this.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );
					this.setup_camera(1,GPE_SETTINGS_SCREEN_WIDTH/2,0, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );
					//bottom screens
					this.setup_camera(2,0,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );							
					this.setup_camera(3,GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT/2 );							
						
				}
				else if( screenTotal == 5 )
				{
					
				}
				else if( screenTotal == 6 )
				{
					//top screens
					this.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );
					this.setup_camera(1,GPE_SETTINGS_SCREEN_WIDTH/2,0, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );
					this.setup_camera(2,GPE_SETTINGS_SCREEN_WIDTH/2,0, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );
					//bottom screens
					this.setup_camera(3,0,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2 );							
					this.setup_camera(4,GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT/2 );							
					this.setup_camera(5,GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT/2 );							
				
				}
				else if( screenTotal == 7 )
				{
					if( useBlanks )
					{
						
					}
				}
				else if( screenTotal == 8 )
				{
					//top screens
					this.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH/4,GPE_SETTINGS_SCREEN_HEIGHT/4 );
					this.setup_camera(1,GPE_SETTINGS_SCREEN_WIDTH/2,0, GPE_SETTINGS_SCREEN_WIDTH/4,GPE_SETTINGS_SCREEN_HEIGHT/4 );
					this.setup_camera(2,GPE_SETTINGS_SCREEN_WIDTH/2,0, GPE_SETTINGS_SCREEN_WIDTH/4,GPE_SETTINGS_SCREEN_HEIGHT/4 );
					this.setup_camera(3,GPE_SETTINGS_SCREEN_WIDTH/2,0, GPE_SETTINGS_SCREEN_WIDTH/4,GPE_SETTINGS_SCREEN_HEIGHT/4 );
					//bottom screens
					this.setup_camera(4,0,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH/4,GPE_SETTINGS_SCREEN_HEIGHT/4 );							
					this.setup_camera(5,0,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH/4,GPE_SETTINGS_SCREEN_HEIGHT/4 );							
					this.setup_camera(6,0,GPE_SETTINGS_SCREEN_HEIGHT/2, GPE_SETTINGS_SCREEN_WIDTH/4,GPE_SETTINGS_SCREEN_HEIGHT/4 );							
					this.setup_camera(7,GPE_SETTINGS_SCREEN_WIDTH/2,GPE_SETTINGS_SCREEN_HEIGHT/4, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT/4 );							
				
				}
				return true;
			 }
			 gpe.enable_camera(0);
			 gpe.setup_camera(0,0,0, GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT );

			return 0;
		 };
		
		this.update_object_in_map = function(objectBeingUpdated)
		{
			if( typeof objectBeingUpdated!=-1 )
			{
				var matchingLayer = IS_NULL;
				var foundLayerMatch = false;
				var layerIdToCheck = objectBeingUpdated.sceneLayerPos;
				for( var iCLayers = GPR.currentScene.sceneLayers.length-1; iCLayers >=0; iCLayers--)
				{
					matchingLayer = GPR.currentScene.sceneLayers[iCLayers];
					if( matchingLayer.layerId == layerIdToCheck)
					{
						foundLayerMatch = true;
						break;
					}
				
				}
				if( foundLayerMatch )
				{
					var objPartition = [-1, -1, -1, -1];
					objPartition[0] = -1; //top-left corner
					objPartition[1] = -1; //top-right corner
					objPartition[2] = -1; //bottom-left corner
					objPartition[3] = -1; //bottom-right corner
					var iCtrPartition = 0;
					var jCtrPartition = 0;
					var foundPartitionSpaceId;
					var foundPartitionSpace;
					
					//places objects in up to 4 spatial grids.
					var sgX1 = objectBeingUpdated.collisionBox.xPos;
					var sgY1 = objectBeingUpdated.collisionBox.yPos;
					var sgX2 = objectBeingUpdated.collisionBox.x2Pos;
					var sgY2 = objectBeingUpdated.collisionBox.y2Pos;
					
					//console.log("Processing coords ("+sgX1+","+sgY1+") to ("+sgX2+","+sgY2+").");
					//removes object from previous grids
					
					//top left
					if( sgX1 < 0 || sgY1 < 0 || sgX1 > this.sceneWidth || sgY1 > this.sceneHeight)
					{
						objPartition[0] = -1;
					}
					else
					{
						objPartition[0] = ( (sgX1 / matchingLayer.spatialGridWidthSize)|0)*matchingLayer.spatialGridHeightAmount;
						objPartition[0]+= ( sgY1 / matchingLayer.spatialGridHeightSize)|0;
					}
					
					//top right
					if( sgX2 < 0 || sgY1 < 0 || sgX2 > this.sceneWidth || sgY1 > this.sceneHeight)
					{
						objPartition[1] = -1;
					}
					else
					{
						objPartition[1] = (( sgX2 / matchingLayer.spatialGridWidthSize)|0)*matchingLayer.spatialGridHeightAmount;
						objPartition[1]+= ( sgY1 / matchingLayer.spatialGridHeightSize)|0;
					}
					
					//bottom left
					if( sgX1 < 0 || sgY2 < 0 || sgX1 > this.sceneWidth || sgY2 > this.sceneHeight)
					{
						objPartition[2] = -1;
					}
					else
					{
						objPartition[2] =( ( sgX1 / matchingLayer.spatialGridWidthSize)|0 )*matchingLayer.spatialGridHeightAmount;
						objPartition[2]+= ( sgY2 / matchingLayer.spatialGridHeightSize)|0;
					}
					
					//bottom right
					if( sgX2 < 0 || sgY2 < 0 || sgX2 > this.sceneWidth || sgY2 > this.sceneHeight)
					{
						objPartition[3] = -1;
					}
					else
					{
						objPartition[3] =  ( ( sgX2 / matchingLayer.spatialGridWidthSize)|0 )*matchingLayer.spatialGridHeightAmount;
						objPartition[3]+=( sgY2 / matchingLayer.spatialGridHeightSize)|0;
						//console.log( matchingLayer.spatialGridHeightAmount );
					}
					
					
					objPartition = gpe.make_array_unique(objPartition);
					var idInBoth = false;
					if( objectBeingUpdated.GPE_AddedToGridSystem==true)
					{
						//Removes from grids the object jumped out of...
						for( iCtrPartition = objectBeingUpdated.SPATIAL_PARTITION_LIST.length-1; iCtrPartition>=0; iCtrPartition--)
						{
							foundPartitionSpaceId = objectBeingUpdated.SPATIAL_PARTITION_LIST[iCtrPartition];
							idInBoth = false;
							for( jCtrPartition = objPartition.length-1; jCtrPartition>=0; jCtrPartition--)
							{
								if( foundPartitionSpaceId== objPartition[jCtrPartition] )
								{
									idInBoth = true;
									break;
								}
							}
							
							if( !idInBoth )
							{
								if( foundPartitionSpaceId < 0 || foundPartitionSpaceId >= matchingLayer.COLLISION_AREA_SPACES.length  )
								{
									matchingLayer.COLLISION_AREA_OUTSIDE_SCENE.remove_held_object(objectBeingUpdated);
								}
								else
								{
									foundPartitionSpace = matchingLayer.COLLISION_AREA_SPACES[foundPartitionSpaceId];
									foundPartitionSpace.remove_held_object(objectBeingUpdated);
								}
							}
						}
						
					
						//Adds to grids the object jumped into...
						for( iCtrPartition = objPartition.length-1; iCtrPartition>=0; iCtrPartition--)
						{
							foundPartitionSpaceId = objPartition[iCtrPartition];
							idInBoth = false;
							for( jCtrPartition = objectBeingUpdated.SPATIAL_PARTITION_LIST.length-1; jCtrPartition>=0; jCtrPartition--)
							{
								if( foundPartitionSpaceId == objectBeingUpdated.SPATIAL_PARTITION_LIST[jCtrPartition] )
								{
									idInBoth = true;
									break;
								}
							}
							
							if( !idInBoth )
							{
								if( foundPartitionSpaceId < 0 || foundPartitionSpaceId >= matchingLayer.COLLISION_AREA_SPACES.size()  )
								{
									if( objectBeingUpdated.isOutsideScene ==false)
									{
										matchingLayer.COLLISION_AREA_OUTSIDE_SCENE.add_object(objectBeingUpdated);
										objectBeingUpdated.isOutsideScene = true;
										//console.log("Adding object of type ["+objectBeingUpdated.gpeObjectType+"] to  ["+layerIdToCheck+"] layer's to ourofScene collision space...");
									}
								}
								else
								{
									//console.log("Box #"+iCtrPartition+" with value ["+foundPartitionSpaceId+"].");
									//console.log("Adding object of type ["+objectBeingUpdated.gpeObjectType+"] to  ["+layerIdToCheck+"] layer's [ "+foundPartitionSpaceId +"] collision space...");
									foundPartitionSpace = matchingLayer.COLLISION_AREA_SPACES.at( foundPartitionSpaceId );
									foundPartitionSpace.add_object(objectBeingUpdated);
									//
								}
							}
						}
					}
					else
					{
						for( iCtrPartition = objPartition.length-1; iCtrPartition>=0; iCtrPartition--)
						{
							foundPartitionSpaceId = objPartition[iCtrPartition];
							if( foundPartitionSpaceId < 0 || foundPartitionSpaceId >= matchingLayer.COLLISION_AREA_SPACES.size()  )
							{
								if( objectBeingUpdated.isOutsideScene ==false)
								{
									matchingLayer.COLLISION_AREA_OUTSIDE_SCENE.add_object(objectBeingUpdated);
									objectBeingUpdated.isOutsideScene = true;
									//console.log("Adding object of type ["+objectBeingUpdated.gpeObjectType+"] to  ["+layerIdToCheck+"] layer's to ourofScene collision space...");
								}
							}
							else
							{
								//console.log("Box #"+iCtrPartition+" with value ["+foundPartitionSpaceId+"].");
								//console.log("Adding object of type ["+objectBeingUpdated.gpeObjectType+"] to  ["+layerIdToCheck+"] layer's [ "+foundPartitionSpaceId +"] collision space...");
								foundPartitionSpace = matchingLayer.COLLISION_AREA_SPACES.at( foundPartitionSpaceId );
								foundPartitionSpace.add_object(objectBeingUpdated);
								//
							}
						}
				
				}
					objectBeingUpdated.SPATIAL_PARTITION_LIST = objPartition;
				}
				else
				{
					console.log("Unable to find matching layer in map for ["+layerIdToCheck+"] layer...");
				}
			}
		};
		
		//GPR New Backgroun functions added as of Version 1.12 [ BEGIN ] 
		this.replace_background = function( layerBeingChanged, needleBgId, newBgId )
		{
			this.currentScene.replace_background( layerBeingChanged, needleBgId, newBgId );
		};
		
		this.replace_all_backgrounds = function( needleBgId, newBgId )
		{
			this.currentScene.replace_all_backgrounds( needleBgId, newBgId );
		};
		
		this.set_background = function( layerBeingChanged, bgId, newBgId )
		{
			this.currentScene.replace_background( layerBeingChanged, bgId, newBgId );
		};
		//GPR New Backgroun functions added as of Version 1.12 [ END ] 

		this.GAME_OBJECTS[0] = [];
		this.COLLISION_CHECK_ARRAY = new gpe.SuperVector();
		var newVector;
		
		for(var i=0; i <this.GAME_OBJECTS.length; i+=1)
		{
			newVector = new gpe.SuperVector();
			for(var j=0; j <this.GAME_OBJECTS; j+=1)
			{
				newVector.push_back([]);
			}
			this.COLLISION_CHECK_ARRAY.push_back(newVector);
		}
	};
	
	function GPE_AudioResource()
	{
		this.html5AudioSource = new Audio();
		this.audioIsLoaded = false;
		this.audioCouldBePlayed = -1;
		this.audioId = -1;
		this.audioFileName = [];
		this.actualAudioFileName = "";
		this.audioName = "unnamed";
		this.audioGroupName = '';
		this.audioLoopsAround = false;
		this.defaultVolume = 100;
		this.audioWasStopped = false;
		this.audioWasPaused = false
		this.audioLoadedInMemory = false;
		this.isBeingLooped = false;
		for( var iRes = 0; iRes < GPE_SUPPORTED_AUDIO_FORMAT_COUNT; iRes++)
		{
			this.audioFileName[iRes] = "";
		}
		
		this.check_audio_state = function()
		{
			var audioShouldLoad = false;
			for( var ii = 0; ii < GPE_SUPPORTED_AUDIO_FORMAT_COUNT; ii++)
			{
				if(  GPE_AUDIO_PLAYABLE[ii] >=0 && this.audioFileName[ii].length > 3 )
				{
					audioShouldLoad = true;
				}
			}

			//this audio is doomed since the user didn't use all 4 data types
			if( this.audioShouldLoad == false)
			{
				this.audioIsLoaded = true;
			}
			
			if( this.audioIsLoaded!=true && this.html5AudioSource.readyState >=2 )
			{
				this.audioIsLoaded=true; 
				this.audioLoadedInMemory = true;
			}
			else if(this.html5AudioSource.error )
			{
				this.audioIsLoaded=true; 
				this.audioLoadedInMemory = false;
				console.log("Error loading "+this.audioName+".");
			}
			return this.audioIsLoaded;
		}
		
		this.check_is_playing = function()
		{
			if( this.html5AudioSource!=-1)
			{
				return !this.html5AudioSource.paused;
			}
			return false;
		}
		
		this.load_audio = function()
		{
			if( this.html5AudioSource==-1)
			{
				this.html5AudioSource = new Audio();
				this.html5AudioSource.volume = this.defaultVolume;
			}
			
			if( this.audioIsLoaded ==false)
			{
				this.actualAudioFileName = "";
				var audioShouldLoad = false;
				this.audioCouldBePlayed = -1;
				for( var ii = 0; ii < GPE_SUPPORTED_AUDIO_FORMAT_COUNT; ii++)
				{
					if(  GPE_AUDIO_PLAYABLE[ii] > this.audioCouldBePlayed && this.audioFileName[ii].length > 3 )
					{
						this.actualAudioFileName = this.audioFileName[ii]
						this.audioCouldBePlayed = GPE_AUDIO_PLAYABLE[ii];
						audioShouldLoad = true;
					}
				}
				
				//this audio is doomed since the user didn't use all 4 data types
				if( audioShouldLoad == false)
				{
					this.audioIsLoaded = true;
				}
				if( this.actualAudioFileName.length > 3 )
				{
					this.html5AudioSource.addEventListener("canplaythrough", function ()
					{
						this.audioIsLoaded = true;
						this.audioLoadedInMemory = true;
					});
					this.html5AudioSource.src = this.actualAudioFileName;
				}
			}
		}
		
		this.pause_audio = function()
		{
			if(gpe.lastLoopedTrack == this.audioId )
			{
				gpe.lastLoopedTrack = -1;
			}
			this.isBeingLooped = false;
			if( this.html5AudioSource!=-1 && this.audioLoadedInMemory==true )
			{
				this.html5AudioSource.oncanplaythrough = function () 
				{
					
				};
				this.html5AudioSource.pause();
				this.audioWasPaused = true
				this.audioWasStopped = false;
			}
		}
		
		this.play_audio = function(loops)
		{
			if(typeof loops=="undefined")
			{
				var loops = false
			}
			if( this.html5AudioSource!=-1)
			{
				if( this.audioIsLoaded==true && this.audioLoadedInMemory==true  )
				{
					if ( !this.html5AudioSource.paused )
					{
						//	this.html5AudioSource.pause();
					
						this.html5AudioSource.currentTime  = 0.5;
					}
					else if(this.html5AudioSource.ended)
					{
						this.html5AudioSource.currentTime  = 0;
					}
					this.html5AudioSource.play();
				}
				else if( this.html5AudioSource.readyState >=2)
				{
					this.audioIsLoaded=true; 
					this.html5AudioSource.play();
				}
				else
				{
					this.html5AudioSource.oncanplaythrough = function () 
					{
						this.play(loops);
					};
				}
				this.audioWasPaused = false;
				this.audioWasStopped = false;
				if( loops)
				{
					if( GPE_SETTINGS_IS_DEBUGGING )
					console.log("Playing Audio"+this.audioId+" that loops");
					gpe.lastLoopedTrack = this.audioId;
					this.isBeingLooped = true;
				}
				else
				{
					if( GPE_SETTINGS_IS_DEBUGGING )
					console.log("Playing Audio"+this.audioId+" that doesnt loop");
					this.isBeingLooped = false;
				}
			}
		}
		
		this.stop_audio = function()
		{
			if(gpe.lastLoopedTrack == this.audioId )
			{
				gpe.lastLoopedTrack = -1;
			}
			this.isBeingLooped = false;
			if( GPE_SETTINGS_IS_DEBUGGING )
			console.log("Stopping audio track: ["+this.audioId+"].");
			if( this.html5AudioSource!=-1  )
			{
				this.html5AudioSource.oncanplaythrough = function () 
				{
					
				};
				this.html5AudioSource.currentTime = 0;
				this.html5AudioSource.pause();
				this.audioWasPaused = false;
				this.audioWasStopped = true;
			}
		}
	}
	
	function GPE_FontResource()
	{
		this.fontId = -1;
		this.fontFileName = [];
		this.fontFamilyName = "";
		this.fontName = "unnamed";
		this.fontCanvasInfo = "";
		this.fontSize = 12;
		this.isMonoSpacedFont = false;
		for( var iRes = 0; iRes < GPE_SUPPORTED_FONT_FORMAT_COUNT; iRes++)
		{
			this.fontFileName[iRes] = "";
		}
		
		this.load_font = function()
		{
			var foundFontFamilyName = false;
			this.fontCanvasInfo = ""+this.fontSize+"pt ";
			if( this.fontFamilyName.length > 0)
			{
				this.fontCanvasInfo+=this.fontFamilyName;
				foundFontFamilyName = true;
			}
			else
			{
				for( var iRes = 0; iRes < GPE_SUPPORTED_FONT_FORMAT_COUNT; iRes++)
				{
					this.fontFileName[iRes]
					if( this.fontFileName[iRes].length > 0 && foundFontFamilyName==false)
					{
						this.fontCanvasInfo+=this.fontFileName[iRes];
						foundFontFamilyName = true;
						break;
					}
				}
				if( foundFontFamilyName==false)
				{
					this.fontCanvasInfo+="Arial";
				}
			}
		};
		
		
		this.get_text_width = function( wordsIn)
		{
			if( wordsIn.length > 0 )
			{
				if( isMonoSpacedFont )
				{
					return wordsIn.length * fontWidth;
				}
			}
			else
			{
				gpe.GPE_Current_Context.font = fFont.fontCanvasInfo;
				gpe.GPE_Current_Context.fillStyle = fontRenderColor;
				gpe.GPE_Current_Context.textBaseline="top";
				gpe.GPE_Current_Context.textAlign = 'left';
				metrics = gpe.GPE_Current_Context.measureText(wordsIn);
				return metrics.width;
			}
		};
		
		this.get_text_height = function( wordsIn)
		{
			if( wordsIn.length > 0 )
			{
				if( isMonoSpacedFont )
				{
					return wordsIn.length * fontHeight;
				}
			}
			else
			{
				//Currently uses the max-estimated height with M being the "tallest character".
				gpe.GPE_Current_Context.font = fFont.fontCanvasInfo;
				gpe.GPE_Current_Context.fillStyle = fontRenderColor;
				gpe.GPE_Current_Context.textBaseline="top";
				gpe.GPE_Current_Context.textAlign = 'left';
				metrics = gpe.GPE_Current_Context.measureText("M");
				return metrics.width * wordsIn.length;
			}
		};
		
		this.get_family = function( )
		{
			return this.fontFamilyName;
		};
		
		this.get_name = function( )
		{
			return this.fontName;
		};
		
		this.get_size = function( wordsIn)
		{
			return this.fontSize;
		};
		
		this.is_mono_spaced = function( )
		{
			return this.isMonoSpacedFont;
		};
	};
	
	function GPE_VideoResource()
	{
		this.html5VideoSource = document.createElement("video");
		this.html5VideoSource.controls = false;
		//this.html5VideoSource.style="visibility: hidden; display: none;";
		document.body.appendChild(this.html5VideoSource);
		this.videoIsLoaded = false;
		this.videoCouldBePlayed = -1;
		this.videoId = -1;
		this.videoFileName = [];
		this.actualVideoFileName = "";
		this.videoName = "unnamed";
		this.videoGroupName = '';
		this.videoLoopsAround = false;
		this.defaultVolume = 100;
		this.previousVolume = 100;
		this.videoWasStopped = false;
		this.videoWasPaused = false
		this.videoLoadedInMemory = false;
		for( var iRes = 0; iRes < GPE_SUPPORTED_VIDEO_FORMAT_COUNT; iRes++)
		{
			this.videoFileName[iRes] = "";
		}
		
		this.check_video_state = function()
		{
			var videoShouldLoad = false;
			for( var ii = 0; ii < GPE_SUPPORTED_VIDEO_FORMAT_COUNT; ii++)
			{
				if(  GPE_AUDIO_PLAYABLE[ii] >=0 && this.videoFileName[ii].length > 3 )
				{
					videoShouldLoad = true;
				}
			}

			//this video is doomed since the user didn't use all 4 data types
			if( videoShouldLoad == false)
			{
				this.videoIsLoaded = true;
			}
			
			if( this.videoIsLoaded!=true && this.html5VideoSource.readyState >=2 )
			{
				this.videoIsLoaded=true; 
				this.videoLoadedInMemory = true;
			}
			return this.videoIsLoaded;
		};
		
		this.check_is_playing = function()
		{
			if( this.html5VideoSource!=-1)
			{
				return !this.html5VideoSource.paused;
			}
			return false;
		};
		
		this.load_video = function()
		{
			if( this.html5VideoSource==-1)
			{
				this.html5VideoSource = new Video();
				this.html5VideoSource.volume = this.defaultVolume;
			}
			
			if( this.videoIsLoaded ==false)
			{
				this.actualVideoFileName = "";
				var videoShouldLoad = false;
				this.videoCouldBePlayed = -1;
				for( var ii = 0; ii < GPE_SUPPORTED_VIDEO_FORMAT_COUNT; ii++)
				{
					if(  GPE_VIDEO_PLAYABLE[ii] > this.videoCouldBePlayed && this.videoFileName[ii].length > 3 )
					{
						this.actualVideoFileName = this.videoFileName[ii]
						this.videoCouldBePlayed = GPE_VIDEO_PLAYABLE[ii];
						addSourceToVideo(this.html5VideoSource,this.videoFileName[ii],GPE_AUD_TYPE_NAME[ii]);
						videoShouldLoad = true;
					}
				}
				
				//this video is doomed since the user didn't use all 4 data types
				if( videoShouldLoad == false)
				{
					this.videoIsLoaded = true;
				}
				if( this.actualVideoFileName.length > 3 )
				{
					this.html5VideoSource.addEventListener("canplaythrough", function ()
					{
						this.videoIsLoaded = true;
						this.videoLoadedInMemory = true;
					});
					this.html5VideoSource.src = this.actualVideoFileName;
				}
			}
		};
		
		this.pause_video = function()
		{
			if( this.html5VideoSource!=-1 && this.videoLoadedInMemory==true )
			{
				this.html5VideoSource.pause();
				this.videoWasPaused = true
				this.videoWasStopped = false;
			}
			if(gpe.lastLoopedVideo == this.videoId )
			{
				gpe.lastLoopedVideo = -1;
			}
		};
		
		
		this.play_video = function(loops, videoSkippable)
		{
			this.load_video();
			if(typeof loops=="undefined")
			{
				var loops = false
			}
			if( typeof videoPlayingIsSkippable=="undefined")
			{
				var videoSkippable = true;
			}
			if( this.html5VideoSource!=-1)
			{
				gpe.currentVideoPlaying = this.videoId;
				gpe.isPlayingVideo = true;
				gpe.videoPlayingIsSkippable = videoSkippable;
				if( this.videoIsLoaded==true && this.videoLoadedInMemory==true  )
				{
					if ( !this.html5VideoSource.paused )
					{
						//	this.html5VideoSource.pause();
					
						this.html5VideoSource.currentTime  = 0.5;
					}
					else if(this.html5VideoSource.ended)
					{
						this.html5VideoSource.currentTime  = 0;
					}
					this.html5VideoSource.play();
				}
				else if( this.html5VideoSource.readyState >=2)
				{
					this.videoIsLoaded=true; 
					this.html5VideoSource.play();
				}
				else
				{
					this.html5VideoSource.oncanplay = function () 
					{
					
						this.play();
					};
				}
				this.videoWasPaused = false;
				this.videoWasStopped = false;
				if( loops)
				{
					if( GPE_SETTINGS_IS_DEBUGGING )
					console.log("Playing Video["+this.videoId+"[ that loops");
					gpe.lastLoopedVideo = this.videoId;
					this.html5VideoSource.addEventListener('ended', gpe.loop_last_video, false);
				}
				else
				{
					if( GPE_SETTINGS_IS_DEBUGGING )
					console.log("Playing Video["+this.videoId+"] that doesnt loop");
					this.html5VideoSource.addEventListener('ended', gpe.stop_last_video, false);
				}
			}
		};
		
		this.process_playing_video = function()
		{
			if( this.html5VideoSource!=-1)
			{
				if( this.videoWasStopped)
				{
					gpe.exit_video_view_mode();
					this.stop_video();
				}
				else if( this.html5VideoSource.ended ||  this.html5VideoSource.error )
				{
					gpe.exit_video_view_mode();
					this.stop_video();
				}
			}
			else
			{
				gpe.exit_video_view_mode();
			}
		}
		
		this.render_video = function( rx, ry, rw, rh)
		{
			if( this.html5VideoSource!=-1)
			{
				//console.log("Attempting to render frame...");
				//if( this.html5VideoSource.canplaythrough)
				{
					gpe.context.drawImage( this.html5VideoSource,0, 0, this.html5VideoSource.videoWidth,
                    this.html5VideoSource.videoHeight,rx,ry,rw, rh);
					//console.log("Rendering frame...");
				}
			}
		};
		
		this.stop_video = function()
		{
			if( this.html5VideoSource!=-1)
			{
				this.html5VideoSource.currentTime = 0;
				this.html5VideoSource.pause();
				this.videoWasPaused = false;
				this.videoWasStopped = true;
			}
			if(gpe.lastLoopedVideo == this.videoId )
			{
				gpe.lastLoopedVideo = -1;
			}
		};
		
		
	};
	
	//Controls and handles all of the game's resources( Sprites, Backgrounds and Audio ).
	function ResourceController()
	{
		this.randomVariableOn = IS_NULL; //random private variable for testing purposes
		this.rAllImages = new Array();
		this.rTextures = new Array(); //vector of every game texture loaded
		this.rTilesheets = new Array();
		this.rAudio = []; //vector of every game audio
		this.rSprites = new Array(); //vector of every game sprite
		this.rBackgrounds = []; //vector of every game background
		this.rFonts = []; //vector of every game font
		this.rVideos = []; //vector of every game video
		this.nameBackgrounds = [];
		this.SPRITE_COUNT = 0;
		this.RESOURCES_ADDED = false;
		this.texturesLoadedCount = 0;
		this.spritesLoadedCount = 0;
		this.spritePercentage = 50;
		this.percentLoaded = 9;
		this.latestLoadMessage = "";
		
		this.audioLoadedCount = 0;
		this.audioIsReadyForLoading = false;
		
		this.spritesIsAllLoaded = 0;
		this.texturesIsAlLoaded = 0;
		this.audioIsAllLoaded = 0;
			
		this.check_resources_state = function()
		{
			if( this.RESOURCES_ADDED !=-1)
			{
				var spriteToCheck;
				var iLoop = 0;
				
				var textureToCheck = IS_NULL;
				if (this.texturesLoadedCount != this.rTextures.length)
				{
					this.texturesLoadedCount = 0;
					for (iLoop = 0; iLoop < this.rTextures.length; iLoop++)
					{
						textureToCheck = this.rTextures[iLoop];
						if( textureToCheck!=-1  && typeof textureToCheck!='undefined')
						{
							if (textureToCheck.check_texture_state() )
							{
								this.texturesLoadedCount++;
							}
						}
						else
						{
							this.texturesLoadedCount++;
						}
					}
				}
				
				if (this.spritesLoadedCount != this.SPRITE_COUNT)
				{
					this.spritesLoadedCount = 0; 
					for (iLoop = 0; iLoop < imageCount; iLoop++)
					{
						var spriteToCheck = this.rSprites[iLoop];
						if( spriteToCheck!=-1 && typeof spriteToCheck!='undefined')
						{
							if( spriteToCheck.imageObj!=-1 && typeof spriteToCheck.imageObj!='undefined')
							{
								if( IsImageOk( spriteToCheck.imageObj ) )
								{
									spriteToCheck.imageObj.alt = "true";
									this.spritesLoadedCount++;
									this.percentLoaded+=(1/this.SPRITE_COUNT)*this.spritePercentage;
								}
								else if( spriteToCheck.imageObj.alt=="failed")
								{
									spriteToCheck.imageObj.alt = "failed";
									this.spritesLoadedCount++;
									this.percentLoaded+=(1/this.SPRITE_COUNT)*this.spritePercentage;
									console.log("Error loading sprite");
								}
							}
						}
					}
				}
				
				if ( this.audioIsReadyForLoading==true && this.audioLoadedCount != this.rAudio.length)
				{
					this.audioLoadedCount = 0;
					if( this.audioIsReadyForLoading )
					{
						for( var iAud = 0; iAud < this.rAudio.length; iAud++)
						{
							if( this.rAudio[iAud].check_audio_state()==true )
							{
								this.audioLoadedCount++;
							}
						}
					}
				}
				
				if (this.spritesLoadedCount == this.SPRITE_COUNT)
				{
					this.spritesIsAllLoaded = 1;
				}
				
				if (this.texturesLoadedCount == this.rTextures.length)
				{
					this.texturesIsAlLoaded = 1;
				}
				
				if ( this.audioIsReadyForLoading==true && this.audioLoadedCount == this.rAudio.length)
				{
					this.audioIsAllLoaded = 1;
				}
				if( this.spritesIsAllLoaded==1 && this.texturesIsAlLoaded==1 && this.audioIsAllLoaded==1)
				{
					this.RESOURCES_ADDED = true;
				}
			}
		};
		
		//adds a sprite with only one row of subimages
		this.sprite_add = function(spriteFileName,imgnumb,xorig,yorig, addMirror)
		{
		
		};
		
		this.sprite_add_collision = function(spriteFileName,imgnumb,xorig,yorig, cx, cy, cw, ch, addMirror)
		{
		
		};
		
		this.add_audio = function(audioIdIn, newAudioName, aacSrc, mp3Src, oggSrc,wavSrc, audioType, audioGroupName,audioDefaultVolume)
		{
			var newAudio = new GPE_AudioResource();
			newAudio.id = audioIdIn;
			newAudio.audioId = audioIdIn;
			if( newAudioName.length > 0)
			{
				newAudio.audioName = newAudioName;
			}
			newAudio.audioFileName[GPE_AAC] = aacSrc;
			newAudio.audioFileName[GPE_MP3] = mp3Src;
			newAudio.audioFileName[GPE_OGG] = oggSrc;
			newAudio.audioFileName[GPE_WAV] = wavSrc;
			newAudio.audioType = audioType;
			newAudio.audioGroupName = audioGroupName;
			newAudio.defaultVolume = audioDefaultVolume;
			newAudio.load_audio();
			this.rAudio.push(newAudio);
			return this.rAudio.length-1;
		}
		
		this.add_font = function(fontIdIn, newFontFamilyName, eotSrc, svgSrc, ttfSrc, wOffSrc,wOff2Src, fontSizeIn, estimatedWidth, estimatedHeight, isMonoSpacedFont  )
		{
			var newFont = new GPE_FontResource();
			newFont.id=fontIdIn;
			if( newFontFamilyName.length > 0)
			{
				newFont.fontFamilyName = newFontFamilyName;
			}
			newFont.fontFileName[GPE_EOT] = eotSrc;
			newFont.fontFileName[GPE_SVG] = svgSrc;
			newFont.fontFileName[GPE_TTF] = ttfSrc;
			newFont.fontFileName[GPE_WOFF] = wOffSrc;
			newFont.fontFileName[GPE_WOFF2] = wOff2Src;
			newFont.fontSize = fontSizeIn;
			newFont.estimatedWidth = estimatedWidth;
			newFont.estimatedHeight = estimatedHeight;
			newFont.isMonoSpacedFont = isMonoSpacedFont;
			newFont.load_font();
			this.rFonts.push(newFont);
			return this.rFonts.length-1;
		}
		
		this.add_sprite_empty = function()
		{
			this.rSprites.push( IS_NULL );
			this.SPRITE_COUNT += 1;
			return this.rSprites.length-1;
		}
		
		//colWidth = colR for circles, colHeight = ignored if circle
		this.add_sprite = function (setId,maxSpriteFrames,spriteFileName,xStart, yStart,frameWidth, frameHeight, spriteShape, colX, colY, colW, colH)
		{
			var previousSpriteId = this.get_sprite(spriteFileName);
			//If the image loaded
			if( previousSpriteId == IS_NULL )
			{
				var newSprite = new GPE_GameSprite();
				newSprite.maxSpriteFrames = maxSpriteFrames;
				newSprite.spriteName = spriteFileName;
				newSprite.spriteWidth = frameWidth;
				newSprite.spriteHeight = frameHeight;
				newSprite.id=setId;
				newSprite.setId=setId;
				if( spriteFileName.length > 4)
				{
					var isPreviouslyLoaded = false;
					
					//finds preloaded images to load
					var newSprImage = this.load_new_image(spriteFileName);
					newSprite.update_sprite_data(newSprImage,xStart, yStart, frameWidth, frameHeight);
				}
				newSprite.colX = colX;
				newSprite.colY = colY;
				newSprite.colShape = spriteShape;
				if( spriteShape==1)
				{
					newSprite.colRadius = colW;
					newSprite.colWidth = colW;
					newSprite.colHeight = colW;
				}
				else
				{
					newSprite.colWidth = colW;
					newSprite.colHeight = colH;
				}
				this.rSprites.push( newSprite );
				this.SPRITE_COUNT += 1;
				return this.rSprites.length-1;
			}
			return previousSpriteId;
		};

		this.add_spritesheet= function (setId,maxSpriteFrames,spriteFileName, xStart, yStart,frameWidth, frameHeight, spriteShape, colX, colY, colW, colH)
		{
			return this.add_sprite(setId,maxSpriteFrames,spriteFileName, xStart, yStart,frameWidth, frameHeight, spriteShape, colX, colY, colW, colH);
		}
		
		this.add_video = function(videoIdIn, newVideoName, mp4Src, webmSrc, oggSrc, videoGroupName,audioDefaultVolume)
		{
			var newVideo = new GPE_VideoResource();
			newVideo.id = videoIdIn;
			newVideo.videoId = videoIdIn;
			if( newVideoName.length > 0)
			{
				newVideo.audioName = newVideoName;
			}
			newVideo.videoFileName[GPE_WEBM] = webmSrc;
			newVideo.videoFileName[GPE_MP4] = mp4Src;
			newVideo.videoFileName[GPE_VIDEO_OGG] = oggSrc;
			newVideo.videoGroupName = videoGroupName;
			newVideo.defaultVolume = audioDefaultVolume;
			this.rVideos.push(newVideo);
			return this.rVideos.length-1;
		}
		
		this.edit_collision_box = function( spriteIn, cx, cy, cw, ch)
		{
		
		};

		
		this.load_new_image = function( fileName)
		{
			var newImageSrc = this.get_game_image(fileName);
			if( newImageSrc ==-1)
			{
				newImageSrc = new Image();
				newImageSrc.src = fileName;
				this.rAllImages.push(newImageSrc);
			}
			return newImageSrc;
		}
		
		this.load_new_texture = function(fileName,  id)
		{
			//Load the image
			var loadedImage = this.get_texture(fileName);
			//If the image loaded
			if( loadedImage == IS_NULL )
			{
				var newText = new GPE_Texture();
				newText.texId=id;
			
				newText.imageSource = new Image();
				newText.imageSource.src = fileName;
				
				newText.fileLocation=fileName;
				newText.texId = id;
				if( this.rTextures.length == 0)
				{
					this.rTextures[0] = newText;
				}
				else
				{
					this.rTextures.push_back(newText);
				}
				this.texturesLoadedCount = 0;
				return newText;
			}
			return loadedImage;
		};

		
		this.add_texture = function( textIdIn, fileName)
		{
			var loadedImageId = this.get_texture(fileName);
			//If the image loaded
			if( loadedImageId == IS_NULL )
			{
				var newTexture = new GPE_Texture();
				newTexture.imageSource = this.load_new_image( fileName );
				newTexture.check_texture_state();
				newTexture.texId = textIdIn;
				newTexture.fileLocation = fileName;
				this.rTextures.push(newTexture);
				return this.rTextures.length-1;
			}
			else
			{
				return loadedImageId;
			}
			return loadedImage;
		};

		this.add_tilesheet= function(setId,fileName, tWidth, tHeight, hOff, vOff, hSep, vSep)
		{
			var nTilesheet = new GPE_Tilesheet();
			nTilesheet.id=setId;
			
			nTilesheet.tex = this.load_new_image( fileName);
			
			nTilesheet.fileLocation=fileName;
			nTilesheet.tWidth=tWidth;
			nTilesheet.tHeight=tHeight;
			nTilesheet.hOff=hOff;
			nTilesheet.vOff=vOff;
			nTilesheet.hSep=hSep;
			nTilesheet.vSep=vSep;
			nTilesheet.update_tex_specs();
			this.rTilesheets.push(nTilesheet);
			return this.rTilesheets.length - 1;
		};
		
		//gets an image based on its filename
		this.get_game_image = function( idIn)
		{
			var foundTex = IS_NULL;
			var i=0;
			if( typeof idIn =="number" )
			{
				if( idIn >=0 && idIn < this.rAllImages.length)
				{
					return this.rAllImages[idIn];
				}
				for( i=0; i< this.rAllImages.length; i++)
				{
					foundTex = this.rAllImages[i];
					if( foundTex.texId==idIn )
					{
						return foundTex;
					}
				}
			}
			else if( typeof idIn =="string" )
			{
				for( i=0; i< this.rAllImages.length; i++)
				{
					foundTex = this.rAllImages[i];
					if( foundTex.id==idIn)
					{
						return foundTex;
					}
				}
			}
			console.log("Unable to find: ["+idIn+"].");
			return IS_NULL;
		}
		
		this.get_audio = function(nameIn)
		{
			var foundAudio = IS_NULL;
			var i=0;
			if( typeof idIn =="number" )
			{
				if( idIn >=0 && idIn < this.rAudio.length)
				{
					return idIn;
				}
				for( i=this.rAudio.length-1; i >=0; i--)
				{
					foundAudio = this.rAudio[i];
					if( foundAudio.audioId==idIn )
					{
						return i;
					}
				}
			}
			else if( typeof idIn =="string" )
			{
				for( i=this.rAudio.length-1; i >=0; i--)
				{
					foundAudio = this.rAudio[i];
					if(  foundAudio.audioName==idIn)
					{
						return i;
					}
				}
			}
			return IS_NULL;
		};
		
		//gets the texture image based on it's name or id
		this.get_texture = function(idIn)
		{
			var foundTex = IS_NULL;
			var i=0;
			if( typeof idIn =="number" )
			{
				if( idIn >=0 && idIn < this.rTextures.length)
				{
					return idIn;
				}
				for( i=this.rTextures.length-1; i >=0; i--)
				{
					foundTex = this.rTextures[i];
					if( foundTex.texId==idIn )
					{
						return i;
					}
				}
			}
			else if( typeof idIn =="string" )
			{
				for( i=this.rTextures.length-1; i >=0; i--)
				{
					foundTex = this.rTextures[i];
					if( foundTex.fileLocation==idIn || foundTex.name==idIn)
					{
						return i;
					}
				}
			}
			return IS_NULL;
		};
		
		this.get_tilesheet = function(idIn)
		{
			var foundTex = IS_NULL;
			var i=0;
			if( typeof idIn =="number" )
			{
				if( idIn >=0 && idIn < this.rTilesheets.length)
				{
					return idIn;
				}
				for( i=this.rTilesheets.length-1; i >=0; i--)
				{
					foundTex = this.rTilesheets[i];
					if( foundTex.texId==idIn )
					{
						return i;
					}
				}
			}
			else if( typeof idIn =="string" )
			{
				for( i=this.rTilesheets.length-1; i >=0; i--)
				{
					foundTex = this.rTilesheets[i];
					if( foundTex.fileLocation==idIn || foundTex.name==idIn)
					{
						return i;
					}
				}
			}
			return IS_NULL;
		};
		
		this.get_sprite = function(idIn)
		{
			var foundSprite = IS_NULL;
			var i=0;
			if( typeof idIn =="number" )
			{
				if( idIn >=0 && idIn < this.rSprites.length)
				{
					return idIn;
				}
				for( i=this.rSprites.length-1; i >=0; i--)
				{
					foundSprite = this.rSprites[i];
					if( foundSprite.texId==idIn )
					{
						return i;
					}
				}
			}
			else if( typeof idIn =="string" )
			{
				for( i=this.rSprites.length-1; i >=0; i--)
				{
					foundSprite = this.rSprites[i];
					if( foundSprite.fileLocation==idIn || foundSprite.name==idIn)
					{
						return i;
					}
				}
			}
			return IS_NULL;
		};
				
		//File loading
		this.load_files = function()
		{
		
		};
		
		//Clean up
		this.clean_up = function()
		{
		
		};
	};

	//The resource loading class
	function GPE_ResourceLoader()
	{
		this.stateName = "ResourceLoader";
		this.imagesLoaded = 0;
		this.audioLoaded = false;
		this.spritesLoaded = false;
		this.percentLoaded = 0;
		this.latestLoadMessage = "";
		//For now images take up the main amount of resources.
		//This will either be a full even split or actually dependent on amount of files per category
		this.imagePercentage = 100;
		this.musicPercentage = 0;
		this.spritesPercentage = 0;
		this.postLoadPauseTick = 0;
		this.postLoadPauseMax = 5;
		this.loadingBegan = false;
		this.imageStartLoadPos = 0;
		this.gpeAppOpend = false;
		
		this.apply_logic = function()
		{
			if( gpe.preload_completed() )
			{
				this.postLoadPauseTick += 1;
				if( this.postLoadPauseTick>=this.postLoadPauseMax )
				{
					if( this.gpeAppOpend == false)
					{
						gpe.fileReader.open_file('js/gpe_app.js');
						this.gpeAppOpend = true;
					}
					else if( gpe.rsm.audioIsReadyForLoading==true)
					{
						set_next_state(programStates.STATE_MAINGAME);
					}
				}
			}
			else
			{
				if( this.loadingBegan==false)
				{
					imageCount = GPE_PreloadImageResource.length;
					this.postLoadPauseTick += 1;
					if( this.postLoadPauseTick>=this.postLoadPauseMax )
					{
						this.postLoadPauseTick = 0;
						
						var imagesToPreload = GPE_PreloadImageResource.length;
						var newImageToLoad = IS_NULL;
						if (this.imageStartLoadPos >=0 && this.imageStartLoadPos < imageCount)
						{
							newImageToLoad = new Image();
							newImageToLoad.onerror = function(){ this.alt = "failed"; console.log("Failed to load image["+this.src+"]...");}
							newImageToLoad.src = GPE_PreloadImageResource[this.imageStartLoadPos];
							newImageToLoad.id = GPE_PreloadImageResource[this.imageStartLoadPos];
							newImageToLoad.alt = "false";
							preloadedImages.push(newImageToLoad);
						}
						this.imageStartLoadPos++;
					}
					if( this.imageStartLoadPos >= imageCount )
					{
						this.loadingBegan = true;
					}
				}
				//
				{
					for (iLoop = 0; iLoop < preloadedImages.length; iLoop++)
					{
						if( preloadedImages[iLoop].alt == "false")
						{
							if( IsImageOk( preloadedImages[iLoop] ) )
							{
								preloadedImages[iLoop].alt = "true";
								this.imagesLoaded++;
								this.percentLoaded+=(1/imageCount)*this.imagePercentage;
								gpe.rsm.rAllImages.push( preloadedImages[iLoop]);
							}
						}
						else if( preloadedImages[iLoop].alt == "failed")
						{
							console.log("Failed to load ["+GPE_PreloadImageResource[iLoop]+"] image.");
							preloadedImages[iLoop].alt = "failed";
							this.imagesLoaded++;
							this.percentLoaded+=(1/imageCount)*this.imagePercentage;
							gpe.rsm.rAllImages.push( preloadedImages[iLoop]);
						}
					}
					if (this.imagesLoaded == imageCount)
					{
						gpe.PRELOAD_COMPLETE = 1;
					}
				}
			}
		}
		
		this.render_state = function()
		{
			gpe.context.fillStyle = '#333';
			gpe.context.fillRect(0, 0, gpe.canvas.width, gpe.canvas.height);
			if( gpe.preload_completed() )
			{
				gpe.context.font = 'bold 24pt Calibri';
				gpe.context.textAlign = 'center';
				gpe.context.textBaseline="middle"; 
				gpe.context.fillStyle = '#FFF';
				gpe.context.fillText('Starting Game...', gpe.canvas.width/2, gpe.canvas.height/2 );
			}
			else
			{
				gpe.context.font = 'bold 12pt Calibri';
				gpe.context.textAlign = 'center';
				gpe.context.textBaseline="top"; 
				gpe.context.fillStyle = '#FFF';
				gpe.context.fillText("Now loading...["+(this.percentLoaded|0)+"%]", gpe.canvas.width/2, gpe.canvas.height/2+32 );
				
				if( imageCount > 0)
				{
					gpe.render_rectangle(0,gpe.canvas.height/2-32,(this.percentLoaded/100*gpe.canvas.width),gpe.canvas.height/2,'blue',false, 4 , true);
					gpe.render_rectangle(0,gpe.canvas.height/2-32,(this.percentLoaded/100*gpe.canvas.width),gpe.canvas.height/2,'white',true, 4 , true);
				}
				else
				{
					gpe.context.font = 'bold 24pt Calibri';
					gpe.context.textAlign = 'center';
					gpe.context.textBaseline="middle"; 
					gpe.context.fillText('Loading...', gpe.canvas.width/2, gpe.canvas.height/2 );
				}
			}
		}
	}

	//The main menu state

	//Menu options class just to do even cooler things
	function GPE_MainMenuOption()
	{
		this.optionName = "";
		this.isUnlocked = false;
	}

	//Really generic at 1st
	function GPE_MainMenu()
	{
		this.stateName = "MainMenu";
		this.menuFont = 'bold 32pt Calibri';
		this.menuFooterFont = 'bold 18pt Calibri';
		this.naviSound = new Audio();
		this.naviSelectSound = new Audio();
		//this.naviMusic = gpe.load_music("resources/sfx/music/GameIntroMusic.ogg",true);
		this.menuIcon;
		this.menuBackground = preloadedImages[2];
		this.menuSelectPos = 2;
		
		this.menuFontColor = '#FFF';
		this.menuHighLightFontColor = 'blue';
		this.menuLockedColor = 'rgb(128, 128, 128)';
		this.menuLockedHighLightColor = 'maroon';
		this.menuChoices = new Array();
		
		this.add_menu_option = function(opName, optionIsUnlocked)
		{
			var newOp = new GPE_MainMenuOption();
			newOp.optionName = opName;
			newOp.isUnlocked = optionIsUnlocked;
			this.menuChoices.push(newOp);
		}
		
		this.apply_logic = function()
		{
			var player1Controller = gpe.inputController.get_controller(1);
			if( player1Controller!=-1)
			{
				if( player1Controller.check_button_released(gpe.cbi.UP_BUT) )
				{
					this.menuSelectPos-=1;
				}
				
				if( player1Controller.check_button_released(gpe.cbi.DOWN_BUT) )
				{
					this.menuSelectPos+=1;
				}
			}
			
			if( gpe.inputController.keyIsReleased[gp_w] )
			{
				this.menuSelectPos-=1;
			}
			
			if( gpe.inputController.keyIsReleased[gp_s] )
			{
				this.menuSelectPos+=1;
			}
			
			if( gpe.inputController.keyIsReleased[gp_enter] )
			{
				set_next_state(programStates.STATE_MAINGAME);
			}
			if( this.menuSelectPos < 0)
			{
				this.menuSelectPos=0;
			}
			if( this.menuSelectPos >= this.menuChoices.length)
			{
				this.menuSelectPos = this.menuChoices.length - 1;
			}
		}
		
		this.render_state = function()
		{
			gpe.context.fillStyle = '#333';
			gpe.context.fillRect(0, 0, gpe.canvas.width, gpe.canvas.height);
			
			if( gpe.preload_completed() )
			{
				gpe.context.drawImage(this.menuBackground, 0, 0, gpe.canvas.width, gpe.canvas.height );
			}
			gpe.context.font = this.menuFont;
			gpe.context.textAlign = 'center';
			var foundOption;
			for(var iCtr=this.menuChoices.length-1;iCtr>=0;iCtr--)
			{
				foundOption = this.menuChoices[iCtr];
				
				if( foundOption.isUnlocked )
				{
					if( this.menuSelectPos == iCtr)
					{
						gpe.context.fillStyle = this.menuHighLightFontColor;
					}
					else
					{
						gpe.context.fillStyle = this.menuFontColor;
					}
				}
				else
				{
					if( this.menuSelectPos == iCtr)
					{
						gpe.context.fillStyle = this.menuLockedHighLightColor;
					}
					else
					{
						gpe.context.fillStyle = this.menuLockedColor;
					}
				}
				gpe.context.fillText(foundOption.optionName, gpe.SCREEN_WIDTH/2, gpe.SCREEN_HEIGHT-64+( (iCtr-this.menuChoices.length )*64) );
			}
			gpe.context.font = this.menuFooterFont;
			gpe.context.textAlign = 'right';
			gpe.context.fillStyle = '#FFF';
			gpe.context.fillText("Version "+gpe.GPE_APP_VERSION_NUMBER.toFixed(1),gpe.SCREEN_WIDTH-32, gpe.SCREEN_HEIGHT-32);
			gpe.context.textAlign = 'center';
			gpe.context.fillText("Published By "+gpe.GPE_APP_PUBLISHER, gpe.SCREEN_WIDTH/2, gpe.SCREEN_HEIGHT-48);
			gpe.context.fillText("Developed By "+gpe.GPE_APP_DEVELOPER, gpe.SCREEN_WIDTH/2, gpe.SCREEN_HEIGHT-16);
		}
		
		this.add_menu_option("Continue",0);
		this.add_menu_option("New Game",1);
		this.add_menu_option("Options",1);
		this.add_menu_option("Multiplayer",1);
		this.add_menu_option("Extras",0);
		//this.add_menu_option("Exit",1);
	}

	window.countFPS = (function ()
	{
	  var lastLoop = (new Date()).getMilliseconds();
	  var count = 1;
	  var fps = 0;

	  return function ()
	  {
		var currentLoop = (new Date()).getMilliseconds();
		if (lastLoop > currentLoop)
		{
		  fps = count;
		  count = 1;
		}
		else
		{
		  count += 1;
		}
		lastLoop = currentLoop;
		return fps;
	  };
	}());

	/**
	* The GPE_GameController Class.
	*/
	function GPE_GameController(mainDisplayWidth, mainDisplayHeight, secondaryDisplayWidth, secondaryDisplayHeight, deviceIn)
	{
		this.OUT_OF_SIGHT_OUT_OF_PROCESSING = true;
		this.GPE_VERSION_NUMBER = 1;
		this.GPE_APP_VERSION_NUMBER = 1.0;
		this.GPE_APP_DEVELOPER = GPE_GAME_DEVELOPER;
		this.GPE_APP_PUBLISHER = GPE_GAME_PUBLISHER;
		this.SCREEN_WIDTH = mainDisplayWidth;
		this.SCREEN_HEIGHT = mainDisplayHeight;
		this.SCREEN2_WIDTH = secondaryDisplayWidth
		this.SCREEN2_HEIGHT = secondaryDisplayHeight;
		this.originalScreenWidth = mainDisplayWidth;
		this.originalScreenHeight = mainDisplayHeight;
		this.screenScale = 1;
		this.isInFullScreenMode = false;
		this.fullScreenScale = 'aspect';
		this.fps = 30;
		
		this.maxCameras = 8;
		this.deviceType = deviceIn;
		
		this.MOUSE_X = 0;
		this.MOUSE_Y = 0;
		this.MOUSE_SCALED_X = 0;
		this.MOUSE_SCALED_Y = 0;
		
		this.currentState = 0;
		
		this.canvasIsEnabled = true;
		this.OBJECT_RENDER_CHECKS = 0;
		this.OBJECT_RENDER_CALLS = 0;
		this.PRELOAD_COMPLETE = 0;
		
		this.EXTRA_CANVASES = [];
		this.EXTRA_CONTEXTS = [];
		
		this.LEVEL_CURRENT_NAME = "";
		
		this.exPlugins = [];
		this.EX_PLUGIN_COUNT = 0;
		//canvas variables
		this.canvas = IS_NULL;
		this.context = IS_NULL;
		this.currentCameraInView = 0;
		this.GPE_Current_Context = IS_NULL;
		this.mapCanvas = IS_NULL;
		this.mapContext = IS_NULL;
		this.preGuiCanvas = IS_NULL;
		this.preGuiContext = IS_NULL;
		//engine main limbs
		this.inputController = IS_NULL;
		this.rsm = IS_NULL;
		this.fileReader = IS_NULL;

		this.CURRENT_VIEW = IS_NULL;
		this.CURRENT_VIEW_BOX = {xPos:0, yPos: 0};
		this.lastLoopedTrack = -1;
		this.lastLoopedVideo = -1;
		this.currentVideoPlaying = -1;
		this.isPlayingVideo = false;
		this.videoPlayingIsSkippable = true;
		this.cbi = 
		{
			LEFT_BUT: 0,
			RIGHT_BUT: 1,
			UP_BUT: 2,
			DOWN_BUT: 3,
			A_BUT: 4,
			B_BUT: 5,
			X_BUT: 6,
			Y_BUT: 7,
			START_BUT: 8,
			SELECT_BUT: 9,
			L_BUT: 10,
			R_BUT: 11 ,
			ZL_BUT: 12,
			ZR_BUT: 13,
			L2_BUT: 14,
			L3_BUT: 15,
			R2_BUT: 16,
			R3_BUT: 17,
			HOME_BUT: 18,
			SHARE_BUT: 19,
			EXT1_BUT: 20,
			EXT2_BUT: 21,
			EXT3_BUT: 22,
			EXT4_BUT: 23,
			EXT5_BUT: 24
		};
		
		//scene related variables
		this.gpeScenes = [];
		this.gpeScenePos = 0;
		
		//Trig related variables
		this.cosValues = [];
		this.sinValues = [];
		this.tanValues = [];
		this.arcCosValues = [];
		this.arcSinValues = [];
		this.tarcTanValues = [];
	
		//Path related functions/objects/variables as of Version 1.12 [ BEGIN ] 
		this.gamePaths = [];
		
		this.GPE_GamePath = (function (pId, pIsClosed) 
		{
			function GPE_GamePath(pId, pIsClosed)
			{
				this.pathId = pId;
				this.isClosedPath =  pIsClosed
				this.pathPoints = [];
				this.pathOriginalPoints = [];
				this.pathLength = 0;
				this.originalPathLength = 0;
			};
			
			//Used for the default path. 
			GPE_GamePath.prototype.add_original_point = function( pX,pY, pSpeed )
			{
				if( typeof pX != "number" )
				{
					pX = 0;
				}
				
				if( typeof pY != "number" )
				{
					pY = 0;
				}
				
				if( typeof pSpeed == "number" )
				{
					if( pSpeed <= 0 )
					{
						pSpeed = 0.1;
					}
				}
				else
				{
					pSpeed = 1;
				}
				var newPoint = { pointX: pX, pointY: pY, pointRSpeed: pSpeed };
				this.pathOriginalPoints.push( newPoint );
				this.pathPoints.push( newPoint );
				this.originalPathLength++;
				this.pathLength++;
				return newPoint;
			};
			
			//Used for points created from gpe_app.js and are rewriteable
			GPE_GamePath.prototype.add_new_point = function( pX,pY, pSpeed )
			{
				if( typeof pX != "number" )
				{
					pX = 0;
				}
				
				if( typeof pY != "number" )
				{
					pY = 0;
				}
				
				if( typeof pSpeed == "number" )
				{
					if( pSpeed <= 0 )
					{
						pSpeed = 0.1;
					}
				}
				else
				{
					pSpeed = 1;
				}
				var newPoint = { pointX: pX, pointY: pY, pointRSpeed: pSpeed };
				this.pathPoints.push( newPoint );
				this.pathLength++;
				return newPoint;
			};
			
			GPE_GamePath.prototype.insert_new_point = function( pos, pX,pY, pSpeed )
			{
				if( typeof pos == "number" && pos >=0 && pos < this.pathLength )
				{
					if( typeof pX != "number" )
					{
						pX = 0;
					}
					
					if( typeof pY != "number" )
					{
						pY = 0;
					}
					
					if( typeof pSpeed == "number" )
					{
						if( pSpeed <= 0 )
						{
							pSpeed = 0.1;
						}
					}
					else
					{
						pSpeed = 1;
					}
					var newPoint = { pointX: pX, pointY: pY, pointRSpeed: pSpeed };
					this.pathPoints.insert( pos, newPoint );
					this.pathLength++;
					return newPoint;
				}
				//if the pos is out of bounds, we just add it to the end of the path list
				return this.add_new_point(pX,pY, pSpeed );
			};
			
			GPE_GamePath.prototype.get_point = function( pointNumber )
			{
				if( typeof pointNumber == "number" )
				{
					if( pointNumber >=0 && pointNumber < this.pathLength )
					{
						return this.pathPoints[ pointNumber ];
					}
				}
				return IS_NULL;
			}
			
			GPE_GamePath.prototype.get_size = function(  )
			{
				return this.pathLength;
			}
			GPE_GamePath.prototype.restore_path = function(  )
			{
				this.pathPoints = [];
				this.pathLength = 0;
				var fPoint;
				var newPoint;
				for( var i = 0; i < this.originalPathLength; i++ )
				{
					fPoint = this.pathOriginalPoints[ i ];
					newPoint = { pointX: fPoint.pointX, pointY: fPoint.pointY, pointRSpeed: pointRSpeed.pointRSpeed };
				}
				this.pathLength = this.originalPathLength;
			}
			return GPE_GamePath;
		}());

		this.add_path = function(pId, pIsClosed )
		{
			var newPath =  new gpe.GPE_GamePath( pId, pIsClosed );
			this.gamePaths.push  ( newPath );
			return newPath;
		};
		
		this.create_path = function( pIsClosed, addToList )
		{
			var newPath = IS_NULL;
			if( addToList )
			{
				newPath =  new gpe.GPE_GamePath( this.gamePaths.length, pIsClosed );
				this.gamePaths.push  ( newPath );
			}
			else
			{
				
				newPath =  new gpe.GPE_GamePath( -1, pIsClosed );
			}
			return newPath;
		};
		
		this.get_path = function( pId )
		{
			if( typeof pId == "number" )
			{
				if( pId >=0 && pId < this.gamePaths.length )
				{
					return this.gamePaths[ pId ];
				}
			}
			return IS_NULL;
		};
		
		this.get_path_length = function( pId )
		{
			if( typeof pId == "number" )
			{
				if( pId >=0 && pId < this.gamePaths.length )
				{
					return this.gamePaths[ pId ].pathLength;
				}
			}
			return IS_NULL;
		};
		
		this.get_path_original_length = function( pId )
		{
			if( typeof pId == "number" )
			{
				if( pId >=0 && pId < this.gamePaths.length )
				{
					return this.gamePaths[ pId ].originalPathLength;
				}
			}
			return IS_NULL;
		};
		
		this.restore_path = function( pId )
		{
			if( typeof pId == "number" )
			{
				if( pId >=0 && pId < this.gamePaths.length )
				{
					this.gamePaths[ pId ].restore_path();
				}
			}
		};
		//Path related functions/objects/variables[ END ] 
		
		for( var iMaths = 0; iMaths <=360; iMaths++)
		{
			this.cosValues[iMaths] = Math.cos(iMaths*radiansMultiple);
			this.sinValues[iMaths] = Math.sin(iMaths*radiansMultiple);
			this.tanValues[iMaths] = Math.tan(iMaths*radiansMultiple);
		}
		
		//Math Functions[ BEGIN ] 
		this.abs = function( x)
		{
			if( typeof x ==='number')
			{
				if( x < 0)
				{
					x*=-1;
				}
				return x;
			}
			return 0;
		};
		
		this.bound_number = function( x,  xMin, xMax)
		{
			if( x < xMin)
			{
				x = xMin;
			}
			else if( x > xMax)
			{
				x = xMax;
			}
			return x;
		};
		
		this.cos = function( angleIn)
		{
			if( angleIn < 0)
			{
				angleIn*=-1;
				while( angleIn < 0)
				{
					angleIn+=360;
				}
			}
			else if( angleIn >360)
			{
				while( angleIn > 360)
				{
					angleIn-=360;
				}
			}
			return this.cosValues[angleIn|0];
		};
		
		this.degree = function(angle)
		{
			angle*=degreesMultiple;
			angle+=180;
			return angle;
		};
		
		this.get_direction = function( x1, y1, x2, y2)
		{
			angle = Math.atan2(y1-y2,x1-x2);
			return angle;
		};

		this.get_distance = function( x1,  y1,  x2,  y2)
		{
			if(x1==x2&&y1==y2)
			{
				return 0;
			}
			return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
		};
		
		this.get_random = function(min,max)
		{
			return  ( (Math.random()*(max-min+1)+min)|0 );
		};
		
		this.round = function( number)
		{
			return number|0;
		};
		
		this.sin = function( angleIn)
		{
			if( angleIn < 0)
			{
				angleIn*=-1;
				while( angleIn < 0)
				{
					angleIn+=360;
				}
			}
			else if( angleIn >360)
			{
				while( angleIn > 360)
				{
					angleIn-=360;
				}
			}
			return this.sinValues[angleIn|0]
		};
		
		this.tan = function( angleIn)
		{
			if( angleIn < 0)
			{
				angleIn*=-1;
				while( angleIn < 0)
				{
					angleIn+=360;
				}
			}
			else if( angleIn >360)
			{
				while( angleIn > 360)
				{
					angleIn-=360;
				}
			}
			if( angleIn >=0 & angleIn <= 360)
			{
				return this.tanValues[angleIn|0];
			}
			return 0;
		};
	
		this.radian = function(angle)
		{
			angle-=180;
			angle*=radiansMultiple;
			return angle;
		};
		
		this.sign = function( x )
		{
			return typeof x === 'number' ? x ? x < 0 ? -1 : 1 : x === x ? 0 : NaN : NaN;
		};
		
		//Math Functions[ END ] 
		
		//
		//Kool functions
		this.add_canvas = function(newCanvName, width, height)
		{
			//var linebreak = document.createElement('br');
			//document.body.appendChild(linebreak);
			var newCanvas = document.createElement('canvas');
			newCanvas.width = width;
			newCanvas.height = height;
			newCanvas.id = newCanvName;
			document.body.appendChild(newCanvas);
			return newCanvas;
		};

		this.add_plugin = function(newPlugin)
		{
			if( typeof newPlugin!="undefined")
			{
				this.exPlugins[this.EX_PLUGIN_COUNT] = newPlugin;
				this.EX_PLUGIN_COUNT+=1;
			}
		};
		
		this.apply_plugins_logics = function()
		{
			var fPlugin = IS_NULL;
			for(var iPlugin = 0; iPlugin< gpe.exPlugins.size();iPlugin++)
			{
				fPlugin = gpe.exPlugins.at(iPlugin);
				if( fPlugin!=-1)
				{
					fPlugin.process_plugin();
				}
			}
		};
		
		this.add_gamescene = function( sId, sName)
		{
			var newScene = new GPE_GameScene(sId, sName);
			this.gpeScenes.push(newScene);
			return newScene;
		};
		
		this.audio_loop = function(audioIdIn)
		{
			this.play_audio(audioIdIn,true);
		};
		
		this.audio_play = function(audioIdIn,loops)
		{
			this.play_audio(audioIdIn,loops);
		};
		
		this.audio_playing = function(audioIdIn)
		{
			return this.is_audio_playing(audioIdIn);
		};

		this.audio_stop_all = function() 
		{
			this.stop_all_audio();
		};
		
		this.camera_on = function(cameraId)
		{
			return GPR.camera_on(cameraId);
		};
		
		this.center_camera = function(cameraId, centerX, centerY)
		{
			GPR.center_camera(cameraId, centerX, centerY);
		};
		
		this.check_collision_rect = function( rectA, rectB)
		{
			//Calculate the sides of rect A
			var leftA = rectA.xPos;
			var rightA = rectA.xPos + rectA.rectWidth;
			var topA = rectA.yPos;
			var bottomA = rectA.yPos + rectA.rectHeight;

			//Calculate the sides of rect B
			var leftB = rectB.xPos;
			var rightB = rectB.xPos + rectB.rectWidth;
			var topB = rectB.yPos;
			var bottomB = rectB.yPos + rectB.rectHeight;
			return !(  bottomA <= topB ||  topA >= bottomB  || rightA <= leftB || leftA >= rightB );
		};

		this.check_collision_with_rect = function( boxX, boxY, boxW, boxH, rectA)
		{
			var newRectB= new GPE_Rect();
			newRectB.update_shape(boxX, boxY, boxW, boxH);
			return this.check_collision_rect(rectA,newRectB);
		}

		//used for checking the loading state of an image
		this.check_image_loading_status = function(img)
		{
			//Credit: (sajithmr) http://blog.sajithmr.me/javascript-check-an-image-is-loaded-or-not 
			// During the onload event, IE correctly identifiesany images that
			// weren’t downloaded as not complete. Others should too. Gecko-based
			// browsers act like NS4 in that they report this incorrectly.
			if (!img.complete) 
			{
				return false;
			}

			// However, they do have two very useful properties:naturalWidth and
			// naturalHeight. These give the true size of the image. If it failed
			// to load, either of these should be zero.
			if (typeof img.naturalWidth	!= "undefined" && img.naturalWidth	== 0)
			{
				return false;
			}
			return true;
		}
		
		//gamepad functions ADDED Since Version 1.1.0
		this.gamepad_connected = function(controllerPos)
		{
			return this.inputController.gamepad_connected(controllerPos);
		};
		
		this.gamepad_object = function(controllerPos)
		{
			return this.inputController.gamepad_object(controllerPos);
		};
		
		this.gamepad_name = function(controllerPos)
		{
			return this.inputController.gamepad_name(controllerPos);
		};
		
		this.find_gamepad_mapping = function(controllerPos, buttonId)
		{
			return this.inputController.gamepad_mapping(controllerPos, buttonId);
		};
		
		this.check_gamepad = function(controllerPos, buttonId)
		{
			return this.inputController.check_gamepad_down(controllerPos, buttonId);
		}
		
		this.check_gamepad_down = function(controllerPos, buttonId)
		{
			return this.inputController.check_gamepad_down(controllerPos, buttonId);
		};
		
		this.check_gamepad_pressed = function(controllerPos, buttonId)
		{
			return this.inputController.check_gamepad_pressed(controllerPos, buttonId);
		};
		
		this.check_gamepad_released = function(controllerPos, buttonId)
		{
			return this.inputController.check_gamepad_released(controllerPos, buttonId);
		};
		
		//keyboard functions
		this.check_keyboard = function(keyId)
		{
			return this.inputController.check_keyboard_down(keyId);
		}
		
		this.check_keyboard_down = function(keyId)
		{
			return this.inputController.check_keyboard_down(keyId);
		};
		
		this.check_keyboard_pressed = function(keyId)
		{
			return this.inputController.check_keyboard_pressed(keyId);
		};
		
		this.check_keyboard_released = function(keyId)
		{
			return this.inputController.check_keyboard_released(keyId);
		};
		
		//mouse functions
		this.check_mouse = function(mbId)
		{
			return this.inputController.check_mouse_down(mbId);
		}
		
		this.check_mouse_down = function(mbId)	
		{
			return this.inputController.check_mouse_down(mbId);
		};
		
		this.check_mouse_pressed = function(mbId)
		{
			return this.inputController.check_mouse_pressed(mbId);
		};
		
		this.check_mouse_released = function(mbId)
		{
			return this.inputController.check_mouse_released(mbId);
		};
		
		this.disable_camera = function(cameraId)
		{
			GPR.disable_camera(cameraId);
		}
		
		this.disable_canvas  = function()
		{
			this.canvasIsEnabled = false;
			this.canvas.className= "hide";
		}
		
		this.disable_outofsightmode = function()
		{
			this.OUT_OF_SIGHT_OUT_OF_PROCESSING = false;
		}
		
		this.enable_outofsightmode = function()
		{
			this.OUT_OF_SIGHT_OUT_OF_PROCESSING = true;
		}
		
		this.enable_camera = function(cameraId)
		{
			GPR.enable_camera(cameraId);
		}
		
		this.enable_canvas  = function()
		{
			this.canvasIsEnabled = true;
			this.canvas.className= "show";
		}
		
		this.enter_render = function()
		{
			gpe.GPE_Current_Context = gpe.context;
			if( gpe.isPlayingVideo==false && gpe.currentState != 0)
			{
				gpe.currentState.render_state();
			}
			this.render_top_hud();
			SYS_FPS = window.countFPS();
		};
		
		this.erase_data = function( dataIdentifier, dataTitle)
		{
			if (typeof(Storage) !== "undefined") 
			{
				localStorage.removeItem(dataIdentifier+"_"+dataTitle);
			}
		};
		
		this.exit_video_view_mode = function()
		{
			gpe.isPlayingVideo = false;
			gpe.currentVideoPlaying= -1;
		};
		
		this.find_furthest_object = function( theObjectType, xIn, yIn, avoidObjId)
		{
			return GPR.find_furthest_object( theObjectType, xIn, yIn, avoidObjId);
		}
		
		this.find_nearest_object = function( theObjectType, xIn, yIn, avoidObjId)
		{
			return GPR.find_nearest_object( theObjectType, xIn, yIn, avoidObjId);
		}
		
		this.get_camera_height = function(cameraId)
		{
			return GPR.get_camera_height( cameraId);
		};
		
		this.get_camera_info = function(cameraId)
		{
			return GPR.get_camera_info( cameraId );
		}
		
		this.get_camera_width = function(cameraId)
		{
			return GPR.get_camera_width( cameraId);
		}
		
		this.get_camera_x = function(cameraId)
		{
			return GPR.get_camera_x( cameraId);
		}
		
		this.get_camera_y = function(cameraId)
		{
			return GPR.get_camera_y( cameraId);
		}
		
		this.get_camera_mouse_x = function(cameraId)
		{
			return GPR.get_camera_mouse_x(cameraId);
		};
		
		this.get_camera_mouse_y = function(cameraId)
		{
			return GPR.get_camera_mouse_y(cameraId);
		};
		
		this.get_camera_screen_height = function(cameraId)
		{
			return GPR.get_camera_screen_height( cameraId);
		}
		
		this.get_camera_screen_width = function(cameraId)
		{
			return GPR.get_camera_screen_width(cameraId);
		}
		
		this.get_camera_screen_x = function(cameraId)
		{
			return GPR.get_camera_screen_x( cameraId);
		}
		
		this.get_camera_screen_y = function(cameraId)
		{
			return GPR.get_camera_screen_y( cameraId);
		}
		
		this.get_current_camera = function()
		{
			return this.currentCameraInView;
		}
		
		this.get_current_camera_id = function()
		{
			return this.currentCameraInView;
		}
		
		this.handle_canvas_size = function()
		{
			if( gpe.canvasIsEnabled )
			{
				if( programStateId==programStates.STATE_MAINGAME )
				{
					//half-scale
					if( GPE_GAME_SCALING_SETTING ==1)
					{
						gpe.canvas.width = GPE_SETTINGS_SCREEN_WIDTH/2;
						gpe.canvas.height =  GPE_SETTINGS_SCREEN_HEIGHT/2;
						gpe.masterScaleX = 0.5;
						gpe.masterScaleY = 0.5;
					}
					else if( GPE_GAME_SCALING_SETTING ==2)
					{
						//double-scale
						gpe.canvas.width = GPE_SETTINGS_SCREEN_WIDTH * 2;
						gpe.canvas.height =  GPE_SETTINGS_SCREEN_HEIGHT * 2;
						gpe.masterScaleX = 2;
						gpe.masterScaleY = 2;
						
					}
					else if( GPE_GAME_SCALING_SETTING ==3)
					{
						//aspect ratio[Reccommended]
						gpe.canvas.height = window.innerHeight;
						gpe.canvas.width =  window.innerHeight * (GPE_SETTINGS_SCREEN_WIDTH/GPE_SETTINGS_SCREEN_HEIGHT);
						gpe.masterScaleX = gpe.canvas.width / GPE_SETTINGS_SCREEN_WIDTH;
						gpe.masterScaleY = gpe.canvas.height / GPE_SETTINGS_SCREEN_HEIGHT;
					}
					else if( GPE_GAME_SCALING_SETTING ==4 )
					{
						//full on scaling
						gpe.canvas.height = window.innerHeight;
						gpe.canvas.width =  window.innerWidth;
						gpe.masterScaleX = gpe.canvas.width / GPE_SETTINGS_SCREEN_WIDTH;
						gpe.masterScaleY = gpe.canvas.height / GPE_SETTINGS_SCREEN_HEIGHT;
					}
					else
					{
						//no scaling
						gpe.canvas.height = GPE_SETTINGS_SCREEN_HEIGHT;
						gpe.canvas.width =  GPE_SETTINGS_SCREEN_WIDTH;
						gpe.masterScaleX = 1;
						gpe.masterScaleY = 1;
					}
				}
				else
				{
					//full on scaling for non main game state
					gpe.canvas.height = window.innerHeight;
					gpe.canvas.width =  window.innerWidth;
					gpe.masterScaleX = gpe.canvas.width / GPE_SETTINGS_SCREEN_WIDTH;
					gpe.masterScaleY = gpe.canvas.height / GPE_SETTINGS_SCREEN_HEIGHT;
				}
			}
			else
			{
				gpe.canvas.height = 0;
				gpe.canvas.width =  0;
				gpe.masterScaleX = 1;
				gpe.masterScaleY = 1;
			}
		};
		
		this.load_data = function( dataIdentifier, dataTitle)
		{
			if (typeof(Storage) !== "undefined") 
			{
				var returnVal = localStorage.getItem(dataIdentifier+"_"+dataTitle);
				if( returnVal==null)
				{
					return IS_NULL;
				}
				return returnVal;
			}
			return IS_NULL;
		}
		
		/*
		* The GPE_GameObject Class.
			default_load/initialize stuff should be done in the GPE_GameObject constructor.
		*/
		
		this.GPE_GameObject = (function (xPosIn, yPosIn, objectLayerId) 
		{
			function GPE_GameObject(xPosIn, yPosIn, objectLayerId)
			{
				/************ PLEASE ONLY USE THE FOLLOWING VARIABLES AS GETTERS/SETTERS TO AVOID BUGS *///////////////
				//Added as of Version 1.12 [ BEGIN ] 
				this.pathId = -1;
				this.foundPathObject = -1;
				this.pathPosition = 0;
				this.pathRelativeX = 0;
				this.pathRelativeY = 0;
				this.pathTargetX = 0;
				this.pathTargetY = 0;
				this.pathTargetPosition = 0;
				this.pathReplayOnEnd = true;
				this.pathSpeed = 0;
				this.pathCurrentSpeed = 0;
				this.pathCurrentDirection = 0;
				this.pathGoingForward = true;
				//Added as of Version 1.12 [ END ] 
				
				this.sceneLayerPos = objectLayerId;
				this.sceneLayerStartPos = objectLayerId;
				this.customComponents = {};
				this.gpeSceneStartId = IS_NULL;
				this.spriteId = IS_NULL;
				this.outlineTestColor = 'lime';
				this.objectDepth = 0;
				this.prevObjectDepth = 0;
				this.objectDepthPosition = 0;
				this.gpeObjectType = 0;
				this.gpeUniqGameObjectId = -1;
				this.GPE_AddedToGridSystem = false;
				this.gpeLogicAppliedInFrame = false;
				this.gpePrelogicAppliedInFrame = false;
				this.gpeAddedToRenderList = false;
				this.gpeAddedToContinuousList = false;
				this.hasPreRenderFunction = false;
				this.hasRenderFunction = true;
				this.hasPostRenderFunction = false;
				this.gpeHasTimedFunctions = false;
				this.hasHudRenderFunction = false;
				this.gpeHasStartLogicFunction = false;
				this.gpeHasLogicFunction = false;
				this.gpeSceneStartCodeUsed = false;
				
				this.prerenderedInFrame = [];
				this.renderedInFrame = [];
				this.renderedAboveInFrame = [];
				this.hudrenderedInFrame = [];
				
				for( var iView = 0; iView < gpe.maxCameras; iView++)
				{
					this.prerenderedInFrame[iView] = false;
					this.renderedInFrame[iView] = false;
					this.renderedAboveInFrame[iView] = false;
					this.hudrenderedInFrame[iView] = false;
				}
				this.collisionBox = new GPE_Rect();
				this.collisionBox.update_shape( xPosIn, yPosIn, 32, 32);
				/************ END OF GETTER/SETTER ONLY VARIABLES *///////////////
				this.isOutsideScene = false;
				this.lastCollisionObjId = -1;
				this.applyGravity = true;
				this.gravityDirection = 270;
				this.gravityEffect = 0;
				this.spriteFrame = 0;
				this.imageSpeed = 0;
				this.animationEnded = false;
				this.isVisible = false;
				this.xBegin = xPosIn;
				this.xPos = xPosIn;
				this.x2Pos = xPosIn + 32;
				this.yBegin = yPosIn;
				this.yPos = yPosIn;
				this.y2Pos = yPosIn + 32;
				this.xEnd = 0;
				this.yEnd = 0;
				this.xVel = 0;
				this.yVel = 0;
				this.xPast = 0;
				this.x2Past = 0;
				this.yPast = 0;
				this.y2Past = 0;
				this.xspeed = 0;
				this.yspeed = 0;
				this.direction = 0;
				this.speed = 0;
				this.defaultSpeed = 0;
				this.animationDuration = 0;
				this.gpeIsBeingDestroyed = false;
				this.isInit = true;
				this.isViewIndependent = false;
				this.isInView = [];
				this.timeToCheckForInView = [];
				
				var ii = 0;
				for( ii=0; ii <gpe.maxCameras; ii++)
				{
					this.isInView[ii] = false;
					this.timeToCheckForInView[ii] = 0;
				}
				
				this.functionTimer = [];
				for( ii = 0; ii <=10; ii++)
				{
					this.functionTimer[ii] = 0;
				}
				this.timedFunction0 = function(){};
				this.timedFunction1 = function(){};
				this.timedFunction2 = function(){};
				this.timedFunction3 = function(){};
				this.timedFunction4 = function(){};
				this.timedFunction5 = function(){};
				this.timedFunction6 = function(){};
				this.timedFunction7 = function(){};
				this.timedFunction8 = function(){};
				this.timedFunction9 = function(){};
				this.timedFunction10 = function(){};
				
				this.OBJECT_COLLISION_LIST = [];
				this.SPATIAL_PARTITION_LIST = [];
				this.GPE_SearchForCollisionBegin = 0;
				
				this.under_mouse = function( cameraId )
				{
					if( typeof cameraId=="undefined")
					{
						var cameraId = 0;
					}
					else if( cameraId < 0 || cameraId >= gpe.maxCameras)
					{
						cameraId = 0;
					}
					if( GPR.MOUSE_CAM_X[cameraId]>=this.xPos && GPR.MOUSE_CAM_X[cameraId] <= this.x2Pos )
					{
						if( GPR.MOUSE_CAM_Y[cameraId] >=this.yPos && GPR.MOUSE_CAM_Y[cameraId]<=this.y2Pos )
						{
							return true;
						}
					}
					return false
				}
			};
			
			GPE_GameObject.prototype.add_object = function(objType, nX, nY)
			{
				return GPE.add_new_object( objType, nX, nY, this.sceneLayerStartPos);
			};
			
			GPE_GameObject.prototype.bounce_from = function(otherObj)
			{
				if( typeof otherObj!="undefined")
				{
					
				}
			};
			
			GPE_GameObject.prototype.collide_with = function(otherObj)
			{
			
			};
			
			GPE_GameObject.prototype.initialize = function ()
			{
				// Call superclass constructor
				this.default_load();
			}
			
			GPE_GameObject.prototype.equals = function( otherObject)
			{
				if( typeof otherObject !="undefined" )
				{
					if( otherObject.gpeObjectType==this.gpeObjectType && otherObject.gpeUniqGameObjectId == this.gpeUniqGameObjectId )
					return true;
					else
					return false;
				}
				return -1;
			}
			
			GPE_GameObject.prototype.default_load = function ()
			{
				this.spriteId = IS_NULL;
				this.lastCollisionObjId = IS_NULL;
				this.applyGravity = true;
				this.gravityDirection = 270;
				this.gravityEffect = 0;
				this.spriteFrame = 0;
				this.imageSpeed = 0;
				this.animationEnded = false;
				this.isVisible = false;
				this.xEnd = 0;
				this.yEnd = 0;
				this.xVel = 0;
				this.yVel = 0;
				this.xPast = 0;
				this.yPast = 0;
				this.x2Past = 0;
				this.y2Past = 0;
				this.xspeed = 0;
				this.yspeed = 0;
				this.direction = 0;
				this.speed = 0;
				this.defaultSpeed = 0;
				this.animationDuration = 0;
				this.gpeUniqGameObjectId = -1;
				this.SPATIAL_PARTITION_LIST = [];
			}
			
			GPE_GameObject.prototype.get_component = function(componentName)
			{
				var returnVal = this.customComponents[componentName];
				if(typeof returnVal!="undefined")
				{
					return returnVal;
				}
				else
				{
					return IS_NULL;
				}
			}
			
			GPE_GameObject.prototype.gpe_constructor = function()
			{
				// Call superclass constructor
				this.default_load();
			}
			
			GPE_GameObject.prototype.get_depth = function ()
			{
				return this.objectDepth;
			}
			
			GPE_GameObject.prototype.get_collision_height = function()
			{
				return this.collisionBox.rectHeight;
			}
			
			GPE_GameObject.prototype.get_collision_width = function()
			{
				return this.collisionBox.rectWidth;
			}
			
			GPE_GameObject.prototype.get_my_direction = function(  x2, y2)
			{
				return gpe.get_direction(this.collisionBox.xPos,this.collisionBox.yPos,x2,y2);
			};

			GPE_GameObject.prototype.get_my_distance = function(  x2,  y2)
			{
				return gpe.get_distance(this.collisionBox.xPos,this.collisionBox.yPos, x2,y2);
			};
			
			GPE_GameObject.prototype.get_sprite = function()
			{
				return this.spriteId;
			}
			
			GPE_GameObject.prototype.getx = function ( )
			{
				return this.xPos;
			}
			
			GPE_GameObject.prototype.getx2 = function ( )
			{
				return this.x2Pos;
			}
			
			GPE_GameObject.prototype.gety = function ( )
			{
				return this.yPos;
			}
			
			GPE_GameObject.prototype.gety2 = function ( )
			{
				return this.y2Pos;
			}
			
			GPE_GameObject.prototype.end_frame_logic = function()
			{
			
			}
			
			GPE_GameObject.prototype.gravity_use = function ()
			{
				if( this.gravityEffect!=0 || this.vspd!=0)
				{
					if (this.gravityDirection == 270)
					{
						this.yspeed +=this.gravityEffect;
						this.move_down(this.yspeed);
					}
					else if( this.gravityDirection== 90)
					{
						this.yspeed -= this.gravityEffect;
						this.move_down(this.yspeed);
					}
					else
					{
						this.xspeed+=Math.cos(this.gravityDirection)*this.gravityEffect;
						this.yspeed+=Math.sin(this.gravityDirection)*this.gravityEffect;
						
						this.move_right(this.xspeed);
						this.move_down(this.yspeed);
					}
				}
			};

			//this function is used to simply set the sprite( not updated in collision system)
			GPE_GameObject.prototype.init_sprite = function(nextSprite )
			{
				if( typeof nextSprite=="number")
				{
					this.spriteId = nextSprite;
					this.spriteFrame = 0;
					
					if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
					{
						var fSprite = gpe.rsm.rSprites[this.spriteId];
						this.collisionBox.update_shape(this.xPos+fSprite.colX, this.yPos+fSprite.colY , fSprite.colWidth,fSprite.colHeight );
						this.x2Pos = this.xPos+fSprite.colX+fSprite.colWidth;
						this.y2Pos = this.yPos+fSprite.colY+fSprite.colHeight;
					}
					else
					{
						this.collisionBox.update_shape( this.xPos ,this.yPos,32,32 );
						this.x2Pos = this.xPos+32;
						this.y2Pos = this.yPos+32;	
					}
					this.x2Past = this.x2Pos;
					this.y2Past = this.y2Pos;
				}
			};
			
			GPE_GameObject.prototype.make_view_dependent = function()
			{
				if( this.isViewIndependent )
				{
					this.isViewIndependent = false;
					var fObjToCheck = IS_NULL;
					var currentSceneLayer = this.currentScene.get_layer(this.sceneLayerPos, GPE_LAYER_TYPE_OBJECTS);
					if( currentSceneLayer!=-1)
					{
						for( var i=currentSceneLayer.independentObjects.length-1; i >=0; i--)
						{
							fObjToCheck = currentSceneLayer.independentObjects[i];
							if( fObjToCheck.gpeUniqGameObjectId == this.gpeUniqGameObjectId)
							{
								currentSceneLayer.independentObjects.splce(i);
							}
						}
					}
				}
			};

			GPE_GameObject.prototype.make_view_independent = function()
			{
				if( !this.isViewIndependent )
				{
					this.isViewIndependent = true;
					var fObjToCheck = IS_NULL;
					var currentSceneLayer = this.currentScene.get_layer(this.sceneLayerPos, GPE_LAYER_TYPE_OBJECTS);
					if( currentSceneLayer!=-1)
					{
						currentSceneLayer.independentObjects.push( this );
					}
				}
			};
			
			GPE_GameObject.prototype.move_left = function ( leftMore)
			{
				this.collisionBox.add_x(-leftMore);
				this.xPos -= leftMore;
				this.x2Pos -= leftMore;
				/*if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
				{
					var fSprite = gpe.rsm.rSprites[this.spriteId];
				}*/
				GPR.update_object_in_map(this);
			};
			
			GPE_GameObject.prototype.move_right = function ( rightMore)
			{
				this.collisionBox.add_x(rightMore);
				this.xPos += rightMore;
				this.x2Pos += rightMore;
				GPR.update_object_in_map(this);
			};
			
			GPE_GameObject.prototype.move_up = function ( upMore)
			{
				this.collisionBox.add_y( -upMore);
				this.yPos -= upMore;
				this.y2Pos -= upMore;
				GPR.update_object_in_map(this);
			};
			
			GPE_GameObject.prototype.move_down = function ( downMore)
			{
				this.collisionBox.add_y( downMore);
				this.yPos +=downMore;
				this.y2Pos +=downMore;
				GPR.update_object_in_map(this);
			};
			
			GPE_GameObject.prototype.move_towards = function( targX, targY, newSpeed)
			{
				if( typeof targX=="number" && typeof targY=="number" && typeof newSpeed=="number" )
				{
					this.direction = this.get_my_direction( targX, targY );
					this.speed = newSpeed;
					return;
				}
				this.speed = 0;
				this.direction = 0;
			};
			
			//Added as of Version 1.12 [ BEGIN ] 
			GPE_GameObject.prototype.path_begin = function ( path, pathSpeed, continueOnEnd, relativePath )
			{
				var fPathObject = IS_NULL;
				if( typeof path == "number" )
				{
					//console.log("Starting Path ["+path+"]");
					fPathObject = GPE.get_path( path );
				}
				else if( typeof path =="Object")
				{
					//console.log("Starting Path [Object]");
					fPathObject = path;
				}
				
				if( typeof pathSpeed!= "number" )
				{
					 pathSpeed = 1;
				}
				
				if( typeof continueOnEnd!= "bool" )
				{
					 continueOnEnd = true;
				}
				
				if( typeof relativePath != "bool" )
				{
					 relativePath =false;
				}
				
				if( fPathObject !=IS_NULL )
				{
					this.pathId = fPathObject.pathId;
					this.foundPathObject = fPathObject;
					this.pathPosition = 0;
					
					this.pathRelativeX = 0;
					this.pathRelativeY = 0;
					
					this.pathSpeed = Math.abs( pathSpeed );
					this.pathCurrentSpeed = Math.abs( pathSpeed );
					this.pathReplayOnEnd = continueOnEnd;
					if(  fPathObject.pathPoints.length > 1 )
					{
						//the relative point is point[0]
						//The object will now move to point[1]
						if( relativePath )
						{
							this.pathRelativeX  = fPathObject.pathPoints[0].pointX-this.collisionBox.xCenter;
							this.pathRelativeY  = fPathObject.pathPoints[0].pointY-this.collisionBox.yMiddle;
							if( pathSpeed >=0 )
							{
								this.pathPosition = fPathObject.pathPoints.length - 1;
								this.path_move_towards_point( 1 );
								this.pathGoingForward = true;
							}
							else
							{
								this.path_move_towards_point( fPathObject.pathPoints.length - 1 );
								this.pathGoingForward = false;
							}
						}
						else
						{
							//The object will now move to point[0]
							if( pathSpeed >=0 )
							{
								this.pathPosition = fPathObject.pathPoints.length - 1;
								this.path_move_towards_point( 0 );
								this.pathGoingForward = true;
							}
							else
							{
								this.path_move_towards_point( fPathObject.pathPoints.length - 1 );
								this.pathGoingForward = false;
							}
						}
					}
					else
					{
						console.log("Path does not have enough points to begin...");
					}
				}
				//console.log("Object Path Stats: Position: "+this.pathPosition+", Speed: "+this.pathCurrentSpeed+", Direction: "+this.pathCurrentDirection+" ,X:"+this.pathTargetX+" ,Y: "+this.pathTargetY+".");
			};
			
			GPE_GameObject.prototype.path_start = function ( path, pathSpeed, continueOnEnd, relativePath )
			{
				this.path_begin( path, pathSpeed, continueOnEnd, relativePath )
			};
			
			GPE_GameObject.prototype.path_move_along = function ( )
			{
				if( pathCurrentSpeed > 0 )
				{
					
				}
				return false;
			};
			
			GPE_GameObject.prototype.path_move_towards_point = function( pointId )
			{
				if( this.foundPathObject !=IS_NULL )
				{
					if( typeof pointId=="number" && pointId >=0 && pointId < this.foundPathObject.pathPoints.length )
					{
						this.pathTargetX = this.foundPathObject.pathPoints[pointId].pointX-this.pathRelativeX;
						this.pathTargetY = this.foundPathObject.pathPoints[pointId].pointY-this.pathRelativeY;
						this.pathCurrentDirection = this.get_my_direction( this.pathTargetX, this.pathTargetY );
						this.pathCurrentSpeed = this.foundPathObject.pathPoints[pointId].pointRSpeed*this.pathSpeed;
						this.pathTargetPosition = pointId;
						return true;
					}
				}
				//console.log( "Unattainable point or unreachable object..." );
				//console.log( this.foundPathObject );				
				this.pathPosition = 0;
				this.pathTargetPosition = 0;
				this.pathCurrentSpeed = 0;
				this.pathCurrentDirection = 0;
				return false;
			};
			
			
			GPE_GameObject.prototype.path_end = function ( )
			{
				this.pathId = -1;
				this.foundPathObject = IS_NULL;
				this.pathPosition = 0;
				this.pathRelativeX = 0;
				this.pathRelativeY = 0;
				this.pathReplayOnEnd = true;
				this.pathReplayOnEnd = true;
				this.pathSpeed = 0;
				this.pathCurrentSpeed = 0;
				this.pathTargetX = 0;
				this.pathTargetY = 0;
				//console.log("Ending path...");
			};
			
			GPE_GameObject.prototype.path_stop = function ( )
			{
				this.pathId = -1;
				this.foundPathObject = IS_NULL;
				this.pathPosition = 0;
				this.pathRelativeX = 0;
				this.pathRelativeY = 0;
				this.pathReplayOnEnd = true;
				this.pathReplayOnEnd = true;
				this.pathSpeed = 0;
				this.pathCurrentSpeed = 0;
				this.pathTargetX = 0;
				this.pathTargetY = 0;
			};
			
			GPE_GameObject.prototype.using_path= function ( )
			{
				if( this.pathId !=-1 && this.foundPathObject != IS_NULL)
				{
					return true;
				}
				return false;
			};
			
			GPE_GameObject.prototype.get_path_id= function ( )
			{
				return this.pathId;
			};
			
			GPE_GameObject.prototype.get_path_object= function ( )
			{
				return this.foundPathObject ;
			};
			
			GPE_GameObject.prototype.get_path_targetx= function ( )
			{
				return this.pathTargetX;
			};
			
			GPE_GameObject.prototype.get_path_targety= function ( )
			{
				return this.pathTargetY;
			};
			
			GPE_GameObject.prototype.get_path_position= function ( )
			{
				return this.pathPosition;
			};
			
			GPE_GameObject.prototype.get_path_original_size= function ( )
			{
				if( this.foundPathObject !=IS_NULL )
				{
					this.foundPathObject.originalPathLength;
				}
				return -1;
			};
			
			GPE_GameObject.prototype.get_path_size= function ( )
			{
				if( this.foundPathObject !=IS_NULL )
				{
					this.foundPathObject.pathLength;
				}
				return -1;
			};
			
			//Added as of Version 1.12 [ END ] 
				
			GPE_GameObject.prototype.perform_object_logic = function ()
			{
				
			};
			
			GPE_GameObject.prototype.perform_object_prelogic = function ()
			{
				this.xPast = this.collisionBox.xPos;
				this.yPast = this.collisionBox.yPos;
				this.x2Past = this.collisionBox.x2Pos;
				this.y2Past = this.collisionBox.y2Pos;
				this.gpeLogicAppliedInFrame = false;
				for( var iView = gpe.maxCameras-1; iView>=0; iView--)
				{
					this.prerenderedInFrame[iView] = false;
					this.renderedInFrame[iView] = false;
					this.hudrenderedInFrame[iView] = false;
				}
				this.gpeAddedToRenderList=false;
			};
			
			GPE_GameObject.prototype.process_gpe_movement = function()
			{
				//Added as of Version 1.12 [ BEGIN ] 
				if( typeof this.pathCurrentSpeed =="number" && this.pathCurrentSpeed > 0 )
				{
					//console.log("Moving along path..["+this.pathPosition+"]");
					if( this.get_my_distance( this.pathTargetX, this.pathTargetY) > this.pathCurrentSpeed*2 )
					{
						this.move_left( Math.cos(this.pathCurrentDirection)*this.pathCurrentSpeed );
						this.move_up( Math.sin(this.pathCurrentDirection)*this.pathCurrentSpeed);
					}
					else if( this.pathGoingForward )
					{
						this.pathPosition = this.pathTargetPosition;
						//Going forward if possible
						if( this.pathTargetPosition+1 < this.foundPathObject.pathPoints.length -1 )
						{
							this.path_move_towards_point( this.pathTargetPosition+1 );
						}
						else if( this.pathReplayOnEnd )
						{
							if( this.foundPathObject.isClosedPath )
							{
								this.path_move_towards_point( 0 );
							}
							else
							{
								this.path_move_towards_point( this.foundPathObject.pathPoints.length -1 );
								this.pathGoingForward = false;
							}
						}
						else
						{
							this.path_end();
						}
					}
					else
					{
						//Going backwards if possible
						if( this.pathTargetPosition-1 >=0 && this.pathTargetPosition-1 < this.foundPathObject.pathPoints.length -1 )
						{
							this.path_move_towards_point( this.pathTargetPosition-1 );
						}
						else if( this.pathReplayOnEnd )
						{
							if( this.foundPathObject.isClosedPath )
							{
								this.path_move_towards_point( this.foundPathObject.pathPoints.length -1 );
							}
							else
							{
								this.path_move_towards_point( 0 );
								this.pathGoingForward = true;
							}
						}
						else
						{
							this.path_end();
						}
					}
					//Added as of Version 1.12 [ END ] 
				}
				else if( typeof this.speed =="number" && this.speed!=0)
				{
					this.xspeed=Math.cos(this.direction)*this.speed;
					this.yspeed=Math.sin(this.direction)*this.speed;
					this.move_left(this.xspeed);
					this.move_up(this.yspeed);
					return;
				}
				this.speed = 0;
			};
			
			GPE_GameObject.prototype.process_timed_functions = function()
			{
				if(this.functionTimer[0]>0)
				{
					if(--this.functionTimer[0]<=0)
					{
						this.functionTimer[0]=-1;
						this.timedFunction0();
					}
				}
				if(this.functionTimer[1]>0)
				{
					if(--this.functionTimer[1]<=0)
					{
						this.functionTimer[1]=-1;
						this.timedFunction1();
					}
				}
				if(this.functionTimer[2]>0)
				{
					if(--this.functionTimer[2]<=0)
					{
						this.functionTimer[2]=-1;
						this.timedFunction2();
					}
				}
				if(this.functionTimer[3]>0)
				{
					if(--this.functionTimer[3]<=0)
					{
						this.functionTimer[3]=-1;
						this.timedFunction3();
					}
				}
				if(this.functionTimer[4]>0)
				{
					if(--this.functionTimer[4]<=0)
					{
						this.functionTimer[4]=-1;
						this.timedFunction4();
					}
				}
				if(this.functionTimer[5]>0)
				{
					if(--this.functionTimer[5]<=0)
					{
						this.functionTimer[5]=-1;
						this.timedFunction5();
					}
				}
				if(this.functionTimer[6]>0)
				{
					if(--this.functionTimer[6]<=0)
					{
						this.functionTimer[6]=-1;
						this.timedFunction6();
					}
				}
				if(this.functionTimer[7]>0)
				{
					if(--this.functionTimer[7]<=0)
					{
						this.functionTimer[7]=-1;
						this.timedFunction7();
					}
				}
				if(this.functionTimer[8]>0)
				{
					if(--this.functionTimer[8]<=0)
					{
						this.functionTimer[8]=-1;
						this.timedFunction8();
					}
				}
				if(this.functionTimer[9]>0)
				{
					if(--this.functionTimer[9]<=0)
					{
						this.functionTimer[9]=-1;
						this.timedFunction9();
					}
				}

				if(this.functionTimer[10]>0)
				{
					if(--this.functionTimer[10]<=0)
					{
						this.functionTimer[10]=-1;
						this.timedFunction10();
					}
				}
			};
			
			//mainly for foreground and light source objects
			GPE_GameObject.prototype.render_above = function ()
			{

			};
			
			GPE_GameObject.prototype.render_under = function ()
			{

			};

			GPE_GameObject.prototype.render_hud = function ()
			{

			};
			
			GPE_GameObject.prototype.render_self = function ()
			{
				if (gpe.preload_completed() && this.spriteId >=0 )
				{
					gpe.OBJECT_RENDER_CALLS+=1;
					gpe.render_sprite(this.spriteId, this.spriteFrame,this.xPos, this.yPos,1,1);
				}
				else if( GPE_DRAW_OBJECT_BOXES )
				{
					gpe.render_rectangle(this.collisionBox.xPos,this.collisionBox.yPos,this.collisionBox.x2Pos,this.collisionBox.y2Pos,'blue',true,1);	
				}
				gpe.render_calls++;
			};
			
			GPE_GameObject.prototype.scene_end = function()
			{
			
			};
			
			GPE_GameObject.prototype.scene_start = function()
			{
			
			};
			
			GPE_GameObject.prototype.self_destruct = function ()
			{
				if( this.gpeIsBeingDestroyed==false )
				{
					this.self_obliterate();
					this.gpeIsBeingDestroyed = true;
					GPR.add_object_to_deletion_list( this );
					var myObjectList = GPR.GAME_OBJECTS[this.gpeObjectType];
					var tempObj = IS_NULL;
					
					var matchingLayer = IS_NULL;
					var foundLayerMatch = false;
					for( var iCLayers = GPR.currentScene.sceneLayers.length-1; iCLayers >=0; iCLayers--)
					{
						matchingLayer = GPR.currentScene.sceneLayers[iCLayers];
						if( matchingLayer.layerId == this.sceneLayerPos)
						{
							foundLayerMatch = true;
							break;
						}
					
					}
					if( foundLayerMatch )
					{
						matchingLayer.remove_object( this );
					}
					else
					{
						console.log("unable to find matching layer to delete from...");
					}
					
					for( var iObj = myObjectList.length-1; iObj >=0; iObj--)
					{
						tempObj = myObjectList[iObj];
						if( tempObj.gpeUniqGameObjectId == this.gpeUniqGameObjectId )
						{
							 myObjectList.splice(iObj,1);
							return;
						}
					}
					
				}
			};
			
			GPE_GameObject.prototype.set_depth = function (newDepth)
			{
				if( this.depth!= newDepth)
				{
					
				}
			};
			
			GPE_GameObject.prototype.self_obliterate = function ()
			{
				
			};
			/**
			
			*/
			GPE_GameObject.prototype.set_sprite = function(nextSprite )
			{
				if( typeof this.spriteId=="number" )
				{
					if( this.spriteId!=nextSprite)
					{
						this.spriteId = nextSprite;
						this.spriteFrame = 0;
					
						if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
						{
							var fSprite = gpe.rsm.rSprites[this.spriteId];
							this.collisionBox.update_shape(this.xPos+fSprite.colX, this.yPos+fSprite.colY , fSprite.colWidth,fSprite.colHeight );
							this.x2Pos = this.xPos+fSprite.colX+fSprite.colWidth;
							this.y2Pos = this.yPos+fSprite.colY+fSprite.colHeight;
						}
						else
						{
							this.collisionBox.update_shape( this.xPos ,this.yPos,32,32 );
							this.x2Pos = this.xPos+32;
							this.y2Pos = this.yPos+32;	
						}
						this.x2Past = this.x2Pos;
						this.y2Past = this.y2Pos;							
						GPR.update_object_in_map(this);
					}
				}
			};
			
			GPE_GameObject.prototype.setx = function ( newX)
			{
				if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
				{
					var fSprite = gpe.rsm.rSprites[this.spriteId];
					this.collisionBox.set_x( newX+fSprite.colX);
					this.x2Pos = this.xPos+fSprite.colX+fSprite.colWidth;
				}
				else
				{
					this.collisionBox.set_x( newX);
				}
				this.xPos = newX;
				this.x2Pos = newX+32;
				GPR.update_object_in_map(this);
			};
			
			GPE_GameObject.prototype.sety = function ( newY)
			{
				if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
				{
					var fSprite = gpe.rsm.rSprites[this.spriteId];
					this.collisionBox.set_y( newX+fSprite.colY);
					this.y2Pos = this.yPos+fSprite.colY+fSprite.colHeight;
				}
				else
				{
					this.collisionBox.set_y( newY);
					this.y2Pos = newY+32;
				}
				this.yPos = newY;
				GPR.update_object_in_map(this);
			};
			
			GPE_GameObject.prototype.speed_move = function ()
			{
				//GPR.update_object_in_map(this);
			};
			
			GPE_GameObject.prototype.set_velocity = function( newDir, newSpeed )
			{
				if( newDir==-0)
				{
					newDir = 0;
				}
				if( typeof newDir=="number")
				{
					this.direction = newDir;
					if( typeof newSpeed=="number")
					{
						this.speed = newSpeed;
					}
				}
			};
			
			GPE_GameObject.prototype.start_countdown = function( timerId, countDownStart)
			{
				if( timerId >=0 && timerId <= 10 && typeof countDownStart =="number")
				{
					this.functionTimer[timerId] = countDownStart;
				}
			};
			
			GPE_GameObject.prototype.set_countdown = function( timerId, countDownStart)
			{
				if( timerId >=0 && timerId <= 10 && typeof countDownStart =="number")
				{
					this.functionTimer[timerId] = countDownStart;
				}
			};
			
			GPE_GameObject.prototype.start_frame_logic = function()
			{
			
			};
			
			GPE_GameObject.prototype.set_timer = function( timerId, secondsToCount)
			{
				if( timerId >=0 && timerId <= 10 && typeof secondsToCount =="number")
				{
					this.functionTimer[timerId] = secondsToCount*60;
				}
			};
			GPE_GameObject.prototype.start_timer = function( timerId, secondsToCount)
			{
				if( timerId >=0 && timerId <= 10 && typeof secondsToCount =="number")
				{
					this.functionTimer[timerId] = secondsToCount*60;
				}
			};
			
			
			GPE_GameObject.prototype.update_animation = function (evt)
			{
				if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
				{
					var fSprite = gpe.rsm.rSprites[this.spriteId];
					if( fSprite.frameCount > 1)
					{
						this.spriteFrame += this.imageSpeed;
						if (this.spriteFrame >= fSprite.frameCount-1)
						{
							this.spriteFrame = 0;
						}
						if( this.spriteFrame < 0 )
						{
							this.spriteFrame = fSprite.frameCount - 2;
						}
					}
				}
			};
			
			GPE_GameObject.prototype.under_mouse = function ( cameraId)
			{
				if( typeof cameraId=="undefined")
				{
					var cameraId = 0;
				}
				if( cameraId >=0 && cameraId < GPE.maxCameras )
				{
					
				}
			};
			
			GPE_GameObject.prototype.update_cords = function (newX, newY)
			{
				this.xPos = newX;
				this.yPos = newY;
				if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
				{
					var fSprite = gpe.rsm.rSprites[this.spriteId];
					this.collisionBox.update_position(newX+fSprite.colX,newY+fSprite.colY);
					this.collisionBox.update_size(fSprite.colWidth,fSprite.colHeight);
					this.x2Pos = this.xPos+fSprite.colX+fSprite.colWidth;
					this.y2Pos = this.yPos+fSprite.colY+fSprite.colHeight;
				}
				else
				{
					this.collisionBox.update_position(newX,newY);
					this.collisionBox.update_position(32,32);
					this.x2Pos = newX+32;
					this.y2Pos = newY+32;
				}
				GPR.update_object_in_map(this);
			};
				
			GPE_GameObject.prototype.touches_tile = function( layerToCheck )
			{
				return GPR.find_tile_in_box( layerToCheck, this.xPos, this.yPos, this.x2Pos, this.y2Pos )
			}

			GPE_GameObject.prototype.touches_certain_tile = function( layerToCheck, needleTile)
			{
				return GPR.find_matching_tile_in_box( layerToCheck, this.xPos, this.yPos, this.x2Pos, this.y2Pos, needleTile );
			}

			return GPE_GameObject;
		}());
		
		this.SuperSpatialPartition = (function () 
		{
			function SuperSpatialPartition()
			{
				this.internalArray = [];
				this.heldObjectsArray = [];
				this.objectsHeld = 0;
				this.boxArea = new GPE_Rect();
				this.spaceName = "CAS_X2015_";
				for( var ii= 0; ii < GPE_MAX_OBJECT_TYPES; ii++)
				{
					this.heldObjectsArray[ii] = [];
				}
			}
			
			SuperSpatialPartition.prototype.add_object = function(newObject)
			{
				this.internalArray[this.objectsHeld] = newObject;
				this.heldObjectsArray[newObject.gpeObjectType].push_back( newObject );
				this.objectsHeld+=1;
			}
			
			SuperSpatialPartition.prototype.empty_list = function()
			{
				this.internalArray.length = 0;
				this.objectsHeld = 0;
				for( var ii= 0; ii < GPE_MAX_OBJECT_TYPES; ii++)
				{
					this.heldObjectsArray[ii] = [];
				}
			}
			
			SuperSpatialPartition.prototype.remove_held_object = function(objectIn)
			{
				if( typeof objectIn!="undefined" )
				{	
					var foundHeldObject = IS_NULL;
					var foundSubHeldObject = IS_NULL;
					for( var ii = this.internalArray.length-1;  ii>=0; ii--)
					{
						foundHeldObject = this.internalArray[ii];
						if( foundHeldObject.gpeUniqGameObjectId==objectIn.gpeUniqGameObjectId )
						{
							this.internalArray.splice(ii,1);
							
							this.objectsHeld-=1;
						}
					}
					var subHeldArray = this.heldObjectsArray[objectIn.gpeObjectType];
					for( var jj = subHeldArray.length-1; jj>=0; jj--)
					{
						foundSubHeldObject = subHeldArray[jj];
						if( foundSubHeldObject.gpeUniqGameObjectId==objectIn.gpeUniqGameObjectId)
						{
							this.heldObjectsArray[objectIn.gpeObjectType].splice(jj,1);
						}
					}
				}
			}
			
			SuperSpatialPartition.prototype.set_space = function(newX, newY, newW, newHeight)
			{
				this.boxArea.update_shape(newX, newY, newW, newHeight);
			}
			return SuperSpatialPartition;
		}());
		
		//gets the last object of theObjectType
		this.find_last_object = function( theObjectType)
		{
			return GPR.find_last_object(theObjectType);
		};
		
		this.getfps = function()
		{
			return SYS_FPS;
		};
		
		this.get_fps = function()
		{
			return SYS_FPS;
		};
		
		this.get_mouse_x = function()
		{
			return this.MOUSE_SCALED_X;
		};
		
		this.get_mouse_y = function()
		{
			return this.MOUSE_SCALED_Y;
		};
		
		//returns the number of objects of the object type
		this.get_object_count = function( theObjectType)
		{
			return GPR.get_object_count(theObjectType);
		};
		
		//returns the number of objects of the object type
		this.get_object_in_view_count = function( theObjectType)
		{
			//finds the object array
			if(theObjectType>=0 && theObjectType < GPE_MAX_OBJECT_TYPES )
			{
				return GPR.get_object_in_view_count(theObjectType);
			}
			return 0;
		};
				
		this.find_tile = function( layerToCheck, xPos, yPos )
		{
			return GPR.find_tile( layerToCheck, xPos, yPos )
		}
		
		this.find_tile_in_box = function( layerToCheck, x1,  y1,  x2, y2 )
		{
			return GPR.find_tile_in_box( layerToCheck, x1,  y1,  x2, y2 );
		}
		
		this.find_matching_tile = function( layerToCheck,  xPos,  yPos,  tileTypeToSearchFor )
		{
			return GPR.find_matching_tile( layerToCheck,  xPos,  yPos,  tileTypeToSearchFor );
		}
		
		this.find_matching_tile_in_box = function( layerToCheck,  x1,  y1,  x2, y2,  tileTypeToSearchFor )
		{
			return GPR.find_tile_in_box( layerToCheck,  x1,  y1,  x2, y2,  tileTypeToSearchFor );
		}

		//GPE Scene Functions [ BEGIN ]
		this.get_current_scene_id = function()
		{
			return GPR.currentSceneId;
		};
		
		this.get_current_scene_name = function()
		{
			return this.get_scene_name( GPR.currentSceneId );
		};
			
		this.get_scene_height = function(  sceneIdNumberIn )
		{
			if( typeof sceneIdNumberIn === 'number' )
			{
				if( sceneIdNumberIn >= 0 && sceneIdNumberIn < this.gpeScenes.length )
				{
					return this.gpeScenes[sceneIdNumber].sceneHeight;
				}
			}
			return GPR.get_scene_height();
		};
		
		this.get_scene_name = function( sceneIdNumberIn)
		{
			if( typeof sceneIdNumberIn === 'number' )
			{
				if( sceneIdNumberIn >= 0 && sceneIdNumberIn < this.gpeScenes.length )
				{
				return this.gpeScenes[sceneIdNumber].sceneName;
				}
			}
			return this.get_current_scene_name();
		};
		
		this.get_scene_width = function( sceneIdNumberIn)
		{
			if( typeof sceneIdNumberIn === 'number' )
			{
				if( sceneIdNumberIn >= 0 && sceneIdNumberIn < this.gpeScenes.length )
				{
					return this.gpeScenes[sceneIdNumber].sceneWidth;
				}
			}
			return GPR.get_scene_width();
		};
		
		this.scene_enter_named = function( nextSceneName)
		{
			for( var i = this.gpeScenes.length - 1; i >=0; i--)
			{
				if( this.gpeScenes[i].sceneName==nextSceneName)
				{
					GPR.scene_enter(i);
					return true;
				}
			}
			return false;
		};
			
		this.scene_enter = function( nextSceneId )
		{
			return GPR.scene_enter(nextSceneId);
		};
		
		this.scene_enter_next = function()
		{
			return GPR.scene_enter_next();
		};
		
		this.scene_enter_previous = function()
		{
			return GPR.scene_emter_previous();
		};
		
		this.scene_reset = function( hardReset )
		{
			GPR.scene_reset(hardReset);
		};

		//GPE Scene Functions [ END ]
		
		//GPE Sprite Functions [ BEGIN ]
		this.get_sprite_width = function(spriteIdIn)
		{
			if( typeof spriteIdIn=="number" )
			{
				spriteIdIn = spriteIdIn|0;
				if( spriteIdIn >=0 && spriteIdIn < gpe.rsm.rSprites.length )
				{
					//var fSprite = gpe.rsm.rSprites[spriteIdIn];
					//if( fSprite!=-1)
					{
						return gpe.rsm.rSprites[spriteIdIn].spriteWidth;
					}
				}
			}
			return 0;
		};
		
		this.get_sprite_height = function(spriteIdIn)
		{
			if( typeof spriteIdIn=="number" )
			{
				spriteIdIn = spriteIdIn|0;
				if( spriteIdIn >=0 && spriteIdIn < gpe.rsm.rSprites.length )
				{
					//var fSprite = gpe.rsm.rSprites[spriteIdIn];
					//if( fSprite!=-1)
					{
						return gpe.rsm.rSprites[spriteIdIn].spriteHeight;
					}
				}
			}
			return 0;
		};
		
		this.get_sprite_length = function(spriteIdIn)
		{
			if( typeof spriteIdIn=="number" )
			{
				spriteIdIn = spriteIdIn|0;
				if( spriteIdIn >=0 && spriteIdIn < gpe.rsm.rSprites.length )
				{
					//var fSprite = gpe.rsm.rSprites[spriteIdIn];
					//if( fSprite!=-1)
					{
						return gpe.rsm.rSprites[spriteIdIn].frameCount;
					}
				}
			}
			return 0;
		};
		
		this.get_sprite_object = function(spriteIdIn)
		{
			if( typeof spriteIdIn=="number" )
			{
				spriteIdIn = spriteIdIn|0;
				if( spriteIdIn >=0 && spriteIdIn < gpe.rsm.rSprites.length )
				{
					//var fSprite = gpe.rsm.rSprites[spriteIdIn];
					//if( fSprite!=-1)
					{
						return gpe.rsm.rSprites[spriteIdIn];
					}
				}
			}
			return IS_NULL;
		};
		
		//GPE Sprite Functions [ END ]

		//GPE Texture Functions [ BEGIN ]

		this.get_texture_width = function(textureIdIn)
		{
			if( typeof textureIdIn=="number" )
			{
				textureIdIn = textureIdIn|0;
				if( textureIdIn >=0 && textureIdIn < gpe.rsm.rSprites.length )
				{
					//var fTexture = gpe.rsm.rSprites[textureIdIn];
					//if( fTexture!=-1)
					{
						return gpe.rsm.rTextures[textureIdIn].texWid;
					}
				}
			}
			return 0;
		};
		
		this.get_texture_height = function(textureIdIn)
		{
			if( typeof textureIdIn=="number" )
			{
				textureIdIn = textureIdIn|0;
				if( textureIdIn >=0 && textureIdIn < gpe.rsm.rSprites.length )
				{
					//var fTexture = gpe.rsm.rSprites[textureIdIn];
					//if( fTexture!=-1)
					{
						return gpe.rsm.rTextures[textureIdIn].texHeight;
					}
				}
			}
			return 0;
		};
		
		//GPE Texture Functions [ END ]
		
		//gets the last object of theObjectType
		this.find_newest_object = function( theObjectType)
		{
			return GPR.find_newest_object(theObjectType);
		};
		
		//returns the first object of otherObjectType that objOne collides with if it is at (xIn,yIn)
		this.find_collision_rectangle = function(  objOne,  xIn,  yIn,  otherObjectType, layerId)
		{
			return GPR.find_collision_rectangle(objOne,  xIn,  yIn,  otherObjectType, layerId);
		};
		
		this.game_reset = function()
		{
			GPR.game_reset();
			location.reload();
		}
		
		this.is_audio_playing = function(audioIdIn)
		{
			if(typeof audioIdIn!="undefined")
			{
				if( audioIdIn>=0 && audioIdIn < this.rsm.rAudio.length )
				{
					var fAudio = this.rsm.rAudio[audioIdIn|0];
					if( fAudio!=-1)
					{
						return fAudio.check_is_playing();
					}
				}
				return false;
			}
		};
		
		this.is_video_playing = function(videoIdIn)
		{
			if(typeof videoIdIn!="undefined")
			{
				if( videoIdIn>=0 && videoIdIn < this.rsm.rVideos.length )
				{
					var fVideo = this.rsm.rVideos[videoIdIn];
					if( fVideo!=-1)
					{
						return fVideo.check_is_playing();
					}
				}
				return false;
			}
		};
		
		this.loop_last_track = function()
		{
			//console.log("Looping ["+gpe.lastLoopedTrack+"]....");
			gpe.play_audio(gpe.lastLoopedTrack,true);
		};
		
		this.loop_last_video = function()
		{
			//console.log("Looping ["+gpe.lastLoopedTrack+"]....");
			gpe.play_video(gpe.lastLoopedVideo,true);
		};
		
		this.make_array_unique = function(arr)
		{
			return arr.reduce(function(p, c) 
			{
			if (p.indexOf(c) < 0) p.push(c);
			return p;
			}, []);
		};
			
		this.move_camera = function(cameraId, newX, newY)
		{
			GPR.move_camera(cameraId, newX, newY);
		};
		
		this.pause_audio = function(audioIdIn)
		{
			if(typeof audioIdIn!="undefined")
			{
				if( audioIdIn>=0 && audioIdIn < this.rsm.rAudio.length )
				{
					var fAudio = this.rsm.rAudio[audioIdIn|0];
					if( fAudio!=-1)
					{
						fAudio.pause_audio();
					}
				}
			}
		};
		
		this.pause_all_audio = function()
		{
			for(var fAudioId = this.rsm.rAudio.length-1; fAudioId>=0; fAudioId-- )
			{
				var fAudio = this.rsm.rAudio[fAudioId];
				if( fAudio!=-1)
				{
					fAudio.pause_audio();
				}
			}
		};
		
		this.loop_audio = function( audioIdIn)
		{
			if(typeof audioIdIn!="undefined")
			{
				if( audioIdIn>=0 && audioIdIn < this.rsm.rAudio.length )
				{
					var fAudio = this.rsm.rAudio[audioIdIn|0];
					if( fAudio!=-1)
					{
						fAudio.play_audio(true);
					}
				}
			}
		}
		
		this.play_audio = function(audioIdIn,loops)
		{
			if(typeof loops=="undefined")
			{
				var loops = false
			}
			if(typeof audioIdIn!="undefined")
			{
				if( audioIdIn>=0 && audioIdIn < this.rsm.rAudio.length )
				{
					var fAudio = this.rsm.rAudio[audioIdIn|0];
					if( fAudio!=-1)
					{
						fAudio.play_audio(loops);
					}
				}
			}
		};
		
		this.pause_audio_group = function(audioGroupId)
		{
			if(typeof audioGroupId!="undefined")
			{
				for(var fAudioId = this.rsm.rAudio.length-1; fAudioId>=0; fAudioId-- )
				{
					var fAudio = this.rsm.rAudio[fAudioId];
					if( fAudio!=-1)
					{
						if( fAudio.audioGroupName == audioGroupId)
						{
							fAudio.pause_audio();
						}
					}
				}
			}
		};
		
		//Video functions
		this.pause_video = function(videoIdIn)
		{
			if(typeof videoIdIn!="undefined")
			{
				if( videoIdIn>=0 && videoIdIn < this.rsm.rVideos.length )
				{
					var fVideo = this.rsm.rVideos[videoIdIn|0];
					if( fVideo!=-1)
					{
						fVideo.pause_video();
					}
				}
			}
		};
		
		this.pause_all_video = function()
		{
			for(var fVideoId = this.rsm.rVideos.length-1; fVideoId>=0; fVideoId-- )
			{
				var fVideo = this.rsm.rVideos[fVideoId];
				if( fVideo!=-1)
				{
					fVideo.pause_video();
				}
			}
		};
		
		this.play_video = function(videoIdIn,loops, isVideoSkippable)
		{
			if(typeof loops=="undefined")
			{
				var loops = false
			}
			if(typeof videoIdIn!="undefined")
			{
				if( videoIdIn>=0 && videoIdIn < this.rsm.rVideos.length )
				{
					var fVideo = this.rsm.rVideos[videoIdIn|0];
					if( fVideo!=-1)
					{
						fVideo.play_video(loops, isVideoSkippable);
					}
				}
			}
		};
		
		this.pause_video_group = function(videoGroupId)
		{
			if(typeof videoGroupId!="undefined")
			{
				for(var fVideoId = this.rsm.rVideos.length-1; fVideoId>=0; fVideoId-- )
				{
					var fVideo = this.rsm.rVideos[fVideoId];
					if( fVideo!=-1)
					{
						if( fVideo.videoGroupName == videoGroupId)
						{
							fVideo.pause_video();
						}
					}
				}
			}
		};
		
		this.position_collides = function(  objOne,  xIn,  yIn,  otherObjectType, layerId )
		{
			return GPR.find_collision_rectangle(objOne,  xIn,  yIn,  otherObjectType, layerId );
		};
		
		//checks if the position is open and free of ANYTHING  or of the otherObjectType
		this.position_open = function(  xIn, yIn, otherObjectType, layerId )
		{
			return GPR.position_open( xIn, yIn, otherObjectType, layerId );
		};
		
		this.point_between = function(pointX, pointY, xMin, yMin, xMax, yMax)
		{
			var pxmin=Math.min(xMin, xMax);
			var pxmax=Math.max(xMin,xMax);
			var pymin=Math.min(yMin,yMax);
			var pymax=Math.max(yMin,yMax);
			if((pointX>pxmin) && (pointX<pxmax) )
			{
				if( (pointY>pymin) && (pointY<pymax) )
				{
					return true;
				}
			}
			return false;
		};
		
		this.point_within = function(pointX, pointY, xMin, yMin, xMax, yMax)
		{
			var pxmin=Math.min(xMin, xMax);
			var pxmax=Math.max(xMin,xMax);
			var pymin=Math.min(yMin,yMax);
			var pymax=Math.max(yMin,yMax);
			if((pointX>=pxmin) && (pointX<=pxmax) )
			{
				if( (pointY>=pymin) && (pointY<=pymax) )
				{
					return true;
				}
			}
			return false;
		};
		
		this.preload_completed = function()
		{
			return this.PRELOAD_COMPLETE;
		};
		
		//GPE Rendering functions
		this.render_circle = function( x, y, radius, circleColor, isOutline, oulineWidth, renderImmediately )
		{
			x-=gpe.CURRENT_VIEW_BOX.xPos;
			y-=gpe.CURRENT_VIEW_BOX.yPos;
			if( radius > 0)
			{
				if( radius < 0)
				{
					radius*=-1;
				}
				
				gpe.GPE_Current_Context.beginPath();
				gpe.GPE_Current_Context.arc(x, y, radius, 0, 2 * Math.PI, false);
				
				if( isOutline==true)
				{
					gpe.GPE_Current_Context.lineWidth = oulineWidth;
					gpe.GPE_Current_Context.strokeStyle = circleColor;
					gpe.GPE_Current_Context.stroke();
				}
				else
				{
					gpe.GPE_Current_Context.fillStyle = circleColor;
					gpe.GPE_Current_Context.fill();
				}
				gpe.GPE_Current_Context.closePath();
			}
		};
		
		this.render_current_video = function()
		{
			if( this.currentVideoPlaying>=0 && this.currentVideoPlaying < this.rsm.rVideos.length )
			{
				var fVideo = this.rsm.rVideos[this.currentVideoPlaying];
				if( fVideo!=-1)
				{
					fVideo.render_video(0,0,GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT);
					//console.log("Is playing video...["+this.currentVideoPlaying+"]");
				}
			}
		};
		
		this.render_line = function( x1, y1, x2, y2, lineColor, lineWidth, renderImmediately)
		{
			x1-=gpe.CURRENT_VIEW_BOX.xPos;
			y1-=gpe.CURRENT_VIEW_BOX.yPos;
			x2-=gpe.CURRENT_VIEW_BOX.xPos;
			y2-=gpe.CURRENT_VIEW_BOX.yPos;
			
			if( typeof lineWidth !="number")
			{
				lineWidth = 1;
			}
			if( lineWidth !=0)
			{
				if( lineWidth < 0)
				{
					lineWidth*=-1;
				}
				gpe.GPE_Current_Context.beginPath();
				gpe.GPE_Current_Context.moveTo(x1, y1);
				gpe.GPE_Current_Context.lineTo(x2, y2);
				gpe.GPE_Current_Context.lineWidth = lineWidth;
				gpe.GPE_Current_Context.strokeStyle = lineColor;
				gpe.GPE_Current_Context.stroke();
			}
		};
		
		this.render_rect = function(rectIn,rectColor, outline,rectLineWidth,renderImmediately)
		{
			if( typeof rectIn=="undefined")
			{
				this.render_rectangle(rectIn.xPos ,rectIn.yPos ,rectIn.x2Pos ,rectin.y2Pos ,rectColor,outline,rectLineWidth);
			}
		};
	
		this.render_rectangle = function(x1, y1, x2, y2,rectColor, outline,rectLineWidth,renderImmediately)
		{
			var xx=Math.min(x1,x2)-gpe.CURRENT_VIEW_BOX.xPos;
			var yy=Math.min(y1,y2)-gpe.CURRENT_VIEW_BOX.yPos;
			var ww=Math.abs(x1-x2);
			var hh=Math.abs(y1-y2);
			if(outline)
			{
				gpe.GPE_Current_Context.beginPath();
				gpe.GPE_Current_Context.rect(xx, yy, ww,hh);
				gpe.GPE_Current_Context.lineWidth = rectLineWidth;
				gpe.GPE_Current_Context.strokeStyle = rectColor;
				gpe.GPE_Current_Context.stroke();
			}
			else
			{
				gpe.GPE_Current_Context.fillStyle = rectColor;
				gpe.GPE_Current_Context.fillRect(xx, yy, ww,hh);
			}
		};
		
		this.render_sprite = function(spriteIdIn, frameNumb, xOn, yOn, scaleX, scaleY)
		{
			if( typeof spriteIdIn=="number" )
			{
				spriteIdIn = spriteIdIn|0;
				if( spriteIdIn >=0 && spriteIdIn < gpe.rsm.rSprites.length )
				{
					//var fSprite = gpe.rsm.rSprites[spriteIdIn];
					//if( fSprite!=-1)
					{
						gpe.rsm.rSprites[spriteIdIn].render_to_context(gpe.GPE_Current_Context, frameNumb, xOn|0, yOn|0, scaleX, scaleY);
					}
				}
			}
		};
		
		this.render_sprite_rotated = function(spriteIdIn, frameNumb, xOn, yOn, rotationAngle,scaleX, scaleY)
		{
			if( spriteIdIn )
			{
				if( spriteIdIn>=0 && spriteIdIn < gpe.rsm.rSprites.length )
				{
					var fSprite = gpe.rsm.rSprites[spriteIdIn|0];
					if( fSprite!=-1)
					{
						if( rotationAngle > 0 && rotationAngle < 360)
						{
							gpe.GPE_Current_Context.save(); 
							gpe.GPE_Current_Context.translate(xOn, yOn);

							gpe.GPE_Current_Context.rotate( rotationAngle * GPE_TO_RADIANS); 
						
							fSprite.render_to_context(gpe.GPE_Current_Context, frameNumb, -(fSprite.get_width()/2),  -(fSprite.get_height()/2), scaleX, scaleY);
							gpe.GPE_Current_Context.restore();
						}
						else
						{
							fSprite.render_to_context(gpe.GPE_Current_Context, frameNumb, xOn, yOn, scaleX, scaleY);
						}
					}
				}
			}
		};
		
		this.render_square = function(xOn, yOn, squareSize,squareColor, outline,squareLineWidth,renderImmediately)
		{
			var xx=yOn-gpe.CURRENT_VIEW_BOX.xPos;
			var yy=xOn-gpe.CURRENT_VIEW_BOX.yPos;
			if( squareSize !=0)
			{
				if( squareSize < 0)
				{
					xx-=squareSize;
					yy-=squareSize;
					squareSize*=-1;
				}
				if(outline)
				{
					gpe.GPE_Current_Context.beginPath();
					gpe.GPE_Current_Context.rect(xx, yy, squareSize,squareSize);
					gpe.GPE_Current_Context.lineWidth = squareLineWidth;
					gpe.GPE_Current_Context.strokeStyle = squareColor;
					gpe.GPE_Current_Context.stroke();
				}
				else
				{
					gpe.GPE_Current_Context.fillStyle = squareColor;
					gpe.GPE_Current_Context.fillRect(xx|0, yy|0, squareSize,squareSize);
				}
			}
		};
		
		this.render_text = function(fontIdIn, xOn, yOn, textToRender, fontRenderColor, fontHalign, fontValign)
		{
			//console.log("Rendering ["+textToRender+"] at ("+xOn+","+yOn+").");
			//console.log("Rendering using font ["+fontIdIn+"].");
			textToRender =""+textToRender;
			if( textToRender.length > 0)
			{
				xOn-=gpe.CURRENT_VIEW_BOX.xPos;
				yOn-=gpe.CURRENT_VIEW_BOX.yPos;
				var fontToRenderFound = false;
				if(typeof fontIdIn!="undefined")
				{
					if( fontIdIn>=0 && fontIdIn < gpe.rsm.rFonts.length )
					{
						var fFont = gpe.rsm.rFonts[fontIdIn|0];
						if( fFont!=-1)
						{
							gpe.GPE_Current_Context.font = fFont.fontCanvasInfo;
							fontToRenderFound = true;
						}
					}
				}
				if( fontToRenderFound==false)
				{
					gpe.GPE_Current_Context.font = "20px Arial";
				}
				
				if( fontHalign==fa_right)
				{
					gpe.GPE_Current_Context.textAlign = 'right';
				}
				else if(fontHalign==fa_center)
				{
					gpe.GPE_Current_Context.textAlign = 'center';
				}
				else
				{
					gpe.GPE_Current_Context.textAlign = 'left';
				}
				
				if( fontValign==fa_bottom)
				{
					gpe.GPE_Current_Context.textBaseline="bottom";
				}
				else if(fontValign==fa_middle)
				{
					gpe.GPE_Current_Context.textBaseline="middle";
				}
				else
				{
					gpe.GPE_Current_Context.textBaseline="top";
				}
				gpe.GPE_Current_Context.fillStyle = fontRenderColor;
				gpe.GPE_Current_Context.fillText(textToRender,xOn|0,yOn|0);
				//console.log("Render should've worked...ish...");
			}
		};
		
		this.render_texture = function(texureIdIn,  xDraw, yDraw)
		{
			if(typeof texureIdIn!="undefined")
			{
				if( texureIdIn>=0 && texureIdIn < gpe.rsm.rTextures.length )
				{
					var fTexture = gpe.rsm.rTextures[texureIdIn|0];
					if( fTexture!=-1)
					{
			
						fTexture.render_to_context(gpe.GPE_Current_Context, xDraw|0, yDraw|0);
					}
				}
			}
		};
		
		this.render_texture_scaled = function(  texureIdIn, xDraw, yDraw, width, height )
		{
			if(typeof texureIdIn!="undefined")
			{
				if( texureIdIn>=0 && texureIdIn < gpe.rsm.rTextures.length )
				{
					var fTexture = gpe.rsm.rTextures[texureIdIn|0];
					if( fTexture!=-1)
					{
						fTexture.render_tex_scaled(gpe.GPE_Current_Context, xDraw|0, yDraw|0);
					}
				}
			}
		};
		
		this.render_texture_clipped = function(  texureIdIn,xDraw, yDraw, clipBox )
		{
			if(typeof texureIdIn!="undefined")
			{
				if( texureIdIn>=0 && texureIdIn < gpe.rsm.rTextures.length )
				{
					var fTexture = gpe.rsm.rTextures[texureIdIn|0];
					if( fTexture!=-1)
					{
						fTexture.render_tex_clipped(gpe.GPE_Current_Context, xDraw|0, yDraw|0,clipBox);
					}
				}
			}
		};
		
		this.render_top_hud = function ()
		{
			if( gpe.isPlayingVideo )
			{
				this.render_current_video();
			}
			
			if( GPE_SETTINGS_IS_DEBUGGING )
			{
				gpe.context.font = 'bold 16pt Calibri';
				gpe.context.textAlign = 'left';
				gpe.context.textAlign = 'left';
				gpe.context.fillStyle = 'red';
				gpe.GPE_Current_Context.fillText(SYS_FPS+" fps ||| Checking "+this.colisionObjectLoops+"times with "+GPR.spatialCollisionChecks+" collision checks", 40, gpe.SCREEN_HEIGHT-32);
			}
		};

		this.render_triangle = function(x1, y1, x2, y2, x3, y3, triangleColor, outline, outlineSize, renderImmediately)
		{
			x1-=gpe.CURRENT_VIEW_BOX.xPos;
			y1-=gpe.CURRENT_VIEW_BOX.yPos;
			x2-=gpe.CURRENT_VIEW_BOX.xPos;
			y2-=gpe.CURRENT_VIEW_BOX.yPos;
			x3-=gpe.CURRENT_VIEW_BOX.xPos;
			y3-=gpe.CURRENT_VIEW_BOX.yPos;
			gpe.GPE_Current_Context.beginPath();
			gpe.GPE_Current_Context.moveTo(x1,y1);
			gpe.GPE_Current_Context.lineTo(x2,y2);
			gpe.GPE_Current_Context.lineTo(x3,y3);
			gpe.GPE_Current_Context.closePath();
			if( outline)
			{
				if( typeof outlineSize=="undefined")
				{
					outlineSize = 5;
				}
				gpe.GPE_Current_Context.lineWidth = outlineSize;
				gpe.GPE_Current_Context.lineJoin = "round";	
				gpe.GPE_Current_Context.strokeStyle = triangleColor;
				gpe.GPE_Current_Context.stroke();
			}
			else
			{
				gpe.GPE_Current_Context.fillStyle = triangleColor;
				gpe.GPE_Current_Context.fill();
			}
		};
		
		this.render_wrapped_text = function(fontIdIn, xOn, yOn, textToRender,fontRenderColor, maxWidth, lineHeight )
		{
			if(typeof fontIdIn!="undefined"&& textToRender.length > 0)
			{
				if( fontIdIn>=0 && fontIdIn < gpe.rsm.rFonts.length )
				{
					var fFont = gpe.rsm.rFonts[fontIdIn];
					if( fFont!=-1)
					{
						xOn-=gpe.CURRENT_VIEW_BOX.xPos;
						yOn-=gpe.CURRENT_VIEW_BOX.yPos;
						gpe.GPE_Current_Context.font = fFont.fontCanvasInfo;
						gpe.GPE_Current_Context.fillStyle = fontRenderColor;
						gpe.GPE_Current_Context.textBaseline="top";
						gpe.GPE_Current_Context.textAlign = 'left';
						var line = '';
						var testLine = '';
						var metrics = '';
						var testWidth = 0;
						var words = textToRender.split(' ');
						for(var n = 0; n < words.length; n++)
						{
							testLine = line + words[n] + ' ';
							metrics = gpe.GPE_Current_Context.measureText(testLine);
							testWidth = metrics.width;
							if (testWidth > maxWidth && n > 0)
							{
								gpe.GPE_Current_Context.fillText(line, xOn, yOn);
								//console.log("[Rendering "+line+"]");
								line = words[n] + ' ';
								yOn += lineHeight;
							}
							else
							{
								line = testLine;
							}
						}
						gpe.GPE_Current_Context.fillText(line, xOn, yOn);
					}
					else
					{
						console.log("Unable to render fontId ["+fontIdIn+"].");
					}
				}
				else
				{
					console.log("FontId ["+fontIdIn+"] out of range.");
				}
			}
			else
			{
				console.log("Unable to render wrapped text ["+textToRender+"] with font ["+fontIdIn+"].");
			}
		};
		
		this.save_data = function( dataIdentifier, dataTitle, actualData)
		{
			if (typeof(Storage) !== "undefined")
			{
				localStorage.setItem(dataIdentifier+"_"+dataTitle,actualData);
			}
		};
			
		//New GPE Background functions added as of Version 1.12 [ BEGIN ] 
		//Replaces all backgrounds whose bgId matches needleBgId with the new newBgId texture on the specific layer
		this.replace_background = function( layerBeingChanged, needleBgId, newBgId )
		{
			GPR.replace_background( layerBeingChanged, needleBgId, newBgId );
		};
		
		//Replaces all backgrounds whose bgId matches needleBgId with the new newBgId texture
		this.replace_all_backgrounds = function( needleBgId, newBgId )
		{
			GPR.replace_all_backgrounds( needleBgId, newBgId );
		};
		
		//Changes the background id/teture of the layer's bgIdth background.
		this.set_background = function( layerBeingChanged, bgId, newBgId )
		{
			GPR.replace_background( layerBeingChanged, bgId, newBgId );
		};
		//New GPE Background functions added as of Version 1.12 [ END ] 

				
		this.setup_camera = function(cameraId, cameraX,cameraY, cameraW, cameraH)
		{
			GPR.setup_camera(cameraId, cameraX, cameraY, cameraW, cameraH);
		};
		
		this.split_screen = function( screenTotal, verticallySplit, useBlanks)
		{
			return GPR.split_screen( screenTotal, verticallySplit, useBlanks);
		};
		
		this.set_render_alpha = function( alphaValue)
		{
			//if( gpe.GPE_Current_Context.globalAlpha!=alphaValue)
			{
				if( alphaValue >0 && alphaValue <255)
				{
					gpe.GPE_Current_Context.globalAlpha = alphaValue/255;
				}
				else if( alphaValue == 0 )
				{
						gpe.GPE_Current_Context.globalAlpha = 0;
				
				}
				else
				{
					gpe.GPE_Current_Context.globalAlpha = 1;
				}
			}
		};
		
		/////
		this.SuperString = (function () 
		{
			function SuperString( startString)
			{
				this.lastAteInt = -1;
				this.lastAteString = -1;
				this.str = "";
				if( typeof startString =="string")
				{
					this.str = startString;
					this.length = this.str.length;
				}
				else
				{
					this.length = IS_NULL;
				}
				
				this.j_str = function()
				{
					return this.str;
				}
				
				
				this.js_str = function()
				{
					return this.str;
				}
				
				this.empty = function()
				{
					if( this.str.length==0)
					{
						return true;
					}
					return false;
				}
				
				this.update_string = function(newString)
				{
					if( typeof newString =="string")
					{
						this.str = newString;
						this.length = this.str.length;
					}
					else
					{
						this.str = "";
					}
				}
				
				this.size = function()
				{
					return length;
				}
			}
			return SuperString;
		}());
		////
	
		this.SuperVector = (function () 
		{
			function SuperVector()
			{
				this.internalArray = [];
				this.vectorLength = 0;
			}
			
			SuperVector.prototype.assign = function(listPos,newValue)
			{
				if( listPos < this.internalArray.length)
				{
					this.internalArray[listPos] = newValue
				}
			}
			
			SuperVector.prototype.at = function(atPos)
			{
				if( atPos < this.vectorLength )
				{
					return this.internalArray[atPos];
				}
				return -1;
			}

			SuperVector.prototype.back = function()
			{
				if( this.vectorLength > 0 )
				{
					return this.internalArray[this.vectorLength-1];
				}
				return -1;
			}

			SuperVector.prototype.clear = function()
			{
				this.internalArray.length = 0;

			}
			
			//Added as of Version 1.12 [ BEGIN ] 
			SuperVector.prototype.insert = function (pos, inValue)
			{
				this.internalArray.insert( pos, inValue );
				this.vectorLength++;
			};
			//Added as of Version 1.12 [ END ] 

			SuperVector.prototype.makeLowercase = function() 
			{
				for (var i = 0; i < this.vectorLength; i++)
				{
					if (typeof this.internalArray[i] === 'string')
					{
						 this.internalArray[i] = this.internalArray[i].toLowerCase();
					 }
				 }
			}

			SuperVector.prototype.makeUppercase = function() 
			{
				for (var i = 0; i < this.vectorLength; i++)
				{
					if (typeof this.internalArray[i] === 'string')
					{
						 this.internalArray[i] = this.internalArray[i].toUpperCase();
					 }
				 }
			}

			SuperVector.prototype.floorValues = function() 
			{
				for (var i = 0; i < this.vectorLength; i++)
				{
					if (typeof this.internalArray[i] === 'number')
					{
						 this.internalArray[i] = this.internalArray[i]|0;
					 }
				 }
			}

			SuperVector.prototype.roundValues = function() 
			{
				for (var i = 0; i < this.vectorLength; i++)
				{
					if (typeof this.internalArray[i] === 'number')
					{
						 this.internalArray[i] = Math.round(this.internalArray[i]);
					 }
				 }
			}

			SuperVector.prototype.roundByDecimals = function(decimalPlaces) 
			{
				var roundByTens = 0;
				decimalPlaces = Math.floor(decimalPlaces);
				if (decimalPlaces >=0 )
				{
					roundByTens = Math.pow(10,decimalPlaces);
					for (var i = 0; i < this.vectorLength; i++)
					{
						if (typeof this.internalArray[i] === 'number')
						{
							 this.internalArray[i] = Math.round(this.internalArray[i]/roundByTens)* roundByTens;
						 }
					 }
				}
				else
				{
					 //handle negative decimal places(.1, .01, .001, etc, etc)
					for (var i = 0; i < this.length; i++)
					{
						roundByTens = Math.pow(10,-1*decimalPlaces);
						if (typeof this[i] === 'number')
						{
							this[i] = roundedBeforeDecimal+ (Math.round(this[i]*roundByTens)/roundByTens);
						 }
					 } 
				}
			}

			SuperVector.prototype.ceilValues = function() 
			{
				for (var i = 0; i < this.vectorLength; i++)
				{
					if (typeof this.internalArray[i] === 'number')
					{
						 // this is a string
						 this.internalArray[i] = Math.ceil(this.internalArray[i]);
					 }
				 }
			}

			SuperVector.prototype.size = function() 
			{
				 return this.vectorLength;
			}

			//emulates the push_back push_front 
			SuperVector.prototype.push_back = function(inValue) 
			{
				 this.internalArray.push(inValue);
				 this.vectorLength = this.internalArray.length;
			}

			SuperVector.prototype.pop_back = function() 
			{
				 var returnVal = this.internalArray.pop();
				 this.vectorLength = this.internalArray.length;
				 return returnVal;
			}
			return SuperVector;
		}());
		////
		
		this.stop_all_audio = function()
		{
			for(var fAudioId = this.rsm.rAudio.length-1; fAudioId>=0; fAudioId-- )
			{
				var fAudio = this.rsm.rAudio[fAudioId];
				if( fAudio!=-1)
				{
					fAudio.stop_audio();
				}
			}	
		};
		
		this.stop_audio_group = function(audioGroupId)
		{
			if(typeof audioGroupId!="undefined")
			{
				for(var fAudioId = this.rsm.rAudio.length-1; fAudioId>=0; fAudioId-- )
				{
					var fAudio = this.rsm.rAudio[fAudioId];
					if( fAudio!=-1)
					{
						if( fAudio.audioGroupName == audioGroupId)
						{
							fAudio.stop_audio();
						}
					}
				}
			}
		};
		
		this.stop_audio = function(audioIdIn)
		{
			if(typeof audioIdIn!="undefined")
			{
				if( audioIdIn >=0 && audioIdIn < this.rsm.rAudio.length )
				{
					var fAudio = this.rsm.rAudio[audioIdIn|0];
					if( fAudio!=-1)
					{
						fAudio.stop_audio();
					}
				}
			}
		};
		
		this.stop_all_video = function()
		{
			for(var fVideoId = this.rsm.rVideos.length-1; fVideoId>=0; fVideoId-- )
			{
				var fVideo = this.rsm.rVideos[fVideoId];
				if( fVideo!=-1)
				{
					fVideo.stop_video();
				}
			}	
		};
		
		this.stop_video_group = function(videoGroupId)
		{
			if(typeof videoGroupId!="undefined")
			{
				for(var fVideoId = this.rsm.rVideos.length-1; fVideoId>=0; fVideoId-- )
				{
					var fVideo = this.rsm.rVideos[fVideoId];
					if( fVideo!=-1)
					{
						if( fVideo.videoGroupName == videoGroupId)
						{
							fVideo.stop_video();
						}
					}
				}
			}
		};
		
		this.stop_last_video = function()
		{
			this.isPlayingVideo = false;
		};
		
		this.stop_video = function(videoIdIn)
		{
			if(typeof videoIdIn!="undefined")
			{
				if( videoIdIn >=0 && videoIdIn < this.rsm.rVideos.length )
				{
					var fVideo = this.rsm.rVideos[videoIdIn|0];
					if( fVideo!=-1)
					{
						fVideo.stop_video();
					}
				}
			}
		};
		
		this.to_bool = function(strToProcess)
		{
			if( typeof strToProcess !="undefined")
			{
				if( typeof strToProcess=="string")
				{
					if (strToProcess == "true") return true;
					if (strToProcess == "yes") return true;
					if (strToProcess == "1") return true;
					if (strToProcess == "false") return false;
					if (strToProcess == "no") return false;
					if (strToProcess == "0") return false;
				}
				else if( typeof strToProcess =="number")
				{
					if( strToProcess==1)
					{
						return true;
					}
				}
			}
			return false;
		}
		
		this.touches_object = function(  objOne,  xIn,  yIn,  otherObjectType, layerId)
		{
			return GPR.find_collision_rectangle(objOne,  xIn,  yIn,  otherObjectType, layerId);
		};
		
		this.touches_family = function(  objOne,  xIn,  yIn,  familyToSearch, layerId)
		{
			return GPR.touches_family(objOne,  xIn,  yIn,  familyToSearch, layerId);
		};
		
		this.handle_key_down = function (evt)
		{
			if (typeof evt =="undefined")
			{
				var evt = window.event;
			}
			gpe.inputController.handle_key_down(evt);
		};
		
		this.handle_key_press = function (evt)
		{
			if (typeof evt =="undefined")
			{
				var evt = window.event;
			}
			gpe.inputController.handle_key_press(evt);
		};
		
		this.handle_key_up = function (evt)
		{
			if (typeof evt =="undefined")
			{
				var evt = window.event;
			}
			gpe.inputController.handle_key_up(evt);
		};
		
		this.update_mouse_down = function (evt)
		{
			if (typeof evt =="undefined")
			{
				var evt = window.event;
			}
			gpe.inputController.handle_mouse_press(evt);
		}
		
		this.update_game_mouse = function (evt)
		{
			if (typeof evt =="undefined")
			{
			//	var evt = window.event;
				console.log('undefined event for mouse....');
			}
			
			var rect = gpe.canvas.getBoundingClientRect();
			gpe.MOUSE_X = (evt.clientX-rect.left)/(rect.right-rect.left)*(gpe.canvas.width+2);
			gpe.MOUSE_Y = (evt.clientY-rect.top)/(rect.bottom-rect.top)*(gpe.canvas.height+2);
			if( gpe.masterScaleX ==0)
			{
				gpe.masterScaleX  = 1;
			}
			if( gpe.masterScaleY ==0)
			{
				gpe.masterScaleY  = 1;
			}
			
			if( GPE.masterScaleX<=1)
			{
				gpe.MOUSE_SCALED_X =  1/gpe.masterScaleX * gpe.MOUSE_X;
			}
			else if( GPE.MOUSE_X!=0)
			{
				gpe.MOUSE_SCALED_X = gpe.MOUSE_X / gpe.masterScaleX;
			}
			else
			{
				gpe.MOUSE_SCALED_X = 0;
			}
			
			if( GPE.masterScaleY<=1)
			{
				gpe.MOUSE_SCALED_Y = 1/gpe.masterScaleY * gpe.MOUSE_Y;
			}
			else if( GPE.MOUSE_Y!=0)
			{
				gpe.MOUSE_SCALED_Y = gpe.MOUSE_Y / gpe.masterScaleY;
			}
			else
			{
				gpe.MOUSE_SCALED_Y = 0;
			}
			gpe.MOUSE_X = Math.round( gpe.MOUSE_X);
			gpe.MOUSE_Y = Math.round( gpe.MOUSE_Y);
			//gpe.MOUSE_SCALED_X = Math.round( gpe.MOUSE_SCALED_X);
			//gpe.MOUSE_SCALED_Y = Math.round( gpe.MOUSE_SCALED_Y);
			
			/*
			var rect = gpe.canvas.getBoundingClientRect();
			this.MOUSE_X = Math.round((evt.clientX - rect.left) / (rect.right - rect.left) * gpe.canvas.width);
			this.MOUSE_Y = Math.round((evt.clientY - rect.top) / (rect.bottom - rect.top) * gpe.canvas.height);
			*/
		};
		
		this.reset_game_mouse = function(evt)
		{
			gpe.MOUSE_X = 0;
			gpe.MOUSE_Y = 0;
			
			gpe.MOUSE_SCALED_X = 0;
			gpe.MOUSE_SCALED_Y = 0;
		}
		
		this.update_mouse_down = function (evt)
		{
			if (typeof evt =="undefined")
			{
				var evt = window.event;
			}
			gpe.inputController.handle_mouse_press(evt);
		}
		
		this.update_mouse_up = function (evt)
		{
			if (typeof evt =="undefined")
			{
				var evt = window.event;
			}
			gpe.inputController.handle_mouse_up(evt);
		}
		
		this.video_loop = function(videoIdIn)
		{
			this.play_video(videoIdIn,true);
		}
		
		this.video_play = function(videoIdIn,loops)
		{
			this.play_video(videoIdIn,loops);
		}
		
		this.video_playing = function(videoIdIn)
		{
			return this.is_video_playing(videoIdIn);
		}

		this.video_stop_all = function() 
		{
			this.stop_all_video();
		}
		
		this.gameloop_logic = function()
		{
			if( gpe.rsm!=-1)
			{
				gpe.rsm.check_resources_state();
			}
			
			if( gpe.inputController!=-1)
			{
				gpe.inputController.process_input();
			}
			
			gpe.apply_plugins_logics();
			if( gpe.isPlayingVideo )
			{
				if( gpe.currentVideoPlaying>=0 && gpe.currentVideoPlaying < gpe.rsm.rVideos.length )
				{
					var fVideo = gpe.rsm.rVideos[gpe.currentVideoPlaying];
					if( fVideo!=-1)
					{
						if( gpe.videoPlayingIsSkippable )
						{
							if( gpe.check_keyboard(gp_esc) || gpe.check_keyboard(gp_enter) )
							{
								fVideo.stop_video();
								gpe.exit_video_view_mode();	
							}
						}
						fVideo.process_playing_video();
					}
					else
					{
						gpe.exit_video_view_mode();	
					}
				}
				else
				{
					gpe.exit_video_view_mode();	
				}
			}
			else if(gpe.currentState != 0 )
			{
				//automatically loops audio
				var fAudio = IS_NULL;
				for( var iAudio =gpe.rsm.rAudio.length-1; iAudio >=0;iAudio--)
				{
					fAudio = gpe.rsm.rAudio[iAudio];
					if( fAudio!=-1)
					{
						if( fAudio.isBeingLooped && fAudio.html5AudioSource!=-1)
						{
							if( fAudio.html5AudioSource.ended )
							{
								fAudio.play_audio(true);
							}
						}
					}						
				}
				gpe.currentState.apply_logic();
			}
			
			if( gpe.inputController!=-1)
			{
				gpe.inputController.reset_input();
			}
			
			gpe.apply_plugins_logics();
		}
		
		this.gameloop_render = function()
		{
			if ( gpe.canvasIsEnabled && (GPE_FPS_UNCAPPED || GPE_STANDARD_DELTA > GPE_STANDARD_INTERVAL) )
			{
				gpe.OBJECT_RENDER_CHECKS = 0;
				gpe.OBJECT_RENDER_CALLS = 0;
			
				GPE_STANDARD_THEN = GPE_STANDARD_NOW - (GPE_STANDARD_DELTA % GPE_STANDARD_INTERVAL);
         
				gpe.enter_render();
			}
		}
		
		this.gameloop = function()
		{
			GPE_STANDARD_NOW = Date.now();
			GPE_STANDARD_DELTA = GPE_STANDARD_NOW - GPE_STANDARD_THEN;
		 
			gpe.gameloop_logic();
				
			change_state();
			gpe.gameloop_render();
				
			window.requestAnimationFrame(gpe.gameloop );
		};
		
		//sets the 1st program state
		//Assigning this to that and passing it into a function is an easy way to avoid any scope issues.
		var that = this;
		
		//collision variables
		//Debugging variables
		this.collisionsCheckedCount = 0;
		this.colisionObjectLoops = 0;
	};

	function start()
	{
		// Run your javascript code here
		gpe = GPE = new GPE_GameController(GPE_SETTINGS_SCREEN_WIDTH, GPE_SETTINGS_SCREEN_HEIGHT,854,480,GPE_SETTINGS_SYSTEM_OS);
		gpe.canvas = document.getElementById('gpeCanvas');
		gpe.masterScaleX = 1;
		gpe.masterScaleY = 1;
		//processes the mouse input within the canvas
		var eventObj = 0;
		if( typeof event =="undefined")
		{
			eventObj = window.event;
		}
		else
		{
			eventObj = event;
		}
		
		window.addEventListener("resize", gpe.handle_canvas_size, true);
		document.addEventListener('keydown',  gpe.handle_key_down, false);
        document.addEventListener('keypress', gpe.handle_key_press, false);
		document.addEventListener('keyup',  gpe.handle_key_up,false );
		
		gpe.canvas.addEventListener('mousedown',  gpe.update_mouse_down, false);
        gpe.canvas.addEventListener('mouseup', gpe.update_mouse_up, false);
		gpe.canvas.addEventListener('mousemove', gpe.update_game_mouse ,false);
		gpe.canvas.addEventListener('mouseout', gpe.reset_game_mouse ,false);
		gpe.canvas.addEventListener('mouseleave', gpe.reset_game_mouse ,false);
		gpe.context = gpe.canvas.getContext('2d',{
					antialias: '2x',
					stencil: false,
					format: 'rgba8888',
					graphicsMemory: true,
					dirty: true
				});
		gpe.GPE_Current_Context = gpe.context;
		gpe.context.rect(0,0,gpe.context.width, gpe.context.height);
		gpe.context.save();
		gpe.inputController = new GPE_InputManager();
		gpe.rsm = new ResourceController();
		
		for( var iCam = 0; iCam < gpe.maxCameras; iCam++ )
		{
			gpe.EXTRA_CANVASES[iCam] = gpe.add_canvas("exCamera"+iCam,gpe.SCREEN_WIDTH,gpe.SCREEN_HEIGHT);
			gpe.EXTRA_CANVASES[iCam].style.visibility = "hidden";
			gpe.EXTRA_CANVASES[iCam].style.display = "none";
			gpe.EXTRA_CONTEXTS[iCam] = gpe.EXTRA_CANVASES[iCam].getContext('2d',{
						antialias: '2x',
						stencil: false,
						format: 'rgba8888',
						graphicsMemory: true,
						dirty: true
					});
		}
		
		gpe.mapCanvas = gpe.add_canvas("gameMap",GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT);
		gpe.mapCanvas.className = "hide";
		
		gpe.preGuiCanvas = gpe.add_canvas("preGuiCanvas",GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT);
		gpe.preGuiCanvas.className = "hide";
		
		gpe.fileReader = new GPE_FileReader();
		
		programStateId = programStates.STATE_LOGO_DISPLAY;
		gpe.currentState = new GPE_LogoDisplay();		
		GPR = gpe.currentState;
		gpe.handle_canvas_size();
	}


	function init_gpe()
	{
		start();
		window.requestAnimationFrame( gpe.gameloop );
	}
	

	/**
	* The GPE_Rect Class.
	*/
	function GPE_Rect()
	{
		this.xPos = 0;
		this.yPos = 0;
		this.xCenter = 0;
		this.yMiddle = 0;
		this.rectWidth = 0;
		this.rectHeight = 0;
		this.x2Pos = 0;
		this.y2Pos = 0;
		
		this.add_x = function( xPlus)
		{
			this.xPos+=xPlus
			this.x2Pos+=xPlus;
			this.xCenter+=xPlus;
		}
		
		this.add_y = function( yPlus)
		{
			this.yPos+=yPlus;
			this.y2Pos+=yPlus;
			this.yMiddle+=yPlus;
		}
		
		this.get_center = function()
		{
			return this.xCenter
		};
		
		this.get_height = function()
		{
			return this.rectHeight;
		};
		
		this.get_middle = function()
		{
			return this.yMiddle;
		};

		this.get_width = function()
		{
			return this.rectWidth;
		};
		
		this.get_x = function()
		{
			return this.xPos;
		};
		
		this.get_x2 = function()
		{
			return this.x2Pos;
		};
		
		this.get_y = function()
		{
			return this.yPos;
		};
		
		this.get_y2 = function()
		{
			return this.y2Pos;
		};
		
		this.set_x = function( newX)
		{
			this.xPos = newX;
			this.x2Pos = this.xPos+this.rectWidth;
			this.xCenter = newX+this.rectWidth/2;
			this.xCenter = this.xPos+this.rectWidth/2;
		};
		
		this.set_y = function( newY )
		{
			this.yPos = newY;
			this.y2Pos = this.yPos+this.rectHeight;
			this.yMiddle = this.yPos+this.rectHeight/2;
		};
		
		this.update_position = function ( xNew, yNew)
		{
			this.xPos = xNew;
			this.yPos = yNew;
			this.x2Pos = this.xPos+this.rectWidth;
			this.y2Pos = this.yPos+this.rectHeight;
			this.xCenter = this.xPos+this.rectWidth/2;
			this.yMiddle = this.yPos+this.rectHeight/2;
		};
		
		this.update_box = function ( xNew, yNew, widNew, hgtNew)
		{
			this.update_shape(xNew, yNew, widNew, hgtNew );
		}
		
		this.update_shape = function ( xNew, yNew, widNew, hgtNew)
		{
			if( typeof xNew =="number" && typeof yNew =="number" && typeof widNew =="number" && typeof hgtNew =="number" )
			{
				this.xPos = xNew;
				this.yPos = yNew;
				this.rectWidth = widNew;
				this.rectHeight = hgtNew;
				
				if( widNew < 0)
				{
					this.xPos= xNew-widNew;
					this.rectWidth=Math.abs(widNew);
					if( GPE_SETTINGS_IS_DEBUGGING )
					console.log( this.rectWidth);
				}
				
				if( hgtNew < 0)
				{
					this.yPos= yNew-hgtNew;
					this.rectHeight=Math.abs(hgtNew);
					if( GPE_SETTINGS_IS_DEBUGGING )
					console.log( this.rectHeigh);
				}
				this.x2Pos = this.xPos+this.rectWidth;
				this.y2Pos = this.yPos+this.rectHeight;
					
				this.xCenter = this.xPos+this.rectWidth/2;
				this.yMiddle = this.yPos+this.rectHeight/2;
			}
		};
		
		this.update_size = function ( widNew, hgtNew)
		{
			this.rectWidth = widNew;
			this.rectHeight = hgtNew;
			if( widNew < 0)
			{
				this.xPos= xNew-widNew;
				this.rectWidth=Math.abs(widNew);
			}
			
			if( hgtNew < 0)
			{
				this.yPos= yNew-hgtNew;
				this.rectHeight=Math.abs(hgtNew);
			}
			
			this.x2Pos = this.xPos+this.rectWidth;
			this.y2Pos = this.yPos+this.rectHeight;
			this.xCenter = this.xPos+this.rectWidth/2;
			this.yMiddle = this.yPos+this.rectHeight/2;
		};
		
	}

	/**
	* The GPE_GameSprite Class.
	*/
	function GPE_GameSprite()
	{
		this.imageObj;
		this.frameCount = 0;
		this.maxSpriteFrames = 0;
		this.spriteName = "";
		this.setId = -1;
		this.gpeId = -1;
		this.xOrg = 0;
		this.yOrg = 0;
		this.spriteWidth = 1;
		this.spriteHeight = 1;
		this.colX = 0;
		this.colY = 0;
		this.colWidth = 1;
		this.colHeight = 1;
		this.colRadius = 1;
		this.colShape = 0;
		//colshapes: rectangle =  0,default, circle = 1, others to be implemented soonISH
		this.imageWidth = 0;
		this.imageHeight = 0;
		
		this.frameRect = [];

		this.get_height = function ()
		{
			return this.spriteHeight;
		}
		
		this.get_width = function ()
		{
			return this.spriteWidth;
		}
		
		this.update_sprite_data = function (usedImage, xStart, yStart,frameWidth, frameHeight)
		{
			this.imageWidth = usedImage.naturalWidth;
			this.imageHeight = usedImage.naturalHeight;
			if( GPE_SETTINGS_IS_DEBUGGING )
			console.log( "["+this.setId+"]Size: "+this.imageWidth+" , "+this.imageHeight+".");
			var countedFrames = 0;
			this.spriteWidth =  this.colWidth = frameWidth;
			this.spriteHeight = this.colHeight = frameHeight;
			this.frameCount = 0;
			this.imageObj = usedImage;
			
			var spWid = 0;
			var spHeight = 0;
			var sprY = 0;
			var sprX = 0;
			for( sprY = yStart; sprY < this.imageHeight;sprY+=frameHeight)
			{
				for( sprX = xStart; sprX < this.imageWidth;sprX+=frameWidth)
				{
					if( this.frameCount < this.maxSpriteFrames || this.maxSpriteFrames <= 0 )
					{
						var newRect = new GPE_Rect();
						spWid = frameWidth;
						spHeight = frameHeight;
						if( sprX+frameWidth > this.imageWidth)
						{
							spWid = this.imageWidth - sprX;
						}
						if( sprY+frameHeight > this.imageHeight)
						{
							spHeight = this.imageHeight - spHeight;
						}
						newRect.update_shape(sprX,sprY,spWid, spHeight);
						this.frameRect[countedFrames] = newRect;
						countedFrames+=1;
						this.frameCount += 1;
					}
				}
			}
		};

		this.render_to_context = function (contextTo, frameNumb, xOn, yOn, scaleX, scaleY)
		{
			xOn-=gpe.CURRENT_VIEW_BOX.xPos;
			yOn-=gpe.CURRENT_VIEW_BOX.yPos;
			if( typeof scaleX=="undefined" )
			{
				var scaleX = 1;
			}
			if( typeof scaleY=="undefined" )
			{
				var scaleY = 1;
			}
			if(frameNumb>= 0 && frameNumb < this.frameRect.length)
			{
				frameNumb = frameNumb|0;
				var frameToRender = this.frameRect[frameNumb];
				if( scaleX==1 && scaleY==1)
				{
					contextTo.drawImage(this.imageObj, frameToRender.get_x(),frameToRender.get_y(), frameToRender.rectWidth,frameToRender.rectHeight, xOn, yOn, frameToRender.rectWidth,frameToRender.rectHeight);
				}
				else if( scaleX < 0 || scaleY < 0)
				{
					var hScale = 1;
					var vScale = 1;
					if( scaleX < 0)
					{
						scaleX*=-1;
						hScale = -1;
					}
					if( scaleY < 0)
					{
						vScale = -1;
						scaleY*=-1;
					}
					contextTo.save();
					contextTo.translate(xOn,yOn);
 
					var half_width = frameToRender.rectWidth/2;
					var half_height = frameToRender.rectHeight/2;
					 
					contextTo.translate(half_width, half_height);
					contextTo.scale(hScale, vScale); 
					contextTo.drawImage(this.imageObj, frameToRender.get_x(),frameToRender.get_y(),
					frameToRender.rectWidth,frameToRender.rectHeight, 
					-half_width, -half_height, frameToRender.rectWidth * scaleX,frameToRender.rectHeight * scaleY);
					contextTo.restore();
				}
				else
				{		
					contextTo.drawImage(this.imageObj, frameToRender.get_x(),frameToRender.get_y(),
					frameToRender.rectWidth,frameToRender.rectHeight, 
					xOn*hScale, yOn*vScale, frameToRender.rectWidth * scaleX,frameToRender.rectHeight * scaleY);
				}
			
			}
		};
	
	}

	function GPE_Tilesheet()
	{
		this.tex = IS_NULL; //the texture aka the image for the tile
		this.id = IS_NULL; //the tile's id
		this.tAmountX = 0;
		this.tAmountY = 0; //the amount of tiles vertically and horizontally in one texture
		this.tWidth = 32;
		this.tHeight = 32; //the tile's width and height
		this.hOff = 0;
		this.vOff = 0; //the tile's horizontal and vertical offset
		this.hSep = 0;
		this.vSep = 0; //the tile's horizontal and vertical separation between tiles
		this.tileStartPos = 1;
		this.tileEndPos = 1;
		this.texIsLoaded = false;
		this.tilesetIsProcessed = false;
		
		this.load_tilesheet_texture = function( fileName,  id)
		{
			this.tex = gpe.rsm.load_new_texture( fileName, id );
		}
		
		this.pixel_from_xpos = function( xPos)
		{
			return this.hOff+xPos*(this.tWidth+this.hSep);
		}
		
		this.pixel_from_ypos = function(yPos)
		{
			return this.vOff+yPos*(this.tHeight+this.vSep);
		}
		
		this.get_xpos_at = function( numb)
		{
			//makes sure the number is smaller than the max number of x Tiles
			var returnVal=0;
			//this.update_tex_specs();
			if(this.tAmountX>0 && numb >=this.tAmountX && numb>0)
			{
				returnVal= (this.hOff+ Math.abs(numb % this.tAmountX) *(this.tWidth+this.hSep) );
			}
			else if(numb>=0)
			{
				returnVal= (this.hOff+ numb*(this.tWidth+this.hSep) );
				//otherwise use basic math to get the x position of tile
			}
			return returnVal;
		}
		
		this.get_ypos_at = function(numb)
		{
			var returnVal=0;
			//this.update_tex_specs();
			if(this.tAmountX!=0 && numb >=this.tAmountX && numb>0 )
			{
				returnVal=this.vOff+( (numb/ this.tAmountX )|0)*(this.tHeight+this.vSep);
			}
			else if(numb==0)
			{
				returnVal=this.vOff;
			}
			return returnVal;
		}
		
		this.get_tile_width = function()
		{
			return this.tWidth;
		}
		
		this.get_tile_height = function()
		{
			return this.tHeight;
		}
		
		this.update_tex_specs = function()
		{
			this.tAmountX = 0;
			this.tAmountY = 0;
			var texXTotal = this.hOff;
			var texYTotal = this.vOff;
			this.tileEndPos=this.tileStartPos;
			if(this.tex!=-1)
			{
				//if( this.tex.check_texture_state() )
				{
					while(texXTotal< this.tex.naturalWidth )
					{
						texXTotal += this.tWidth+this.hSep;
						this.tAmountX+=1;
					}
					while(texYTotal< this.tex.naturalHeight )
					{
						texYTotal += this.tHeight+this.vSep;
						this.tAmountY+=1;
					}
				}
			}
			this.tileEndPos+=this.tAmountX*this.tAmountY;
		}
		
		this.init_tilesheet = function()
		{
			if( this.texIsLoaded==false)
			{
				if( this.tex!=-1)
				{
					this.texIsLoaded = this.tex.check_texture_state();
				}
			}
			return this.texIsLoaded;
		};
		
	}

	function GPE_Texture()
	{
		this.imageSource = IS_NULL;
		this.texWid = 0;
		this.texHeight = 0;
		this.fileLocation = "";
		this.name = "";
		this.texId = IS_NULL;
		this.textureIsFullyLoaded = false;
		this.textureFailedToLoad = false;
		this.get_id = function()
		{
			return texId;
		};
		
		this.check_texture_state = function()
		{
			if( this.imageSource!=-1)
			{
				if( this.textureIsFullyLoaded==false )
				{
					this.textureIsFullyLoaded = IsImageOk(this.imageSource);
					if( this.textureIsFullyLoaded )
					{
						this.texWid = this.imageSource.naturalWidth;
						this.texHeight = this.imageSource.naturalHeight;
					}
					else if( this.imageSource.alt=="failed")
					{
						this.textWid = 0;
						this.texHeight = 0;
						this.textureIsFullyLoaded = true;
						this.textureFailedToLoad = true;
					}
				}
			}
			else
			{
				this.textureIsFullyLoaded=false;
			}
			return this.textureIsFullyLoaded;
		};
		
		this.change_texture = function( newTexture)
		{
			this.imageSource = newTexture;
		};
		
		this.get_width = function()
		{
			return this.texWid;
		};
		
		this.get_height = function()
		{
			return this.texHeight;
		};
		
		this.render_tex = function(  contextTo, xDraw, yDraw )
		{
			if(this.imageSource!=-1 && this.textureFailedToLoad==false )
			{
				if( contextTo!=-1 && this.textureIsFullyLoaded )
				{
					contextTo.drawImage(this.imageSource, xDraw, yDraw);
				}
			}
		};
		
		this.render_tex_scaled = function(  contextTo, xDraw, yDraw, width, height )
		{
			if(this.imageSource!=-1 && this.textureFailedToLoad==false )
			{
				if( contextTo!=-1 && this.textureIsFullyLoaded )
				{
					if( width > 0 && height > 0)
					{
						contextTo.drawImage(this.imageSource, xDraw, yDraw,width, height);	
					}
					else if( width < 0 || height < 0)
					{
						var hScale = 1;
						var vScale = 1;
						if( width < 0)
						{
							width*=-1;
							hScale = -1;
						}
						if( scaleY < 0)
						{
							vScale = -1;
							height*=-1;
						}
						contextTo.save();
						contextTo.translate(xOn,yOn);
	 
						var half_width = width/2;
						var half_height = height/2;
						 
						contextTo.translate(half_width, half_height);
						contextTo.scale(hScale, vScale); 
						contextTo.drawImage(this.imageObj, -half_width, -half_height, width,height);
						contextTo.restore(); 
					
					}
					
				}
			}
		};
		
		this.render_tex_clipped = function(  contextTo, xDraw, yDraw, clipBox )
		{
			if(this.imageSource!=-1)
			{
				if( contextTo!=-1 && this.textureIsFullyLoaded && this.textureFailedToLoad==false  )
				{
					if( typeof clipBox!="undefined")
					{
						contextTo.drawImage(this.imageSource, clipBox.get_x(),clipBox.get_y(), clipBox.get_width(),clipBox.get_height(), xDraw, yDraw, clipBox.get_width(), clipBox.get_height() );
					}
				}
			}
		};
	}

	function GPE_Background()
	{
		this.backgroundTexture = IS_NULL;
		this.bgXOffset = 0;
		this.bgYOffset = 0;
		this.bgXSpeed = 0;
		this.bgYSpeed = 0;
		this.fileLocation = IS_NULL;
		this.bgId = IS_NULL;
		this.bgColor = "black";
		this.renderBgSpeed = false;
		this.loopBGHori = false;
		this.loopBGVert = false;
		
		this.get_scene_background_color = function()
		{
			return this.bgColor;
		}
		
		this.set_background = function(newBG)
		{
			if(  newBG>=0 && newBG < gpe.rsm.rTextures.length )
			{
				newBG.bgId = newBG;
				newBG.backgroundTexture = gpe.rsm.rTextures[newBG ];
			}
			else
			{
				newBG.bgId = IS_NULL;
				newBG.backgroundTexture = IS_NULL;
			}
		}
		
		this.set_background_color = function( colorIn)
		{
			this.bgColor=colorIn;
		};
		
		this.set_bgoffsets = function(nextX, nextY)
		{
			this.bgXOffset = nextX;
			this.bgYOffset = nextY;
		};

		this.set_bgspeed = function( speedX, speedY)
		{
			this.bgXSpeed = speedX;
			this.bgYSpeed = speedY;
		};

		this.scroll_bg = function()
		{
			if(this.backgroundTexture!=-1 && this.stretchBG==false)
			{
				this.bgXOffset +=this.bgXSpeed;
				this.bgYOffset +=this.bgYSpeed;
				//If the background has gone too far
				if(this.loopBGHori)
				{
					 if( this.bgXOffset <= -this.backgroundTexture.get_width() )
					 {
						  //Reset the offset
						  this.bgXOffset = 0;
					 }
					 if( this.bgXOffset >=this.backgroundTexture.get_width() )
					 {
						  //Reset the offset
						 this.bgXOffset = 0;
					 }
				}
				if(this.loopBGVert)
				{
					 if( this.bgYOffset <= -this.backgroundTexture.get_height() )
					 {
						  //Reset the offset
						  this.bgYOffset = 0;
					 }
					 if(this.bgYOffset >= this.backgroundTexture.get_height())
					 {
						  //Reset the offset
						  this.bgYOffset = 0;
					 }
				}
			}
		};
		
		this.render_bg_speed = function( xx, yy, destination, cam, renderCam)
		{
			//Show the background
			//var cameraScaleX = renderCam.get_width()/cam.get_width();
			//var cameraScaleY = renderCam.get_height()/cam.get_height();
			var cameraScaleX = 1;
			var cameraScaleY = 1;
			this.backgroundTexture.render_tex( destination, xx*cameraScaleX - cam.xPos, yy*cameraScaleY - cam.yPos,this.backgroundTexture.get_width()*cameraScaleX,this.backgroundTexture.get_height()*cameraScaleY );
		};
		
		this.render_bg = function( renderToArea, cam, renderCam,lvlW, lvlH)
		{
			
			var colCam=true;
			if(this.backgroundTexture!=-1)
			{
				if( this.backgroundTexture.textureIsFullyLoaded )
				{
					if( this.loopBGHori )
					{
						 if( this.loopBGVert )
						 {
							  for( var i=this.bgXOffset-this.backgroundTexture.get_width();i<=lvlW+this.backgroundTexture.get_width(); i+=this.backgroundTexture.get_width() )
							  {
								   for( var j=this.bgYOffset-this.backgroundTexture.get_height();j<=lvlH+this.backgroundTexture.get_height(); j+=this.backgroundTexture.get_height() )
								   {
										//colCam=gpe.check_collision_with_rect (i,j,this.backgroundTexture.get_width(),this.backgroundTexture.get_height(),cam );
										if(colCam)
										{
											 //Show the background
											 this.render_bg_speed(i, j,renderToArea, cam, renderCam);
										}
								   }
							  }
						 }
						 else
						 {
							 for(var i=this.bgXOffset-this.backgroundTexture.get_width();i<=lvlW+this.bgXOffset+this.backgroundTexture.get_width();i+=this.backgroundTexture.get_width())
							 {
								  //colCam=gpe.check_collision_with_rect( i, this.bgYOffset, this.backgroundTexture.get_width(),this.backgroundTexture.get_height(),cam );
								  if(colCam)
								  {
									   //Show the background
									   this.render_bg_speed(i,this.bgYOffset,renderToArea, cam,renderCam);
								  }
							 }
						 }
					}
					else
					{
						if(this.loopBGVert)
						{
							 for(var j=this.bgYOffset-this.backgroundTexture.get_height();j<=lvlH;j+=this.backgroundTexture.get_height() )
							 {
								  //colCam=gpe.check_collision_with_rect( this.bgXOffset,j,this.backgroundTexture.get_width() , this.backgroundTexture.get_height(),cam );
								  if(colCam)
								  {
									   //Show the background
									   this.render_bg_speed(this.bgXOffset,j,renderToArea, cam,renderCam);
								  }
							 }
						}
						else
						{
							 //Show the background
							 this.render_bg_speed(this.bgXOffset, this.bgYOffset,renderToArea, cam,renderCam);
						}
					}
				}
			}
		};
		
	}

	function GPE_GameMapTile()
	{
		this.tset = IS_NULL;
		this.tileIdNumb = IS_NULL;
		this.tSetPos = -1;
		this.xCoord = 0;
		this.yCoord = 0;
		this.tileLayer = -1;
		this.tileType =0;
		this.isSolid = false;
		this.drewBefore = false;
		this.tileBox = new GPE_Rect();
		this.tileBox.update_shape(0,0,32,32);
		this.get_type = function()
		{
			return this.tileType;
		};
		
		this.update_box = function( xNew,  yNew,  wNew, hNew)
		{
			this.tileBox.update_shape(xNew,yNew,wNew,hNew);
		};
		
		this.render_self = function( renderToArea, cam)
		{
			//ensures that the Tilesheet for the tile isn't null
			if(this.tset >=0 && this.tset < gpe.rsm.rTilesheets.length)
			{	
				var tsTex = gpe.rsm.rTilesheets[this.tset];
				//console.log( this.tset);
				if( tsTex!=-1)
				{
					//checks to see if tile is in the camera region
					//if(gpe.check_collision_with_rect(this.xCoord,this.yCoord-this.tileBox.get_height()-cam.get_y(),this.tileBox.get_width(),this.tileBox.get_height(),cam) == true )
					//{
						tsTex = tsTex.tex;
						if( tsTex!=-1)
						renderToArea.drawImage(tsTex, this.tileBox.get_x(),this.tileBox.get_y(), this.tileBox.get_width(),this.tileBox.get_height(), this.xCoord-cam.xPos, this.yCoord-cam.yPos, this.tileBox.get_width(), this.tileBox.get_height() );
					//}
				}
			}
		};
		
		this.render_self_auto = function( renderToArea)
		{
			//ensures that the Tilesheet for the tile isn't null
			if(this.tset >=0 && this.tset < gpe.rsm.rTilesheets.length)
			{	
				var tsTex = gpe.rsm.rTilesheets[this.tset];
				//console.log( this.tset);
				if( tsTex!=-1)
				{
					tsTex = tsTex.tex;
					renderToArea.drawImage(tsTex, 
					this.tileBox.get_x(),this.tileBox.get_y(), this.tileBox.get_width(),this.tileBox.get_height(), 
					this.xCoord, this.yCoord, this.tileBox.get_width(), this.tileBox.get_height() );
				}
			}
		
		};
	}

	//GPE_Layer Object
	function GPE_Layer(newLayerType, newLayerId, newlayerAlpha)
	{
		this.spatialGridBoxSize = 128;
		this.spatialGridWidthSize = 128;
		this.spatialGridHeightSize = 128;
		this.spatialGridWidthAmount = 2;
		this.spatialGridHeightAmount = 2;
		this.layerType = newLayerType;
		this.layerObjects = [];
		this.independentObjects = [];
		this.layerBackgrounds = [];
		this.scnStartBackgrounds = [];
		this.scnStartObjects = [];
		this.scnStartTiles  = [];
		this.tileAmountX = IS_NULL;
		this.tileAmountY = IS_NULL;
		this.layerId = newLayerId;
		this.layerAlpha = newlayerAlpha;
		this.layerName = "";
		this.prevTileAmountX = IS_NULL;
		this.prevTileAmountY = IS_NULL;
		this.layerTiles = [];
		
		//Collision related variables
		this.collisionSpacesInView = [];
		this.spatialGridIsCreated = false;
		this.spatialMapWidthSize = 0;
		this.spatialMapHeightSize = 0;
		this.COLLISION_AREA_SPACES = []; //Used for spatial partitioning collision checker
		this.COLLISION_AREA_OUTSIDE_SCENE = new gpe.SuperSpatialPartition();
		this.superDebugCollisions = false;

		this.objectCollisionIsChecked = {};
		this.collisionPairA = [];
		this.collisionPairB = [];
		this.collisionPairsTotal = 0;
		
		this.parsed_objects = 0;
		
		this.prerenderObjectRenderList = [];
		this.regularObjectRenderList = [];
		this.renderAboveObjectRenderList = [];
		this.renderHudObjectRenderList = [];
		
		
		this.set_default_grid_size = function()
		{
			this.spatialGridBoxSize = 128;
			this.spatialGridWidthSize = 128;
			this.spatialGridHeightSize = 128;
		};
		
		this.set_default_grid_size();
		
		this.spatialGridWidthAmount = 2;
		this.spatialGridHeightAmount = 2;
		this.objectCollisionHashesChecked = 0;
		this.spatialCollisionChecks = 0;
		//

		this.spatialGridIsCreated = false;
		this.spatialMapWidthSize = -1;
		this.spatialMapHeightSize = -1;

		//All class functions should be declared as prototypes.
		this.add_collision_pair = function( objPairA, objPairB)
		{
			this.collisionPairA[this.collisionPairsTotal] = objPairA;
			this.collisionPairB[this.collisionPairsTotal] = objPairB;
			this.collisionPairsTotal+=1;
		}
		
		this.begin_spatialpartioning = function( cSceneWidth, cSceneHeight )
		{
			if(this.spatialGridIsCreated && this.spatialMapWidthSize == cSceneWidth && this.spatialMapHeightSize == cSceneHeight)
			{
				/*
				this.COLLISION_AREA_OUTSIDE_SCENE.empty_list();
				for( var iSpace = this.COLLISION_AREA_SPACES.size()-1;iSpace >=0; iSpace--)
				{
					this.COLLISION_AREA_SPACES.at(iSpace).empty_list();
				}
				*/
				console.log("Not overwriting scene grid...");
			}
			else
			{
				this.COLLISION_AREA_OUTSIDE_SCENE.empty_list();
				this.set_default_grid_size();
				
				if( typeof cSceneWidth!="undefined" && typeof cSceneHeight!="undefined")
				{
					//Restarts the collision spaces grid.
					this.COLLISION_AREA_SPACES.clear();
					if( cSceneWidth>0 && cSceneHeight>0 )
					{
						this.spatialMapWidthSize = cSceneWidth;
						this.spatialMapHeightSize = cSceneHeight;
						
						this.spatialGridWidthAmount = Math.ceil(cSceneWidth/this.spatialGridWidthSize);
						if( this.spatialGridWidthAmount<2)
						{
							this.spatialGridWidthSize = cSceneWidth/2;
							this.spatialGridWidthAmount = 2; 
						}
						this.spatialGridHeightAmount = Math.ceil(cSceneHeight/this.spatialGridHeightSize);
						if( this.spatialGridHeightAmount<2 )
						{
							this.spatialGridHeightSize = cSceneHeight/2;
							this.spatialGridHeightAmount = 2;
						}
						
						
						//smallest size possible is 2x2
						var iGrid, jGrid;
						var xStart=0, yStart=0;
						var newSpace;
						for( iGrid =0; iGrid < this.spatialGridWidthAmount; iGrid+=1)
						{
							for( jGrid=0; jGrid < this.spatialGridHeightAmount; jGrid+=1)
							{
								newSpace = new gpe.SuperSpatialPartition();
								newSpace.set_space(xStart,yStart,this.spatialGridWidthSize,this.spatialGridHeightSize);
								yStart+=this.spatialGridHeightSize;
								newSpace.spaceName+="testrun_"+iGrid+"_"+jGrid+"_test";
								this.COLLISION_AREA_SPACES.push_back(newSpace);
							}
							yStart = 0;
							xStart+=this.spatialGridWidthSize;
						}
						this.spatialGridIsCreated = true;
						
					}
					else
					{
						console.log("Negative dimensions given for spatial partitioning...");
					}
				}
				else
				{
					console.log("Invalid dimensions given for spatial partitioning...");
				}
			}

		};
		
		this.begin_render = function( spacesInView )
		{
			this.prerenderObjectRenderList = [];
			this.regularObjectRenderList = [];
			this.renderAboveObjectRenderList = [];
			this.renderHudObjectRenderList = [];
			var foundGameObject = IS_NULL;
			var jObjectHolderParse = 0;
			
			//Adds camera independent objects of this layer into render list
			for( jLoop = this.independentObjects.length -1; jLoop >=0; jLoop--)
			{
				foundGameObject = this.independentObjects[jLoop];
				
				if( foundGameObject.isInit && foundGameObject.isVisible  && !foundGameObject.gpeIsBeingDestroyed  )
				{
					if(foundGameObject.hasPreRenderFunction)
					this.prerenderObjectRenderList.push(foundGameObject);
					
					//if( foundGameObject.hasRenderFunction)
					this.regularObjectRenderList.push(foundGameObject);
					
					if( foundGameObject.hasPostRenderFunction)
					this.renderAboveObjectRenderList.push(foundGameObject);
					
					if( foundGameObject.hasHudRenderFunction)
					this.renderHudObjectRenderList.push(foundGameObject);
					
					foundGameObject.gpeAddedToRenderList = true;
				}
			}
			
			var cSpacesMax = this.COLLISION_AREA_SPACES.size();
			//Adds camera dependent objects of this layer into render list
			for( jLoop = spacesInView.length-1; jLoop >=0; jLoop-- )
			{
				foundCollisionSpaceId= spacesInView[jLoop];
				
				if( foundCollisionSpaceId >=0 && foundCollisionSpaceId < cSpacesMax )
				{
					currentSpace = this.COLLISION_AREA_SPACES[foundCollisionSpaceId];
					for( jObjectHolderParse = currentSpace.internalArray.length-1; jObjectHolderParse >=0; jObjectHolderParse-- )
					{
						foundGameObject = currentSpace.internalArray[jObjectHolderParse];
						if( !foundGameObject.gpeAddedToRenderList &&  foundGameObject.isInit && foundGameObject.isVisible && !foundGameObject.gpeIsBeingDestroyed )
						{
							if(foundGameObject.hasPreRenderFunction)
							this.prerenderObjectRenderList.push(foundGameObject);
							
							//if( foundGameObject.hasRenderFunction)
							this.regularObjectRenderList.push(foundGameObject);
							
							if( foundGameObject.hasPostRenderFunction)
							this.renderAboveObjectRenderList.push(foundGameObject);
							
							if( foundGameObject.hasHudRenderFunction)
							this.renderHudObjectRenderList.push(foundGameObject);
							
							foundGameObject.gpeAddedToRenderList = true;
						}
					}
				}
			}
		};
		
		this.get_tile_at = function( xIn,  yIn )
		{
			var seekedTile = xIn+ yIn*this.tileAmountX;
			if(  seekedTile>=0 && seekedTile < this.layerTiles.length )
				return this.layerTiles[seekedTile];
			return IS_NULL;
		};
		
		this.clear_dynamic_data = function()
		{
			this.independentObjects = [];
			this.layerObjects = [];
			this.layerBackgrounds = [];
			for( var i = this.COLLISION_AREA_SPACES.length-1; i >=0; i--)
			{
				this.COLLISION_AREA_SPACES[i].empty_list();
			}
			
			this.COLLISION_AREA_OUTSIDE_SCENE.empty_list();
		};
		
		this.create_new_map = function( newTX, newTY)
		{
			this.tileAmountX=newTX;
			this.tileAmountY=newTY;
			var newSize = this.tileAmountX*this.tileAmountY;
			var i=0;
			this.layerTiles = [];
			for(i=0;i<newSize;i++)
			{
				this.layerTiles[i] = new GPE_GameMapTile();;
				this.layerTiles[i].tileType = 0;
			}
		};
		
		this.get_map_size = function()
		{
			return this.layerTiles.length;
		};
		
		this.remove_object = function( objToRemove)
		{
			var tObject = -1;
			for( var iItr = this.layerObjects.length-1; iItr >=0; iItr--)
			{
				tObject = this.layerObjects[iItr];
				if( tObject.gpeUniqGameObjectId==objToRemove.gpeUniqGameObjectId)
				{
					this.layerObjects.splice(iItr,1);
				}
			}
		};
		
		this.find_spatial_at = function( xIn, yIn)
		{
			var sPar = -1;
			if( xIn >=0 && yIn >=0 )
			{
				sPar =  (( xIn / this.spatialGridWidthSize)|0 )*this.spatialGridHeightAmount;
				sPar+=( yIn / this.spatialGridHeightSize)|0;
				if( sPar < 0 || sPar >= this.COLLISION_AREA_SPACES.length )
				{
					sPar = -1;
				}
			}
			return sPar;
		}
	
		this.render_objects = function()
		{
			var foundGameObject = IS_NULL;
			var jLoop = 0;
			//Renders pre-rendered objects under regular render cals.
			for( jLoop = this.prerenderObjectRenderList.length-1; jLoop >=0; jLoop-- )
			{
				foundGameObject = this.prerenderObjectRenderList[jLoop];
				//if( foundGameObject.prerenderedInFrame[gTemp]==false )
				{
					foundGameObject.render_under();
					//foundGameObject.prerenderedInFrame[gTemp] = true;
				}
			}
			gpe.set_render_alpha(255);
			
			//Does regular render calls(For objects missing render info. just draws the object's sprite at its x,y.
			for( jLoop = this.regularObjectRenderList.length-1; jLoop >=0; jLoop-- )
			{
				foundGameObject = this.regularObjectRenderList[jLoop];
				//if(  foundGameObject.renderedInFrame[gTemp]==false )
				{
					foundGameObject.render_self();
					//foundGameObject.renderedInFrame[gTemp] = true;
				}
			}
			gpe.set_render_alpha(255);
			
			//renders the object's 3rd layer render call. 
			for( jLoop = this.renderAboveObjectRenderList.length-1; jLoop >=0; jLoop-- )
			{
				foundGameObject = this.renderAboveObjectRenderList[jLoop];
				//if(  foundGameObject.renderedAboveInFrame[gTemp]==false )
				{
					foundGameObject.render_above();
					//foundGameObject.renderedAboveInFrame[gTemp] = true;
				}
			}
			gpe.set_render_alpha(255);
		};
		
		this.render_hud_objects = function()
		{
			var foundGameObject = IS_NULL;
			var jLoop = 0;
			for( jLoop = this.renderHudObjectRenderList.length-1; jLoop >=0; jLoop-- )
			{
				foundGameObject = this.renderHudObjectRenderList[jLoop];
				//if(  foundGameObject.hudrenderedInFrame[gTemp]==false )
				{
					foundGameObject.render_hud();
					//foundGameObject.hudrenderedInFrame[gTemp] = true;
				}
			}
		};
		
		this.scroll_backgrounds = function()
		{
			for (var iTemp=this.layerBackgrounds.length-1; iTemp >=0; iTemp--)
			{
				 if(this.layerBackgrounds[iTemp]!=-1)
				 this.layerBackgrounds[iTemp].scroll_bg();
			}
		};
		
		//Layer New Background functions added as of Version 1.12 [ BEGIN ] 
		this.replace_all_backgrounds = function( needleBgId, newBgId )
		{
			for( var i = this.layerBackgrounds.length; i >=0; i-- )
			{
				if( this.layerBackgrounds[i].bgId == needleBgId )
				{
					this.layerBackgrounds[i].set_background( newBgId );
				}
			}
		};
		
		this.set_background = function( layerBeingChanged, bgId, newBgId )
		{
			if( layerBeingChanged >=0 && layerBeingChanged < this.layerBackgrounds.length )
			{
				this.layerBackgrounds[layerBeingChanged].set_background( newBgId );
			}
		};
		//Layer New Background functions added as of Version 1.12 [ END ] 
		
	}

	function GPE_SceneCamera(cameraX, cameraY, cameraW, cameraH, cameraIdNumber)
	{
		 this.cameraRect = new GPE_Rect();
		 this.cameraBoundaries = new GPE_Rect();
		 this.cameraRect.update_shape(cameraX, cameraY, cameraW, cameraH);
		 
		 this.renderRect = new GPE_Rect()
		 this.scaledRect = new GPE_Rect();
		 this.scaledRect.update_shape(cameraX, cameraY, cameraW, cameraH);
		 this.viewTarget = IS_NULL;
		 this.horiBorder = IS_NULL;
		 this.vertBorder = IS_NULL;
		 
		 this.hSpeed = 0;
		 this.vSpeed = 0;
		 
		 this.isVisible = false;
		 
		 this.maxXTilesInView = 0;
		 this.maxYTilesInView = 0;
		 
		 this.cSceneWidth = 0;
		 this.cSceneHeight = 0;
		 
		
		 
		 this.center_camera = function( centerX, centerY)
		 {
			this.cameraRect.update_position(centerX-this.cameraRect.rectWidth/2,centerY-this.cameraRect.rectHeight/2);
			
			if( this.cameraRect.get_x2() > this.cameraBoundaries.get_width() )
			{
				this.cameraRect.set_x(this.cameraBoundaries.get_width()-this.cameraRect.get_width() );
			}
			if( this.cameraRect.get_y2() > this.cameraBoundaries.get_height() )
			{
				this.cameraRect.set_y( this.cameraBoundaries.get_height()-this.cameraRect.get_height() );
			}
			
			if( this.cameraRect.get_x() < 0)
			{
				this.cameraRect.set_x(0);
			}
			if( this.cameraRect.get_y() < 0)
			{
				this.cameraRect.set_y(0);
			}
		 }
		 
		 this.move_hori = function( movSpace )
		 {
			if (movSpace < 0 )
			{
				if( this.cameraBoundaries.get_x() < this.cameraRect.get_x() +movSpace )
				{
					this.cameraRect.add_x( movSpace);
				}
				else
				{
					this.cameraRect.set_x(0);
				}
			}
			else
			{
				if( this.cameraBoundaries.x2Pos > this.cameraRect.x2Pos +movSpace )
				{
					this.cameraRect.add_x( movSpace);
				}
			}
		};
		 
		 this.move_vert = function( movSpace )
		 {
			if (movSpace < 0 )
			{
				if( this.cameraBoundaries.get_y() < this.cameraRect.get_y() +movSpace )
				{
					this.cameraRect.add_y( movSpace);
				}
				else
				{
					this.cameraRect.set_y(0);
				}
			}
			else
			{
				if( this.cameraBoundaries.y2Pos > this.cameraRect.y2Pos +movSpace )
				{
					this.cameraRect.add_y( movSpace);
				}
			}
		 };
		 
		 this.setup_camera = function(cameraX,cameraY, cameraW, cameraH)
		 {
			this.isVisible = true;
			console.log("Updaing Camera with ("+cameraX+", "+cameraY+", "+ cameraW+", "+cameraH+") dimensions...");
			this.renderRect.update_shape(cameraX, cameraY, cameraW, cameraH);
			this.cameraRect.update_shape(cameraX, cameraY, cameraW, cameraH);
		 };
		 
		 this.update_tile_size = function(newTX, newTY)
		 {
			this.maxXTilesInView = newTX;
			this.maxYTilesInView = newTY;
		 };

		 this.update_screen_space = function( cameraX, cameraY, cameraW, cameraH)
		 {
			this.renderRect.update_shape(cameraX, cameraY, cameraW, cameraH);
		 };
		 
		 this.update_view_space = function( cameraX, cameraY, cameraW, cameraH)
		 {
			this.cameraRect.update_shape(cameraX, cameraY, cameraW, cameraH);
		 };
	}

	function GPE_GameScene(sId, sName)
	{
		if( typeof sId !="number")
		{
			sId = -1;
		}
		
		if( typeof sName !="string")
		{
			sName = "";
		}
		
		this.sceneName = sName;
		this.sceneId = sId;
		this.sceneHasBeenActivated = false;
		this.sceneWidth = 1024;
		this.sceneHeight = 1024;
		gpe.sceneWidth = 1024;
		gpe.sceneHeight = 1024;
		this.sceneIsContinuous = false;
		this.sceneWasVisitedBefore = false;
		
		this.defaultLayerName = "game - layer";
		
		//tile data info
		this.tileAmountX = 0;
		this.tileAmountY = 0;
		this.tileWidth = 32;
		this.tileHeight = 32;
		this.mapTextures = [];
		this.layerTilesheets = [];
		this.sceneObjects = [];
		this.persistentObjects = [];
		this.sceneLayers = [];
		this.scenesStartLayers = [];
		
		//background data
		this.bgColor = "aqua";
		this.sceneVersion = -1;
		this.bgMusic = -1;
		this.startAudio = -1;
		this.endAudio = -1;
		this.BACKGROUNDS = [];
		
		this.sceneMapIn = IS_NULL;
		this.sceneIsProcessed = false;
		this.sceneResourcesAreProcessed = false;
		this.sceneIsSpatiallyPartitioned = false;
		this.sceneCamera = [];
		
		this.disable_camera = function( cameraId)
		{
			if( cameraId >=0 && cameraId < gpe.maxCameras)
			{
				this.sceneCamera[cameraId].isVisible = false;
			}
		};
		
		this.enable_camera = function( cameraId)
		{
			if( cameraId >=0 && cameraId < gpe.maxCameras)
			{
				this.sceneCamera[cameraId].update_view_space(0,0,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
				this.sceneCamera[cameraId].update_screen_space(0,0,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
				this.sceneCamera[cameraId].isVisible = true;
			}
		};
		
		this.reset_scene_meta = function()
		{
			this.sceneCamera = [];
			for(var iV=0; iV < gpe.maxCameras; iV++)
			{
				this.sceneCamera[iV] = new GPE_SceneCamera(0,0,gpe.SCREEN_WIDTH,gpe.SCREEN_HEIGHT,iV);
			}
			
			this.sceneCamera[0].update_view_space(0,0,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
			this.sceneCamera[0].update_screen_space(0,0,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
			this.sceneCamera[0].isVisible = true;
			
			
			this.sceneCamera[1].update_view_space(0,0,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT/2);
			this.sceneCamera[1].update_screen_space(0,gpe.SCREEN_HEIGHT/2,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT/2);
			this.sceneCamera[1].isVisible = false;
			//useful for swapping between screens per 60 fps
			this.splitRenderMode = 0;


			this.sceneCamera[2].update_view_space(0,0,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
			this.sceneCamera[2].update_screen_space(0,gpe.SCREEN_HEIGHT/2,gpe.SCREEN_WIDTH/2, gpe.SCREEN_HEIGHT/2);
			this.sceneCamera[2].isVisible = false;

			this.sceneCamera[3].update_view_space(0,0,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
			this.sceneCamera[3].update_screen_space(gpe.SCREEN_WIDTH/2,gpe.SCREEN_HEIGHT/2,gpe.SCREEN_WIDTH/2, gpe.SCREEN_HEIGHT/2);
			this.sceneCamera[3].isVisible = false;
			
			this.sceneIsSpatiallyPartitioned = false;
			this.sceneHasBeenActivated = false;
		}
		
		this.clear_layers_data = function()
		{
			var cLayer = IS_NULL;
			for(var tl=this.sceneLayers.length-1; tl>=0; tl--)
			{
				cLayer = this.sceneLayers[tl];
				if(cLayer!=-1)
				{
					cLayer.clear_dynamic_data();
				}
			}
		};
		
		this.reset_scene_meta();
		
		this.add_to_persistent_objects = function( nObject)
		{
			if( nObject!=-1)
			{
				for( var i = this.persistentObjects.length - 1; i >=0; i--)
				{
					if( this.persistentObjects[i].gpeUniqGameObjectId == nObject.gpeUniqGameObjectId)
					{
						return false;
					}
				}
			}
			this.persistentObjects.push( nObject );
			return true;
		};
		
		//returns the layer if it exists
		//@parameters: (var layerIdIn)
		this.find_layer = function(layerIdIn, seekedType)
		{
			var foundLayer = IS_NULL;
			if( layerIdIn < 32 && layerIdIn>=0 )
			{
				for( var ii = this.sceneLayers.length - 1; ii >=0; ii--)
				{
					foundLayer =this.sceneLayers[ii];
					if( foundLayer!=-1)
					{
						if( foundLayer.layerId==layerIdIn && foundLayer.layerType == seekedType)
						{
							return foundLayer;
						}
					}
				}
			}
			return IS_NULL;
		};
		
		this.get_layer = function(layerIdIn, newLayerType)
		{
			var foundLayer = IS_NULL;
			if( layerIdIn < 32 && layerIdIn>=0 )
			{
				for( var ii = this.sceneLayers.length - 1; ii >=0; ii--)
				{
					foundLayer =this.sceneLayers[ii];
					if( foundLayer!=-1)
					{
						if( foundLayer.layerId==layerIdIn )
						{
							return foundLayer;
						}
					}
				}
			}
			if( typeof layerIdIn=="number" && layerIdIn >=0 && layerIdIn < 32)
			{
				console.log( "Unable to find layer with ID[" +layerIdIn+"]" );
				return this.add_layer( newLayerType, layerIdIn,1);

			}
			return IS_NULL;
		};
		
		this.get_scene_width = function()
		{
			return this.sceneWidth;
		}
		
		this.get_scene_height = function()
		{
			return this.sceneHeight;
		}
		
		//checks map for used tile
		this.find_tile = function( layerToCheck, xPos, yPos )
		{
			var foundTile = -1;
			// find rectangle to check (limited to field size):
			xPos = Math.max(0, Math.min(this.sceneWidth - 1, (xPos / this.tileWidth)|0 ) );
			yPos = Math.max(0, Math.min(this.sceneHeight - 1,(yPos / this.tileHeight)|0 ) );
			var cLayer = this.find_layer( layerToCheck, GPE_LAYER_TYPE_TILES );
			//if we have an actual layer to look for
			if( cLayer!= IS_NULL )
			{
				foundTile = cLayer.get_tile_at(xPos, yPos);
				if(foundTile!=- IS_NULL )
				{
					return foundTile.tset;
				}
			}
			return IS_NULL;
		};
		
		//thanks to the help of Yal
		// grid_check_solid(left, top, right, bottom): INT
		this.find_tile_in_box = function( layerToCheck, x1,  y1,  x2, y2 )
		{
			var i=0;
			var j=0;
			var l=0;
			var t=0;
			var r=0;
			var b=0;
			var foundTile = -1;
			// find rectangle to check (limited to field size):
			l = Math.max(0, Math.min(this.sceneWidth - 1, (x1 / this.tileWidth)|0 ) );
			t = Math.max(0, Math.min(this.sceneHeight - 1,(y1 / this.tileHeight)|0 ) );
			r = Math.max(0, Math.min(this.sceneWidth - 1, (x2 / this.tileWidth)|0 ) );
			b = Math.max(0, Math.min(this.sceneHeight - 1, (y2 / this.tileHeight)|0  ) );
			var cLayer = this.find_layer( layerToCheck, GPE_LAYER_TYPE_TILES );
			//if we have an actual layer to look for
			if( cLayer!= IS_NULL )
			{
				for (j = t; j <= b; j += 1)
				{
					for (i = l; i <= r; i += 1)
					{
						foundTile = cLayer.get_tile_at(i, j);
						if(foundTile!=- IS_NULL )
						{
							return foundTile.tset;
						}
					}
				}
			}
			return IS_NULL;
		};
		
		//takes the object and searches tilemap for collision with any possible collisions
		this.find_matching_tile = function( layerToCheck,  xPos,  yPos,  tileTypeToSearchFor )
		{
			var foundTile = -1;
			// find rectangle to check (limited to field size):
			xPos = Math.max(0, Math.min(this.sceneWidth - 1, (xPos / this.tileWidth)|0 ) );
			yPos = Math.max(0, Math.min(this.sceneHeight - 1,(yPos / this.tileHeight)|0 ) );
			var cLayer = this.find_layer( layerToCheck, GPE_LAYER_TYPE_TILES );
			//if we have an actual layer to look for
			if( cLayer!= IS_NULL )
			{
				foundTile = cLayer.get_tile_at(xPos, yPos);
				if( foundTile!=- IS_NULL && foundTile.tset == tileTypeToSearchFor )
				{
					return true;
				}
			}
			return false;
		};

		//takes the object and searches tilemap for collision with any possible collisions
		this.find_matching_tile_in_box = function( layerToCheck,  x1,  y1,  x2, y2,  tileTypeToSearchFor )
		{
			var i=0;
			var j=0;
			var l=0;
			var t=0;
			var r=0;
			var b=0;
			var foundTile = -1;
			// find rectangle to check (limited to field size):
			l = Math.max(0, Math.min(this.sceneWidth - 1, (x1 / this.tileWidth)|0 ) );
			t = Math.max(0, Math.min(this.sceneHeight - 1,(y1 / this.tileHeight)|0 ) );
			r = Math.max(0, Math.min(this.sceneWidth - 1, (x2 / this.tileWidth)|0 ) );
			b = Math.max(0, Math.min(this.sceneHeight - 1, (y2 / this.tileHeight)|0  ) );
			var cLayer = this.find_layer( layerToCheck, GPE_LAYER_TYPE_TILES );
			//if we have an actual layer to look for
			if( cLayer!= IS_NULL )
			{
				for (j = t; j <= b; j += 1)
				{
					for (i = l; i <= r; i += 1)
					{
						foundTile = cLayer.get_tile_at(i, j);
						if(foundTile!=- IS_NULL && foundTile.tset == tileTypeToSearchFor )
						{
							return true;
						}
					}
				}
			}
			return false;
		};

		
		this.scene_is_ready = function()
		{
			if( this.sceneResourcesAreProcessed && this.sceneIsProcessed)
			{
				return true;
			}		
			return false;
		}
		
		this.scene_init = function()
		{
			this.sceneIsSpatiallyPartitioned = false;
			var iItr = 0;
			var jItr = 0;
			var foundObjTypeHolder = 0;
			var foundRegObj = 0;
			var currTileToAdd = IS_NULL;
			var cLayer = IS_NULL;
			var cTile = IS_NULL;
			var currFoundTilesheet = IS_NULL;
			var newObj = IS_NULL;
			
			if( GPE_SETTINGS_IS_DEBUGGING )
			console.log("Adding ["+GPR.GAME_OBJECTS_CONTINUOUS.length+"] continuous objects to scene #"+this.sceneIdNumber+".");
			try
			{
				for( iItr = 0; iItr < GPR.GAME_OBJECTS_CONTINUOUS.length; iItr++)
				{
					GPR.GAME_OBJECTS_CONTINUOUS[iItr].SPATIAL_PARTITION_LIST = [];
					GPR.add_object(GPR.GAME_OBJECTS_CONTINUOUS[iItr], true, GPR.GAME_OBJECTS_CONTINUOUS[iItr].sceneLayerStartPos );
				}
			}
			catch (e)
			{
				alert(e);
			}
			GPR.GAME_OBJECTS_CONTINUOUS = [];
			
			if( this.sceneWasVisitedBefore && this.sceneIsContinuous )
			{
				if( GPE_SETTINGS_IS_DEBUGGING )
				console.log("Adding ["+this.persistentObjects.length+"] persistant objects to scene #"+this.sceneIdNumber+".");
				for( iItr = 0; iItr < this.persistentObjects.length; iItr++)
				{
					this.persistentObjects[iItr].SPATIAL_PARTITION_LIST = [];
					GPR.add_object(this.persistentObjects[iItr], true, GPR.GAME_OBJECTS_CONTINUOUS[iItr].sceneLayerStartPos);
				}
			}
			
			if( this.sceneWasVisitedBefore==false || this.sceneIsContinuous==false )
			{
				//Layer initialization
				var maxSceneLayers = this.sceneLayers.length;
				var newBG = IS_NULL;
				var sBG = -1;
				var sTile = -1;
				
				console.log("Updating ["+maxSceneLayers+"] layers for scene init...");
				for( iItr = 0; iItr < maxSceneLayers; iItr++)
				{
					cLayer = this.sceneLayers[iItr];
					if( cLayer!=IS_NULL)
					{
						if( cLayer.layerType == GPE_LAYER_TYPE_TILES)
						{
							for( jItr = 0; jItr < cLayer.scnStartTiles.length; jItr++)
							{
								sTile = cLayer.scnStartTiles[jItr];
								if( sTile!=-1)
								{
									if(sTile.tileNumber >=0 && sTile.tileNumber < cLayer.layerTiles.length )
									{
										cTile = cLayer.layerTiles[sTile.tileNumber];
										if( sTile.tileSheetId >=0 && sTile.tileSheetId < gpe.rsm.rTilesheets.length )
										{
											currFoundTilesheet = gpe.rsm.rTilesheets[sTile.tileSheetId];
											cTile.tset = sTile.tileSheetId;
											cTile.tileIdNumb = sTile.tileIndexId;
											if( currFoundTilesheet!=-1)
											{
												cTile.tileBox.update_box( currFoundTilesheet.get_xpos_at(sTile.tileIndexId),currFoundTilesheet.get_ypos_at(sTile.tileIndexId),currFoundTilesheet.get_tile_width(),currFoundTilesheet.get_tile_height() );
											}
										}
									}
								}
							}
						}
						else if(cLayer.layerType == GPE_LAYER_TYPE_BACKGROUND )
						{
							for( jItr = 0; jItr < cLayer.scnStartBackgrounds.length; jItr++)
							{
								sBG = cLayer.scnStartBackgrounds[jItr];
								if( sBG!=-1)
								{
									newBG = new GPE_Background();
									newBG.set_background( sBG.bgTexId );
									newBG.bgXOffset = sBG.bgXPos;
									newBG.bgYOffset = sBG.bgYPos;
									newBG.set_bgspeed(sBG.bgXSpeed, sBG.bgYSpeed);
									newBG.loopBGHori = sBG.bgTileHori;
									newBG.loopBGVert = sBG.bgTileVert;
									newBG.stretchBG = sBG.bgStartStretch;
									cLayer.layerBackgrounds.push(newBG );
								}
							}
						}
						
						if( cLayer.layerType == GPE_LAYER_TYPE_OBJECTS  && !(this.sceneWasVisitedBefore && this.sceneIsContinuous ) )
						{
							if( GPE_SETTINGS_IS_DEBUGGING )
							console.log("Adding ["+cLayer.scnStartObjects.length+"] regular objects to scene #"+this.sceneIdNumber+".");
							for( jItr = 0; jItr < cLayer.scnStartObjects.length; jItr++)
							{
								try
								{
									newObj = gpe.add_new_object( cLayer.scnStartObjects[jItr].objId, cLayer.scnStartObjects[jItr].xPos, cLayer.scnStartObjects[jItr].yPos , cLayer.layerId );
									newObj.customComponents = cLayer.scnStartObjects[jItr].customComponents;
									newObj.layerOrigID = cLayer.layerId;
									newObj.sceneLayerPos = cLayer.layerId;
									newObj.layerOrigName = cLayer.layerId;
								}
								catch (e)
								{
									alert(e);
								}
							}
						}
					}
				}
			}
			
			//activate scene objects
			try
			{
				for(iItr=GPR.GAME_OBJECTS.length-1; iItr>=0; iItr--)
				{
					foundObjTypeHolder = GPR.GAME_OBJECTS[iItr];
					for(jItr=foundObjTypeHolder.length-1; jItr>=0; jItr--)
					{
						foundRegObj = foundObjTypeHolder[jItr];
						if( foundRegObj.gpeIsBeingDestroyed==false && foundRegObj.gpeSceneStartCodeUsed==false)
						{
							foundRegObj.scene_start();
							foundRegObj.gpeSceneStartCodeUsed = true;
						}
					}
				}
			}
			
			catch (e)
			{
				alert(e);
			}
			this.sceneHasBeenActivated = true;
			this.sceneWasVisitedBefore = true;
		};
			
		this.update_scene_specs = function( lvlW, lvlH)
		{
			lvlW = Math.abs(lvlW);
			lvlH = Math.abs(lvlH);
			this.cSceneWidth = lvlW;
			this.cSceneHeight = lvlH;
			gpe.sceneWidth = lvlW;
			gpe.sceneHeight = lvlH;
			for(var iV=gpe.maxCameras-1; iV>=0; iV--)
			{
				this.sceneCamera[iV].cameraBoundaries.update_shape(0,0,lvlW, lvlH);
			}
		};
		
		this.update_tilemap = function()
		{
			if(this.tileAmountX>0 && this.tileWidth>0)
			{
				this.sceneWidth = this.tileAmountX*this.tileWidth;
			}
			else
			{
				this.sceneWidth=0;
			}

			if(this.tileAmountY>0 && this.tileHeight>0)
			{
				this.sceneHeight = this.tileAmountY*this.tileHeight;
			}
			else
			{
				this.sceneHeight = 0;
			}
			gpe.sceneWidth = this.sceneWidth;
			gpe.sceneHeight = this.sceneHeight;
			var newTile = -1;
			var cLayer = -1;
			var ii, jj;
			for(var tl=0; tl<this.sceneLayers.length;tl++)
			{
				cLayer = this.sceneLayers[tl];
				if(cLayer!=-1 && cLayer.layerType == GPE_LAYER_TYPE_TILES )
				{
					//console.log("Tilemap["+tl+"] is ("+this.tileAmountX+" X "+this.tileAmountY+") tiles.");
					//removes previous tile information					
					//creates a brand new tilemap for each tile layer
					for( ii=0;ii<this.tileAmountY;ii++)
					{
						for( jj=0;jj<this.tileAmountX;jj++)
						{
							newTile = cLayer.get_tile_at(jj, ii);
							if( typeof newTile!="undefined")
							{
								newTile.yCoord = ii*this.tileHeight;
								newTile.xCoord = jj*this.tileWidth;
								newTile.tileType= 0;
							}
						}
					}
				}
			}
			
			for(var iV=gpe.maxCameras-1; iV>=0; iV--)
			{
				this.sceneCamera[iV].cameraBoundaries.update_shape(0,0,this.sceneWidth, this.sceneHeight);
			}
		};

		this.add_background = function( bSurf, loopH, loopV, bgX, bgY, bgXSpd, bgYSpd)
		{
			var newBG = new sceneBackground(bSurf, bgX, bgY);
			newBG.set_bgspeed(bgXSpd, bgYSpd);
			newBG.loopBGHori=loopH;
			newBG.loopBGVert=loopV;
			this.BACKGROUNDS.push(newBG);
		};

		//GameScene New Backgroun functions added as of Version 1.12 [ BEGIN ] 
		this.replace_background = function( layerBeingChanged, needleBgId, newBgId )
		{
			if( layerBeingChanged >=0 && layerBeingChanged < this.sceneLayers.length )
			{
				this.sceneLayers[layerBeingChanged].replace_all_backgrounds(  needleBgId, newBgId );
			}
		};
		
		this.replace_all_backgrounds = function( needleBgId, newBgId )
		{
			for( var i = this.sceneLayers.length; i >=0; i-- )
			{
				this.sceneLayers[i].replace_all_backgrounds( needleBgId, newBgId );
			}
		};
		
		this.set_background = function( layerBeingChanged, bgId, newBgId )
		{
			if( layerBeingChanged >=0 && layerBeingChanged < this.sceneLayers.length )
			{
				this.sceneLayers[layerBeingChanged].currentScene.set_background( bgId, newBgId );
			}
		};
		//GameScene New Backgroun functions added as of Version 1.12 [ END ] 

		this.add_layer = function( newLayerType, newLayerId, newlayerAlpha )
		{
			if( typeof newLayerId=="number" && newLayerId >=0 && newLayerId < 32 )
			{
				var newLayer = new GPE_Layer(newLayerType,newLayerId, newlayerAlpha);
				newLayer.layerType = newLayerType;
				newLayer.layerId = newLayerId;
				newLayer.layerAlpha = newlayerAlpha;
				if( newLayerType == GPE_LAYER_TYPE_TILES )
				{
					newLayer.create_new_map(this.tileAmountX,this.tileAmountY);
				}
				this.sceneLayers.push(newLayer);
				return newLayer;
			}
			return IS_NULL;
		};
		
		this.add_start_layer = function( newLayerType, newLayerId, newlayerAlpha)
		{
			var newLayer = this.add_layer( newLayerType, newLayerId, newlayerAlpha );
			if( newLayer!=IS_NULL )
			{
				this.scenesStartLayers.push( { lType: newLayerType, lId: newLayerId, lAlpha: newlayerAlpha} );				
			}
			return newLayer;
		};
		
		//loading and saving scenes
		this.get_twsize = function()
		{
			return this.tileWidth;
		};

		this.get_thsize = function()
		{
			return this.tileHeight;
		};

		this.get_map_width = function()
		{
			return this.tileAmountX;
		};
		
		this.get_map_height = function()
		{
			return this.tileAmountY;
		};
		
		//this is where all of the kool game code happens at
		this.apply_logic = function()
		{
		
		};

		this.apply_postlogic = function()
		{
		  //random post-logic codes
		};

		this.get_scene_name = function()
		{
			if(this.sceneName.length >0)
			{
				return sceneName;
			}
			else
			{
				return "";
			}
		};

		//gets the FIRST TEXTURE with id number# textId
		
		this.set_scene_name = function( inSceneName)
		{
			if( inSceneName.length>0 )
			{
				this.sceneName = inSceneName;
			}
		};

		this.change_background = function( newBG, bgNumber)
		{
			if(this.BACKGROUNDS.size()>=bgNumber)
			{
				 if(this.BACKGROUNDS.at(bgNumber)!=-1)
				 {
					 this.BACKGROUNDS.at(bgNumber).set_background(newBG);
				 }
			}
		};

		this.change_foreground = function( newBG, bgNumber)
		{
			if(this.FOREGROUNDS.size()>=bgNumber)
			{
				 if(this.FOREGROUNDS.at(bgNumber)!=-1)
				 {
					 this.FOREGROUNDS.at(bgNumber).set_background(newBG);
				 }
			}
		};

		
		this.snap_xto_grid = function ( xPos)
		{
			if( xPos / this.tileWidth > this.tAmountX)
			{
				return this.tAmountX;
			}
			else
			{	return Math.max(0,(xPos/this.tileWidth)|0 );
			}
		};
		
		this.snap_yto_grid = function ( yPos)
		{
			if( yPos / this.tileHeight > this.tAmountY )
			{
				return this.tAmountY;
			}
			else
			{
				return Math.max(0,( yPos/this.tileHeight)|0 );
			}
		};
		
		this.load_game_scene = function( sceneFile )
		{
			this.sceneVersion = -1;
			//The tile offsets
			//var x = 0, y = 0, findingLoopi=0, findingLoopj=0;

			//Open the map
			this.sceneMapIn = gpe.fileReader.open_file( "gamedata/"+sceneFile+".js" );
		};
		
		this.load_and_parse_scene = function()
		{
			this.sceneIsProcessed = true;
		};
		
		this.process_scene = function()
		{
			var tInId= 0;
			
			if( this.sceneIsProcessed==false)
			{
				this.load_and_parse_scene();
			}
			if( this.sceneResourcesAreProcessed ==false && this.sceneHasBeenActivated==true)
			{
				var loadedTilesheetCount = 0;
				var foundTS = IS_NULL;
				var iLoop = 0;
				
				gpe.mapCanvas.width = this.sceneWidth;
				gpe.mapCanvas.height = this.sceneHeight;
				gpe.mapContext = gpe.mapCanvas.getContext('2d');
				gpe.GPE_Current_Context = gpe.preGuiContext = gpe.preGuiCanvas.getContext('2d',{
					antialias: '2x',
					stencil: false,
					format: 'rgba8888',
					graphicsMemory: true,
					dirty: true
				});
				var fullMap = new GPE_Rect();
				fullMap.update_shape(0,0,this.sceneWidth, this.sceneHeight);
				this.sceneResourcesAreProcessed = true;
			}
			else
			{
				var cLayer;
				for( var iLayer = this.sceneLayers.length - 1; iLayer >=0; iLayer--)
				{
					cLayer = this.sceneLayers[iLayer];
					cLayer.scroll_backgrounds();
				}
			}
			return true;
		};
		
		this.render_background_color = function( renderToArea,renderCam)
		{
			//SDL_FillRect( renderToArea, &renderToArea.clip_rect, bgColor );
		};
		
		this.render_backgrounds = function(renderToArea, cam, renderCam)
		{
			if( typeof cam!="undefined")
			{
				for (var iTemp=0; iTemp<this.BACKGROUNDS.size(); iTemp++)
				{
					 if( this.BACKGROUNDS.at(iTemp)!=-1)
					 {
						this.BACKGROUNDS.at(iTemp).render_bg(renderToArea, cam,renderCam,this.sceneWidth, this.sceneHeight);
					 }
				}
			}
		};

		this.render_foregrounds = function(renderToArea, cam)
		{
			for (var iTemp=0; iTemp<this.FOREGROUNDS.size(); iTemp++)
			{
				 if(this.FOREGROUNDS.at(iTemp)!=-1)
				 {
					this.FOREGROUNDS.at(iTemp).render_bg(renderToArea, cam, this.sceneWidth, this.sceneHeight);
				 }
			}
		};

		this.render_scene = function(renderToArea,state_controller)
		{
			gpe.GPE_Current_Context = renderToArea;
			if( this.sceneResourcesAreProcessed && this.sceneIsProcessed)
			{
				var tempTileId;
				var cTile = 0;
				var gTemp = 0;
				var hTemp=0;
				var iTemp=0;
				var jTemp=0;
				var kTemp = 0;
				var mainRenderToArea = renderToArea;
				var viewInUse = 0;
				var cLayer = 0;
				
				var rendableCameraCount = 0;
				
				for (gTemp = 0; gTemp < this.sceneCamera.length; gTemp++)
				{
					viewInUse = this.sceneCamera[ gTemp];
					if( viewInUse.isVisible)
					{
						rendableCameraCount+1;
					}
				}
				
				GPE.currentCameraInView = 0;
				//

				var cSpacesIn = state_controller.collisionSpacesInView;
				gpe.render_calls = 0;
				
				//Loops through each scene layer to prepare for rendering...
				for ( gTemp= this.sceneLayers.length-1; gTemp >=0; gTemp-- )
				{
					cLayer = this.sceneLayers.at(gTemp);
					if(cLayer!=-1 )
					{
						cLayer.begin_render( cSpacesIn );
					}
				}

				iYTempFrom = 0;
				jXTempFrom = 0;
				iYTempTo = 0;
				jXTempTo = 0;
				//Renders each camera
				for (gTemp = this.sceneCamera.length - 1; gTemp >= 0; gTemp-- )
				{
					GPE.currentCameraInView = gTemp;
					viewInUse = this.sceneCamera[ gTemp];
					if( viewInUse.isVisible)
					{
						renderToArea = gpe.GPE_Current_Context = gpe.EXTRA_CONTEXTS[gTemp];
						renderToArea.fillStyle = this.bgColor;
						gpe.set_render_alpha(255);
						renderToArea.fillRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
						
						//in theory this will render at 60 FPS with alternating switching between even and odd cameras(30FPS per camera) unless its one camera which goes at 60 FPS
						//if(  (rendableViewCount<2) || (gTemp%2 == 0 && this.splitRenderMode%2==0) || (gTemp%2 != 0 && this.splitRenderMode%2!=0) )
						{
							
							//Variables relaed to rendering layers and tiles
							hTemp = 0;
							jTemp = 0;
							
							iYTempFrom = viewInUse.cameraRect.get_y();
							jXTempFrom = viewInUse.cameraRect.get_x();
							
							iYTempTo = viewInUse.cameraRect.get_y()+viewInUse.cameraRect.get_height();
							jXTempTo = viewInUse.cameraRect.get_x()+viewInUse.cameraRect.get_width();
							
							//Used to offset render calls by camera data.
							gpe.CURRENT_VIEW = viewInUse;
							gpe.CURRENT_VIEW_BOX = viewInUse.cameraRect;

							for  (hTemp=0; hTemp<this.sceneLayers.length; hTemp++ )
							{
								cLayer = this.sceneLayers.at(hTemp);
								if(cLayer!=-1 )
								{
									if( cLayer.layerType == GPE_LAYER_TYPE_TILES )
									{
										if( this.tileWidth > 0 && this.tileHeight > 0 )
										{
											jXTempFrom = (jXTempFrom/this.tileWidth)|0;
											iYTempFrom = (iYTempFrom/this.tileHeight)|0;
											jXTempTo = ( (jXTempTo+this.tileWidth)/this.tileWidth)|0;
											iYTempTo = ( (iYTempTo+this.tileHeight)/this.tileHeight)|0;
											
											for(  iTemp = iYTempFrom; iTemp < iYTempTo; iTemp++)
											{
												for(  jTemp = jXTempFrom; jTemp < jXTempTo; jTemp++)
												{
													cTile = cLayer.get_tile_at(jTemp, iTemp);
													if( typeof cTile!="undefined" && cTile!=-1)
														cTile.render_self( renderToArea,viewInUse.cameraRect );
												}
											}
										}
									}
									else if( cLayer.layerType == GPE_LAYER_TYPE_BACKGROUND && cLayer.layerBackgrounds.length > 0 )
									{
										for( iTemp = 0; iTemp < cLayer.layerBackgrounds.length ; iTemp++)
										{
											cLayer.layerBackgrounds[iTemp].render_bg(renderToArea, viewInUse.cameraRect, viewInUse.cameraRect,this.sceneWidth, this.sceneHeight);
										}
									}
									cLayer.render_objects();
									
								}
							}
							
							//
							//Render HUD/GUI elements
							gpe.CURRENT_VIEW_BOX.xPos = 0;
							gpe.CURRENT_VIEW_BOX.yPos = 0;
							for (hTemp=0; hTemp<this.sceneLayers.length; hTemp++)
							{
								cLayer = this.sceneLayers.at(hTemp);
								if(cLayer!=-1 )
								{
									cLayer.render_hud_objects();
								}
							}
							
							if( typeof mainRenderToArea!="undefined")
							{
								mainRenderToArea.drawImage(gpe.EXTRA_CANVASES[gTemp],
								viewInUse.renderRect.get_x(),viewInUse.renderRect.get_y(), viewInUse.renderRect.get_width(), viewInUse.renderRect.get_height() );
							/* */
							}
						}
					}
				}
				
				GPE.currentCameraInView = 0;
				this.splitRenderMode+=1;
				renderToArea = mainRenderToArea;
				renderToArea.font = 'bold 16pt Calibri';
				renderToArea.textAlign = 'right';
				renderToArea.fillStyle = '#000';
				if( GPE_SETTINGS_IS_DEBUGGING )
				{
					for (gTemp = 0; gTemp < this.sceneCamera.length; gTemp++)
					{
						GPE.currentCameraInView = gTemp;						
						viewInUse = this.sceneCamera[ gTemp];
						if( viewInUse.isVisible )
						{
							//renderToArea.fillText("["+gTemp+"] "+viewInUse.cameraRect.get_x()+","+viewInUse.cameraRect.get_y()+","+ viewInUse.cameraRect.get_width()+","+ viewInUse.cameraRect.get_height(), gpe.SCREEN_WIDTH-32,48*(2+gTemp ) );
							
							var iii = 0;
							var tempColSpace = 0;
							var tempColSpacee = 0;
							for( iii = state_controller.COLLISION_AREA_SPACES.length-1;iii >=-0; iii--)
							{
								tempColSpacee = state_controller.COLLISION_AREA_SPACES[iii];
								tempColSpace = tempColSpacee.boxArea;
								mainRenderToArea.beginPath();
								mainRenderToArea.rect(tempColSpace.get_x()-viewInUse.cameraRect.get_x(), tempColSpace.get_y()-viewInUse.cameraRect.get_y(), tempColSpace.get_width(), tempColSpace.get_height() );
								mainRenderToArea.lineWidth = 1;
								if(GPR.collisionSpacesInView.indexOf( iii) >= 0 )
								{
									mainRenderToArea.strokeStyle = 'red';
								}
								else
								{
									mainRenderToArea.strokeStyle = 'blue';
								}
								mainRenderToArea.stroke();		
								mainRenderToArea.font = 'bold 16pt Calibri';
								mainRenderToArea.textAlign = 'center';
								mainRenderToArea.fillStyle = 'blue';
								mainRenderToArea.fillText( iii, tempColSpace.get_x()-viewInUse.cameraRect.get_x()+32, tempColSpace.get_y()-viewInUse.cameraRect.get_y() +32);
							}
						}
					}
					
				
					renderToArea.font = 'bold 16pt Calibri';
					renderToArea.textAlign = 'left';
					renderToArea.fillStyle = 'maroon';
					renderToArea.fillText('Scene Info || Version: '+this.sceneVersion+" | "+this.sceneWidth+" | "+this.sceneHeight, 32, 300);
					
					renderToArea.fillText('Collision Grids: '+state_controller.COLLISION_AREA_SPACES.length+' ('+state_controller.spatialGridWidthAmount+' X '+state_controller.spatialGridHeightAmount+') for '+state_controller.OBJECT_COUNT+' objects', 32, 364);
					renderToArea.textAlign = 'left';
					renderToArea.fillText('Scene Name: '+this.sceneName, 32, gpe.SCREEN2_HEIGHT-64);
				}
			}
			else
			{
				gpe.context.fillStyle = this.bgColor;
				gpe.context.fillRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
			}
		};
	
	};
})();