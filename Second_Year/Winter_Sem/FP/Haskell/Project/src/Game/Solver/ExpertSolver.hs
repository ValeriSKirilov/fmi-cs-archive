module Game.Solver.ExpertSolver (expertSolver) where

import Game.Solver.SolverCore
import Game.Types

import Utils.SolverUtils

removeAt :: Int -> [a] -> [a]
removeAt i xs = take i xs ++ drop (i + 1) xs

applyFeedbacks :: [String] -> GuessHistory -> [String]
applyFeedbacks words [] = words
applyFeedbacks words ((guess, feedback) : rest) = applyFeedbacks (filterWords guess feedback words) rest

findLyingFeedback :: [String] -> GuessHistory -> (String, [Color]) -> Int -> Int
findLyingFeedback allWords history current index
    | index >= length allFeedbacks = -1
    | not (null filtered)          = index
    | otherwise                    = findLyingFeedback allWords history current (index + 1)
    where
        allFeedbacks = history ++ [current]
        filtered = applyFeedbacks allWords (removeAt index allFeedbacks)

runSolverExpertGameLogic :: String -> [String] -> [String] -> Int -> GuessHistory -> IO ()
runSolverExpertGameLogic guess allWords possibleWords attemptNum history = do
    putStrLn $ "Attempt " ++ show attemptNum ++ ": " ++ guess

    feedback <- getColorFeedback

    if allGreen feedback
        then putStrLn "I guessed it!"
        else do
            let newPossible = filterWords guess feedback possibleWords

            if null newPossible
                then do
                    let lieIndex = findLyingFeedback allWords history (guess, feedback) 0
                    
                    if lieIndex == -1
                        then putStrLn "Error: Cannot find the lie!"
                        else do
                            let allFeedbacks = history ++ [(guess, feedback)]
                                truthfulFeedbacks = removeAt lieIndex allFeedbacks
                                correctedPossible = applyFeedbacks allWords truthfulFeedbacks
                                nextGuess = pickBestGuess correctedPossible

                            runSolverExpertGameLogic nextGuess allWords correctedPossible (attemptNum + 1) truthfulFeedbacks
                else do
                    let newHistory = history ++ [(guess, feedback)]
                        nextGuess = pickBestGuess newPossible
                    runSolverExpertGameLogic nextGuess allWords newPossible (attemptNum + 1) newHistory    


expertSolver :: IO ()
expertSolver = runSolver runSolverExpertGameLogic