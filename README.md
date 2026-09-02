# cpp-json-mini

A header-only micro JSON tokenizer written in C++11. Parses objects, strings, and numbers into a stream of tokens.

## Limitations
- Very basic, intended as a toy project/learning tool.
- Doesn't support boolean `true`/`false` or `null` yet.

## Usage
Include `json_mini.hpp` and build `main.cpp`:
```bash
g++ -std=c++11 main.cpp -o json_parse
./json_parse
```
# Fresh Comment : Wed Sep  2 05:43:47 AM UTC 2026
