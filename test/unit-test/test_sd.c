// SPDX-License-Identifier: Apache-2.0

#include <sd.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <cmocka.h>

static void test_sd_list_subdir_long_path(void** state)
{
    (void)state;

    assert_true(sd_format());

    sd_list_t list = {0};
    char long_dir[600] = {0};
    memset(long_dir, 'a', sizeof(long_dir) - 1);

    assert_false(sd_list_subdir(&list, long_dir));
    assert_null(list.files);
    assert_int_equal(list.num_files, 0);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sd_list_subdir_long_path),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
