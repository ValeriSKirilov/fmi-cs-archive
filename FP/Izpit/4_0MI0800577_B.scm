#lang racket

(define (concat lst1 lst2)
  (if (null? lst1) lst2
                   (cons (car lst1) (concat (cdr lst1) lst2))))

(define (encode lst)
  (define (helper lst1 prevValue currDouble result)
    (if (null? lst1) result
                     (if (= (car lst1) prevValue) (helper (cdr lst1) prevValue (cons (+ 1 (car currDouble)) (cdr currDouble)) result)
                                                  (helper (cdr lst1) (car lst1) (cons 1 (car lst1)) (if (= (car currDouble) 1) (concat result (list (cdr currDouble)))
                                                                                                                               (concat result currDouble))))))
  (helper (cdr lst) (car lst) (cons 1 (car lst)) '()))

(define (pairToList pairFst pairSnd)
  (if (= pairFst 0) '()
                    (cons pairSnd (pairToList (- pairFst 1) pairSnd))))

(define (handlePair lst pair)
  (concat lst (pairToList (car pair) (cdr pair))))

(define (decode lst)
  (define (helper coded result)
    (if (null? coded) result
                      (if (not (pair? (car coded))) (helper (cdr coded) (concat result (list (car coded))))
                                                    (helper (cdr coded) (handlePair result (car coded))))))
  (helper lst '()))

;(decode '(1 2 (3 . 8) 0 (7 . 8) 9))

(define (removeAt lst idx)
  (define (helper lst1 start)
    (if (null? lst1) '()
                     (if (= start idx) (cdr lst1)
                                       (cons (car lst1) (helper (cdr lst1) (+ 1 start))))))
  (helper lst 0))

(define (removeFromTo lst startIdx endIdx)
  (define (helper lst1 start end)
    (if (> start end) lst1
                      (removeFromTo (removeAt lst1 start) (+ 1 start) end)))
  (helper lst startIdx endIdx))

(define (getFromTo lst startIdx endIdx)
  (define (helper lst1 start result)
    (if (< start startIdx) (helper (cdr lst1) (+ 1 start) result)
                           (if (start > endIdx) result
                                                (helper (cdr lst1) (+ 1 start) (concat result (list (car lst1)))))))
  (helper lst 0 '()))

(define (delete lst startIdx endIdx)
  (cons (encode (removeFromTo lst startIdx endIdx)) (encode (getFromTo lst startIdx endIdx))))