//52.286947, 8.026087

var mongo = require('mongodb').MongoClient
var express = require('express')
var bodyParser = require('body-parser')
var geocoder = require('geocoder')
var _ = require('lodash')

var app = express()

app.use(bodyParser.json())

var url = 'mongodb://192.168.99.100:32769/jupiter'

var calculateDistance = function(lat1, lng1, lat2, lng2){
	var R = 6371; //Radius of earth in km
	var dLat = toRad(lat2-lat1);//toRad(location2.lat()-location1.lat());
	var dLon = toRad(lng2-lng1);
	var dLat1 = toRad(lat1);
	var dLat2 = toRad(lat2);
	var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
	        Math.cos(dLat1) * Math.cos(dLat1) *
	        Math.sin(dLon/2) * Math.sin(dLon/2);
	var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
	distance = R * c; //Distance in km

}

var toRad = function(Value) {
    /** Converts numeric degrees to radians */
    return Value * Math.PI / 180;
}

app.get('/', function(req, res) {
	res.send('Hello World')
})

app.post('/data', function(req, res) {
	console.log('*', 'access to /data')
	if(!req.is('application/json')) {
		return res.status(400).end()
	}

	geocoder.reverseGeocode(req.body.lat, req.body.lon, function(err, data){

		var town = _.where(data.results[0].address_components, {types: ["locality"]})[0].long_name
		var country = _.where(data.results[0].address_components, {types: ["country"]})[0].long_name

		if(!town || !country) {
			return res.status(500).send('reverseGeocoding failed').end
		}

		req.body.town = town
		req.body.country = country

		mongo.connect(url, function(err, db) {
			if(err) return res.status(500).json(err).end()
			console.log('*', 'mongodb connected')

			var col = db.collection('positions')
			req.body.timestamp = Date.now()/1000
			col.insert(req.body, function(err, result) {
				if(err) return res.status(500).json(err).end()
				console.log(result)
				return res.status(200).json(result).end()
			})
		})
	}, { language: 'en' })
})

var server = app.listen(3000, function() {
	console.log('*', 'express server at', server.address().address, server.address().port)
})
