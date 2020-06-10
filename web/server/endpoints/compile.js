const {
  ArrayType,
  UserArguments,
  Diagnostic,
  Arguments,
  Enum2,
  Enum1,
  Enum0
} = require("../types.js");

const {
  Job
} = require("../models.js");

const {
  run
} = require('../nlex-wrapper.js');
const fs = require('fs');

/**
 * res_output
 * @param bytes: ArrayType(Number)
 * @param extension: String
 */

/**
 * res_input
 * @param job_id: String
 */

let constructBytes = x => (ArrayType(Number)).call(null, x);
let constructExtension = x => (String).call(null, x);


module.exports = async function process_compile(res_input, res_output) {
  return res_output;
}