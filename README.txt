I am working by myself.

I made various improvements to make my AI tournament worthy.
1) I changed the implementation of the board from 2 bitsets (which are inherently more time and memory consuming)
     to use two bitboards instead, using a uint64_t type integer to represent both black and taken. Then when accessing
    the various elements, I used bit shifting and bitwise logical operators.
2) In addition, to reduce the memory usage (memory = time after some point), I decided to maintain a stack of moves
     done, and then undo the moves using the stack after the move had been tried out. Otherwise, creating an instance
     of the board using copy to try out each move would have become way too time consuming after a certain depth.
3) In addition, beyond the heuristic evaluation mentioned in class, I changed mine to include the number of edges both
    players have, as these pieces are also important in that only another edge piece can capture them.
4) I also implemented both alpha-beta pruning and then improved it using  Negascout or Principal variation search to
     avoid looking at unnecessary branches of the game tree, making the minimax search a lot faster.
5) I also maintain a transposition table of what moves to do for certain board configurations. To do this, I wrote my
     own linked list/node structs, specifically geared towards the transposition table use, and had an array of linked lists.
    My  hash value was the number of opponent's colors on the board, which allowed for about 64 different hash values.
6) Finally, I also changed the depth of search depending on how much time I had. In all cases, I first did a search of depth 7.
      Then, if I had atleast 3 minutes left (and hadn't been cutoff a search once before), I did a search again of depth 8, which
     could use the results from the previous search. However, if there were fewer empty spaces on the board, meaning
     fewer moves to check, I increased the depth of search to 12 and then to 20. This extra depth of the search makes the
    the AI better able to win at the end.




I was not able to use the transposition tables to the extent I could have to make searching more efficient, but was not
able to do so. I'm not sure if it fully helps now, but it is not slowing down the program's running.

Also, I tried to be able to search to greater depths earlier, but had timeout issues, and thus had to restrict myself
to the above-mentioned depths.



Resources used:
radagast.se/othello/Help/strategy.html
pages.cs.wisc.edu/~psilord/blog/data/chess-pages/rep.html
http://www.cs.cornell.edu/~yuli/othello/othello.html
https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
https://en.wikipedia.org/wiki/Principal_variation_search
https://en.wikipedia.org/wiki/Transposition_table