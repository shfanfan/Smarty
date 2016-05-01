var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);

var smartyGWPkg = require('./smartyGW');
var smartyGW = new smartyGWPkg();

var button_state = 0;

function sendDigitalOut(message) {
	//smartyGW.sendCommand(0, 0xCD, 13, led, 0, 0, 0, 0);
	smartyGW.sendCommand(message.SmartyID, 0xCD, message.pin1Id, message.pin1Value, message.pin2Id, message.pin2Value, message.pin3Id, message.pin3Value);
	button_state = message.pin1Value;
	console.log(message);
	
}

app.use('/node_modules', express.static(__dirname + '/node_modules'));
app.use('/client', express.static(__dirname + '/client'));

var PORT = 80;

app.get('/', function(req, res){
	res.sendFile(__dirname + '/client/prototype.html');
});


http.listen(PORT, function () {
	console.log('Helper app listening on port ' + PORT);
});



io.on('connection', function(socket){
	console.log('a user connected');
	socket.on('disconnect', function(){
		console.log('user disconnected');
	});
	socket.on('sendDigitalOut', function(message){
		console.log('user made an input!' + message.toString());
		sendDigitalOut(JSON.parse(message));
	});
});


//smartyGW.on('rxmsg', onRxMsg);
//
//app.listen(PORT, function () {
//	console.log('Helper app listening on port ' + PORT);
//});



app.get('/poll', function (req, res) {
	var valueString = (button_state == 0 ? "false" : "true");
	res.send('digitalIn ' + valueString + '\n');
	
	io.emit("poll");
	
});

//app.get('/reset_all', function (req, res) {
//	console.log('RESET ALL!');
//	button_state = 0;
//	res.send('');
//});
//
////app.use(function (req, res) {
////	var cmd = req.url.replace(new RegExp('%2F', "g"), '/');
////	console.log('command: ' + cmd);
////	var args = cmd.split('/');
////	//console.log(args);
////	if (args[1] == "digitalOut") {
////		if (args[3] == "off")	args[3] = "0";
////		if (args[3] == "on")	args[3] = "1";
////		smartyGW.sendCommand(0, 0xCD, parseInt(args[2]), parseInt(args[3]), 0, 0, 0, 0);	
////	}
////	res.send('');
////});
//
//function onRxMsg(rxBuffer) {
//	console.log("rxmsg : ");
//	for (var i = 0; i < rxBuffer.length; i++)
//		console.log(rxBuffer[i].toString(16));
//	if (rxBuffer[0] == 0 && rxBuffer[1] == 0xD1 && rxBuffer[2] == 1)
//		button_state = rxBuffer[3];
//}
//