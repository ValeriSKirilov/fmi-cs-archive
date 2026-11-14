-- 1
segments :: Ord a => [a] -> [[a]]
segments [] = []
segments (x:xs) = seg : segments rest
    where
        (seg, rest) = buildSegments [x] xs

        buildSegments acc [] = (reverse acc, [])
        buildSegments acc (y:ys)
            | y < head acc = buildSegments (y:acc) ys
            | otherwise    = (reverse acc, y:ys)

fillGaps :: (Num a, Enum a) => [a] -> [a]
fillGaps [] = [0]
fillGaps (x:_) = [x, x - 1..0]

fillSegments :: [Int] -> [Int]
fillSegments (x:xs) = concatMap fillGaps (segments (x:xs)) 

-- 3
generateWords :: Int -> [String]
generateWords len = concat $ take (len + 1) levels
    where
        levels = [""] : map generateLevel levels
        generateLevel = concatMap (\w -> [w ++ [letter] | letter <- ['a'..'z']])

getAllWordsWithSize :: Int -> [String]
getAllWordsWithSize n = filter (\w -> length w == n) $ generateWords n

letterPairs :: Num t => ([Char], [Char]) -> t -> [(Char, Char, t)]
letterPairs ("", "") _ = []
letterPairs (letter:letters, symbol:symbols) idx
    | length (letter:letters) /= length (symbol:symbols) = error "Invalid pair!"
    | otherwise                      = (letter, symbol, idx) : letterPairs (letters, symbols) (idx + 1)

filterWords :: Eq a => [(a, Char, Int)] -> [[a]] -> [[a]]
filterWords [] wordlist = wordlist
filterWords ((letter, symbol, idx):rules) wordlist
    | symbol == '+' = filterWords rules (filter (\word -> (word !! idx) == letter) wordlist)
    | symbol == '?' = filterWords rules (filter (\word -> (word !! idx) /= letter && letter `elem` word) wordlist)
    | symbol == '-' = filterWords rules (filter (\word -> letter `notElem` word) wordlist)
    | otherwise     = error "Incorrect symbol!"

wordle :: [([Char], [Char])] -> String
wordle rules
    | null eligibleWords        = "no solutions"
    | length eligibleWords == 1 = head eligibleWords
    | otherwise                 = "many solutions" 
    where
        wordsize = length $ fst $ head rules
        wordlist = getAllWordsWithSize wordsize

        helper [] wList = wList
        helper (r:rs) wList = helper rs (filterWords (letterPairs r 0) wList)

        eligibleWords = helper rules wordlist

