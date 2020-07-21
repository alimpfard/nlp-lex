from .wrapper import NLexWrappedObject
import os
import requests
import zipfile
from zipfile import ZipExtFile

class NLexTokenizerCreationException(Exception):
    pass

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
        arch='x64', vendor='', sys='', target='',
        object_format='', library='on', cpu='generic', relocation_model='pic',
        features='', output_file='tokenizer', compiler_server='https://nlex.herokuapp.com'):
    if len(args) == 0:
        return lambda fn: NLexTokenizer(fn,
                arch=arch, vendor=vendor, sys=sys, target=target, relocation_model=relocation_model,
                object_format=object_format, library=library, compiler_server=compiler_server,
                cpu=cpu, features=features, output_file=output_file)

    fn = args[0]
    doc = fn.__doc__
    if not doc:
        raise NLexTokenizerCreationException(f"Function {fn.__name__} does not have a docstring")

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

    return NLexWrappedObject(path).process_json

