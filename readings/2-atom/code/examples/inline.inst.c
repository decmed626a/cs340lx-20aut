/*
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: inline.inst.c,v $
 * Revision 1.1.2.2  1995/04/27  19:46:43  Greg_Lueck
 * 	First revision of productized Atom and tools.
 * 	[1995/04/26  21:19:06  Greg_Lueck]
 *
 * $EndLog$
 */
#pragma ident "@(#)$RCSfile: inline.inst.c,v $ $Revision: 1.1.2.2 $ (DEC) $Date: 1995/04/27 19:46:43 $"


#include <stdio.h>
#include <cmplrs/atom.inst.h>

static int numberCallSites=0;

static const char *	SafeProcName(Proc *);


void InstrumentInit(int argc, char **argv)
{
   AddCallProto("OpenFile(int)");
   AddCallProto("CallSiteCount(int)");
   AddCallProto("CallSitePrint(int,long,char *,char *)");
   AddCallProto("CloseFile()");
   AddCallProgram(ProgramAfter,"CloseFile");
}

Instrument(int argc, char **argv, Obj *obj)
{
   Proc *p; Block *b; Inst *i;
   ProcRes target;
   int opcode,function;

   for (p = GetFirstObjProc(obj); p != NULL; p = GetNextProc(p)) {
     for (b = GetFirstBlock(p); b != NULL; b = GetNextBlock(b)) {
       for (i = GetFirstInst(b); i != NULL; i = GetNextInst(i)) {
	 /* JSR and BSR */
	 opcode = GetInstInfo(i,InstOpcode);
	 function = (GetInstInfo(i,InstMemDisp) >> 14) & 0x3;
	 if ((opcode == 0x1A && function == 1 ) || opcode == 0x34) {
	   ResolveTargetProc(i, &target);
	   if (target.name != NULL) {
	     AddCallInst(i,InstBefore,"CallSiteCount", numberCallSites);
	     AddCallObj(obj, ObjAfter,"CallSitePrint",
			numberCallSites, InstPC(i), target.name,
			SafeProcName(p));
	     numberCallSites++;
	   }
	 }
       }
     }
   }
}

void InstrumentFini(void)
{
   AddCallProgram(ProgramBefore,"OpenFile",numberCallSites);
}


static const char *SafeProcName(Proc *p)
{
    const char *	name;
    static char 	buf[128];

    name = ProcName(p);
    if (name)
	return(name);
    sprintf(buf, "proc_at_0x%lx", ProcPC(p));
    return(buf);
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
