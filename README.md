## NLP-Lex (NLex)

Hopefully a lexer-generator that doesn't suck


## Syntax

token rules can be defined as such

```
    rule_name :: regular_expression
```

non-captured (inlined) matches can be defined as such:

```
    constant_name :- "literal string to match"

#   === or ===

    constant_name :- -"file/to/read/string/from.txt"
```

the regex engine is currently very limited in what it supports, however here is a road map:

- [X] execution
- [ ] standalone execution
- [X] state traversal codegen
- [X] helper functions codegen
- [X] normalisation codegen
- [ ] stopword codegen
- [X] normalisation declarations (`c <= abcd`)
- [X] stopword declarations (`stopwords "stop" "word"`)
- [X] normal literal character matching
- [X] sequence matching (/ab/ -> match a, match b)
- [X] rule substitution (`a :: {{b}}x` -> a = match b, then match 'x')
- [X] semi-optimised alternative matching
- [X] basic character classes
- [ ] character classes with extensions
- [ ] optimised alternative matching
- [ ] alternatives with priorities
- [X] simple quantifiers (\+, \*)
- [X] medium-simple quantifiers ({x,y})
- [X] not-simple quantifiers (?)
- [X] insane quantifiers (??)
- [ ] backreferences
- [ ] recursive matching
