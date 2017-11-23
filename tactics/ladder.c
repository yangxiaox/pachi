#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define QUICK_BOARD_CODE

#define DEBUG
#include "board.h"
#include "debug.h"
#include "mq.h"
#include "tactics/1lib.h"
#include "tactics/selfatari.h"
#include "tactics/dragon.h"
#include "tactics/ladder.h"


/* Read out middle ladder countercap sequences ? Otherwise we just
 * assume ladder doesn't work if countercapturing is possible. */
#define MIDDLE_LADDER_CHECK_COUNTERCAP 1


bool
is_border_ladder(struct board *b, coord_t coord, group_t laddered, enum stone lcolor)
{
	if (can_countercapture(b, laddered, NULL, 0))
		return false;
	
	int x = coord_x(coord, b), y = coord_y(coord, b);

	if (DEBUGL(5))
		fprintf(stderr, "border ladder\n");
	/* Direction along border; xd is horiz. border, yd vertical. */
	int xd = 0, yd = 0;
	if (board_atxy(b, x + 1, y) == S_OFFBOARD || board_atxy(b, x - 1, y) == S_OFFBOARD)
		yd = 1;
	else
		xd = 1;
	/* Direction from the border; -1 is above/left, 1 is below/right. */
	int dd = (board_atxy(b, x + yd, y + xd) == S_OFFBOARD) ? 1 : -1;
	if (DEBUGL(6))
		fprintf(stderr, "xd %d yd %d dd %d\n", xd, yd, dd);
	/* | ? ?
	 * | . O #
	 * | c X #
	 * | . O #
	 * | ? ?   */
	/* This is normally caught, unless we have friends both above
	 * and below... */
	if (board_atxy(b, x + xd * 2, y + yd * 2) == lcolor
	    && board_atxy(b, x - xd * 2, y - yd * 2) == lcolor)
		return false;

	/* ...or can't block where we need because of shortage
	 * of liberties. */
	group_t g1 = group_atxy(b, x + xd - yd * dd, y + yd - xd * dd);
	int libs1 = board_group_info(b, g1).libs;
	group_t g2 = group_atxy(b, x - xd - yd * dd, y - yd - xd * dd);
	int libs2 = board_group_info(b, g2).libs;
	if (DEBUGL(6))
		fprintf(stderr, "libs1 %d libs2 %d\n", libs1, libs2);
	/* Already in atari? */
	if (libs1 < 2 || libs2 < 2)
		return false;
	/* Would be self-atari? */
	if (libs1 < 3 && (board_atxy(b, x + xd * 2, y + yd * 2) != S_NONE
			  || coord_is_adjecent(board_group_info(b, g1).lib[0], board_group_info(b, g1).lib[1], b)))
		return false;
	if (libs2 < 3 && (board_atxy(b, x - xd * 2, y - yd * 2) != S_NONE
			  || coord_is_adjecent(board_group_info(b, g2).lib[0], board_group_info(b, g2).lib[1], b)))
		return false;
	return true;
}



static int middle_ladder_walk(struct board *b, group_t laddered, enum stone lcolor,
			      struct move_queue *prev_ccq, coord_t prevmove, int len);

static int
middle_ladder_chase(struct board *b, group_t laddered, enum stone lcolor, struct move_queue *ccq,
		    coord_t prevmove, int len)
{
	laddered = group_at(b, laddered);
	
	if (DEBUGL(8)) {
		board_print(b, stderr);
		fprintf(stderr, "%s c %d\n", coord2sstr(laddered, b), board_group_info(b, laddered).libs);
	}

	if (!laddered || board_group_info(b, laddered).libs == 1) {
		if (DEBUGL(6))
			fprintf(stderr, "* we can capture now\n");
		return len;
	}
	if (board_group_info(b, laddered).libs > 2) {
		if (DEBUGL(6))
			fprintf(stderr, "* we are free now\n");
		return 0;
	}

	/* Now, consider alternatives. */
	int liblist[2], libs = 0;
	for (int i = 0; i < 2; i++) {
		coord_t ataristone = board_group_info(b, laddered).lib[i];
		coord_t escape = board_group_info(b, laddered).lib[1 - i];
		if (immediate_liberty_count(b, escape) > 2 + coord_is_adjecent(ataristone, escape, b)) {
			/* Too much free space, ignore. */
			continue;
		}
		liblist[libs++] = i;
	}

	/* Try more promising one first */
	if (libs == 2 && immediate_liberty_count(b, board_group_info(b, laddered).lib[0]) <
	                 immediate_liberty_count(b, board_group_info(b, laddered).lib[1])) {
		liblist[0] = 1; liblist[1] = 0;
	}

	/* Try out the alternatives. */
	for (int i = 0; i < libs; i++) {		
		coord_t ataristone = board_group_info(b, laddered).lib[liblist[i]];

		with_move(b, ataristone, stone_other(lcolor), {
			/* If we just played self-atari, abandon ship. */
			if (board_group_info(b, group_at(b, ataristone)).libs <= 1)
				break;
			
			if (DEBUGL(6))
				fprintf(stderr, "(%d=0) ladder atari %s (%d libs)\n", i, coord2sstr(ataristone, b), board_group_info(b, group_at(b, ataristone)).libs);

			int l = middle_ladder_walk(b, laddered, lcolor, ccq, prevmove, len);
			if (l)
				with_move_return(l);
		});

	}
	
	return 0;
}

