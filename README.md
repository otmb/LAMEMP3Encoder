# LAMEMP3Encoder

WebAssembly MP3 Encoder
To adjust the length of the audio file, we considered the parameters when encoding with CBR (Constant Bit Rate) using the LAME MP3 encoder.
We have confirmed that video files of up to 2 hours can be converted to MP3.
However, I don't think it works well with very large files.

TestPage: https://otmb.github.io/LAMEMP3Encoder/

## Development

### Environment

- M1 Macbook
- Podman

### Instration

```
$ bash build-podman.sh
$ bash build-wasm.sh
```
If you want to use docker, please rewrite podman execution.

## References

- [LAME (Lame Aint an MP3 Encoder)](https://sourceforge.net/projects/lame/files/)
- [Emscripten Docker Image](https://hub.docker.com/r/emscripten/emsdk)
