const { ArrayType, Diagnostic, Enum0 } = require("../types.js");

const { Job } = require("../models.js");

/**
 * set content type
 */
/**
 * res_output
 * = String
 */

/**
 * res_input
 * @param res: String
 */

module.exports = async function action_index_0(res, req, res_output, res_input) {
    res.type(res_input.res.includes('.js') ? 'text/javascript' : 'text/html')
    return true;
}