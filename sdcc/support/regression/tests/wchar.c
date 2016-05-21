/* Tests wide character conversion functions.
 */
#include <testfwk.h>

#include <stdlib.h>
#include <limits.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409L
#include <wchar.h>
#endif
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#include <uchar.h>
#endif

static void
testwcharnorestart(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409L && !(defined(__SDCC_mcs51) && defined(__SDCC_MODEL_SMALL))
	wchar_t w;
	char c[MB_LEN_MAX];

	c[0] = 'C';
	ASSERT(mbtowc(&w, c, 1) == 1);
	ASSERT(wctomb(c, w) == 1);
	ASSERT(c[0] == 'C');
#endif
}

static void
testwcharrestart(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !(defined(__SDCC_mcs51) && defined(__SDCC_MODEL_SMALL))
	static mbstate_t ps;
	wchar_t w;
	char c[MB_LEN_MAX];

	c[0] = 'C';
	ASSERT(mbrtowc(&w, c, 1, &ps) == 1);
	ASSERT(wcrtomb(c, w, &ps) == 1);
	ASSERT(c[0] == 'C');
#endif
}

static void
testchar16restart(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !(defined(__SDCC_mcs51) && defined(__SDCC_MODEL_SMALL))
	static mbstate_t ps;
	char16_t c16[2];
	char c[MB_LEN_MAX];

	c[0] = 'C';
	ASSERT(mbrtoc16(c16, c, 1, &ps) == 1);
	ASSERT(c16rtomb(c, c16[0], &ps) == 1);
	ASSERT(c[0] == 'C');
#endif
}

static void
testchar32restart(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !(defined(__SDCC_mcs51) && defined(__SDCC_MODEL_SMALL))
	static mbstate_t ps;
	char32_t c32[2];
	char c[MB_LEN_MAX];

	c[0] = 'C';
	ASSERT(mbrtoc32(c32, c, 1, &ps) == 1);
	ASSERT(c32rtomb(c, c32[0], &ps) == 1);
	ASSERT(c[0] == 'C');
#endif
}

