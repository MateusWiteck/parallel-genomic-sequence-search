# Assignment scaling benchmark

Runs per configuration: 10. Raw observations: [assignment-scaling.csv](assignment-scaling.csv).

| Configuration | Program | Threads | Mean seconds | SD seconds | Occurrences |
| --- | --- | ---: | ---: | ---: | ---: |
| size-1x | reference | 1 | 0.000915 | 0.000085 | 7 |
| size-1x | parallel | 4 | 0.001194 | 0.000136 | 7 |
| size-1x | sequential-optimized | 1 | 0.001194 | 0.000138 | 7 |
| size-1000x | reference | 1 | 0.010001 | 0.002193 | 7000 |
| size-1000x | parallel | 4 | 0.010675 | 0.000310 | 7000 |
| size-1000x | sequential-optimized | 1 | 0.010367 | 0.002179 | 7000 |
| size-100000x | reference | 1 | 0.392285 | 0.005216 | 700000 |
| size-100000x | parallel | 4 | 0.438524 | 0.017153 | 700000 |
| size-100000x | sequential-optimized | 1 | 0.385084 | 0.005814 | 700000 |
| size-200000x | reference | 1 | 0.779838 | 0.012332 | 1400000 |
| size-200000x | parallel | 4 | 0.831923 | 0.021144 | 1400000 |
| size-200000x | sequential-optimized | 1 | 0.765526 | 0.009564 | 1400000 |
| threads-200000x | parallel | 1 | 0.879919 | 0.022792 | 1400000 |
| threads-200000x | parallel | 2 | 0.844345 | 0.021393 | 1400000 |
| threads-200000x | parallel | 4 | 0.846829 | 0.028214 | 1400000 |
| threads-200000x | parallel | 8 | 0.839397 | 0.026844 | 1400000 |
