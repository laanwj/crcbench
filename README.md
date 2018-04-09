ARM (AARCH64):

    ./build_bench.sh arm64

X86_64:

    ./build_bench.sh sse42

Benchmarks are executed on a single core.

Some results:

- AMD FX-8370 Eight-Core Processor (4.0Ghz)
```
[sw] 65543000 bytes in 63679us monotonic 63643us CPU (1029.9MB/s)
[hw-sse42] 65543000 bytes in 21084us monotonic 21072us CPU (3110.4MB/s)
```
- odroid-c2 (1.2Ghz)
```
[sw] 65543000 bytes in 134897us monotonic 134858us CPU (486.0MB/s)
[hw-arm] 65543000 bytes in 22079us monotonic 22079us CPU (2968.6MB/s)
```
- On an old laptop
```
[sw] 65543000 bytes in 229564us monotonic 229254us CPU (285.9MB/s)
[hw-sse42] 65543000 bytes in 41722us monotonic 41647us CPU (1573.8MB/s)
```
- Intel Atom (C72750):
```
[sw] 65543000 bytes in 74659us monotonic 74642us CPU (878.1MB/s)
[hw-sse42] 65543000 bytes in 12714us monotonic 12719us CPU (5153.1MB/s)
```
- Xeon D-1520@2ghz 
```
[sw] 65543000 bytes in 132594us monotonic 132571us CPU (494.4MB/s)
[hw-sse42] 65543000 bytes in 18035us monotonic 18035us CPU (3634.2MB/s)
```
- Xeon e5v3 (Haswell) 
```
[sw] 65543000 bytes in 112121us monotonic 112187us CPU (584.2MB/s)
[hw-sse42] 65543000 bytes in 15292us monotonic 15304us CPU (4282.7MB/s)
```

