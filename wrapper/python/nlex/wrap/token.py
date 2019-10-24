class Token(object):
    def __init__(self, value, length, tag, metadata, offset):
        self.raw = value[:length]
        self.length = length
        self.tag = tag[:]
        self.metadata = metadata
        self.offset = offset

    def __repr__(self):
        return f'Token(value={repr(self.raw)}, length={self.length}, tag={self.tag}, metadata={self.metadata}, offset={self.offset})'
