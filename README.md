# AI-Final-Proj2
My AI final project, Constraint graph coloring using backtracking, forward checking and Minimum Remaining Value (MRV). Matrials used: `sigma.js - v1.2.1` for node representation. `FileSaver.js` to export a file from webpage. The logic is done in `C` language.
# How to use
First, read the steps bellow. You can use `Graph_visualization.html` for viewing graphs. On the web page, options are to open existing graph (from `Map.txt` or what ever file that follow the correct conventions) or to generate new random graph of arbitrary node size and color count. Read the instructions on that page for better understanding of the procedures. Results can be seen in output of `Main.exe` and/or the webpage.\
Also the time required to caculate and solve the problem is computed and shown at end of each outputs from `Main.exe`.

## Prerequisite
`gcc` compiler. You can also compile easily with DevC++ and stuff
## Compile
On Windows, first open CMD, or PowerShell. Then run `gcc .\Main.c -o Main` to produce `Main.exe` file.
## Run
On Windows, simply double click the `Main.exe`. If you also want to see outputs, simply open CMD, or PowerShell and then run `.\Main.exe`.

# Adjacent matrix of graph
If you want to draw a graph manually, open `Map.txt` and follow the structure bellow.\
```
NUM_OF_NODES NUM_OF_COLORS
MATRIX
```
Note that all the inputs are delimited by space.
The input bellow constructs a 10 node graph. 20 colors will be used for coloring problem. Form the second line and on, the adjacent matrix of graph is represented.
```
10 20
0 1 1 0 1 1 1 0 0 0
1 0 1 0 1 1 1 1 1 1
1 1 0 1 1 1 1 1 0 0
0 0 1 0 1 1 1 0 1 0
1 1 1 1 0 0 0 1 1 0
1 1 1 1 0 0 1 1 1 1
1 1 1 1 0 1 0 0 1 0
0 1 1 0 1 1 0 0 0 0
0 1 0 1 1 1 1 0 0 0
0 1 0 0 0 1 0 0 0 0
```
