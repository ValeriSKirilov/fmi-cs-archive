module Game.Wordle.Normal (normalWordle) where

import Game.Types
import Game.Wordle.WordleCore

import Utils.Display
import Utils.Words

runWordleNormalGameLogic :: (Show t, Ord t, Num t) => [Char] -> t -> [(Char, Color)] -> [String] -> Bool -> IO ()
runWordleNormalGameLogic secret attempts keyboard allWords _ = do
    guess <- getValidGuess allWords

    let markedGreensInGuess = checkGreen secret guess
        secretRemLet = remainingLetters secret markedGreensInGuess
        allLettersMarked = markOtherLetters (getLettersCount secretRemLet) markedGreensInGuess
        updatedKeyboard = checkKeyboard allLettersMarked keyboard

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

            runWordleNormalGameLogic secret (attempts - 1) updatedKeyboard allWords True

normalWordle :: IO ()
normalWordle = runGame runWordleNormalGameLogic