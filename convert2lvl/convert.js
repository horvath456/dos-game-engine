const fs = require('fs');
const { promisify } = require('util');
const path = require('path');

const writeFile = promisify(fs.writeFile);
const readFile = promisify(fs.readFile)

console.log("USAGE: node convert.js INPUT_FILE.CSV OUTPUT_FILE.LVL");

(async () => {
    try {
        const data_string = (await readFile(process.argv[2])).toString();
        const lines = (data_string.split('\n')).filter(v => v.length > 0);
        const data = lines.map(el => el.split(','));
        const width = data[0].length;
        const height = data.length;
        const size = width * height;

        let output = new Uint8Array(4 + size);

        output[0] = ((width >> 8) & 0xFF);
        output[1] = width & 0xFF;
        output[2] = ((height >> 8) & 0xFF);
        output[3] = height & 0xFF;

        console.log("WIDTH: " + width + " HEIGHT: " + height);

        let cnt = 4;
        data.forEach(line => {
            let str = '';
            line.forEach(value => {
                const v = parseInt(value) + 1;
                output[cnt] = parseInt(value) + 1;
                str += v + ' ';
                cnt++;
            });
            console.log(str);
        });

        await writeFile(process.argv[3], output);
    } catch (e) {
        console.error(e);
    }
})();
