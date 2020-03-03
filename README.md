# OpenMPBestFit

1.    Introduction
The goal for this assessment is to find the best linear fit Y = mX+c, using two OpenMP parallelisation strategies; Data Parallelism and Task Parallelism. The two parallelisation strategies will be compared to see the most efficient strategy for the problem. 
 
The pseudo-code in figure 2 was developed from the problem sheet, it shows how X, Y M and C was initialized. I chose the easiest way to develop the serial code. Which is by first deriving the values of X and Y and then getting the sums, squares and multiplications need for the M and C formulas. The values computed by the serial code for the line of best fit are M = -0.000013  and C = 0.951413.
 
Moving on to the rest of the report, we will discuss the strategies for the parallelisation of the pseudocode in figure 2.
 
 
2.    Data Parallelism 
 
The Data-Parallelism was the simplest part of the assessment. For it’s design, two parallel zones were implemented. Making it Fine Grained, as can be seen in figure 2. Fine-Grained granularity was chosen because despite the overhead that could gain from a fine-grained implementation the performance gain would not be significant due to there being only two parallelized regions, furthermore it prevents potential data dependency issues, due to the fact that the next computations need the values of X and Y for its calculations . The important parts of the Data-Parallelism implementation and the strategy behind its implementation will be discussed below.
 
The first parallel zone was used to initialise the values of X and Y. The second was used to compute the sums of X, Y, the square of X and the product of X and Y. The directives chosen for this is the #pragma omp parallel for. This was done because the only parallelisation needed in this problem is the work-sharing of the computation in the for loops.
 
The values of X and Y are set to share data between the set number of threads through the use of the shared Clause, with the scope being the entire parallel region. The shared clause was used so the threads can write the new initialized value into the empty vector element. There is no potential Data Race situation even with the use while sharing the data. This is because each thread can initialize its chunk of the vector, this is done without the threads interfering with each other. 
 
The most important part of the implementation of the Data-Parallelism strategy is the use of the reduction clause. The reduction clause is used on the second fork, this was done to get the summation on the values needed for the computation of M and C. The reduction operator in the context of the second parallel region gathers the thread private values of the summations and then aggregates them into one sum. This is what we used in our computation of M and C. The final result for M and C, parallelized using a data parallelism strategy are  -0.000013  and 0.951413 respectively. 
 
 
 
Error! Filename not specified.
Figure 1.  Fork-Join Model for Data Parallelism
 
 
 
3.    Task Parallelism
 
The Task-Parallelism was the harder part of this assessment. For its design, two parallel zones were implemented. The task parallelisation strategy was implemented to be of fine-grained granularity. The reason for this is the same as that of the Data-Parallelism section, the gain in performance is insignificant.  The serial part of the program to be parallelised for the task parallelism section is the same as that of the data parallelism section.  The important parts of the Task-Parallelism implementation and the reasons for their implementation will be discussed below.
 
The first parallel region was used to initialize the variables X and Y that will be used in future computations of M and C. The parallel region is forked using the #pragma omp parallel directive. Furthermore, the #pragma omp single is used right before the for loop, to allow only one thread into its scope. A similar directive that could have been used here is the #pragma omp master directive. However, this was not chosen because; it would give the master thread even more work than it already has, it would remove the implicit barrier at the end of the parallel region. These reasons could cause complications to the program and would have additional performance costs to fix, which the program cannot afford. The #pragma omp single directive prevents threads from running the same block of code over and over again and it allows one thread to schedule the tasks without running the code in the task. The tasks will then be picked up by idle threads waiting at the implicit barrier, which is the end of the parallel region. The threads pick up whatever tasks are on the queue and run the code contained in the task until the queue is empty. 
 
The #pragma omp task directive is used in the implementation to specify the tasks that will be put in the task queue. This part of the code is dangerous. There is a lot of potential for data racing. I solved this by adding a #pragma omp atomic directive for each operation. The atomic directive puts a lock on a single operation, only one thread can read and write its data at a time. This is unlike the critical directive which does something similar, however, it puts a hard lock on a region of code that can’t be accessed without a key. The atomic directive was chosen because it has significantly lower overhead to the critical directive. The summation of the values needed for the computation of M and C is done within the scope of the atomic directives. The final result for M and C, parallelized using a Task parallelism strategy are  -0.000013  and 0.951413 respectively.
 
There are better ways to implement tasks in more recent versions of OpenMP. In version 5.0 the Reduction clause is added to openMP. This means that most of the work will be performed for the programmer without much worry about problems like data race. A feature that would also help increase the performance of the task parallelism problem is the Task loop directive. This just like the parallel for directive combines two directives into one to promote simplicity and reduce overhead.
 
4.    Running the Code
 
To run the code in this assessment, the module must be set using the shell command “module load intel”. Thereafter, the code should be compiled using “icc -qopenmp data-bestfit.c -lm -O0 -o output.exe”. Use “-qopenmp-stubs” instead to run the code serially with openmp functions like the openMP timer. The “lm” flag used to import the math library for the computations needed in the code. To run the code on Chadwick, use “qrsh -l cputype=sandybridge,h_rt=00:05:00,exclusive \-cwd -V -pe smp 16 ./scaling-binding.sh ./ask.exe 10 1”. This runs the code on Chadwick interactively on 10 threads.
 
