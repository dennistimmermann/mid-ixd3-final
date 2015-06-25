//52.286947, 8.026087

var mongo = require('mongodb').MongoClient
var express = require('express')
var bodyParser = require('body-parser')
var geocoder = require('geocoder')
var _ = require('lodash')

var app = express()

app.use(bodyParser.json())

var url = 'mongodb://192.168.99.100:32769/jupiter'

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
