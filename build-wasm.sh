#!/bin/bash
set -ev
podman run --rm -v $(pwd):/src otmb/emscripten-lame bash ./build.sh
