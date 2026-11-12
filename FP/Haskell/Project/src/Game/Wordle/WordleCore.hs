module Game.Wordle.WordleCore (getLettersCount, checkKeyboard, checkGreen, markOtherLetters, remainingLetters, runGame) where

import Game.Types

import Utils.Display (initialKeyboardColor)
import Utils.Words (loadWords, getRandomWord)

getLettersCount :: Eq a => [a] -> [(a, Int)]
getLettersCount [] = []
getLettersCount (x : xs) = (x, countOccurrences x) : getLettersCount (filter (/= x) xs)
    where
        countOccurrences el = length (filter (== el) (x : xs))


checkKeyboard :: Eq t => [(t, Color)] -> [(t, Color)] -> [(t, Color)]
checkKeyboard guessResult = map updateKey
    where
        updateKey (key, oldColor) = (key, betterColor oldColor (findColor key guessResult) )

        findColor _ [] = Other
        findColor k ((letter, color) : rest)
            | k == letter = color
            | otherwise   = findColor k rest

        betterColor old new
            | new == Green                                 = Green
            | new == Yellow && old /= Green                = Yellow
            | new == Gray && old /= Green && old /= Yellow = Gray
            | otherwise                                    = old

checkGreen :: Eq b => [b] -> [b] -> [(b, Color)]
checkGreen = zipWith markPosition
    where
        markPosition s g
            | s == g    = (g, Green)
            | otherwise = (g, Other)

markOtherLetters :: (Ord a1, Eq a2, Num a1) => [(a2, a1)] -> [(a2, Color)] -> [(a2, Color)]
markOtherLetters _ [] = []
markOtherLetters lettersCounted (currLetter : rest)
    | snd currLetter == Green = currLetter : markOtherLetters lettersCounted rest 
    | hasLetterAvailable      = (fst currLetter, Yellow) : markOtherLetters updatedCounts rest
    | otherwise               = (fst currLetter, Gray) : markOtherLetters lettersCounted rest
    where        
        hasLetterAvailable = any (\(letter, count) -> letter == fst currLetter && count > 0) lettersCounted
        updatedCounts = map (\(letter,count) -> if letter == fst currLetter then (letter, count -1) else (letter, count)) lettersCounted

remainingLetters :: [a1] -> [(a2, Color)] -> [a1]
remainingLetters secret marked = [s | ((_, color), s) <- zip marked secret, color /= Green]

runGame :: Num t => (String -> t -> [(Char, Color)] -> [String] -> Bool -> IO b) -> IO b
runGame gameLogic = do
    word <- getRandomWord "wordlist.txt"
    -- let word = "FRUITY"
    allWords <- loadWords "wordlist.txt"
    -- allWords <- loadWords "wordlist2.txt"

    let keyboard = concat [x : "" | x <- ['A'..'Z']]
        initialKeyboard = [(key, Other) | key <- keyboard]
        
    initialKeyboardColor keyboard

    gameLogic word 6 initialKeyboard allWords False
