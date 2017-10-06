var lastAteInt = -1;
var lastAteString = -1;

function GPE_String( startString)
{
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
	
	this.eatFirstInt = function( separator )
	{
		var seppos = this.str.indexOf(separator);
		if (seppos == IS_NULL)
		{
			return 0; // not found
		}
		var num = parseInt( this.str.substr(0, seppos) );
		lastAteString = this.str.substr( seppos+separator.length );
		this.str = lastAteString;
		this.length = this.str.length;
		return num;
	}

	this.eatFirstString = function( separator)
	{
		if( separator.length >= 1 )
		{
			var seppos = this.str.indexOf(separator);
			if (seppos == IS_NULL) return ""; // not found
			var outs = this.str.substr(0, seppos);
			lastAteString = this.str.substr( seppos+separator.length );
			this.str = lastAteString;
			this.length = this.str.length;
			return outs;
		}
		return "";
	}
}