const { ArrayType, Diagnostic, UserArguments } = require("../types.js");

const { Job } = require("../models.js");

const { run } = require('../nlex-wrapper.js');
const fs = require('fs');

/**
 * res_output
 * @param diagnostics: ArrayType(Diagnostic)
 * @param identifier: String
 */

/**
 * res_input
 * @param source: String
 */

let constructDiagnostics = x => (ArrayType(Diagnostic)).call(null, x);
let constructIdentifier = x => (String).call(null, x);


module.exports = async function process_run(res_input, res_output) {
    const outputName = 'aaa';
    const arguments = {};
    UserArguments.forEach(res_input.arguments, (k, v) => {
        if (v !== undefined && v !== '' && v != 'undefined')
            arguments[k] = v;
    });
    let job = {
        source: res_input.source,
        arguments: {
            dry_run: true,
            output_name: outputName,
            arguments: arguments
        },
        is_done: false,
        output_name: outputName
    };
    let ident = await Job.db().insertOne(job);
    console.log(ident);
    res_output.identifier = ident.insertedId;
    res_output.diagnostics = [];
    for (let diagnostic of(await run({ job })).diagnostics)
        res_output.diagnostics.push({
            line: diagnostic.position.line,
            column: diagnostic.position.offset,
            level: 'error',
            data: diagnostic.message
        });
    res_output.dot_data = fs.readFileSync(outputName + '.dot').toString();
    return res_output;
}