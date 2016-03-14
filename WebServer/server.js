var PORT = 1234;
var express = require('express');
var path = require('path');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);

//node static folders definition
app.use(express.static(path.join(__dirname,'client')));
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
  res.sendFile (path.join(__dirname, 'client/index.html'))
});

app.get('/crossdomain.xml', function (req, res) {
	console.log('flash asked for a crossdomain authentication');
	var str = "<cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"" + PORT + "\"/></cross-domain-policy>";
	res.send (str);
})


//scratch poll response
app.get('/poll', function (req, res) {

	var responseString="";
	if (smartyDatbase.smartiesMap!==undefined && Object.keys(smartyDatbase.smartiesMap).length>0)
	{
		for (var smarty in smartyDatbase.smartiesMap){
			//responseString += "GetAnalogValue/" + smartyDatbase.smartiesMap[smarty].serial + "\n";
			responseString += "GetAnalogValue/1/1 55" + "\n";
			responseString += "GetAnalogValue/2/1 65" + "\n";
		}
	}else{
		responseString="_problem no smatrties are connected."
	}
	res.send (responseString);

	//original: res.send('GetAnalogValue/1/1 ' + counter++ + '\n');
});

app.get('/*', function (req, res) {
	console.log(req.url);
	res.send();
  //TODO: parse the request and return data
  
});

//TODO: read the file that contains all the scretch plugins and extract their port number
app.listen(PORT, function () {
  console.log('smarty server is listening on port ' + PORT);
});






//      ----------------------------------------------------------
//      	serial communication with the smartBrick
//      ----------------------------------------------------------

//      r smartyGWPkg = require('./smartyGW');
//      r smartyGW = new smartyGWPkg();
//      r led = 0;

//      nction blink() {
//      	smartyGW.sendCommand(1, 0xCA, 5, led, 0, 0, 0, 0);
//      	led = 255-led;
//

//      nction onDebug(smartyID, debugString) {
//      	console.log("Debug string from smarty #" + smartyID + " : " + debugString);
//

//      nction onRxMsg(rxBuffer) {
//      	console.log("rxmsg : ");
//      	for (var i = 0; i < rxBuffer.length; i++)
//      		console.log(rxBuffer[i].toString(16));
//

//      tInterval(blink, 1000);
//      artyGW.on('debugmsg', onDebug);
//      smartyGW.on('rxmsg', onRxMsg);
