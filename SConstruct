#!/usr/bin/env python

EnsureSConsVersion(3, 0, 0)
EnsurePythonVersion(3, 6)

import atexit
import time

time_at_start = time.time()

env = Environment(CPPPATH=['.', './include'], LIBS=['yaml', 'GL', 'openal', 'squirrel', 'sqstdlib', 'SDL2', 'SDL2_image', 'SDL2_ttf', 'SDL2_sound'], CXXCOMSTR="Compiling $TARGET", LINKCOMSTR="Linking $TARGET")

Export("env")

SConscript("src/SCsub")
SConscript("thirdparty/SCsub")
SConscript("modules/SCsub")

program = env.Program('mousey', ['main.cpp'] + env.core_files + env.thirdparty_files + env.modules_files)
env.NoCache(program)
env.Precious(program)

def print_elapsed_time():
    elapsed_time_sec = round(time.time() - time_at_start, 3)
    time_ms = round((elapsed_time_sec % 1) * 1000)
    print("[Time elapsed: {}.{:03}]".format(time.strftime("%H:%M:%S", time.gmtime(elapsed_time_sec)), time_ms))

atexit.register(print_elapsed_time)