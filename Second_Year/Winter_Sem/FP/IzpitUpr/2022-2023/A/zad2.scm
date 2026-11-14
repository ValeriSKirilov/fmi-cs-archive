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

(define (zip-streams op s1 s2)
  (cons-stream (op (head s1) (head s2)) (zip-streams op (tail s1) (tail s2))))

(define (sum-pairs lst idx1 idx2)
  (if (> idx1 idx2) 0
                    (+ (* (list-ref lst idx1) (list-ref lst idx2))
                      (sum-pairs lst (+ idx1 1) (- idx2 1)))))

(define (calculate-element currIdx k elements)
  (if (< currIdx k)
    1
    (let ((currElements (list-tail elements (- currIdx k))))
      (/ (sum-pairs (cdr currElements) 0 (- k 2))
        (car currElements)))))

(define (somos k)
  (define (helper elements currIdx)
    (let ((new-element (calculate-element currIdx k elements)))
      (cons-stream new-element
        (helper (append elements (list new-element)) (+ currIdx 1)))))
  (helper '() 0))

(take 18 (somos 8))

(define (map-stream f . streams)
  (cons-stream (apply f (map head streams))
    (apply map-stream f (map tail streams))))