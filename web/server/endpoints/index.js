const { ArrayType, Diagnostic, Enum0 } = require("../types.js");

const { Job } = require("../models.js");
const { readFile } = require('fs');

/**
 * res_output
 * = String
 */

/**
 * res_input
 */
module.exports = async function process_index(res_input, res_output) {
    return new Promise((resolve, reject) => readFile('../' + res_input.res,
        (err, data) => {
            if (err)
                reject(err);
            else
                resolve(data);
        }));
}