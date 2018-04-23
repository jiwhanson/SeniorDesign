const testFolder = './TestFolder/';
var fs = require('fs');
var express = require('express' );
const SocketServer = require('ws').Server;
const path = require ('path');
const server = express()
//.use((req, res) => res.sendFile(INDEX) )
  .listen(3333, () => console.log(`Listening on ${ 3333 }`));   

const wss = new SocketServer({ server });

wss.on('connection', (ws) =>{
    console.log('Client Connected');
// function to encode file data to base64 encoded string

//var base64str = base64_encode('penguin.jpg');
//console.log(base64str)
    var Images = [];
    fs.readdir(testFolder, (err, files) => {
        files.forEach(file => {
            Images.push (base64_encode(`./TestFolder/${file}`,file));
        });
    });

    ws.on('message', function incoming (data) {
//        if(err){
  //          console.log('error');
    //    }
      //  else{
        ws.send(Images);
        //}
    });
    ws.on('close', () => console.log('Client Disconnected'));
});




function base64_encode(file) {
    // read binary data
    var bitmap = fs.readFileSync(file);
    // convert binary data to base64 encoded string
    return new Buffer(bitmap).toString('base64');
}











