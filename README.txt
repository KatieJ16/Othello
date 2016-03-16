I have one last extension.

Improvements::
I am working solo (for the 2 point part)

I started by just getting the AI to make legal moves. 
Then I added a heuristic that took the highest score. Then added to favor corners and edges and disfavor spaces that lead to corners. However, I had some error when the opponent passed, so I didn’t get any points for beating SimplePlayer.
I also made the minimax work for the test, but it seemed that my other method worked better.

The new improvements:
tried added more depth to the minimax, but original AI was working better
Didn’t always pick corners, which are very important pieces, so I added that if a corner was a legal move, it would be picked.
When I tried to make the minimax have a more depth, I was getting to many bugs that I couldn’t figure out.
Made a negamax that uses recursion. didn’t always pick corners, so added part to help get to corners
the negamax beats ConstantTimePlayer pretty well. Beats while white more than while black

