#lang racket

; 1.1

(define a (cons 0 0))
(define b (cons 3 4))

(define (distance p1 p2)
  (sqrt (+ (expt (- (car p2) (car p1)) 2) (expt (- (cdr p2) (cdr p1)) 2)))
  )

; 2.1

(define l (list 1 2 3 4))

(define (list-squares-sum l)
  (if (null? l) 0
                (+ ((lambda (x) (* x x)) (car l)) (list-squares-sum (cdr l)))
    )
  )

; 2.2
(define (list-char-count l)
  (if (null? l) l
                (cons (string-length (car l)) (list-char-count (cdr l)))
    )
  )

(list-char-count '("hello" "world" "scheme" "racket"))  ;  => (5 5 6 6)
(list-char-count '("a" "bc" "def" "ghij"))  ;  => (1 2 3 4)

; 3.1
(define (list-cubes l)
  (map (lambda (x) (expt x 3)) l)
  )

(list-cubes l)

; 3.2
(define (list-even-cubes l)
  (filter even? (list-cubes l))
  )

(list-even-cubes l)

; 3.3
(define (list-even-cubes-sum l)
  (apply + (list-even-cubes l))
  )

(list-even-cubes-sum l)

; 3.4
(define (count-predicate pred l)
  (foldr (lambda (x y) (if (pred x) (+ 1 y) y)) 0 l)
  )

(count-predicate even? '(1 2 3 4 5 6))  ;  => 3
(count-predicate (lambda (x) (> x 3)) '(1 2 3 4 5))  ;  => 2

; 5.1
; (define (zip l1 l2)
;   (if(null? l1) l1
;        (cons (+ (car l1) (car l2)) (zip (cdr l1) (cdr l2)))
;   )
; )

; (zip '(1 2 3) '(4 5 6)) ;  '(5 7 9)

; 5.2
(define (zip l1 l2)
  (map + l1 l2)
  )

(zip '(1 2 3) '(4 5 6)) ;  '(5 7 9)

; 5.3
(define (matrix-transpose l)
  (if (null? l) l
                (apply map list l)
    )
  )

(matrix-transpose '((1 2 3) (4 5 6)))  ;  => ((1 4) (2 5) (3 6))

; 5.4
(define (flatten l)
  (cond
    [(null? l) '()]
    [(list? (car l))
      (append (flatten (car l)) (flatten (cdr l)))]
    [else
      (cons (car l) (flatten (cdr l)))]
    ))

(flatten '(1 (2 3) (4 (5 6))))  ; => (1 2 3 4 5 6)