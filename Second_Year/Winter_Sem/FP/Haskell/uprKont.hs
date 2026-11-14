--2021/2022
-- 1
isNPerm :: (Num a, Enum a, Eq a) => a -> (a -> a) -> Bool
isNPerm n f 
    | length resultList /= length uniqueResultList = False
    | length [0..(n - 1)] /= length uniqueResultList = False
    | otherwise = areEqual [0..(n - 1)] resultList
    where 
        resultList = map f [0..(n - 1)]
        uniqueResultList = removeDublicates resultList

        removeDublicates [] = []
        removeDublicates (x : xs) = x : removeDublicates [y | y <- xs, y /= x]

        areEqual lst1 lst2 = all (`elem` lst2) lst1

maxCycle n f = []

generateCycles :: (Num t, Enum t) => t -> (t -> a) -> [a]
generateCycles n f = [f x | x <- [0..(n - 1)]]
        
-- 2
movingAverage :: Fractional a => [a] -> Int -> [a]
movingAverage stream n = map (/ fromIntegral n) (createSumList stream n)
    where
        createSumList lst 1 = lst
        createSumList lst k = zipWith (+) lst (createSumList (tail lst) (k - 1))

allAverages :: Fractional a => [a] -> [[a]]
allAverages stream = [movingAverage stream n | n <- [2..]]

-- 3
inv :: [(String, [String])]
inv = [ ("docs", ["ids", "invoices"]), ("ids", ["passport"]),  ("invoices", []), ("memes", []),
        ("family", ["new year", "birthday"]), ("funny", ["memes"]), ("pics", ["family", "funny"]) ]
        
allObjects :: Eq a => [(a, [a])] -> [a]
allObjects inv = concatMap (filter (`isNotAFolder` inv) . snd) inv
    where 
        isNotAFolder el inv = el `notElem` map fst inv

cleanUp :: Eq a => [(a, [a])] -> [(a, [a])]
cleanUp inv 
    | hasEmptyFolders inv = cleanUp (removeEmptyFolders inv)
    | otherwise           = inv
    where 
        hasEmptyFolders = any (null . snd)
        removeEmptyFolders = foldr removeEmpty [] 
            where
                removeEmpty (x1, lst1) pair2
                    | null lst1 = pair2
                    | otherwise = (x1, filter (`notElem` emptyFolders inv) lst1) : pair2
                emptyFolders inv = map fst (filter (null . snd) inv)



















-- cleanUp inv = deleteEmpty noInitiallyEmptyFoldersInv
--     where
--         emptyFolders = filter (null . snd) inv
--         removeEmptyFolders = filter (`notElem` map fst emptyFolders)
--         cleanedUpInv = map (removeEmptyFolders . snd) inv
--         removeEmptyLists = filter (\x -> fst x `notElem` map fst emptyFolders)
--         noInitiallyEmptyFoldersInv = zip (map fst (removeEmptyLists inv)) cleanedUpInv
--         deleteEmpty lst = foldr (\x y -> if (snd x) == [] then y else x : y) [] lst


emptyFolders = filter (null . snd) inv

-- >>> emptyFolders
-- [("invoices",[]),("memes",[])]

test1 = foldr (\x y -> if fst x `elem` map fst emptyFolders then y else x : y) [] inv

-- test2 = foldr (\x y -> ) [] test1

-- >>> :t inv
-- inv :: [(String, [String])]

removeEmptyFolders = filter (`notElem` map fst emptyFolders)

cleanedUpInv = map (removeEmptyFolders . snd) inv

-- >>> cleanedUpInv
-- [["ids"],["passport"],[],[],["new year","birthday"],[],["family","funny"]]

noEmptyFoldersInv = filter (\x -> fst x `notElem` map fst emptyFolders) inv

-- >>> noEmptyFoldersInv
-- [("docs",["ids","invoices"]),("ids",["passport"]),("family",["new year","birthday"]),("funny",["memes"]),("pics",["family","funny"])]

removeEmptyLists = filter (\x -> fst x `notElem` map fst emptyFolders)

