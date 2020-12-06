const express = require('express');
const mqtt = require('mqtt');


var options = { // https://stackoverflow.com/a/36460844/4378475
    port: 23821,
    host: 'mqtt://m13.cloudmqtt.com',
    clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
    username: 'pyirguyn',
    password: 'hf6ihTrfIuoD',
    keepalive: 60,
    reconnectPeriod: 1000,
    protocolId: 'MQIsdp',
    protocolVersion: 3,
    clean: true,
    encoding: 'utf8'
};

var client = mqtt.connect('mqtt://m13.cloudmqtt.com', options);

client.on('connect', function () {
    // client.subscribe('Topic07');
    console.log('client has subscribed successfully');
});

client.on('error', function (err) {
    // client.subscribe('Topic07');
    console.log('Connection error: ', err);
});


client.on('message', function (topic, message) {
    console.log("message received on client", message.toString()); //if toString is not given, the message comes as buffer
});



app = express()
// app.use(cors())
// app.use(bodyParser.json());

const PORT = process.env.PORT || 5000

app.post("/bulb", (req, res, next) => {

    if (!req.body.topic || typeof !req.body.topic != "string"
        || !req.body.message || typeof req.body.message !== "string") {
        res.status(400).send("please send json body with string value of topic and message. e.g: {topic: 'Topic abc', message: 'some string message'} ")
        return;
    }

    // Publishing a message to all other MQTT clients 
    client.publish(req.body.topic,
        Buffer.from(
            `{
                    "data": ${req.body.data},
                    "time": ${new Date().getHours()}:${new Date().getMinutes()}:${new Date().getSeconds()}:${new Date().getMilliseconds()}"
                }`
        ), () => {
            console.log("a topic is published from client ");
            res.send("published message on topic" + req.body.topic);
        });
})

app.listen(PORT, function () {
    console.log("server is running on", PORT);
})