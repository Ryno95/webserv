#include <criterion/criterion.h>

Test(SimpleTest, Basic)
{
    int a = 2, b = 2;
    cr_expect(a + b == 4);
}