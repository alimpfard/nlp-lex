import inotify.adapters
import argparse
from subprocess import Popen
import subprocess
import re
import os
import tempfile
import threading

import gi
gi.require_version('Gtk', '3.0')

from gi.repository import Gtk
import xdot

nlex_path = '../src/build/nlex'
mx = re.compile(r' \(line (\d+) offset (\d+) length (\d+)\) ')

class DotWindow(xdot.DotWindow):
    def __init__(self):
        xdot.DotWindow.__init__(self)
        self.dotwidget.connect('clicked', self.on_clicked)

    def on_clicked(self, widget, url, event):
        return True

dot_window = None

def generate_graph(path):
    f,tmpp = tempfile.mkstemp('.dot')
    os.close(f)

    p = Popen([nlex_path, '-r', '--gout', tmpp, path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.communicate()
    xpath = tmpp.replace('.dot', '.xdot')
    p = Popen(['dot', '-Txdot', tmpp, '-o', xpath])
    p.communicate()
    with open(xpath, 'rb') as f:
        return f.read()


def show_dot(xdot):
    dot_window.set_dotcode(xdot)

def read_error(err):
    errnum = int(err[2:6])
    errstr = err[6:]
    erriidx = err.index('-')
    erristr = err[8:erriidx-1]
    errstr = err[erriidx+1:]
    errposre = mx.search(errstr)
    errposline = errposre.group(1)
    errposoff = errposre.group(2)
    errposlen = errposre.group(3)
    errstr = errstr[errposre.end() + 2:]
    return {
            "code": errnum,
            "kind": erristr,
            "position": {
                "line": int(errposline),
                "offset": int(errposoff),
                "length": int(errposlen),
            },
            "message": errstr
    }

def display_errors(err):
    for error in err:
        print('In line %d, offset %d:\n\tError %d -- %s (%s)' % (
            error['position']['line'],
            error['position']['offset'],
            error['code'],
            error['kind'],
            error['message']))

def compile_once(path, options):
    output = {
            'errors': [],
            'warnings': [],
            'infos': []
    }
    p = Popen([nlex_path, *options], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out,err = p.communicate()
    lines = out.split(b'\n')
    for line in lines:
        if line.startswith(b'[E0'):
            # this is an error
            output['errors'].append(read_error(line.decode('utf8')))
        else:
            pass
    return output

def _main(path, nlex_options=[], graph=False):
    i = inotify.adapters.Inotify()
    i.add_watch(path)
    print(f'Watching file {path} for changes')
    print(f'In file {path}:\n')
    for event in i.event_gen(yield_nones=False):
        _, type_names, path, filename = event
        if 'IN_CLOSE_WRITE' not in type_names:
            continue

        print('\x1b[3J\x1b[H\x1b[2J')
        print(f'Watching file {path} for changes')
        print(f'In file {path}:\n')

        stuff = compile_once(path, nlex_options)
        errors = stuff['errors']
        if errors:
            display_errors(errors)
        else:
            print('Everything OK')
            if graph:
                xpath = generate_graph(path)
                show_dot(xpath)



def complement(args, path):
    if '-r' not in args:
        args = ['-r', *args]
    args = [*args, path]
    return args

def rundot():
    dot_window.connect('delete-event', Gtk.main_quit)
    Gtk.main()

parser = argparse.ArgumentParser(description='Watch and compile nlex files')
parser.add_argument('--compiler-path', help='set compiler path', default=nlex_path)
parser.add_argument('-g', action='store_true', help='show a graph for successful compilations')
parser.add_argument('path', metavar='P', type=str, help='input nlex file')
parser.add_argument('ARGS', metavar='AS', nargs='*', help='arguments to pass to nlex compiler')

args = parser.parse_args()
nlex_path = args.compiler_path

if __name__ == '__main__':
    if args.g:
        dot_window = DotWindow()
        threading.Thread(target=rundot).start()
    _main(args.path, complement(args.ARGS, args.path), graph=args.g)