static void
add_chaser_captures(struct board *b, group_t laddered, enum stone lcolor, struct move_queue *ccq,
		    coord_t nextmove)
{
	foreach_neighbor(b, nextmove, {
		if (board_at(b, c) == stone_other(lcolor) && board_group_info(b, group_at(b, c)).libs == 1) {
			/* Ladder stone we can capture later, add it to the list */
			coord_t lib = board_group_info(b, group_at(b, c)).lib[0];
			if (DEBUGL(6))
				fprintf(stderr, "adding potential chaser capture %s\n", coord2sstr(lib, b));
			mq_add(ccq, lib, 0);
		}
	});
}

/* Can we escape by capturing chaser ? */
static bool
chaser_capture_escapes(struct board *b, group_t laddered, enum stone lcolor, struct move_queue *ccq)
{
	for (unsigned int i = 0; i < ccq->moves; i++) {
		coord_t lib = ccq->move[i];
		if (!board_is_valid_play(b, lcolor, lib))
			continue;

#ifndef MIDDLE_LADDER_CHECK_COUNTERCAP
		return true;
#endif

		/* We can capture one of the ladder stones, investigate ... */
		if (DEBUGL(6)) {
			fprintf(stderr, "------------- can capture chaser, investigating %s -------------\n", coord2sstr(lib, b));
			board_print(b, stderr);
		}

		with_move_strict(b, lib, lcolor, {
			if (!middle_ladder_chase(b, laddered, lcolor, ccq, lib, 0))
				with_move_return(true); /* escape ! */		
		});

		if (DEBUGL(6))
			fprintf(stderr, "-------------------------- done %s ------------------------------\n", coord2sstr(lib, b));
	}
	
	return false;
}


/* This is a rather expensive ladder reader. It can read out any sequences
 * where laddered group should be kept at two liberties. The recursion
 * always makes a "to-be-laddered" move and then considers the chaser's
 * two alternatives (usually, one of them is trivially refutable). The
 * function returns true if there is a branch that ends up with laddered
 * group captured, false if not (i.e. for each branch, laddered group can
 * gain three liberties). */
static int
middle_ladder_walk(struct board *b, group_t laddered, enum stone lcolor,
		   struct move_queue *prev_ccq, coord_t prevmove,
		   int len)
{
	assert(board_group_info(b, laddered).libs == 1);

	/* Check ko */
	if (b->ko.coord != pass)
		foreach_neighbor(b, b->last_move.coord, {
				if (group_at(b, c) == laddered) {
					if (DEBUGL(6))
						fprintf(stderr, "* ko, no ladder\n");
					return 0;
				}
		});

	/* Check countercaptures */
	struct move_queue ccq = *prev_ccq;
	if (prevmove != pass)
		add_chaser_captures(b, laddered, lcolor, &ccq, prevmove);
	if (chaser_capture_escapes(b, laddered, lcolor, &ccq))
		return 0;

	/* Escape then */
	coord_t nextmove = board_group_info(b, laddered).lib[0];
	if (DEBUGL(6))
		fprintf(stderr, "  ladder escape %s\n", coord2sstr(nextmove, b));
	with_move_strict(b, nextmove, lcolor, {
		len = middle_ladder_chase(b, laddered, lcolor, &ccq, nextmove, len + 1);
	});

	return len;
}

static __thread int length = 0;

bool
is_middle_ladder(struct board *b, coord_t coord, group_t laddered, enum stone lcolor)
{
	/* TODO: Remove the redundant parameters. */
	assert(board_group_info(b, laddered).libs == 1);
	assert(board_group_info(b, laddered).lib[0] == coord);
	assert(board_at(b, laddered) == lcolor);

	/* If we can move into empty space or do not have enough space
	 * to escape, this is obviously not a ladder. */
	if (immediate_liberty_count(b, coord) != 2) {
		if (DEBUGL(5))
			fprintf(stderr, "no ladder, wrong free space\n");
		return false;
	}

	/* A fair chance for a ladder. Group in atari, with some but limited
	 * space to escape. Time for the expensive stuff - play it out and
	 * start selective 2-liberty search. */

	/* We could escape by countercapturing a group. */
	struct move_queue ccq = { .moves = 0 };
	can_countercapture(b, laddered, &ccq, 0);

	length = middle_ladder_walk(b, laddered, lcolor, &ccq, pass, 0);

	if (DEBUGL(6) && length) {
		fprintf(stderr, "is_ladder(): stones: %i  length: %i\n",
			group_stone_count(b, laddered, 50), length);	    
	}

	return (length != 0);
}

