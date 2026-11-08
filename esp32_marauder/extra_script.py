import os
from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

proj = env.get('PROJECT_DIR')
libdir = os.path.join(proj, '.pio', 'libdeps', env.get('PIOENV'), 'TFT_eSPI')
root = os.path.abspath(os.path.join(proj, '..'))
sel_src = os.path.join(root, 'User_Setup_Select.h')
setup_src = os.path.join(root, 'User_Setup_maramau_freenove_s3_28.h')

if os.path.isdir(libdir):
    if os.path.isfile(sel_src):
        env.Execute('cp "%s" "%s"' % (sel_src, os.path.join(libdir, 'User_Setup_Select.h')))
    if os.path.isfile(setup_src):
        env.Execute('cp "%s" "%s"' % (setup_src, os.path.join(libdir, 'User_Setup_maramau_freenove_s3_28.h')))
