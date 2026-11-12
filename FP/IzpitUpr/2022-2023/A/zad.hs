type PlayerName = String
type Prediction = Int
type Result = Int

-- 1


-- 2
generateWords :: Int -> [String]
generateWords len = concat $ take (len + 1) levels
    where
        levels = [""] : map generateLevel levels
        generateLevel = concatMap (\w -> [w ++ "a", w ++ "b"])

type Transition q = (q, Char, q)
type Automaton q = ([q], q, [Transition q], [q])

rejectedWord :: Eq q => Automaton q -> Maybe String
rejectedWord (states, start, transitions, finals) = helper allWords
    where
        allWords = generateWords (length states)

        helper [] = Nothing
        helper (w:ws)
            | not (isRecognized w) = Just w
            | otherwise = helper ws

        isRecognized = helper start
            where
                helper currState "" = currState `elem` finals
                helper currState (c:cs) =
                    let nextStates = [dest | (src, letter, dest) <- transitions, src == currState, letter == c]
                    in any (`helper` cs) nextStates

-- 3
applyFuncs :: [b -> b -> b] -> [b] -> b
applyFuncs funcs args = foldr (\(f, x) acc -> f x acc) (last args) pairs
    where
        neededFuncs = take (length args - 1) funcs
        pairs = zip neededFuncs args
        
findMin :: Ord a => [a -> a -> a] -> [a] -> a
findMin funcs args = minimum results
    where 
        m = length funcs
        results = [applyFuncs (drop i (cycle funcs)) args | i <- [0..m - 1]]

-- 4
finalScores :: [(PlayerName, Prediction, Result)] -> [(PlayerName, Result)]
finalScores players = addBonuses
    where
        error (name, prediction, result) = abs (prediction - result) 
        
        sortByError = foldr putInPlace [] 
            where
                putInPlace player [] = [player]
                putInPlace player (p : ps) 
                    | error player > error p = p : putInPlace player ps
                    | otherwise              = player : p : ps

        getName (name, _, _) = name
        getPrediction (_, prediction, _) = prediction
        getResult (_, _, result) = result 

        sortedByErrorAsc = sortByError players
        sortedByErrorDes = reverse $ sortByError players

        addBonuses = map (\idx -> (getName $ sortedByErrorAsc !! idx, getResult (sortedByErrorAsc !! idx) + error (sortedByErrorDes !! idx))) [0..length sortedByErrorAsc - 1]