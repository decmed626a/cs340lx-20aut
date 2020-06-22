/*
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: branch.anal.c,v $
 * Revision 1.1.2.2  1995/04/27  19:41:39  Greg_Lueck
 * 	First revision of productized Atom and tools.
 * 	[1995/04/26  21:12:32  Greg_Lueck]
 *
 * $EndLog$
 */
#pragma ident "@(#)$RCSfile: branch.anal.c,v $ $Revision: 1.1.2.2 $ (DEC) $Date: 1995/04/27 19:41:39 $"


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define TABLESIZE 256
int branchHistory[TABLESIZE];

struct BranchType {
  long pc;
  long predicts;
  long references;
} *branches;

int branchct;
FILE *file;

void OpenFile(int number)
{
  branchct = number;
  branches = (struct BranchType *) calloc(sizeof(struct BranchType),number);
  file = fopen("branch.out","w");
}

int takenTable[] =    { 1, 2, 3, 3 };
int notTakenTable[] = { 0, 0, 1, 2 };

void CondBranch(int number, long pc, long taken)
{
  long index, history;

  index = (pc >> 2) & (TABLESIZE-1);
  history = branchHistory[index];
  if (taken) {
    if (history >= 2) branches[number].predicts++;
    branchHistory[index] = takenTable[history];
  } else {
    if (history <= 1) branches[number].predicts++;
    branchHistory[index] = notTakenTable[history];
  }
  branches[number].references++;
  branches[number].pc = pc;
}

void PrintResults(void)
{
  long references;
  long predicts;
  int i;

  fprintf(file,"%18s %20s %20s\n","PC","References","Predicted");
  references = 0;
  predicts = 0;
  for (i = 0; i < branchct; i++) {
    if (branches[i].references > 0) {
      references += branches[i].references;
      predicts += branches[i].predicts;
      fprintf(file,"0x%016lx %20ld %20f\n",
	      branches[i].pc, branches[i].references,
	      (float) branches[i].predicts / branches[i].references);
    }
  }
  fprintf(file,"%18s %20ld %20f\n","Totals", references,
                  (float) predicts / references);
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
