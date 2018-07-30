/*
va-arg-14.c from the execute part of the gcc torture tests.
*/

#include <testfwk.h>

#include <stdarg.h>

va_list global;

void vat(va_list param, ...)
{
  va_list local;

  va_start (local, param);
  va_copy (global, local);
  va_copy (param, local);
  if (va_arg (local, int) != 1)
    ASSERT(0);
  va_end (local);
  if (va_arg (global, int) != 1)
    ASSERT(0);
  va_end (global);
  if (va_arg (param, int) != 1)
    ASSERT(0);
  va_end (param);

  va_start (param, param);
  va_start (global, param);
  va_copy (local, param);
  if (va_arg (local, int) != 1)
    ASSERT(0);
  va_end (local);
  va_copy (local, global);
  if (va_arg (local, int) != 1)
    ASSERT(0);
  va_end (local);
  if (va_arg (global, int) != 1)
    ASSERT(0);
  va_end (global);
  if (va_arg (param, int) != 1)
    ASSERT(0);
  va_end (param);
}

void
testTortureExecute (void)
{
  va_list t;
  vat (t, 1);
  return;
}
