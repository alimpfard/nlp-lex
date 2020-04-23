const { ArrayType, UserArguments, Arguments, Diagnostic, Enum2, Enum1, Enum0 } = require("../types.js");

const { Job } = require("../models.js");

const fs = require('fs');

const zip = require('express-zip');

/**
 * download
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
module.exports = async function action_compile_1(res, req, res_output, res_input, total_failure) {
    let result = res_input._result;
    let job = res_input.job;
    let bytes = result.outputName;
    let filename = result.outputName.replace('.out', '');
    let module_definition = job.arguments.arguments.target_sys === 'windows' ? result.outputName + '.def' : ''

    res.zip([
        { path: result.outputName, name: 'tokenizer.obj' }
    ].concat(module_definition ? [{ path: module_definition, name: 'tokenizer.def' }] : []));

    total_failure.action_handled_response = true;

    return true;
}