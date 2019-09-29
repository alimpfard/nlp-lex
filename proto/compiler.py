from plex import make_parser, get_parsed_values

parser = None
def getparser():
    global parser
    if parser is not None:
        return parser
    parser = make_parser()
    return parser

def parse_file(path):
    with open(path) as f:
        for line in f:
            getparser().parse(line)
        return get_parsed_values()
