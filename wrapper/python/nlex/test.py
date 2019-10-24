from wrap.wrapper import NLexWrappedObject
from pprint import pprint
import json

wo = NLexWrappedObject("../../../src/playground/all.so")
ts = wo.process_json(open('/home/Test/out.json', 'r').read(), clean=False)
print(ts)
