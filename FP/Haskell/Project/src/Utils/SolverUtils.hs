module Utils.SolverUtils (getSecretWord, getColorFeedback) where

import Game.Types
import Utils.Words (loadWords, getValidGuess)
import Data.Char (toUpper)

getSecretWord :: IO String
getSecretWord = do
    wordList <- loadWords "wordlist.txt"
    getValidGuess wordList

charToColor :: Char -> Color
charToColor 'G' = Green
charToColor 'Y' = Yellow
charToColor 'X' = Gray
charToColor  _  = Gray

getColorFeedback :: IO [Color]
getColorFeedback = do
    putStrLn "Enter colors (G = Green, Y = Yellow, X = Gray): "
    input <- getLine
    let normalized = map toUpper input
    if length normalized == 5 && all (`elem` "GYX") normalized
        then return (map charToColor normalized)
        else do
            putStrLn "Invalid input! Use 5 characters: G, Y, or X"
            getColorFeedback
