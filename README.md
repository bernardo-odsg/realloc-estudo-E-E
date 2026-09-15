# O Bug da Realocação

O que acontece com seu ponteiro quando o `realloc` move de E para E'?

Este repo documenta minha análise de um bug clássico em C:

**Bug:** `processa_dados(int *vetor)` recebe cópia do ponteiro. Se o `realloc` faz out-of-place (sem espaço contíguo), ele libera E e aloca E'. Resultado: main com dangling pointer.

**Consequências:**
- `printf(dados[2])` -> Use-After-Free (Undefined Behavior)
- E' nunca liberado -> Memory Leak
- `free(dados)` -> Double Free

**Fix:** `int **vetor` + `&dados` -> `*vetor = temp`

> Commit 1: Tentativa inicial | Commit 2: Correção com modelo E/E'

Aprendizado: Gerenciamento de memória no HEAP






