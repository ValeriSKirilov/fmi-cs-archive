#lang racket

; ZAD 1
(define (sum-digits x)
  (if (> x 0) (+ (remainder x 10) (sum-digits (quotient x 10)))
              0))

(define (prime? x)
  (define (helper x currDel status)
    (if (not (= x currDel)) (if (or (= (remainder x currDel) 0) (< currDel (- 1 x))) #f
                                                                                     (helper x (+ 1 currDel) status))
                            status))
  (helper x 2 #t))

(define (sumDig x)
  (define (helper x currDel)
    (if (and (= (remainder x currDel) 0) (prime? currDel)) (sum-digits currDel)
                                                           (helper x (+ 1 currDel))))
  (helper x 2))

;(sumDig 121)

; ZAD 2
(define (give-list-from-to lst a b)
  (define (helper lst a b start)
    (if (< start a) (helper (cdr lst) a b (+ 1 start))
                    (if (> start b) '()
                                    (cons (car lst) (helper (cdr lst) a b (+ 1 start))))))
  (helper lst a b 1))

;(give-list-from-to '(1 2 3 4 5 6 7 8 10 9) 3 6)

(define (get-position-el lst el)
  (define (helper lst el pos)
    (if (= (car lst) el) pos
                         (helper (cdr lst) el (+ 1 pos))))
  (helper lst el 1))

;(get-position-el '(10 2 7 1 2 9) 7)

(define (find-valid-nums func lst)
  (filter (lambda (x) (= (func x) 0)) lst))

(find-valid-nums (lambda (x) (remainder x 3)) '(1 2 3 4 5 6 7 8 10 9))

(define (minSub func lst)
  ;  (define (helper valid-nums valid-nums-copy)
  ;    (if(null? valid-nums) '()
  ;                          ()))
  ;  (helper (test func lst) (test func lst))
  (apply list (foldl () (car lst) (cdr lst))))

;
;;(define (test func lst)
;;  (if (not (= (func (car l)) 0)) (test func (cdr lst))
;;                                 (cons (car l))))
;
;;(define (minSub func lst)
;;  (list () ()))
;
;; ZAD 3
;(define (leaf? bt)
;  (and (null? (cadr bt)) (null? (caddr bt))))
;
;
;(define (count-elements bt)
;  (if (leaf? bt) 1
;                 (+ 1 (count-elements (cadr bt)) (count-elements (caddr bt)))))
;
