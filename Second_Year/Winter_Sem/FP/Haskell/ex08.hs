-- 1.1
clamp x minVal maxVal
    | x < minVal = minVal
    | x > maxVal = maxVal
    | otherwise = x

-- 2.1
swap (x, y) = (y, x)

-- 2.2
distance (x1, y1) (x2, y2) = sqrt ((x2 - x1)^2 + (y2 - y1)^2)

-- 2.3
midpoint (x1, y1) (x2, y2) = ((x1 + x2) / 2, (y1 + y2) / 2)

-- 2.4
triangleArea (x1, y1) (x2, y2) (x3, y3) = sqrt (p * (p - a) * (p - b) * (p - c))
    where a = distance (x1, y1) (x2, y2)
          b = distance (x2, y2) (x3, y3)
          c = distance (x1, y1) (x3, y3)
          p = (a + b + c) / 2

-- 3.1
mySum [] = 0
mySum (x : xs) = x + mySum xs

-- 3.2
myConcat [] = []
myConcat (x : xs) = x ++ myConcat xs

-- 3.3
myDrop _ [] = []
myDrop n xs@(_ : rest)
    | n <= 0 = xs 
    |otherwise = myDrop (n - 1) rest

-- 4.1
cubes = [x ^ 3 | x <- [1..10]]

-- 4.2
-- divisibleBy3And5 = [x | x <- [1..100], x `mod` 3 == 0, x `mod` 5 == 0]
divisibleBy3And5 = [15, 30..100]

-- 4.3
-- pythagoreanTriples n = [(a, b, c) | a <- [1..n], b <- [a..n], c <- [b..n], a^2 + b^2 == c^2] -- NO REPEATS
pythagoreanTriples n = [(a, b, c) | a <- [1..n], b <- [1..n], c <- [1..n], a ^ 2 + b ^ 2 == c ^ 2]

-- 4.4
factors n = [x | x <- [1..n], n `mod` x == 0]

-- 4.5
primesUpTo n = [x | x <- [1..n], isPrime x]
    where 
        isPrime k = length (factors k) == 2

-- 4.6
removeDuplicates [] = []
removeDuplicates (x : xs) = x : removeDuplicates [y | y <- xs, y /= x]

-- 5.1
listStats lst = (total, avg, minVal, maxVal)
    where
        total = mySum lst
        avg = fromIntegral (mySum lst) / fromIntegral (length lst)
        minVal = minimum lst
        maxVal = maximum lst

-- 5.2
perfectNumbers n = [x | x <- [1 .. n], isPerfect x]
    where 
        isPerfect k = sum (init (factors k)) == k

-- 5.3
zipWithIndex = zip [0 ..] 

-- 5.4
groupByValue [] = []
groupByValue xs = sortByCount [(x, count x xs) | x <- removeDuplicates xs]
    where 
        count val list = length [y | y <- list, y == val]
        sortByCount pairs = reverse (sortPairs pairs)
        sortPairs [] = []
        sortPairs (p : ps) =
            sortPairs [x | x <- ps, snd x < snd p]
            ++ [p]
            ++ sortPairs [x | x <- ps, snd x >= snd p]

-- 5.5
isDigit c = c >= '0' && c <= '9'

decode :: String -> String
decode [] = []
decode (c : cs)
    | isDigit c =
        let (numStr, rest) = takeDigits (c : cs)
            num = read numStr :: Int
            (letter : remaining) = rest
        in replicate num letter ++ decode remaining
    | otherwise = c : decode cs
    where 
        takeDigits :: String -> (String, String)
        takeDigits s = (digits, rest)
            where 
                digits = takeWhile isDigit s
                rest = dropWhile isDigit s
