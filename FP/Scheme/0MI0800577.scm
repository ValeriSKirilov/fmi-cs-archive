#lang racket

; Задача 1
(define (sum-digits n)
  (if (= n 0)
    0
    (+ (remainder n 10) (sum-digits (quotient n 10)))))

(sum-digits 12345) ;  => 15 (1+2+3+4+5)
(sum-digits 42) ; => 6 (4+2)

; Задача 2
(define (sum-digits-interval-cubes n)
  (define (s-d-i-c-helper n i)
    (if (> (+ n 1) i)
      (cons (sum-digits (expt i 3)) (s-d-i-c-helper n (+ i 1)))
      '()))
  (s-d-i-c-helper n 1))

(sum-digits-interval-cubes 6) ; => (1 8 9 10 8)

; Задача 3
(define simple-tree '(2 (3 (4) (5)) (6)))

(define (leaf? tree)
  (null? (cdr tree)))

(define (sum-digits-tree-cubes-product tree)
  (if (leaf? tree)
    (list (apply * (sum-digits-interval-cubes (car tree))))
    (cons (apply * (sum-digits-interval-cubes (car tree))) (map sum-digits-tree-cubes-product (cdr tree)))))

(sum-digits-tree-cubes-product simple-tree) ; => (8 (72 (720) (5760)) (51840))
