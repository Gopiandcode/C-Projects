#include "atom.h"
#include <string.h>
#include "assert.h"
#include <limits.h>

#define NELEMS(x) ((sizeof (x))/(sizeof ((x)[0])))

static struct atom {
	struct atom *link;
	int len;
	char *str;
} *buckets[2048];

const char *Atom_string(const char *str) {
	assert(str);
	return Atom_new(str, strlen(str));
}

static unsigned long scatter[] = {1038692610, 494030272, 156148809, 1446006368, 531166888, 222751637, 1725299818, 821574850, 
1034174141, 485577328, 2007723474, 2057293960, 340694245, 1610670574, 197715461, 1117919410, 1120085956, 823472205, 438495035, 
1459189773, 2020687004, 482027932, 715383196, 872930622, 1672881076, 1957106460, 539964656, 244336778, 430703608, 1391275483, 
1351522641, 1469396218, 1885305756, 1507671450, 767918938, 268988996, 1730423087, 345735108, 1090563847, 617113580, 831312436, 
950803673, 526923892, 1172006681, 413990599, 724639354, 142442443, 1534076555, 1548111559, 580937479, 845782681, 1421314915, 
1062965411, 1561165877, 146761889, 588362839, 1370788689, 686726546, 832699618, 1801492297, 2078002029, 36738611, 1123404867, 
1815824137, 1544410061, 1891323805, 2084813134, 1127349500, 89575266, 1027893333, 1744463080, 920887702, 1978697006, 123903324, 
2092894384, 245203957, 848542678, 87853179, 1779280513, 249170589, 668790658, 477579546, 1670485504, 1731756069, 2038745423, 
1817247394, 172635261, 1262050465, 356490292, 1005334879, 916059114, 287008673, 1042073490, 2039463982, 2102832811, 438999903, 
1783304139, 2040162297, 1566349403, 1872879405, 920571982, 1163328835, 646283460, 751785340, 1287232159, 591694196, 996989297, 
2135774838, 679547375, 628786162, 237461779, 1348338034, 1106365708, 1907947284, 932610455, 997627484, 1577711030, 1105245716, 
112194301, 1934201322, 2110580595, 1028253415, 73726347, 1005170437, 920233749, 29075510, 1444170340, 556054241, 2069237807, 
863036095, 281449998, 842326141, 2026364930, 927733458, 1594111481, 1166113442, 1519427654, 443617131, 1154404632, 51491382, 
1072403293, 1391866411, 1399829416, 31285354, 1152330047, 184956223, 1028912838, 582557429, 1290201940, 1141107139, 369275103, 
1253298887, 21876906, 443001451, 110985677, 942110656, 472076961, 1555156017, 1498164897, 393831121, 270708465, 1779614895, 
1236157262, 149589747, 559864706, 682785096, 1315703189, 2079292360, 1126402227, 322624173, 2130783742, 51321872, 1714490585, 
1383129510, 82607226, 719336984, 1568085734, 1111520064, 1301894414, 710804026, 105143555, 1671169517, 1964102913, 127020462, 
2114170968, 2075088590, 1069131118, 438764282, 1482760960, 419812367, 832595403, 1753469425, 51943614, 2068752665, 1903059172, 
611808320, 604054113, 1071278714, 543617033, 1730456340, 1393902887, 526917127, 1781778213, 960909824, 1910046638, 1864385439, 
1680246809, 1330648724, 828421856, 834657575, 2041452750, 933565411, 358343444, 1858072015, 1060585873, 325030765, 1785676958, 
2129716991, 763795047, 1120954270, 402045710, 1596390450, 726940047, 453989325, 1517659467, 482515571, 1065797645, 2121713581, 
1553794285, 1609414678, 1704686273, 800213525, 2136331806, 1338980838, 1761123349, 1898894796, 1055882630, 1293886510, 
1082059872, 1884304486, 2128544085, 976028974, 670386249, 339403882, 686617341, 1730972123, 664434647, 324810651, 1713205466, 
1428229694, 1445764921, 2115251177, 877136496, 25221320, 421756854, 247312315, 507736892};

const char *Atom_int(long n) {
	char str[43];
	char *s = str = str + sizeof str;
	unsigned long m;

	if(n == LONG_MIN)
		m = LONG_MAX + 1UL;
	else if(n < 0)
		m = -n;
	else
		m = n;

	do
		*--s = m%10 + '0';
	while((m /= 10) > 0);
	if(n < 0)
		*--s = '-';
	return Atom_new(s, (str +sizeof str) - s);
}

const char *Atom_new(const char *str, int len) {
	unsigned long h;
	int i;
	struct atom *p;

	assert(str);
	assert(len >= 0);

	for(h = 0, i = 0; i < len; i++)
		h = (h << 1) + scatter[(unsigned char) str[i]];

	h %= NELEMS(buckets);
	for(p = buckets[h]; p; p = p->link)
		if(len == p->len) {
			for(i = 0; i < len && p->str[i] == str[i];)
				i++;
			if(i == len)
				return p->str;
		}

	p = ALLOC(sizeof (*p) + len + 1);
	p->len = len;
	p->str = (char *)(p+1);
	if(len > 0)
		memcpy(p->str, str, len);
	p->str[len] = '\0';
	p->link = buckets[h];
	buckets[h] = p;

	return p->str;
}

int Atom_length(const char *str) {
       struct atom *p;
	int i;
 	
	assert(str);
	for(i = 0; i<NELEMS(buckets); i++) 
		if(p->str == str)
			return p->len;
	assert(0);
	return 0;	
}


