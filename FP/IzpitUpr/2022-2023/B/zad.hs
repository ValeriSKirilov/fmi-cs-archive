import Data.List(nub)
import Data.Maybe(mapMaybe)
import qualified Data.Set as Set

type PlayerName = String
type Prediction = Int
type Result = Int

-- 2
generateWords :: Int -> [String]
generateWords len = concat $ take (len + 1) levels
    where
        levels = [""] : map generateLevel levels
        generateLevel = concatMap (\w -> [w ++ "a", w ++ "b"])

type Transition q = (q, Char, q)
type Automaton q = ([q], q, [Transition q], [q])

detectedWords :: (Eq q, Ord q) => Automaton q -> Either String [String]
detectedWords (states, start, transitions, finals)
    | hasInfinitePath = Left "Infinite language"
    | otherwise = Right (nub allFiniteWords)
  where
    -- Get all states reachable from a given state
    reachableStates state = dfs [state] Set.empty
      where
        dfs [] visited = visited
        dfs (s:ss) visited
            | s `Set.member` visited = dfs ss visited
            | otherwise = 
                let visited' = Set.insert s visited
                    nextStates = [dest | (src, _, dest) <- transitions, src == s]
                in dfs (nextStates ++ ss) visited'
    
    -- Check if there's a cycle that can reach a final state
    hasInfinitePath = hasCycleToFinal start Set.empty
      where
        hasCycleToFinal currState visited
            | currState `Set.member` visited = 
                -- Found a cycle! Check if any final state is reachable from here
                any (`Set.member` reachableStates currState) finals
            | otherwise = 
                let visited' = Set.insert currState visited
                    nextStates = [dest | (src, _, dest) <- transitions, src == currState]
                in any (\next -> hasCycleToFinal next visited') nextStates
    
    -- Generate all finite words by BFS
    allFiniteWords = bfs [(start, "")]
      where
        bfs [] = []
        bfs ((state, word):queue)
            | length word > length states = bfs queue  -- Skip this path
            | state `elem` finals = 
                word : bfs (queue ++ nextConfigs)
            | otherwise = 
                bfs (queue ++ nextConfigs)
          where
            nextConfigs = [(dest, word ++ [c]) | (src, c, dest) <- transitions, src == state]

automaton1 :: Automaton Int
automaton1 = ([0, 1, 2, 3], 0, [(0,'a',1),(1,'b',1),(0,'a',2),(0,'b',2),(2,'a',3)], [0, 2, 3])

automaton2 :: Automaton Int
automaton2 = ([0, 1, 2, 3], 0, [(0,'a',1),(1,'b',1),(0,'a',2),(0,'b',2),(2,'a',3),(1,'a',2)], [0, 2, 3])

main :: IO ()
main = do
    print $ detectedWords automaton1
    print $ detectedWords automaton2

-- 3
applyFuncs :: [t -> t -> t] -> [t] -> t
applyFuncs funcs args = foldr (\(f, x) acc -> f x acc) (last neededArgs) pairs
    where
        neededArgs = take (length funcs + 1) args
        pairs = zip funcs neededArgs

findMax :: Ord a => [a -> a -> a] -> [a] -> a
findMax funcs args = maximum result
    where
        result = [applyFuncs funcs (drop i (cycle args)) | i <- [0..length funcs]]

-- 4
-- [("Dian",10,14),("Eslin",5,8),("Maria",11,10),("Martin",1,6)]
finalScores :: [(PlayerName, Prediction, Result)] -> [(PlayerName, Result)]
finalScores players = addBonuses
    where
        error (_, prediction, result) = abs (prediction - result)

        sortByError [] = []
        sortByError (x:xs) = sortByError lowerError ++ [x] ++ sortByError higherError
            where
                lowerError = [y | y <- xs, error y < error x]
                higherError = [y | y <- xs, error y > error x]

        sortedAsc = sortByError players
        sortedDes = reverse sortedAsc

        addBonuses = zipWith (\p1@(name1, _, res1) p2@(name2, pred2, res2) -> (name1, res1 + error p2)) sortedAsc sortedDes