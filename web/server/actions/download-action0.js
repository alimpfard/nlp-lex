const {ArrayType, namedFile, Diagnostic, UserArguments, Arguments, Enum2, Enum1, Enum0} = require("../types.js");

const {Job, CompiledFile} = require("../models.js");

const async = require('async')
  , fs = require('fs')
  , express = require('express')
  , res = express.response
  , zipstream = require('zip-stream');

const { Readable } = require('stream')


res.zip = function(files, filename) {
  let cb = function() {};

  this.header('Content-Type', 'application/zip');
  this.header('Content-Disposition', 'attachment; filename="' + filename + '"');

  var zip = zipstream(exports.options);
  zip.pipe(this); // res is a writable stream

  var addFile = function(file, cb) {
    zip.entry(Readable.from(file.data), { name: file.name }, cb);
  };

  async.forEachSeries(files, addFile, function(err) {
    if (err) return cb(err);
    zip.finalize();
    cb(null, zip.getBytesWritten());
  });
};

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
    res.zip(files, 'attachment.zip');
    total_failure.action_handled_response = true;
    return true;
}
