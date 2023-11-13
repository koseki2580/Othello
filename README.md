# Othello

[emscripten](https://emscripten.org/docs/)を使用したオセロができる WebAssembly。

```bash
em++ main.cpp lib/othello.cpp lib/evaluation.cpp lib/action/action.cpp lib/action/random_action.cpp lib/action/mini_max_action.cpp lib/action/alpha_beta_action.cpp lib/player.cpp -o ./output/othello.js -s EXPORTED_FUNCTIONS="['_createPlayOthello', '_deletePlayOthello', '_getLegalActions', '_getBoardInfo', '_put', '_cpuPut', '_isDone', '_free']" -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap', 'getValue']" -s WASM=1 -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE='$allocate' -sNO_DISABLE_EXCEPTION_CATCHING
```
