#lang racket

(define (search p l)
  (and (not (null? l)) (or (p (car l)) (search p (cdr l)))))

; Създаване на асоциативен списък
(define phone-book
  '((alice . "555-1234")
     (bob . "555-5678")
     (charlie . "555-9012")))

; Основни функции за работа с асоциативни списъци
;(assoc 'alice phone-book)     ; => (alice . "555-1234")
;(assq 'bob phone-book)        ; => (bob . "555-5678") - използва eq? за сравнение
;(assv 42 '((1 . "one") (42 . "forty-two"))) ; => (42 . "forty-two") - използва eqv?

; 1.1
(define (add-entry key value list)
  (cons (cons key value) list))

;(add-entry 'david "555-3456" phone-book)  ; => ((david . "555-3456") (alice . "555-1234") ...)

; 1.2
(define (remove-entry key list)
  (filter (lambda (entry) (not (eq? (car entry) key))) list))

;(remove-entry 'bob phone-book)  ; => ((alice . "555-1234") (charlie . "555-9012"))

; 1.3
(define (update-entry key value list)
  (add-entry key value (remove-entry key list)))

;(update-entry 'alice "555-9999" phone-book)  ; => ((alice . "555-9999") (bob . "555-5678") ...)

; Дърво (tree) - първият елемент е стойността, останалите са децата
(define simple-tree
  '(1
     (2 (4) (5))
     (3 (6) (7))
     (8)))

; Функции за достъп до елементите на дърво
(define (tree-value tree) (car tree)) ;
(define (tree-children tree) (cdr tree))

;(tree-children simple-tree)

; Проверка за листо (възел без деца)
(define (leaf? tree) (null? (tree-children tree)))

;; 2.1
(define (tree-size tree)
  (if (leaf? tree)
    1
    (+ 1 (foldl + 0 (map tree-size (tree-children tree))))))

;(tree-size simple-tree)  ; => 8
;(tree-size '(1))         ; => 1

; 2.2
(define (tree-height tree)
  (if (leaf? tree)
    1
    (+ 1 (foldl max 0 (map tree-height (tree-children tree))))))

;(tree-height simple-tree)  ; => 3
;(tree-height '(1))         ; => 1

; 2.3
(define (tree-leaves tree)
  (if (leaf? tree)
    (list (tree-value tree))
    (apply append (map tree-leaves (tree-children tree)))))

;(tree-leaves simple-tree)  ; => (4 5 6 7 8)

; 2.4
(define (tree-sum tree)
  (if (leaf? tree)
    (tree-value tree)
    (+ (tree-value tree) (foldl + 0 (map tree-sum (tree-children tree))))))

;(tree-sum simple-tree)  ; => 36 (1+2+3+4+5+6+7+8)

; 2.5
(define (tree-map f tree)
  (if (leaf? tree)
    (list (f (tree-value tree)))
    (cons (f (tree-value tree))
      (map (lambda (child-tree) (tree-map f child-tree))
        (tree-children tree)))))

;(tree-map (lambda (x) (* x 2)) simple-tree)  ; => (2 (4 (8) (10)) (6 (12) (14)) (16))

; 2.6
(define (tree-find pred tree)
  (cond
    [(pred (tree-value tree)) (tree-value tree)]
    [(leaf? tree) #f]
    [else (search (lambda (child) (tree-find pred child)) (tree-children tree))]))

;(tree-find even? simple-tree)  ; => 2
;(tree-find (lambda (x) (> x 10)) simple-tree)  ; => #f

; 3.1
(define (tree-balance tree)
  (if (leaf? tree)
    #t
    (let* ([children (tree-children tree)]
            [heights (map tree-height children)]
            [max-h (apply max heights)]
            [min-h (apply min heights)])
      (and (<= (- max-h min-h) 1)
        (andmap tree-balance children)))))

(displayln (tree-balance simple-tree)) ; #t
(displayln (tree-balance '(1 (2) (3))))  ; #t
(displayln (tree-balance '(1 (2 (4)) (3)))) ; #t
(displayln (tree-balance '(1 (2 (4 (5))) (3)))) ; #f

; 3.2
(define (count-occurances lst)
  (define (count-helper lst acc)
    (if (null? lst)
      acc
      (let* ([current (car lst)]
              [entry (assoc current acc)])
        (if entry
          (count-helper (cdr lst)
            (update-entry current (+ 1 (cdr entry)) acc))
          (count-helper (cdr lst)
            (add-entry current 1 acc))))))
  (count-helper lst '()))

;(count-occurances '(1 2 2 3 3 3 4))  ; => ((4 . 1) (3 . 3) (2 . 2) (1 . 1))
;(count-occurances '(5 5 5 5))  ; => ((5 . 4))
;(count-occurances '(1 2 3))  ; => ((3 . 1) (2 . 1) (1 . 1))