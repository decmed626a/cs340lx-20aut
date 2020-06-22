/*
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: iprof.anal.c,v $
 * Revision 1.1.2.2  1995/04/27  19:46:54  Greg_Lueck
 * 	First revision of productized Atom and tools.
 * 	[1995/04/26  21:19:20  Greg_Lueck]
 *
 * $EndLog$
 */
#pragma ident "@(#)$RCSfile: iprof.anal.c,v $ $Revision: 1.1.2.2 $ (DEC) $Date: 1995/04/27 19:46:54 $"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

long instrTotal = 0;
long *instrPerProc;
long *callsPerProc;

FILE *OpenUnique(char *fileName, char *type)
{
  FILE *file;
  char Name[200];

  if (getenv("ATOMUNIQUE") != NULL)
    sprintf(Name,"%s.%d",fileName,getpid());
  else
    strcpy(Name,fileName);

  file = fopen(Name,type);
  if (file == NULL)
    {
      fprintf(stderr,"Atom: can't open %s for %s\n",Name, type);
      exit(1);
    }
  return(file);
}

static FILE *file;
void OpenFile(int number)
{
  file = OpenUnique("iprof.out","w");
  fprintf(file,"%30s %15s %15s %12s\n","Procedure","Calls",
	  "Instructions","Percentage");
  instrPerProc = (long *) calloc(sizeof(long), number);
  callsPerProc = (long *) calloc(sizeof(long), number);
  if (instrPerProc == NULL || callsPerProc == NULL) {
    fprintf(stderr,"Malloc failed\n");
    exit(1);
  }
}

void ProcedureCalls(int number)
{
  callsPerProc[number]++;
}
  
void ProcedureCount(int number, int instructions)
{
  instrTotal += instructions;
  instrPerProc[number] += instructions;
}
  

void ProcedurePrint(int number, char *name)
{
  if (instrPerProc[number] > 0) {
    fprintf(file,"%30s %15ld %15ld %12.3f\n",
	    name, callsPerProc[number], instrPerProc[number], 
	    100.0 * instrPerProc[number] / instrTotal);
  }
}

void CloseFile()
{
  fprintf(file,"\n%30s %15s %15ld\n", "Total", "", instrTotal);
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
