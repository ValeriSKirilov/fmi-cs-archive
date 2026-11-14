module Game.Solver where

import Game.Solver.NormalSolver (normalSolver)
import Game.Solver.ExpertSolver (expertSolver)

solver :: IO ()
solver = do
    putStrLn ""
    putStrLn "    \ESC[48;2;100;100;120m\ESC[1m\ESC[37m ===================================== \ESC[0m"
    putStrLn "    \ESC[48;2;100;100;120m\ESC[1m\ESC[37m           WELCOME TO SOLVER!          \ESC[0m"
    putStrLn "    \ESC[48;2;100;100;120m\ESC[1m\ESC[37m ===================================== \ESC[0m"
    putStrLn ""
    putStrLn "    \ESC[48;2;60;100;55m\ESC[1m\ESC[37m  1. Normal Mode  \ESC[0m\ESC[1m  - Standard Solver"
    putStrLn "    \ESC[48;2;120;40;40m\ESC[1m\ESC[37m  2. Expert Mode  \ESC[0m\ESC[1m  - You may lie once!"
    putStrLn ""
    putStrLn   "   \ESC[1m Select difficulty (1-2): "
    
    choice <- getLine
    putStrLn ""
    
    case choice of
        "1" -> normalSolver
        "2" -> expertSolver
        _   -> do
            putStrLn "    \ESC[48;2;120;40;40m\ESC[37m Invalid choice! Please enter 1 or 2. \ESC[0m"
            putStrLn ""
            solver