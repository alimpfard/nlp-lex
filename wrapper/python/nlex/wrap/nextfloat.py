import math
import struct

def next_up(x, i):
    # NaNs and positive infinity map to themselves.
    if math.isnan(x) or (math.isinf(x) and x > 0):
        return x

    # 0.0 and -0.0 both map to the smallest +ve float.
    if x == 0.0:
        x = 0.0

    n = struct.unpack('<q', struct.pack('<d', x))[0]
    if n >= 0:
        n += i
    else:
        n -= i
    return struct.unpack('<d', struct.pack('<q', n))[0]
