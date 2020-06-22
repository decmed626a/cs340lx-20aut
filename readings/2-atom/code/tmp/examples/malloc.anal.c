/*
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: malloc.anal.c,v $
 * Revision 1.1.2.2  1995/04/27  19:47:19  Greg_Lueck
 * 	First revision of productized Atom and tools.
 * 	[1995/04/26  21:19:51  Greg_Lueck]
 *
 * $EndLog$
 */
#pragma ident "@(#)$RCSfile: malloc.anal.c,v $ $Revision: 1.1.2.2 $ (DEC) $Date: 1995/04/27 19:47:19 $"

/*
**  malloc.anal.c -	Analysis routines for malloc recording tool.
**
**	This tool records each call to the routine malloc() and prints a
**	summary of the application's allocated memory.  See "malloc.inst.c"
**	for a description of how to use this tool.
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>


/*
 * We record a histogram of allocation request.  Requests for less than
 * this amount are included in the histogram.  Requests for more than this
 * amount are reported outside of the histogram.
 */
#define MAXSIZE 	16384

/*
 * Allocate an array of structures to record information about various
 * sized allocations.  Allocate an additional structure to hold
 * information about large allocations.
 */
struct {
    long 	calls;
    long 	memoryAllocated;
} mallocHistory[MAXSIZE], mallocOther;


/*
 * Malloc() may call itself recursively.  This keeps track of recursive
 * calls, so we only record allocations made directly by the application.
 */
int 	depth = 0;


/*
 * These keep track of the initial, current, and maximum 'break' value
 * for the application.  The 'break' value is the last address of the
 * application's heap area.  We use these to determine the amount of
 * heap memory actually used by the application.
 */
long 	StartBrk;
long 	CurBrk;
long 	MaxBrk;


/*
 * The handle for the output file.
 */
FILE *	file;



/*
**  Record a call to malloc().
*/
void BeforeMalloc(long size)
{
   /*
    * Ignore recursive calls.
    */
   if (depth++ != 0)
	return;

   /*
    * Record the allocation request.
    */
   if (size < MAXSIZE) {
	mallocHistory[size].calls++;
	mallocHistory[size].memoryAllocated += size;
   } else {
	mallocOther.calls++;
	mallocOther.memoryAllocated += size;
   }
}


/*
**  Note that we have returned from a malloc() call, so we can keep track
**  of recursive calls.
*/
void AfterMalloc()
{
    depth--;
}


/*
**  Record a call to brk().  This call changes the application's
**  'break' value.
*/
void BeforeBrk(long newbrk)
{
    CurBrk = newbrk;
    if (CurBrk > MaxBrk)
	MaxBrk = CurBrk;
}


/*
**  Record a call to sbrk().  This call increases the application's
**  'break' value.
*/
void BeforeSbrk(long size)
{
    CurBrk += size;
    if (CurBrk > MaxBrk)
	MaxBrk = CurBrk;
}


/*
**  Create the output file and set the initial address for the
**  application's 'break' value.
*/
void Initialize(long startbrk)
{
    file = fopen("malloc.out", "w");
    if (file == NULL) {
	fprintf(stderr, "malloc: Unable to create file 'malloc.out'.\n");
	exit(1);
    }

    StartBrk = startbrk;
    MaxBrk = startbrk;
    CurBrk = startbrk;
}


/*
**  Print the summary information about this application's malloc() calls.
*/
void PrintResults()
{
    long 	calls;
    long	allocated;
    int 	i;


    fprintf(file, "%8s %15s %20s\n", "Size", "calls", "allocated");

    calls = 0;
    allocated = 0;
    for (i = 0; i < MAXSIZE; i++) {
	if (mallocHistory[i].calls > 0) {
	    fprintf(file, "%8ld %15ld %20ld\n", i,
		mallocHistory[i].calls, mallocHistory[i].memoryAllocated);
	    calls += mallocHistory[i].calls;
	    allocated += mallocHistory[i].memoryAllocated;
	}
    }

    if (mallocOther.calls > 0) {
	fprintf(file, ">=%7ld %15ld %20ld\n", MAXSIZE,
	    mallocOther.calls, mallocOther.memoryAllocated);
	calls += mallocOther.calls;
	allocated += mallocOther.memoryAllocated;
    }

    fprintf(file, "\n%8s %15ld %20ld\n", "Totals", calls, allocated);
    fprintf(file, "\nMaximum Memory Allocated: %ld bytes\n", MaxBrk - StartBrk);
    fclose(file);
}
/****************************************************************************
 *                                                                          *
 *  COPYRIGHT (c) 1995 BY                                                   *
 *  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASSACHUSETTS.                  *
 *  ALL RIGHTS RESERVED.                                                    *
 *                                                                          *
 *  THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND COPIED   *
 *  ONLY IN  ACCORDANCE WITH  THE  TERMS  OF  SUCH  LICENSE  AND WITH THE   *
 *  INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR ANY  OTHER   *
 *  COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY   *
 *  OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE IS  HEREBY   *
 *  TRANSFERRED.                                                            *
 *                                                                          *
 *  THE INFORMATION IN THIS SOFTWARE IS  SUBJECT TO CHANGE WITHOUT NOTICE   *
 *  AND  SHOULD  NOT  BE  CONSTRUED AS  A COMMITMENT BY DIGITAL EQUIPMENT   *
 *  CORPORATION.                                                            *
 *                                                                          *
 *  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE  OR  RELIABILITY OF ITS   *
 *  SOFTWARE ON EQUIPMENT WHICH IS NOT SUPPLIED BY DIGITAL.                 *
 *                                                                          *
 ****************************************************************************
 *              Copyright 1995 Digital Equipment Corporation
 *                         All Rights Reserved
 *
 * Permission to use, copy, and modify this software and its documentation is
 * hereby granted only under the following terms and conditions.  Both the
 * above copyright notice and this permission notice must appear in all copies
 * of the software, derivative works or modified versions, and any portions
 * thereof, and both notices must appear in supporting documentation.
 *
 * This software may be distributed (but not offered for sale or transferred
 * for compensation) to third parties, provided such third parties agree to
 * abide by the terms and conditions of this notice.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */
