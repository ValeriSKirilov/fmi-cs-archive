data List a = Empty
            | Cons a (List a)
            deriving (Show)

listConcat :: List a -> List a -> List a
listConcat l1 Empty = l1
listConcat Empty l2 = l2
listConcat (Cons x xs) l2 = Cons x (listConcat xs l2)

data Tree a = EmptyTree
            | Node a (Tree a) (Tree a) 
            deriving (Show, Eq)

t :: Tree Integer
t = Node 5 (Node 3 EmptyTree EmptyTree) (Node 7 EmptyTree EmptyTree) 

isEmpty :: Tree a -> Bool
isEmpty tree = t == EmptyTree

size :: Tree a -> Int
size EmptyTree = 0
size (Node root left right) = 1 + size left + size right

leafsList :: Tree a -> List a
leafsList EmptyTree = Empty
leafsList (Node root EmptyTree EmptyTree) = Cons root Empty
leafsList (Node root left right) = listConcat (leafsList left) (leafsList right)

instance Functor Tree where
    fmap :: (a -> b) -> Tree a -> Tree b
    fmap _ EmptyTree = EmptyTree
    fmap f (Node root left right) = Node (f root) (fmap f left) (fmap f right)

bstInsert :: Ord a => a -> Tree a -> Tree a
bstInsert num EmptyTree = Node num EmptyTree EmptyTree
bstInsert num (Node root left right)
    | num < root = Node root (bstInsert num left) right
    | otherwise  = Node root left (bstInsert num right)

bstSearch :: Ord a => a -> Tree a -> Bool
bstSearch num EmptyTree = False
bstSearch num (Node root left right)
    | num == root = True
    | num < root  = bstSearch num left
    | otherwise   = bstSearch num right

bstValues :: Tree a -> [a]
bstValues EmptyTree = []
bstValues (Node root left right) = bstValues left ++ [root] ++ bstValues right

-- isSymmetric :: Tree a -> Bool
-- isSymmetric EmptyTree = True
-- isSymmetric (Node root EmptyTree EmptyTree) = True
-- isSymmetric (Node root (Node leftValue left1 right1) (Node rightValue left2 right2)) 
--     | left1 == EmptyTree && right2 == EmptyTree = True

data Expr = Val Int
            | Add Expr  Expr
            | Mult Expr Expr
            | IfZero Expr Expr Expr
            deriving (Show)

eval :: Expr -> Int
eval (Val num) = num
eval (Add expr1 expr2) = eval expr1 + eval expr2
eval (Mult expr1 expr2) = eval expr1 * eval expr2
eval (IfZero cond expr1 expr2) 
    | eval cond == 0 = eval expr1
    | otherwise      = eval expr2


