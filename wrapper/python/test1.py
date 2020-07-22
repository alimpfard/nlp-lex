import nlex

@nlex.NLexTokenizer
def tokenize(inp, process_docs):
    """
    foo :: [124]
    """
    return process_docs(inp)


print(tokenize("124"))
