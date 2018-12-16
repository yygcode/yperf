#include <check.h>

Suite* suite_atomic(void);
Suite* suite_atomic64(void);

int main()
{
    int nf;

    SRunner *sr = srunner_create(NULL);

    srunner_add_suite(sr, suite_atomic());
    srunner_add_suite(sr, suite_atomic64());

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    if (nf)
        return 1;

    return 0;
}
