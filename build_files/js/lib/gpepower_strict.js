/**

 * gpepower.js v0.2.0
 *
 `* Copyright (c) PawByte Entertainment
 * Dual licensed under the MIT or GPL Version 3 licenses
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */	

"use strict"; 
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
		this.fileLines[0]  = ""; //default line, will be overwritten in onload event
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
		this.deviceType  = IS_NULL;
		this.systemController = IS_NULL;
		this.isConnected = false;
		
		this.button_count = 25; //way more than enough for a video game controller
		this.joy_count = 4; //also way more than enough for any normal controller
		this.touch_mouse_x = 0; //for touch screen enabled controllers
		this.touch_mouse_y = 0; 
		
		this.prevHit = [];
		this.butIsDown = [];
		this.butIsPressed = [];
		this.butIsReleased = [];
		this.butIsLocked = [];
		
		for (var but=0; but<this.button_count; but++)
		{
			this.prevHit[but] = false;
			this.butIsDown[but] = false;
			this.butIsPressed[but] = false;
			this.butIsReleased[but] = false;
			this.butIsLocked[but] = false;
		}
		
		this.check_connection = function()
		{
			return this.isConnected;
		};
		
		this.check_button = function(buttonId)
		{
			if( this.isConnected && buttonId >=0 && buttonId < this.button_count)
			{
				return this.butIsDown[buttonId];
			}
			return false;
		};
		
		this.check_button_pressed = function(buttonId)
		{
			if( this.isConnected && buttonId >=0 && buttonId < this.button_count)
			{
				return this.butIsPressed[buttonId];
			}
			return false;
		};
		
		this.check_button_released = function(buttonId)
		{
			if( this.isConnected && buttonId >=0 && buttonId < this.button_count)
			{
				return this.butIsReleased[buttonId];
			}
			return false;
		};
		
		
		this.reset_controller = function()
		{
			for(var but=0; but<this.button_count; but++)
			{
				this.butIsPressed[but] = this.butIsDown[but] && !this.prevHit[but];
				this.butIsReleased[but] = !this.butIsDown[but] && this.prevHit[but];
				this.prevHit[but] = this.butIsDown[but];
				this.butIsDown[but] = false;
			}
		};
		
		this.reset_down_controller = function()
		{
			for( var but=0; but<this.button_count; but++)
			{
				this.butIsDown[but] = false;
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
		this.game_controllers = [];
		this.max_controllers = 5; //max of 4 players. Change this if you want manually.
		
		for (var key=0; key<this.key_count; key++)
		{
			this.prevHit[key] = false;
			this.keyIsDown[key] = false;
			this.keyIsPressed[key] = false;
			this.keyIsReleased[key] = false;
			this.keyIsLocked[key] = false;
		}
		
		for( var mbi = 0; mbi < this.mouse_button_count;mbi++)
		{
			this.mousePrevHit[mbi] = false;
			this.mbIsDown[mbi] = false;
			this.mbIsPressed[mbi] = false;
			this.mbIsReleased[mbi] = false;
			this.mbIsLocked[mbi] = false;
		}

		for (var cController=0; cController<this.max_controllers; cController++)
		{
			this.game_controllers[cController] = new GPE_InputManager_GameController();
		}
		
		//keyboard functions
		this.check_keyboard_down = function(keyId)
		{
			if(  keyId >=0 && keyId < this.key_count)
			{
				return this.keyIsDown[keyId];
			}
			return false;
		};
		
		this.check_keyboard_pressed = function(keyId)
		{
			if(  keyId >=0 && keyId < this.key_count)
			{
				return this.keyIsPressed[keyId];
			}
			return false;
		};
		
		this.check_keyboard_released = function(keyId)
		{
			if(  keyId >=0 && keyId < this.key_count)
			{
				return this.keyIsReleased[keyId];
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
			evt.preventDefault();
			var foundKey = parseInt( evt.keyCode ? evt.keyCode : evt.which)  ;
			if( foundKey < this.key_count )
			{
				this.keyIsPressed[foundKey] = true;
				this.keyIsDown[foundKey] = true;
				this.keyIsReleased[foundKey] = false;
				//this.prevHit[foundKey] = true;
			}
		};
		
		this.handle_key_press = function( evt )
		{
			if (typeof evt=="undefined")
			{
				var evt = window.event;
			}
			evt.preventDefault();
			var foundKey = parseInt( evt.keyCode ? evt.keyCode : evt.which)  ;
			if( foundKey > 33)
			{
				foundKey -= 32;
			}
			if( foundKey < this.key_count )
			{
				this.keyIsPressed[foundKey] = true;
				this.keyIsDown[foundKey] = true;
				this.keyIsReleased[foundKey] = false;
				//this.prevHit[foundKey] = true;
			}
		};
		
		this.handle_key_up = function( evt )
		{
			if (typeof evt=="undefined")
			{
				var evt = window.event;
			}
			evt.preventDefault();
			var foundKey = parseInt( evt.which ) ;
			if( foundKey < this.key_count )
			{
				this.keyIsPressed[foundKey] = false;
				this.keyIsDown[foundKey] = false;
				this.keyIsReleased[foundKey] = true;
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
				this.mbIsPressed[foundButton] = true;
				this.mbIsDown[foundButton] = true;
				this.mbIsReleased[foundButton] = false;
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
				this.mbIsPressed[foundButton] = false;
				this.mbIsDown[foundButton] = false;
				this.mbIsReleased[foundButton] = true;
			}
		}
	}

	//the logo display class
	function GPE_LogoDisplay()
	{
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
				gpe.context.fillRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
				if( IsImageOk( this.logoImg) )
				{
					var xRender = 0;
					var rWidth = gpe.SCREEN_WIDTH;
					var rHeight = gpe.SCREEN_HEIGHT;
					var yRender = 0;
					if( this.logoImg.width < gpe.SCREEN_WIDTH)
					{
						rWidth  = this.logoImg.width;
						xRender = (gpe.SCREEN_WIDTH - rWidth)/2;
					}
					if( this.logoImg.height < gpe.SCREEN_HEIGHT)
					{
						rHeight  = this.logoImg.height;
						xRender = (gpe.SCREEN_HEIGHT - rHeight)/2;
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
		this.currentScene = gpe.gpeScenes[0];
		this.currentSceneId = 0;
		this.sceneToEnter = 0;
		this.currentScene.update_tilemap();
		//this.currentScene.load_game_scene(GPE_SETTINGS_START_SCENE,'text');
		
		//Used to keep track of game objects
		this.GAME_OBJECTS = []; //listed version of game objects based on object types
		this.GAME_OBJECTS_DEPTH_LIST = []; //used to keep track of object depth. Updated when objects are destroyed or their depth changes
		this.GAME_OBJECTS_VIEW_INDEPENDENT = [];
		this.GAME_OBJECTS_TO_BE_DESTROYED = [];
		this.GAME_OBJECTS_CONTINUOUS = [];
		this.GAME_OBJECTS_FAMILIES = [];
		this.OBJECT_COUNT = 0;
		this.MAIN_GAME_CODE_IS_READ = false;
		//Collision related variables
		this.collisionSpacesInView = [];
		this.spatialGridIsCreated = false;
		this.spatialMapWidthSize = 0;
		this.spatialMapHeightSize = 0;
		this.COLLISION_AREA_SPACES = [];
		this.COLLISION_AREA_OUTOFLEVEL = 0;
		this.objectCollisionIsChecked = {};
		this.collisionPairA = [];
		this.collisionPairB = [];
		this.collisionPairsTotal = 0;
		
		this.parsed_objects = 0;
		this.spatialGridBoxSize = 256;
		this.spatialGridWidthSize = 256;
		this.spatialGridHeightSize = 256;
		this.spatialGridWidthAmount = 2;
		this.spatalGridHeightAmount = 2;
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
	
		this.add_object = function (newObject)
		{
			if( typeof newObject!="undefined")
			{
				if( newObject!=IS_NULL)
				{
					if( newObject.gpeObjectType>=0)
					{
						if( this.GAME_OBJECTS.length > newObject.gpeObjectType)
						{
							var objectHolder = this.GAME_OBJECTS[newObject.gpeObjectType];
							objectHolder.push_back(newObject);
						}
						else
						{
							var newObjectHolder;
							for(var ii=this.GAME_OBJECTS.length; ii<= newObject.gpeObjectType; ii++)
							{
									newObjectHolder = [];
									this.GAME_OBJECTS.push(newObjectHolder);
							}
							var newestObjectHolder = this.GAME_OBJECTS[this.GAME_OBJECTS.length-1];
							newestObjectHolder.push_back(newObject);
						}
						this.OBJECT_COUNT += 1;
						newObject.uniqGameObjectId = this.OBJECT_COUNT;
						
						newObject.isInit = true;
						for(var ii=0; ii <gpe.maxCameras; ii++)
						{
							newObject.isInView[ii] = true;
						}
						if(this.GAME_OBJECTS_DEPTH_LIST.size()==0)
						{
							this.GAME_OBJECTS_DEPTH_LIST.push_back(newObject);
						}
						else
						{
							var objectPlace = IS_NULL;
							var objectDepth = newObject.get_depth();
							var foundObject = IS_NULL;
							for(var i=0; i<this.OBJECT_COUNT; i++)
							{
								foundObject = this.GAME_OBJECTS_DEPTH_LIST.at(i);
								if( typeof foundObject!="undefined")
								{
									if( foundObject!=IS_NULL)
									{
										if(objectDepth<foundObject.get_depth())
										{
											this.GAME_OBJECTS_DEPTH_LIST[this.OBJECT_COUNT] = IS_NULL;
											for(var j=GAME_OBJECTS_DEPTH_LIST.size()-1; j>i;j--)
											{
												GAME_OBJECTS_DEPTH_LIST[j]=GAME_OBJECTS_DEPTH_LIST[j-1];
											}
											GAME_OBJECTS_DEPTH_LIST[i]=newObject;
										}
									}
								}
							}
						}
						if( newObject.isViewIndependent == true )
						{
							this.GAME_OBJECTS_VIEW_INDEPENDENT.push(newObject);
						}
						newObject.GPE_AddedToGridSystem = true;
						this.update_object_in_map(newObject);
						this.sceneIsSpatiallyPartitioned = false;
						//console.log("Adding an object of type ["+newObject.gpeObjectType+"].");
						//console.log(newObject);
					}
					else
					{
						console.log("Unable to add object of type ["+newObject.gpeObjectType+"].");
					}
				}
				else
				{
					console.log("Null object added");
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

		this.apply_game_objects_prelogics = function()
		{
			var iObject;
			var foundGameObject;
			var gameObjHolder;
			var iHolderType;
			//iterates through the object's holder list
			for( iHolderType = this.GAME_OBJECTS.length-1; iHolderType>=0; iHolderType--)
			{
				gameObjHolder = this.GAME_OBJECTS[iHolderType];
				for (iObject = gameObjHolder.length-1; iObject >=0; iObject--)
				{
					foundGameObject = gameObjHolder[iObject];
					if( typeof foundGameObject!="undefined")
					{
						if( foundGameObject!=IS_NULL)
						{
							if( foundGameObject.isInit && foundGameObject.gpeIsBeingDestroyed == false)
							{
								foundGameObject.perform_object_prelogic();
								foundGameObject.process_timed_functions();
							}
						}
					}
				}
			}			
		}
		
		this.apply_spatial_game_objects_prelogics = function(cSpacesIn)
		{
			var jOBjectParse = 0;
			var currentSpace = IS_NULL;
			var foundGameObject = IS_NULL;
			for(var iSpace = cSpacesIn.length-1; iSpace >= 0; iSpace--)
			{
				currentSpace = this.COLLISION_AREA_SPACES.at( cSpacesIn[iSpace] );
				for( jOBjectParse = currentSpace.objectsHeld-1;jOBjectParse >= 0; jOBjectParse--)
				{
					foundGameObject = currentSpace.internalArray[jOBjectParse];
					if( typeof foundGameObject!="undefined")
					{
						if( foundGameObject!=IS_NULL)
						{
							if( foundGameObject.isInit && foundGameObject.prelogicAppliedInFrame==false)
							{
								foundGameObject.perform_object_prelogic();
								foundGameObject.prelogicAppliedInFrame = true;
							}
						}
					}
				}
			}
			for( var i=this.GAME_OBJECTS_VIEW_INDEPENDENT.length-1; i >=0; i--)
			{
				foundGameObject = this.GAME_OBJECTS_VIEW_INDEPENDENT[i];
				if( typeof foundGameObject!="undefined")
				{
					if( foundGameObject!=IS_NULL)
					{
						if( foundGameObject.isInit && foundGameObject.prelogicAppliedInFrame==false)
						{
							foundGameObject.perform_object_prelogic();
							foundGameObject.prelogicAppliedInFrame = true;
						}
					}
				}
			}
		}

		this.apply_game_objects_logics = function()
		{
			var iObject;
			var foundGameObject;
			var iHolderType;
			var gameObjHolder;
			for( iHolderType = this.GAME_OBJECTS.length-1; iHolderType>=0; iHolderType--)
			{
				gameObjHolder = this.GAME_OBJECTS[iHolderType];
				for (iObject = gameObjHolder.size()-1; iObject >=0; iObject--)
				{
					foundGameObject = gameObjHolder.at(iObject);
					if( typeof foundGameObject!="undefined")
					{
						if( foundGameObject!=IS_NULL)
						{
							if( foundGameObject.isInit)
							{
								foundGameObject.update_animation();
								foundGameObject.perform_object_logic();
							}
						}
					}
				}
			}
		}
		
		this.apply_game_objects_speeds = function()
		{
			var iObject;
			var foundGameObject;
			var iHolderType;
			var gameObjHolder;
			for( iHolderType = this.GAME_OBJECTS.length-1; iHolderType>=0; iHolderType--)
			{
				gameObjHolder = this.GAME_OBJECTS[iHolderType];
				for (iObject = gameObjHolder.size()-1; iObject >=0; iObject--)
				{
					foundGameObject = gameObjHolder.at(iObject);
					if( typeof foundGameObject!="undefined")
					{
						if( foundGameObject!=IS_NULL)
						{
							if( foundGameObject.isInit)
							{
								foundGameObject.gravity_use();
							}
						}
					}
				}
			}
		}

		this.apply_game_objects_endframes = function()
		{
			var iObject;
			var foundGameObject;
			var iHolderType;
			var gameObjHolder;
			for( iHolderType = this.GAME_OBJECTS.length-1; iHolderType>=0; iHolderType--)
			{
				gameObjHolder = this.GAME_OBJECTS[iHolderType];
				for (iObject = gameObjHolder.size()-1; iObject >=0; iObject--)
				{
					foundGameObject = gameObjHolder.at(iObject);
					if( typeof foundGameObject!="undefined")
					{
						if( foundGameObject!=IS_NULL)
						{
							if( foundGameObject.isInit)
							{
								foundGameObject.end_frame_logic();
							}
						}
					}
				}
			}
		}

		this.apply_spatial_game_objects_logics = function(cSpacesIn)
		{
			var jOBjectParse = 0;
			var currentSpace = IS_NULL;
			var foundGameObject = IS_NULL;
			for(var iSpace = cSpacesIn.length-1; iSpace >= 0; iSpace--)
			{
				currentSpace = this.COLLISION_AREA_SPACES.at( cSpacesIn[iSpace] );
				for( jOBjectParse = currentSpace.objectsHeld-1;jOBjectParse >= 0; jOBjectParse--)
				{
					foundGameObject = currentSpace.internalArray[jOBjectParse];
					if( typeof foundGameObject!="undefined")
					{
						if( foundGameObject!=IS_NULL)
						{
							if( foundGameObject.isInit && foundGameObject.logicAppliedInFrame==false  && foundGameObject.gpeIsBeingDestroyed == false)
							{
								foundGameObject.update_animation();
								foundGameObject.perform_object_logic();
								foundGameObject.logicAppliedInFrame  = true;
							}
						}
					}
				}
			}
			for( var i=this.GAME_OBJECTS_VIEW_INDEPENDENT.length-1; i >=0; i--)
			{
				foundGameObject = this.GAME_OBJECTS_VIEW_INDEPENDENT[i];
				if( typeof foundGameObject!="undefined")
				{
					if( foundGameObject!=IS_NULL)
					{
						if( foundGameObject.isInit && foundGameObject.logicAppliedInFrame==false)
						{
							foundGameObject.update_animation();
							foundGameObject.perform_object_logic();
							foundGameObject.logicAppliedInFrame  = true;
						}
					}
				}
			}

		}
		
		this.apply_logic = function()
		{
			if( this.GAME_OBJECTS_FAMILIES.length == 0)
			{
				GPE.activate_object_families();
			}
			this.process_scene_movement();
			if( this.currentScene!=IS_NULL)
			{
				if( this.currentScene.sceneHasBeenActivated!=true)
				{
					this.currentScene.scene_init();
				}
				this.currentScene.process_scene();
				if( this.currentScene.sceneIsProcessed && this.currentScene.sceneResourcesAreProcessed )
				{
					var foundRegObj = IS_NULL;
					var foundTypedObj = IS_NULL;
					var iHolderType = -1;
					var foundObjTypeHolder = IS_NULL;
					var jj = 0;
					var kItr = 0;
					var fcSpace = 0;
					
					//deletes objects set for deletion
					var spaceToDeleteObjectFrom = 0;;
					for( iHolderType = this.GAME_OBJECTS.length-1; iHolderType>=0; iHolderType--)
					{
						foundObjTypeHolder = this.GAME_OBJECTS[iHolderType];
						for( jj=foundObjTypeHolder.length-1; jj>=0; jj--)
						{
							foundRegObj = foundObjTypeHolder[jj];
							if( foundRegObj.gpeIsBeingDestroyed==true)
							{
								foundObjTypeHolder.splice(jj,1);
								for( kItr = foundRegObj.SPATIAL_PARTITION_LIST.length -1; kItr>=0; kItr--)
								{
									fcSpace = foundRegObj.SPATIAL_PARTITION_LIST[kItr];
									if( fcSpace < this.COLLISION_AREA_SPACES.length && fcSpace >=0 )
									{
										spaceToDeleteObjectFrom = this.COLLISION_AREA_SPACES[fcSpace];
										spaceToDeleteObjectFrom.remove_held_object(foundRegObj);
									}
									else
									{
										this.COLLISION_AREA_OUTOFLEVEL.remove_held_object(foundRegObj);
									}
								}
							}
						}
					}
					//apply game object prelogics
					this.apply_game_objects_prelogics();
					//this.apply_spatial_game_objects_prelogics(this.collisionSpacesInView);
					this.begin_spatialpartioning(this.currentScene.sceneWidth, this.currentScene.sceneHeight);					
					
					this.collisionSpacesInView = [];
					var cSceneViewFound;
					var icameraX = 0;
					var jcameraY = 0;
					var viewPartitionSpace = 0;
					var currentSpace = IS_NULL;
					if( this.currentScene.sceneIsProcessed && this.currentScene.sceneResourcesAreProcessed )
					{
						for(var iV=0; iV < gpe.maxCameras; iV++)
						{
							cSceneViewFound =this.currentScene.sceneCamera[iV];
							if( cSceneViewFound.isVisible)
							{
								for( var icameraX = cSceneViewFound.cameraRect.xPos; icameraX <= cSceneViewFound.cameraRect.x2Pos+this.spatialGridWidthSize;icameraX+=this.spatialGridWidthSize)
								{
									for( var jcameraY = cSceneViewFound.cameraRect.yPos; jcameraY <= cSceneViewFound.cameraRect.y2Pos+this.spatialGridHeightSize;jcameraY+=this.spatialGridHeightSize)
									{
										viewPartitionSpace = (( icameraX / this.spatialGridWidthSize)|0)*this.spatialGridHeightAmount;
										viewPartitionSpace+= ( jcameraY / this.spatialGridHeightSize)|0;
										if( viewPartitionSpace < this.COLLISION_AREA_SPACES.length )
										{
											this.collisionSpacesInView.push(viewPartitionSpace);
										}
									}
								}
							}
						}
					}
					
					this.collisionSpacesInView  = gpe.make_array_unique(this.collisionSpacesInView );
					//process collisions
					this.parse_spatial_collisions();
					
					for(var iSpace = this.collisionSpacesInView.length-1; iSpace >= 0; iSpace--)
					{
						currentSpace = this.COLLISION_AREA_SPACES.at( this.collisionSpacesInView[iSpace] );
						this.find_collisions_of_spatialpartion(currentSpace);
					}
					this.apply_game_collisions();
					//apply game object logics
					this.apply_spatial_game_objects_logics(this.collisionSpacesInView);
					//this.apply_game_objects_logics();
					//this.apply_game_objects_speeds();
					//this.apply_game_objects_endframes();
					this.GAME_OBJECTS_TO_BE_DESTROYED = [];
				}
			}
		};
		
		this.camera_on = function( cameraId)
		{
			if( this.currentScene!=IS_NULL)
			{
				if( cameraId >=0 && cameraId < gpe.maxCameras)
				{
					return currentScene.sceneCamera[cameraId].isVisible;
				}
			}
			return IS_NULL;
		}
		
		this.center_camera = function( cameraId, centerX, centerY)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
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
			return !(  bottomA <= topB ||  topA >= bottomB  || rightA <= leftB || leftA >= rightB );
		}

		this.check_collision_with_rect = function( boxX, boxY, boxW, boxH, rectA)
		{
			var newRectB= new GPE_Rect();
			newRectB.update_shape(boxX, boxY, boxW, boxH);
			return this.check_collision_rect(rectA,newRectB);
		}

		this.disable_camera = function( cameraId)
		{
			if( this.currentScene!=IS_NULL)
			{
				if( cameraId >=0 && cameraId < gpe.maxCameras)
				{
					this.currentScene.sceneCamera[cameraId].isVisible = false;
				}
			}
		}
		
		this.enable_camera = function( cameraId)
		{
			if( this.currentScene!=IS_NULL)
			{
				if( cameraId >=0 && cameraId < gpe.maxCameras)
				{
					this.currentScene.sceneCamera[cameraId].isVisible = true;
				}
			}
		}
		
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
					//console.log("Searching "+jObjectToCollide.uniqGameObjectId+"...");
					for( kObjectTypeToCollide = jObjectToCollide.OBJECT_COLLISION_LIST.length - 1; kObjectTypeToCollide >=0; kObjectTypeToCollide--)
					{
						kObjectHolder = currentSpace.heldObjectsArray[ jObjectToCollide.OBJECT_COLLISION_LIST[kObjectTypeToCollide] ];
						//loops through objects within object type
						for(kSubObjItr = kObjectHolder.length-1; kSubObjItr >= 0 ; kSubObjItr-- )
						{
							kObjectToCollide = kObjectHolder[kSubObjItr];
							if( jObjectToCollide.uniqGameObjectId !=kObjectToCollide.uniqGameObjectId )
							{
								this.objectCollisionHashesChecked+=2;
								objUniqColIdA = jObjectToCollide.uniqGameObjectId + ':' + kObjectToCollide.uniqGameObjectId;
								objUniqColIdB = kObjectToCollide.uniqGameObjectId + ':' + jObjectToCollide.uniqGameObjectId;
								 
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
		
		this.find_collisions_in_spatialpartions = function()
		{
			var currentSpace = IS_NULL;
			var iSpace = IS_NULL;
			//loops through collision spaces
			for(iSpace = this.COLLISION_AREA_SPACES.size()-1; iSpace >= 0; iSpace--)
			{
				currentSpace = this.COLLISION_AREA_SPACES.at(iSpace);
				this.find_collisions_of_spatialpartion(currentSpace);
			}
			//this.find_collisions_of_spatialpartion(this.COLLISION_AREA_OUTOFLEVEL);
		}
		
		this.find_furthest_object = function( theObjectType, xIn, yIn, avoidObjId)
		{
			if(theObjectType>=0 && theObjectType < this.GAME_OBJECTS.length )
			{
				var fObjC=  this.GAME_OBJECTS[theObjectType];
				if(fObjC!=IS_NULL)
				{
					if( fObjC.length > 0)
					{
						var objMinDistance = IS_NULL;
						var foundObj = IS_NULL;
						var mostDisObj = IS_NULL;
						var objAvX, objAvY;
						var cObjDistace = IS_NULL;
						for( var iObj = 0; iObj < fObjC.length; iObj++)
						{
							foundObj = fObjC[iObj];
							if( foundObj!=IS_NULL)
							{
								if( foundObj.gpeIsBeingDestroyed==false && foundObj.uniqGameObjectId !=avoidObjId)
								{
									objAvX = foundObj.collisionBox.get_center();
									objAvY = foundObj.collisionBox.get_middle();
									cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
									if( cObjDistace > objMinDistance )
									{
										mostDisObj = foundObj;
										objMinDistance = cObjDistace;
									}
								}
							}
						}
						return mostDisObj;
					}
					return IS_NULL;
				}
			}
			else if( theObjectType < 0)
			{
				//loops thru every game object of any type
				var objMinDistance = IS_NULL;
				var foundObj = IS_NULL;
				var mostDisObj = IS_NULL;
				var objAvX, objAvY;
				var fObjC= IS_NULL;
				var iObj = IS_NULL;
				var cObjDistace = IS_NULL;
				for( var hOBjType = 0; hObjType < this.GAME_OBJECTS; hObjType++)
				{
					fObjC = this.GAME_OBJECTS[hObjType];
					if(fObjC!=IS_NULL)
					{
						if( fObjC.length > 0)
						{
							for( iObj = 0; iObj < fObjC.length; iObj++)
							{
								foundObj = fObjC[iObj];
								if( foundObj!=IS_NULL)
								{
									if( foundObj.gpeIsBeingDestroyed==false && foundObj.uniqGameObjectId !=avoidObjId)
									{
										objAvX = foundObj.collisionBox.get_center();
										objAvY = foundObj.collisionBox.get_middle();
										cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
										if( cObjDistace > objMinDistance )
										{
											mostDisObj = foundObj;
											objMinDistance = cObjDistace;
										}
									}
								}
							}
							return mostDisObj;
						}
						return IS_NULL;
					}
				}
			}
			return IS_NULL;
		}
		
		//returns the number of objects of the object type
		this.find_numb_of_objects = function( theObjectType)
		{
			//finds the object array
			if(theObjectType>=0 && theObjectType < this.GAME_OBJECTS.length )
			{
				var fObjC=  this.GAME_OBJECTS[theObjectType];
				if(fObjC!=IS_NULL)
				{
					return fObjC.objs.length;
				}
				return 0;
			}
			return IS_NULL;
		}
		
		this.find_object_at_pos = function(theObjectType, pos)
		{
			if(theObjectType>=0 && theObjectType < this.GAME_OBJECTS.length )
			{
				var fObjC=  this.GAME_OBJECTS[theObjectType];
				if(fObjC!=IS_NULL)
				{
					if(pos>=0 && pos < this.GAME_OBJECTS.length )
					{
						return fObjC[pos];
					}
				}
			}
			return IS_NULL;
		};
		
		//gets the oldest object of theObjectType
		this.find_last_object = function( theObjectType)
		{
			if(theObjectType>=0 &&theObjectType < this.GAME_OBJECTS.length )
			{
				var fObjC=  this.GAME_OBJECTS[theObjectType];
				if(fObjC!=IS_NULL)
				{
					if( fObjC.length > 0)
					{
						return fObjC[0];
					}
				}
			}
			return IS_NULL;
		}

		this.find_nearest_object = function( theObjectType, xIn, yIn, avoidObjId)
		{
			if(theObjectType>=0 && theObjectType < this.GAME_OBJECTS.length )
			{
				var fObjC=  this.GAME_OBJECTS[theObjectType];
				if(fObjC!=IS_NULL)
				{
					if( fObjC.length > 0)
					{
						var objMaxDistance = IS_NULL;
						var foundObj = IS_NULL;
						var leastDisObj = IS_NULL;
						var objAvX, objAvY;
						var cObjDistace = IS_NULL;
						for( var iObj = 0; iObj < fObjC.length; iObj++)
						{
							foundObj = fObjC[iObj];
							if( foundObj!=IS_NULL)
							{
								if( foundObj.gpeIsBeingDestroyed==false && foundObj.uniqGameObjectId !=avoidObjId)
								{
									objAvX = foundObj.collisionBox.get_center();
									objAvY = foundObj.collisionBox.get_middle();
									cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
									if( objMaxDistance ==IS_NULL ||  cObjDistace < objMaxDistance )
									{
										leastDisObj = foundObj;
										objMaxDistance = cObjDistace;
									}
								}
							}
						}
						return leastDisObj;
					}
					return IS_NULL;
				}
			}
			else if( theObjectType < 0)
			{
				//loops thru every game object of any type
				var objMaxDistance = IS_NULL;
				var foundObj = IS_NULL;
				var leastDisObj = IS_NULL;
				var objAvX, objAvY;
				var fObjC= IS_NULL;
				var iObj = IS_NULL;
				var cObjDistace = IS_NULL;
				
				for( var hOBjType = 0; hObjType < this.GAME_OBJECTS; hObjType++)
				{
					fObjC = this.GAME_OBJECTS[hObjType];
					if(fObjC!=IS_NULL)
					{
						if( fObjC.length > 0)
						{
							for( iObj = 0; iObj < fObjC.length; iObj++)
							{
								foundObj = fObjC[iObj];
								if( foundObj!=IS_NULL)
								{
									if( foundObj.gpeIsBeingDestroyed==false && foundObj.uniqGameObjectId !=avoidObjId)
									{
										objAvX = foundObj.collisionBox.get_center();
										objAvY = foundObj.collisionBox.get_middle();
										cObjDistace = gpe.get_distance(xIn, yIn, objAvX, objAvY);
										if( objMaxDistance ==IS_NULL ||  cObjDistace < objMaxDistance )
										{
											leastDisObj = foundObj;
											objMaxDistance = cObjDistace;
										}
									}
								}
							}
							return leastDisObj;
						}
						return IS_NULL;
					}
				}
			}
			return IS_NULL;
		}
		
		//gets the last object of theObjectType
		this.find_newest_object = function( theObjectType)
		{
			if(theObjectType>=0 && theObjectType < this.GAME_OBJECTS.length )
			{
				var fObjC=  this.GAME_OBJECTS[theObjectType];
				if(fObjC!=IS_NULL)
				{
					if( fObjC.length > 0)
					{
						return fObjC.back();
					}
				}
			}
			return IS_NULL;
		}
		
		//returns the first object of otherObjectType that objOne collides with if it is at (xIn,yIn)
		this.find_collision_rectangle = function(  objOne,  xIn,  yIn,  otherObjectType)
		{
			//makes sure our object isn't null before checking
			if(objOne!=IS_NULL)
			{
				var tempX =objOne.xPos;
				var tempY = objOne.yPos;
				
				objOne.collisionBox.update_position( xIn, yIn);
				var tempColSpaceList = [];
				var positionSpaceId = IS_NULL;
				
				positionSpaceId = this.find_spatial_at( objOne.collisionBox.xPos, objOne.collisionBox.yPos);
				tempColSpaceList.push( positionSpaceId);

				positionSpaceId = this.find_spatial_at( objOne.collisionBox.x2Pos, objOne.collisionBox.yPos);
				tempColSpaceList.push( positionSpaceId);
				
				positionSpaceId = this.find_spatial_at( objOne.collisionBox.xPos, objOne.collisionBox.y2Pos);
				tempColSpaceList.push( positionSpaceId);
				
				positionSpaceId = this.find_spatial_at( objOne.collisionBox.x2Pos, objOne.collisionBox.y2Pos);
				tempColSpaceList.push( positionSpaceId);
				
				tempColSpaceList = gpe.make_array_unique(tempColSpaceList);
				var positionSpace = IS_NULL;
				var cObjController = IS_NULL;
				var iItr = IS_NULL;
				var fObject = IS_NULL;
				for( var iColSpace = tempColSpaceList.length-1; iColSpace>=0; iColSpace--)
				{
					positionSpaceId = tempColSpaceList[iColSpace];
					if( positionSpaceId < 0 || positionSpaceId >= this.COLLISION_AREA_SPACES.length)
					{
						positionSpace = this.COLLISION_AREA_OUTOFLEVEL;
					}
					else
					{
						positionSpace = this.COLLISION_AREA_SPACES.at(positionSpaceId);
					}

					if(otherObjectType < 0 || otherObjectType >= this.GAME_OBJECTS.length)
					{
						for( iItr = 0; iItr < positionSpace.internalArray.length; iItr++)
						{
							fObject = positionSpace.internalArray[iItr];
							
							if( gpe.check_collision_rect( objOne.collisionBox, fObject.collisionBox) )
							{
								objOne.collisionBox.update_position( tempX, tempY);
								return fObject;
							}
						}
					}
					else
					{
						cObjController = positionSpace.heldObjectsArray[otherObjectType];
						for( iItr = 0; iItr < cObjController.length; iItr++)
						{
							fObject = cObjController[iItr];
							if( fObject!=IS_NULL)
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
				objOne.collisionBox.update_position( tempX, tempY);
			}
			return IS_NULL;
		};

		this.find_object_collision = function(  objOne,  xIn,  yIn,  otherObjectType)
		{
			
		}
		
		this.get_camera_info = function(cameraId)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
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
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.cameraRect.get_height();
			}
			return -1;
		}
		
		this.get_camera_width = function(cameraId)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.cameraRect.get_width();
			}
			return -1;
		}
		
		this.get_camera_x = function(cameraId)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.cameraRect.get_x();
			}
			return -1;
		}
		
		this.get_camera_y = function(cameraId)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.cameraRect.get_y();
			}
			return -1;
		}
		
		//
		this.get_camera_screen_height = function(cameraId)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.renderRect.get_height();
			}
			return -1;
		}
		
		this.get_camera_screen_width = function(cameraId)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.renderRect.get_width();
			}
			return -1;
		}
		
		this.get_camera_screen_x = function(cameraId)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.renderRect.get_x();
			}
			return -1;
		}
		
		this.get_camera_screen_y = function(cameraId)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				var cCam = this.currentScene.sceneCamera[cameraId];
				return cCam.renderRect.get_y();
			}
			return -1;
		}
		
		
		this.get_scene_width = function()
		{
			return this.currentScene.get_scene_width();
		}
		
		this.get_scene_height = function()
		{
			return this.currentScene.get_scene_height();
		}
		
		//checks if the position is open and free of ANYTHING  or of the otherObjectType
		this.position_open = function(  xIn, yIn, otherObjectType)
		{
			var positionSpaceId = this.find_spatial_at( xIn, yIn);
			var positionSpace = IS_NULL;
			if( positionSpaceId < 0 || positionSpace >= this.COLLISION_AREA_SPACES.length)
			{
				positionSpace = this.COLLISION_AREA_OUTOFLEVEL;
			}
			else
			{
				positionSpace = this.COLLISION_AREA_SPACES.at(positionSpaceId);
			}
			var iItr = IS_NULL;
			var fObject = IS_NULL;
			if(otherObjectType < 0 || otherObjectType >= this.GAME_OBJECTS.length)
			{
				for( iItr = 0; iItr < positionSpace.internalArray.length; iItr++)
				{
					fObject = positionSpace.internalArray[iItr];
					if( gpe.point_within(xIn, yIn, fObject.collisionBox.xPos, fObject.collisionBox.yPos, fObject.collisionBox.x2Pos, fObject.collisionBox.y2Pos) )
					{
						return fObject;
					}
				}
			}
			else
			{
				var cObjController = positionSpace.heldObjectsArray[otherObjectType];
				for( iItr = 0; iItr < cObjController.length; iItr++)
				{
					fObject = cObjController[iItr];
					if( fObject!=IS_NULL)
					{
						if( gpe.point_within(xIn, yIn, fObject.collisionBox.xPos, fObject.collisionBox.yPos, fObject.collisionBox.x2Pos, fObject.collisionBox.y2Pos) )
						{
							return fObject;
						}
					}
				}
			}
			return IS_NULL;
		};
		
		this.find_nearest_spatial_at = function( xIn, yIn)
		{
			var sPar = -1;
			if( xIn >=0)
			{
				xIn = 0;
			}
			if( yIn >=0)
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
		}
		
		this.render_state = function()
		{
			if( this.currentScene!=IS_NULL)
			{
				gpe.context.font = 'bold 16pt Calibri';
				gpe.context.textAlign = 'left';
				gpe.context.fillStyle = '#FFF';

				
								
				if( this.currentScene.scene_is_ready() )
				{
					if( this.currentScene.sceneIsProcessed && this.currentScene.sceneResourcesAreProcessed )
					{
					
						gpe.GPE_Current_Context = gpe.preGuiContext;
						gpe.preGuiContext.fillStyle = "black";
						gpe.preGuiContext.clearRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
				
						this.currentScene.render_scene(gpe.GPE_Current_Context,this);
					}
					
					if( gpe.CURRENT_VIEW_BOX!=IS_NULL)
					{
						gpe.context.fillText(	gpe.CURRENT_VIEW_BOX.xPos+","+gpe.CURRENT_VIEW_BOX.yPos, 40, gpe.SCREEN_HEIGHT-96);
					}
					else
					{
						gpe.context.fillText(	"NULL VIEWPOINT", 40, gpe.SCREEN_HEIGHT-96);
					}
				}
				else
				{
					gpe.context.fillStyle = '#333';
					gpe.context.fillRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
					gpe.context.fillStyle = '#FFF';
					gpe.context.fillText("Loading...", 40, gpe.SCREEN_HEIGHT-64);
				}
				gpe.context.drawImage(gpe.preGuiCamvas,0,0 );
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
		};

		//Used to separate scene into zones for collision checks
		this.begin_spatialpartioning = function( cSceneWidth, cSceneHeight)
		{
			if(this.spatialGridIsCreated && this.spatialMapWidthSize == cSceneWidth && this.spatialMapHeightSize == cSceneHeight)
			{
/*				this.COLLISION_AREA_OUTOFLEVEL.empty_list();
				for( var iSpace = this.COLLISION_AREA_SPACES.size()-1;iSpace >=0; iSpace--)
				{
					this.COLLISION_AREA_SPACES.at(iSpace).empty_list();
				}
*/
			}
			else
			{
				this.COLLISION_AREA_OUTOFLEVEL.empty_list();
				this.spatialGridBoxSize = 256; //default size of one grid
				this.spatialGridWidthSize = 256; //default size of one grid in pixels
				this.spatialGridHeightSize = 256.; //default size of one grid in pixels
				
				if( typeof cSceneWidth!="undefined" && typeof cSceneHeight!="undefined")
				{
					//Restarts the collision spaces grid.
					this.COLLISION_AREA_SPACES.clear();
					if( cSceneWidth>0 && cSceneHeight>0)
					{
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
						//smallest size possible is 2x2
						var iGrid, jGrid;
						var xStart=0, yStart=0;
						var newSpace;
						for( iGrid =0; iGrid < this.spatialGridWidthAmount; iGrid+=1)
						{
							for( jGrid=0; jGrid < this.spatialGridHeightAmount; jGrid+=1)
							{
								newSpace = new this.SuperSpatialPartition();
								newSpace.set_space(xStart,yStart,this.spatialGridWidthSize,this.spatialGridHeightSize);
								yStart+=this.spatialGridHeightSize;
								newSpace.spaceName+="testrun_"+i+"_"+j+"_test";
								this.COLLISION_AREA_SPACES.push_back(newSpace);
							}
							yStart = 0;
							xStart+=this.spatialGridWidthSize;
						}
						this.spatialGridIsCreated = true;
						this.spatialMapWidthSize = cSceneWidth;
						this.spatialMapHeightSize = cSceneHeight;
					}
				}
			}
		}
		
		//creates all of the collisions
		this.parse_spatial_collisions = function()
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
			if( this.currentScene.sceneIsProcessed && this.currentScene.sceneResourcesAreProcessed )
			{
				if( this.currentScene.sceneHasBeenActivated==true && this.currentScene.sceneIsSpatiallyPartitioned == false)
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
					objPartition[0] = -1; //top-left corner
					objPartition[1] = -1; //top-right corner
					objPartition[2] = -1; //bottom-left corner
					objPartition[3] = -1; //bottom-right corner
					var sgX1;
					var sgY1;
					var sgX2;
					var sgY2;
					
					var iCtrPartition = 0;
					var foundPartitionSpace = IS_NULL;
					this.parsed_objects  = 0;
					//check for collisions with grids		
					//iterates through the object's holder list
					for( iHolderType = this.GAME_OBJECTS.length-1; iHolderType>=0; iHolderType--)
					{
						gameObjHolder = this.GAME_OBJECTS[iHolderType];
						for (iObject = gameObjHolder.size()-1; iObject >=0; iObject--)
						{
							foundGameObject = gameObjHolder.at(iObject);
							//makes sure object still exists
							if( typeof foundGameObject!="undefined")
							{
								if( foundGameObject!=IS_NULL)
								{						
									//makes sure the object has collisions(To be edited out on new update )
									OBJECT_LIST = foundGameObject.OBJECT_COLLISION_LIST;
									foundGameObject.collisionBeingChecked = true;
									foundGameObject.isOutsideRoom = false;
									foundGameObject.logicAppliedInFrame = false;
									foundGameObject.prelogicAppliedInFrame = false;
									for( var iView = 0; iView < gpe.maxCameras; iView++)
									{
										foundGameObject.prerenderedInFrame[iView] = false;
										foundGameObject.renderedInFrame[iView] = false;
									}

									this.parsed_objects  += 1;
										
									//places objects in up to 4 spatial grids.
									sgX1 = foundGameObject.collisionBox.xPos;
									sgY1 = foundGameObject.collisionBox.yPos;
									sgX2 = foundGameObject.collisionBox.x2Pos;
									sgY2 = foundGameObject.collisionBox.y2Pos;
									
									if( sgX1 < 0 || sgY1 < 0 || sgX1 > this.sceneWidth || sgY1 > this.sceneHeight)
									{
										objPartition[0] = -1;
									}
									else
									{
										objPartition[0] = ( (sgX1 / this.spatialGridWidthSize)|0)*this.spatialGridHeightAmount;
										objPartition[0]+= ( sgY1 / this.spatialGridHeightSize)|0;
									}
									if( sgX2 < 0 || sgY1 < 0 || sgX2 > this.sceneWidth || sgY1 > this.sceneHeight)
									{
										objPartition[1]  = -1;
									}
									else
									{
										objPartition[1] = (( sgX2 / this.spatialGridWidthSize)|0)*this.spatialGridHeightAmount;
										objPartition[1]+= ( sgY1 / this.spatialGridHeightSize)|0;
									}
									
									if( sgX1 < 0 || sgY2 < 0 || sgX1 > this.sceneWidth || sgY2 > this.sceneHeight)
									{
										objPartition[2]  = -1;
									}
									else
									{
										objPartition[2] =( ( sgX1 / this.spatialGridWidthSize)|0 )*this.spatialGridHeightAmount;
										objPartition[2]+= ( sgY2 / this.spatialGridHeightSize)|0;
									}
									
									if( sgX2 < 0 || sgY2 < 0 || sgX2 > this.sceneWidth || sgY2 > this.sceneHeight)
									{
										objPartition[3]  = -1;
									}
									else
									{
										objPartition[3] =  (( sgX2 / this.spatialGridWidthSize)|0 )*this.spatialGridHeightAmount;
										objPartition[3]+=( sgY2 / this.spatialGridHeightSize)|0;
									}
									foundGameObject.SPATIAL_PARTITION_LIST = gpe.make_array_unique(objPartition);
									for( iCtrPartition = foundGameObject.SPATIAL_PARTITION_LIST.length-1; iCtrPartition>=0; iCtrPartition--)
									{
										if( foundGameObject.SPATIAL_PARTITION_LIST[iCtrPartition] < 0 || foundGameObject.SPATIAL_PARTITION_LIST[iCtrPartition] >= this.COLLISION_AREA_SPACES.size()  )
										{
											if( foundGameObject.isOutsideRoom ==false)
											{
												this.COLLISION_AREA_OUTOFLEVEL.add_object(foundGameObject);
												foundGameObject.isOutsideRoom = true;
											}
										}
										else
										{
											foundPartitionSpace = this.COLLISION_AREA_SPACES.at( foundGameObject.SPATIAL_PARTITION_LIST[iCtrPartition] );
											foundPartitionSpace.add_object(foundGameObject);
										}
									}
									foundGameObject.collisionAddedToSpaceGrid = true;
								}
							}
							foundGameObject.GPE_AddedToGridSystem = true;
						}
						this.superDebugCollisions = false;
						this.currentScene.sceneIsSpatiallyPartitioned = true;
					}
				}
			}
		}
		
		this.process_scene_movement = function()
		{
			if( this.currentSceneId!=this.sceneToEnter && this.sceneToEnter>=0 && this.sceneToEnter < gpe.gpeScenes.length)
			{
				var nextSceneToGo = this.sceneToEnter;
				var previousSceneIn = this.currentSceneId;
				var jj;
				var iHolderType;
				var foundObjTypeHolder;
				var foundRegObj;
				for( iHolderType = this.GAME_OBJECTS.length-1; iHolderType>=0; iHolderType--)
				{
					this.GAME_OBJECTS_CONTINUOUS = [];
					foundObjTypeHolder = this.GAME_OBJECTS[iHolderType];
					for( jj=foundObjTypeHolder.length-1; jj>=0; jj--)
					{
						foundRegObj = foundObjTypeHolder[jj];
						if( foundRegObj.gpeIsBeingDestroyed==false)
						{
							foundRegObj.scene_start();
						}
						if( foundRegObj.isContinuousObject)
						{
							this.GAME_OBJECTS_CONTINUOUS.push(foundRegObj);
						}
					}
				}
				//if( this.currentScene.sceneIsContinuous ==false)
				{
					//Restart Object and scene related things
					this.scene_restart_object_info();
				
					this.currentScene.scene_restart();
				}
				if( this.currentScene!=IS_NULL)
				{
					gpe.stop_audio(this.currentScene.bgMusic);
					gpe.play_audio(this.currentScene.endAudio,false);
				}
				this.currentScene = gpe.gpeScenes[ nextSceneToGo ];
				this.sceneToEnter = nextSceneToGo;
				this.currentSceneId = nextSceneToGo;
				this.currentScene.update_tilemap();
			}
		}
		
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
			
			SuperSpatialPartition.prototype.empty_list = function(newObject)
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
					var subHeldArray = 0;
					for( var ii = this.internalArray.length-1;  ii>=0; ii--)
					{
						foundHeldObject = this.internalArray[ii];
						if( foundHeldObject.uniqGameObjectId==objectIn.uniqGameObjectId )
						{
							this.internalArray.splice(ii,1);
							subHeldArray = this.heldObjectsArray[objectIn.gpeObjectType];
							for( var jj = subHeldArray.length-1; jj>=0; jj--)
							{
								foundSubHeldObject = subHeldArray[jj];
								if( foundSubHeldObject.uniqGameObjectId==objectIn.uniqGameObjectId)
								{
									this.heldObjectsArray[objectIn.gpeObjectType].splice(jj,1);
								}
					
							}
							this.objectsHeld-=1;
							return true;
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
		
		this.scene_restart_object_info = function()
		{
			this.GAME_OBJECTS_DEPTH_LIST =[];
			this.GAME_OBJECTS_VIEW_INDEPENDENT = [];

			this.GAME_OBJECTS_TO_BE_DESTROYED = [];


			this.collisionSpacesInView = [];

			this.spatialGridIsCreated = false;

			this.spatialMapWidthSize = 0;

			this.spatialMapHeightSize = 0;

			this.objectCollisionIsChecked = {};


			this.GAME_OBJECTS[0] = [];
			this.COLLISION_CHECK_ARRAY = new gpe.SuperVector();
			var newVector;

			for(var i=0; i <GPE_GAME_OBJECTS_COUNT; i+=1)
			{
				newVector = new gpe.SuperVector();
				for(var j=0; j <GPE_GAME_OBJECTS_COUNT; j+=1)
				{
					newVector.push_back(false);
				}
				this.COLLISION_CHECK_ARRAY.push_back(newVector);
			}
			this.COLLISION_AREA_SPACES = []; //Used for spatial partitioning collision checker
			this.COLLISION_AREA_OUTOFLEVEL = new this.SuperSpatialPartition();
			this.superDebugCollisions = false;
			this.collisionPairA = [];

			this.collisionPairB = [];

			this.collisionPairsTotal = 0;


			this.parsed_objects = 0;

			this.spatialGridBoxSize = 256;

			this.spatialGridWidthSize = 256;

			this.spatialGridHeightSize = 256;

			this.spatialGridWidthAmount = 2;

			this.spatalGridHeightAmount = 2;

			this.objectCollisionHashesChecked = 0;

			this.spatialCollisionChecks = 0;

			this.spatialGridIsCreated = false;

			this.spatialMapWidthSize = -1;

			this.spatialMapHeightSize = -1;
		}
		
		this.scene_enter_previous = function()
		{
			if( this.sceneToEnter==this.currentSceneId)
			{
				if( this.currentSceneId > 0)
				{
					this.sceneToEnter = this.currentSceneId - 1;
				}
			}
		}
		
		this.scene_enter_next = function()
		{
			if( this.sceneToEnter==this.currentSceneId)
			{
				if( this.currentSceneId < gpe.gpeScenes.length-1)
				{
					this.sceneToEnter = this.currentSceneId + 1;
				}
			}
		}
		
		this.scene_enter = function( nextSceneId )
		{
			if( this.sceneToEnter==this.currentSceneId)
			{
				if( nextSceneId>=0 && nextSceneId < gpe.gpeScenes.length)
				{
					this.sceneToEnter = nextSceneId;
				}
			}
		}
		
		this.setup_camera = function(cameraId, cameraX,cameraY, cameraW, cameraH)
		{
			if( this.currentScene!=IS_NULL && cameraId >=0 && cameraId < gpe.maxCameras)
			{
				this.currentScene.sceneCamera[cameraId].setup_camera( cameraX, cameraY, cameraW, cameraH );
			}
		}
		
		this.update_object_in_map = function(objectBeingUpdated)
		{
			if( this.currentScene.sceneIsProcessed && this.currentScene.sceneResourcesAreProcessed )
			{
				if( this.currentScene.sceneHasBeenActivated==true && this.currentScene.sceneIsSpatiallyPartitioned == true)
				{
					if( typeof objectBeingUpdated!="undefined" )
					{
						var objPartition = [-1, -1, -1, -1];
						objPartition[0] = -1; //top-left corner
						objPartition[1] = -1; //top-right corner
						objPartition[2] = -1; //bottom-left corner
						objPartition[3] = -1; //bottom-right corner
						var iCtrPartition = 0;
						var foundPartitionSpace;
						
						//places objects in up to 4 spatial grids.
						var sgX1 = objectBeingUpdated.collisionBox.xPos;
						var sgY1 = objectBeingUpdated.collisionBox.yPos;
						var sgX2 = objectBeingUpdated.collisionBox.x2Pos;
						var sgY2 = objectBeingUpdated.collisionBox.y2Pos;
						
						//removes object from previous grids
						if( objectBeingUpdated.GPE_AddedToGridSystem==true)
						{
							for( iCtrPartition = objectBeingUpdated.SPATIAL_PARTITION_LIST.length-1; iCtrPartition>=0; iCtrPartition--)
							{
								if( objectBeingUpdated.SPATIAL_PARTITION_LIST[iCtrPartition] < 0 || objectBeingUpdated.SPATIAL_PARTITION_LIST[iCtrPartition] >= this.COLLISION_AREA_SPACES.size()  )
								{
									this.COLLISION_AREA_OUTOFLEVEL.remove_held_object(objectBeingUpdated);
								}
								else
								{
									foundPartitionSpace = this.COLLISION_AREA_SPACES.at( objectBeingUpdated.SPATIAL_PARTITION_LIST[iCtrPartition] );
									foundPartitionSpace.remove_held_object(objectBeingUpdated);
								}				
							}
						}
						if( sgX1 < 0 || sgY1 < 0 || sgX1 > this.sceneWidth || sgY1 > this.sceneHeight)
						{
							objPartition[0] = -1;
						}
						else
						{
							objPartition[0] = ( (sgX1 / this.spatialGridWidthSize)|0)*this.spatialGridHeightAmount;
							objPartition[0]+= ( sgY1 / this.spatialGridHeightSize)|0;
						}
						if( sgX2 < 0 || sgY1 < 0 || sgX2 > this.sceneWidth || sgY1 > this.sceneHeight)
						{
							objPartition[1]  = -1;
						}
						else
						{
							objPartition[1] = (( sgX2 / this.spatialGridWidthSize)|0)*this.spatialGridHeightAmount;
							objPartition[1]+= ( sgY1 / this.spatialGridHeightSize)|0;
						}

						if( sgX1 < 0 || sgY2 < 0 || sgX1 > this.sceneWidth || sgY2 > this.sceneHeight)
						{
							objPartition[2]  = -1;
						}
						else
						{
							objPartition[2] =( ( sgX1 / this.spatialGridWidthSize)|0 )*this.spatialGridHeightAmount;
							objPartition[2]+= ( sgY2 / this.spatialGridHeightSize)|0;
						}

						if( sgX2 < 0 || sgY2 < 0 || sgX2 > this.sceneWidth || sgY2 > this.sceneHeight)
						{
							objPartition[3]  = -1;
						}
						else
						{
							objPartition[3] =  (( sgX2 / this.spatialGridWidthSize)|0 )*this.spatialGridHeightAmount;
							objPartition[3]+=( sgY2 / this.spatialGridHeightSize)|0;
						}
						objectBeingUpdated.SPATIAL_PARTITION_LIST = gpe.make_array_unique(objPartition);
						if( objectBeingUpdated.GPE_AddedToGridSystem==true)
						{
							for( iCtrPartition = objectBeingUpdated.SPATIAL_PARTITION_LIST.length-1; iCtrPartition>=0; iCtrPartition--)
							{
								if( objectBeingUpdated.SPATIAL_PARTITION_LIST[iCtrPartition] < 0 || objectBeingUpdated.SPATIAL_PARTITION_LIST[iCtrPartition] >= this.COLLISION_AREA_SPACES.size()  )
								{
									if( objectBeingUpdated.isOutsideRoom ==false)
									{
										this.COLLISION_AREA_OUTOFLEVEL.add_object(objectBeingUpdated);
										objectBeingUpdated.isOutsideRoom = true;
									}
								}
								else
								{
									foundPartitionSpace = this.COLLISION_AREA_SPACES.at( objectBeingUpdated.SPATIAL_PARTITION_LIST[iCtrPartition] );
									foundPartitionSpace.add_object(objectBeingUpdated);
								}
							}
						}
					}	
				}
			}
		}
		
		this.GAME_OBJECTS[0] = [];
		this.COLLISION_CHECK_ARRAY = new gpe.SuperVector();
		var newVector;
		
		for(var i=0; i <GPE_GAME_OBJECTS_COUNT; i+=1)
		{
			newVector = new gpe.SuperVector();
			for(var j=0; j <GPE_GAME_OBJECTS_COUNT; j+=1)
			{
				newVector.push_back(false);
			}
			this.COLLISION_CHECK_ARRAY.push_back(newVector);
		}
		this.COLLISION_AREA_SPACES = []; //Used for spatial partitioning collision checker
		this.COLLISION_AREA_OUTOFLEVEL = new this.SuperSpatialPartition();
		this.superDebugCollisions = false;
	}
	
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
			if( this.html5AudioSource!=IS_NULL)
			{
				return !this.html5AudioSource.paused;
			}
			return false;
		}
		
		this.load_audio = function()
		{
			if( this.html5AudioSource==IS_NULL)
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
						this.audioCouldBePlayed  = GPE_AUDIO_PLAYABLE[ii];
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
			if( this.html5AudioSource!=IS_NULL && this.audioLoadedInMemory==true )
			{
				this.html5AudioSource.pause();
				this.audioWasPaused = true
				this.audioWasStopped = false;
			}
			if(gpe.lastLoopedTrack == this.audioId )
			{
				gpe.lastLoopedTrack = -1;
			}
		}
		
		this.play_audio = function(loops)
		{
			if(typeof loops=="undefined")
			{
				var loops  = false
			}
			if( this.html5AudioSource!=IS_NULL)
			{
				if( this.audioIsLoaded==true && this.audioLoadedInMemory==true  )
				{
					if ( !this.html5AudioSource.paused )
					{
						//	this.html5AudioSource.pause();
					
						this.html5AudioSource.currentTime   = 0.5;
					}
					else if(this.html5AudioSource.ended)
					{
						this.html5AudioSource.currentTime   = 0;
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
					console.log("Playing Audio"+this.audioId+" that loops");
					gpe.lastLoopedTrack = this.audioId;
					this.html5AudioSource.addEventListener('ended', gpe.loop_last_track, false);
				}
				else
				{
					console.log("Playing Audio"+this.audioId+" that doesnt loop");
				}
			}
		}
		
		this.stop_audio = function()
		{
			if( this.html5AudioSource!=IS_NULL && this.audioLoadedInMemory==true )
			{
				this.html5AudioSource.currentTime = 0;
				this.html5AudioSource.pause();
				this.audioWasPaused = false;
				this.audioWasStopped = true;
			}
			if(gpe.lastLoopedTrack == this.audioId )
			{
				gpe.lastLoopedTrack = -1;
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
			if( this.html5VideoSource!=IS_NULL)
			{
				return !this.html5VideoSource.paused;
			}
			return false;
		};
		
		this.load_video = function()
		{
			if( this.html5VideoSource==IS_NULL)
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
						this.videoCouldBePlayed  = GPE_VIDEO_PLAYABLE[ii];
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
			if( this.html5VideoSource!=IS_NULL && this.videoLoadedInMemory==true )
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
				var loops  = false
			}
			if( typeof videoPlayingIsSkippable=="undefined")
			{
				var videoSkippable = true;
			}
			if( this.html5VideoSource!=IS_NULL)
			{
				gpe.currentVideoPlaying = this.videoId;
				gpe.isPlayingVideo = true;
				gpe.videoPlayingIsSkippable = videoSkippable;
				if( this.videoIsLoaded==true && this.videoLoadedInMemory==true  )
				{
					if ( !this.html5VideoSource.paused )
					{
						//	this.html5VideoSource.pause();
					
						this.html5VideoSource.currentTime   = 0.5;
					}
					else if(this.html5VideoSource.ended)
					{
						this.html5VideoSource.currentTime   = 0;
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
					this.html5VideoSource.oncanplay  = function () 
					{
					
						this.play();
					};
				}
				this.videoWasPaused = false;
				this.videoWasStopped = false;
				if( loops)
				{
					//console.log("Playing Video["+this.videoId+"[ that loops");
					gpe.lastLoopedVideo = this.videoId;
					this.html5VideoSource.addEventListener('ended', gpe.loop_last_video, false);
				}
				else
				{
					//console.log("Playing Video["+this.videoId+"] that doesnt loop");
					this.html5VideoSource.addEventListener('ended', gpe.stop_last_video, false);
				}
			}
		};
		
		this.process_playing_video = function()
		{
			if( this.html5VideoSource!=IS_NULL)
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
			if( this.html5VideoSource!=IS_NULL)
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
			if( this.html5VideoSource!=IS_NULL)
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
			if( this.RESOURCES_ADDED !=1)
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
						if( textureToCheck!=IS_NULL  && typeof textureToCheck!='undefined')
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
						if( spriteToCheck!=IS_NULL && typeof spriteToCheck!='undefined')
						{
							if( spriteToCheck.imageObj!=IS_NULL && typeof spriteToCheck.imageObj!='undefined')
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
		
		this.add_font = function(fontIdIn, newFontFamilyName, eotSrc, svgSrc, ttfSrc, wOffSrc,wOff2Src, fontSizeIn,fontTypeIn)
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
			newFont.fontType = fontTypeIn;
			newFont.load_font();
			this.rFonts.push(newFont);
			return this.rFonts.length-1;
		}
		
		//colW = colR for circles, colH = ignored if circle
		this.add_sprite = function (setId,spriteFileName, xStart, yStart,frameWidth, frameHeight, spriteShape, colX, colY, colW, colH)
		{
			var previousSpriteId = this.get_sprite(spriteFileName);
			//If the image loaded
			if( previousSpriteId == IS_NULL )
			{
				var newSprite = new GPE_GameSprite();
				newSprite.spriteName = spriteFileName;
				newSprite.spriteWidth = frameWidth;
				newSprite.spriteHeight = frameHeight;
				newSprite.id=setId;
				if( spriteFileName.length > 4)
				{
					var isPreviouslyLoaded = false;
					
					//finds preloaded images to load
					newSprite.update_sprite_data(this.load_new_image(spriteFileName),xStart, yStart, frameWidth, frameHeight);
				}
				newSprite.colX = 0;
				newSprite.colY = 0;
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

		this.add_spritesheet= function (setId,spriteFileName, xStart, yStart,frameWidth, frameHeight, spriteShape, colX, colY, colW, colH)
		{
			return this.add_sprite(setId,spriteFileName, xStart, yStart,frameWidth, frameHeight, spriteShape, colX, colY, colW, colH);
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
			if( newImageSrc ==IS_NULL)
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
					if( foundTex.src==idIn)
					{
						return foundTex;
					}
				}
			}
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
				for( i=0; i< this.rAudio.length; i++)
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
				for( i=0; i< this.rAudio.length; i++)
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
				for( i=0; i< this.rTextures.length; i++)
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
				for( i=0; i< this.rTextures.length; i++)
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
				for( i=0; i< this.rTilesheets.length; i++)
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
				for( i=0; i< this.rTilesheets.length; i++)
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
			var foundTex = IS_NULL;
			var i=0;
			if( typeof idIn =="number" )
			{
				if( idIn >=0 && idIn < this.rSprites.length)
				{
					return idIn;
				}
				for( i=0; i< this.rSprites.length; i++)
				{
					foundTex = this.rSprites[i];
					if( foundTex.texId==idIn )
					{
						return i;
					}
				}
			}
			else if( typeof idIn =="string" )
			{
				for( i=0; i< this.rSprites.length; i++)
				{
					foundTex = this.rSprites[i];
					if( foundTex.fileLocation==idIn || foundTex.name==idIn)
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
		this.imagesLoaded = 0;
		this.audioLoaded = false;
		this.spritesLoaded = false;
		this.percentLoaded = 0;
		this.latestLoadMessage = "";
		//For now images take up the main amount of resources.
		//This will either be a full even split or actually dependent on amount of files per category
		this.imagePercentage = 90;
		this.musicPercentage = 0;
		this.spritesPercentage = 10;
		this.postLoadPauseTick = 0;
		this.postLoadPauseMax = 60;
		this.loadingBegan = false;
		
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
					this.postLoadPauseTick += 1;
					if( this.postLoadPauseTick>=this.postLoadPauseMax )
					{
						this.postLoadPauseTick = 0;
						this.loadingBegan = true;
						var ii;
						var imagesToPreload = GPE_PreloadImageResource.length;
						var newImageToLoad = IS_NULL;
						for (ii = 0; ii < imagesToPreload; ii += 1)
						{
							newImageToLoad = new Image();
							newImageToLoad.onerror = function(){ this.alt = "failed"; console.log("Failed to load image["+this.src+"]...");}
							newImageToLoad.src = GPE_PreloadImageResource[ii];
							newImageToLoad.alt = "false";
							preloadedImages.push(newImageToLoad);
						}
					}
				}
				else
				{
					for (iLoop = 0; iLoop < imageCount; iLoop++)
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
			gpe.context.fillRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
			gpe.context.textBaseline="top"; 
			if( gpe.preload_completed() )
			{
				gpe.context.font = 'bold 48pt Calibri';
				gpe.context.textAlign = 'right';
				gpe.context.textBaseline="bottom"; 
				gpe.context.fillStyle = '#FFF';
				gpe.context.fillText('Loading Complete', gpe.SCREEN_WIDTH-32, gpe.SCREEN_HEIGHT-32 );
			}
			else
			{
				gpe.context.font = 'bold 48pt Calibri';
				gpe.context.textAlign = 'left';
				gpe.context.textBaseline="top"; 
				gpe.context.fillStyle = '#FFF';
				if( this.loadingBegan==false)
				{
					gpe.context.fillText('Now loading...', 32, 32 );
				}
				else
				{
					gpe.context.fillText('Loading Resources', gpe.SCREEN_WIDTH-32, gpe.SCREEN_HEIGHT-32);
					gpe.context.font = 'bold 48pt Calibri';
					gpe.context.textAlign = 'right';
					gpe.context.textBaseline="bottom"; 
					gpe.context.fillText(this.imagesLoaded +" / "+ imageCount, gpe.SCREEN_WIDTH-32, gpe.SCREEN_HEIGHT-32 );
					gpe.context.fillText(this.latestLoadMessage, gpe.SCREEN_WIDTH-64, gpe.SCREEN_HEIGHT-96);
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
			if( player1Controller!=IS_NULL)
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
			gpe.context.fillRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
			
			if( gpe.preload_completed() )
			{
				gpe.context.drawImage(this.menuBackground, 0, 0,  gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
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
		this.GPE_APP_VERSION_NUMBER  = 1.0;
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
		this.MOUSE_X = 0;
		this.MOUSE_Y = 0;
		this.currentState = 0;
		
		this.OBJECT_RENDER_CHECKS = 0;
		this.OBJECT_RENDER_CALLS = 0;
		this.PRELOAD_COMPLETE = 0;
		
		this.EXTRA_CANVASES = [];
		this.maxCameras = 8;
		this.maxSceneBackgrounds = 8;
		this.deviceType = deviceIn;
		
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
		this.preGuiCamvas = IS_NULL;
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
				if( fPlugin!=IS_NULL)
				{
					fPlugin.process_plugin();
				}
			}
		};
		
		this.add_gamescene = function()
		{
			var newScene = new GPE_GameScene();
			newScene.sceneIdNumber = this.gpeScenes.length;
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
			return this.is_audio_playing(audioIn);
		};

		this.audio_stop_all = function() 
		{
			this.stop_all_audio();
		};
		
		this.bound_number = function( numbIn,  xMin, xMax)
		{
			if( numbIn < xMin)
			{
				numbIn = xMin;
			}
			else if( numbIn > xMax)
			{
				numbIn = xMax;
			}
			return numbIn;
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
		
		this.enter_render = function()
		{
			gpe.GPE_Current_Context  = gpe.context;
			gpe.render_rectangle(0,0,1024,720,"blue",false,4);
			if( gpe.isPlayingVideo==false && gpe.currentState != 0)
			{
				gpe.currentState.render_state();
			}
			this.render_top_hud();
			SYS_FPS = countFPS();
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
			gpe.isPlayingVideo  = false;
			gpe.currentVideoPlaying= -1;
		};
		
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
		
		this.get_camera_screen_height = function(cameraId)
		{
			return GPR.get_camera_screen_height( cameraId);
		}
		
		this.get_camera_screen_width = function()
		{
			return GPR.get_camera_screen_width(this.cameraId);
		}
		
		this.get_camera_screen_x = function(cameraId)
		{
			return GPR.get_camera_screen_x( cameraId);
		}
		
		this.get_camera_screen_y = function(cameraId)
		{
			return GPR.get_camera_screen_y( cameraId);
		}
		
		this.get_current_camera_info = function()
		{
			return GPR.get_camera_info(this.currentCameraInView);
		}
		
		this.get_direction = function( x1, y1, x2, y2)
		{
			if(x1==x2&&y1==y2)
			{
				return 0;
			}
			return Math.atan2(y2-y1,x2-x1);
		};

		this.get_distance = function( x1,  y1,  x2,  y2)
		{
			if(x1==x2&&y1==y2)
			{
				return 0;
			}
			return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
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
		/**
		* The GPE_GameObject Class.
			default_load/initialize stuff should be done in the GPE_GameObject constructor.
		*/
		this.GPE_GameObject = (function () 
		{
			function GPE_GameObject(xPosIn, yPosIn)
			{
				/************ PLEASE ONLY USE THE FOLLOWING VARIABLES AS GETTERS/SETTERS TO AVOID BUGS *///////////////
				this.spriteId = IS_NULL;
				this.outlineTestColor = 'lime';
				this.objectDepth  = 0;
				this.prevObjectDepth  = 0;
				this.objectDepthPosition = 0;
				this.gpeObjectType = 0;
				this.GPE_AddedToGridSystem = false;
				this.logicAppliedInFrame = false;
				this.prelogicAppliedInFrame = false;
				
				this.prerenderedInFrame = [];
				this.renderedInFrame = [];
				this.hudrenderedInFrame = [];
				
				for( var iView = 0; iView < gpe.maxCameras; iView++)
				{
					this.prerenderedInFrame[iView] = false;
					this.renderedInFrame[iView] = false;
					this.hudrenderedInFrame[iView] = false;
				}
				this.collisionBox = new GPE_Rect();
				this.collisionBox.update_shape( xPosIn, yPosIn, 32, 32);
				/************ END OF GETTER/SETTER ONLY VARIABLES *///////////////
				this.isOutsideRoom = false;
				this.lastCollisionObjId = -1;
				this.applyGravity = true;
				this.gravityDirection = 270;
				this.gravityEffect = 0;
				this.jumpCount = 1;
				this.spriteFrame = 0;
				this.imageSpeed = 0.125;
				this.animationEnded = false;
				this.isSolid = false;
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
				this.xPrev = 0;
				this.x2Prev = 0;
				this.yPrev = 0;
				this.y2Prev = 0;
				this.xspd = 0;
				this.yspeed = 0;				
				this.objectMovedInThisFrame = true;
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
				
				this.collide_with = function(otherObj)
				{
				
				};
				
				this.initialize = function ()
				{
					// Call superclass constructor
					this.default_load();
				}

				this.default_load = function ()
				{
					this.spriteId = 	IS_NULL;
					this.lastCollisionObjId = IS_NULL;
					this.applyGravity = true;
					this.gravityDirection = 270;
					this.gravityEffect = 0;
					this.jumpCount = 1;
					this.spriteFrame = 0;
					this.imageSpeed = 0.125;
					this.animationEnded = false;
					this.isSolid = false;
					this.isVisible = false;
					this.xEnd = 0;
					this.yEnd = 0;
					this.xVel = 0;
					this.yVel = 0;
					this.xPrev = 0;
					this.yPrev = 0;
					this.x2Prev = 0;
					this.y2Prev = 0;
					this.xspd = 0;
					this.yspeed = 0;
					this.direction = 0;
					this.speed = 0;
					this.defaultSpeed = 0;
					this.animationDuration = 0;
					this.uniqGameObjectId = -1;
					this.SPATIAL_PARTITION_LIST = [];
				}

				this.gpe_constructor = function()
				{
					// Call superclass constructor
					this.default_load();
				}
				
				this.get_depth = function ()
				{
					return this.objectDepth;
				}
				
				this.get_collision_height = function()
				{
					return this.collisionBox.rectHeight;
				}
				
				this.get_collision_width = function()
				{
					return this.collisionBox.rectWidth;
				}
				
				this.get_my_direction = function(  x2, y2)
				{
					return gpe.get_direction(this.collisionBox.xPos,this.collisionBox.yPos, x2,y2);
				};

				this.get_my_distance = function(  x2,  y2)
				{
					return gpe.get_distance(this.collisionBox.xPos,this.collisionBox.yPos, x2,y2);
				};
				
				this.get_sprite = function()
				{
					return this.spriteId;
				}
				
				this.getx = function ( )
				{
					return this.xPos;
				}
				
				this.getx2 = function ( )
				{
					return this.collisionBox.x2Pos;
				}
				
				this.gety = function ( )
				{
					return this.yPos;
				}
				
				this.gety2 = function ( )
				{
					return this.collisionBox.y2Pos;
				}
				
				this.end_frame_logic  = function()
				{
				
				}
				
				this.gravity_use = function ()
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
							this.xspd+=Math.cos(this.gravityDirection)*this.gravityEffect;
							this.yspeed+=Math.sin(this.gravityDirection)*this.gravityEffect;
							
							this.move_right(this.xspd);
							this.move_down(this.yspeed);
						}
					}
				}

				//this function is used to simply set the sprite( not updated in collision system)
				this.init_sprite = function(nextSprite )
				{
					this.spriteId = nextSprite;
					this.spriteFrame = 0;
					if( typeof this.spriteId!="undefined")
					{
						if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
						{
							var fSprite = gpe.rsm.rSprites[this.spriteId];
							this.collisionBox.update_size(fSprite.spriteWidth,fSprite.spriteHeight);
						}
						else
						{
							this.collisionBox.update_size(32,32);
						}
					}
				}
				
				this.make_view_dependent = function()
				{
					this.isViewDependent = false;
					var fObjToCheck = IS_NULL;
					for( var i=GPR.GAME_OBJECTS_VIEW_INDEPENDENT.length-1; i >=0; i--)
					{
						fObjToCheck = GPR.GAME_OBJECTS_VIEW_INDEPENDENT[i];
						if( fObjToCheck.uniqGameObjectId == this.uniqGameObjectId)
						{
							GPR.GAME_OBJECTS_VIEW_INDEPENDENT.splce(i);
						}
					}
				}

				this.make_view_independent = function()
				{
					this.isViewIndependent = true;
				}
				
				this.move_left = function ( leftMore)
				{
					this.collisionBox.add_x(-leftMore);
					this.xPos = this.collisionBox.xPos;
					GPR.update_object_in_map(this);
				}
				
				this.move_right = function ( rightMore)
				{
					this.collisionBox.add_x(rightMore);
					this.xPos = this.collisionBox.xPos;
					GPR.update_object_in_map(this);
				}
				
				this.move_up = function ( upMore)
				{
					this.collisionBox.add_y( -upMore);
					this.yPos = this.collisionBox.yPos;
					GPR.update_object_in_map(this);
				}
				
				this.move_down = function ( downMore)
				{
					this.collisionBox.add_y( downMore);
					this.yPos = this.collisionBox.yPos;
					GPR.update_object_in_map(this);
				}
								
				this.perform_object_logic = function ()
				{
					if( gpe.inputController.keyIsReleased[gp_r] )
					{
						var randX = gpe.get_random(0,6400);
						var randY = gpe.get_random(0,768);
						this.update_coords( randX,randY);
					}
				};
				
				this.perform_object_prelogic = function ()
				{
					this.imageSpeed = 0.125;
					this.outlineTestColor = 'lime';
					if(this.collisionBox.xPos ==this.xPrev && this.collisionBox.yPos ==this.yPrev && this.collisionBox.x2Pos ==this.x2Prev && this.collisionBox.y2Pos ==this.y2Prev)
					{
						this.objectMovedInThisFrame = false;
					}
					else
					{
						this.objectMovedInThisFrame = true;
					}
					this.xPrev = this.collisionBox.xPos;
					this.yPrev = this.collisionBox.yPos;
					this.x2Prev = this.collisionBox.x2Pos;
					this.y2Prev = this.collisionBox.y2Pos;
					this.logicAppliedInFrame = false;
					this.prelogicAppliedInFrame = true;
					for( var iView = 0; iView < gpe.maxCameras; iView++)
					{
						this.prerenderedInFrame[iView] = false;
						this.renderedInFrame[iView] = false;
						this.hudrenderedInFrame[iView] = false;
					}
				};
												
				this.process_timed_functions = function()
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
				
				this.render_under = function ()
				{

				}

				this.render_hud = function ()
				{

				}
				
				this.render_self = function ()
				{
					if (gpe.preload_completed() && this.spriteId!=IS_NULL)
					{
						gpe.OBJECT_RENDER_CALLS+=1;
						gpe.render_sprite(this.spriteId, this.spriteFrame,this.xPos, this.yPos,1,1);
					}
					else
					{
						gpe.render_rectangle(this.collisionBox.xPos,this.collisionBox.yPos,this.collisionBox.x2Pos,this.collisionBox.y2Pos,'blue',true,1);	
					}
					gpe.render_calls++;
				}
				
				this.scene_end = function()
				{
				
				}
				
				this.scene_start = function()
				{
				
				}
				
				this.self_destruct = function ()
				{
					if( this.gpeIsBeingDestroyed==false )
					{
						this.self_obliterate();
						this.gpeIsBeingDestroyed = true;
					}
				}			
				
				this.set_depth = function (newDepth)
				{
					if( this.depth!= newDepth)
					{
						
					}
				}
				
				this.self_obliterate = function ()
				{
					
				}			
				
				this.set_sprite = function(nextSprite )
				{
					if( this.spriteId!=nextSprite)
					{
					
						this.spriteId = nextSprite;
						this.spriteFrame = 0;
						if( typeof this.spriteId!="undefined")
						{
							if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
							{
								var fSprite = gpe.rsm.rSprites[this.spriteId];
								this.collisionBox.update_size(fSprite.spriteWidth,fSprite.spriteHeight);
							}
							else
							{
								this.collisionBox.update_size(32,32);
							}
							GPR.update_object_in_map(this);
						}
					}
				}
				
				this.setx = function ( newX)
				{
					this.collisionBox.set_x( newX);
					this.xPos = newY;
					GPR.update_object_in_map(this);
				}
				
				this.sety = function ( newY)
				{
					this.collisionBox.set_y( newY);
					this.yPos = newY;
					GPR.update_object_in_map(this);
				}
				
				this.speed_move = function ()
				{
					//GPR.update_object_in_map(this);
				}
				
				this.start_countdown = function( timerId, countDownStart)
				{
					if( timerId >=0 && timerId <= 10 && typeof countDownStart =="number")
					{
						this.functionTimer[timerId] = countDownStart;
					}
				}
				
				this.start_frame_logic = function()
				{
				
				}
				
				this.start_timer = function( timerId, gameFrames)
				{
					if( timerId >=0 && timerId <= 10 && typeof gameFrames =="number")
					{
						this.functionTimer[timerId] = gameFrames*60;
					}
				}
				
				this.update_animation = function (evt)
				{
					if(this.spriteId!=IS_NULL && this.imageSpeed!=0)
					{
						if( this.spriteId>=0 && this.spriteId < gpe.rsm.rSprites.length )
						{
							var fSprite = gpe.rsm.rSprites[this.spriteId];
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
				}

				this.update_coords = function (newX, newY)
				{
					this.collisionBox.update_position(newX,newY);
					this.xPos = this.collisionBox.xPos;
					this.yPos = this.collisionBox.yPos;
					GPR.update_object_in_map(this);
				}
			}

			return GPE_GameObject;
		}());
		
		//returns the number of objects of the object type
		this.find_numb_of_objects = function( theObjectType)
		{
			return GPR.find_numb_of_objects(theObjectType);
		}
		
		this.find_object_at_pos = function(theObjectType, pos)
		{
			return GPR.find_object_at_pos(theObjectType,pos);
		};
		
		//gets the last object of theObjectType
		this.find_last_object = function( theObjectType)
		{
			return GPR.find_last_object(theObjectType);
		}
		
		this.getfps = function()
		{
			return SYS_FPS;
		}
		
		this.get_fps = function()
		{
			return SYS_FPS;
		}
		
		this.get_mouse_x = function()
		{
			return this.MOUSE_X;
		}
		
		this.get_mouse_y = function()
		{
			return this.MOUSE_Y;
		}
		
		//gets the last object of theObjectType
		this.get_scene_height = function(  sceneIdNumberIn )
		{
			if( typeof sceneIdNumberIn === 'undefined' || sceneIdNumberIn === null )
			{
				return GPR.get_scene_height();
			}
			else if( sceneIdNumberIn >= 0 && sceneIdNumberIn < this.gpeScenes.length )
			{
				return this.gpeScenes[sceneIdNumber].sceneHeight;
			}
			else
			{
				return GPR.get_scene_height();
			}
		}
		
		this.get_scene_width = function( sceneIdNumberIn)
		{
			if( typeof sceneIdNumberIn === 'undefined' || sceneIdNumberIn === null )
			{
				return GPR.get_scene_width();
			}
			else if( sceneIdNumberIn >= 0 && sceneIdNumberIn < this.gpeScenes.length )
			{
				return this.gpeScenes[sceneIdNumber].sceneWidth;
			}
			else
			{
				return GPR.get_scene_width();
			}
		}
		
		//gets the last object of theObjectType
		this.find_newest_object = function( theObjectType)
		{
			return GPR.find_newest_object(theObjectType);
		}
		
		//returns the first object of otherObjectType that objOne collides with if it is at (xIn,yIn)
		this.find_collision_rectangle = function(  objOne,  xIn,  yIn,  otherObjectType)
		{
			return GPR.find_collision_rectangle(objOne,  xIn,  yIn,  otherObjectType);
		};
		
		this.is_audio_playing = function(audioIdIn)
		{
			if(typeof audioIdIn!="undefined")
			{
				if( audioIdIn>=0 && audioIdIn < this.rsm.rAudio.length )
				{
					var fAudio = this.rsm.rAudio[audioIdIn];
					if( fAudio!=IS_NULL)
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
					if( fVideo!=IS_NULL)
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
		}
		
		this.loop_last_video = function()
		{
			//console.log("Looping ["+gpe.lastLoopedTrack+"]....");
			gpe.play_video(gpe.lastLoopedVideo,true);
		}
		
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
		}
		
		this.pause_audio = function(audioIdIn)
		{
			if(typeof audioIdIn!="undefined")
			{
				if( audioIdIn>=0 && audioIdIn < this.rsm.rAudio.length )
				{
					var fAudio = this.rsm.rAudio[audioIdIn];
					if( fAudio!=IS_NULL)
					{
						fAudio.pause_audio();
					}
				}
			}
		};
		
		this.pause_all_audio = function()
		{
			for(var fAudioId = 0;fAudioId < this.rsm.rAudio.length; fAudioId++ )
			{
				var fAudio = this.rsm.rAudio[fAudioId];
				if( fAudio!=IS_NULL)
				{
					fAudio.pause_audio();
				}
			}
		};
		
		this.play_audio = function(audioIdIn,loops)
		{
			if(typeof loops=="undefined")
			{
				var loops  = false
			}
			if(typeof audioIdIn!="undefined")
			{
				if( audioIdIn>=0 && audioIdIn < this.rsm.rAudio.length )
				{
					var fAudio = this.rsm.rAudio[audioIdIn];
					if( fAudio!=IS_NULL)
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
				for(var fAudioId = 0;fAudioId < this.rsm.rAudio.length; fAudioId++ )
				{
					var fAudio = this.rsm.rAudio[fAudioId];
					if( fAudio!=IS_NULL)
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
					var fVideo = this.rsm.rVideos[videoIdIn];
					if( fVideo!=IS_NULL)
					{
						fVideo.pause_video();
					}
				}
			}
		};
		
		this.pause_all_video = function()
		{
			for(var fVideoId = 0;fVideoId < this.rsm.rVideos.length; fVideoId++ )
			{
				var fVideo = this.rsm.rVideos[fVideoId];
				if( fVideo!=IS_NULL)
				{
					fVideo.pause_video();
				}
			}
		};
		
		this.play_video = function(videoIdIn,loops, isVideoSkippable)
		{
			if(typeof loops=="undefined")
			{
				var loops  = false
			}
			if(typeof videoIdIn!="undefined")
			{
				if( videoIdIn>=0 && videoIdIn < this.rsm.rVideos.length )
				{
					var fVideo = this.rsm.rVideos[videoIdIn];
					if( fVideo!=IS_NULL)
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
				for(var fVideoId = 0;fVideoId < this.rsm.rVideos.length; fVideoId++ )
				{
					var fVideo = this.rsm.rVideos[fVideoId];
					if( fVideo!=IS_NULL)
					{
						if( fVideo.videoGroupName == videoGroupId)
						{
							fVideo.pause_video();
						}
					}
				}
			}
		};
		
		this.position_collides = function(  objOne,  xIn,  yIn,  otherObjectType)
		{
			return GPR.find_collision_rectangle(objOne,  xIn,  yIn,  otherObjectType);
		}
		
		//checks if the position is open and free of ANYTHING  or of the otherObjectType
		this.position_open = function(  xIn, yIn, otherObjectType)
		{
			return GPR.position_open( xIn, yIn, otherObjectType);
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
		}
		
		this.preload_completed = function()
		{
			return this.PRELOAD_COMPLETE;
		}
		
		this.get_random = function(min,max)
		{
			return  ( (Math.random()*(max-min+1)+min)|0 );
		}
		
		this.render_circle = function( x, y, radius, circleColor, isOutline, oulineWidth, renderImmediately)
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
		}
		
		this.render_current_video = function()
		{
			if( this.currentVideoPlaying>=0 && this.currentVideoPlaying < this.rsm.rVideos.length )
			{
				var fVideo = this.rsm.rVideos[this.currentVideoPlaying];
				if( fVideo!=IS_NULL)
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
		}
		
		this.render_rect = function(rectIn,rectColor, outline,rectLineWidth,renderImmediately)
		{
			if( typeof rectIn=="undefined")
			{
				this.render_rectangle(rectIn.xPos ,rectIn.yPos ,rectIn.x2Pos ,rectin.y2Pos ,rectColor,outline,rectLineWidth);
			}
		}
	
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
		}
		
		this.render_sprite = function(spriteIdIn, frameNumb, xOn, yOn, scaleX, scaleY)
		{
			if(typeof spriteIdIn!="undefined")
			{
				if( spriteIdIn>=0 && spriteIdIn < gpe.rsm.rSprites.length )
				{
					var fSprite = gpe.rsm.rSprites[spriteIdIn];
					if( fSprite!=IS_NULL)
					{
						fSprite.render_to_context(gpe.GPE_Current_Context, frameNumb, xOn, yOn, scaleX, scaleY);
					}
				}
			}
		}
		
		this.render_sprite_rotated = function(spriteIdIn, frameNumb, xOn, yOn, rotationAngle,scaleX, scaleY)
		{
			if(typeof spriteIdIn!="undefined")
			{
				if( spriteIdIn>=0 && spriteIdIn < gpe.rsm.rSprites.length )
				{
					var fSprite = gpe.rsm.rSprites[spriteIdIn];
					if( fSprite!=IS_NULL)
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
		}
		
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
					gpe.GPE_Current_Context.fillRect(xx, yy, squareSize,squareSize);
				}
			}
		}
		
		this.render_text = function(fontIdIn, xOn, yOn, textToRender, fontRenderColor, fontHalign, fontValign)
		{
			if(typeof fontIdIn!="undefined"&& textToRender.length > 0)
			{
				if( fontIdIn>=0 && fontIdIn < gpe.rsm.rFonts.length )
				{
					var fFont = gpe.rsm.rFonts[fontIdIn];
					if( fFont!=IS_NULL)
					{
						xOn-=gpe.CURRENT_VIEW_BOX.xPos;
						yOn-=gpe.CURRENT_VIEW_BOX.yPos;
						gpe.GPE_Current_Context.font = fFont.fontCanvasInfo;
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
						gpe.GPE_Current_Context.fillText(textToRender,xOn,yOn);
						
					}
				}
			}
		}
		
		this.render_texture = function(texureIdIn,  xDraw, yDraw)
		{
			if(typeof texureIdIn!="undefined")
			{
				if( texureIdIn>=0 && texureIdIn < gpe.rsm.rTextures.length )
				{
					var fTexture = gpe.rsm.rTextures[texureIdIn];
					if( fTexture!=IS_NULL)
					{
			
						fTexture.render_to_context(gpe.GPE_Current_Context, xDraw, yDraw);
					}
				}
			}
		}
		
		this.render_texture_scaled = function(  texureIdIn, xDraw, yDraw, width, height )
		{
			if(typeof texureIdIn!="undefined")
			{
				if( texureIdIn>=0 && texureIdIn < gpe.rsm.rTextures.length )
				{
					var fTexture = gpe.rsm.rTextures[texureIdIn];
					if( fTexture!=IS_NULL)
					{
						fTexture.render_tex_scaled(gpe.GPE_Current_Context, xDraw, yDraw);
					}
				}
			}
		}
		
		this.render_texture_clipped = function(  texureIdIn,xDraw, yDraw, clipBox )
		{
			if(typeof texureIdIn!="undefined")
			{
				if( texureIdIn>=0 && texureIdIn < gpe.rsm.rTextures.length )
				{
					var fTexture = gpe.rsm.rTextures[texureIdIn];
					if( fTexture!=IS_NULL)
					{
						fTexture.render_tex_clipped(gpe.GPE_Current_Context, xDraw, yDraw,clipBox);
					}
				}
			}
		}
		
		this.render_top_hud = function ()
		{
			if( gpe.isPlayingVideo )
			{
				this.render_current_video();
			}
			
			if( GPE_SETTINGS_IS_DEBUGGING )
			{
				gpe.context.font = 'bold 16pt Calibri';
				gpe.context.fillStyle = '#FFF';


				gpe.context.font = 'bold 16pt Calibri';
				gpe.context.textAlign = 'right';
				gpe.context.fillStyle = '#FFF';
				
				var fController = IS_NULL;
				var gTemp = 0;
				for (gTemp = 0; gTemp < gpe.inputController.game_controllers.length; gTemp++)
				{		
					fController = gpe.inputController.game_controllers[ gTemp];
					if( fController.isConnected)
					{
						gpe.context.fillText("["+gTemp+"] "+fController.controllerType+" | "+fController.butIsDown[gpe.cbi.UP_BUT]+" | "+fController.butIsDown[gpe.cbi.LEFT_BUT]+" | "+fController.butIsDown[gpe.cbi.RIGHT_BUT]+" | "+fController.butIsDown[gpe.cbi.DOWN_BUT]+" | "+fController.butIsDown[gpe.cbi.L_BUT]+" | "+fController.butIsDown[gpe.cbi.R_BUT], gpe.SCREEN_WIDTH-32,48*(2+gTemp ) );
					}
				}
				var p1Controller = gpe.inputController.get_controller(1);
				if( p1Controller!=IS_NULL)
				{
					gpe.context.fillText("Player 1: "+p1Controller.controllerType+" | "+p1Controller.butIsReleased[gpe.cbi.UP_BUT]+" | "+p1Controller.butIsReleased[gpe.cbi.LEFT_BUT]+" | "+p1Controller.butIsReleased[gpe.cbi.RIGHT_BUT]+" | "+p1Controller.butIsReleased[gpe.cbi.DOWN_BUT]+" | "+p1Controller.butIsReleased[gpe.cbi.L_BUT]+" | "+p1Controller.butIsReleased[gpe.cbi.R_BUT], gpe.SCREEN_WIDTH-32,24 );
				}
				else
				{
					gpe.context.fillText("Player 1 Not Found", gpe.SCREEN_WIDTH-32,24 );
				}
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
		}
		
		this.render_wrapped_text = function(fontIdIn, xOn, yOn, textToRender,fontRenderColor, maxWidth, lineHeight)
		{
			if(typeof fontIdIn!="undefined"&& textToRender.length > 0)
			{
				if( fontIdIn>=0 && fontIdIn < gpe.rsm.rFonts.length )
				{
					var fFont = gpe.rsm.rFonts[fontIdIn];
					if( fFont!=IS_NULL)
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
		}
		
		this.save_data = function( dataIdentifier, dataTitle, actualData)
		{
			if (typeof(Storage) !== "undefined")
			{
				localStorage.setItem(dataIdentifier+"_"+dataTitle,actualData);
			}
		}
		
		this.scene_enter_next = function()
		{
			GPR.scene_enter_next();
		}
		
		this.scene_enter_previous = function()
		{
			GPR.scene_emter_previous();
		}
		
		this.scene_enter = function( nextSceneId )
		{
			GPR.scene_enter(nextSceneId);
		}
		
		this.setup_camera = function(cameraId, cameraX,cameraY, cameraW, cameraH)
		{
			GPR.setup_camera(cameraId, cameraX, cameraY, cameraW, cameraH);
		}
		
		this.set_render_alpha = function( alphaValue)
		{
			if( alphaValue >=0 && alphaValue <=255)
			{
				gpe.GPE_Current_Context.globalAlpha = alphaValue/255;
			}
			else
			{
				gpe.GPE_Current_Context.globalAlpha = 1;
			}
		}
		
		
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
		
		this.sign = function( x )
		{
			return typeof x === 'number' ? x ? x < 0 ? -1 : 1 : x === x ? 0 : NaN : NaN;
		}
		
		this.stop_all_audio = function()
		{
			for(var fAudioId = 0;fAudioId < this.rsm.rAudio.length; fAudioId++ )
			{
				var fAudio = this.rsm.rAudio[fAudioId];
				if( fAudio!=IS_NULL)
				{
					fAudio.stop_audio();
				}
			}	
		};
		
		this.stop_audio_group = function(audioGroupId)
		{
			if(typeof audioGroupId!="undefined")
			{
				for(var fAudioId = 0;fAudioId < this.rsm.rAudio.length; fAudioId++ )
				{
					var fAudio = this.rsm.rAudio[fAudioId];
					if( fAudio!=IS_NULL)
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
					var fAudio = this.rsm.rAudio[audioIdIn];
					if( fAudio!=IS_NULL)
					{
						fAudio.stop_audio();
					}
				}
			}
		};
		
		this.stop_all_video = function()
		{
			for(var fVideoId = 0;fVideoId < this.rsm.rVideos.length; fVideoId++ )
			{
				var fVideo = this.rsm.rVideos[fVideoId];
				if( fVideo!=IS_NULL)
				{
					fVideo.stop_video();
				}
			}	
		};
		
		this.stop_video_group = function(videoGroupId)
		{
			if(typeof videoGroupId!="undefined")
			{
				for(var fVideoId = 0;fVideoId < this.rsm.rVideos.length; fVideoId++ )
				{
					var fVideo = this.rsm.rVideos[fVideoId];
					if( fVideo!=IS_NULL)
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
					var fVideo = this.rsm.rVideos[videoIdIn];
					if( fVideo!=IS_NULL)
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
			if (evt.layerX || evt.layerX == 0)
			{
				gpe.MOUSE_X = evt.layerX;
				gpe.MOUSE_Y =  evt.layerY;
			}
			else if (evt.offsetX || evt.offsetX == 0)
			{
				gpe.MOUSE_Y = evt.offsetX;
				gpe.MOUSE_Y = evt.offsetY;
			}
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
			return this.is_video_playing(videoIn);
		}

		this.video_stop_all = function() 
		{
			this.stop_all_video();
		}
		
		this.gameloop = function()
		{
			window.requestAnimationFrame(function()
			{
				gpe.gameloop();
				
			});
			
			if( gpe.rsm!=IS_NULL)
			{
				gpe.rsm.check_resources_state();
			}

			gpe.apply_plugins_logics();
			if( gpe.isPlayingVideo )
			{
				if( gpe.currentVideoPlaying>=0 && gpe.currentVideoPlaying < gpe.rsm.rVideos.length )
				{
					var fVideo = gpe.rsm.rVideos[gpe.currentVideoPlaying];
					if( fVideo!=IS_NULL)
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
				gpe.currentState.apply_logic();
			}
			
			if( gpe.inputController!=IS_NULL)
			{
				gpe.inputController.reset_input();
			}
			gpe.apply_plugins_logics();
			
			change_state();
			gpe.OBJECT_RENDER_CHECKS = 0;
			gpe.OBJECT_RENDER_CALLS = 0;
			gpe.enter_render();
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
		//processes the mouse input within the canvas
		var eventObj = 0;
		if( typeof event =="undefined")
		{
			eventObj = window.event;
		}
		else
		{
			eventObj  = event;
		}
		
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
		
		for(var ii=0; ii< gpe.maxCameras; ii+=1)
		{
			gpe.EXTRA_CANVASES[ii] = gpe.add_canvas("exView"+ii,gpe.SCREEN_WIDTH,gpe.SCREEN_HEIGHT);
			gpe.EXTRA_CANVASES[ii].style.visibility = "hidden";
			gpe.EXTRA_CANVASES[ii].style.display = "none";
		}
		gpe.mapCanvas = gpe.add_canvas("gameMap",GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT);
		gpe.mapCanvas.className = "hide";
		
		gpe.preGuiCamvas = gpe.add_canvas("preGuiCamvas",GPE_SETTINGS_SCREEN_WIDTH,GPE_SETTINGS_SCREEN_HEIGHT);
		gpe.preGuiCamvas.className = "hide";
		
		gpe.fileReader = new GPE_FileReader();
		
		programStateId = programStates.STATE_LOGO_DISPLAY;
		gpe.currentState = new GPE_LogoDisplay();		
		GPR = gpe.currentState;
	}


	function init_gpe()
	{
		start();
		window.requestAnimationFrame( gpe.gameloop );
	};

	/**
	* The GPE_Rect Class.
	*/
	function GPE_Rect()
	{
		this.xPos = 0;
		this.yPos = 0;
		this.rectWidth = 0;
		this.rectHeight = 0;
		this.x2Pos = 0;
		this.y2Pos = 0;
		
		this.add_x = function( xPlus)
		{
			this.xPos+=xPlus
			this.x2Pos+=xPlus;
		}
		
		this.add_y = function( yPlus)
		{
			this.yPos+=yPlus;
			this.y2Pos+=yPlus;
		}
		
		this.get_center = function()
		{
			return this.xPos+this.rectWidth/2;
		};
		
		this.get_height = function()
		{
			return this.rectHeight;
		};
		
		this.get_middle = function()
		{
			return this.yPos+this.rectHeight/2;
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
		};
		
		this.set_y = function( newY )
		{
			this.yPos = newY;
			this.y2Pos = this.yPos+this.rectHeight;
		};
		
		this.update_position = function ( xNew, yNew)
		{
			this.xPos = xNew;
			this.yPos = yNew;
			this.x2Pos = this.xPos+this.rectWidth;
			this.y2Pos = this.yPos+this.rectHeight;
		};
		
		this.update_box = function ( xNew, yNew, widNew, hgtNew)
		{
			this.update_shape(xNew, yNew, widNew, hgtNew );
		}
		
		this.update_shape = function ( xNew, yNew, widNew, hgtNew)
		{
			this.xPos = xNew;
			this.yPos = yNew;
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
		};
		
	}

	/**
	* The GPE_GameSprite Class.
	*/
	function GPE_GameSprite()
	{
		this.imageObj;
		this.frameCount = 0;
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
			var countedFrames = 0;
			this.spriteWidth  =  this.colWidth = frameWidth;
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
		};

		this.render_to_context = function (contextTo, frameNumb, xOn, yOn, scaleX, scaleY)
		{
			xOn-=gpe.CURRENT_VIEW_BOX.xPos;
			yOn-=gpe.CURRENT_VIEW_BOX.yPos;
			if( typeof scaleX =="undefined")
			{
				var scaleX = 1;
			}
			if( typeof scaleY =="undefined")
			{
				var scaleY = 1;
			}
			if(frameNumb>= 0 && frameNumb < this.frameRect.length)
			{
				frameNumb  = frameNumb|0;
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
			if(this.tex!=IS_NULL)
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
				if( this.tex!=IS_NULL)
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
			if( this.imageSource!=IS_NULL)
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
			if(this.imageSource!=IS_NULL && this.textureFailedToLoad==false )
			{
				if( contextTo!=IS_NULL && this.textureIsFullyLoaded )
				{
					contextTo.drawImage(this.imageSource, xDraw, yDraw);
				}
			}
		};
		
		this.render_tex_scaled = function(  contextTo, xDraw, yDraw, width, height )
		{
			if(this.imageSource!=IS_NULL && this.textureFailedToLoad==false )
			{
				if( contextTo!=IS_NULL && this.textureIsFullyLoaded )
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
			if(this.imageSource!=IS_NULL)
			{
				if( contextTo!=IS_NULL && this.textureIsFullyLoaded && this.textureFailedToLoad==false  )
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
			this.backgroundTexture = newBG;
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
			if(this.backgroundTexture!=IS_NULL && this.stretchBG==false)
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
			this.backgroundTexture.render_tex( destination, xx*cameraScaleX, yy*cameraScaleY,this.backgroundTexture.get_width()*cameraScaleX,this.backgroundTexture.get_height()*cameraScaleY );
		};
		
		this.render_bg = function( renderToArea, cam, renderCam,lvlW, lvlH)
		{
			
			var colCam=false;
			if(this.backgroundTexture!=IS_NULL)
			{
				if( this.backgroundTexture.textureIsFullyLoaded )
				{
					if( this.loopBGHori )
					{
						 if( this.loopBGVert )
						 {
							  for( var i=this.bgXOffset-this.backgroundTexture.get_width();i<=lvlW+this.backgroundTexture.get_width(); i+=this.backgroundTexture.get_width() )
							  {
								   for( var j=this.bgYOffset-this.backgroundTexture.get_height();j<=lvlH; j+=this.backgroundTexture.get_height() )
								   {
										colCam=gpe.check_collision_with_rect (i,j,this.backgroundTexture.get_width(),this.backgroundTexture.get_height(),cam );
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
							 for(var i=this.bgXOffset-this.backgroundTexture.get_width();i<=lvlW+this.bgXOffset;i+=this.backgroundTexture.get_width())
							 {
								  colCam=gpe.check_collision_with_rect( i, this.bgYOffset, this.backgroundTexture.get_width(),this.backgroundTexture.get_height(),cam );
								  if(colCam)
								  {
									   //Show the background
									   this.render_bg_speed(-cam.get_x()+i, -cam.get_y()+this.bgYOffset,renderToArea, cam,renderCam);
								  }
							 }
						 }
					}
					else
					{
						if(this.loopBGVert)
						{
							 for(var j=this.bgYOffset-this.backgroundTexture.get_height();j<=sceneHeight;j+=this.backgroundTexture.get_height() )
							 {
								  colCam=gpe.check_collision_with_rect( this.bgXOffset,(j-this.bgYOffset),this.backgroundTexture.get_width() , this.backgroundTexture.get_height(),cam );
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
		
		this.render_self = function( renderToArea, cam, renderCam)
		{
			//ensures that the Tilesheet for the tile isn't null
			if(this.tset!=IS_NULL && this.tileIdNumb > 0)
			{
				if(this.tset.tex!=IS_NULL)
				{
					//checks to see if tile is in the camera region
					if(gpe.check_collision_with_rect(this.xCoord,this.yCoord-this.tileBox.get_height()-cam.get_y(),this.tileBox.get_width(),this.tileBox.get_height(),cam) == true )
					{
						var tsTex = gpe.rsm.rTilesheets[this.tset].tex;
						if( tsTex!=NULL)
						{
							renderToArea.drawImage(tsTex, this.tileBox.get_x(),this.tileBox.get_y(), this.tileBox.get_width(),this.tileBox.get_height(), this.xCoord, this.yCoord, this.tileBox.get_width(), this.tileBox.get_height() );
						}
					}
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
				if( tsTex!=IS_NULL)
				{
					tsTex = tsTex.tex;
					renderToArea.drawImage(tsTex, 
					this.tileBox.get_x(),this.tileBox.get_y(), this.tileBox.get_width(),this.tileBox.get_height(), 
					this.xCoord, this.yCoord, this.tileBox.get_width(), this.tileBox.get_height() );
				}
				else
				{
				  renderToArea.beginPath();
				  renderToArea.rect(this.xCoord, this.yCoord, this.tileBox.get_width(), this.tileBox.get_height());
				  
				  renderToArea.lineWidth = 7;
				  renderToArea.strokeStyle = 'blue';
				  renderToArea.stroke();
				}
			}
		
		};
	}

	function GPE_TileLayer()
	{
		this.tileAmountX = IS_NULL;
		this.tileAmountY = IS_NULL;
		this.layerId = IS_NULL;
		this.layerDepth = IS_NULL;
		this.layerName = "";
		this.prevTileAmountX = IS_NULL;
		this.prevTileAmountY = IS_NULL;
		this.layerTiles = [];
		
		this.get_tile_at = function( xIn,  yIn )
		{
			var seekedTile = xIn+ yIn*this.tileAmountX;
			if( this.layerTiles.size() > seekedTile && seekedTile>=0 )
			{
				return this.layerTiles.at(seekedTile);
			}
			return IS_NULL;
		};
		
		this.create_new_map = function( newTX, newTY)
		{
			this.tileAmountX=newTX;
			this.tileAmountY=newTY;
			var newSize = this.tileAmountX*this.tileAmountY;
			var i=0;
			var mpT = IS_NULL;
			if( this.layerTiles.size() >0 )
			{
				for(i=this.layerTiles.size();i>0;i--)
				{
					mpT = this.layerTiles[i];
					mpT = IS_NULL;
					this.layerTiles[i] = IS_NULL;
				}
			}
			var newTile = IS_NULL;
			for(i=0;i<newSize;i++)
			{
				newTile = new GPE_GameMapTile();
				newTile.tileType = 0;
				this.layerTiles[i] = newTile;
			}
		};
		
		this.get_map_size = function()
		{
			return this.layerTiles.size();
		};
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
		 
		 this.renderCanvas = IS_NULL;
		 this.renderContext = IS_NULL;
		 this.cSceneWidth = 0;
		 this.cSceneHeight = 0;
		 
		 if( cameraIdNumber >=0 && cameraIdNumber < gpe.maxCameras)
		 {
			this.renderCanvas = gpe.EXTRA_CANVASES[ cameraIdNumber];
			this.renderContext = gpe.EXTRA_CANVASES[ cameraIdNumber].getContext('2d');
		 }
		 
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
		 
		 this.setup_camera = function(cameraId, cameraX,cameraY, cameraW, cameraH)
		 {
			this.renderRect.update_shape(cameraX, cameraY, cameraW, cameraH);
		 }
		 
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

	function GPE_GameScene()
	{
		this.sceneName = "";
		this.sceneId = -1;
		this.sceneHasBeenActivated = false;
		this.sceneWidth = 1024;
		this.sceneHeight = 1024;
		gpe.sceneWidth = 1024;
		gpe.sceneHeight = 1024;
		this.isContinuousScene = false;
		
		this.defaultLayerName = "game - layer";
		
		//tile data info
		this.tileAmountX = 0;
		this.tileAmountY = 0;
		this.tileWidth = 32;
		this.tileHeight = 32;
		this.mapTextures = [];
		this.layerTilesheets = [];
		this.sceneObjects = [];
		this.tileMapLayers = new Array();
		
		this.scnStartBackgrounds = new Array();
		this.scnStartObjects = new Array();
		this.scnStartTiles = new Array();

		//background data
		//background data
		this.bgColor = "gray";
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
		for(var iV=0; iV < gpe.maxCameras; iV++)
		{
			this.sceneCamera[iV] = new GPE_SceneCamera(0,0,gpe.SCREEN2_WIDTH,gpe.SCREEN_HEIGHT,iV);
		}
		
		for(var iBg=0; iBg < gpe.maxSceneBackgrounds; iBg++)
		{
			this.BACKGROUNDS[iBg]  = IS_NULL;
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

		//returns the layer if it exists
		//@parameters: (var layerIdIn, var layerNameIn)
		this.get_layer = function(layerIdIn,  layerNameIn)
		{
			var foundLayer = IS_NULL;
			if( layerIdIn < this.tileMapLayers.length && layerIdIn>=0 )
			{
				foundLayer =this.tileMapLayers[layerIdIn];
				{
					if( foundLayer!=IS_NULL)
					{
						if( foundLayer.layerId==layerIdIn || foundLayer.layerName == layerNameIn)
						{
							return foundLayer;
						}
					}
				}
			}
			for( var i=0; i < this.tileMapLayers.length; i++ )
			{
				foundLayer = this.tileMapLayers[i];
				{
					if( foundLayer!=IS_NULL)
					{
						if( foundLayer.layerId==layerIdIn || foundLayer.layerName == layerNameIn)
						{
							return foundLayer;
						}
					}
				}
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
		
		//checks map forsolid
		this.check_map_for_solid = function( x1,  y1,  x2, y2, layerToCheck)
		{
			//thanks to the help of Yal
			// grid_check_solid(left, top, right, bottom): bool
			var i=0;
			var j=0;
			var l=0;
			var t=0;
			var r=0;
			var b=0;
			var d  = -1;
			// find rectangle to check (limited to field size):
			l = Math.max(0, Math.min(this.sceneWidth - 1, (x1 / this.tileWidth)|0 ) );
			t = Math.max(0, Math.min(this.sceneHeight - 1,(y1 / this.tileHeight)|0 ) );
			r = Math.max(0, Math.min(this.sceneWidth - 1, (x2 / this.tileWidth)|0 ) );
			b = Math.max(0, Math.min(this.sceneHeight - 1, (y2 / this.tileHeight)|0  ) );
			var cLayer = -1;
			// actual check:

			//if we have an actual layer to look for
			if(layerToCheck>-9999)
			{
				cLayer = this.get_layer(layerToCheck,defaultLayerName);
				if(cLayer!=IS_NULL)
				for (j = t; j <= b; j += 1)
				{
					for (i = l; i <= r; i += 1)
					{
						d = cLayer.get_tile_at(i, j);
						if(d!=IS_NULL)
						{
							if (d.tileType==tTILE_SOLID)
							{
								return true;
							}
						}
					}
				}
			}
			else
			{
				//search the entire map
				var tlay = 0;
				for(tlay = 0; tlay < this.tileMapLayers.length;tlay++)
				{
					cLayer = this.tileMapLayers[tlay];
					if(cLayer!=IS_NULL)
					{
						for (j = t; j <= b; j += 1)
						{
							for (i = l; i <= r; i += 1)
							{
								d = cLayer.get_tile_at(i, j);
								if(d!=IS_NULL)
								{
									if (d.tileType==tTILE_SOLID)
									{
										return true;
									}
								}
							}
						}
					}
				}
			}
			return false;
		};
		
		//takes the object and searches tilemap for collision with any possible collisions
		this.check_map_for = function( objBeingChecked,  xPos,  yPos,  tileTypeToSearchFor, layerToCheck)
		{
			if(objBeingChecked!=IS_NULL)
			{
				switch(tileTypeToSearchFor)
				{
					case tTILE_SOLID:
						if(this.check_map_for_solid(  xPos+1, yPos+1,objBeingChecked.get_sprite_bound_width()-1  + xPos, objBeingChecked.get_sprite_bound_height()-1 + yPos ,layerToCheck) )
						{
							return true;
						}
					break;

					default:

					break;
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
			this.sceneObjects = [];
			this.sceneIsSpatiallyPartitioned = false;
			var iItr = 0;
			var newObj = IS_NULL;
			for( iItr = 0; iItr < GPR.GAME_OBJECTS_CONTINUOUS.length; iItr++)
			{
				GPR.add_object(GAME_OBJECTS_CONTINUOUS[iItr]);
			}
			for( iItr = 0; iItr < this.scnStartObjects.length; iItr++)
			{
				newObj = gpe.add_new_object( this.scnStartObjects[iItr].objId, this.scnStartObjects[iItr].xPos, this.scnStartObjects[iItr].yPos );
			}
			var currTileToAdd = IS_NULL;
			var cTileLayer = IS_NULL;
			var cTile = IS_NULL;
			var currFoundTilesheet = IS_NULL;
			
			var newBG = IS_NULL;
			for( iItr = 0; iItr < this.scnStartBackgrounds.length; iItr++)
			{
				if( this.scnStartBackgrounds[iItr]!=IS_NULL)
				{
					newBG = new GPE_Background();
					if(  this.scnStartBackgrounds[iItr].bgTexId>=0 && this.scnStartBackgrounds[iItr].bgTexId < gpe.rsm.rTextures.length )
					{
						newBG.backgroundTexture = gpe.rsm.rTextures[this.scnStartBackgrounds[iItr].bgTexId ];
					}
					newBG.bgXOffset = this.scnStartBackgrounds[iItr].bgXPos;
					newBG.bgYOffset = this.scnStartBackgrounds[iItr].bgYPos;
					newBG.set_bgspeed(this.scnStartBackgrounds[iItr].bgXSpeed, this.scnStartBackgrounds[iItr].bgYSpeed);
					newBG.bgIsForeground=this.scnStartBackgrounds[iItr].bgInFont;
					newBG.loopBGHori = this.scnStartBackgrounds[iItr].bgTileHori;
					newBG.loopBGVert = this.scnStartBackgrounds[iItr].bgTileVert;
					newBG.stretchBG = this.scnStartBackgrounds[iItr].bgStretch;
					this.BACKGROUNDS[iItr] = newBG;
				}
			}
			var maxTileLayers = this.tileMapLayers.length;
			for( iItr = 0; iItr < this.scnStartTiles.length; iItr++)
			{
				currTileToAdd = this.scnStartTiles[iItr];
				if( currTileToAdd.tileLayerNumber>=0 && currTileToAdd.tileLayerNumber <maxTileLayers )
				{
					cTileLayer = this.tileMapLayers[ currTileToAdd.tileLayerNumber]
					if(currTileToAdd.tileNumber >=0 && currTileToAdd.tileNumber < cTileLayer.layerTiles.length )
					{
						cTile = cTileLayer.layerTiles[currTileToAdd.tileNumber];
						if( currTileToAdd.tileSheetId >=0 && currTileToAdd.tileSheetId < gpe.rsm.rTilesheets.length )
						{
							currFoundTilesheet = gpe.rsm.rTilesheets[currTileToAdd.tileSheetId];
							cTile.tset = currTileToAdd.tileSheetId;
							cTile.tileIdNumb = currTileToAdd.tileIndexId;
							if( currFoundTilesheet!=IS_NULL)
							{
								cTile.tileBox.update_box( currFoundTilesheet.get_xpos_at(currTileToAdd.tileIndexId),currFoundTilesheet.get_ypos_at(currTileToAdd.tileIndexId),currFoundTilesheet.get_tile_width(),currFoundTilesheet.get_tile_height() );
							}
						}
					}
				}
			}
			gpe.play_audio(this.startAudio,false);
			gpe.play_audio(this.bgMusic,true);
			this.sceneHasBeenActivated = true;
		}
		
		this.scene_restart = function()
		{
			for(var iV=0; iV < gpe.maxCameras; iV++)
			{
				this.sceneCamera[iV] = new GPE_SceneCamera(0,0,854,480,iV);
			}
			for(var iBg=0; iBg < gpe.maxSceneBackgrounds; iBg++)
			{
				this.BACKGROUNDS[iBg]  = IS_NULL;
			}
			var cLayer = IS_NULL;
			for(var tl=0; tl<this.tileMapLayers.length;tl++)
			{
				cLayer = this.tileMapLayers[tl];
				if(cLayer!=IS_NULL)
				{
					cLayer.layerTiles = [];
				}
			}
			this.sceneCamera[0].update_view_space(0,0,gpe.SCREEN_WIDTH/2, gpe.SCREEN_HEIGHT/2);
			this.sceneCamera[0].update_screen_space(0,0,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
			this.sceneCamera[0].isVisible = true;
			
			this.sceneCamera[1].update_view_space(0,0,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT/2);
			this.sceneCamera[1].update_screen_space(0,gpe.SCREEN_HEIGHT/2,gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT/2);
			this.sceneCamera[1].isVisible = false;
			
			this.sceneIsSpatiallyPartitioned = false;
			this.sceneHasBeenActivated = false;
		}
		
		this.update_scene_specs = function( lvlW, lvlH)
		{
			lvlW = Math.abs(lvlW);
			lvlH = Math.abs(lvlH);
			this.cSceneWidth = lvlW;
			this.cSceneHeight = lvlH;
			gpe.sceneWidth = lvlW;
			gpe.sceneHeight = lvlH;
			for(var iV=0; iV < gpe.maxCameras; iV++)
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
			for(var tl=0; tl<this.tileMapLayers.length;tl++)
			{
				cLayer = this.tileMapLayers[tl];
				if(cLayer!=IS_NULL)
				{
					//removes previous tile information
					
					//creates a brand new tilemap for each tile layer
					for( ii=0;ii<this.tileAmountY;ii++)
					{
						for( jj=0;jj<this.tileAmountX;jj++)
						{
							newTile = new GPE_GameMapTile();
							newTile.xCoord=jj*this.tileWidth;
							newTile.yCoord=ii*this.tileHeight;
							newTile.tileType= 0;
							cLayer.layerTiles[jj+(ii*this.tileAmountX)] = newTile;
						}
					}
				}
			}
			
			for(var iV=0; iV < gpe.maxCameras; iV++)
			{
				this.sceneCamera[iV].cameraBoundaries.update_shape(0,0,this.sceneWidth, this.sceneHeight);
			}
		};

		this.add_background = function( bSurf, loopH, loopV, bgX, bgY, bgXSpd, bgyspeed)
		{
			var newBG = new sceneBackground(bSurf, bgX, bgY);
			newBG.set_bgspeed(bgXSpd, bgyspeed);
			newBG.loopBGHori=loopH;
			newBG.loopBGVert=loopV;
			this.BACKGROUNDS.push(newBG);
		};

		this.add_foreground = function( bSurf, loopH,  loopV, bgX,  bgY, bgXSpd, bgyspeed)
		{
		  var newBG = new sceneBackground(bSurf, bgX, bgY);
		  newBG.set_bgspeed(bgXSpd, bgyspeed);
		  newBG.loopBGHori=loopH;
		  newBG.loopBGVert=loopV;
		  this.FOREGROUNDS.push(newBG);
		};
		
		this.add_tile_layer = function(newLayerName)
		{
			var newLayer = new GPE_TileLayer();
			newLayer.layerId = this.tileMapLayers.size();
			newLayer.layerName = newLayerName;
			newLayer.create_new_map(this.tileAmountX,this.tileAmountY);
			this.tileMapLayers.push_back(newLayer);
			return newLayer;
		}
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

		this.apply_prelogic = function()
		{
			var iTemp=0;
			var jTemp=0;
			var jTempMax=0;
			var objBeingProcessed;
			for (iTemp=0; iTemp<this.objectTypes; iTemp++)
			{
				//loops through the object list of iTemp objects
				jTempMax=this.find_numb_of_objects(iTemp);
				for(jTemp=0;jTemp<=jTempMax;jTemp++)
				{
					objBeingProcessed=this.find_object_at_pos(iTemp,jTemp);
					if(objBeingProcessed!=IS_NULL)
					{
						if( this.check_map_for(objBeingProcessed,objBeingProcessed.get_x(), objBeingProcessed.get_y() + 1, tTILE_SOLID ) )
						{
							objBeingProcessed.yspeed = 0;
						}
						else
						{
							objBeingProcessed.gravity_use();
							objBeingProcessed.speedMove();
						}
						objBeingProcessed.apply_prelogic();
					}
				}
			}
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
			if(this.sceneName.length()>0)
			{
				return sceneName;
			}
			else
			{
				return "";
			}
		}

		//gets the FIRST TEXTURE with id number# textId
		
		this.set_scene_name = function( inSceneName)
		{
			if(inSceneName.length>0)
			{
				this.sceneName = inSceneName;
			}
		}

		this.change_background = function( newBG, bgNumber)
		{
			if(this.BACKGROUNDS.size()>=bgNumber)
			{
				 if(this.BACKGROUNDS.at(bgNumber)!=IS_NULL)
				 {
					 this.BACKGROUNDS.at(bgNumber).set_background(newBG);
				 }
			}
		}

		this.change_foreground = function( newBG, bgNumber)
		{
			if(this.FOREGROUNDS.size()>=bgNumber)
			{
				 if(this.FOREGROUNDS.at(bgNumber)!=IS_NULL)
				 {
					 this.FOREGROUNDS.at(bgNumber).set_background(newBG);
				 }
			}
		}

		this.scroll_backgrounds = function()
		{
			for (var iTemp=0; iTemp<this.BACKGROUNDS.size(); iTemp++)
			{
				 if(this.BACKGROUNDS.at(iTemp)!=IS_NULL)
				 this.BACKGROUNDS.at(iTemp).scroll_bg();
			}
		}
		
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
		}
		
		this.load_and_parse_scene = function()
		{
			this.sceneIsProcessed = true;
		}
		
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
				var cLayer = IS_NULL;
				var cTile = IS_NULL;
				gpe.mapCanvas.width = this.sceneWidth;
				gpe.mapCanvas.height = this.sceneHeight;
				gpe.mapContext = gpe.mapCanvas.getContext('2d');
				gpe.GPE_Current_Context = gpe.preGuiContext = gpe.preGuiCamvas.getContext('2d');
				var fullMap = new GPE_Rect();
				fullMap.update_shape(0,0,this.sceneWidth, this.sceneHeight);
				var iTemp = IS_NULL;
				for (var hTemp=0; hTemp<this.tileMapLayers.length; hTemp++)
				{
					cLayer = this.tileMapLayers.at(hTemp);
					if(cLayer!=IS_NULL)
					{
						if(cLayer.layerName!="foreground")
						{
							for(  iTemp = 0; iTemp < cLayer.layerTiles.length; iTemp++)
							{
								cTile = cLayer.layerTiles.at(iTemp);
								if( cTile!=IS_NULL)
								{
									cTile.render_self_auto(gpe.mapContext );
								}
							}
						}
					}
				}
				this.sceneResourcesAreProcessed = true;
			}
			else
			{
				this.scroll_backgrounds();
			}
			return true;
		}
		
		this.render_background_color = function( renderToArea,renderCam)
		{
			//SDL_FillRect( renderToArea, &renderToArea.clip_rect, bgColor );
		}
		
		this.render_backgrounds = function(renderToArea, cam, renderCam)
		{
			if( typeof cam!="undefined")
			{
				for (var iTemp=0; iTemp<this.BACKGROUNDS.size(); iTemp++)
				{
					 if( this.BACKGROUNDS.at(iTemp)!=IS_NULL)
					 {
						this.BACKGROUNDS.at(iTemp).render_bg(renderToArea, cam,renderCam,this.sceneWidth, this.sceneHeight);
					 }
				}
			}
		}

		this.render_foregrounds = function(renderToArea, cam)
		{
			for (var iTemp=0; iTemp<this.FOREGROUNDS.size(); iTemp++)
			{
				 if(this.FOREGROUNDS.at(iTemp)!=IS_NULL)
				 {
					this.FOREGROUNDS.at(iTemp).render_bg(renderToArea, cam, this.sceneWidth, this.sceneHeight);
				 }
			}
		}

		this.render_scene = function(renderToArea,state_controller)
		{
			gpe.GPE_Current_Context = renderToArea;
			if( this.sceneResourcesAreProcessed && this.sceneIsProcessed)
			{
				var gTemp = 0;
				var hTemp=0;
				var iTemp=0;
				var jTemp=0;
				var kTemp = 0;
				var mainRenderToArea = renderToArea;
				var viewInUse = IS_NULL;
				
				var rendableViewCount = 0;
				
				for (gTemp = 0; gTemp < this.sceneCamera.length; gTemp++)
				{
					viewInUse = this.sceneCamera[ gTemp];
					if( viewInUse.isVisible)
					{
						rendableViewCount+1;
					}
				}
				GPE.currentCameraInView = 0;
				for (gTemp = 0; gTemp < this.sceneCamera.length; gTemp++)
				{
					GPE.currentCameraInView = gTemp;
					viewInUse = this.sceneCamera[ gTemp];
					if( viewInUse.isVisible)
					{
						gpe.GPE_Current_Context = renderToArea = viewInUse.renderContext;
						renderToArea.fillStyle = this.bgColor;
						gpe.set_render_alpha(255);
						renderToArea.fillRect(0, 0, gpe.SCREEN_WIDTH, gpe.SCREEN_HEIGHT);
						
						//in theory this will render at 60 FPS with alternating switching between even and odd cameras(30FPS per camera) unless its one camera which goes at 60 FPS
						//if(  (rendableViewCount<2) || (gTemp%2 == 0 && this.splitRenderMode%2==0) || (gTemp%2 != 0 && this.splitRenderMode%2!=0) )
						{
							gpe.CURRENT_VIEW = viewInUse;
							gpe.CURRENT_VIEW_BOX = viewInUse.cameraRect;
					
							this.render_backgrounds(renderToArea, viewInUse.cameraRect, viewInUse.cameraRect);
							renderToArea.drawImage(gpe.mapCanvas,viewInUse.cameraRect.get_x(),viewInUse.cameraRect.get_y(), viewInUse.cameraRect.get_width(), viewInUse.cameraRect.get_height(),
							0,0,viewInUse.cameraRect.get_width(), viewInUse.cameraRect.get_height() );
							
							var cLayer = IS_NULL;
							var cTile = IS_NULL;
							var iYTempFrom = this.snap_yto_grid(viewInUse.cameraRect.get_y() );
							var jXTempFrom = this.snap_xto_grid( viewInUse.cameraRect.get_x() );
							
							var iYTempTo = this.snap_yto_grid(viewInUse.cameraRect.get_y()+viewInUse.cameraRect.get_height() );
							var jXTempTo = this.snap_xto_grid(viewInUse.cameraRect.get_x()+viewInUse.cameraRect.get_width() );
							var tempTilePos = 0;
							//render objects on top of tiles and backgrounds
							if( gpe.preload_completed() && gpe.rsm.RESOURCES_ADDED)
							{
								var fObject = IS_NULL;
								var objectHolder = IS_NULL;
								var jLoop = 0;
								
								var jObjectHolderParse = 0;
								var kObjectParse = 0;
								var currentSpace = IS_NULL;
								var foundGameObject = IS_NULL;
								var cSpacesIn = state_controller.collisionSpacesInView;
								gpe.render_calls = 0;
								
								var iOutSideHeldObject = IS_NULL;
								var iSpace  = 0;
								//prerendering ( Shadows, etc,etc)
								for( iSpace = cSpacesIn.length-1; iSpace >= 0; iSpace--)
								{
									currentSpace = state_controller.COLLISION_AREA_SPACES.at(cSpacesIn[iSpace] );
									for( jObjectHolderParse = currentSpace.heldObjectsArray.length-1;jObjectHolderParse >= 0; jObjectHolderParse--)
									{
										objectHolder = currentSpace.heldObjectsArray[jObjectHolderParse];
										for(kObjectParse = objectHolder.length-1;kObjectParse >= 0; kObjectParse--)
										{
											foundGameObject = objectHolder[kObjectParse];
											if( typeof foundGameObject!="undefined")
											{
												if( foundGameObject!=IS_NULL)
												{
													if(  foundGameObject.isVisible && foundGameObject.prerenderedInFrame[gTemp]==false  && foundGameObject.gpeIsBeingDestroyed == false)
													{
														foundGameObject.render_under();
														foundGameObject.prerenderedInFrame[gTemp] = true;
														foundGameObject.ieTested = true;
													}
												}
											}
										}
									}
								}
								//
								for(iOutSideHeldObject = state_controller.COLLISION_AREA_OUTOFLEVEL.internalArray.length-1; iOutSideHeldObject >= 0; iOutSideHeldObject--)
								{
									foundGameObject = state_controller.COLLISION_AREA_OUTOFLEVEL.internalArray[kObjectParse];
									if( typeof foundGameObject!="undefined")
									{
										if( foundGameObject!=IS_NULL)
										{
											if(  foundGameObject.isVisible && foundGameObject.prerenderedInFrame[gTemp]==false  && foundGameObject.gpeIsBeingDestroyed == false)
											{
												foundGameObject.render_under();
												foundGameObject.prerenderedInFrame[gTemp] = true;
												foundGameObject.ieTested = true;
											}
										}
									}
								}
								//
								for( kTemp=GPR.GAME_OBJECTS_VIEW_INDEPENDENT.length-1; kTemp >=0; kTemp--)
								{
									foundGameObject = GPR.GAME_OBJECTS_VIEW_INDEPENDENT[kTemp];
									if( typeof foundGameObject!="undefined")
									{
										if( foundGameObject!=IS_NULL)
										{
											if( foundGameObject.isInit && foundGameObject.isVisible && foundGameObject.prerenderedInFrame[gTemp]==false)
											{
												foundGameObject.render_under();
												foundGameObject.prerenderedInFrame[gTemp] = true;
												foundGameObject.ieTested = true;
											}
										}
									}
								}
								
								gpe.set_render_alpha(255);
								//regular rendering
								for( iSpace = cSpacesIn.length-1; iSpace >= 0; iSpace--)
								{
									currentSpace = state_controller.COLLISION_AREA_SPACES.at(cSpacesIn[iSpace] );
									for( jObjectHolderParse = currentSpace.heldObjectsArray.length-1;jObjectHolderParse >= 0; jObjectHolderParse--)
									{
										objectHolder = currentSpace.heldObjectsArray[jObjectHolderParse];
										for(kObjectParse = objectHolder.length-1;kObjectParse >= 0; kObjectParse--)
										{
											foundGameObject = objectHolder[kObjectParse];
											if( typeof foundGameObject!="undefined")
											{
												if( foundGameObject!=IS_NULL)
												{
													if(  foundGameObject.isVisible && foundGameObject.renderedInFrame[gTemp]==false  && foundGameObject.gpeIsBeingDestroyed == false)
													{
														foundGameObject.render_self();
														foundGameObject.renderedInFrame[gTemp] = true;
														foundGameObject.ieTested = true;
													}
												}
											}
										}
									}
								}
								//
								for(iOutSideHeldObject = state_controller.COLLISION_AREA_OUTOFLEVEL.internalArray.length-1; iOutSideHeldObject >= 0; iOutSideHeldObject--)
								{
									foundGameObject = state_controller.COLLISION_AREA_OUTOFLEVEL.internalArray[kObjectParse];
									if( typeof foundGameObject!="undefined")
									{
										if( foundGameObject!=IS_NULL)
										{
											if(  foundGameObject.isVisible && foundGameObject.renderedInFrame[gTemp]==false  && foundGameObject.gpeIsBeingDestroyed == false)
											{
												foundGameObject.render_self();
												foundGameObject.renderedInFrame[gTemp] = true;
												foundGameObject.ieTested = true;
											}
										}
									}
								}
								//
								
								for( kTemp=GPR.GAME_OBJECTS_VIEW_INDEPENDENT.length-1; kTemp >=0; kTemp--)
								{
									foundGameObject = GPR.GAME_OBJECTS_VIEW_INDEPENDENT[kTemp];
									if( typeof foundGameObject!="undefined")
									{
										if( foundGameObject!=IS_NULL)
										{
											if( foundGameObject.isInit && foundGameObject.isVisible && foundGameObject.renderedInFrame[gTemp]==false)
											{
												foundGameObject.render_self();
												foundGameObject.renderedInFrame[gTemp] = true;
												foundGameObject.ieTested = true;
											}
										}
									}
								}
								
								gpe.set_render_alpha(255);
								//Render hud elements
								gpe.CURRENT_VIEW_BOX = viewInUse.renderRect;
								for( iSpace = cSpacesIn.length-1; iSpace >= 0; iSpace--)
								{
									currentSpace = state_controller.COLLISION_AREA_SPACES.at(cSpacesIn[iSpace] );
									for( jObjectHolderParse = currentSpace.heldObjectsArray.length-1;jObjectHolderParse >= 0; jObjectHolderParse--)
									{
										objectHolder = currentSpace.heldObjectsArray[jObjectHolderParse];
										for(kObjectParse = objectHolder.length-1;kObjectParse >= 0; kObjectParse--)
										{
											foundGameObject = objectHolder[kObjectParse];
											if( typeof foundGameObject!="undefined")
											{
												if( foundGameObject!=IS_NULL)
												{
													if(  foundGameObject.isVisible && foundGameObject.hudrenderedInFrame[gTemp]==false  && foundGameObject.gpeIsBeingDestroyed == false)
													{
														foundGameObject.render_hud();
														foundGameObject.hudrenderedInFrame[gTemp] = true;
														foundGameObject.ieTested = true;
													}
												}
											}
										}
									}
								}
								//
								for(iOutSideHeldObject = state_controller.COLLISION_AREA_OUTOFLEVEL.internalArray.length-1; iOutSideHeldObject >= 0; iOutSideHeldObject--)
								{
									foundGameObject = state_controller.COLLISION_AREA_OUTOFLEVEL.internalArray[kObjectParse];
									if( typeof foundGameObject!="undefined")
									{
										if( foundGameObject!=IS_NULL)
										{
											if(  foundGameObject.isVisible && foundGameObject.hudrenderedInFrame[gTemp]==false  && foundGameObject.gpeIsBeingDestroyed == false)
											{
												foundGameObject.render_hud();
												foundGameObject.hudrenderedInFrame[gTemp] = true;
												foundGameObject.ieTested = true;
											}
										}
									}
								}
								///
								
								for( kTemp=GPR.GAME_OBJECTS_VIEW_INDEPENDENT.length-1; kTemp >=0; kTemp--)
								{
									foundGameObject = GPR.GAME_OBJECTS_VIEW_INDEPENDENT[kTemp];
									if( typeof foundGameObject!="undefined")
									{
										if( foundGameObject!=IS_NULL)
										{
											if( foundGameObject.isInit && foundGameObject.isVisible && foundGameObject.hudrenderedInFrame[gTemp]==false)
											{
												foundGameObject.render_hud();
												foundGameObject.hudrenderedInFrame[gTemp] = true;
												foundGameObject.ieTested = true;
											}
										}
									}
								}
								
							}
							
							if( typeof mainRenderToArea!="undefined")
							{
								mainRenderToArea.drawImage(viewInUse.renderCanvas,
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
						viewInUse = this.sceneCamera[ gTemp];
						if( viewInUse.isVisible)
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
	
	}
})();