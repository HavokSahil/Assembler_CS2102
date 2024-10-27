#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdio.h>
#include <common_ds.h>
#include <common_types.h>
#include <err_codes.h>
#include <decoder/decoder.h>

/* Log in stdout  */
/* Colored output in terminal  */
/* Catchy Format of output  */

/* Log in file   */
/* Logging levels  */
/* Proper definations of error and warning codes  */


struct _lg_logger_interface {
	FILE* stream1;	/*   */
	FILE* stream2;	/*   */

	AType level;
	EWList* elist;

	
	/* Assets for  Advanced Listing File Generation   */
	IList* ilist;
	SymTable* stable;
	DList* dlist;
	MnMap* mnmap;
	RegMap* rmap;

	AErr (*log)(struct _lg_logger_interface*, AType);
	AErr (*logmn)(struct _lg_logger_interface*);
	AErr (*logreg)(struct _lg_logger_interface*);
	AErr (*generate_alf)(struct _lg_logger_interface*, DecoderInterface* di ,FILE*);
	void (*destroy)(struct _lg_logger_interface*);
};

typedef struct _lg_logger_interface LoggerInterface;


LoggerInterface* lg_new_LoggerInterface(FILE*, FILE*, AType, EWList*, IList*, SymTable*, DList*, MnMap*, RegMap*);
void lg_destroy_LoggerInterface(LoggerInterface*);

#endif
