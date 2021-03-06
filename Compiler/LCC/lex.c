#include "lex.h"
#include "input.h"
#include "string.h"
#include "sym.h"

static struct symbol tval;

enum {
#define xx(a,b,c,d,e,f,g) a=b,
#define yy(a,b,c,d,e,f,g)
#include "token.h"
	LAST
};

enum {
	BLANK=01, NEWLINE=02, LETTER=04,
	DIGIT=010, HEX=020, OTHER=040 };


static unsigned char map[256] = {
	// map initializer
};

// deals with preprocessing number
static void ppnumber(which) char *which; {
	unsigned char *rcp = cp--;
	for(; (map[*cp]&(DIGIT|LETTER)) || *cp == '.'; cp++)
		if((cp[0] == 'E'||cp[0] == 'e')
		&& (cp[1] == '-' || cp[1] == '+'))
			cp++;
	if(cp > rcp)
		error("'%S' is a preprocessing number but an _ invalid %s constant\n", token, (char *)cp-token, which);

}

static Symbol icon(n, overflow, base)
	unsigned n; int overflow, base; {
	if((*cp=='u'||*cp=='U') && (cp[1] == 'l' || cp[1] == 'L')
	|| (*cp=='l' || *cp=='L') && (cp[1] == 'u' || cp[1] =='U')) {
	tval.type = unsignedlong;
	cp += 2;
	} else if(*cp == 'u' || *cp == 'U') {
	tval.type = unsignedtype;
	cp+=1;
	} else if(*cp =='l'||*cp=='L') {
		if(n > (unsigned)LONG_MAX)
			tval.type = unsignedlong;
		else
			tval.type=longtype;
		cp += 1;
	} else if(base == 10 && n > (unsigned)LONG_MAX)
		tval.type = unsignedlong;
	else if(n > (unsigned)INT_MAX)
		tval.type=unsignedtype;
	else
		tval.type = inttype;
	if(overflow) {
		warning("overflow in constant '%S'\n", token,
				(char *)cp - token);
		n = LONG_MAX;
	}
	// set tval's position
	if(isunsigned(tval.type))
	tval.u.c.v.u = n;
	else
	tval.u.c.v.i = n;	
	ppnumber("integer");
	return &tval;
	}


static Symbol fcon() {
	// scan past floating constant
	if(*cp == '.')
		// scan past digits
		do
			cp++;
		while(map[*cp]&DIGIT);
	if(*cp == 'e' || *cp == 'E') {
		if(*++cp=='-'||*cp=='+')
			cp++;
		if(map[*cp]&DIGIT)
			// scan past a run of digits
			do
				cp++;
			while(map[*cp]&DIGIT);
		else
			error("invalid floating constant '%S'\n", token, (char *)cp-token);
	}
	errno = 0;
	tval.u.c.v.d = strtod(token,NULL);
	if(errno == ERANGE)
		// warn about overflow
		warning("overflow in floating constant '%S'\n", token, (char *)cp-token);
	// set tvals type and value
	if(*cp == 'f' || *cp == 'F') {
		++cp;
		if(tval.u.c.v.d > FLT_MAX)
			warning("overflow in floating constant '%S'\n", token, (char *)cp - token);
		tval.type = floattype;
		tval.u.c.v.f = tval.u.c.v.d;
	} else if(*cp == 'l' || *cp == 'L') {
		cp++;
		tval.type = longdouble;
	} else {
		tval.type = doubletype;
	}
	ppnumber("floating");
	return &tval;
}