-- >>> removeEmptyLists inv
-- [("docs",["ids","invoices"]),("ids",["passport"]),("family",["new year","birthday"]),("funny",["memes"]),("pics",["family","funny"])]

noInitiallyEmptyFoldersInv = zip (map fst (removeEmptyLists inv)) cleanedUpInv

-- >>> noInitiallyEmptyFoldersInv
-- [("docs",["ids"]),("ids",["passport"]),("family",[]),("funny",[]),("pics",["new year","birthday"])]












--2024/2025
-- 1
outsideCircle :: (Num t, Enum t) => t -> t -> t -> [(t, t)]
outsideCircle x y r = generateCirclePoints (r + 1) ++ outsideCircle x y (r + 1)
    where 
        generateCirclePoints n = 
            zip [-n + x..n + x] ([0 + y..n + y] ++ reverse [0 + y..(n - 1) + y]) 
            ++ 
            zip (reverse [-(n - 1) + x..(n - 1) + x]) (reverse [-n + y..(-1) + y] ++ [-(n - 1) + y..(-1) + y])

-- 2   

-- 3
season :: [(String, [(String, Integer, Integer, Integer)])]
season = [("Silverstone", [("Leclerc", 15, 10, 82), ("Hamilton", 25, 5, 80),
                           ("Verstappen", 18, 0, 78)]),
          ("Monza", [("Sainz", 18, 0, 81), ("Hamilton", 15, 10, 79),
                     ("Verstappen", 25, 0, 77)]),
          ("Spa", [("Norris", 18, 5, 78), ("Russell", 25, 0, 76)]) ]

-- a)
allWinners :: Ord a1 => [(a2, [(b, a1, c, d)])] -> [b]
allWinners = map (head . getWinner . snd)
    where
        getPoints (_, points, _, _) = points
        getName (name, _, _, _) = name
        winnersPoints players = maximum (map getPoints players)
        getWinner players = map getName (filter (\x -> getPoints x == winnersPoints players) players)

-- b)
penaltyImpact :: (Foldable t, Num b1, Ord b1, Eq a1) => t (a2, [(a1, b2, b1, d)]) -> [(a1, b1)]
penaltyImpact season = map (\(name, points, penalty, time) -> (name, penalty)) (foldr putInPlace [] punishedPlayersList)
    where 
        getName (name, _, _, _) = name
        getPenalty (_, _, penalty, _) = penalty
        isNameInList player lst = getName player `elem` map getName lst
        allPlayers = concatMap snd season
        bringPlayerToFront playerName lst = filter (\x -> getName x == playerName) lst ++ filter (\x -> getName x /= playerName) lst
        sumPenalty (name, points, penalty1, time) (name2, points2, penalty2, time2) = (name, points, penalty1  + penalty2, time)
        punishedPlayersList = foldr (\player playersSoFar -> 
            if isNameInList player playersSoFar 
            then sumPenalty player (head (bringPlayerToFront (getName player) playersSoFar)) : tail (bringPlayerToFront (getName player) playersSoFar) 
            else 
                if getPenalty player > 0 
                    then player : playersSoFar 
                    else playersSoFar) [] allPlayers
        putInPlace player [] = [player]
        putInPlace player players
            | getPenalty player > getPenalty (head players) = player : players 
            | otherwise                                     = head players : putInPlace player (tail players)

-- c)
fastestLap :: (Foldable t, Ord a1) => t (a2, [(a3, b, c, a1)]) -> a3
fastestLap season = head (map getName (filter (\x -> getTime x == fastestLap) allPlayers))
    where 
        getName (name, _, _, _) = name
        getTime (_, _, _, time) = time
        allPlayers = concatMap snd season
        fastestLap = minimum (map getTime allPlayers)

-- d)
missedFastestLapWins :: (Ord b1, Ord d, Eq a) => [(b2, [(a, b1, c, d)])] -> [b2]
missedFastestLapWins season = map fst (filter (nameCheck . snd) winnersAndBestTimeFiltered)
    where 
        getName (name, _, _, _) = name
        getPoints (_, points, _, _) = points
        getTime (_, _, _,time) = time
        getWinner players = filter (\x -> getPoints x == maximum (map getPoints players)) players
        getBestTime players = filter (\x -> getTime x == minimum (map getTime players)) players
        winnersAndBestTimeFiltered = map (\x -> (fst x, getWinner (snd x) ++ getBestTime (snd x))) season
        nameCheck players = all (\x -> getName x /= getName (head players)) (tail players)

