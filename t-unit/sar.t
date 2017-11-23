% Basic self-atari check
boardsize 3
X X .
X X X
X X .
sar b c1 1
sar b c3 1
sar w c1 1
sar w c3 1

% Basic suicide check
boardsize 3
X X X
X X X
X X .
sar b c1 1
sar w c1 0

% 2 stones suicide check
boardsize 3
X X .
O . X
X X X
sar w b2 1
sar b b2 0

% Almost-nakade
boardsize 3
O O O
. . X
X X .
sar b c1 0
sar w c1 1
sar b b2 0
sar w b2 1
sar b a2 0
sar w a2 1

% Bulky-five / tetris-four nakade
boardsize 3
O O O
. . X
X X X
sar b b2 0
sar w b2 0
sar b a2 1
sar w a2 1

% Bulky-five
boardsize 3
O O O
. X .
X X X
sar b c2 0
sar w c2 1  # Makes bent 4, bad normally    (except in corner ... FIXME ?)
sar b a2 0
sar w a2 1

% Rabbity six nakade 1
boardsize 4
X X O O
. X . O
O X O O
O O O O
sar w a3 1
sar b a3 0
sar w c3 1
sar b c3 1

% Rabbity six nakade 2 - seki
boardsize 4
X . O O
. X X O
O X O O
O O O O
sar w a3 1
sar b a3 1
sar w b4 1
sar b b4 1

% Rabbity six nakade 3
boardsize 4
X X O O
X X . O
O . O O
O O O O
sar w b2 1
sar b b2 0
sar w c3 1
sar b c3 0

% Triangle six lives
boardsize 5
O O O O O
O . . O O
O X X O O
O X X X O
O O O O O
sar b b4 1
sar b c4 1

% Seki destruction
boardsize 4
X X X .
X O O O
X . X X
X X X X
sar w b2 1
sar b b2 1
sar w d4 1
sar b d4 1

% Bulky five nakade
boardsize 5
X X X X X
X O O X X
X O . X X
X X . X X
X X X X X
sar b c3 0
sar w c3 0
sar b c2 1
sar w c2 1

% Bulky five nakade (outside lib)
boardsize 5
X X X X X
X O O X X
X O . X X
X X . X X
X X X X .
sar b c3 0
sar w c3 0
sar b c2 1
sar w c2 1

% Capture-from-within 2pt-eye nakade
boardsize 3
O O O
O . .
O O O
sar w b2 1
sar b b2 0
sar w c2 1
sar b c2 0

% Not Capture-from-within 2pt-eye nakade
boardsize 4
. . . .
. O O O
. O . .
. O O O
sar b c2 1
sar b d2 1

% Forbidden throw-in to get seki
boardsize 5
. X X X .
X X O O O
X O O . .
X O O O O
X X X X X
! sar b d3 0	# Gosh, sometimes bad moves are good
! sar b e3 0

% Capture-from-within 3pt-eye (straight) nakade
boardsize 3
O O O
. X .
O O O
sar w a2 1
sar b a2 0
sar w c2 1
sar b c2 0

% Capture-from-within 3pt-eye (bent) nakade
boardsize 3
O . O
O X .
O O O
sar w b3 1
sar b b3 0
sar w c2 1
sar b c2 0

% Capture-from-within 4pt-eye (square) nakade
boardsize 3
O . X
O . X
O O O
sar w b3 1
sar b b3 0
sar w b2 1
sar b b2 0

% Eye falsification nakade
boardsize 3
O O O
. . X
O O O
sar w a2 1
sar b a2 1
sar w b2 0
sar b b2 0

% Bulky five multi-w-group nakade
boardsize 5
X X X X X
X O O X X
X O . . X
X X O X X
X X X X X
sar b c3 0
sar w c3 0 
sar b d3 1
sar w d3 0	# Captures !

% Multi-b-group nakade
boardsize 5
. X X . .
X O O X .
X O . X X
X X . X X
. . X X X
sar b c3 0
sar w c3 0
sar b c2 0
sar w c2 1
sar b d5 0
sar w d5 0	# Throw-in
sar b e4 0
sar w e4 1
sar b e5 0
sar w e5 0

