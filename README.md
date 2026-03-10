# Euler-Project-4-Largest-Palindrome-from-multiplication-2-or-3-number
This project contains optimized methods to find the largest palindrome smaller than a number 𝑁≥0, such that it is the product of 2 or 3 numbers with n digits.
This repository contains the necessary tools to test the optimization and validity of a solution to the problem of finding the largest palindrome resulting from the multiplication of 2 or 3 numbers.

compilation : gcc x -o out -lm -fopenmp

compilation : gcc test_solution.cx -o out -lm -fopenmp -pal_3_factor.c

compilation : gcc test_solution_3_factor.c pal_3_factor.h -o out -lm -fopenmp 