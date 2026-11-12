-- 1
groupBy :: Eq a1 => (a2 -> a1) -> [a2] -> [(a1, [a2])]
groupBy _ [] = []
groupBy key (x : xs) = (key x, [y | y <- x : xs, key y == key x]) : groupBy key (filter (\y -> key x /= key y) xs)

-- 2
conditionalWindows :: (a -> Bool) -> Int -> [a] -> [[a]]
conditionalWindows _ _ [] = []
conditionalWindows pred n lst
    | pred (head lst) && length lst >= n = take n lst : conditionalWindows pred n (tail lst)
    | otherwise     = conditionalWindows pred n (tail lst)

-- 3
nestedMap _ [] = []
nestedMap funcs (x : xs) = applyFuncs funcs x : nestedMap funcs xs
    where
        applyFuncs [] y = [y]
        applyFuncs (func : funcs) y = y : applyFuncs funcs (func y)

-- 4
streamWithMemory f current memory = current : streamWithMemory f (f current memory) current

-- 5
mergeLists :: [[Int]] -> [Int]
mergeLists = foldr merge []
    where 
        merge [] lst2 = lst2
        merge lst1 [] = lst1
        merge (x : xs) (y : ys)
            | x <= y    = x : merge xs (y : ys)
            | otherwise = y : merge (x : xs) ys

-- 6
selectiveTranspose pred matrix
    | 2 * sum (map (length . filter pred) matrix) >= sum (map length matrix) = transpose matrix
    | otherwise                                                              = matrix
    where 
        transpose [] = []
        transpose ([] : _) = []
        transpose matrix = map head matrix : transpose (map tail matrix)

-- 7
deepSum pred lst = sum (map (sum . filter pred) lst)

-- 8
rotateMatrix n matrix 
    | rotations == 0 = matrix
    | rotations == 1 = map reverse (transpose matrix)
    | rotations == 2 = reverse (map reverse matrix)
    | otherwise      = reverse (transpose matrix)
    where
        rotations = n `mod` 4
        transpose [] = []
        transpose ([] : _) = []
        transpose matrix = map head matrix : transpose (map tail matrix)

-- 9
pascalTriangle = iterate nextRow [1]
    where
        nextRow row = 1 : zipWith (+) row (tail row) ++ [1]
