#lang racket

;1
(define (apply-n-times f n x)
  (define (apply-helper f n x)
    (if(= n 1)
       (f x)
       (apply-helper f (- n 1) (f x))
     )
  )
    ( apply-helper f n x)
)

(apply-n-times ((lambda (x) (lambda (y) (* x y))) 2) 2 (+ 5 3))

;2
(define (compose f g)
  (define (result x)
    (f (g x)))
  result
)

(define plus-one-square (compose (lambda (y) (* y y)) (lambda (x) (+ x 1))))
(plus-one-square 4)

;3
(define (partial-apply f x)
  (define (result y)
    (f x y))
  result
)

(define multiply-by-2 (partial-apply * 2))
(multiply-by-2 4)

;4.1
(define (accumulate-interval combiner null-value term a b)
  (if (> a b)
      null-value
      (combiner (term a)
                (accumulate-interval combiner null-value term (+ a 1) b)
      )
  )
)

(accumulate-interval + 0 (lambda (x) (* x x)) 1 5) ; => 55 (1+4+9+16+25)

;4.2
(define (product-interval x y) (accumulate-interval * 1 (lambda (x) x) x y))
(product-interval 1 5)   ; => 120 (факториел на 5)
(product-interval 3 6)   ; => 360 (3*4*5*6)

;4.3
(define (sum-odd-squares x y) (accumulate-interval + 0 (lambda (x) (if(= (remainder x 2) 0)
                                                                      0
                                                                      ((lambda (y) (* y y)) x)
                                                                   )
                                                       ) x y
                              )
)

(sum-odd-squares 1 5)   ; => 35 (1+9+25)
(sum-odd-squares 2 10)  ; => 165 (9+25+49+81)

;4.4
(define (count-divisors-in-interval n a b) (accumulate-interval + 0 (lambda (x) (if (= (remainder n x) 0)
                                                                                    1
                                                                                    0
                                                                                )
                                                                    ) a b
                                           )
)

(count-divisors-in-interval 12 1 12)  ; => 6 (1,2,3,4,6,12)
(count-divisors-in-interval 20 1 10)  ; => 5 (1,2,4,5,10)