% Real multi-b-group nakade
boardsize 6
X X X O O .
X O . X O .
X . O X O .
O X X X O .
O O X X O .
O O O O O .
sar b b4 1
sar w b4 0
sar b c5 1
sar w c5 0

% Almost multi-b-group nakade
boardsize 6
X X X O O .
X O . X O .
X . O X O .
O X X X O .
O . X X O .
O O O O O .
sar b b4 0
sar w b4 0	# Have to allow this or we may never be able to kill that group
sar b c5 0
sar w c5 0

% Almost multi-b-group nakade (mirrored)
boardsize 6
. O O O O O
. O X X . O
. O X X X O
. O X O . X
. O X . O X
. O O X X X
sar b d2 0
sar w d2 0
sar b e3 0
sar w e3 0

% Eyeshape-avoidance nakade 1
boardsize 4
X X X X
X O . X
X X . X
X X X X
sar w c3 0
sar w c2 1

% Eyeshape-avoidance nakade 1 (outside lib)
boardsize 4
X X X X
X O . X
X X . X
. X X X
sar w c3 0
sar w c2 1

% Eyeshape-avoidance nakade 2
boardsize 4
X X X X
X O . O
X X . X
X X X X
sar w c3 0
sar w c2 1

% Eyeshape-avoidance nakade 2 (outside lib)
boardsize 4
X X X X
X O . O
X X . X
. X X X
sar w c3 0
sar w c2 1

% Eyeshape-avoidance nakade 3
boardsize 4
X X X X
X O . O
X X X .
X X X X
sar w c3 0
sar w d2 1

% Eyeshape-avoidance nakade 3 (outside lib)
boardsize 4
X X X X
X O . O
X X X .
. X X X
sar w c3 0
sar w d2 1

% False nakade
boardsize 5
X . X X .
X O O X X
X O O X .
X . O X X
X O O X X
sar w b2 1
sar b b2 1
sar w b5 1
sar b b5 0

% Not-quite-snapback
boardsize 5
X X X X O
X X X . O
X X X . O
O O O X X
O O . . .
sar b c1 0
sar w c1 1
sar b d3 0
sar w d3 1
sar b d4 1
sar w d4 0

% Snapback
boardsize 4
X X O .
. . X .
O X . .
. . . .
sar b a1 1
sar w a1 0
sar b a3 1
sar w a3 0
sar b b3 0
sar w b3 0	# Snapback !
sar b d4 0
sar w d4 0

% Real game 1
boardsize 9
O . O . . O X X .
. O . O . O O X .
O O . . O O X X O
X O O O O X O O O
X X O X X X X X O
. X O O O X X X X
. X O X X X . . X
X X X O . . X X X
X O . . O . . . O
sar w j8 1
sar b j8 0

% 3 stones sar nakade to dead shape (middle)
boardsize 5
X X X X X
O . O . X
X X X X X
X X X X X
X X X X X
sar w b4 0	# Fill eye
sar b b4 0	# Capture and live
sar b d4 1   

% 3 stones sar nakade to dead shape 
boardsize 5
X X X X X
O O . . X
X X X X X
X X X X X
X X X X X
sar w c4 0	# Fill eye
sar b c4 0	# Capture and live
sar b d4 1   

% 3 stones sar nakade to dead shape (outside libs)
boardsize 5
X X X X X
O O . . X
X X X X X
. . . . .
. . . . .
sar w c4 0	# Fill eye
sar w d4 1   

% 2 stones sar nakade to dead shape 
boardsize 4
X X X X
O . . X
X X X X
X X X X
sar w b3 0	# Fill eye
sar b b3 0	# Capture and live
sar b c3 1   

% 2 stones sar nakade to dead shape (outside libs)
boardsize 4
X X X X
O . . X
X X X X
. . . .
sar w b3 0	# Fill eye


% Bulky-five nakade (outside libs)
boardsize 6
X X X X X X
O O O O X X
. . X O X .
X X X O X X
O O O O X .
O O . . X X
sar b b4 0
sar b a4 1

% 4 stone nakade
boardsize 4
X O O O
X X . X
X X . X
X X X X
sar w c3 0
sar w c2 1

