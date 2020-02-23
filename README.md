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

Comments span from the character `#` until the end of line except inside regex rules, wherein you can use a comment group `(# foobar)`

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
| `capturing_groups`    | enables group captures and generates the functions `nlex_get_group_{{start,end}_ptr,length}` | `off` |
| `log_{verbose,warning,debug}` | set log level (verbose \< warning \< debug) | (unset) |

### Regular Expressions

the regex engine is currently very limited in what it supports, however here is a road map:

- [X] execution
- [X] standalone execution
- [X] state traversal codegen
- [X] helper functions codegen
- [X] normalisation codegen
- [X] stopword codegen
- [X] normalisation declarations (`normalise {abcd} to c`)
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
- [X] Regex captures (start-end)
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
$ cd nlp-lex/src
$ mkdir build && cd build && cmake ..
$ make
```

## Using the Compiler 

This is still in alpha stages, so a multistep procedure is used to produce binaries and libraries:

```sh
# To create an executable (mostly for test)
$ build/nlex -o output_object.o ../examples/test.nlex # create an object file
$ clang -static -lc output_object.o -o tokenise       # link it as a static executable

# To create a shared library
$ build/nlex -o output_object.o --relocation-model pic ../examples/test.nlex
$ clang -shared -lc output_object.o -o libtokenise.so
```

To target other OSs/architectures/etc, use the appropriate --target-_option_ and --object-format parameters

For instance, to create an object file for x86_64 windows, use

```sh
$ build/nlex \
    --library \
    -o output_object.obj \
    --target-arch x86_64 \
    --target-sys windows \
    --object-format coff \
    ../examples/test.nlex

# to create dll for windows
$ link /dll /def:output_object.def output_object.obj
```

_Note_: Generating executables for windows is currently not supported (RTS issues)


## Compiler Options
<details><summary> Expand for commandline options</summary>

```
-h
    Shows a descriptions of commandline arguments

-g
    Generates a graph for what the lexer is supposed to do

-r
    Dry run (only perform syntax and semantic checks)

-o [file]
    sets the output filename
    if the target is a binary file, it will be placed next to the source if this option is not provided
 
--library
    Builds a pure library (standalone, no libc dependency)

--target[-option] <value>
    if 'option' is not provided, set the target triple (behaves like clang's -target option)
    otherwise, replaces parts of the native target with the provided value

--relocation-model <model>
    Sets the relocation model
    linking the output to a binary will likely require this to be 'pie'

--object-format <format>
    Output a specific kind of object file (default is ELF)

--emit-llvm
    If specified, will output llvm IR instead of an object file

-mcpu <cpu>
    Sets target CPU family (the default is generic)

--features <features>
    Sets target CPU features (no extra features are assumed to exist by default)
```

</details>
