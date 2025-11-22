#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int check_reversibility(long long *A, int N, long long S);

int run_test(long long *A, int N, long long S, int expected, int test_no) {
    int out = check_reversibility(A, N, S);
    if (out == expected) {
        printf("Test %d: PASS\n", test_no);
        return 0;
    } else {
        printf("Test %d: FAIL (expected %d, got %d)\n", test_no, expected, out);
        return 1;
    }
}

int main() {
    int failures = 0;

    long long t1[] = {5,1}; failures += run_test(t1,2,2,0,1);
    long long t2[] = {1,2}; failures += run_test(t2,2,10,0,2);
    long long t3[] = {2,3}; failures += run_test(t3,2,100,1,3);
    long long t4[] = {1,3,5}; failures += run_test(t4,3,10,1,4);
    long long t5[] = {1,2,10,11}; failures += run_test(t5,4,5,0,5);
    long long t6[] = {1,2,3,4,5}; failures += run_test(t6,5,1000,1,6);
    long long t7[] = {1,2,3}; failures += run_test(t7,3,3,0,7);
    long long t8[] = {1,4,2,5,6}; failures += run_test(t8,5,20,0,8);
    long long t9[] = {10,20}; failures += run_test(t9,2,500,1,9);
    long long t10[] = {1,2,4,5,6}; failures += run_test(t10,5,20,1,10);
    long long t11[] = {3,4,9,10,11,2}; failures += run_test(t11,6,30,1,11);
    long long t12[] = {7,14}; failures += run_test(t12,2,100,1,12);
    long long t13[] = {2,5,6,20,30}; failures += run_test(t13,5,40,0,13);

    if (failures == 0) {
        printf("ALL TESTS PASS\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED\n", failures);
        return 1;
    }
}
