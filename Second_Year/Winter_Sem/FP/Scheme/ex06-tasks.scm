#lang racket

;(define (tree? t)
;  (or (null? t)
;    (and (list? t)
;      (= (length t) 3)
;      (tree? (cadr t))
;      (tree? (caddr t))))) ; рекурсивната дефиниция представена като предикат
;
;(define empty-tree '()) ; празното дърво е празен списък
;(define (make-tree root left right) (list root left right)) ; конструктор за дърво с ляво и дясно поддърво
;(define (make-leaf root) (make-tree root empty-tree empty-tree))  ; конструктор за листо
;(define root-tree car) ; getter за стойността на възела
;(define left-tree cadr) ; getter за лявото поддърво
;(define right-tree caddr) ; getter за дясното поддърво
;(define empty-tree? null?) ; предикат за проверка на празно поддърво
;
;
;(define t (make-tree 5
;            (make-tree 1
;              (make-tree 4
;                '()
;                (make-leaf 13))
;              (make-leaf 3))
;            (make-tree 8
;              (make-tree 0
;                (make-leaf 10)
;                (make-leaf 9))
;              (make-leaf 11))))

; Двоични дървета


; Задачи за упражнение
; 1a
(define (num-size n)
  (if (= n 0)
    0
    (+ 1 (num-size (quotient n 10)))))

(define (reverse n)
  (define (helper n i)
    (if (and (> i -1) (> n 0))
      (+ (* (expt 10 (- i 1)) (remainder n 10)) (helper (quotient n 10) (- i 1)))
      0))
  (helper n (num-size n)))

(define (diff-reverse n)
  (- n (reverse n)))

(diff-reverse 7641)  ; => 7641 - 1467 = 6174
(diff-reverse 123)   ; => 123 - 321 = -198
(diff-reverse 7)     ; => 7 - 7 = 0

; 1b
(define (sort-digits n)
  )