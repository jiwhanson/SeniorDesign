var ws = new WebSocket('ws://localhost:3333');

ws.onmessage = function (event){
    console.log(event.data);
};