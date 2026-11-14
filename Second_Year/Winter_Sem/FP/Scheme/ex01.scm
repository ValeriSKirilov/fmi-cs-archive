#lang racket

(/ (+ 100 50) (* 10 3))

;(+ 1/3 2.25 (/ (- (expt 7 (remainder 60 7)) (* 3 (quotient 21 6))) 11))

(define a 8)

(define b 15)

(define c (sqrt (+ (expt a 2) (expt b 2))))

;(define pi 3 .141592)

(define (square x) (* x x))

;(define (CircleArea r) (* pi (square r)))

;(define (CylVol r h) (* pi (CircleArea r) h))

(define (grade-label grade) (
                              if (< grade 3)
                              (print "Скъсаха ме")
                              (print "Пръснах ги")
                              )
  )

(define (safe-divide a b) (
                            if (equal? b 0)
                            (print 0)
                            (print (/ a b)
                              )
                            )

  )

(define (fizz-buzz x)
  (cond ((= (remainder x 15) 0) "FizzBuzz")
    ((= (remainder x 5) 0) "Buzz")
    ((= (remainder x 3) 0) "Fizz")
    (else "Аз съм си число съм си")
    )
  )

(define (leap-year? x)
  (cond ((= (remainder x 400) 0) "LEAP")
    ((and (= (remainder x 4) 0) (not (= (remainder x 100) 0))) "LEAP")
    (else "NOT LEAP")
    )
  )

(define (sum-down-from n)
  (if (equal? n 0) 0
                   (+ n (sum-down-from (- n 1)))))

(sum-down-from 5)

(define (string-repeat str n)
  (define (s-r-helper str n i)
    (if (equal? n i)
      str
      (string-append str (s-r-helper str n (+ i 1)))))
  (s-r-helper str n 1))

(string-repeat "NIGGA_" 5)

(define (oddCount x)
  (cond
    [(= x 0) 0]
    [(odd? (remainder x 10))
      (+ 1 (oddCount (quotient x 10)))]
    [else
      (oddCount (quotient x 10))]))

(define (evenCount x)
  (cond
    [(= x 0) 0]
    [(even? (remainder x 10))
      (+ 1 (evenCount (quotient x 10)))]
    [else
      (evenCount (quotient x 10))]))

(define (digit-parity-majority x)
  (cond
    [(> (oddCount x) (evenCount x))
      "odd"]
    [(< (oddCount x) (evenCount x))
      "even"]
    [else "equal"]))

(digit-parity-majority 1122334)

(define (count-digits x)
  (if (= x 0) 0
              (+ 1 (count-digits (quotient x 10)))))

(count-digits 10000)

