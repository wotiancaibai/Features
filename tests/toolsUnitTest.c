
#include "stdio.h"
#include "stdlib.h"
#include "CUnit/Basic.h"

#include "tools.h"
#include "image.h"
#include "feature.h"

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void fmax3_test() {
    CU_ASSERT_DOUBLE_EQUAL(fmax3(4, 5, 9), 9, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(fmax3(76, 52, -9), 76, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(fmax3(-49.2, -52, -98.234), -49.2, 0.00001);
}

void fmin3_test() {
    CU_ASSERT_DOUBLE_EQUAL(fmin3(4, 5, 9), 4, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(fmin3(76, 52, -9), -9, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(fmin3(-49.2, -52, -98.234), -98.234, 0.00001);
}

void amax_test() {
    float array1[] = {234, 238, -24, -987, 456};
    float array2[] = {235, 222,  23,  987, 234, 670};
    
    CU_ASSERT_DOUBLE_EQUAL(amax(array1, 5), 456, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(amax(array2, 6), 987, 0.00001);
}

void amin_test() {
    float array1[] = {234, 238, -24, -987, 456};
    float array2[] = {235, 222,  23,  987, 234, 670};
    
    CU_ASSERT_DOUBLE_EQUAL(amin(array1, 5), -987, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(amin(array2, 6), 23, 0.00001);
}

void amean_test() {
    float array1[] = {234, 238, -24, -987, 456};
    float array2[] = {235, 222, 23, 987, 234, 670};
    
    CU_ASSERT_DOUBLE_EQUAL(amean(array1, 5), -16.6, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(amean(array2, 6), 395.166666, 0.00001);
}

void sort_test() { // fsort & isort
    int array1[] = {234, 238, -24, -987, 456};
    int array2[] = {235, 222, 23, 987, 234, 670};
    int index[6];
    
    float array3[] = {234, 238, -24, -987, 456};
    float array4[] = {235, 222, 23, 987, 234, 670};
    
    isort(array1, 0, 3);
    CU_ASSERT_EQUAL(array1[0], 238);
    CU_ASSERT_EQUAL(array1[3], -987);
    
    isort(array1, 0, 5);
    isort(array2, index, 6);
    
    CU_ASSERT_EQUAL(array1[0], 456);
    CU_ASSERT_EQUAL(array1[2], 234);
    CU_ASSERT_EQUAL(array1[4], -987);
    
    CU_ASSERT_EQUAL(index[0], 3);
    CU_ASSERT_EQUAL(index[3], 4);
    CU_ASSERT_EQUAL(index[5], 2);
    
    fsort(array3, 0, 3);
    CU_ASSERT_DOUBLE_EQUAL(array3[0], 238, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(array3[3], -987, 0.0001);
    
    fsort(array3, 0, 5);
    fsort(array4, index, 6);
    
    CU_ASSERT_DOUBLE_EQUAL(array3[0], 456, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(array3[2], 234, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(array3[4], -987, 0.0001);
    
    CU_ASSERT_EQUAL(index[0], 3);
    CU_ASSERT_EQUAL(index[3], 4);
    CU_ASSERT_EQUAL(index[5], 2);
}

void get_arc_length_test() {
    CU_ASSERT_DOUBLE_EQUAL(get_arc_length(0.3, 0.7), 0.4, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(get_arc_length(0.7, 0.3), 0.4, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(get_arc_length(0.9, 0.3), 0.4, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(get_arc_length(0.3, 0.9), 0.4, 0.0001);
}

void get_stdev_test() {
    float array1[] = {234, 238, -24, -987, 456};
    float array2[] = {235, 222, 23, 987, 234, 670};
    float a, b;
    a = get_stdev(array1, 5);
    b = get_stdev(array2, 6);
    CU_ASSERT_DOUBLE_EQUAL(a, 508.468917, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(b, 328.106394, 0.0001);
}

int main() {
    CU_pSuite pSuite = NULL;

    // Initialize the CUnit test registry 
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Add a suite to the registry 
    pSuite = CU_add_suite("Tools Unit Test", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the tests to the suite 
    if (
            (NULL == CU_add_test(pSuite, "fmax3_test", fmax3_test)) ||
            (NULL == CU_add_test(pSuite, "fmin3_test", fmin3_test)) || 
            (NULL == CU_add_test(pSuite, "amax_test", amax_test))   ||
            (NULL == CU_add_test(pSuite, "amin_test", amin_test))   ||
            (NULL == CU_add_test(pSuite, "amean_test", amean_test)) ||
            (NULL == CU_add_test(pSuite, "sort_test", sort_test))   ||
            (NULL == CU_add_test(pSuite, "get_arc_length_test", get_arc_length_test)) ||
            (NULL == CU_add_test(pSuite, "get_stdev_test", get_stdev_test)) 
       ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the CUnit Basic interface 
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
