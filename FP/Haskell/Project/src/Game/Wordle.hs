module Game.Wordle where

import Game.Wordle.Normal (normalWordle)
import Game.Wordle.Easy (easyWordle)
import Game.Wordle.Hard (hardWordle)

wordle :: IO ()
wordle = do
    putStrLn ""
    putStrLn "    \ESC[48;2;100;100;120m\ESC[1m\ESC[37m ========================================= \ESC[0m"
    putStrLn "    \ESC[48;2;100;100;120m\ESC[1m\ESC[37m             WELCOME TO WORDLE!            \ESC[0m"
    putStrLn "    \ESC[48;2;100;100;120m\ESC[1m\ESC[37m ========================================= \ESC[0m"
    putStrLn ""
    putStrLn "    \ESC[48;2;60;100;55m\ESC[1m\ESC[37m  1. Normal Mode  \ESC[0m\ESC[1m - Standard Wordle"
    putStrLn "    \ESC[48;2;90;90;50m\ESC[1m\ESC[37m  2. Easy Mode    \ESC[0m\ESC[1m - Helpful warnings"
    putStrLn "    \ESC[48;2;120;40;40m\ESC[1m\ESC[37m  3. Expert Mode  \ESC[0m\ESC[1m - Computer may lie once!"
    putStrLn ""
    putStrLn "   \ESC[1m Select difficulty (1-3): "
    
    choice <- getLine
    putStrLn ""
    
    case choice of
        "1" -> normalWordle
        "2" -> easyWordle
        "3" -> hardWordle
        _   -> do
            putStrLn "    \ESC[48;2;120;40;40m\ESC[1m\ESC[37m Invalid choice! Please enter 1, 2, or 3. \ESC[0m"
            putStrLn ""
            wordle  