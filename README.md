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

Comments span from the character `#` until the end of line except inside regex rules, wherein there are no comments available (`(?#...)` might be an option later)

```
    # Hi, I am a comment
```

### Options

an option is of the form `option <name> <value>` where \<value\> is either `on` or `off`

currently significant options:

| Option         | effect         | default        |
| :------------- | :------------- | :------------- |
| `ignore_stopwords` | entirely removes stopwords from the output stream       | `off`       |
| `pure_normaliser`   | creates a function `__nlex_pure_normalise` that returns the next normalised character in the stream | `off` |
| `unsafe_normaliser`   | disable the length check on normalised values | `off` |
| `skip_on_error`       | skip unmatchable characters | `off` |
| `log_{verbose,warning,debug}` | set log level (verbose \< warning \< debug) | (unset) |

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
- [X] rule actions
- [ ] backreferences
- [X] recursive matching

read [regex_flavour](regex_flavour.md) for further details on the specific flavour of regular expressions used in tandem with the generator

## Building

To build the compiler the following libraries are required:

+ LLVM (\>= 8)
+ TCL  (soft dependency, will be removed later)
+ Intel TBB
+ OpenMP + pthread

how to build:

```sh
$ git clone https://github.com/alimpfard/nlp-lex
$ cd nlp-lex
$ make
```

## Using the Compiler 

This is still in alpha stages, so a multistep procedure is used to produce binaries and libraries:

```sh
$ src/a.out -o src/playground/ll.ll path/to/nlex
$ cd src/playground # the runtime system is here (will be moved out later)
$ sh run.sh
```

which will generate a binary (`./a.out`) and a shared library (`all.so`) which can be used with the wrappers
