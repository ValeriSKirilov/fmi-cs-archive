module Utils.Words (loadWords, pickRandomWord, getRandomWord, isValidWord, getValidGuess) where

import System.Random (randomRIO)
import Data.Char (toUpper)

loadWords :: FilePath -> IO [String]
loadWords path = do
    content <- readFile path
    return (lines content)

pickRandomWord :: [String] -> IO String
pickRandomWord wordsList = do
    index <- randomRIO (0, length wordsList - 1) :: IO Int
    return (wordsList !! index)

getRandomWord :: FilePath -> IO String
getRandomWord path = do
    importedWords <- loadWords path
    pickRandomWord importedWords

isValidWord :: String -> [String] -> Bool
isValidWord guess wordList = guess `elem` wordList

getValidGuess :: [String] -> IO String
getValidGuess wordList = do
    userGuess <- getLine
    let normalized = map toUpper userGuess
    if isValidWord normalized wordList
        then return normalized
        else do
            putStrLn "Not in word list! Try again."
            getValidGuess wordList