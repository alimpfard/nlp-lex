## NLP-Lex (NLex)

Hopefully a lexer-generator that doesn't suck


## Syntax

token rules can be defined as such

```
    rule_name :: regular_expression
```

non-captured (inlined) literal matches can be defined as such:

```
    constant_name :- "literal string to match"

#   === or ===

    constant_name :- -"file/to/read/string/from.txt"
```

normalisation rules can be defined as such:

```
  normalised_codepoint <= codepoints_to_normalise
```

  for example
    `a <= bcdef`
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
- [ ] character classes with extensions
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
- [ ] recursive matching

read [regex_flavour](regex_flavour.md) for further details on the specific flavour of regular expressions used in tandem with the generator
