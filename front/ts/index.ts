import { KeyCatcher } from './KeyCatcher.js'
import {
	Alignment,
	ArtificialConstraint,
	BasicTextInput,
	Button,
	Column,
	Container,
	DOMPointerDriver,
	DOMRoot,
	FlexAlignment,
	Icon,
	Label,
	LabelledCheckbox,
	Margin,
	Row,
	ScrollableViewportWidget,
	ScrollbarMode,
	Slider,
	TextAlignMode,
	TextButton,
	Theme,
	ThemeScope,
	ViewportWidget,
} from './lib/canvas-ui.js'

const ws = new WebSocket(`ws://${location.host}`)
const FPS = 128
let bool = false
const label = new Label('yoyoyo')
const inputLabel = new Label('')

const initialMinWidth = 200
const initialMinHeight = 200

const viewport = new ScrollableViewportWidget(
	new ThemeScope(
		new Margin(
			new Column()
				.add(label)
				.add(inputLabel)
				.add(
					new BasicTextInput(
						text => (inputLabel.source = `Text input: ${text}`),
					),
				)
				.add(
					new Row({
						multiContainerAlignment: {
							main: FlexAlignment.Center,
							cross: Alignment.Stretch,
						},
					}).add([
						new TextButton(
							'Button 1',
							() => (label.source = 'Button 1 clicked!'),
						),
						new TextButton(
							'Button 2',
							() => (label.source = 'Button 2 clicked!'),
						),
					]),
				),
		),
		new Theme({ canvasFill: 'red' }),
	),
	initialMinWidth,
	initialMinHeight,
	false,
	false,
	ScrollbarMode.Overlay,
	true,
)

const widthLabel = new Label(`Minimum width: ${initialMinWidth}`)
const heightLabel = new Label(`Minimum height: ${initialMinHeight}`)

const controls = new Container(
	new ArtificialConstraint(
		new Column()
			.add(
				new LabelledCheckbox(
					'Width tied',
					tied => (viewport.widthTied = tied),
					false,
				),
			)
			.add(
				new LabelledCheckbox(
					'Height tied',
					tied => (viewport.heightTied = tied),
					false,
				),
			)
			.add(
				new LabelledCheckbox(
					'Overlay scrollbars',
					overlay => {
						viewport.scrollbarMode = overlay
							? ScrollbarMode.Overlay
							: ScrollbarMode.Layout
					},
					true,
				),
			)
			.add(widthLabel)
			.add(
				new Slider(
					min => {
						viewport.minWidth = min
						widthLabel.source = `Minimum width: ${min}`
					},
					0,
					500,
					1,
					initialMinWidth,
				),
			)
			.add(heightLabel)
			.add(
				new Slider(
					min => {
						viewport.minHeight = min
						heightLabel.source = `Minimum height: ${min}`
					},
					0,
					500,
					1,
					initialMinHeight,
				),
			),
		[0, 300, 0, Infinity],
	),
)
controls.containerAlignment = {
	horizontal: Alignment.Start,
	vertical: Alignment.End,
}
const view = new ViewportWidget(
	new Column().add([
		controls,
		new Row({ multiContainerAlignment: { main: 0.5, cross: 0.5 } }).add([
			new TextButton(
				() => (bool ? 'Play' : 'Pause'),
				() => {
					if (bool) {
						console.log('play')
						bool = false
					} else {
						console.log('pause')
						bool = true
					}
				},
			),
			new ThemeScope(viewport, new Theme({ canvasFill: 'purple' })),
		]),
		new Button(
			new Label(
				'BANANfdsajklfjsdkafjdskljffdsafdsafdsafsdvcxzvxczfgadgfdagdsa',
				{
					bodyTextAlign: TextAlignMode.Center,
				},
			),
			() => console.log('bannana'),
		),
		new Label('ufisdoaunjkvds'),
	]),
	0,
	0,
	false,
	false,
	true,
	{ canvasFill: '#00000000' },
)

const canvas = document.getElementById('game') as HTMLCanvasElement
const ctx = canvas.getContext('2d') as CanvasRenderingContext2D
const resize = () => {
	canvas.width = view.minWidth = window.innerWidth
	canvas.height = view.minHeight = window.innerHeight
}
resize()
window.addEventListener('resize', resize)
const root = new DOMRoot(view)
const pointerDriver = new DOMPointerDriver()
pointerDriver.bindDOMElem(root, root.domElem)
root.registerDriver(pointerDriver)

ws.onopen = ev => {
	window.requestAnimationFrame(update)
	console.log('connected')
}
ws.onclose = ev => {
	console.log('disconnected')
}
ws.onmessage = async ev => {
	const ab = new Float32Array(await ev.data.arrayBuffer())
	// console.log(ab)
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
	root.update()
	window.requestAnimationFrame(update)
}

document.body.appendChild(root.domElem)
