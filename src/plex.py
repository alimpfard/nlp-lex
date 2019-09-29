import re

tokens = (
    # keywords
    'OPTION', 'STOPWORD',
    # operators
    'OPCONST', 'OPDEFINE', 'OPNORM',
    # string
    'LITSTRING', 'FILESTRING',
    # name
    'NAME', 'BOOL'
)

def t_newline(t):
    r'\n+'
    t.lexer.lineno += t.value.count('\n')

def t_OPTION(t):
    r'option'
    return t

def t_STOPWORD(t):
    r'stopword'
    return t

def t_OPCONST(t):
    r':-'
    return t

def t_OPDEFINE(t):
    r'\s*::\s*.*$'
    t.value = t.value.strip()[2:].strip()
    return t

def t_OPNORM(t):
    r'.\s*<=\s*.*\s*$'
    t.value = [t.value[0], t.value[1:].strip()[2:].strip()]
    return t

def t_LITSTRING(t):
    r'"[^"]*"'
    t.value = [True, eval(t.value)]
    return t

def t_FILESTRING(t):
    r'-"[^"|\\"]*"'
    t.value = [False, eval(t.value[1:])]
    return t

def t_comment(t):
    r'\#.*$'
    return None

def t_BOOL(t):
    r'on|off'
    return t

def t_NAME(t):
    r'\w+'
    return t

def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)

t_ignore = '\t '

import ply.lex as lex
lex.lex()

# precedence = ()

values = {}
normalisations = {}
options = {
    'lemmatise': False,
    'stem': False,
}
stopwords = set()

def p_statement_empty(p):
    'statement : '
    p[0] = None

def p_statement_define_constant(p):
    'statement : NAME OPCONST string'
    if p[1] in values:
        compile_error_fmt(
            Errors.ALREADY_DEFINED,
            values[p[1]].lineinfo,
            p[1],
        )
        p[0] = None
    else:
        value = Value(
            value=p[3].value, type=Types.Constant,
            lineinfo=LineInfo(p.lineno(1), p.lexpos(1), len(p[1]))
        )
        values[p[1]] = value
        p[0] = True

def p_statement_define_rule(p):
    'statement : NAME OPDEFINE'
    if p[1] in values:
        compile_error_fmt(
            Errors.ALREADY_DEFINED,
            values[p[1]].lineinfo,
            p[1]
        )
        p[0] = None
    else:
        value = Value(
            value=p[2], type=Types.Rule,
            lineinfo=LineInfo(p.lineno(1), p.lexpos(1), len(p[1]))
        )
        value.resolve_names(values)
        values[p[1]] = value
        p[0] = True

def p_statement_option(p):
    r'statement : OPTION NAME BOOL'
    if p[2] not in options:
        compile_error_fmt(
            Errors.UNKNOWN_OPTION,
            LineInfo.Unknown,
            p[2]
        )
        p[0] = None
    else:
        options[p[2]] = p[3] == "on"
        p[0] = None

def p_statement_stopword(p):
    r'statement : STOPWORD strlist'
    stopwords.update(p[2])
    p[0] = None

def p_expression_strlist(p):
    r'''strlist : string ',' strlist
                | string strlist
                | '''
    if len(p) == 1:
        p[0] = set()
        return

    ps = p[:]
    if len(p) == 4:
        ps[2] = ps[3]

    if ps[1].kind == 'File':
        try:
            with open(ps[1].value) as f:
                ps[2].update(map(lambda x: x.strip(), f.readlines()))
        except Exception as e:
            compile_error_fmt(
                Errors.READ_ERROR,
                LineInfo(p.lineno(1), p.lexpos(1), 3+len(p[1].value)),
                p[1].value
            )
    else:
        # literal string
        ps[2].add(ps[1].value)

    p[0] = ps[2]

def p_statement_normalise(p):
    r'statement : OPNORM'
    norm = p[1][0]
    for char in p[1][1]:
        if normalisations.get(char, norm) != norm:
            compile_error_fmt(
                Errors.NORMALISATION_DOUBLY_DEFINED,
                LineInfo(p.lineno(1), p.lexpos(1), 1),
                char, normalisations[char]
            )
        else:
            normalisations[char] = norm
    p[0] = True

class String:
    def __init__(self, kind, value):
        self.kind = kind
        self.value = value

    def __str__(self):
        return f"String({self.kind}, %r)" % (self.value,)
    __repr__ = __str__

class Value:
    def __init__(self, value, type, lineinfo):
        self.value = value
        self.type = type
        self.lineinfo = lineinfo

    def __str__(self):
        return f"Value({self.value}, {self.type}, {self.lineinfo})"
    __repr__ = __str__

    def resolve_names(self, values):
        def match(x):
            name = x.group(1)
            if name not in values:
                compile_error_fmt(
                    Errors.UNDEFINED,
                    LineInfo(self.lineinfo.line, self.lineinfo.end, self.lineinfo.length),
                    x
                )
                return ''
            else:
                return values[name].value

        self.value = re.sub(r'\{\{(\w+)\}\}', match, self.value)

class Types:
    Rule = 0
    Constant = 1

class Errors:
    _number = 0
    def __init__(self, string, format):
        self.string = string
        self.format = format
        self.number = Errors._number
        Errors._number += 1

Errors.ALREADY_DEFINED = Errors("Already Defined", "`%s' already defined")
Errors.UNDEFINED = Errors("Undefined", "Value `%s' has not been previously defined")
Errors.READ_ERROR = Errors("Read Error", "Reading file `%s' failed")
Errors.NORMALISATION_DOUBLY_DEFINED = Errors("Normalisation previously defined", "Normalisation '%s' already defined as '%s'")
Errors.UNKNOWN_OPTION = Errors("Unknown Option", "Option `%s' makes no sense to me")

class LineInfo:
    def __init__(self, line, end, length):
        self.line = line
        self.end = end
        self.length = length

    def __str__(self):
        return f"LineInfo(line={self.line}, end={self.end}, length={self.length})"
    __repr__ = __str__

LineInfo.Unknown = LineInfo(0, 0, 0)

def compile_error_fmt(err, lineinfo, *args):
    print(("[E%d] %s (line %d, index %d) - " + err.format) % ((err.number, err.string, lineinfo.line, lineinfo.end)+args))

def p_expression_string(p):
    '''string : LITSTRING
              | FILESTRING'''
    if p[1][0]:
        p[0] = String('Literal', p[1][1])
    else:
        p[0] = String('File', p[1][1])


import ply.yacc as yacc

def make_parser():
    return yacc.yacc()

def get_parsed_values():
    return {
        'values': values,
        'normalisations': normalisations,
        'options': options,
        'stopwords': stopwords
    }
