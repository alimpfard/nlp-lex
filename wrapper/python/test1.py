import nlex
import json
import sys

@nlex.NLexTokenizer
def tokenize(inp, process_docs, outfile='output.json'):
    r"""
    # Emit a pure_normalise function that simply returns a normalised character
    option pure_normaliser on
    #
    # Allow width-lengthening normalisations
    option unsafe_normaliser on
    #
    # only log errors
    option log_error on
    #
    #
    #
    PersianWord       :: [آائبپتثجچحخدذرزژسشصضطظعغفقکكلمنوهی\u200c]+
    EnglishWord       :: [a-zA-Z]+
    Number            :: [0123456789۰۱۲۳۴۵۶۷۸۹]+
    Punc              :: [,:;'()*%~:؛«»/\u002d#$%^&_<>{}[\]="^@~`\\\|\+]+
    SentenceDelimiter :: [.؟!?]
    Space             :: [ 	\u000a]+
    Mention           :: @[A-Za-z][A-Za-z_0-9]{4,}
    #
    # \uc{RGI} is the set of all RGI emojis
    emoji   -- "\uc{RGI}"
    #
    maybe_specifier :- ([a-z]+://)?
    uri :- ([a-zA-Z0-9_]|-)+(\.([a-zA-Z0-9_]|-)+)+
    maybe_port :- (:\d+)?
    #
    Link :: {{maybe_specifier}}{{uri}}{{maybe_port}}([a-zA-Z_0-9@:%\+.~\?&/=]|-)+
    #
    # omit spaces from token output
    ignore [Space]
    #
    #
    stopword "i" "me" "my" "myself" "we" "our" "ours" "ourselves" "you" "your"
    stopword "yours" "yourself" "yourselves" "he" "him" "his" "himself" "she" "her"
    stopword "hers" "herself" "it" "its" "itself" "they" "them" "their" "theirs"
    stopword "themselves" "what" "which" "who" "whom" "this" "that" "these" "those"
    stopword "am" "is" "are" "was" "were" "be" "been" "being" "have" "has" "had"
    stopword "having" "do" "does" "did" "doing" "a" "an" "the" "and" "but" "if" "or"
    stopword "because" "as" "until" "while" "of" "at" "by" "for" "with" "about"
    stopword "against" "between" "into" "through" "during" "before" "after" "above"
    stopword "below" "to" "from" "up" "down" "in" "out" "on" "off" "over" "under"
    stopword "again" "further" "then" "once" "here" "there" "when" "where" "why"
    stopword "how" "all" "any" "both" "each" "few" "more" "most" "other" "some"
    """
    def read(x):
        with open(x, 'r+', encoding='utf-8') as f:
            return f.read()
    return process_docs(list(read(x) for x in inp), to_json=outfile, clean=False)


if len(sys.argv) > 1:
    print(tokenize(sys.argv[1:]))
else:
    print(tokenize(input('read files> ')))
