module Main where

import Game.Wordle
import Game.Solver

main :: IO ()
main = do
    -- Ето за тази част е използван AI, както е описано в документацията и обхваща до коментара с "$$$"
    putStrLn ""
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m  ============================================================= \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m                                                                \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m     ██╗  ██╗ █████╗ ███████╗██╗  ██╗███████╗██╗     ██╗        \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m     ██║  ██║██╔══██╗██╔════╝██║ ██╔╝██╔════╝██║     ██║        \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m     ███████║███████║███████╗█████╔╝ █████╗  ██║     ██║        \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m     ██╔══██║██╔══██║╚════██║██╔═██╗ ██╔══╝  ██║     ██║        \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m     ██║  ██║██║  ██║███████║██║  ██╗███████╗███████╗███████╗   \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m     ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝   \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m                                                                \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m                        >> WORDLE GAMES <<                      \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m                                                                \ESC[0m"
    putStrLn "                                                   \ESC[48;2;100;100;120m\ESC[1m\ESC[37m  ============================================================= \ESC[0m"
    putStrLn ""                                                 
    putStrLn "                                                             \ESC[48;2;60;100;55m\ESC[37m ========================================== \ESC[0m"
    putStrLn "                                                             \ESC[48;2;60;100;55m\ESC[37m │             1. PLAY WORDLE             │ \ESC[0m"
    putStrLn "                                                             \ESC[48;2;60;100;55m\ESC[37m │       Classic word guessing game       │ \ESC[0m"
    putStrLn "                                                             \ESC[48;2;60;100;55m\ESC[37m ========================================== \ESC[0m"
    putStrLn ""                                                        
    putStrLn "                                                             \ESC[48;2;70;80;120m\ESC[37m ========================================== \ESC[0m"
    putStrLn "                                                             \ESC[48;2;70;80;120m\ESC[37m │             2. SOLVER MODE             │ \ESC[0m"
    putStrLn "                                                             \ESC[48;2;70;80;120m\ESC[37m │         Watch AI solve Wordle          │ \ESC[0m"
    putStrLn "                                                             \ESC[48;2;70;80;120m\ESC[37m ========================================== \ESC[0m"
    putStrLn ""
    -- $$$

    putStrLn "    Select your choice (1-2): "
    
    choice <- getLine
    putStrLn ""
    
    case choice of
        "1" -> wordle
        "2" -> solver
        _   -> do
            putStrLn "    \ESC[48;2;120;40;40m\ESC[37m Invalid choice! Please enter 1 or 2. \ESC[0m"
            putStrLn ""
            main

    