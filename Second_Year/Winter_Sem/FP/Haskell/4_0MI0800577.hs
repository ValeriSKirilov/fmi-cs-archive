-- 1
plusStarStream = helper 0
    where
        generateString 0 = ""
        generateString n = concatMap (const "*") [1..n] ++ concatMap (const "+") [1..n]
        helper n = generateString n : helper (n + 1)

-- 2
getPos :: (Ord a1, Num a1) => [[a2]] -> [a2 -> a1] -> [a1]
getPos [] [] = []
getPos matrix funcs = concatMap (filter (>0)) (applyAllFuncs matrix funcs)
    where
        applyFunc row func = map func row
        
        applyAllFuncs [] [] = []
        applyAllFuncs (m : ms) (f : fs) = applyFunc m f : applyAllFuncs ms fs

-- 3
data Nat
  = Zero
  | Succ Nat
  deriving (Eq, Ord, Show)

prevNat :: Nat -> Nat
prevNat (Succ num) = num

nextNat :: Nat -> Nat
nextNat = Succ 

sumNat :: Nat -> Nat -> Nat
sumNat num1 Zero = num1
sumNat num1 num2 = sumNat (nextNat num1) (prevNat num2)  

-- a)
divNat :: Nat -> Nat -> Nat
-- divNat _ Zero = -- exception

divNat (Succ Zero) (Succ Zero) = Succ Zero
divNat Zero num2 = Zero

divNat num1 (Succ Zero) = prevNat num1
divNat num1 num2 = divNat (prevNat num1) (prevNat num2) 

-- b)
divByNum :: (Eq t, Num t) => Nat -> t -> Nat
divByNum nat 1 = nat
divByNum nat num = divByNum (prevNat nat) (num - 1)

avgNat :: [Nat] -> Nat
avgNat nums = sumsNatElems nums
    where
        divisor = length nums
        dividedNums = map (`divByNum` divisor) nums

        sumNatElems [] = Zero
        sumsNatElems (nat : nats) = sumNat nat (sumNatElems nats)