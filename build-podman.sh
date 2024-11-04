#!/bin/sh

set -ev

NAME=emscripten-lame

podman build . \
    -t otmb/$NAME \
    -t otmb/$NAME:`date +%Y-%m-%d` \
    -f ./Dockerfile \
