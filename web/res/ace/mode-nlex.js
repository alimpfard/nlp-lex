/*
 * To try in Ace editor, copy and paste into the mode creator
 * here : http://ace.c9.io/tool/mode_creator.html
 */

define("ace/mode/nlex_highlight_rules", ["require", "exports", "module", "ace/lib/oop"], function(require, exports, module) {
    "use strict";
    var oop = require("../lib/oop");
    var TextHighlightRules = require("./text_highlight_rules").TextHighlightRules;
    /* --------------------- START ----------------------------- */
    var NlexHighlightRules = function() {
        this.$rules = {
            "start": [{
                    "token": "comment",
                    "regex": "(#.*$)"
                },
                {
                    "token": ["keyword", "punctuation", "entity.name.type.class", "punctuation", "keyword.other.special-method"],
                    "regex": "(option)(\\s+)(\\w+)(\\s+)(on|off)$"
                },
                {
                    "token": ["keyword", "punctuation"],
                    "regex": "(define)(\\s+)",
                    "push": "kaleid_out_of_line"
                },
                {
                    "token": ["keyword punctuation", "punctuation", "keyword"],
                    "regex": "(tag)(\\s*)(pos)",
                    "push": "main__1"
                },
                {
                    "token": ["entity.name.type.class", "punctuation", "keyword punctuation"],
                    "regex": "(\\w+)(\\s*)(::)",
                    "push": "main__2"
                },
                {
                    "token": ["entity.name.type.class", "punctuation", "keyword punctuation", "punctuation", "punctuation"],
                    "regex": "(\\w+)(\\s*)(:-)(\\s*)(\"[^\"]*\")",
                    "push": "main__3"
                },
                {
                    "token": ["entity.name.type.class", "punctuation", "keyword punctuation"],
                    "regex": "(\\w+)(\\s*)(:-)",
                    "push": "main__4"
                },
                {
                    "token": ["entity.name.type.class", "punctuation", "keyword punctuation"],
                    "regex": "(\\w+)(\\s*)(--)",
                    "push": "main__5"
                },
                {
                    "token": ["keyword", "punctuation", "punctuation", "constant.character", "punctuation", "punctuation", "keyword", "constant.character"],
                    "regex": "(normalise)(\\s+)(\\{)((?:[^}]|\\\\})*)(\\})(\\s+)(to)(.)",
                    "push": "main__6"
                },
                {
                    "token": ["keyword", "punctuation"],
                    "regex": "(stopword)(\\s*)",
                    "push": "stopwords"
                },
                {
                    "token": ["keyword", "punctuation"],
                    "regex": "(ignore)(\\s*)",
                    "push": "ignores"
                },
                {
                    defaultToken: "text",
                }
            ],
            "ignores": [{
                    "token": "punctuation",
                    "regex": "(^)",
                    "next": "pop"
                },
                {
                    "token": ["entity.name.type.class", "punctuation"],
                    "regex": "(\\w+)(\\s*)"
                },
                {
                    defaultToken: "text",
                }
            ],
            "kaleid": [{
                    "token": "constant.character.escape.comment.block.preprocessor",
                    "regex": "(\\})",
                    "next": "pop"
                },
                {
                    "token": "constant.numeric.comment.block.preprocessor",
                    "regex": "(\\d+|\\d*\\.\\d+)"
                },
                {
                    "token": ["string.comment.block.preprocessor", "string.comment.block.preprocessor", "string.comment.block.preprocessor"],
                    "regex": "(\")((?:[^\"]|\\\\\")*)(\")"
                },
                {
                    "token": "keyword.comment.block.preprocessor",
                    "regex": "(\\b(?:if|then|else|def|extern|for|in|var|global|while)\\b)"
                },
                {
                    "token": ["entity.name.function.comment.block.preprocessor", "keyword.operator.comment.block.preprocessor"],
                    "regex": "(\\w+)(\\s*\\(\\s*)"
                },
                {
                    "token": "keyword.operator.comment.block.preprocessor",
                    "regex": "(\\)|,)"
                },
                {
                    "token": "entity.name.function.comment.block.preprocessor",
                    "regex": "(\\w+)"
                },
                {
                    "token": "keyword.operator.comment.block.preprocessor",
                    "regex": "([^\"\\w]+)"
                },
                {
                    defaultToken: "text",
                }
            ],
            "kaleid_out_of_line": [{
                    "token": "constant.character.escape.comment.block.preprocessor",
                    "regex": "(^$)",
                    "next": "pop"
                },
                {
                    "token": "comment.block.preprocessor",
                    "regex": "(\\\\$)"
                },
                {
                    "token": "constant.numeric.comment.block.preprocessor",
                    "regex": "(\\d+|\\d*\\.\\d+)"
                },
                {
                    "token": ["string.comment.block.preprocessor", "string.comment.block.preprocessor", "string.comment.block.preprocessor"],
                    "regex": "(\")((?:[^\"]|\\\\\")*)(\")"
                },
                {
                    "token": "keyword.comment.block.preprocessor",
                    "regex": "(\\b(?:if|then|else|def|extern|for|in|var|global|while)\\b)"
                },
                {
                    "token": ["entity.name.function.comment.block.preprocessor", "keyword.operator.comment.block.preprocessor"],
                    "regex": "(\\w+)(\\s*\\(\\s*)"
                },
                {
                    "token": "keyword.operator.comment.block.preprocessor",
                    "regex": "(\\)|,)"
                },
                {
                    "token": "entity.name.function.comment.block.preprocessor",
                    "regex": "(\\w+)"
                },
                {
                    "token": "keyword.operator.comment.block.preprocessor",
                    "regex": "([^\"\\w]+)"
                },
                {
                    defaultToken: "text",
                }
            ],
            "main__1": [{
                    "token": "punctuation",
                    "regex": "(^(?=.{0,1})(?:|))",
                    "next": "pop"
                },
                {
                    "token": "punctuation",
                    "regex": "(\\s*)",
                    "push": "tag_pos"
                },
                {
                    defaultToken: "text",
                }
            ],
            "main__2": [{
                    "token": "punctuation",
                    "regex": "(^(?=.{0,1})(?:|))",
                    "next": "pop"
                },
                {
                    "token": "punctuation",
                    "regex": "(\\s*)",
                    "push": "regularExpression"
                },
                {
                    defaultToken: "text",
                }
            ],
            "main__3": [{
                    "token": "punctuation",
                    "regex": "(^(?=.{0,1})(?:|))",
                    "next": "pop"
                },
                {
                    defaultToken: "entity.name.type.class",
                }
            ],
            "main__4": [{
                    "token": "punctuation",
                    "regex": "(^(?=.{0,1})(?:|))",
                    "next": "pop"
                },
                {
                    "token": "punctuation",
                    "regex": "(\\s*)",
                    "push": "regularExpression"
                },
                {
                    defaultToken: "text",
                }
            ],
            "main__5": [{
                    "token": "punctuation",
                    "regex": "(^(?=.{0,1})(?:|))",
                    "next": "pop"
                },
                {
                    "token": ["keyword punctuation", "punctuation", "entity.name.type.class", "string", "punctuation"],
                    "regex": "(-?)(\")(?:(\\\\uc\\{[^}]*\\})|((?:[^\"]|\\\\\")*))(\")"
                },
                {
                    defaultToken: "text",
                }
            ],
            "main__6": [{
                    "token": "punctuation",
                    "regex": "(^(?=.{0,1})(?:|))",
                    "next": "pop"
                },
                {
                    defaultToken: "constant.character",
                }
            ],
            "regularExpression": [{
                    "token": "string.regexp",
                    "regex": "(^|\\)|\\])",
                    "next": "pop"
                },
                {
                    "token": ["string.regexp", "comment", "string.regexp"],
                    "regex": "(\\(\\?\\#)([^)]*)(\\))"
                },
                {
                    "token": "string.regexp",
                    "regex": "(\\(\\?.)",
                    "push": "regularExpression"
                },
                {
                    "token": "string.regexp",
                    "regex": "(\\()",
                    "push": "regularExpression"
                },
                {
                    "token": ["constant.character.escape", "string.other.link", "string.regexp", "string.other.link", "constant.character.escape"],
                    "regex": "(\\{)(\\d+)(,?)(\\d+)?(\\})"
                },
                {
                    "token": "string.regexp",
                    "regex": "(\\\\u[0-9a-fA-F]{1,6})"
                },
                {
                    "token": "string.other.link",
                    "regex": "(\\||\\\\[^pguE])",
                    "push": "regularExpression"
                },
                {
                    "token": "constant.character.escape.comment.block.preprocessor",
                    "regex": "(\\\\E\\{)",
                    "push": "kaleid"
                },
                {
                    "token": ["string.other.link", "string.regexp"],
                    "regex": "(\\\\g)(\\d+)",
                    "push": "regularExpression"
                },
                {
                    "token": ["string.other.link", "constant.character.escape", "string.regexp", "constant.character.escape"],
                    "regex": "(\\\\p)(\\{)(\\w+)(\\})",
                    "push": "regularExpression"
                },
                {
                    "token": ["constant.character.escape", "entity.name.type.class", "constant.character.escape"],
                    "regex": "(\\{\\{)(\\w+)(\\}\\})"
                },
                {
                    "token": "string.regexp",
                    "regex": "(\\[)",
                    "push": "regularExpression__1"
                },
                {
                    "token": "string.regexp",
                    "regex": "(\\+|\\*|\\?)"
                },
                {
                    "token": "constant.character",
                    "regex": "(.)"
                },
                {
                    defaultToken: "text",
                }
            ],
            "regularExpression__1": [{
                    "token": "string.regexp",
                    "regex": "(\\])",
                    "next": "pop"
                },
                {
                    "token": "string.other.link",
                    "regex": "(\\\\u[0-9a-fA-F]{1,6})"
                },
                {
                    "token": "string.regexp",
                    "regex": "([^\\\\\\]])"
                },
                {
                    "token": ["string.other.link", "string.other.link"],
                    "regex": "(\\\\)([^u])"
                },
                {
                    defaultToken: "text",
                }
            ],
            "stopwords": [{
                    "token": "punctuation",
                    "regex": "(^)",
                    "next": "pop"
                },
                {
                    "token": ["keyword punctuation", "punctuation", "string", "punctuation"],
                    "regex": "(-?)(\")([^\"]*)(\")"
                },
                {
                    defaultToken: "text",
                }
            ],
            "tag_pos": [{
                    "token": "punctuation",
                    "regex": "(^(?=.{0,1})(?:|))",
                    "next": "pop"
                },
                {
                    "token": ["keyword punctuation", "punctuation", "keyword", "punctuation", "keyword punctuation"],
                    "regex": "(every)(\\s*)(\\d+)(\\s*)(tokens?)"
                },
                {
                    "token": ["keyword punctuation", "punctuation", "string"],
                    "regex": "(from)(\\s*)(\"(?:[^\"]|\\\\\")*\")"
                },
                {
                    "token": ["keyword punctuation", "punctuation", "keyword punctuation", "punctuation", "entity.name.type.class", "punctuation", "constant.character", "punctuation"],
                    "regex": "(with)?(\\s*)(delimiter)(\\s*)(\\w+)(\\{)([^{]+)(\\})"
                },
                {
                    defaultToken: "text",
                }
            ]
        };
        this.normalizeRules();
    };
    /* ------------------------ END ------------------------------ */
    oop.inherits(NlexHighlightRules, TextHighlightRules);
    exports.Rules = NlexHighlightRules;
});

define("ace/mode/nlex", ["require", "exports", "module", "ace/lib/oop", "ace/mode/text", "ace/mode/nlex"], function(require, exports, module) {
    "use strict";

    var oop = require("../lib/oop");
    var TextMode = require("./text").Mode;
    var NSISHighlightRules = require("./nlex_highlight_rules").Rules;

    var Mode = function() {
        this.HighlightRules = NSISHighlightRules;
        this.$behaviour = this.$defaultBehaviour;
    };
    oop.inherits(Mode, TextMode);

    (function() {
        this.lineCommentStart = ["#"];
        this.$id = "ace/mode/nlex";
    }).call(Mode.prototype);

    exports.Mode = Mode;
});
(function() {
    window.require(["ace/mode/nlex"], function(m) {
        if (typeof module == "object" && typeof exports == "object" && module) {
            module.exports = m;
        }
    });
})();