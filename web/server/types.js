module.exports = {};
let Arguments = {
	'target': arg => String.call(null, arg),
	'output_name': arg => String.call(null, arg),
	'dry_run': arg => Boolean.call(null, arg),
	'image_format': arg => String.call(null, arg),
	aliasedName: {},
	staticProperties: ["call", "staticProperties", "forEach", "aliasedName"],
	call(_, value) {
		let obj = {};
		for (property in Arguments)
			if ((!Arguments.staticProperties.includes(property))) {
				if (Arguments.hasOwnProperty(property))
					obj[property] = Arguments[property](value[property]);
				if (Arguments.aliasedName[property])
						obj[property] = Arguments[property](value[Arguments.aliasedName[property]]);
			}
		return obj;
	},
	forEach(self, callback) {
		for (property in Arguments)
			if ((!Arguments.staticProperties.includes(property)) && self.hasOwnProperty(property))
				callback(property, self[property], self);
	}
};
module.exports["Arguments"] = Arguments

let Diagnostic = {
	'data': arg => String.call(null, arg),
	'level': arg => Enum0.call(null, arg),
	'column': arg => Number.call(null, arg),
	'line': arg => Number.call(null, arg),
	aliasedName: {},
	staticProperties: ["call", "staticProperties", "forEach", "aliasedName"],
	call(_, value) {
		let obj = {};
		for (property in Diagnostic)
			if ((!Diagnostic.staticProperties.includes(property))) {
				if (Diagnostic.hasOwnProperty(property))
					obj[property] = Diagnostic[property](value[property]);
				if (Diagnostic.aliasedName[property])
						obj[property] = Diagnostic[property](value[Diagnostic.aliasedName[property]]);
			}
		return obj;
	},
	forEach(self, callback) {
		for (property in Diagnostic)
			if ((!Diagnostic.staticProperties.includes(property)) && self.hasOwnProperty(property))
				callback(property, self[property], self);
	}
};
module.exports["Diagnostic"] = Diagnostic

let Enum1 = {
	choices: ["res/ace/mode-nlex.js", "editor.html", "res/ace/ace.js", "res/ace/theme-twilight.js"],
	call(_, value) {
		if (Enum1.choices.includes(value))
			return value;
		else
			return undefined;
	}
};
module.exports["Enum1"] = Enum1
let Enum0 = {
	choices: ["error", "warning"],
	call(_, value) {
		if (Enum0.choices.includes(value))
			return value;
		else
			return undefined;
	}
};
module.exports["Enum0"] = Enum0
function ArrayType(inner) {
	return {
		call(_, value) {
			return Array.prototype.slice.call(value).map(x => inner.call(null, x));
		}
	};
};
module.exports["ArrayType"] = ArrayType;
