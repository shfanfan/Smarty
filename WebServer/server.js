var PORT = 1234;
var express = require('express');
var path = require('path');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);

//node static folders definition
//app.use(express.static(path.join(__dirname,'node_modules')));
app.use(express.static(__dirname));

var smartyDatbase = {
	brick:{
		port:"com3"
	},
	smartiesMap:{
		"1":{
			serial: "1/1",
			type: "analogIO",
			serial: "SDFGsg4h56SE",
			latestDataBytesValue:[10,20,30,0,0,0,0,0]
		} 
	}
};

//node socketio activity 
io.on('connection', function (socket) {
	console.log('socket connected.');
	socket.on('pleaseGiveSmartiesState', function (data) {
		console.log('data arrived');
		socket.emit('smartiesState', smartyDatbase);
	});  
});

//client page root
app.get('/', function (req, res) {
  res.sendFile (path.join(__dirname, 'index.html')) 
});

//scratch poll response
app.get('/poll', function (req, res) {
	var responseString="_problem no smatrties are connected."
	if (smartyDatbase.smartiesMap!==undefined && smartyDatbase.smartiesMap.length>0)
	{
		for (var smarty in smartyDatbase.smartiesMap){
			responseString += "GetAnalogValue/" + serial + "\n";
		}	
	}
	res.send (responseString);

	//original: res.send('GetAnalogValue/1/1 ' + counter++ + '\n');
});

app.get('/*', function (req, res) {
  //TODO: parse the request and return data
  
});

//TODO: read the file that contains all the scretch plugins and extract their port number
app.listen(PORT, function () {
  console.log('smarty server is listening on port ' + PORT);
});






//----------------------------------------------------------
//	serial communication with the smartBrick
//----------------------------------------------------------

var smartyGWPkg = require('./smartyGW');
var smartyGW = new smartyGWPkg();
var led = 0;

function blink() {
	smartyGW.sendCommand(1, 0xCA, 5, led, 0, 0, 0, 0);
	led = 255-led;
}

function onDebug(smartyID, debugString) {
	console.log("Debug string from smarty #" + smartyID + " : " + debugString);
}

function onRxMsg(rxBuffer) {
	console.log("rxmsg : ");
	for (var i = 0; i < rxBuffer.length; i++)
		console.log(rxBuffer[i].toString(16));
}

setInterval(blink, 1000);
smartyGW.on('debugmsg', onDebug);
//smartyGW.on('rxmsg', onRxMsg);
