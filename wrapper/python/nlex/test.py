from wrap.wrapper import NLexWrappedObject
from pprint import pprint
import json
import time
import sys

wo = NLexWrappedObject("../../../src/playground/all.so")
ns = time.time_ns()
ts = wo.process_json(open('input.json', 'r').read(), clean=False)
ns2 = time.time_ns()
print(ts)
dt = ns2 - ns
print("Read", wo.total, "tokens in",
      dt/1000_000, 'ms', 'which is',
      int(wo.total/dt*1000_000_000), 
      'tokens/s',
      file=sys.stderr)