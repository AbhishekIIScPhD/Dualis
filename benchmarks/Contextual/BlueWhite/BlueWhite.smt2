(declare-var inserted_blue Int)
(declare-var inserted_blue1 Int)
(declare-var bcount Int)
(declare-var bcount1 Int)
(declare-var color Int)
(declare-rel push (Int Int Int))
(declare-rel push1 (Int Int Int))
(declare-rel inv (Int Int))
(declare-rel fail ())
(define-fun is_valid ((x Int)) Bool (or (= x 1) (= x 0)))


(rule (=> (and (= inserted_blue 0) (= bcount 0)) (inv inserted_blue bcount)))
(rule (=> (and (inv inserted_blue bcount) (is_valid inserted_blue) (= color 0) (= inserted_blue 0) (push color bcount bcount1) (= inserted_blue1 1)) (inv inserted_blue1 bcount1)))
(rule (=> (and (inv inserted_blue bcount) (is_valid inserted_blue) (= color 1) (push1 color bcount bcount1)) (inv inserted_blue bcount1)))
(rule (=> (and (inv inserted_blue bcount) (is_valid inserted_blue) (not (or (and (= color 0) (= inserted_blue 0)) (= color 1)))) (inv inserted_blue bcount)))
(rule (=> (and (inv inserted_blue bcount) (is_valid inserted_blue) (not (=> (= inserted_blue 1) (= bcount 1)))) fail))

(query fail :print-certificate true)