-- УПРАЖЕНИЕ 11
-- 1
groupBy :: Eq a1 => (a2 -> a1) -> [a2] -> [(a1, [a2])]
groupBy key [] = []
groupBy key (x : xs) = (key x, [y | y <- x : xs, key y == key x]) : groupBy key [y | y <- xs, key y /= key x]

-- 2
conditionalWindows :: (a -> Bool) -> Int -> [a] -> [[a]]
conditionalWindows _ _ [] = []
conditionalWindows pred n (x : xs)
    | pred x && length (x : xs) >= n   = take n (x : xs) : conditionalWindows pred n xs
    | otherwise = conditionalWindows pred n xs

-- 3
nestedMap :: [t -> t] -> [t] -> [[t]]
nestedMap _ [] = []
nestedMap funcs (x : xs) = applyFuncs funcs x : nestedMap funcs xs
    where
        applyFuncs [] x = [x]
        applyFuncs (func : funcs) x = x : applyFuncs funcs (func x)

-- 4
streamWithMemory :: Num t1 => t2 -> t1 -> t1 -> [t1]
streamWithMemory f current memory = current : streamWithMemory f (current + memory) current

-- 5
mergeLists :: [[Int]] -> [Int]
mergeLists = foldr merge []
    where 
        merge lst [] = lst
        merge [] lst = lst
        merge (x : xs) (y : ys)
            | x < y = x : merge xs (y : ys)
            | otherwise = y : merge (x : xs) ys

-- 6
selectiveTranspose :: (a -> Bool) -> [[a]] -> [[a]]
selectiveTranspose pred matrix 
    | 2 * sum (map (length . filter pred) matrix) >= sum (map length matrix) = transpose matrix
    | otherwise = matrix
    where
        transpose [] = []
        transpose ([] : _) = []
        transpose matrix = map head matrix : transpose (map tail matrix)

-- 7
deepSum :: Num a => (a -> Bool) -> [[a]] -> a
deepSum pred dl = sum (map (sum . filter pred) dl)

-- 8
rotateMatrix :: Integral a1 => a1 -> [[a2]] -> [[a2]]
rotateMatrix n matrix
    | rotations == 0  = matrix
    | rotations == 1 = map reverse (transpose matrix)
    | rotations == 2 = reverse (map reverse matrix)
    | otherwise      = reverse (transpose matrix)
    where  
        rotations = n `mod` 4
        transpose [] = []
        transpose ([] : _) = []
        transpose matrix = map head matrix : transpose (map tail matrix)

-- 9
pascalTriangle :: [[Integer]]
pascalTriangle = iterate nextRow [1]
    where
        nextRow row = 1 : zipWith (+) row (tail row) ++ [1]

-- CLAUDE TASKS
-- 1
spiralCoords :: (Num b, Enum b) => b -> [(b, b)]
spiralCoords n = (0, 0) : spiralHelper (n, 0) n
    where
        spiralHelper (x, y) step =
            let 
                upMoves = [(x, y + i) | i <- [1..step]]
                leftMoves = [(x - i, y + step) | i <- [1..step]]
                downMoves = [(x - step, y + step - i) | i <- [1..step + 1]]
                rightMoves = [(x - step + i, y - 1) | i <- [1..step + 1]]
                nextStart = (x + 1, y - 1)
            in
                upMoves ++ leftMoves ++ downMoves ++ rightMoves ++ spiralHelper nextStart (step + 1)

-- 2
layeredTransform :: [(a -> Bool, a -> a)] -> [a] -> [a]
layeredTransform _ [] = []
layeredTransform (pair : pairs) (x : xs) = applyTransformation (pair : pairs) x : layeredTransform (pair : pairs) xs
    where
        applyTransformation [] x = x
        applyTransformation (pair : pairs) x
            | fst pair x = snd pair x
            | otherwise  = applyTransformation pairs x

