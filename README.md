# capitalism

In this simple model of a capitalistic society, we simulate an ensemble of individuals, each starting with the same amount of money.
At each timestep, the individuals interact with each other by forming pairs and exchanging a random amount of money
(without allowing for anybody to go in the negative!).

Hypothesis: after a sufficiently long equilibration time, >80% of the money is held by <20% of the individuals;
what's more, the individuals that are holding the money are frequently changing between interactions.
This seems to be a consequence of not allowing debts: if a poor (~0) exchanges with a rich (>>1), the rich is forced to give!

Useful xmgrace commands:
`xmgrace -block testrun/capital_distribution.dat -bxy 1:2 -bxy 1:3 -bxy 1:4 -bxy 1:5 -bxy 1:6`
`xmgrace testrun/top_10_file.dat testrun/top_20_file.dat`
`xmgrace -block testrun/top_10_file.dat -bxy 1:3 -bxy 1:4 -bxy 1:5 -bxy 1:6 -bxy 1:7`
`xmgrace -block testrun/top_20_file.dat -bxy 1:3 -bxy 1:4 -bxy 1:5 -bxy 1:6 -bxy 1:7`
