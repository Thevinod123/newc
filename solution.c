#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static int GLOBAL_FOUND = 0;

#define MAX_L_LIMIT 200
#define WINDOW_RADIUS 3
#define SLOPE_TOL 4   /* slope tolerance */

/* --------------------------------------------
   ENUMERATION WITH WINDOWS + SHAPE CONSTRAINT
   -------------------------------------------- */

static int count_sequences_fixed_L_with_windows(
    long long target_score,
    long long S,
    int L,
    int limit,
    long long start_min, long long start_max,
    long long end_min, long long end_max,
    long long A_i, long long A_j,
    int orig_len
) {
    int K = L - 1;
    long long base_min = (long long)L*(L-1)/2;
    long long max_a1 = (S - base_min) / L;
    if (max_a1 < 1) return 0;
    if (max_a1 > S) max_a1 = S;

    long long a1_low = start_min, a1_high = start_max;
    if (a1_low < 1) a1_low = 1;
    if (a1_high > max_a1) a1_high = max_a1;
    if (a1_low > a1_high) return 0;

    long long coeff[256];
    for (int i=1;i<=K;i++) coeff[i] = (L - i);

    typedef struct { int idx; long long rem; long long sumx; long long extr; } Frame;

    for (long long a1 = a1_low; a1 <= a1_high; ++a1) {
        long long base_sum = (long long)L*a1 + base_min;
        if (base_sum > S) break;
        long long max_extra = S - base_sum;

        Frame st[4096];
        int sp = 0;
        st[sp++] = (Frame){1, max_extra, 0LL, 0LL};

        while (sp>0) {
            Frame f = st[--sp];

            if (f.idx > K) {
                long long total_sum = base_sum + f.sumx;
                if (total_sum <= S) {
                    long long D = K + f.extr;
                    long long aL = a1 + D;
                    long long score = D + (total_sum % 7);

                    if (score == target_score) {

                        /* END WINDOW CHECK */
                        if (aL < end_min || aL > end_max) continue;

                        /* SHAPE CONSISTENCY */
                        long long slope_left = (aL - a1)*(orig_len-1);
                        long long slope_right = (A_j - A_i)*(L-1);
                        if ( llabs(slope_left - slope_right) > SLOPE_TOL ) continue;

                        /* LENGTH CONSISTENCY */
                        if ( llabs(L - orig_len) > 1 ) continue;

                        /* LONGEST RULE: cannot extend */
                        long long next_val = aL + 1;
                        if (total_sum + next_val <= S) continue;

                        GLOBAL_FOUND++;
                        if (GLOBAL_FOUND >= limit) return GLOBAL_FOUND;
                    }
                }
                continue;
            }

            long long c = coeff[f.idx];
            long long max_e = (c==0?0:(f.rem/c));
            if (max_e > 200) max_e = 200;

            for (long long val=0; val<=max_e; ++val) {
                if (sp < 4095)
                    st[sp++] = (Frame){f.idx+1, f.rem - val*c, f.sumx + val*c, f.extr + val};
            }
        }
    }
    return GLOBAL_FOUND;
}

static int count_theoretical_segments(
    long long target, long long S, int limit,
    long long start_min, long long start_max,
    long long end_min, long long end_max,
    long long A_i, long long A_j,
    int orig_len
) {
    GLOBAL_FOUND = 0;

    int Lmax = 0;
    for (int L=2;;L++) {
        long long ms = (long long)L + (long long)L*(L-1)/2;
        if (ms > S) break;
        Lmax = L;
        if (L > MAX_L_LIMIT) break;
    }
    if (Lmax < 2) return 0;

    for (int L=2; L<=Lmax; L++) {
        long long base_min = (long long)L*(L-1)/2;
        long long max_a1 = (S - base_min)/L;
        if (max_a1 < 1) continue;

        int res = count_sequences_fixed_L_with_windows(
            target,S,L,limit,
            start_min,start_max,
            end_min,end_max,
            A_i,A_j,
            orig_len
        );
        if (res >= limit) return res;
    }
    return GLOBAL_FOUND;
}

/* --------------------------------------------
   PHASE 1: LEFTMOST-LONGEST COMPRESSION
   -------------------------------------------- */

int check_reversibility(long long *A, int N, long long S) {
    long long *C = malloc(sizeof(long long)*(2*N));
    int *isC = malloc(sizeof(int)*(2*N));
    int *Lidx = malloc(sizeof(int)*(2*N));
    int *Ridx = malloc(sizeof(int)*(2*N));
    int csz=0;

    int i=0;
    while (i<N) {
        long long sum=A[i], mn=A[i], mx=A[i];
        int best=-1;
        for (int j=i+1;j<N;j++){
            if (A[j-1]>=A[j]) break;
            sum+=A[j];
            if (sum>S) break;
            if (A[j]<mn) mn=A[j];
            if (A[j]>mx) mx=A[j];
            best=j;
        }
        if (best==-1){
            C[csz]=A[i];
            isC[csz]=0;
            Lidx[csz]=Ridx[csz]=i;
            csz++;
            i++;
        } else {
            long long seg_sum=0,seg_mn=mn,seg_mx=mx;
            for(int k=i;k<=best;k++){
                seg_sum+=A[k];
                if(A[k]<seg_mn) seg_mn=A[k];
                if(A[k]>seg_mx) seg_mx=A[k];
            }
            long long score=(seg_mx-seg_mn)+(seg_sum%7);
            C[csz]=score;
            isC[csz]=1;
            Lidx[csz]=i;
            Ridx[csz]=best;
            csz++;
            i=best+1;
        }
    }

    /* --------------------------------------------
       PHASE 2: UNIQUENESS CHECK WITH FULL FILTERS
       -------------------------------------------- */

    for (int k=0;k<csz;k++){
        if (!isC[k]) continue;

        long long target=C[k];
        int li=Lidx[k], ri=Ridx[k];
        long long A_i=A[li], A_j=A[ri];
        int orig_len = (ri - li + 1);

        long long start_min = A_i - WINDOW_RADIUS;
        long long start_max = A_i + WINDOW_RADIUS;
        if (start_min < 1) start_min=1;

        long long end_min = A_j - WINDOW_RADIUS;
        if (end_min < 1) end_min=1;
        long long end_max = A_j + WINDOW_RADIUS;

        int cnt = count_theoretical_segments(
            target, S, 2,
            start_min,start_max,
            end_min,end_max,
            A_i,A_j,
            orig_len
        );

        if (cnt != 1) {
            free(C); free(isC); free(Lidx); free(Ridx);
            return 1;
        }
    }

    free(C); free(isC); free(Lidx); free(Ridx);
    return 0;
}
