# Assignment scaling benchmark

Runs per configuration: 10. Raw observations: [assignment-scaling.csv](assignment-scaling.csv).

| Configuration | Program | Threads | Mean seconds | SD seconds | Occurrences |
| --- | --- | ---: | ---: | ---: | ---: |
| size-1x | reference | 1 | 0.001045 | 0.000135 | 7 |
| size-1x | parallel | 4 | 0.001312 | 0.000060 | 7 |
| size-1000x | reference | 1 | 0.009147 | 0.001884 | 7000 |
| size-1000x | parallel | 4 | 0.009682 | 0.000832 | 7000 |
| size-100000x | reference | 1 | 0.387674 | 0.003863 | 700000 |
| size-100000x | parallel | 4 | 0.429252 | 0.013536 | 700000 |
| size-200000x | reference | 1 | 0.765693 | 0.002210 | 1400000 |
| size-200000x | parallel | 4 | 0.858773 | 0.024265 | 1400000 |
| threads-200000x | parallel | 1 | 0.904158 | 0.030414 | 1400000 |
| threads-200000x | parallel | 2 | 0.863836 | 0.022899 | 1400000 |
| threads-200000x | parallel | 4 | 0.859446 | 0.021249 | 1400000 |
| threads-200000x | parallel | 8 | 0.838695 | 0.027387 | 1400000 |
