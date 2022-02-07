                                                                ///////////////////////////////////////////////////
                                                                ///						                                  ///
                                                                ///	                Text Analyzer	    	        ///									
                                                                ///						                                  ///
                                                                ///////////////////////////////////////////////////

A short introduction:

First of all, I would like to apologize how bad the code was written and clumped together inside a single source code file. 
When I wrote this code, we were not taught how to work with multiple source code files, yet. So, my professor forbid us to attempt 
splitting the code into multiple files. I can re-write this cleaner and more optimized given with my current knowledge and skills.

Also, we were not allowed to use vector, so I did a little research about pointers and dynamic allocation. After some experiments 
and playing with my code, I was able to come up with algorithm that allow "my" array to act like a vector. However, it was without 
all the methods or functions similar to vector except the expansion function. Also, by doing this I was able to overcome the constraint 
of having limited size(set by our professor) for the array to store the tokens(words). But of course, the array size is still limited to 
the memory a computer has when perfoming run-time expansion. 


How to use:

1). Create or move a text file inside the same directory where the .exe file is located.
2). Launch the binary file (i.e. ".exe" file) after compiling.
3). Follow the prompts displayed on the console.

- END -
