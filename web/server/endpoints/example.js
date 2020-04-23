const { ArrayType, Arguments, Diagnostic, Enum2, Enum1, Enum0 } = require("../types.js");

const { Job } = require("../models.js");

const fs = require('fs');

/**
 * res_output
 * = String
 */

/**
 * res_input
 * @param name: Enum2
 */
module.exports = async function process_example(res_input, res_output) {
    return { example: fs.readFileSync('../nlex/examples/' + res_input.name).toString() };
}