const {
  ArrayType,
  Diagnostic,
  Arguments,
  UserArguments,
  Enum2,
  Enum1,
  Enum0
} = require("../types.js");
const {
  Job,
  CompiledFile
} = require("../models.js");

const {
  ObjectId
} = require('mongodb');

module.exports = async function resolveDownloadReference(args, failure, file) {
  let file_id = args.file_id;
  file.value = (await CompiledFile.db().findOne({
    _id: new ObjectId(file_id)
  })).files;
  return true;
}