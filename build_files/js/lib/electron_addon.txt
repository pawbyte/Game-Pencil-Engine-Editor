//Code copied from http://ourcodeworld.com/articles/read/106/how-to-choose-read-save-delete-or-create-a-file-with-electron-framework

var remote = require('remote'); 
var dialog = remote.require('dialog');
var fs = require('fs');

function load_file(filepath)
{
	fs.readFile(filepath, 'utf-8', function (err, data)
	{
		if(err)
		{
			console.log("An error ocurred reading the file :" + err.message);
			return;
		}	
		return data;
	});
}

function delete_file(filepath)
{
	fs.exists(filepath, function(exists)
	{
		if(exists)
		{
			// File exists deletings
			fs.unlink(filepath,function(err)
			{
				if(err)
				{
					alert("An error ocurred updating the file"+ err.message);
					console.log(err);
					return;
				}
			});
		}
		else
		{
			console.log("This file doesn't exist, cannot delete");
		}
	});
}

function save_file(filepath,content)
{
	fs.writeFile(filepath, content, function (err)
	{
		if(err)
		{
			alert("An error ocurred updating the file"+ err.message);
			console.log(err);
			return;
		}
		console.log("The file has been succesfully saved");
	});
}	