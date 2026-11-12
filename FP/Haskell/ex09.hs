import System.IO
import Data.List
import Data.Ord (comparing)

-- 2.1
sumFormula :: [Int] -> Int
sumFormula = foldl (\x y -> if y > 0 then x + (y ^ 2 - 3 * y + 2) else x + 0) 0

-- 5.1
task51 :: IO ()
task51 = do 
    input <- getLine
    let nums = map read (words input) :: [Int]
    let sumVal = sum nums
    putStrLn("Sum: " ++ show sumVal)

-- 5.2
task52 :: IO()
task52 = do
    input <- getLine
    let numbers = read input :: [Int]
    let result = sumFormula numbers
    print result

task61 :: IO()
task61 = do
    content <- readFile "input.txt"
    let lineCount = length (lines content)
    putStrLn ("Number of lines: " ++ show lineCount)

task62 :: IO()
task62 = do
    content <- readFile "input.txt"
    let reversedLines = reverse (lines content)
    writeFile "output.txt" (unlines reversedLines)

-- 7.1
task71 :: IO()
task71 = do
    putStr "Enter filename: "
    name <- getLine
    content <- readFile (name ++ ".txt")

    let (nLine : firstLine: secondLine: _) = lines content
        n = read nLine :: Int
        first = read firstLine :: [Int]
        second = read secondLine :: [Int]
        result = zipWith (\f s -> f ^ n + s ^ n) first second
        outputFilename = name ++ "-output.txt"

    writeFile outputFilename (show result)

-- 7.2
mode xs = fst (maximumBy (\a b -> compare (snd a) (snd b)) counts)
    where counts = map (\x -> (x, length (filter (== x) xs))) (nub xs)

task72 :: IO()
task72 = do
    putStr "Enter filename: "
    name <- getLine
    content <- readFile (name ++ ".txt")

    let numbers = read content :: [Int]
        sumVal = sum  numbers
        avg = fromIntegral sumVal / fromIntegral (length numbers)
        modeVal = mode numbers
        minVal = minimum numbers
        maxVal = maximum numbers
        outputFilename = name ++ "-statistics.txt"
        output = 
            unlines 
                ["Sum: " ++ show sumVal,
                 "Average: " ++ show avg,
                 "Mode: " ++ show modeVal,
                 "Min: " ++ show minVal,
                 "Max: " ++ show maxVal
                ]
    writeFile outputFilename output

-- 7.3
task73 :: IO()
task73 = do
    putStr "Enter filename: "
    name <- getLine
    content <- readFile (name ++ ".txt")

    let text = words content
        filteredText = filter (\x -> length x >= 4) text
        outputFilename = name ++ "-filtered.txt"
        output = unwords filteredText

    writeFile outputFilename output

-- 7.4
isPrime n
    | n < 2 = False
    | n == 2 = True
    | even n = False
    | otherwise = null [x | x <- [3, 5..floor (sqrt (fromIntegral n))], n `mod` x == 0]

task74 :: IO()
task74 = do
    putStr "Enter input filename: "
    inputName <- getLine
    putStr "Enter output filename: "
    outputName <- getLine
    content <- readFile inputName
    let numbers = read content :: [Int]
        primes = filter isPrime numbers
        sortedPrimes = sort primes

    writeFile outputName (show sortedPrimes)

-- 7.5
countOccurrences words = map (\w -> (w, length (filter (== w) words))) (nub words)

sortByCount xs = reverse (sortOn snd xs)

task75 :: IO()
task75 = do
    putStr "Enter input filename: "
    inputName <- getLine
    putStr "Enter output filename: "
    outputName <- getLine
    content <- readFile inputName
    let wordsList = words content
        wordCounts = countOccurrences wordsList
        sortedCounts = sortByCount wordCounts
        output = unlines (map (\(w, c) -> w ++ ": " ++ show c) sortedCounts)

    writeFile outputName output

-- 7.6
subsequencesOfLength n xs
    |n > length xs = []
    | n <= 0 = [[]]
    | otherwise = [take n (drop i xs) | i <- [0..length xs - n]]
 
task76 :: IO()
task76 = do
    putStr "Enter input filename: "
    inputName <- getLine
    putStr "Enter n: "
    nStr <- getLine
    let n = read nStr :: Int
    content <- readFile inputName
    let numbers = read content :: [Int]
        allSubseqs = subsequencesOfLength n numbers
        sums = map sum allSubseqs
        maxSum = maximum sums
        maxSubseqs = filter (\sub -> sum sub == maxSum) allSubseqs
        outputFilename = inputName ++ "-output.txt"

    writeFile outputFilename (show maxSubseqs)
