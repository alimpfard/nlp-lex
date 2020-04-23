module.exports = {};
let Diagnostic = {
	'line': arg => Number.call(null, arg),
	'level': arg => Enum0.call(null, arg),
	'column': arg => Number.call(null, arg),
	'data': arg => String.call(null, arg),
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

let UserArguments = {
	'target': arg => String.call(null, arg),
	'object_format': arg => String.call(null, arg),
	'target_arch': arg => String.call(null, arg),
	'target_env': arg => String.call(null, arg),
	'llvm': arg => Boolean.call(null, arg),
	'library': arg => Boolean.call(null, arg),
	'target_vendor': arg => String.call(null, arg),
	'target_sys': arg => String.call(null, arg),
	'features': arg => String.call(null, arg),
	'cpu': arg => String.call(null, arg),
	aliasedName: {},
	staticProperties: ["call", "staticProperties", "forEach", "aliasedName"],
	call(_, value) {
		let obj = {};
		for (property in UserArguments)
			if ((!UserArguments.staticProperties.includes(property))) {
				if (UserArguments.hasOwnProperty(property))
					obj[property] = UserArguments[property](value[property]);
				if (UserArguments.aliasedName[property])
						obj[property] = UserArguments[property](value[UserArguments.aliasedName[property]]);
			}
		return obj;
	},
	forEach(self, callback) {
		for (property in UserArguments)
			if ((!UserArguments.staticProperties.includes(property)) && self.hasOwnProperty(property))
				callback(property, self[property], self);
	}
};
module.exports["UserArguments"] = UserArguments

let Arguments = {
	'arguments': arg => UserArguments.call(null, arg),
	'dry_run': arg => Boolean.call(null, arg),
	'output_name': arg => String.call(null, arg),
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

let Enum2 = {
	choices: ["persian.nlex", "almost-everything.nlex", "branch-reset.nlex", "multiple-paths-converge.nlex", "normalisations.nlex", "csyntax.nlex", "multiline.nlex", "character-classes.nlex", "medium-qtfs.nlex", "basic-subexpr-call.nlex", "kdef.nlex", "backreference.nlex", "star-mutually-recursive.nlex", "test.nlex", "rg.nlex", "capture.nlex", "simple-inline-code.nlex", "repeat-qtf.nlex", "implicit-dependency.nlex", "basic-quantifiers.nlex", "pure_normaliser.nlex", "multiple-paths-with-quantifier.nlex"],
	call(_, value) {
		if (Enum2.choices.includes(value))
			return value;
		else
			return undefined;
	}
};
module.exports["Enum2"] = Enum2
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
