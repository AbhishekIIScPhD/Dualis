(declare-var len Int)
(declare-var len1 Int)
(declare-var maxDiff Int)
(declare-var maxDiff1 Int)
(declare-var ev1 Int)
(declare-var ev2 Int)
(declare-rel insert (Int Int Int Int Int Int))
(declare-rel inv (Int Int))
(declare-rel fail ())
;; (define-fun abs ((val Int)) Int (ite (>= val 0) val (- val)))
(define-fun MIN () Int -129)


(rule (=> (and (= len 0) (= maxDiff MIN)) (inv len maxDiff)))
(rule (=> (and (inv len maxDiff) (and (>= ev1 0) (<= ev1 3) (>= ev2 0) (<= ev2 3) (< (abs (- ev1 ev2)) 2)) (insert len len1 ev1 ev2 maxDiff maxDiff1)) (inv len1 maxDiff1)))
(rule (=> (and (inv len maxDiff) (not (and (>= ev1 0) (<= ev1 3) (>= ev2 0) (<= ev2 3) (< (- ev1 ev2) 2)))) (inv len maxDiff)))
(rule (=> (and (inv len maxDiff) (not (=> (> len 0) (< maxDiff 2)))) fail))

(query fail :print-certificate true)
