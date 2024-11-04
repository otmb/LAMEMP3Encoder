#!/bin/bash
set -e

LAME_DIST=/lame/dist
LAME_SO=${LAME_DIST}/lib/libmp3lame.so

em++ ${LAME_SO} ./encodeMp3.c \
  -s WASM=1 \
  -O3 \
  -I ${LAME_DIST}/include \
  -s ASSERTIONS=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s EXPORTED_RUNTIME_METHODS='["FS", "cwrap"]' \
  -s FORCE_FILESYSTEM=1 \
  -s ASYNCIFY=1 \
  -o wasm/encodeMp3.js --bind
