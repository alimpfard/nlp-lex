## NLP-Lex (NLex)

Hopefully a lexer-generator that doesn't suck


## Syntax

regex token rules can be defined as such

```
    rule_name :: regular_expression
```

and literal token rules can be defined as:

```
  rule_name -- "some" "token" "\uc{SomeCategory}"
```

where `SomeCategory` is a predefined name (internal) for a category of tokens; the currently supported categories are:

| Category name | description |
| :------------ | :---------- |
| `RGI`         | the set of RGI emojis as defined in https://unicode.org/Public/emoji/ |

non-captured (inlined) literal matches can be defined as such:

```
    constant_name :- "literal string to match"

#   === or ===

    constant_name :- -"file/to/read/string/from.txt"
```

normalisation rules can be defined as such:

```
  normalise { codepoints to normalise } to codepoint
```

  for example
    `normalise { b c d e f } to a`
    means "normalise any of 'bcdef' to 'a'"


stopwords can be specified like so:

```
  stopword "stop" "word" "and" "or" "whatever"

#   and with file-strings (not yet impl'd)

  stopword -"list-of-stopwords.txt"
```

to completely omit the result of a rule (discard its match), use the `ignore` statement:

```
  ignore [ some rules to omit ]
```

for example `ignore [space punct]` will omit all the tokens that would've matched either of the rules `space` or `punct`

### Options

an option is of the form `option <name> <value>` where \<value\> is either `on` or `off`

currently significant options:

| Option         | effect         | default        |
| :------------- | :------------- | :------------- |
| `ignore_stopwords` | entirely removes stopwords from the output stream       | `off`       |
| `pure_normaliser`   | creates a function `__nlex_pure_normalise` that returns the next normalised character in the stream | `off` |
| `unsafe_normaliser`   | disable the length check on normalised values | `off` |
| `skip_on_error`       | skip unmatchable characters | `off` |

### Regular Expressions

the regex engine is currently very limited in what it supports, however here is a road map:

- [X] execution
- [X] standalone execution
- [X] state traversal codegen
- [X] helper functions codegen
- [X] normalisation codegen
- [X] stopword codegen
- [X] normalisation declarations (`c <= abcd`)
- [X] stopword declarations (`stopwords "stop" "word"`)
- [X] normal literal character matching
- [X] sequence matching (/ab/ -> match a, match b)
- [X] rule substitution (`a :: {{b}}x` -> a = match b, then match 'x')
- [X] semi-optimised alternative matching
- [X] basic character classes
- [X] ranges in character classes
- [ ] character classe extensions
- [ ] valid escapes in character classes (`[\p{Ll}\p{Lm}]`)
- [X] unicode character classes (`\p{...}`)
- [ ] optimised alternative matching
- [ ] alternatives with priorities
- [ ] File strings
- [X] Zero-width assertions
- [X] simple quantifiers (\+, \*)
- [X] medium-simple quantifiers ({x,y})
- [X] not-simple quantifiers (?)
- [X] insane quantifiers (??)
- [ ] rule actions (design a DSL for it...?)
- [ ] backreferences
- [X] recursive matching

read [regex_flavour](regex_flavour.md) for further details on the specific flavour of regular expressions used in tandem with the generator