-- 3
tournament :: [(Integer, [(String, String, String)])]
tournament = [
  (1, [("Alice", "Bob", "1-0"), ("Carol", "Dave", "1/2-1/2")]),
  (2, [("Alice", "Carol", "1-0"), ("Bob", "Dave", "0-1"), ("Alice", "Dave", "1-0")]),
  (3, [("Bob", "Carol", "1/2-1/2"), ("Dave", "Alice", "0-1")])
  ]

dominantPlayer :: (Foldable t, Eq b) => t (a, [(b, b, String)]) -> [b]
dominantPlayer tournament = map (\(name, wins, loses) -> name) (filter (\(name, wins, loses) -> wins * 100 >= 70 * (wins + loses)) matchedPlayerToWins)
    where 
        getPlayers = foldr (\(num, players) lst -> players ++ lst) [] 
        playerNames = foldr (\(player1, player2, score) lst -> if player1 `notElem` lst then (if player2 `notElem` lst then player1 : player2 : lst else player1 : lst) else if player2 `notElem` lst then player2 : lst else lst) [] (getPlayers tournament)
        matchedPlayerToWins = map (\x -> (x , countWins x, countLoses x)) playerNames
        countWins playerName = foldr (\(player1, player2, score) wins -> (if (player1 == playerName && score == "1-0") || (player2 == playerName && score == "0-1") then wins + 1 else wins)) 0 (getPlayers tournament)
        countLoses playerName = foldr (\(player1, player2, score) loses -> (if (player1 == playerName && score == "0-1") || (player2 == playerName && score == "1-0") then loses + 1 else loses)) 0 (getPlayers tournament)

headToHead :: (Num a1, Num c, Num b1, Eq b2) => b2 -> b2 -> [(a2, [(b2, b2, String)])] -> (a1, b1, c)
headToHead player1 player2 tournament = foldr (\(_, _, score) (p1wins, draws, p2wins) -> if score == "1-0" then (p1wins + 1, draws, p2wins) else if score == "0-1" then (p1wins, draws, p2wins + 1) else (p1wins, draws + 1, p2wins)) (0, 0, 0) formatedPlayerMatches
    where
        getPlayer1 (player1, _, _) = player1
        getPlayer2 (_, player2, _) = player2
        thosePlayerMatches = map (\(num, games) -> (num, filter (\x -> (getPlayer1 x == player1 && getPlayer2 x == player2) || (getPlayer1 x == player2 && getPlayer2 x == player1)) games)) tournament
        formatedPlayerMatches = map (\(p1, p2, score) -> if p1 == player1 then (p1, p2, score) else (p2, p1, swapScore score)) (foldr ((\ x y -> if x /= [] then x ++ y else y) . snd) [] thosePlayerMatches)
        swapScore score
            | score == "1-0" = "0-1"
            | score == "0-1" = "1-0"
            | otherwise      = "1/2-1/2"

mostActiveRound :: Foldable t => [(a1, t a2)] -> a1
mostActiveRound tournament = (fst . head) (filter (\x -> snd x == maxLength) (getRoundActivity tournament))
    where
        getRoundActivity = map (\(num, games) -> (num, length games))
        maxLength = maximum (map snd (getRoundActivity tournament))

-- 4
collatzStats :: Integral t => t -> [(t, Int, t)]
collatzStats n = (n, length currCollatz - 1, maximum currCollatz) : collatzStats (n + 1)
    where
        collatz n 
            | n == 1    = [n]
            | otherwise = n : collatz (nextCollatz n)
            where
                nextCollatz x
                    | even x    = x `div` 2
                    | otherwise = 3 * x + 1
        currCollatz = collatz n

-- 5
conditionalZip :: (a -> a -> Bool) -> [a] -> [a] -> [a]
conditionalZip _ [] _ = []
conditionalZip _ _ [] = []
conditionalZip pred (x : xs) (y : ys)
    | pred x y  = x : y : conditionalZip pred xs ys
    | otherwise = conditionalZip pred xs ys