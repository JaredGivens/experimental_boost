const ws = new WebSocket(`ws://${location.host}`);

ws.onopen = (ev) => {
  console.log("connected");
};

ws.onclose = (ev) => {
  console.log("disconnected");
};

ws.onmessage = (ev) => {
  console.log(ev.data);
};

ws.onerror = (ev) => {
  console.log("error", ev);
};
