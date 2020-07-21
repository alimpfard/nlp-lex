const {
  ArrayType,
  namedFile,
  Diagnostic,
  UserArguments,
  Arguments,
  Enum2,
  Enum1,
  Enum0
} = require("../types.js");

const {
  Job,
  CompiledFile
} = require("../models.js");

const fs = require('fs');

const Archiver = require('archiver');


/**
 * download
 */
/**
 * res_output
 */

/**
 * res_input
 * @param file_id: String
 */

module.exports = async function action_download_0(res, req, res_output, res_input, total_failure) {
  let files = res_input.files;
  let zip = Archiver('zip');
  zip.pipe(res);
  for (let file of files)
    zip.append(file.data.buffer, {name: file.name});
  zip.finalize();

  total_failure.action_handled_response = true;
  return true;
}
