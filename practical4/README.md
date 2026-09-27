# Practical 4 — Git tips + square root search

## Task 1 — rebase / merge snapshots

Branch `feature_rebase` (with `main.c`) is pushed to `origin`.

| Snapshot | What it shows |
|---|---|
| ![](screenshots/1_after_step7.png) | `git log` on `feature_rebase` after steps 7–8: two feature commits, `main_var` commit is not there |
| ![](screenshots/2_after_step13_rebase.png) | after `git rebase main_backup` (conflict in `main.c` resolved, both lines kept): 3 commits — `main_var` + two feature commits on top |
| ![](screenshots/3_after_step15_merge.png) | `main_backup` after `git merge feature_rebase` (fast-forward, because the branch was rebased first) |

## Task 2 — exhaustive search vs bisection

`sqrt_search.c`, epsilon = 0.01, exhaustive step = epsilon² = 0.0001.

```
gcc -std=c11 -Wall -Wextra -o sqrt_search sqrt_search.c -lm
./sqrt_search
```

![](screenshots/task2_sqrt_timing.png)

Conclusions:

* Exhaustive search makes ~sqrt(x) / epsilon² guesses — O(sqrt(x)); bisection makes ~log2(x / epsilon) guesses — O(log x), at most 55 here.
* For x = 1000 both give the answer from the lecture (31.6227), but exhaustive needs 316 227 guesses vs 19.
* The gap grows with x: ~10⁴ times at x = 1000, ~10⁵ at x = 10⁶, ~10⁶–10⁷ at x = 10⁸…10¹⁰.
* **Inputs where exhaustive search is much slower: large x (10⁶ and more).** For x = 10¹⁰ it runs 10⁹ iterations (~3 s) and still fails, bisection needs 55 iterations (~0.5 µs).
* Why exhaustive fails for large x: near sqrt(x) one step changes ans² by 2·ans·epsilon² > epsilon once ans > 50, so it can jump over the answer. The lecture code then calls exit(1); here the function returns found = 0 instead, so every input can be timed.