bool
is_middle_ladder_any(struct board *b, coord_t coord, group_t laddered, enum stone lcolor)
{
	/* TODO: Remove the redundant parameters. */
	assert(board_group_info(b, laddered).libs == 1);
	assert(board_group_info(b, laddered).lib[0] == coord);
	assert(board_at(b, laddered) == lcolor);

	/* We could escape by countercapturing a group. */
	struct move_queue ccq = { .moves = 0 };
	can_countercapture(b, laddered, &ccq, 0);
	
	length = middle_ladder_walk(b, laddered, lcolor, &ccq, pass, 0);
	return (length != 0);
}

bool
wouldbe_ladder(struct board *b, group_t group, coord_t escapelib, coord_t chaselib, enum stone lcolor)
{
	assert(board_group_info(b, group).libs == 2);
	assert(board_at(b, group) == lcolor);

	if (DEBUGL(6))
		fprintf(stderr, "would-be ladder check - does %s %s play out chasing move %s?\n",
			stone2str(lcolor), coord2sstr(escapelib, b), coord2sstr(chaselib, b));

	if (!coord_is_8adjecent(escapelib, chaselib, b)) {
		if (DEBUGL(5))
			fprintf(stderr, "cannot determine ladder for remote simulated stone\n");
		return false;
	}

	if (neighbor_count_at(b, chaselib, lcolor) != 1 || immediate_liberty_count(b, chaselib) != 2) {
		if (DEBUGL(5))
			fprintf(stderr, "overly trivial for a ladder\n");
		return false;
	}

	bool is_ladder = false;
	with_move(b, chaselib, stone_other(lcolor), {
		is_ladder = is_middle_ladder_any(b, board_group_info(b, group).lib[0], group, lcolor);
	});
	
	return is_ladder;
}


bool
wouldbe_ladder_any(struct board *b, group_t group, coord_t escapelib, coord_t chaselib, enum stone lcolor)
{
	assert(board_group_info(b, group).libs == 2);
	assert(board_at(b, group) == lcolor);
	
	if (!board_is_valid_play(b, stone_other(lcolor), chaselib) ||
	    is_selfatari(b, stone_other(lcolor), chaselib) )   // !can_play_on_lib() sortof       
		return false;

	bool ladder = false;
	with_move(b, chaselib, stone_other(lcolor), {
		assert(group_at(b, group) == group);
		ladder = is_ladder_any(b, escapelib, group, true);
	});

	return ladder;
}

/* Laddered group can't escape, but playing it out could still be useful.
 *
 *      . . . * . . .    For example, life & death:
 *      X O O X O O X
 *      X X O O O X X
 *          X X X     
 *
 * Try to weed out as many useless moves as possible while still allowing these ...
 * Call right after is_ladder() succeeded, uses static state.  
 *
 * XXX can also be useful in other situations ? Should be pretty rare hopefully */
bool
useful_ladder(struct board *b, group_t laddered)
{
	if (length >= 4 ||
	    group_stone_count(b, laddered, 6) > 5 ||
	    neighbor_is_safe(b, laddered))
		return false;

	coord_t lib = board_group_info(b, laddered).lib[0];
	enum stone lcolor = board_at(b, laddered);

	/* Check capturing group is surrounded */
	with_move(b, lib, stone_other(lcolor), {	
		assert(!group_at(b, laddered));
		if (!dragon_is_surrounded(b, lib))
			with_move_return(false);
	});
	
	/* Group safe even after escaping + capturing us ? */
	// XXX can need to walk ladder twice to become useful ...
	bool still_safe = false, cap_ok = false;
	with_move(b, lib, lcolor, {
		if (!group_at(b, lib))
			break;
		
		group_t g = group_at(b, lib);
		// Try diff move order, could be suicide !
		for (int i = 0; !cap_ok && i < board_group_info(b, g).libs; i++) {
			coord_t cap = board_group_info(b, g).lib[i];
			with_move(b, cap, stone_other(lcolor), {				       
				if (!group_at(b, lib) || !group_at(b, cap))
					break;				
				coord_t cap = board_group_info(b, group_at(b, lib)).lib[0];						
				with_move(b, cap, stone_other(lcolor), {
						assert(!group_at(b, lib));
						cap_ok = true;
						still_safe = dragon_is_safe(b, group_at(b, cap), stone_other(lcolor));
				});
			});
		}
	});
	if (still_safe)
		return false;

	/* Does it look useful as selfatari ? */
	foreach_neighbor(b, lib, {
		if (board_at(b, c) != S_NONE)
			continue;
		with_move(b, c, stone_other(lcolor), {
			if (board_group_info(b, group_at(b, c)).libs - 1 <= 1)
				break;
			if (!is_bad_selfatari(b, lcolor, lib))
				with_move_return(true);
		});
	});
	return false;
}
