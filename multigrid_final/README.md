# multigrid

## Usage
- Be in the project root and then:
    - To run the program with vcycles use:   
            ./mgsolve [levels] [iterations]

    - To run the bonus task fmg use:  
        ./mgsolve [levels] [iterations] --fmg
        
    - eg: ./mgsolve 5 10 --fmg

## Graphs and visualization
- The 3d plot for "u" is created using the provided gnu plot script on studon(solutionplt.sec). This is for levels = 5 and iterations =10. 
- Other required plots are under root/vizualization
- The rest of the graphs are created using a custom python script called graphs.py available under root/vizualization. 
- It needs the result.dat files generated from the shell script run_iter.sh which runs the ./mgsolve binary for levels 3....13, for 10 iterations. 
- I have run it separetly for multigrid solver with and without fmg and then saved the result files as result.dat and results_fmg.dat respectively, which is uded by the pyhton script to generate the graphs.