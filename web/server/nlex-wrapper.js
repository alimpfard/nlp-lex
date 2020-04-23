const { nlexBinary } = require('./config.js');
const fs = require('fs');
const ps = require('child_process');

function read_error(error) {
    let errnum = Number(error.substr(2, 4));
    let errstr = error.substr(6);
    let erriidx = error.indexOf('-');
    let erristr = error.substr(8, erriidx - 9);
    errstr = error.substr(erriidx + 1);
    let errposre = errstr.match(/ \(line (\d+) offset (\d+) length (\d+)\) /);
    let errposline = errposre[1];
    let errposoff = errposre[2];
    let errposlen = errposre[3];
    errstr = errstr.substr(errposre.index + errposre[0].length + 2);
    return {
        code: errnum,
        kind: erristr,
        position: {
            line: Number(errposline),
            offset: Number(errposoff),
            length: Number(errposlen)
        },
        message: errstr
    };
}

module.exports = {
    run: async function({ job }) {
        const args = job.arguments;
        const bin_arguments = [];
        if (args.dry_run) {
            bin_arguments.push('-r');
            bin_arguments.push('--gout');
            bin_arguments.push(args.output_name + '.dot');
        } else {
            if (args.image_format) {
                bin_arguments.push('--object-format');
                bin_arguments.push(args.image_format);
            }
            if (args.target) {
                bin_arguments.push('--target');
                bin_arguments.push(args.target);
            }
        }
        bin_arguments.push(args.output_name + '.in');

        await new Promise((resolve) => fs.writeFile(args.output_name + '.in', job.source + '\n\n', () => { resolve(); }));

        let child = ps.spawn(nlexBinary, bin_arguments);
        console.log(bin_arguments);
        let diagnostics = ""

        function handle_data(data) {
            diagnostics += data.toString();
        }

        child.stderr.on('data', handle_data);
        child.stdout.on('data', handle_data);

        return new Promise((resolve, reject) => {
            child.on('exit', (code, signal) => {
                console.log('code', code, signal);
                let _diagnostics = [];
                for (let line of diagnostics.split('\n')) {
                    console.log(line);
                    if (line.startsWith('[E0')) {
                        _diagnostics.push(read_error(line));
                    }
                }
                resolve({ diagnostics: _diagnostics });
            });
            child.on('error', err => {
                console.log('error', err);
                reject(err);
            });
        });
    }
};