int gettok() {

	for(;;) {
		register unsigned char *rcp = cp;
		// skip whitespace
		while(map[*rcp]&BLANK)
			rcp++;
		// if less than maxtoken elements, fill buffer again
		if(limit - rcp < MAXTOKEN) {
			// move cp to rcp
			cp = rcp;
			// fill buf
			fillbuf();
			rcp = cp;
		}
		src.file = file;
		src.x = (char *)rcp - line;
		src.y = lineno;
		// move cp to the next char past rcp
		cp = rcp + 1;
		// rcp will equal cp, switch value is cp-1
		switch(*rcp++){
	// gettok cases
		case '\n': case '\v': case'\r': case'\f':
			nextline();
			if(cp == limit) {
				tsym = NULL;
				return EOI;
			}
			continue;
		case 'i':
			// if
			if(rcp[0] == 'f'
			&& !(map[rcp[1]]&(DIGIT|LETTER))){
			cp = rcp+1;
			return IF;
			}
			//int
			if(rcp[0] == 'n'
			&& rcp[1] == 't'
			&& !(map[rcp[2]]&(DIGIT|LETTER))){
				cp = rcp + 2;
				tsym =  inttype->u.sym;
				return INT;
			}
			// otherwise identiifier
			goto id;
		case 'h': case 'j': case 'k': case 'm': case 'n':
		case 'o': case 'p': case 'q': case 'x': case 'y':
		case 'z': case 'A': case 'B': case 'C': case 'D':
		case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'M': case 'N': case 'O':
		case 'P': case 'Q': case 'R': case 'S': case 'T':
		case 'U': case 'V': case 'W': case 'X': case 'Y':
		case 'Z': case '_':
		id:
			// ensure there are atleast maxline chars
			if(limit - rcp < MAXLINE) {
				cp = rcp -1;
				fillbuf();
				rcp = ++cp;
			}
			// rcp is 1 more than switch value
			token = (char *)rcp-1;
			// while identifier move forward
			while(map[*rcp]&(DIGIT|LETTER))
				rcp++;
			// convert to permanent string
			token = stringn(token, (char *)rcp-token);
			//tsym <- type named by token
			tsym = lookup(token, identifiers);
			cp = rcp;
			return ID;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':{
	unsigned int n = 0;
	// ensure there are atleast maxline chars
	if(limit - rcp < MAXLINE) {
			cp = rcp -1;
			fillbuf();
			rcp = ++cp;
		}
	token = (char *)rcp - 1;
	if(*token == '0' && (*rcp == 'x' || *rcp == 'X')) {
		// hexidecimal constant	
	} else if(*token == '0') {
		// octal constant
	} else {
		//decimalconstant
		int overflow = 0;
		// while is a digit
		for(n = *token - '0'; map[*rcp]&DIGIT;) {
			// if n > 
			int d = *rcp++ - '0';
			// if adding it results in a overflow
			if(n > ((unsigned)UINT_MAX - d)/10)
				overflow = 1;
			else
				n = 10*n + d;
		}
		// check for floating constant
		if(*rcp == '.' || *rcp == 'e' || *rcp == 'E') {
			cp = rcp;
			tsym = fcon();
			return FCON;
		}
		cp = rcp;
		tsym = icon(n, overflow, 10);
	}
	return ICON;
								 }
	case '.':
	if(rcp[0] == '.' && rcp[1] == '.') {
	       cp += 2;
	       return ELLIPSES;
	}
	if((map[*rcp]&DIGIT) == 0)
		return '.';
	// ensure that there are at least characters
	if(limit - rcp < MAXLINE) {
		cp = rcp -1;
		fillbuf();
		rcp = ++cp;
	}
	cp = rcp-1;
	token = (char *)cp;
	tsym = fcon();
	return FCON;
	return ELLIPSES;       
scon:
	case '\'': case '"': {
		static char cbuf[BUFSIZE+1];
		char *c = cbuf;
		int nbad = 0;
		*s++ = *--cp;
		do {
			cp++;
			// scan one string literal
			while(*cp != cbuf[0]) {
				int c;
				if(map[*cp]&NEWLINE) {
				if(cp < limit)
					break;
				cp++;
				nextline();
				if(cp == limit)
					break;
				continue;
				}
				c = *cp++;
				if(c == '\\') {
					if(map[*cp]&NEWLINE){
						if(cp < limit)
							break;
					cp++;
					nextline();
					}
					if(limit - cp < MAXTOKEN)
						fillbuf();
					c = backslash(cbuf[0]);
				} else if(map[c] == 0)
					nbad++;
				if(s < &cbuf[sizeof cbuf] - 2)
					*s++ - c;
			}
			if(*cp == cbuf[0])
				cp++;
			else
				error("missing %c\n",cbuf[0]);
		} while(cbuf[0] == '"' && getchr() == '"');
		*s++ - 0;
		if(s >= &cbuf[sizeof cbuf])
			error("%s literal too long\n",
					cbuf[0] == '"' ? "string": "character");
		// warn about nonansi literals
		// set tval and return ICON SCON
		token = cbuf;
		tsym = &tval;
		if(cbuf[0] == '"') {
			tval.type = array(chartype, s-cbuf-1,0);
			tval.u.c.v.p = cbuf + 1;
			return SCON;
		} else {
			if(s - cbuf >3)
				warning("excess characters in multibyte character _ literal '%S' ignored\n", token, (char*)cp-token);
			else if(s - cbuf <= 2)
				error("missing \n");
			tval.type = inttype;
			tval.u.c.v.i = cbuf[1];
			return ICON;
		}
		}
			     }
		default:
			if((map[cp[-1]]&BLANK) == 0)
				// illegal character


		}
	}
}

}


