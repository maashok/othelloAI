radagast.se/othello/Help/strategy.html

the cornell heuristic function

Bitboard - pages.cs.wisc.edu/~psilord/blog/data/chess-pages/rep.html

Number yours - Number Theirs

Stable discs - Corner, once you get corner, stones next to it
	Number of Your Stable - Number of Their Stable                           <-- Did this
Wedges - Even number of spaces between two edge discs of opposite color
	Good places to play...
Avoid flipping too many frontier discs
	Thing to note
# of Legal Moves
	# of Poss Moves for you - # Poss Moves for Opponents << did this

Edges are good									<-- DID THIS




Stone Difference + Stable Disc differerence * 30 + Edge Difference * 15 + MovesLeft Difference * 10

Tried lowering score for unstable areas (next to corner) -- led to still consistent wins, but with lower margins

Recurse to depth 5
	Was able to do this without taking too much time/memory by keeping a stack of moves done and just undoing moves based on that
	for each move we try out (Since DFS) on just one board --> Creating many boards would be too time intensive

	Seems to work - Can beat BetterPlayer in under 2 minutes total, but am not able to recurse to depth 6 due to allocation issues (not sure what?)

Also eventually, have a different depth if running out of time!!!