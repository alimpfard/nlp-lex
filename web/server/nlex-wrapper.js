const {
  nlexBinary
} = require('./config.js');
const fs = require('fs');
const ps = require('child_process');

function read_error (error) {
  const errnum = Number(error.substr(2, 4));
  let errstr = error.substr(6);
  const erriidx = error.indexOf('-');
  const erristr = error.substr(8, erriidx - 9);
  errstr = error.substr(erriidx + 1);
  const errposre = errstr.match(/ \(line (\d+) offset (\d+) length (\d+)\) /);
  const errposline = errposre[1];
  const errposoff = errposre[2];
  const errposlen = errposre[3];
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
  run: async function ({
    job
  }) {
    const args = job.arguments;
    const bin_arguments = [];
    let buildForWindows = false;
    if (args.dry_run) {
      bin_arguments.push('-r');
      bin_arguments.push('--gout');
      bin_arguments.push(args.output_name + '.dot');
    } else {
      if (args.arguments.object_format) {
        bin_arguments.push('--object-format');
        bin_arguments.push(args.arguments.object_format);
      }
      if (args.arguments.target) {
        bin_arguments.push('--target');
        bin_arguments.push(args.arguments.target);
      } else {
        if (args.arguments.target_arch) {
          bin_arguments.push('--target-arch');
          bin_arguments.push(args.arguments.target_arch);
        }
        if (args.arguments.target_vendor) {
          bin_arguments.push('--target-vendor');
          bin_arguments.push(args.arguments.target_vendor);
        }
        if (args.arguments.target_sys) {
          bin_arguments.push('--target-sys');
          bin_arguments.push(args.arguments.target_sys);
          buildForWindows = args.arguments.target_sys === 'windows';
        }
        if (args.arguments.target_env) {
          bin_arguments.push('--target-env');
          bin_arguments.push(args.arguments.target_env);
        }
      }
      if (args.arguments.library) {
        bin_arguments.push('--library');
      }
      if (args.arguments.cpu) {
        bin_arguments.push('-mcpu');
        bin_arguments.push(args.arguments.cpu);
      }
      if (args.arguments.features) {
        bin_arguments.push('--features');
        bin_arguments.push(args.arguments.features);
      }
      if (args.arguments.relocation_model) {
        bin_arguments.push('--relocation-model');
        bin_arguments.push(args.arguments.relocation_model);
      }
      bin_arguments.push('-o');
      bin_arguments.push(args.output_name + '.out');
    }
    bin_arguments.push(args.output_name + '.in');

    await new Promise((resolve) => fs.writeFile(args.output_name + '.in', job.source + '\n\n', () => {
      resolve();
    }));

    const child = ps.spawn(nlexBinary, bin_arguments);
    console.log(bin_arguments);
    let diagnostics = '';

    function handle_data (data) {
      diagnostics += data.toString();
    }

    child.stderr.on('data', handle_data);
    child.stdout.on('data', handle_data);

    return new Promise((resolve, reject) => {
      child.on('exit', (code, signal) => {
        console.log('code', code, signal);
        const _diagnostics = [];
        for (const line of diagnostics.split('\n')) {
          console.log(line);
          if (line.startsWith('[E0')) {
            _diagnostics.push(read_error(line));
          }
        }
        if (code === 0) {
            if (buildForWindows) {
              const wind = ps.spawn('lld-link', ['/dll', '/nodefaultlib', '/noentry', `/def:${args.output_name}.def`, `/out:${args.output_name}.dll`, args.output_name + '.out']);
              wind.on('exit', (code) => {
                resolve({
                  diagnostics: _diagnostics,
                  outputName: args.output_name + '.out',
                  windowsBuild: true,
                  ok: code === 0
                });
              });
            } else {
              const wind = ps.spawn('clang', ['-shared', args.output_name + '.out', '-o', args.output_name + '.so']);
              wind.on('exit', (code) => {
                console.log({_diagnostics, code});
                resolve({
                  diagnostics: _diagnostics,
                  outputName: args.output_name + '.out',
                  windowsBuild: false,
                  ok: code === 0
                });
              });
            }
        } else {
          resolve({
            diagnostics: _diagnostics,
            outputName: args.output_name + '.out',
            ok: code === 0
          });
        }
      });
      child.on('error', err => {
        console.log('error', err);
        reject(err);
      });
    });
  }
};
