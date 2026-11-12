-- 1
sumDivisors :: Integral a => a -> a
sumDivisors n = sum findDivisors
    where
        findDivisors = [x | x <- [1..(n - 1)], n `mod` x == 0]

-- 2
abundantData :: [(Integer, Integer)]
abundantData = map (\x -> (x, getDivs x - x)) correctNums
    where
        getDivs = sumDivisors
        
        correctNums = filter (\x -> getDivs x > x) (iterate (+1) 1)

-- 3
sumOddExcessAbundants :: [[Int]] -> [Integer]
sumOddExcessAbundants = changedMatrix
    where
        filterOddStream = filter (\(num, remain) -> odd remain) abundantData

        changedMatrix = map  $ sum . map (\num -> fst $ last $ take num filterOddStream)