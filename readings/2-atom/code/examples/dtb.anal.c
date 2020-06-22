/*
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: dtb.anal.c,v $
 * Revision 1.1.2.2  1995/04/27  19:42:15  Greg_Lueck
 * 	First revision of productized Atom and tools.
 * 	[1995/04/26  21:13:33  Greg_Lueck]
 *
 * $EndLog$
 */
#pragma ident "@(#)$RCSfile: dtb.anal.c,v $ $Revision: 1.1.2.2 $ (DEC) $Date: 1995/04/27 19:42:15 $"


/*
	dtb: Determine the number of dtb misses if the application uses
	8Kb pages and a fully associative translation buffer.
 */

#include <stdio.h>
#include <assert.h>

#define NEXTDTB(index)  (((index)+1) & (DTBSIZE-1))
#define PAGESIZE 8192
#define PAGESHIFT 13
/* must be power of 2 */
#define DTBSIZE 32

long dtb[DTBSIZE];

int nextMapping = 0;
long hits,misses;

#define MAPSIZE 8192
int pageMap[MAPSIZE];

void Reference(long address)
{
  long page = address >> PAGESHIFT;
  int i = pageMap[page & (MAPSIZE-1)];

  if (dtb[i] != page) {
    for (i = 0; i < DTBSIZE; i++) {
      if (dtb[i] == page) {
	pageMap[page & (MAPSIZE - 1)] = i;
	break;
      }
    }
  }
  if (i >= DTBSIZE) {
    misses++;
    dtb[nextMapping] = page;
    pageMap[page & (MAPSIZE-1)] = nextMapping;
    nextMapping = NEXTDTB(nextMapping);
  } else {
    hits++;
    if (nextMapping == i) {
      nextMapping = NEXTDTB(i);
    }
  }
}

void PrintResults()
{
  FILE *file;
  file = fopen("dtb.out","w");
  assert(file != NULL);
  fprintf(file,"DTB hits:       %12ld\n",hits);
  fprintf(file,"DTB Misses:     %12ld\n",misses);
  fprintf(file,"DTB references: %12ld\n",hits+misses);
  fprintf(file,"Miss Rate:      %12.4lf\n",(double) misses/(hits+misses));
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
