#lang racket

; TRIFON
(define (tree? t)
  (or (null? t)
    (and (list? t) (= (length t) 3) (tree? (cadr t)) (tree? (caddr t)))))

(define empty-tree '())
(define (make-tree x l r) (list x l r))
;; (define make-tree list)
(define (leaf x) (make-tree x empty-tree empty-tree))

(define (leaf? x) (and (null? (left-tree x)) (null? (right-tree x))))

(define root-tree car)
(define left-tree cadr)
(define right-tree caddr)
(define empty-tree? null?)

(define (tree-children bt) (cdr bt))

(define t (make-tree 1 (leaf 2) (make-tree 3 (leaf 4) (leaf 5))))

(define (depth t)
  (if (empty-tree? t) 0
                      (+ 1 (max (depth (left-tree t)) (depth (right-tree t))))))

(define (square x)
  (* x x))

(define test-list '(1 2 3 4 5 6))
(define test-num 123456)

(define (list-size l)
  (if (null? l) 0
                (+ 1 (list-size (cdr l)))))

;(list-size test-list)

(define (num-size n)
  (if (< n 10) 1
               (+ 1 (num-size (quotient n 10)))))

;(num-size test-num)

(define (list-to-num l)
  (if (null? l) 0
                (+ (* (car l) (expt 10 (- (list-size l) 1))) (list-to-num (cdr l)))
    ))

;(list-to-num test-list)

(define (num-to-list n)
  (define (helper n)
    (if (= n 0)
      '()
      (cons (remainder n 10) (helper (quotient n 10)))))
  (if (= n 0)
    '(0)
    (reverse (helper n))))

;(num-to-list test-num)

; zad 1
(define (multiply-by-position n)
  (define (multiply-digits lst pos)
    (if (null? lst) '()
                    (append (num-to-list (* (car lst) pos)) (multiply-digits (cdr lst) (- pos 1)))))

  (list-to-num (multiply-digits (num-to-list n) (length (num-to-list n)))))

;(multiply-by-position 123)  ;→ 1*3, 2*2, 3*1 → 343
;(multiply-by-position 507)  ;→ 5*3, 0*2, 7*1 → 1507
;(multiply-by-position 987)  ;→ 9*3, 8*2, 7*1 → 27167
;(multiply-by-position 1249) ;→ 1*4, 2*3, 4*2, 9*1 → 4689
;(multiply-by-position 9000) ;→ 9*4, 0*3, 0*2, 0*1 → 36000

; zad 2
(define (helper-single-func f l)
  (apply max (map f l)))

(define (helper-list-of-funcs f l)
  (apply max (map (lambda (element) (helper-single-func element l)) f)))

(define (minmax fm l)
  (apply min (map (lambda (x) (helper-list-of-funcs x l)) fm)))

;(minmax (list (list square exp) (list cos (lambda (x) (+ 1 x)))) '(-1 0 1)) ;→2 .0

; zad 3
(define (list-even-sum l)
  (foldr (lambda (x y) (+ y (if (even? x) x 0))) 0 l))

(define (find-paths bt)
  (define (helper bt currPath)
    (if (leaf? bt) (list (append currPath (list (root-tree bt))))
                   (append (helper (left-tree bt) (append currPath (list (root-tree bt))))
                     (helper (right-tree bt) (append currPath (list (root-tree bt)))))))
  (helper bt '()))

;(find-paths '(1 (4 (4 () ()) (1 () ())) (6 (3 (2 (2 () ()) (1 () ())) (2 () ())) (-4 () ()))))
;(map list-even-sum (find-paths '(1 (4 (4 () ()) (1 () ())) (6 (3 (2 (1 () ()) (1 () ())) (2 () ())) (-4 () ())))))

(define (paths-max-length paths)
  (define max-sum (apply max (map list-even-sum (find-paths paths))))
  (filter (lambda (x) (equal? (list-even-sum x) max-sum)) (find-paths paths)))

;(paths-max-length '(1 (4 (4 () ()) (1 () ())) (6 (3 (2 (2 () ()) (1 () ())) (2 () ())) (-4 () ()))))

(define (maximum-even-nodes-sum bt)
  (if (> (list-size (paths-max-length bt)) 1)
    (car (filter (lambda (x) (= (list-size x) (apply max (map list-size (paths-max-length bt))))) (paths-max-length bt)))
    (car (paths-max-length bt))))

;(maximum-even-nodes-sum '(1 (4 (6 () ()) (1 () ())) (6 (3 (2 (2 () ()) (1 () ())) (2 () ())) (-4 () ()))))
;(maximum-even-nodes-sum '(1 (4 () ()) (6 (3 () ()) (-4 () ())))) ;→(1 6 3)
;(maximum-even-nodes-sum '(1 (4 () ()) (6 (-2 () ()) (-4 () ())))) ;→(1 6)

; Aleks
; TREES
; 3
;(define (list->bst l)
;  ())

; GENERAL TASKS
; 1a
(define (diff-reverse n)
  (list-to-num (map - (num-to-list n) (reverse (num-to-list n)))))

;(diff-reverse 7641)  ; => 7641 - 1467 = 6174
;(diff-reverse 123)   ; => 123 - 321 = -198
;(diff-reverse 7)     ; => 7 - 7 = 0

(define (add-el-sort el l)
  (if (null? l) (list el)
                (if (< el (car l)) (append (list el) l)
                                   (append (list (car l)) (add-el-sort el (cdr l))))))

; 1b
(define (sort-digits n)
  (list-to-num (reverse (foldr add-el-sort '() (num-to-list n)))))

;(sort-digits 6174)     ; => 7641
;(sort-digits 9912939)  ; => 9999321
;(sort-digits 0)        ; => 0
;(sort-digits 123456789) ; => 987654321

; 2
(define (interval-to-list a b)
  (if (>= a b) '()
               (cons a (interval-to-list (+ a 1) b))))

;(interval-to-list 1 9)

(define (give-even-inter a b)
  (filter even? (interval-to-list a b)))

;(give-even-inter 1 9)

(define (permutable? a b f g)
  (equal? (map f (map g (give-even-inter a b))) (map g (map f (give-even-inter a b)))))

;(permutable? 1 9 (lambda (x) (* x x)) (lambda (x) (* x x x))) ; => #t
;(permutable? 1 9 (lambda (x) (+ x 1)) (lambda (x) (- x 2)))   ; => #t
;(permutable? 1 9 (lambda (x) (* x x)) (lambda (x) (/ x 2)))   ; => #f
;(permutable? 1 9 (lambda (x) (* x x)) (lambda (x) (+ 100 x))) ; => #f

; 3
(define test '((24 . 25) (90 . 110) (0 . 100) (10 . 109) (1 . 3) (-4 . 2)))

(define (subintervals i)
  (define a (car i))
  (define b (cdr i))

  (define (intervals-from start)
    (if (>= start b) '()
                     (append (intervals-starting-at start (+ start 1))
                       (intervals-from (+ start 1)))))

  (define (intervals-starting-at start end)
    (if (> end b) '()
                  (cons (cons start end)
                    (intervals-starting-at start (+ end 1)))))

  (intervals-from a))

;(subintervals '(20 . 30))

(define (intervals-in-list-sizes l)
  (map (lambda (x) (length (subintervals x))) l))

(define (longest-interval-subset il)
  (subintervals (car (filter (lambda (x) (= (length (subintervals x)) (apply max (intervals-in-list-sizes il)))) il))))

;(longest-interval-subset test)

; 4
(define (quick-sort l)
  (foldr add-el-sort '() l))

;(quick-sort '(5 2 8 1 9 3))  ; => (1 2 3 5 8 9)
;(quick-sort '(10 5 2 7))     ; => (2 5 7 10)
;(quick-sort '(1))             ; => (1)
;(quick-sort '())              ; => ()
;(quick-sort '(5 5 2 5 2))     ; => (2 2 5 5 5)