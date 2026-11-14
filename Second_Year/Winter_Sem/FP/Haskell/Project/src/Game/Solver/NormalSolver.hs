module Game.Solver.NormalSolver (normalSolver) where

import Game.Solver.SolverCore

import Utils.SolverUtils

runSolverNormalGameLogic :: String -> [String] -> [String] -> Int -> [a0] -> IO ()
runSolverNormalGameLogic guess allWords possibleWords attemptNum [] = do
    putStrLn $ "Attempt " ++ show attemptNum ++ ": " ++ guess

    feedback <- getColorFeedback

    if allGreen feedback
        then putStrLn "I guessed it!"
        else do
            let newPossible = filterWords guess feedback possibleWords

            if null newPossible
                then putStrLn "Error: Your feedback contradicts previous answers!"
                else do
                    let nextGuess = pickBestGuess newPossible
                    runSolverNormalGameLogic nextGuess allWords newPossible (attemptNum + 1) []  

normalSolver :: IO ()
normalSolver = runSolver runSolverNormalGameLogic
