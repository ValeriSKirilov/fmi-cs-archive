#lang racket

(define (sum-func n sum)
  (if (= n 0)
    sum
    (sum-func (- n 1) (+ sum n))))

(define (sum-down-from n)
  (sum-func n 0))

(sum-down-from 5)

(define (string-RHFunc str n result)
  (if (= n 0)
    result
    (string-RHFunc str (- n 1) (string-append result str " "))))

(define (string-repeat str n)
  (string-RHFunc str n ""))

(string-repeat "FMI" 5)

(define (fib-calc n n0 n1 beg-ind)
  (if (= beg-ind n)
    n1
    (fib-calc n n1 (+ n0 n1) (+ beg-ind 1))))


(define (fib n)
  (fib-calc n 0 1 1))

(fib 5)

(define a "global-a")
(define b "global-b")
(define c "global-c")
(define scope (string-append a "," b "," c))

(define (closure-demo)
  (define a "closure-a")
  (define scope (string-append a "," b "," c))

  (define (closure-demo-2)
    (define b "closure-2-b")
    (define scope (string-append a "," b "," c))

    (define (closure-demo-3)
      (define a "closure-3-a")
      (define scope (string-append a "," b "," c))
      scope
      )

    (string-append scope " -> " (closure-demo-3))
    )

  (string-append scope " -> " (closure-demo-2))
  )

(closure-demo)

; 4.1
(define (sum-of-powers a b power)
  (let ([a-power (expt a power)]
         [b-power (expt b power)])
  (+ a-power b-power)))

(sum-of-powers 1 2 2)