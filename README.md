See [this](https://emscripten.org/docs/getting_started/downloads.html) for instructions on how to install emscripten.

Set your env...
```bash
source path/to/emsdk/emsdk_env.sh
```
Build opus with emscripten
```bash
sh build_opus.sh
```
Pull or update submodules
```bash
git submodule foreach git pull origin master
git submodule update --init --recursive
```
Install caddy
```bash
brew install caddy
```
Run caddy
```bash
caddy run #localhost:2015
```

https://stackoverflow.com/questions/40696280/unsafely-treat-insecure-origin-as-secure-flag-is-not-working-on-chrome