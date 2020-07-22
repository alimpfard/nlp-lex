from .wrapper import NLexWrappedObject
from textwrap import dedent
import os
import requests
import zipfile
from zipfile import ZipExtFile
import hashlib
import platform

class NLexTokenizerCreationException(Exception):
    pass

_default_options = {
        'Linux': {'output_file': 'tokenizer.so', 'sys': ''},
        'Windows': {'output_file': 'tokenizer.dll', 'sys': 'windows'},
    }.get(platform.system(), None)

if _default_options is None:
    print(f"Running on unsupported system ({platform.system()}), this is likely to blow up")
    _default_options = {'output_file': 'tokenizer', 'sys': ''}

ZipExtFile._update_crc = lambda self, *args: None

class callout:
    @staticmethod
    def run(source, arguments, compiler_server):
        class RunResult:
            def __init__(self, diagnostics, identifier, dot_data):
                self.diagnostics = diagnostics
                self.identifier = identifier
                self.dot_data = dot_data

        res = requests.post(compiler_server + '/run', json={'arguments':arguments, 'source':source}).json()
        return RunResult(diagnostics=res['diagnostics'], identifier=res['identifier'], dot_data=res['dot_data'])

    @staticmethod
    def download_url(url, save_path, chunk_size=128):
        r = requests.get(url, stream=True)
        with open(save_path, 'wb') as fd:
            for chunk in r.iter_content(chunk_size=chunk_size):
                fd.write(chunk)

    @staticmethod
    def compile_and_download(identifier, output_file, compiler_server):
        res = requests.post(compiler_server + '/compile', json={'job_id':identifier}).json()
        if not res['ok']:
            raise NLexTokenizerCreationException("Compile error")
        callout.download_url(compiler_server + '/download?file_id=' + res['file_id'], output_file + '.zip')
        if os.path.exists(output_file): os.remove(output_file)
        with zipfile.ZipFile(output_file + '.zip', 'r') as f:
            try:
                try:
                    f.extract('tokenizer.dll')
                    os.rename('tokenizer.dll', output_file)
                except:
                    f.extract('tokenizer.so')
                    os.rename('tokenizer.so', output_file)
                os.chmod(output_file, 0o755)
                return os.path.realpath(output_file)
            except:
                raise



def NLexTokenizer(*args,
        arch='x64', vendor='', sys=_default_options['sys'], target='',
        object_format='', library='on', cpu='generic', relocation_model='pic',
        features='', output_file=_default_options['output_file'],
        compiler_server='https://nlex.herokuapp.com'):
    if len(args) == 0:
        return lambda fn: NLexTokenizer(fn,
                arch=arch, vendor=vendor, sys=sys, target=target, relocation_model=relocation_model,
                object_format=object_format, library=library, compiler_server=compiler_server,
                cpu=cpu, features=features, output_file=output_file)

    fn = args[0]
    doc = dedent(fn.__doc__)
    if not doc:
        raise NLexTokenizerCreationException(f"Function {fn.__name__} does not have a docstring")

    path = os.path.realpath(output_file)
    hashpath = os.path.join(os.path.dirname(path), f'.{output_file}.hash')
    hsh = hashlib.sha512(bytes(doc, 'utf8')).hexdigest()
    if os.path.exists(hashpath) and os.path.exists(output_file):
        with open(hashpath, 'r') as f:
            if f.read() == hsh:
                return lambda inp: fn(inp, NLexWrappedObject(path).process_documents)

    res = callout.run(doc, {
        'target_vendor': vendor,
        'target_sys': sys,
        'object_format': object_format,
        'library': library,
        'relocation_model': relocation_model,
        'target': target
    }, compiler_server)
    if res.diagnostics:
        raise NLexTokenizerCreationException(f"Error: {res.diagnostics}")

    path = callout.compile_and_download(res.identifier, output_file, compiler_server)

    with open(hashpath, 'w+') as f:
        f.write(hsh)

    return lambda inp: fn(inp, NLexWrappedObject(path).process_documents)

