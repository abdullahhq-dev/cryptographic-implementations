
## CHACHA20 - Benchmark results

- Compiler: gcc
- Message length: 64 bytes

### -O0
Compilation : gcc -O0 chacha.c test2.c -o benchmark
Output:
Checksum: 98
Iterations            : 1000000
Total Cycles           : 8519843845
Cycles / Iteration    : 8519.84
Cycles / Byte         : 133.1226
Total Time            : 4277188710 ns
Average Time / Iteration     : 4277.19 ns
Average Time / Byte    : 66.83 ns

### -O2
Compilation : gcc -O2 chacha.c test2.c -o benchmark
Output:
Checksum: 98
Iterations            : 1000000
Total Cycles           : 1229106620
Cycles / Iteration    : 1229.11
Cycles / Byte         : 19.2048
Total Time            : 617045244 ns
Average Time / Iteration     : 617.05 ns
Average Time / Byte    : 9.64 ns

### -O3
Compilation : gcc -O3 chacha.c test2.c -o benchmark
Output:
Checksum: 98
Iterations            : 1000000
Total Cycles           : 719098745
Cycles / Iteration    : 719.10
Cycles / Byte         : 11.2359
Total Time            : 361007565 ns
Average Time / Iteration     : 361.01 ns
Average Time / Byte    : 5.64 ns

