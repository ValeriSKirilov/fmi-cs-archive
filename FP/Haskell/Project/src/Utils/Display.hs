module Utils.Display (colorize, colorLetters, initialKeyboardColor) where
    
import Game.Types

colorize :: Char -> Color -> [Char]
colorize letter code 
    | code == Green  = "\ESC[48;2;60;100;55m\ESC[37m " ++ [letter] ++ " \ESC[0m"   
    | code == Yellow = "\ESC[48;2;100;90;30m\ESC[37m " ++ [letter] ++ " \ESC[0m"  
    | code == Gray   = "\ESC[48;2;58;58;60m\ESC[37m " ++ [letter] ++ " \ESC[0m"
    | otherwise      = "\ESC[48;2;97;99;99m\ESC[37m " ++ [letter] ++ " \ESC[0m"

colorLetters :: [(Char, Color)] -> IO ()
colorLetters [] = putStrLn ""
colorLetters ((letter, color) : rest) = do
    putStr (colorize letter color)
    colorLetters rest

initialKeyboardColor :: [Char] -> IO ()
initialKeyboardColor [] = putStrLn ""
initialKeyboardColor (key : keys) = do
    putStr (colorize key Other)
    initialKeyboardColor keys