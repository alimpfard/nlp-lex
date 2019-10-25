class Token(object):
    def __init__(self, value, length, tag, metadata, offset):
        self.raw = value[:length]
        self.length = length
        self.tag = tag[:]
        self.metadata = metadata
        self.offset = offset

    def desanitify(self, nlex):
        return {
            'id': nlex.next_id,
            'raw_token': repr(self.raw),
            'token': self.raw.decode('utf-8', 'ignore'),
            'type': self.tag.decode('utf-8', 'ignore'),
            'delete_ref': [], # we don't support this
            'log': 'correct', # not quite sure if it can handle nulls
            'is_stopword': bool(self.metadata&1),
            'POS': '',
            'stem': ''
        }

    def __repr__(self):
        return f'Token(value={repr(self.raw)}, length={self.length}, tag={self.tag}, metadata={self.metadata}, offset={self.offset})'
