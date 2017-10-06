console.log("Initiating GPE Super Array extension.");
Array.prototype.at = function(atPos)
{
	if( atPos < this.length && atPos >=0 )
	{
	    return this[atPos];
	}
	return -1;
}

Array.prototype.clear = function()
{
	this.length = 0;
}

Array.prototype.back = function()
{
	if( this.length > 0 )
	{
		return this[this.length-1];
	}
	return -1;
}

Array.prototype.makeLowercase = function() 
{
    for (var i = 0; i < this.length; i++)
	{
        if (typeof this[i] === 'string')
        {
             this[i] = this[i].toLowerCase();
         }
     }
}


Array.prototype.makeUppercase = function() 
{
    for (var i = 0; i < this.length; i++)
	{
        if (typeof this[i] === 'string')
        {
             this[i] = this[i].toUpperCase();
         }
     }
}

Array.prototype.floorValues = function() 
{
    for (var i = 0; i < this.length; i++)
	{
        if (typeof this[i] === 'number')
        {
             this[i] = Math.floor(this[i]);
         }
     }
}

Array.prototype.roundValues = function() 
{
    for (var i = 0; i < this.length; i++)
	{
        if (typeof this[i] === 'number')
        {
             this[i] = Math.round(this[i]);
         }
     }
}

Array.prototype.roundByDecimals = function(decimalPlaces) 
{
    var roundByTens = 0;
    decimalPlaces = Math.floor(decimalPlaces);
	if (decimalPlaces >=0 )
	{
	    roundByTens = Math.pow(10,decimalPlaces);
		for (var i = 0; i < this.length; i++)
		{
			if (typeof this[i] === 'number')
			{
				 this[i] = Math.round(this[i]/roundByTens)* roundByTens;
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

Array.prototype.ceilValues = function() 
{
    for (var i = 0; i < this.length; i++)
	{
        if (typeof this[i] === 'number')
        {
             // this is a string
             this[i] = Math.ceil(this[i]);
         }
     }
}

Array.prototype.size = function() 
{
     return this.length;
}

//emulates the push_back push_front 
Array.prototype.push_back = function(inValue) 
{
     this.push(inValue);
}


Array.prototype.pop_back = function() 
{
     return this.pop();
}

console.log("GPE Super Array loaded");