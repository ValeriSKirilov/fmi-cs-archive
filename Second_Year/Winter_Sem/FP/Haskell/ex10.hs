-- 2.1
squares = map (^2) [0..]

-- >>> take 5 squares  -- => [0, 1, 4, 9, 16]
-- [0,1,4,9,16]

-- 2.2
factorials = 1 : zipWith (*) factorials [1..]

-- >>> take 6 factorials
-- [1,1,2,6,24]

-- 2.3
sieve (x:xs) = x : filter (\y -> y `mod` x /= 0) xs
primes = sieve [2..]

-- >>> take 5 primes
-- [2,3,5,7,9]

-- 2.4
megerStreams xs [] = xs
mergeStreams [] ys = ys
mergeStreams (x:xs) (y:ys)
    | x > y = y : mergeStreams (x:xs) ys
    | y < x = x : mergeStreams xs (y:ys)
    | otherwise = x : mergeStreams xs ys

-- 3.1
productOfPositiveSquares = product . map (^2) . filter (>0)

-- >>> productOfPositiveSquares [-2, 3, -1, 4]  -- => 144 (3^2 * 4^2 = 9 * 16)
-- 144

-- 3.2
numLength 0 = 0
numLength n = 1 + numLength (n / 10)
sumOfCubesLength = show . map (numLength . (^3))

-- >>> sumOfCubesLength [1, 2, 3, 4]  -- => 6 (1^3=1 -> 1 цифра, 2^3=8 -> 1 цифра, 3^3=27 -> 2 цифри, 4^3=64 -> 2 цифри; общо: 1+1+2+2=6)
-- 100

-- 5.1
streamZipWith _ [] [] = []
streamZipWith f (x:xs) (y:ys) = f x y : streamZipWith f xs ys

-- 5.2
hamming = 1 : mergeStreams 
                    (mergeStreams 
                        (mergeStreams 
                            (mergeStreams 
                                (mergeStreams 
                                    (mergeStreams [2, 4..] [3, 6..])
                                [5, 10..]) 
                            [6, 12..]) 
                        [10, 20..]) 
                    [15, 30..]) 
                [30, 60..]

-- 5.3
streamInterleave [] [] = []
streamInterleave (x : xs) (y : ys) = x : y : streamInterleave xs ys

-- 5.4
streamProcess f x p n g = sum . map g $ take n $ filter p $ iterate f x

-- 5.5
collatz n
    | n == 1 = n : collatz 4
    | odd n = n : collatz (n * 3 + 1)
    | otherwise = n : collatz (n `div` 2)