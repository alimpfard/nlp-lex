# NLex - Python Wrapper

This is a python wrapper for nlex that interfaces its web compiler server to automatically build a tokeniser from tagged functions.

### Installation
`pip install nlex`

### Usage
The import exposes an `NLexTokenizer` annotation that can be used to construct nlex tokenizers from functions in the following fashion:
```py
from nlex import NLexTokenizer

@NlexTokenizer
def tokenize(input_data, process_documents):
    """
    your_source :: goes here
    """
    # Here you can do whatever you want to `input_data` (which is the stuff passed to `tokenize')
    # before passing it to process_documents (read on for the input formats)
    return process_documents(input_data)
```

### Lexer definition language
Please take a look at the main repository.
it should be noted that POS-tagging is not supported on windows (yet).

### `process_documents` format
The second argument to `tokenize` (`process_documents`) takes as input either a list of strings (if `is_json` is False - the default), or a single json string containing a list of strings.
it can also optionally return a json-encoded object for the output if `to_json` is passed as True (this is False by default).
