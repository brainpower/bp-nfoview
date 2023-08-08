// SPDX-FileCopyrightText: 2010 - 2023 brainpower <brainpower@mailbox.org>
//
// SPDX-License-Identifier: GPL-2.0-or-later

/* This Code may resemble some other code,
 * cause I got the idea out of some example file
 * if you own such other code and you feel offended
 * you can contact me so we can talk about that matter...
 */

#include "qcodepage437codec.h"

QString QCodePage437Codec::convertToUnicode(const char *in, int length, ConverterState *) const {
	QString str;
	if ( length >= 6 && in[0] == '8' && in[1] == '0' &&
	     in[length - 4] == 'F' && in[length - 3] == 'F' &&
	     in[length - 2] == 'F' && in[length - 1] == 'F') {

		// UCS-2 string embedded within a 437-encoded string.
		int nibble = 0;
		int value = 0;
		int digit;
		in += 2;
		length -= 6;
		while ( length-- > 0 ) {
			char ch = *in++;
			if ( ch >= '0' && ch <= '9' )
				digit = ch - '0';
			else if ( ch >= 'A' && ch <= 'F' )
				digit = ch - 'A' + 10;
			else if ( ch >= 'a' && ch <= 'f' )
				digit = ch - 'a' + 10;
			else
				continue;
			value = value * 16 + digit;
			++nibble;
			if ( nibble >= 4 ) {
				str += QChar( (ushort)value );
				nibble = 0;
				value = 0;
			}
		}

	} else {

		// Regular 437-encoded string.
		while ( length-- > 0 )
			str += QChar((unsigned int)cp437ToUnicode[*in++ & 0xFF]);

	}
	return str;
}

QByteArray QCodePage437Codec::convertFromUnicode(const QChar *in, int length, ConverterState *) const {
	QByteArray result;
	unsigned int ch;
	char *out;

	// Determine if the string should be encoded using the UCS-2 hack.
	bool non437 = false;
	for ( int posn = 0; !non437 && posn < length; ++posn ) {
		ch = in[posn].unicode();
		if ( ch >= 0x0100 )
			non437 = true;
		else if ( cp437FromUnicode[ch] == '?' && ch != '?' )
			non437 = true;
	}
	if ( non437 ) {
		// There is a non CP437 character in this string, so use UCS-2.
		result.resize( length * 4 + 6 );
		out = result.data();
		*out++ = '8';
		*out++ = '0';
		while ( length-- > 0 ) {
			auto chu = in->unicode();
			++in;
			*out++ = hexchars[ (chu >> 12) & 0x0F ];
			*out++ = hexchars[ (chu >> 8) & 0x0F ];
			*out++ = hexchars[ (chu >> 4) & 0x0F ];
			*out++ = hexchars[ chu & 0x0F ];
		}
		*out++ = 'F';
		*out++ = 'F';
		*out++ = 'F';
		*out   = 'F';
		return result;
	}

	// If we get here, we can guarantee that the string only contains
	// valid CP437 code points between 0x0000 and 0x00FF.
	result.resize( length );
	out = result.data();
	while ( length-- > 0 ) {
		*out++ = (char)cp437FromUnicode[in->unicode()];
		++in;
	}
	return result;
}


QByteArray QCodePage437Codec::name() const {
	return "IBM437";
}

QList<QByteArray> QCodePage437Codec::aliases() const {
	QList<QByteArray> list;
	list << "CP437" << "cp437" << "437" << "csPC8CodePage437";
	return list;
}

int QCodePage437Codec::mibEnum() const {
	return 2011;
	// source: https://www.iana.org/assignments/character-sets/character-sets.xml
}
