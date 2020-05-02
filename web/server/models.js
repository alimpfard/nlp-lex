const {ArrayType, Arguments, Diagnostic, UserArguments, namedFile, Enum2, Enum1, Enum0} = require("./types.js");
const { ObjectId } = require("mongodb");
module.exports = {};
let Job = {
	'arguments': arg => Arguments.call(null, arg),
	'output_name': arg => String.call(null, arg),
	'is_done': arg => Boolean.call(null, arg),
	'source': arg => String.call(null, arg),
	aliasedName: {},
	staticProperties: ["call", "db", "findById", "staticProperties", "forEach", "aliasedName"],
	call(_, value) {
		let obj = {};
		for (property in Job)
			if ((!Job.staticProperties.includes(property))) {
				if (Job.hasOwnProperty(property))
					obj[property] = Job[property](value[property]);
				if (Job.aliasedName[property])
						obj[property] = Job[property](value[Job.aliasedName[property]]);
			}
		return obj;
	},
	forEach(self, callback) {
		for (property in Job)
			if ((!Job.staticProperties.includes(property)) && self.hasOwnProperty(property))
				callback(property, self[property], self);
	},
	db() { return db.collection("Job") },
	async findById(id) {
		return await db.collection("Job").findOne({
			$and:
				[
					{_id: new ObjectId(id)},
					...(Array.prototype.slice.call(arguments, 1))
				]
		});
	},
};
module.exports["Job"] = Job

let CompiledFile = {
	'files': arg => ArrayType(namedFile).call(null, arg),
	aliasedName: {},
	staticProperties: ["call", "db", "findById", "staticProperties", "forEach", "aliasedName"],
	call(_, value) {
		let obj = {};
		for (property in CompiledFile)
			if ((!CompiledFile.staticProperties.includes(property))) {
				if (CompiledFile.hasOwnProperty(property))
					obj[property] = CompiledFile[property](value[property]);
				if (CompiledFile.aliasedName[property])
						obj[property] = CompiledFile[property](value[CompiledFile.aliasedName[property]]);
			}
		return obj;
	},
	forEach(self, callback) {
		for (property in CompiledFile)
			if ((!CompiledFile.staticProperties.includes(property)) && self.hasOwnProperty(property))
				callback(property, self[property], self);
	},
	db() { return db.collection("CompiledFile") },
	async findById(id) {
		return await db.collection("CompiledFile").findOne({
			$and:
				[
					{_id: new ObjectId(id)},
					...(Array.prototype.slice.call(arguments, 1))
				]
		});
	},
};
module.exports["CompiledFile"] = CompiledFile

