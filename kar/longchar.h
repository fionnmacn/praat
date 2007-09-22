#ifndef _longchar_h_
#define _longchar_h_
/* longchar.h
 *
 * Copyright (C) 1992-2007 Paul Boersma
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * pb 2002/03/07 GPL
 * pb 2002/03/11 nativize & genericize are each other's inverse and their second application is identity
 * pb 2004/12/02 differentiate between X11 and Windows encodings
 * pb 2005/03/08 added "PostScript" encodings (mostly Mac and Praat-IPA)
 * pb 2006/11/17 Unicode
 * pb 2006/12/05 first wchar support
 * pb 2007/08/08 more wchar_t support
 * pb 2007/08/16 removed New Century Schoolbook
 */
#include <wchar.h>

/********** NON-ASCII CHARACTERS **********/

/* System-independent representation of some non-ASCII symbols.
	We need this because Praat is multilingual, and not all input methods are available on all international systems.
	These symbols are represented by a backslash (\) plus two ASCII symbols.

	- ASCII would suffice for English.
	- By supporting ISO8859-1, we support the Roman alphabets of Dutch, German, French, Spanish, Portuguese, Italian,
	  Danish, Swedish, Norwegian, Welsh, Rumanian, Luxemburgian, Frisian.
	- We will soon also support the Roman alphabets of Hungarian, Polish, Czech, Icelandic, Serbocroat, Turkish.

	The following symbols are in the Roman alphabet:
		vowel + dieresis: \a" \e" \i" \o" \u" \y"
		vowel + grave: \a` \e` \i` \o` \u`
		vowel + circumflex: \a^ \e^ \i^ \o^ \u^
		vowel + acute: \a' \e' \i' \o' \u' \y'
		letter + tilde: \n~ \a~ \o~
		c + cedilla: \c,
		German double s: \ss
		ligatures: \ae
		o + slash: \o/
		a + ring: \ao
		thorn: \th
	These symbols will look right on all systems (most fonts).
*/

/* Alphabets. */

#define Longchar_ROMAN  0
#define Longchar_SYMBOL  1
#define Longchar_PHONETIC  2
#define Longchar_DINGBATS  3

/********** Conversion of Roman native and generic string encodings. **********/

char * Longchar_nativize (const char *generic, char *native, int educateQuotes);
/*
	Copies the string 'generic' to the string 'native',
	translating backslash trigraphs into natively encoded characters,
	if they are in the Roman alphabet.
	For instance, \n~ will be translated into an "ntilde",
	but the trigraphs \al, \as, and \pf will not be translated,
	because they are in the Symbol, Phonetic, and Dingbats alphabets, respectively.
	Returns 'native' as a convenience.
	'native' will never become longer than 'generic'.
	Usage:
		translating a generic string into a format that your system can put
		into one-font widgets such as menu items, labels, and push buttons.
		For drawing text with mixed alphabets and mixed styles,
		use Graphics_text instead.
*/
wchar_t * Longchar_nativizeW (const wchar_t *generic, wchar_t *native, int educateQuotes);

char * Longchar_genericize (const char *native, char *generic);
wchar_t *Longchar_genericizeW (const wchar_t *native, wchar_t *generic);
/*
	Function:
		Copies the string 'native' to the string 'generic',
		interpreting non-ASCII native characters as belonging to the Roman alphabet.
	Returns 'generic' as a convenience.
	'generic' can become at most three times as long as 'native'.
	Usage:
		translating user input into a generic string.
*/

typedef struct Longchar_Info {
	unsigned char first, second;   /* First and second character of two-byte encoding. */
		/* For ASCII characters, 'second' is a space. */
	unsigned char alphabet;   /* Roman, Symbol, Phonetic, or Dingbats. */
	struct {
		const char *name;   /* The PostScript name, starting with a slash. */
		/* The widths in thousands of the height. */
		short times, timesBold, timesItalic, timesBoldItalic;   /* Times. */
		short helvetica, helveticaBold;   /* Helvetica. */
		short palatino, palatinoBold, palatinoItalic, palatinoBoldItalic;   /* Palatino. */
		/* Courier width always 600. */
	}
		ps;   /* PostScript properties. */
	unsigned short xwinEncoding;   /* The one-byte encoding for X11 (ISO8859-1 for Roman). */
	unsigned short winEncoding;   /* The one-byte encoding for Windows (ISO8859-1 for Roman; SILDoulosIPA 1993). */
	unsigned short macEncoding;   /* The one-byte encoding for Macintosh (Mac for Roman; SILDoulosIPA 1993). */
	unsigned short psEncoding;   /* The one-byte encoding for PostScript (Mac-Praat, TeX-xipa-Praat). */
	unsigned long unicode;   /* The four-byte encoding for Unicode. */
	unsigned short unicodeDecomposition [6];   /* Diacritics decoupled from the base character. */
}
	*Longchar_Info;

Longchar_Info Longchar_getInfo (char kar1, char kar2);
Longchar_Info Longchar_getInfoFromNative (wchar_t kar);
/* If no info found, these two routines return the info for a space. */

/*
	How should we represent the dumb ASCII double quote (")?
	The dumb quote is converted to a left or right double quote,
	on Macintosh, PostScript, and Windows
	(ISO8859-1 has no left and right double quotes).
	You can enforce the way your double quotes look by
	using one of these generic symbols:
		- \"l for a left double quote
		- \"r for a right double quote
		- \"" for a straight double quote
	There is no translation for single quotes, because some languages
	use the right single quote as an apostrophe in unpredictable positions.
	You can get a left single quote by typing "`",
	which looks like a grave accent on Macintosh;
	you will get a right single quote or apostrophe by typing a "'",
	which looks like a straight quote on Macintosh.
	Thus, the string typed as "`hallo'" will give you left and right quotes,
	even on Macintosh. (Reading this note in Xwindows may feel somewhat funny.)
*/

/* End of file longchar.h */
#endif