% 4 stone nakade (outside libs)
boardsize 4
X O O O
X X . X
. X . X
. X X X
sar w c3 0
sar w c2 1

% Connection
boardsize 4
. X O X
O . O X
O X X X
X X . .
sar b b3 0
sar w b3 1	# Connect and die

% Bad self-atari (3 stones, connect first !)
boardsize 5
O . O O .
O X X O .
O X . X .
O O X X .
. . X X .
sar b b5 1

% Bad self-atari (3 stones, can escape) 
boardsize 5
. O O O .
. O . O O
. O X X O
. O X . .
. O O . .
sar b c4 1

% Bad self-atari (4 stones, connect first !)
boardsize 6
O O O . O .
O O O X O O
O O O X O O
. O O X . X
. . O O X X
. . . . . .
sar b d6 1

% Bad self-atari (connecting 4 stones, connect and die)
boardsize 6
O O O X O .
O O O . O O
O O O X O O
. O O X . X
. . O O X X
. . . . . .
sar b d5 1

% Bad self-atari (5 stones, connect first !)
boardsize 6
O O O O O .
O O . X O O
O O X X X O
. O O X . X
. . O O X X
. . . . . .
sar b c5 1

% Connect instead of self-atari !
boardsize 5
X X X O .
X . O . .
X X X O .
. O O O .
. . . . .
sar w b4 1
sar w d4 0

% Bad self-atari (not taking away eyeshape and not atari)
boardsize 4
O O . X
X . X X
X X X .
. . . .
sar w c4 1 

% Not a nakade !  (threatening capture)
boardsize 4
X . O O
X X O O
O . . X
. . . .
sar b b4 1	# Can escape !
sar w b4 1	# Can escape !

% Not a nakade !  (threatening nothing)
boardsize 4
X . O O
X X O O
O . . .
. . . .
sar b b4 1	# Can escape !

% Not a nakade !  (threatening nothing)
boardsize 5
O O O O O
O X O O O
O X . O O
O X X O O
O O . . .
sar b c3 1	# Can escape !


% Corner nakade (shortage of libs)
boardsize 4
. O X .
X O . X
X O O O
X X X .
sar b c3 0
sar b d4 0

% Throw-in
boardsize 5
. . . X .
O O O X .
O . . O O
O O O X .
. . . X .
sar b c3 0
sar b b3 1	# Silly

% Throw-in making atari
boardsize 7
. . . . . . .
. . . X X X X
O O O X O O O
. X . O O . O
O O O X O O O
. . . X X X X
. . . . . . .
sar b c4 0
sar b a4 1      # silly

% Throw-in making atari (outside group with libs)
boardsize 7
. X . O O . O
O O O X X X O
. . X X X . O
. X . X . O O
. . . X . O .
. . . X . O O
. . . . X X .
sar b c7 0      # but cutting is good too ...
sar b a7 1      # silly

% Throw-in making atari with another group on the other side
boardsize 5
. X . O O
O O O X O
. X X X O
. X O O O
. X O . X
sar b c5 0      
sar b a5 1      # silly

% 2 stones throw-ins
boardsize 6
O O O O X .
O . X . O .
O O O O X X
. O X . . X
X O O O X X
. X . O . .

sar b d5 0   # a) in check_throw_in_or_inside_capture()
sar b d3 0   # b)
sar b c1 1   # c) silly, connect first		 [ d) tested in Capture-from-within 3pt-eye (straight) nakade ]

% Not a throw-in 
boardsize 4
. . . .
O O . .
X O O O
. X . O
sar b c1 1	# Captures 2 groups
sar b a1 0

% Side throw-in
boardsize 5
. O X . .
. O . O .
. X O X X
. X O X .
. . O . .
sar b c4 0

% Side throw-in with outside stone
boardsize 6
. O X . X .
. O . O O X
. X O X . .
. X O X X X
. . O . . .
. . . . . .
sar b c5 0
sar b a6 1

% Side throw-in with outside stone (continued)
boardsize 6
. O . O X .
. O . O O X
. X O X . .
. X O X X X
. . O . . .
. . . . . .
sar b c5 0
sar b c6 1