The script scaling-binding.sh used above is the same as the one in the openMP lab 03. However, it will be submitted with the codes for simplicity. The script contains two very important pieces of shellcode. The codes are environmental variables for the program. They are “EXPORT_PROC_BIND = true” which binds a thread to a core to prevent thread migrating from one core to another. This is good because it promotes first touch, which will help with more accurate data consistently. The other environment variable is “OMP_SET_DYNAMIC = false”, which makes the run time give you the exact number of threads that have been requested.
 
5.    Discussion of Performance
 
For this section, we will discuss the data collected from running the steps in the previous section. The data used for this discussion is located in the appendix at the end of this document. The results of the Data Parallelisation strategy will be discussed first.  After which, the Task Parallelisation strategy will be compared to the aforementioned.
 
The results from the data-parallel code was quite good. The data was collected over 5 thread numbers. The Speedup ratio was a constant 1 to 1 with the number of processes. For example; on five threads the speedup is 4.9. This, if rounded up is ideal speed up (S(p) = P). The speed-up keeps this ratio for all values of threads and speedup throughout the collected data. The efficiency of the parallel code is bordering around a hundred percent. The accuracy of the results is also correct, these results are to be expected. The data parallelism strategy is built for problems like our problem question. Moving on to the task parallelism strategy, there is a stark difference in the result.
 
The task-parallel code while producing accurate numbers has abysmal timings. The efficiency, speedup and timings for the task-parallel code are far below the serial code and even worse when compared to the task-parallel code. The suspected reasons why the task parallel code fails to meet the expectations of the serial codes time and pales in comparison to the data-parallel code will be discussed below. 
 
Task parallelism doesn’t have efficient tools to compete with Data parallelisms work sharing. The openMP version used for this assessment is assumed to be version 4.0. This is difficult to confirm. The lecture notes for the module says it is 4.5, however, Taskloops which is a feature of 4.5 was unable to be implemented on Chadwick. Solving a for loop computation requires many clever placements of clauses and directives for the task-parallel code to produce accurate results. Version 5.0 of openmp makes it easier with the availability of taskloops and the introduction of task reductions clauses, without these a significant overhead would have to be paid. Which brings us to the next point.
 
There is a significant amount of overhead in the parallel code, this is due to my inability to use the current tools tasks have, for parallelisation efficiently. This overhead can be seen when comparing the serial code to the task-parallel code. The efficiency on one thread is almost 50 percent lower than that of serial code. This Significant drop in efficiency shows the amount of overhead of generating 50 million tasks for each iteration of the value of N. There are also 200 million(50 million  x 4) atomic directives created, while atomic have far less overhead than Critical regions. 200 Million atomic operations would harm the code’s performance. The efficiency continues to deteriorate as the number of threads increase. This brings us to our final point.
 
False sharing is a situation whereby multiple threads try to read or write the same cache line of data at the same time. This typically happens with for loops and arrays. One of the symptoms of false sharing Is horrible performance and worse performance as more threads are added. This symptom shows itself perfectly in our code as the data shows. The threads are trying to complete the summation operation as instructed in their given task. There are four summation operations each completing 50 million computations over just about the same amount of memory locations. It can be assumed that with such a large degree of computation the possibility of false sharing over the cache lines is highly probable. 
 
Is there a way to fix any of these problems? In terms of using 5.0 features to improve performance, one simply has to upgrade openMP. However, as openMP 5.0 has just recently being released, the support for it would be minimal. In terms of the overhead, one could try; structuring the serial code to be more compatible with Task-parallelism. Task – parallelism works well with unconventional algorithms, like while loops and recursion. In terms of false sharing, like the suggestions in the lecture notes. one could try padding,  using different strides through the data of declaring the summation variables as private. The last solution was tried for the assessment to no avail. Possibility of failure due to the incorrect implementation. 
 
 
 
6.    Conclusion
 
In conclusion for most simple algorithms, it would be most efficient to employ a simple data-parallel strategy. A data-parallel strategy maximizes efficiency with simple algorithms and problems. While for more complicated and unconventional algorithms task-parallelisation strategies might be more efficient. The strategy employed must depend on the problem. However, the most efficient solution might be implementing both data and task in a mixed parallelisation strategy. 
	




	Appendix

N = 50,000,000
X = N
Y = N
 
For each i in x 
x[i] = i/1003.01
y[i] = setY(x[i])

for i in N
sumx += x[i]
sumy += y[i]
sumxSq += x*x
sumxy += x[i]*y[i]

calculate m
m = (N * sumxy) – (sumx * sumy)) / (N * sumxsq) – (sumx * sumx)

calculate c
c = (sumy – (m * sumx)) / N

setY(x)
return 1.0/exp(x/50000.1) + sin(x/501.1) / (log(x+1.0E-06)+cos(x/499.01+x))
					
Figure 2. Pseudo code for Serial Program










Threads	Serial Code	Data Parallelism	Task Parallelism
1	10577.26	10550.71254	18329.728127
2	-	5295.375824	66583.334923
3	-	3533.991814	109973.091125
5	-	2126.138926	184701.005936
10	-	1065.490007	244890.123844

Table 1 – Time(milliseconds) by No of threads





Threads	Data Parallelism S(p)	Task Parallelism S(p)
1	1.002	0.577
2	1.997	0.158
3	2.993	0.096
5	4.974	0.057
10	9.927	0.043
	
	





Table 2 – Speed Up (t1/(t(p)))





Threads	Data Parallelism E(p)	Task Parallelism E(p)
1	100%	57%
2	99%	7.9%
3	99%	3.2%
5	99%	1.14%
10	99%	0.43%

Table 3 – Efficiency ((S(p)))/p  × 100)


 

Figure 3. Graph of SpeedUp
