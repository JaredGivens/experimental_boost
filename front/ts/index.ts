import { KeyCatcher } from './KeyCatcher.js'

const ws = new WebSocket(`ws://${location.host}`)
const FPS = 128

ws.onopen = ev => {
	window.requestAnimationFrame(update)
	console.log('connected')
}
ws.onclose = ev => {
	console.log('disconnected')
}
ws.onmessage = async ev => {
	const ab = new Float32Array(await ev.data.arrayBuffer())
	console.log(ab)
}

ws.onerror = ev => {
	console.log('error', ev)
}

const message = new Float32Array(4)
message[1] = 234
message[2] = 3249.93112
message[3] = 2.30929493
new KeyCatcher('qwerasd', m => {
	message[0] = m
})

const update = () => {
	ws.send(message)
	window.requestAnimationFrame(update)
}
