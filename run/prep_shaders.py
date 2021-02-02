#!/usr/bin/env python3

from os import listdir
from os.path import join

IN  = 'shaders'
OUT = 'src/shaders.h'

with open(OUT, 'w') as out:
  out.write('#ifndef _shaders_h_\n')
  out.write('#define _shaders_h_\n\n')
  for shader in listdir(IN):
    out.write('static const char* sh_' + shader.replace('.', '_') + '_src = (\n')
    with open(join(IN, shader), 'r') as src:
      for line in src.readlines():
        out.write('"' + line.rstrip().replace('"', '\\\\"') + '\\n"\n')
    out.write(');\n\n');
  out.write('#endif // _shaders_h_')
