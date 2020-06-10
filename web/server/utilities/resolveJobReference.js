const {
  ArrayType,
  Diagnostic,
  Enum0
} = require("../types.js");
const {
  Job
} = require("../models.js");

module.exports = async function resolveJobReference(args, failure, job) {
  let job_id = args.job_id;
  let value = Job.findById(job_id);
  if (value == null)
    return false;
  job.value = value;
  return true;
}