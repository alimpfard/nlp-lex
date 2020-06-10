var express = require("express");
var app = express();

db = null;
dbsession = null;
const config = require("./config.js");
const MongoClient = require("mongodb").MongoClient;

const process_run = require("./endpoints/run.js");
const process_example = require("./endpoints/example.js");
const process_examples = require("./endpoints/examples.js");
const process_download = require("./endpoints/download.js");
const action_download_0 = require("./actions/download-action0.js");
const process_compile = require("./endpoints/compile.js");
const action_compile_0 = require("./actions/compile-action0.js");
const action_compile_1 = require("./actions/compile-action1.js");
const process_index = require("./endpoints/index.js");
const action_index_0 = require("./actions/index-action0.js");

const {ArrayType, Arguments, UserArguments, Diagnostic, namedFile, Enum2, Enum1, Enum0} = require("./types.js");
const {Job, CompiledFile} = require("./models.js");
const resolveJobReference = require("./utilities/resolveJobReference.js");
const resolveDownloadReference = require("./utilities/resolveDownloadReference.js");

function getOrFail(type, obj, prop, _default) {
	let oprop = obj[prop];
	if ((typeof(oprop) === "undefined" || oprop === null) && typeof(_default) === "undefined") {
		console.log("[DEBUG] nonexistent getOrFail for " + prop + " from " + JSON.stringify(obj));
		throw new Error(`missing argument ${prop}`);
	}
	if (typeof(oprop) === "undefined" || oprop === null)
		return _default;
	oprop = type.call(null, oprop);
	if ((typeof(oprop) === "undefined" || oprop === null) && typeof(_default) === "undefined") {
		console.log("[DEBUG] nonexistent getOrFail for " + prop + " from " + JSON.stringify(obj));
		throw new Error(`wrong type for argument ${prop}`);
	}
	return (typeof(oprop) === "undefined" || oprop === null) ? _default : oprop;
}
app.get("/res/ace/theme-twilight.js", async (req, res) => {
	console.log("[DEBUG] redirecting from /res/ace/theme-twilight.js to /index?res=res/ace/theme-twilight.js");
	res.redirect("/index?res=res/ace/theme-twilight.js");
});
app.get("/res/ace/ace.js", async (req, res) => {
	console.log("[DEBUG] redirecting from /res/ace/ace.js to /index?res=res/ace/ace.js");
	res.redirect("/index?res=res/ace/ace.js");
});
app.get("/editor.html", async (req, res) => {
	console.log("[DEBUG] redirecting from /editor.html to /index?res=editor.html");
	res.redirect("/index?res=editor.html");
});
app.get("/res/ace/mode-nlex.js", async (req, res) => {
	console.log("[DEBUG] redirecting from /res/ace/mode-nlex.js to /index?res=res/ace/mode-nlex.js");
	res.redirect("/index?res=res/ace/mode-nlex.js");
});
app.get("/", async (req, res) => {
	console.log("[DEBUG] redirecting from / to /index?res=editor.html");
	res.redirect("/index?res=editor.html");
});
app.post("/run", async (req, res) => {
	console.log("[DEBUG] activity on route run");
	var bodyStr = "";
	req.on("data", function(chunk) { 
		bodyStr += chunk.toString();
	});
	req.on("end", async function() {
		bodyStr = bodyStr
		let ok = false, handled = false, res_input = null, res_output = null;
		let jsondec = JSON.parse(bodyStr);
		let total_failure = {fail_early: false, action_handled_response: false};
		try {
			res_input = {
				'source': getOrFail(String, jsondec, 'source', undefined),
				'arguments': getOrFail(UserArguments, jsondec, 'arguments', undefined),
			};

			res_output = {
				'diagnostics': null /* ArrayType(Diagnostic) */,
				'identifier': null /* String */,
				'dot_data': null /* String */,
			};

			(total_failure.action_handled_response ? (x=>{console.log(x)}) : x=>{res.send(x)})(await process_run(res_input, res_output));
			ok = true;

		} catch(e) {
			console.error(e);
			ok = false;
			if (!total_failure.action_handled_response) {
				handled = true;
				res.status(500).send({_error: e.toString(), ...res_output});
			} else {
				handled = true;
			}
		} finally {
			if (!ok && !handled) res.status(500).send(res_output);
			}

});
	});
app.get("/index", async (req, res) => {
	console.log("[DEBUG] activity on index");
	let ok = false, handled = false, res_input = null, res_output = null;
		let total_failure = {fail_early: false, action_handled_response: false};
		try {
			res_input = {
				'res': getOrFail(Enum1, req.query, 'res', undefined),
			};

			res_output = {} /* String */;
			if (await action_index_0(res, req, res_output, res_input, total_failure) &&
			    true) {
				(total_failure.action_handled_response ? (x=>{console.log(x)}) : x=>{res.send(x)})(await process_index(res_input, res_output));
				ok = true;
			}
			else {
				ok = false;
			}
			handled = total_failure.action_handled_response;
		} catch(e) {
			console.error(e);
			ok = false;
			if (!total_failure.action_handled_response) {
				handled = true;
				res.status(500).send({_error: e.toString(), ...res_output});
			} else {
				handled = true;
			}
		} finally {
			if (!ok && !handled) res.status(500).send(res_output);
			}
	});
