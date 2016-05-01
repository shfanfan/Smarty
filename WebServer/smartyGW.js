var SerialPortPkg = require("serialport");
var EventEmitter = require('events').EventEmitter;
//var events = require('events');
var util = require('util');
//var eventEmitter = new events.EventEmitter();

var serialPort = null;
const rxBuffer = new Buffer(8);
const txBuffer = new Buffer(8);

var smartiesDB = new Map();

const OPCODE_GENERIC_CMD = 					0xC0;
const OPCODE_SET_ANALOG_CMD	=				0xCA;
const OPCODE_SET_DIGITAL_CMD =				0xCD;
const OPCODE_DC_MOTOR_CMD =					0xE0;

const OPCODE_SENSORS_DATA_REPORT =			0xD0;
const NUM_ANALOGS_IN_DATA_REPORT =			5;

const OPCODE_BUTTON_PUSHED_REPORT =			0xD1;
const OPCODE_DEBUG_MESSAGE_REPORT =			0xDD;

const SYNC = [ 0xEF, 0xBE, 0xAD, 0xDE ];
var syncIndex = 0;
var synced = false;
var rxBufferIndex = 0;
var self = null;

function oxRxMessage() {
	var smartyID = rxBuffer[0];
	var opcode = rxBuffer[1];
	var debugString = "";
	if (opcode == OPCODE_DEBUG_MESSAGE_REPORT) {
		if (smartiesDB.has(smartyID))
			debugString = smartiesDB.get(smartyID).debugString;
		else
			smartiesDB.set(smartyID, { status: 1, debugString: "" });
		for (var i = 2; i < 8; i++) {
			if (rxBuffer[i] == 0x0D || rxBuffer[i] == 0x0A || rxBuffer[i] == 0) {
				//console.log("Debug string from smarty #" + smartyID + " : " + debugString);
				if (self != null)
					self.emit('debugmsg', smartyID, debugString);
				debugString = "";
				break;
			}
			else
				debugString += String.fromCharCode(rxBuffer[i]);
		}
		smartiesDB.get(smartyID).debugString = debugString;
	}
	else {
		if (self != null)
			self.emit('rxmsg', rxBuffer);
	}
}

function onRxData(data) {
	for (var i = 0; i < data.length; i++) {
		//console.log(data[i].toString(16));
		if (synced) {
			rxBuffer[rxBufferIndex] = data[i];
			rxBufferIndex++;
			if (rxBufferIndex == 8) {
				oxRxMessage();
				rxBufferIndex = 0;
				synced = false;
			}
		}
		else {
			if (data[i] == SYNC[syncIndex]) {
				syncIndex++;
				if (syncIndex == 4) {
					synced = true;
					syncIndex = 0;
				}
			}
			else
				syncIndex = 0;
		}
	}
}

var smartyGW = function () {
	self = this;
	
	SerialPortPkg.list(function (err, ports) {
		ports.forEach(function(port) {
			if (port.manufacturer.indexOf("Arduino") >= 0) {		// this is an arduino port
				console.log("Opening port: " + port.comName + " identified as " + port.manufacturer);
				serialPort = new SerialPortPkg.SerialPort(port.comName, { baudrate: 115200 }, false);
				serialPort.open(function (error) {
					if ( error ) {
						console.log('Failed to open: ' + error);
					} else {
						console.log('Successfully open.');
						serialPort.on('data', onRxData);
					}
				});
			}
		});
	});
}

util.inherits(smartyGW, EventEmitter);

smartyGW.prototype.OPCODE_GENERIC_CMD = OPCODE_GENERIC_CMD;

smartyGW.prototype.isOpen = function() {
	if (serialPort == null)
		return false;
	else
		return serialPort.isOpen();
}

smartyGW.prototype.sendBuffer  = function(data) {
	if (serialPort != null && serialPort.isOpen()) {
		//console.log("Sending: " + data);
		serialPort.write(data);
	}
}

smartyGW.prototype.sendCommand  = function(smartyID, opcode, param1, param2, param3, param4, param5, param6) {
	txBuffer[0] = smartyID;
	txBuffer[1] = opcode;
	txBuffer[2] = param1;
	txBuffer[3] = param2;
	txBuffer[4] = param3;
	txBuffer[5] = param4;
	txBuffer[6] = param5;
	txBuffer[7] = param6;
	//console.log("Wishing to send: " + txBuffer);
	this.sendBuffer(txBuffer);
}

module.exports = smartyGW;

