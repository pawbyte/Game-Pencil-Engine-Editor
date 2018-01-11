var nwft = nwft || {};

(function () {
	"use strict";

	// Initialize the processes shared by the tutorials.
	// Specify TV resolution and callback functions.
	nwft.initialize = function (tvWidth, tvHeight, callback){
		nwft.isWiiU = window.nwf && nwf.system && nwf.system.isWiiU();

		window.requestAnimationFrame = window.requestAnimationFrame ||
			window.webkitRequestAnimationFrame;

		initializeCanvas(tvWidth, tvHeight, callback);
	};

	// Initialize the canvas.
	// Specify TV resolution and callback functions.
	function initializeCanvas(tvWidth, tvHeight, callback) {
		nwft.GP_WIDTH = 854;
		nwft.GP_HEIGHT = 480;
		nwft.TV_WIDTH = tvWidth;
		nwft.TV_HEIGHT = tvHeight;
		nwft.GP_OFFSET_X = (nwft.TV_WIDTH - nwft.GP_WIDTH) * 0.5;
		nwft.GP_OFFSET_Y = (nwft.TV_HEIGHT - nwft.GP_HEIGHT) * 0.5;

		nwft.gpCanvas = window.document.getElementById("gpeCanvas");
		nwft.gpCanvas.width = nwft.GP_WIDTH;
		nwft.gpCanvas.height = nwft.GP_HEIGHT;

		nwft.gpContext = nwft.gpCanvas.getContext("2d",{
					antialias: '2x',
					stencil: false,
					graphicsMemory: true,
				});
		if (nwft.isWiiU) {
			var tv = nwf.display.DisplayManager.getInstance().getTVDisplay();

			tv.addEventListener("load", function () {
				nwft.tvCanvas = tv.window.document.getElementById("gpeCanvas");
				nwft.tvCanvas.width = tvWidth;
				nwft.tvCanvas.height = tvHeight;

				nwft.tvContext = nwft.tvCanvas.getContext("2d",{
					antialias: '2x',
					stencil: false,
					graphicsMemory: true,
				});
				nwft.tvCanvas.className = "show";

				callback();
			});

			//tv.load("TV.html");
		} else {
			nwft.tvCanvas = window.document.getElementById("tvCanvas");
			nwft.tvCanvas.width = nwft.TV_WIDTH;
			nwft.tvCanvas.height = nwft.TV_HEIGHT;
			nwft.tvCanvas.className = "show";

			nwft.tvContext = nwft.tvCanvas.getContext("2d");

			callback();
		}
	}

})();
 