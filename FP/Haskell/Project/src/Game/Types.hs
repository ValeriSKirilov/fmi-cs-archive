module Game.Types where
    
data Color = Gray | Yellow | Green | Other
    deriving (Show, Eq)

type HistoryEntry = (String, [Color])
type GuessHistory = [HistoryEntry]