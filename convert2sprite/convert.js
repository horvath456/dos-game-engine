const pixels = require('image-pixels');
const fs = require('fs');
const { promisify } = require('util');
const path = require('path');

const writeFile = promisify(fs.writeFile);

(async () => {
    try {
        let outputs = [];
        let palette = [];

        const args = process.argv.slice(2);

        for (const filename of args) {
            const { data, width, height } = await pixels(filename);

            let output = new Uint8Array(4 + height * width);

            output[0] = ((width >> 8) & 0xFF);
            output[1] = width & 0xFF;
            output[2] = ((height >> 8) & 0xFF);
            output[3] = height & 0xFF;


            for (let i = 0; i < height; i++) {
                for (let j = 0; j < width; j++) {
                    const r = (data[(i * width + j) * 4]) >> 2;
                    const g = (data[(i * width + j) * 4 + 1]) >> 2;
                    const b = (data[(i * width + j) * 4 + 2]) >> 2;

                    const rgb24 = r << 16 | g << 8 | b;

                    if (palette.indexOf(rgb24) === -1) {
                        palette.push(rgb24);
                    }

                    output[(i * width) + j + 4] = palette.indexOf(rgb24);
                }
            }

            outputs.push({ name: ((path.parse(filename).base).split('.')[0].substr(0, 8)).toUpperCase() + '.SPR', data: output });
        }

        if (palette.length > 256) {
            throw new Error('More than 256 colors');
        }

        for (const entry of outputs) {
            await writeFile(entry.name, entry.data);
        }

        const rawPalette = new Uint8Array(256 * 3 + 1).fill(0xFF);

        for (let i = 0; i < palette.length; i++) {
            const r = (palette[i] & 0xFF0000) >> 16;
            const g = (palette[i] & 0x00FF00) >> 8;
            const b = (palette[i] & 0x0000FF);

            rawPalette[i * 3] = r;
            rawPalette[i * 3 + 1] = g;
            rawPalette[i * 3 + 2] = b;
        }

        await writeFile('PALETTE.DAT', rawPalette);

    } catch (e) {
        console.error(e);
    }
})();