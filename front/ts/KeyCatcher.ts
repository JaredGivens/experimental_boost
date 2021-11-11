export class KeyCatcher {
	private cans: Record<string, number>
	private sum: number
	private callback: (n: number) => void

	constructor(keys: string, callback: (mask: number) => void) {
		this.sum = 0
		this.cans = {}
		this.callback = callback
		for (let i = 0; i < keys.length; ++i) {
			this.cans[keys[i]] = 1 << i
		}
		document.addEventListener('keydown', event => {
			if (
				this.cans[event.key] &&
				0 === (this.sum & this.cans[event.key])
			) {
				this.sum += this.cans[event.key]
				callback(this.sum)
			}
		})

		document.addEventListener('keyup', event => {
			if (this.cans[event.key] && this.sum & this.cans[event.key]) {
				this.sum -= this.cans[event.key]
				callback(this.sum)
			}
		})
	}
}
