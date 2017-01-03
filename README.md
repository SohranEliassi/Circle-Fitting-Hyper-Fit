This is a revised circle fitting algorithm of the original Hyper fit algorithm, by Nikolai Chernov (http://people.cas.uab.edu/~mosya/cl/CPPcircle.html)

It is an algebraic fit, based on the journal article:
    [1] A. Al-Sharadqah and N. Chernov, "Error analysis for circle fitting algorithms",
    Electronic Journal of Statistics, Vol. 3, pages 886-911, (2009)
    
* THIS HYPER FIT ALGORITHM IS CONSIDERED TO BE ONE OF THE BEST (PERHAPS THE BEST) CIRCLE FITTING ALGORITHM, IN TERMS OF ACCURACY. IT IS ALSO RELATIVELY FAST. 

The original algorithm fits a (known) set of data points (in 2D) to a circle.
As a contribution to the original algorithm, we have added functions allowing circle fitting on complete and incomplete circular objects, in live-stream 2D-images.

* THE REVISED ALGORITHM HAS THE FOLLOWING IMPLEMENTATION STEPS:
Image acquisition-> Data points extraction, using Canny edge detection-> Gathering of data points, using allocated arrays
-> Circle fitting onto data points-> Printing of the fit circles arc and radius value onto captured frames

The algorithm is explained with comments in the "Source.cpp" file

IN OUR REPOSITORY, WE HAVE A VIDEO FILE ("CircleFitVideoExample") SHOWING AN EXAMPLE WHERE WE HAVE FIT A CIRCLE TO 2 INCOMPLETE CIRCULAR ARCS, USING OUR CIRCLE FITTING ALGORITHM.

As can be seen from our video example ("CircleFitVideoExample"), the Hyper fit algorithm can be implemented to fit accurate circles to largely incomplete circular arcs.

HOW TO USE THE CODE:
VISUAL STUDIO:
    We have used visual studio 2013 and OpenCV version 2.4.13, download at (http://opencv.org/downloads.html)
    install OpenCV following the guide below, it shows how to configure OpenCV 2.4.13 with visual studio 2010 (same for later versions)
    1. You need to configure OpenCV with visual studio, here is a link showing how to do that: https://www.youtube.com/watch?v=KoJTIs-h-0g
    2. Create a new project in visual studio, add all the files (Cpp and Headerfiles) from our repository to the project
    3. Link OpenCV libraries and additional dependencies (DLL:s)  to the project, following the youtube-tutorial from step 1.
    4. Compile and Run the application

 OTHER IDE:s OR COMPILERS:
    *For any other IDE:s or COMPILERS you need to do similar steps as for VISUAL STUDIO. 
    1. Create a project and add all the files (Cpp and Headerfiles) from our repository.
    2. Link OpenCV libraries and additional dependencies (DLL:s)  to the project
    3. Compile and Run the application
    
* If you struggle with implementing the algorithm, please contact me at soel1300@student.miun.se
