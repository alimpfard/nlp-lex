import ctypes
from ctypes import cdll
import os.path
from .token import Token
import json
from datetime import datetime
from .nextfloat import next_up
import sys

class NLexWrappedObject(object):
    class ValueStruct(ctypes.Structure):
        _fields_ = [
            ("start", ctypes.POINTER(ctypes.c_char)),
            ("length", ctypes.c_int),
            ("tag", ctypes.c_char_p),
            ("errc", ctypes.c_char),
            ("metadata", ctypes.c_ubyte),
        ]
        def __repr__(self):
            return f"NLexWrappedObject.ValueStruct(start={self.start}, length={self.length}, tag={self.tag}, errc={self.errc}, metadata={self.metadata})"

    def __init__(self, path=os.path.join(os.path.dirname(os.path.abspath(__file__)), 'libtokenise.so')):
        self.__lib = cdll.LoadLibrary(path)
        self._fed = None
        self._m_value = NLexWrappedObject.ValueStruct()
        self._nlex_feed = getattr(self.__lib, '__nlex_feed')
        self._nlex_feed.argtypes = (ctypes.c_char_p,)
        self._nlex_root = getattr(self.__lib, '__nlex_root')
        self._nlex_root.argtypes = (ctypes.POINTER(NLexWrappedObject.ValueStruct),)
        self._nlex_distance = getattr(self.__lib, '__nlex_distance')
        self.__nlex_skip = getattr(self.__lib, '__nlex_skip')
        self.__has_normaliser = True
        self.__last_offset = -1
        self.total = 0
        try:
            self._nlex_pure_normalise = getattr(self.__lib, '__nlex_pure_normalise')
            self._nlex_pure_normalise.restype = ctypes.c_char
        except AttributeError:
            self.__has_normaliser = False

    def __feed(self, string):
        self._fed = ctypes.create_string_buffer(bytes(string, 'utf-8') if isinstance(string, str) else string)
        self._fed_ptr = ctypes.cast(self._fed, ctypes.POINTER(ctypes.c_char))
        self._nlex_feed(self._fed)

    def feed(self, string):
        self.__feed(string)
        self.fedlen = len(string.encode('utf-8'))

    def __next_token(self, cleanup):
        if not self._fed:
            raise Exception("NLexWrappedObject.__next_token called before __feed")

        self._nlex_root(ctypes.pointer(self._m_value))
        offset = self._nlex_distance()-self._m_value.length
        if cleanup:
            if self._m_value.errc != b'\0':
                self._fed = None
                return None

        if offset == self.__last_offset or offset >= self.fedlen:
            if offset >= self.fedlen:
                self._fed = None
                return None
            print('early terminate at', offset, 'out of', self.fedlen, 'on', self._fed[offset:offset+4].decode('utf8', 'ignore'), file=sys.stderr)
            self.__nlex_skip()

        self.__last_offset = offset
        print('offset', offset, 'out of', self.fedlen, file=sys.stderr)
        if self._m_value.length == 0:
            return self.__next_token(cleanup)
        return Token(
            value=(ctypes.c_char * self._m_value.length).from_address(ctypes.addressof(self._m_value.start.contents)),
            length=self._m_value.length,
            tag=self._m_value.tag,
            metadata=self._m_value.metadata,
            offset=offset
        )

    def __next_normalised_char(self):
        if not self.__has_normaliser:
            raise Exception("NLex object not built with `option pure_normaliser on`")
        if not self._fed:
            raise Exception("NLexWrappedObject.__next_normalised_char called before __feed")

        return self._nlex_pure_normalise()

    def next_token(self, clean=True):
        return self.__next_token(clean)

    def next_normalised_char(self):
        return self.__next_normalised_char()

    def tokens(self, clean=True):
        i = 0
        while True:
            # if i > 1000:
            #     return
            x = self.__next_token(clean)
            if x is None:
                return
            yield x
            i += 1

    def normalise_all(self):
        x = b''
        buf = b''
        while x != b'\0':
            buf += x
            x = self.__next_normalised_char()
        self._fed = None
        return buf

    def process_json(self, jstr, filename='-', clean=True):
        s = json.loads(jstr)
        docs = []
        res = {
            'description': 'NLex',
            'filename': filename,
            'rundate': datetime.now().strftime('%D %T'),
            'documents': docs,
        }
        for di,req in enumerate(s):
            self.next_id = 0
            self.feed(req['text'])
            docs.append({
                'id': di,
                'sentences': [{
                    'id': 0, # we have no idea :shrug:
                    'tokens': list(x.desanitify(self) for x in self.tokens(clean))
                }]
            })
        return json.dumps(res)

    def next_id():
        doc = "next token id (not unique per token)"

        def fget(self):
            x = self._next_id
            self._next_id += 1 # next_up(self._next_id, 10)
            self.total += 1
            return x

        def fset(self, value):
            self._next_id = value

        def fdel(self):
            del self._next_id

        return locals()
    next_id = property(**next_id())
