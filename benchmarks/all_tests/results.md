# All paired test fixtures

10 runs per program and fixture.

Raw observations: [results.csv](results.csv).

| Fixture pair | Program | Threads | Mean seconds | SD seconds | Reference count | Status |
| --- | --- | ---: | ---: | ---: | ---: | --- |
| arquivo1 / arquivo2-0 | reference | 1 | 0.001080 | 0.000050 | 7 | PASS |
| arquivo1 / arquivo2-0 | parallel | 4 | 0.001555 | 0.000153 | 7 | PASS |
| arquivo1 / arquivo2-0 | sequential-optimized | 1 | 0.001048 | 0.000105 | 7 | PASS |
| arquivo1-grp-t1 / arquivo2-grp-t1 | reference | 1 | 0.001131 | 0.000270 | 1 | PASS |
| arquivo1-grp-t1 / arquivo2-grp-t1 | parallel | 4 | 0.001592 | 0.000176 | 1 | PASS |
| arquivo1-grp-t1 / arquivo2-grp-t1 | sequential-optimized | 1 | 0.001118 | 0.000160 | 1 | PASS |
| arquivo1-t1 / arquivo2-t1 | reference | 1 | 0.001158 | 0.000183 | 15 | PASS |
| arquivo1-t1 / arquivo2-t1 | parallel | 4 | 0.001665 | 0.000226 | 15 | PASS |
| arquivo1-t1 / arquivo2-t1 | sequential-optimized | 1 | 0.001096 | 0.000161 | 15 | PASS |
| arquivo1-t2 / arquivo2-t2 | reference | 1 | 0.001209 | 0.000156 | 23 | PASS |
| arquivo1-t2 / arquivo2-t2 | parallel | 4 | 0.001748 | 0.000165 | 23 | PASS |
| arquivo1-t2 / arquivo2-t2 | sequential-optimized | 1 | 0.001162 | 0.000142 | 23 | PASS |
| arquivo1-t2-extended / arquivo2-t2-extended | reference | 1 | 0.030659 | 0.004313 | 0 | PASS |
| arquivo1-t2-extended / arquivo2-t2-extended | parallel | 4 | 0.026643 | 0.004084 | 0 | PASS |
| arquivo1-t2-extended / arquivo2-t2-extended | sequential-optimized | 1 | 0.008666 | 0.001836 | 0 | PASS |
| arquivo1b / arquivo2b | reference | 1 | 0.001010 | 0.000143 | 3 | PASS |
| arquivo1b / arquivo2b | parallel | 4 | 0.001465 | 0.000188 | 3 | PASS |
| arquivo1b / arquivo2b | sequential-optimized | 1 | 0.000965 | 0.000151 | 3 | PASS |

Count mismatches: 0.
