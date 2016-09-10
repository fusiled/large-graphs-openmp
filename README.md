# large-graphs-openmp

Project of the 2015/2016 course Advanced Algorithms and Parallel Programming.

Parser reads this format:

Comment lines	Comment lines can appear anywhere and are ignored by programs:
 	c This is a comment

Problem line	The problem line is unique and must appear as the first non-comment line. This line has the format on the right, where n and m are the number of nodes and the number of arcs, respectively:
	p sp n m
Arc descriptor lines	Arc descriptors are of the form on the right, where U and V are the tail and the head node ids, respectively, and W is the arc weight_
	a U V W