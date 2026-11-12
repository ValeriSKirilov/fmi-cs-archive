module Game.Wordle.Hard (hardWordle) where

import System.Random (randomRIO)

import Game.Types
import Game.Wordle.WordleCore

import Utils.Display
import Utils.Words

generateFakeColors :: [(Char, Color)] -> [(a, Int)] -> [(Char, Color)] -> IO [(Char, Color)]
generateFakeColors _ _ [] = return []
generateFakeColors keyboard lettersCounted (currLetter : rest) = do
    if snd currLetter == Green  
        then do 
            restResult <- generateFakeColors keyboard lettersCounted rest
            return (currLetter : restResult)
    else do
        roll <- randomRIO (1, 100) :: IO Int
        let isKnownLetter = any (\(letter, color) -> letter == fst currLetter && (color == Yellow || color == Green)) keyboard
            newColor
                | isKnownLetter && roll <= 50 = Yellow
                | isKnownLetter               = Green
                | roll <= 30 = Yellow
                | roll <= 60 = Green
                | otherwise  = Gray
        restResult <- generateFakeColors keyboard lettersCounted rest
        return ((fst currLetter, newColor) : restResult)

runWordleHardGameLogic :: (Show t, Ord t, Num t) => [Char] -> t -> [(Char, Color)] -> [String] -> Bool -> IO ()
runWordleHardGameLogic secret attempts keyboard allWords hasLied = do
    guess <- getValidGuess allWords

    shouldLie <- if not hasLied 
                    then do
                        if attempts == 2 then
                            return True
                        else if attempts > 2 && attempts < 6 then do
                            roll <- randomRIO (1, 100) :: IO Int
                            return (roll <= 30)
                        else
                            return False
                    else
                       return False

    let markedGreensInGuess = checkGreen secret guess
        secretRemLet = remainingLetters secret markedGreensInGuess

    allLettersMarked <- if shouldLie
                            then generateFakeColors keyboard (getLettersCount secretRemLet) markedGreensInGuess 
                            else return (markOtherLetters (getLettersCount secretRemLet) markedGreensInGuess)

    let updatedKeyboard = checkKeyboard allLettersMarked keyboard

    if guess == secret
        then putStrLn "                              \ESC[48;2;60;100;55m\ESC[1m\ESC[37m *** CORRECT! *** \ESC[0m"
    else if attempts == 1
        then do
            putStr "                              "
            colorLetters allLettersMarked   

            putStrLn ""
            colorLetters updatedKeyboard  

            putStrLn ""
            putStr "                  "
            putStrLn $ "\ESC[48;2;120;40;40m\ESC[37m XXX YOU FAILED! THE WORD WAS " ++ secret ++ " XXX \ESC[0m"
    else do
            putStr "                              "
            colorLetters allLettersMarked   

            putStrLn ""
            colorLetters updatedKeyboard

            putStrLn ""
            putStr "                          "
            let attemptsLeft = attempts - 1
                attemptsColor 
                    | attemptsLeft > 3 = "60;100;55"
                    | attemptsLeft > 1 = "100;90;30"
                    | otherwise        = "120;40;40"
                in putStrLn $ "\ESC[48;2;" ++ attemptsColor ++ "m\ESC[37m Attempts remaining: " ++ show attemptsLeft ++ " \ESC[0m"

            runWordleHardGameLogic secret (attempts - 1) updatedKeyboard allWords (hasLied || shouldLie)

hardWordle :: IO ()
hardWordle = runGame runWordleHardGameLogic