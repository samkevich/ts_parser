# ts_parser
ts_parser is a small example how to parse MPEG Transport Streams and extract video and audio from it.  

## How to compile
There is [Makefile](./Makefile) for compilation.
Just execute ` make ` in the root of the project to compile it.
There is a folder `bin` after compilation with executable `ts_parser`.

## How to use
It will be two separate files `video.mpv` and `audio.mpa` in current directory after executing
` ts_parser input.ts `

## Design

Parser consists of several extendable parts:

- [TSParser](./include/parser.hpp): Parser of transport stream headers. It is possible to extend parser with new payload parsers through `void add_payload_parser(...);` function.
- [PATParser](./include/pat_parser.hpp): Parser of PAT packets. Extracts PMT info from PAT packets and returns it through `PATCallback`.
- [PMTParser](./include/pmt_parser.hpp): Parser of PMT packets. Extracts PES info from PMT packets and returns it through `PMTCallback`.
- [PESParser](./include/pes_parser.hpp): Parser of PES packets. Extracts payload from elementary streams and returns it through `PESCallback`.

All these parsers use MPEG-TS format description presented in [format](./include/format.hpp).
There is helper library [reader](./include/reader.hpp) to read and parse binary data.
[Buffer](./include/buffer.hpp) is used for buffered reading from file.
 
