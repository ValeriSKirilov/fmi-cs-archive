module Game.Solver.SolverCore where

import Game.Types
import Game.Wordle.WordleCore (checkGreen, remainingLetters, markOtherLetters, getLettersCount)

import Data.List (maximumBy)
import Data.Ord (comparing)

import Utils.Words (loadWords)
import Utils.SolverUtils (getSecretWord)

allGreen :: [Color] -> Bool
allGreen = all (== Green)

simulateFeedback :: String -> String -> [Color]
simulateFeedback guess secret = map snd (markOtherLetters (getLettersCount secretRemLet) markedGreens)
    where 
        markedGreens = checkGreen secret guess
        secretRemLet = remainingLetters secret markedGreens

matchesFeedback :: String -> [Color] -> String -> Bool
matchesFeedback guess feedback word = simulateFeedback guess word == feedback

filterWords :: String -> [Color] -> [String] -> [String]
filterWords guess feedback = filter (matchesFeedback guess feedback)

countEliminations :: String -> String -> [String] -> Int
countEliminations guess secret possibleWords = length possibleWords - length remaining
    where
        feedback = simulateFeedback guess secret
        remaining = filterWords guess feedback possibleWords

scoreGuess :: [String] -> String -> Int
scoreGuess possibleWords guess = sum [countEliminations guess secret possibleWords | secret <- possibleWords]

pickBestGuess :: [String] -> String
pickBestGuess possibleWords = maximumBy (comparing (scoreGuess possibleWords)) possibleWords

runSolver :: (String -> [String] -> [String] -> Int -> [a0] -> IO ()) -> IO ()
runSolver gameLogic = do
    wordList <- loadWords "wordlist.txt"

    putStrLn "Enter the secret word: "
    secretWord <- getSecretWord

    -- let guess = pickBestGuess wordList

    gameLogic "AROSE" wordList wordList 1 []
    -- gameLogic guess wordList wordList 1 []