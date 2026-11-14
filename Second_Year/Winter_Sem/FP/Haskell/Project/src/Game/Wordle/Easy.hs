module Game.Wordle.Easy (easyWordle) where

import Data.Char (toUpper)

import Game.Types
import Game.Wordle.WordleCore

import Utils.Display
import Utils.Words

checkForGrayLetters :: String -> [(Char, Color)] -> Bool
checkForGrayLetters guess keyboard = any (`elem` grayLetters) guess
    where
        grayLetters = [letter | (letter, color) <- keyboard, color == Gray]

checkForUnusedYellows :: String -> [(Char, Color)] -> Bool
checkForUnusedYellows guess keyboard = not (null yellowLetters) && any (`notElem` guess) yellowLetters
    where
        yellowLetters = [letter | (letter, color) <- keyboard, color == Yellow]

checkForGreensOutOfPosition :: String -> String -> [(Char, Color)] -> Bool
checkForGreensOutOfPosition guess secret keyboard = any (\idx -> isKnownGreen idx && guess !! idx /= secret !! idx) [0..4]
    where
        isKnownGreen idx = any (\(letter, color) -> letter == secretLetter && color == Green) keyboard
            where
                secretLetter = secret !! idx

warningsHandle :: [String] -> String -> [(Char, Color)] -> Int -> IO (String, Bool)
warningsHandle allWords guess keyboard warningMode = do
    let warning
            | warningMode == 1 = "Warning: This guess contains gray letters! Continue anyway? Y / N (Default: Y): "
            | warningMode == 2 = "Warning: This guess doesn't contain some of the known yellow letters! Continue anyway? Y / N (Default: Y): "
            | otherwise        = "Warning: This guess doesn't include all green letters at their known positions! Continue anyway? Y / N (Default: Y): "

    putStrLn warning
    response <- getLine

    let shouldContinue = null response || (map toUpper response == "Y" || map toUpper response == "YES")

    if shouldContinue
        then return (guess, True)
        else do 
            putStr "Enter new guess: "
            newGuess <- getLine
            let normalized = map toUpper newGuess

            if not (isValidWord normalized allWords)
                then do 
                    putStrLn "Not in word list! Try again."
                    warningsHandle allWords guess keyboard warningMode
                else return (normalized, False)

getFullyValidatedGuess :: [String] -> [(Char, Color)] -> String -> IO String
getFullyValidatedGuess allWords keyboard secret = do
    guess <- getValidGuess allWords
    validateAllChecks allWords guess keyboard secret

validateAllChecks :: [String] -> String -> [(Char, Color)] -> String -> IO String
validateAllChecks allWords guess keyboard secret = do
    if checkForGrayLetters guess keyboard
        then do
            (newGuess, shouldSkipValidation) <- warningsHandle allWords guess keyboard 1
            if shouldSkipValidation
                then checkForYellowsAndGreens allWords newGuess keyboard secret
                else validateAllChecks allWords newGuess keyboard secret
        else checkForYellowsAndGreens allWords guess keyboard secret

checkForYellowsAndGreens :: [String] -> String -> [(Char, Color)] -> String -> IO String
checkForYellowsAndGreens allWords guess keyboard secret = do
    if checkForUnusedYellows guess keyboard
        then do
            (newGuess, shouldSkip) <- warningsHandle allWords guess keyboard 2
            if shouldSkip
                then checkGreensPosition allWords newGuess keyboard secret
                else validateAllChecks allWords newGuess keyboard secret
        else checkGreensPosition allWords guess keyboard secret

checkGreensPosition :: [String] -> String -> [(Char, Color)] -> String -> IO String
checkGreensPosition allWords guess keyboard secret = do
    if checkForGreensOutOfPosition guess secret keyboard  
        then do
            (newGuess, shouldSkip) <- warningsHandle allWords guess keyboard 3
            if shouldSkip
                then return newGuess
                else validateAllChecks allWords newGuess keyboard secret
        else return guess
   
runWordleEasyGameLogic :: (Show t, Ord t, Num t) => String -> t -> [(Char, Color)] -> [String] -> Bool -> IO ()
runWordleEasyGameLogic secret attempts keyboard allWords _ = do
    guess <- getFullyValidatedGuess allWords keyboard secret

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

            runWordleEasyGameLogic secret (attempts - 1) updatedKeyboard allWords True

easyWordle :: IO ()
easyWordle = runGame runWordleEasyGameLogic