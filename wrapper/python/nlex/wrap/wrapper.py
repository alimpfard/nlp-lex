import ctypes
from ctypes import cdll
import os.path
from .token import Token

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
        self.__has_normaliser = True
        try:
            self._nlex_pure_normalise = getattr(self.__lib, '__nlex_pure_normalise')
            self._nlex_pure_normalise.restype = ctypes.c_char
        except AttributeError:
            self.__has_normaliser = False

    def __feed(self, string):
        self._fed = ctypes.create_string_buffer(bytes(string, 'utf-8'))
        self._fed_ptr = ctypes.cast(self._fed, ctypes.POINTER(ctypes.c_char))
        self._nlex_feed(self._fed)

    def feed(self, string):
        self.__feed(string)

    def __next_token(self):
        if not self._fed:
            raise Exception("NLexWrappedObject.__next_token called before __feed")

        self._nlex_root(ctypes.pointer(self._m_value))

        if self._m_value.errc != b'\0':
            self._fed = None
            return None

        if self._m_value.length == 0:
            self._fed = None
            return None

        return Token(
            value=(ctypes.c_char * self._m_value.length).from_address(ctypes.addressof(self._m_value.start.contents)),
            length=self._m_value.length,
            tag=self._m_value.tag,
            metadata=self._m_value.metadata,
            offset=self._nlex_distance()-self._m_value.length
        )

    def __next_normalised_char(self):
        if not self.__has_normaliser:
            raise Exception("NLex object not built with `option pure_normaliser on`")
        if not self._fed:
            raise Exception("NLexWrappedObject.__next_normalised_char called before __feed")

        return self._nlex_pure_normalise()

    def next_token(self):
        return self.__next_token()

    def next_normalised_char(self):
        return self.__next_normalised_char()

    def tokens(self):
        while True:
            x = self.__next_token()
            if x is None:
                return
            yield x

    def normalise_all(self):
        x = b''
        buf = b''
        while x != b'\0':
            buf += x
            x = self.__next_normalised_char()
        self._fed = None
        return buf
