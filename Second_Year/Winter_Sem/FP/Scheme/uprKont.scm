#lang racket

(define dl '(1 (2 (5 1) 4) 3))

(define (add-depth l)
  (define (helper l depth)
    (cond
      [(null? l) '()]
      [(pair? (car l))
        (cons (helper (car l) (+ depth 1))
          (helper (cdr l) (+ depth 1)))]
      [else
        (cons (cons (car l) depth)
          (helper (cdr l) depth))]))
  (helper lst 0))

(define (deepMapCond lst pred f1 f2)
  (define lWD (add-depth lst))
  )