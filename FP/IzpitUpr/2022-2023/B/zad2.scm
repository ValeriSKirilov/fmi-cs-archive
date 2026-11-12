#lang racket

(define the-empty-stream '())

(define-syntax cons-stream
  (syntax-rules () ((cons-stream h t) (cons h (delay t)))))

(define head car)

(define (tail s) (force (cdr s)))

(define empty-stream? null?)

(define (take n s)
  (if (or (= n 0) (empty-stream? s)) '()
                                     (cons (head s) (take (- n 1) (tail s)))))

(define (map-stream f s)
  (cons-stream (f (head s)) (map-stream f (tail s))))

(define (filter-stream p? s)
  (if (p? (head s))
    (cons-stream (head s) (filter-stream p? (tail s)))
    (filter-stream p? (tail s))))

(define (from n) (cons-stream n (from (+ n 1))))

(define (nondivisor d) (lambda (n) (> (remainder n d) 0)))

(define (sieve stream)
  (cons-stream (head stream)
    (sieve (filter-stream (nondivisor (head stream)) (tail stream)))))

(define (primes) (sieve (from 2)))

; Merge two sorted streams, removing duplicates
(define (merge-streams s1 s2)
  (cond
    [(< (head s1) (head s2))
      (cons-stream (head s1) (merge-streams (tail s1) s2))]
    [(> (head s1) (head s2))
      (cons-stream (head s2) (merge-streams s1 (tail s2)))]
    [else
      (cons-stream (head s1) (merge-streams (tail s1) (tail s2)))]))

; Scale a stream by multiplying each element by n
(define (scale-stream n s)
  (map-stream (lambda (x) (* n x)) s))

; Weighted merge - each stream has a "weight" (the number we're multiplying by)
; This ensures we can always determine which stream has the smallest next element
(define (weighted-merge base-stream prime-stream)
  (cons-stream
    (* (head base-stream) (head prime-stream))
    (merge-streams
      (scale-stream (head base-stream) (tail prime-stream))
      (weighted-merge (tail base-stream) prime-stream))))

; Main primitive function
(define (primitive k)
  (if (= k 1)
    (primes)
    (weighted-merge (primitive (- k 1)) (primes))))

; Test
(take 10 (primitive 3))