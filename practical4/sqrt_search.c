#include <stdio.h>
#include <math.h>
#include <time.h>

#define EPSILON 0.01

typedef struct {
    double ans;
    long long guesses;
    int found;
} Result;

/* Exhaustive enumeration (Lecture 4, slide 8).
   Instead of exit(1) on failure it returns found = 0, so all inputs can be compared. */
Result sqrt_exhaustive(double x)
{
    double step = EPSILON * EPSILON;
    Result r = {0.0, 0, 0};

    while (fabs(r.ans * r.ans - x) >= EPSILON && r.ans * r.ans <= x) {
        r.ans += step;
        r.guesses++;
    }
    r.found = fabs(r.ans * r.ans - x) < EPSILON;
    return r;
}

/* Bisection search (Lecture 4, slide 45):
   keep [low, high] containing sqrt(x), halve it every iteration. */
Result sqrt_bisection(double x)
{
    double low = 0.0;
    double high = x > 1.0 ? x : 1.0;   /* sqrt(x) > x when x < 1 */
    Result r = {(low + high) / 2.0, 0, 0};

    while (fabs(r.ans * r.ans - x) >= EPSILON) {
        if (r.ans * r.ans < x)
            low = r.ans;
        else
            high = r.ans;
        r.ans = (low + high) / 2.0;
        r.guesses++;
    }
    r.found = 1;
    return r;
}

static double now_sec(void)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(void)
{
    double inputs[] = {0.25, 2, 25, 1000, 2500, 12345, 1e6, 1e8, 1e10};
    int n = sizeof(inputs) / sizeof(inputs[0]);
    const int BISECT_REPEATS = 100000;   /* bisection is too fast to time once */

    printf("%10s | %-26s %12s %10s | %-12s %6s %10s | %8s\n",
           "x", "exhaustive ans", "guesses", "time, s",
           "bisect ans", "guess", "time, s", "slower x");
    printf("-----------+----------------------------------------------------+"
           "--------------------------------+---------\n");

    for (int i = 0; i < n; i++) {
        double x = inputs[i];

        double t0 = now_sec();
        Result e = sqrt_exhaustive(x);
        double te = now_sec() - t0;

        Result b = {0};
        t0 = now_sec();
        for (int k = 0; k < BISECT_REPEATS; k++)
            b = sqrt_bisection(x);
        double tb = (now_sec() - t0) / BISECT_REPEATS;

        char eans[32];
        if (e.found)
            snprintf(eans, sizeof eans, "%.4f", e.ans);
        else
            snprintf(eans, sizeof eans, "FAILED (stopped %.1f)", e.ans);

        printf("%10g | %-26s %12lld %10.6f | %-12.4f %6lld %10.2e | %8.0f\n",
               x, eans, e.guesses, te, b.ans, b.guesses, tb, te / tb);
    }
    return 0;
}
