const fs = require('fs');
const { promisify } = require('util');
const reader = require("readline-sync");

const writeFile = promisify(fs.writeFile);

console.log("USAGE: node create.js OUT_FILE");

(async () => {
    try {
        const num_notes = parseInt(reader.question("Num_notes: "));

        console.log();

        const rawOutput = new Uint8Array(num_notes * 7 + 2 + 1);

        rawOutput[0] = ((num_notes >> 8) & 0xFF);
        rawOutput[1] = num_notes & 0xFF;

        for (let i = 0; i < num_notes; i++) {
            const channel = parseInt(reader.question("Channel #: "));
            const freq = parseInt(reader.question("Freq: "));
            const octave = parseInt(reader.question("Octave: "));
            const waveform = parseInt(reader.question("Waveform: "));
            const time = parseInt(reader.question("Time (in ms): "));

            rawOutput[i * 7 + 2] = channel;
            rawOutput[i * 7 + 3] = ((freq >> 8) & 0xFF);
            rawOutput[i * 7 + 4] = freq & 0xFF;
            rawOutput[i * 7 + 5] = octave;
            rawOutput[i * 7 + 6] = waveform;
            rawOutput[i * 7 + 7] = ((time >> 8) & 0xFF);
            rawOutput[i * 7 + 8] = time & 0xFF;

            console.log();
        }

        await writeFile(process.argv[2], rawOutput);
    } catch (e) {
        console.error(e);
    }
})();