ARM:

    g++ crcbench.cc crc32c.cc -O2 -o crcbench -std=c++11 -march=armv8-a+crc -DBENCH_ARM

X86_64:

    g++ crcbench.cc crc32c.cc -O2 -o crcbench -std=c++11 -msse4.2 -DBENCH_SSE42