app.get("/example", async (req, res) => {
	console.log("[DEBUG] activity on example");
	let ok = false, handled = false, res_input = null, res_output = null;
		let total_failure = {fail_early: false, action_handled_response: false};
		try {
			res_input = {
				'name': getOrFail(Enum2, req.query, 'name', undefined),
			};

			res_output = {
				'example': null /* String */,
			};

			if (true) {
				(total_failure.action_handled_response ? (x=>{console.log(x)}) : x=>{res.send(x)})(await process_example(res_input, res_output));
				ok = true;
			}
			else {
				ok = false;
			}
			handled = total_failure.action_handled_response;
		} catch(e) {
			console.error(e);
			ok = false;
			if (!total_failure.action_handled_response) {
				handled = true;
				res.status(500).send({_error: e.toString(), ...res_output});
			} else {
				handled = true;
			}
		} finally {
			if (!ok && !handled) res.status(500).send(res_output);
			}
	});
app.get("/examples", async (req, res) => {
	console.log("[DEBUG] activity on examples");
	let ok = false, handled = false, res_input = null, res_output = null;
		let total_failure = {fail_early: false, action_handled_response: false};
		try {
			res_input = {
				'foo': getOrFail(String, req.query, 'foo', undefined),
			};

			res_output = {} /* ArrayType(String) */;
			if (true) {
				(total_failure.action_handled_response ? (x=>{console.log(x)}) : x=>{res.send(x)})(await process_examples(res_input, res_output));
				ok = true;
			}
			else {
				ok = false;
			}
			handled = total_failure.action_handled_response;
		} catch(e) {
			console.error(e);
			ok = false;
			if (!total_failure.action_handled_response) {
				handled = true;
				res.status(500).send({_error: e.toString(), ...res_output});
			} else {
				handled = true;
			}
		} finally {
			if (!ok && !handled) res.status(500).send(res_output);
			}
	});
app.get("/download", async (req, res) => {
	console.log("[DEBUG] activity on download");
	let ok = false, handled = false, res_input = null, res_output = null;
		let total_failure = {fail_early: false, action_handled_response: false};
		try {
			res_input = {
				'file_id': getOrFail(String, req.query, 'file_id', undefined),
			};

			let failure = {error:null, ok:null};
			let resolveDownloadReference_files = {set value(val) { res_input["files"] = val; }};
			if (!(await resolveDownloadReference({"file_id": res_input["file_id"]}, failure, resolveDownloadReference_files) &&
			    true)) {
				console.log("failure: ");
				console.log(failure);
				handled = true;
				res.status(500).send(failure);
				return;
			}
			res_output = {
			};

			if (await action_download_0(res, req, res_output, res_input, total_failure) &&
			    true) {
				(total_failure.action_handled_response ? (x=>{console.log(x)}) : x=>{res.send(x)})(await process_download(res_input, res_output));
				ok = true;
			}
			else {
				ok = false;
			}
			handled = total_failure.action_handled_response;
		} catch(e) {
			console.error(e);
			ok = false;
			if (!total_failure.action_handled_response) {
				handled = true;
				res.status(500).send({_error: e.toString(), ...res_output});
			} else {
				handled = true;
			}
		} finally {
			if (!ok && !handled) res.status(500).send(res_output);
			}
	});
app.post("/compile", async (req, res) => {
	console.log("[DEBUG] activity on compile");
	var bodyStr = "";
	req.on("data", function(chunk) { 
		bodyStr += chunk.toString();
	});
	req.on("end", async function() {
		bodyStr = bodyStr
		let ok = false, handled = false, res_input = null, res_output = null;
		let jsondec = JSON.parse(bodyStr);
			dbsession.startTransaction();
		let total_failure = {fail_early: false, action_handled_response: false};
		try {
			res_input = {
				'job_id': getOrFail(String, jsondec, 'job_id', undefined),
			};

			let failure = {error:null, ok:null};
			let resolveJobReference_job = {set value(val) { res_input["job"] = val; }};
			if (!(await resolveJobReference({"job_id": res_input["job_id"]}, failure, resolveJobReference_job) &&
			    true)) {
				console.log("failure: ");
				console.log(failure);
				handled = true;
				res.status(500).send(failure);
				return;
			}
			res_output = {
				'file_id': null /* String */,
			};

			if (await action_compile_0(res, req, res_output, res_input, total_failure) &&
			    await action_compile_1(res, req, res_output, res_input, total_failure) &&
			    true) {
				(total_failure.action_handled_response ? (x=>{console.log(x)}) : x=>{res.send(x)})(await process_compile(res_input, res_output));
				ok = true;
			}
			else {
				ok = false;
			}
			handled = total_failure.action_handled_response;
		} catch(e) {
			console.error(e);
			ok = false;
			if (!total_failure.action_handled_response) {
				handled = true;
				res.status(500).send({_error: e.toString(), ...res_output});
			} else {
				handled = true;
			}
		} finally {
			if (ok && !total_failure.fail_early)
				await dbsession.commitTransaction();
			else await dbsession.abortTransaction();
			if (!ok && !handled) res.status(500).send(res_output);
			}
	});
	});
MongoClient.connect(config.mongo_addr, {useNewUrlParser: true}, (err, client) => {
	if (err) return console.log(err);
	dbsession = client.startSession();
	db = client.db(config.database);
	app.listen(config.port, () => console.log("App listening on port " + config.port));
});
