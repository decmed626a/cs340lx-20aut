## Readings for cs340lx

We wrote a lot of code in cs240lx, which is probably the single most effective
way to understand things at a detailed level.  For cs340lx we'll add 
readings, so you can also get a good idea for the concepts at a higher
level and also so you can see how to write your own research papers.

  - [0-purify](0-purify): We already read this paper for cs240lx,
    but since we're going to build a real version (not the simple 
    allocator checker from lab 11 in cs240lx) it may be worth reading
    over it again.   

    Fun fact: When Rational, the company that did Purify was sold,
    the founder, Reed Hastings used that money to start Netflix.
    Rational also patented some binary rewriting techniques, which set
    the field back significantly and are one of the distal causes for
    why I think software patents are completely stupid.

  - [1-eraser](1-eraser): we're going to make a single-step
    version of eraser for lock checking, so it's good to see the original.
    There's also an annotated version: 

    Fun fact: One of three non-performance papers published in the 1990s
    in the main systems conference (SOSP).

   - [2-atom](2-atom): Atom: A System for Building Customized Program
     Analysis Tools, Amitabh Srivastava and Alan Eustace

     The system that made Eraser (and many other tools possible).
     Deceptively simple interface for the power you got.

     Fun fact: one of the early winners of the PLDI significant papers
     award.  I nominated it.  (Others probably did as well!)


Further readings:
  - In general, most papers from [cs240](http://web.stanford.edu/class/cs240/)
    will be relevant. 
  - For tools labs, the papers in [cs343](http://web.stanford.edu/class/cs343/)
    are a good starting point.
