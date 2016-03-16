Improvements::
I am working solo (for the 2 point part)

I started by just getting the AI to make legal moves. 
Then I added a heuristic that took the highest score. Then added to favor corners and edges and disfavor spaces that lead to corners. However, I had some error when the opponent passed, so I didn’t get any points for beating SimplePlayer.
I also made the minimax work for the test, but it seemed that my other method worked better.

The new improvements:
added more depth to the minimax. 
Didn’t always pick corners, which are very important pieces, so I added that if a corner was a legal move, it would be picked. 
