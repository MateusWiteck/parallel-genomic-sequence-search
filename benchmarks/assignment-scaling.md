# Assignment scaling benchmark

Runs per configuration: 10. Raw observations: [assignment-scaling.csv](assignment-scaling.csv).

| Configuration | Program | Threads | Mean seconds | SD seconds | Occurrences |
| --- | --- | ---: | ---: | ---: | ---: |
| size-1x | reference | 1 | 0.000959 | 0.000087 | 7 |
| size-1x | parallel | 4 | 0.001254 | 0.000154 | 7 |
| size-1x | sequential-optimized | 1 | 0.000897 | 0.000151 | 7 |
| size-1000x | reference | 1 | 0.009962 | 0.001037 | 7000 |
| size-1000x | parallel | 4 | 0.010320 | 0.000677 | 7000 |
| size-1000x | sequential-optimized | 1 | 0.003143 | 0.000197 | 7000 |
| size-100000x | reference | 1 | 0.390360 | 0.005487 | 700000 |
| size-100000x | parallel | 4 | 0.427164 | 0.011285 | 700000 |
| size-100000x | sequential-optimized | 1 | 0.098951 | 0.009216 | 700000 |
| size-200000x | reference | 1 | 0.774163 | 0.009245 | 1400000 |
| size-200000x | parallel | 4 | 0.844698 | 0.026366 | 1400000 |
| size-200000x | sequential-optimized | 1 | 0.179824 | 0.016105 | 1400000 |
| threads-200000x | parallel | 1 | 0.879146 | 0.029965 | 1400000 |
| threads-200000x | parallel | 2 | 0.862125 | 0.030154 | 1400000 |
| threads-200000x | parallel | 4 | 0.835212 | 0.014772 | 1400000 |
| threads-200000x | parallel | 8 | 0.820633 | 0.017828 | 1400000 |
