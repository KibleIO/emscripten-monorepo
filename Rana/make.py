#!/usr/bin/python

from glob import glob
import os
import sys
import re
import json
import shutil
import subprocess
from subprocess import Popen, PIPE, STDOUT

#os.environ['EM_CONFIG'] = '/Users/kord/Documents/source.nosync/emsdk/.emscripten'
#s = open(os.path.expanduser(os.environ['EM_CONFIG']), 'r').read()
#exec(s)

#sys.path.append(EMSCRIPTEN_ROOT)

emcc_args = [
    # '-m32',
    '-O3',
    #   '-D_ERROR_PRINT',
    #   '-DBROADWAY_DEBUG',
    # '-DDEBUG',
    # '-DTRACE',
    # '-Dxxx2yyy'
    '--memory-init-file', '1',
    '--llvm-opts', '3',
    '--llvm-lto', '3',
    '-s', 'NO_EXIT_RUNTIME=1',
    # '-s', 'NO_FILESYSTEM=1',
    # '-s', 'NO_BROWSER=1',
    # '-s', 'CORRECT_SIGNS=1',
    # '-s', 'CORRECT_OVERFLOWS=1',
    '-s', 'TOTAL_MEMORY=' + str(50*1024*1024),
    # '-s', 'FAST_MEMORY=' + str(50*1024*1024),
    # '-s', 'ALLOW_MEMORY_GROWTH=0',
    # '-s', 'INVOKE_RUN=0', #!disable this for main() to work
    # '-s', 'RELOOP=1',
    # '-s', 'INLINING_LIMIT=50',
    # '-s', 'OUTLINING_LIMIT=100',
    '-s', 'DOUBLE_MODE=0',
    # '-s', 'PRECISE_I64_MATH=0',
    # '-s', 'SIMD=1',
    '-s', 'AGGRESSIVE_VARIABLE_ELIMINATION=1',
    '-s', 'ALIASING_FUNCTION_POINTERS=1',
    '-s', 'DISABLE_EXCEPTION_CATCHING=1',
    '-s', 'USE_CLOSURE_COMPILER=1',
    # '-s', 'FORCE_ALIGNED_MEMORY=1', #why doesnt this work?
    # '-s', '''EXPORTED_FUNCTIONS=["_broadwayGetMajorVersion", "_broadwayGetMinorVersion", "_broadwayInit", "_broadwayExit", "_broadwayCreateStream", "_broadwayPlayStream", "_broadwayOnHeadersDecoded", "_broadwayOnPictureDecoded"]''',
    # '--closure', '1',
    '--js-library', 'Broadway-H.264-decoder/Decoder/library.js',
    '--preload-file', 'test2.h264@test2.h264',
    '--preload-file', 'test-2.opus@test-2.opus',
    # '--pre-js', 'Broadway-H.264-decoder/Decoder/download.js',
    # '-s', 'FULL_ES3=1',
    # '-s', 'MODULARIZE=1'
    '-s', 'USE_SDL=2',
    '-s', 'USE_WEBGL2=1',
    '-I', 'Broadway-H.264-decoder/Decoder/src',
    '-I', 'Broadway-H.264-decoder/Decoder/inc',
    '-I', 'Rana_Core_Utils/Utilities',
    '-I', 'Rana_Core_Utils/Utilities/WS',
    '-I', 'Rana_Core_Utils/Hermes',
    '-I', '/Users/kord/Documents/source.nosync/opus',
    '-I', '/Users/kord/Documents/source.nosync/opus/include',
    "-l", "websocket.js",
    "-L", "/Users/kord/Documents/source.nosync/opus/build/.libs",
    "-l", "opus",
    # "-sPROXY_TO_PTHREAD",
    "-s", "USE_PTHREADS",
    "-s", "ASYNCIFY",
]

OBJ_DIR = "obj"
if not os.path.exists(OBJ_DIR):
    os.makedirs(OBJ_DIR)


exclude_dirs_C = set(
    ["arm_neon_asm", "arm_neon_asm_gcc", "arm11_asm", "omxdl"])
exclude_files_C = set(["main.c", "Broadway.c", "h264bsd_sei.c",
                      "TestBenchMultipleInstance.c", "EvaluationTestBench.c", "DecTestBench.c"])
exclude_dirs_CPP = set(["Broadway-H.264-decoder"])
exclude_files_CPP = set()

C_FILES = [f for f in glob("**/*.c", recursive=True) if not any(d in os.path.dirname(f)
                                                                for d in exclude_dirs_C) and not any(ef in os.path.basename(f) for ef in exclude_files_C)]
CPP_FILES = [f for f in glob("**/*.cpp", recursive=True)
             if not any(d in os.path.dirname(f) for d in exclude_dirs_CPP) and not any(ef in os.path.basename(f) for ef in exclude_files_CPP)]


for file in C_FILES:
    target = file.replace('.c', '.o')
    print('emcc %s -> %s' % (file, target))
    subprocess.run(['emcc'] + emcc_args + ['-c', file, '-o', os.path.join('obj', os.path.basename(target))])

for file in CPP_FILES:
    target = file.replace('.cpp', '.o')
    print('emcc %s -> %s' % (file, target))
    subprocess.run(['emcc'] + emcc_args + ['-c', file, '-o', os.path.join('obj', os.path.basename(target))])

object_files_c = [os.path.join('obj', os.path.basename(
    x.replace('.c', '.o'))) for x in C_FILES]
object_files_cpp = [os.path.join('obj', os.path.basename(
    x.replace('.cpp', '.o'))) for x in CPP_FILES]
object_files = [*object_files_c, *object_files_cpp]

print('link -> %s' % 'avc.bc')
subprocess.run(['emcc', '-r'] + object_files + ['-o', 'avc.bc'])

print('emcc %s -> %s' % ('avc.bc', 'avc.js'))
subprocess.run(['emcc', 'avc.bc'] + emcc_args + ['-o', 'avc.js'])
# subprocess.run(['emcc', 'avc.bc', '-o', 'index.html'] + emcc_args)