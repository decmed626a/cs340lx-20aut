#include <stdio.h>
#include <cmplrs/atom.inst.h>

static int number;
static int nprocs;

static const char *	pname(Proc *);

/* 
 * Initialization: supply prototypes + clean up file I/O (may not
 * be flushed otherwise).
 */
void InstrumentInit(int argc, char **argv) {
   	AddCallProto("OpenFile(int)");
   	AddCallProto("ProcedureCount(int,int)");
   	AddCallProto("ProcedurePrint(int,char *)");
   	AddCallProto("CloseFile()");
   	AddCallProgram(ProgramAfter, "CloseFile");
}

int Instrument(int argc, char **argv, Obj *obj) {
	Proc *p; Block *b;

   	nprocs += GetObjInfo(obj, ObjNumberProcs);
   	for (p = GetFirstObjProc(obj); p; p = GetNextProc(p)) {
     		for (b = GetFirstBlock(p); b; b = GetNextBlock(b)) {
       			AddCallBlock(b,BlockBefore,"ProcedureCount",
		    		number,GetBlockInfo(b,BlockNumberInsts));
     		}
     		AddCallObj(obj, ObjAfter, "ProcedurePrint",number,pname(p));
     		number++;
   	}
	return 0;
}

/* Why is open file here? */
void InstrumentFini(void) {
   	AddCallProgram(ProgramBefore, "OpenFile",nprocs);
}

static const char *pname(Proc *p) {
    	const char *	name;
    	static char 	buf[128];

    	if ((name = ProcName(p)))
		return(name);
    	sprintf(buf, "proc_at_0x%lx", ProcPC(p));
    	return(buf);
}
