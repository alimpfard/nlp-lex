const {
  ArrayType,
  UserArguments,
  Arguments,
  Diagnostic,
  Enum2,
  Enum1,
  Enum0
} = require('../types.js');

const {
  Job
} = require('../models.js');

const {
  run
} = require('../nlex-wrapper.js');
const fs = require('fs');
/**
 * compile
 */
/**
 * res_output
 * @param bytes: ArrayType(Number)
 * @param module_definition: String
 * @param extension: String
 */

/**
 * res_input
 * @param job_id: String
 */

const constructBytes = x => (ArrayType(Number)).call(null, x);
const constructModule_definition = x => (String).call(null, x);
const constructExtension = x => (String).call(null, x);
module.exports = async function action_compile_0 (res, req, res_output, res_input) {
  const job = await res_input.job;
  job.arguments.dry_run = false;
  const result = await run({
    job
  });

  res_input._result = result;
  res_input.job = job;

  res_output.ok = result.ok;

  if (result.ok) {
    const bytes = result.outputName;
    const filename = result.outputName.replace('.out', '');
    const module_definition = job.arguments.arguments.target_sys === 'windows' ? filename + '.def' : '';

    const files = [{
      data: fs.readFileSync(result.outputName),
      name: 'tokenizer.obj'
    }];
    if (module_definition) {
      files.push({
        data: fs.readFileSync(module_definition),
        name: 'tokenizer.def'
      });
      files.push({
        data: fs.readFileSync(filename + '.dll'),
        name: 'tokenizer.dll'
      });
    } else {
      files.push({
        data: fs.readFileSync(filename + '.so'),
        name: 'tokenizer.so'
      });
    }

    res_input.files = files;
  }

  return result.ok;
}
;
