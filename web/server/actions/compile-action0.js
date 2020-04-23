const { ArrayType, UserArguments, Arguments, Diagnostic, Enum2, Enum1, Enum0 } = require("../types.js");

const { Job } = require("../models.js");

const { run } = require('../nlex-wrapper.js');
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

let constructBytes = x => (ArrayType(Number)).call(null, x);
let constructModule_definition = x => (String).call(null, x);
let constructExtension = x => (String).call(null, x);
module.exports = async function action_compile_0(res, req, res_output, res_input) {
    let job = await res_input.job;
    job.arguments.dry_run = false;
    let result = await run({ job });

    res_input._result = result;
    res_input.job = job;

    res_output.ok = result.ok;

    return result.ok;
}