struct Item
  item_type : string
  id : string
  count : uint


struct TransactionAssetInfo
  item: Item
  price: string


struct Profile
  items : table<string; Item>
  transaction_history : table<int64; TransactionAssetInfo> // by transaction ids



// user profile global object
var
  persist : Profile
