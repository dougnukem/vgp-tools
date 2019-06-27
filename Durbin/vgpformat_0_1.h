/*  File: vgpformat_0_1.h
 *  Author: Richard Durbin (rd109@cam.ac.uk)
 *  Copyright (C) Richard Durbin, Cambridge University, 2019
 *-------------------------------------------------------------------
 * Description: this contains the VGP format specification 0.1
 *   the idea is to include this file into vgprd.c, so it is compiled into the executable
 *   it is a separate file so as to make it easy to change the format and version
 * Exported functions:
 * HISTORY:
 * Last edited: Jun 23 23:33 2019 (rd109)
 * Created: Sun Feb 24 14:48:21 2019 (rd109)
 *-------------------------------------------------------------------
 */

static LineSpecification *vgpDefineLine (FieldType f0, FieldType f1, FieldType f2,
					 FieldType f3, FieldType f4, FieldType f5)
{
  LineSpecification *ls = new0 (1, LineSpecification) ;
  ls->field[0] = f0 ; ls->field[1] = f1 ; ls->field[2] = f2 ;
  ls->field[3] = f3 ; ls->field[4] = f4 ; ls->field[5] = f5 ;
  return ls ;
}

static FileSpecification *vgpDefineFormat (void)
{
  static int MajorVersion = 0 ;
  static int MinorVersion = 1 ;
  
  int i, j, k ;
  FileSpecification *fileSpec = new0 (MAX_FILE, FileSpecification) ;
  LineSpecification **header = new0 (128, LineSpecification*) ;

  /* header line type 1 is treated separately below */
  header['2'] = vgpDefineLine (STRING, 0, 0, 0, 0, 0) ; /* subtype */
  header['#'] = vgpDefineLine (CHAR, INT, 0, 0, 0, 0) ; /* linetype count */
  header['@'] = vgpDefineLine (CHAR, INT, 0, 0, 0, 0) ; /* linetype max */
  header['+'] = vgpDefineLine (CHAR, INT, 0, 0, 0, 0) ; /* linetype total */
  header['%'] = vgpDefineLine (CHAR, CHAR, CHAR, INT, 0, 0) ; /* grouptype #/+ linetype value */
  header['!'] = vgpDefineLine (STRING_LIST, 0, 0, 0, 0, 0) ;	/* name version command date */
  header['<'] = vgpDefineLine (STRING, INT, 0, 0, 0, 0) ; /* filename objectcount */
  header['>'] = vgpDefineLine (STRING, 0, 0, 0, 0, 0) ;   /* filename */

  for (i = 0 ; i < MAX_FILE ; ++i)
    { fileSpec[i].major = MajorVersion ; fileSpec[i].minor = MinorVersion ;
      if (i > 0) for (j = 0 ; j < 128 ; ++j) if (header[j]) fileSpec[i].line[j] = header[j] ;
    }

  fileSpec[0].line['1'] = vgpDefineLine (STRING, INT, INT, 0, 0, 0) ;

  fileSpec[SEQ].objectType = 'S' ;
  fileSpec[SEQ].line['g'] = vgpDefineLine (INT, STRING, 0, 0, 0, 0) ; /* group number name */
  fileSpec[SEQ].line['S'] = vgpDefineLine (STRING, 0, 0, 0, 0, 0) ; /* the sequence */
  fileSpec[SEQ].line['Q'] = vgpDefineLine (STRING, 0, 0, 0, 0, 0) ; /* qualities ascii 33+q */
  fileSpec[SEQ].line['P'] = vgpDefineLine (0, 0, 0, 0, 0, 0) ; /* start of a pair */
  fileSpec[SEQ].line['W'] = vgpDefineLine (INT, INT, INT, REAL, 0, 0) ; /* well pulseStart pulseEnd score */
  fileSpec[SEQ].line['N'] = vgpDefineLine (REAL, REAL, REAL, REAL, 0, 0) ; /* SNR in A,C,G,T channels */
  fileSpec[SEQ].line['A'] = vgpDefineLine (STRING, 0, 0, 0, 0, 0) ; /* capped pulse widths 1-4 */

  fileSpec[RMP].objectType = 'R' ;
  fileSpec[RMP].line['r'] = vgpDefineLine (INT, STRING_LIST, 0, 0, 0, 0) ; /* number restriction_sites */
  fileSpec[RMP].line['R'] = vgpDefineLine (INT, INT_LIST, 0, 0, 0, 0) ; /* len locations(bp) */
  fileSpec[RMP].line['E'] = vgpDefineLine (INT_LIST, 0, 0, 0, 0, 0) ; /* sites in list in r line */
  fileSpec[RMP].line['I'] = vgpDefineLine (REAL_LIST, 0, 0, 0, 0, 0) ; /* intensities at each site */
  fileSpec[RMP].line['N'] = vgpDefineLine (REAL_LIST, 0, 0, 0, 0, 0) ; /* SNR_values at each sites */
  fileSpec[RMP].line['O'] = vgpDefineLine (INT, 0, 0, 0, 0, 0) ; /* object number in referred sequence file */

  fileSpec[ALN].objectType = 'A' ;
  fileSpec[ALN].line['A'] = vgpDefineLine (INT, INT, 0, 0, 0, 0) ; /* object numbers of aligned objects */
  fileSpec[ALN].line['I'] = vgpDefineLine (INT, INT, INT, INT, INT, INT) ; /* as ae alen bs be blen */
  fileSpec[ALN].line['Q'] = vgpDefineLine (INT, 0, 0, 0, 0, 0) ; /* quality in phred units */
  fileSpec[ALN].line['M'] = vgpDefineLine (INT, 0, 0, 0, 0, 0) ; /* number of matching bases */
  fileSpec[ALN].line['D'] = vgpDefineLine (INT, 0, 0, 0, 0, 0) ; /* number of differences = substitutions + indel bases */
  fileSpec[ALN].line['C'] = vgpDefineLine (STRING, 0, 0, 0, 0, 0) ; /* cigar string */
  fileSpec[ALN].line['T'] = vgpDefineLine (INT, 0, 0, 0, 0, 0) ; /* tracePoint spacing in a (global) */
  fileSpec[ALN].line['U'] = vgpDefineLine (INT_LIST, 0, 0, 0, 0, 0) ; /* tracePoints in a */
  fileSpec[ALN].line['V'] = vgpDefineLine (INT_LIST, 0, 0, 0, 0, 0) ; /* tracePoints in b */
  fileSpec[ALN].line['W'] = vgpDefineLine (INT_LIST, 0, 0, 0, 0, 0) ; /* tracePoint spacings in b */
  fileSpec[ALN].line['X'] = vgpDefineLine (INT_LIST, 0, 0, 0, 0, 0) ; /* inter-tracePoint diff counts in b */
  fileSpec[ALN].line['g'] = vgpDefineLine (INT, STRING, 0, 0, 0, 0) ; /* group number name */

  fileSpec[JNS].objectType = 'J' ;
  fileSpec[JNS].line['J'] = vgpDefineLine (INT, INT, CHAR, INT, INT, CHAR) ; /* a pos_a [s|e] b pos_b [s|e] */
  fileSpec[JNS].line['G'] = vgpDefineLine (INT, INT, 0, 0, 0, 0) ; /* mean and standard deviation of estimated gap size */
  fileSpec[JNS].line['Q'] = vgpDefineLine (INT, 0, 0, 0, 0, 0) ; /* confidence in phred units */
  fileSpec[JNS].line['X'] = vgpDefineLine (INT_LIST, 0, 0, 0, 0, 0) ; /* alignment objects supporting join */
  
  fileSpec[BRK].objectType = 'B' ;
  fileSpec[BRK].line['B'] = vgpDefineLine (INT, INT, INT, 0, 0, 0) ; /* object start end */
  fileSpec[BRK].line['Q'] = vgpDefineLine (INT, 0, 0, 0, 0, 0) ; /* confidence in phred units */
  fileSpec[BRK].line['X'] = vgpDefineLine (INT_LIST, 0, 0, 0, 0, 0) ; /* alignment objects supporting join */
  
  fileSpec[LIS].objectType = 'L' ;
  fileSpec[LIS].line['L'] = vgpDefineLine (INT_LIST, 0, 0, 0, 0, 0) ; /* object identifiers */
  fileSpec[LIS].line['N'] = vgpDefineLine (STRING, 0, 0, 0, 0, 0) ; /* optional name for list */
  fileSpec[LIS].line['S'] = vgpDefineLine (INT, 0, 0, 0, 0, 0) ; /* seed sequence for scaffold */

  /* fill in the listByteSize information */
  int listSize[8] = { 0, 0, 0, 0, 1, sizeof(I64), sizeof(double), 1 } ;
  for (i = 0 ; i < MAX_FILE ; ++i)
    for (j = 0 ; j < 128 ; ++j)
      if (fileSpec[i].line[j])
	for (k = 0 ; k < MAX_FIELD ; k++)
	  if (listSize[fileSpec[i].line[j]->field[k]])
	    { if (fileSpec[i].line[j]->listByteSize && j >= 'A') /* header records are shared */
		die ("VGP format %s spec error: two list types in record %c", fileTypeName[i], j) ;
	      else
		fileSpec[i].line[j]->listByteSize = listSize[fileSpec[i].line[j]->field[k]] ;
	    }
  return fileSpec ;
}

/********* end of file ***********/