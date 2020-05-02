const {ArrayType, UserArguments, Diagnostic, namedFile, Arguments, Enum2, Enum1, Enum0} = require("../types.js");

const {Job, CompiledFile} = require("../models.js");

/**
 * store in database
 */
/**
 * res_output
 * @param file_id: String
 */

/**
 * res_input
 * @param job_id: String
 */

let constructFile_id = x => (String).call(null, x);
module.exports = async function action_compile_1(res, req, res_output, res_input) {
    let files = res_input.files;
    let insert = await CompiledFile.db().insertOne({
        files: files,
        createdAt: new Date()
    });
    res_output.file_id = insert.insertedId;
    return true;
}
