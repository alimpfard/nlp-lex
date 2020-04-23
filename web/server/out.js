var express = require("express");
var app = express();

db = null;
dbsession = null;
const config = require("./config.js");
const MongoClient = require("mongodb").MongoClient;

const process_compile = require("./endpoints/compile.js");
const process_run = require("./endpoints/run.js");
const process_index = require("./endpoints/index.js");
const action_index_0 = require("./actions/index-action0.js");

const {ArrayType, Arguments, Diagnostic, Enum1, Enum0} = require("./types.js");
const {Job} = require("./models.js");
const resolveJobReference = require("./utilities/resolveJobReference.js");

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
app.post("/compile", async (req, res) => {
	console.log("[DEBUG] activity on route compile");
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
				'b64encoded': null /* String */,
			};

			(total_failure.action_handled_response ? (x=>{console.log(x)}) : x=>{res.send(x)})(await process_compile(res_input, res_output));
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
			};

			res_output = {
				'diagnostics': null /* ArrayType(Diagnostic) */,
				'dot_data': null /* String */,
				'identifier': null /* String */,
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
MongoClient.connect(config.mongo_addr, {useNewUrlParser: true}, (err, client) => {
	if (err) return console.log(err);
	dbsession = client.startSession();
	db = client.db(config.database);
	app.listen(config.port, () => console.log("App listening on port " + config.port));
});
