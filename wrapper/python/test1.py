import nlex

@nlex.NLexTokenizer
def tokenize():
    """
    foo :: [124]+
    """


print(tokenize("[]"))
